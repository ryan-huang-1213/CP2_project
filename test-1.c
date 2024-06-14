#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>

#include "uchow.h"

/*
typedef struct {
    char* path; // 圖片的相對路徑
    SDL_Texture* texture;
    SDL_Rect rect;
} Asset;

typedef struct {
    char* text; // 選項的文字敘述 
    SDL_Texture* texture;
    SDL_Rect rect;
    int nextSceneIndex; // 跳轉至下一個頁面 Scene[?]
} Option;

typedef struct {
    char* text;
    SDL_Texture* texture;
    SDL_Rect rect;
    int at_background; // 用來核對台詞應該出現在哪一個背景
    Asset* characters; // 指向需要出現的角色
    int totalCharacters; // 出現角色的總數
    Option* options; // 指向該對話應該顯示的選項
    int totalOptions; // 該對話應該顯示的選項數量
    char* speaker;
} Dialog;

typedef struct {
    Asset* backgrounds;
    int totalBackgrounds;
    Asset* characters;
    int totalCharacters;
    Dialog* dialogs;
    int totalDialogs;
    int currentDialogIndex;
    int currentBackgroundIndex;
    Option* options; // 新增選項
    int totalOptions; // 選項的總數
    int nextSceneIndex;
} Scene;

// 新增一個結構來表示背包
typedef struct {
    bool visible; // 背包是否可見
    SDL_Rect rect; // 背包的矩形區域
    // 可以添加更多背包相關的屬性，如道具列表等
} Backpack;
*/

// 函式宣告
bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Scene scenes[], TTF_Font* font, int totalScenes);
bool loadMedia(SDL_Renderer* renderer, Scene scenes[], TTF_Font** font);
void render(SDL_Renderer* renderer, Scene scenes[], int currentSceneIndex);
void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes);
void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;

    // 後端修改區 開始 

    Scene scenes[1];

    int32_t now_scene=0;
    story(now_scene,&scenes);
    Dialog dialogs[10];
    scenes[0].dialogs[0].text = "Start the game!\n";
    scenes[0].dialogs[0].texture = NULL;
    scenes[0].dialogs[0].rect.x = 0;
    scenes[0].dialogs[0].rect.y = WINDOW_HEIGHT - DIALOG_HEIGHT;
    scenes[0].dialogs[0].rect.w = WINDOW_WIDTH;
    scenes[0].dialogs[0].rect.h = DIALOG_HEIGHT;
    scenes[0].dialogs[0].at_background = 0;
    scenes[0].dialogs[0].characters = NULL;
    scenes[0].dialogs[0].totalCharacters = 0;
    scenes[0].dialogs[0].speaker = caption;

    // 初始化選項
    Option options[] = {
        {"去微軟工作", NULL, {WINDOW_WIDTH - 300, 100, 200, 50},2},
        {"掃操場", NULL, {WINDOW_WIDTH - 300, 200, 200, 50},1}
    };
    int totalOptions = sizeof(options) / sizeof(options[0]);

    // 將選項與對話關聯
    scenes[0].dialogs[2].options = options;
    scenes[0].dialogs[2].totalOptions = totalOptions;

    // 後端修改區 結束

    int currentSceneIndex = 0;

    if (!initSDL(&window, &renderer)) {
        printf("Failed to initialize!\n");
    } else {
        if (!loadMedia(renderer, scenes, &font)) {
            printf("Failed to load media!\n");
        } else {
            bool running = true;
            SDL_Event e;
            while (running) {
                handleEvents(&e, &running, &currentSceneIndex, scenes, MAX_SCENES);
                render(renderer, scenes, currentSceneIndex);
            }
        }
    }

    closeSDL(window, renderer, scenes, font, MAX_SCENES);
    return 0;
}

// 初始化 SDL
bool initSDL(SDL_Window** window, SDL_Renderer** renderer) {
    // 嘗試初始化 SDL 視頻子系統
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    // 創建視窗
    *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    // 為視窗創建渲染器
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }
    // 設置渲染器繪製顏色（用於按鈕背景）
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 204); // 80% 透明度的黑色
    return true;
}

// 關閉 SDL 並釋放資源
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Scene scenes[], TTF_Font* font, int totalScenes) {
    // 遍歷所有場景並銷毀資源
    for (int i = 0; i < totalScenes; i++) {
        Scene scene = scenes[i];
        // 銷毀背景紋理
        for (int j = 0; j < scene.totalBackgrounds; j++) {
            SDL_DestroyTexture(scene.backgrounds[j].texture);
        }
        // 銷毀角色紋理
        for (int j = 0; j < scene.totalCharacters; j++) {
            SDL_DestroyTexture(scene.characters[j].texture);
        }
        // 銷毀對話紋理
        for (int j = 0; j < scene.totalDialogs; j++) {
            SDL_DestroyTexture(scene.dialogs[j].texture);
        }
    }
    // 關閉並銷毀字體
    TTF_CloseFont(font);
    // 銷毀渲染器和視窗
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // 退出 SDL 子系統
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// 加載媒體，包括圖片和對話紋理
bool loadMedia(SDL_Renderer* renderer, Scene scenes[], TTF_Font** font) {
    // 初始化 PNG 加載
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    // 初始化 TTF
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    // 打開字體
    *font = TTF_OpenFont(TEXT_PATH, 24); // 字體大小設為 24px
    if (*font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    // 加載每個場景的媒體
    for (int i = 0; i < MAX_SCENES; i++) {
        Scene* scene = &scenes[i];
        // 加載背景圖片
        for (int j = 0; j < scene->totalBackgrounds; j++) {
            SDL_Surface* loadedSurface = IMG_Load(scene->backgrounds[j].path);
            if (loadedSurface == NULL) {
                printf("Unable to load image %s! SDL_image Error: %s\n", scene->backgrounds[j].path, IMG_GetError());
                return false;
            }
            scene->backgrounds[j].texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            SDL_FreeSurface(loadedSurface);
        }
        // 加載角色圖片
        for (int j = 0; j < scene->totalCharacters; j++) {
            SDL_Surface* loadedSurface = IMG_Load(scene->characters[j].path);
            if (loadedSurface == NULL) {
                printf("Unable to load image %s! SDL_image Error: %s\n", scene->characters[j].path, IMG_GetError());
                return false;
            }
            scene->characters[j].texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            SDL_FreeSurface(loadedSurface);
        }
        // 為對話創建紋理
        for (int j = 0; j < scene->totalDialogs; j++) {
            updateDialogTexture(renderer, *font, &scene->dialogs[j]);
        }
    }
    return true;
}

void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog) {
    SDL_Color textColor = {255, 255, 255}; // 文本顏色為白色

    // 釋放先前的紋理（如果存在）
    if (dialog->texture != NULL) {
        SDL_DestroyTexture(dialog->texture);
        dialog->texture = NULL;
    }

    // 設置換行的寬度為按鈕寬度的 80%
    int wrapLength = (int)(WINDOW_WIDTH * 0.8);

    // 使用 TTF_RenderUTF8_Blended_Wrapped 函數來自動換行
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, dialog->text, textColor, wrapLength);
    if (textSurface == NULL) {
        printf("Unable to create text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    dialog->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (dialog->texture == NULL) {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // 計算每行文字的寬度並置中
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface); // 釋放文本表面

    // 設置對話紋理的位置和大小，使其在按鈕內水平和垂直置中
    dialog->rect.x = (WINDOW_WIDTH - textWidth) / 2; // 水平置中
    dialog->rect.y = WINDOW_HEIGHT - DIALOG_HEIGHT / 2 - textHeight / 2; // 垂直置中
    dialog->rect.w = textWidth; // 設置文本寬度
    dialog->rect.h = textHeight; // 設置文本高度
}

void render(SDL_Renderer* renderer, Scene scenes[], int currentSceneIndex) {
    SDL_RenderClear(renderer);
    Scene* currentScene = &scenes[currentSceneIndex];

    // 渲染當前背景
    SDL_RenderCopy(renderer, currentScene->backgrounds[currentScene->currentBackgroundIndex].texture, NULL, NULL);
    
    // 渲染對話中指定的角色
    Dialog* currentDialog = &currentScene->dialogs[currentScene->currentDialogIndex];
    for (int i = 0; i < currentDialog->totalCharacters; i++) {
        SDL_RenderCopy(renderer, currentDialog->characters[i].texture, NULL, &currentDialog->characters[i].rect);
    }

    // 計算並渲染選項
    int optionY = WINDOW_HEIGHT - DIALOG_HEIGHT - 70; // 從對話框上方 70px 開始
    for (int i = 0; i < currentDialog->totalOptions; i++) {
        // 設置選項的位置
        currentDialog->options[i].rect.x = WINDOW_WIDTH - currentDialog->options[i].rect.w - 20; // 靠右對齊，留出邊距
        currentDialog->options[i].rect.y = optionY - (currentDialog->options[i].rect.h + 10) * (currentDialog->totalOptions - 1 - i); // 每個選項上移並相距 10px

        // 渲染選項背景
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色背景
        SDL_RenderFillRect(renderer, &currentDialog->options[i].rect);

        // 渲染選項文字
        SDL_RenderCopy(renderer, currentDialog->options[i].texture, NULL, &currentDialog->options[i].rect);
    }

    // 渲染對話按鈕背景
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 204); // 設置為半透明黑色
    SDL_Rect dialogRect = {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT};
    SDL_RenderFillRect(renderer, &dialogRect);

    // 渲染對話文本
    SDL_RenderCopy(renderer, currentDialog->texture, NULL, &currentDialog->rect);
    
    SDL_RenderPresent(renderer);
}

void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes) {
    while (SDL_PollEvent(e) != 0) {
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            Scene* currentScene = &scenes[*currentSceneIndex];
            Dialog* currentDialog = &currentScene->dialogs[currentScene->currentDialogIndex];

            // 檢查是否點擊了選項
            for (int i = 0; i < currentDialog->totalOptions; i++) {
                SDL_Rect optionRect = currentDialog->options[i].rect;
                if (x >= optionRect.x && x <= (optionRect.x + optionRect.w) &&
                    y >= optionRect.y && y <= (optionRect.y + optionRect.h)) {
                    // 僅更新下一個場景索引，不立即切換場景
                    currentScene->nextSceneIndex = currentDialog->options[i].nextSceneIndex;
                    currentScene->currentDialogIndex++;
                    
                    // 如果對話所屬的背景和當前的不同，則需要切換背景
                    if (currentDialog->at_background != currentScene->currentBackgroundIndex) {
                        currentScene->currentBackgroundIndex = currentDialog->at_background;
                    }
                    // 如果是場景中的最後一句對話，則切換到下一個場景
                    if (currentScene->currentDialogIndex >= currentScene->totalDialogs) {
                        *currentSceneIndex = currentScene->nextSceneIndex;
                        currentScene = &scenes[*currentSceneIndex];
                        currentScene->currentDialogIndex = 0;
                        currentScene->currentBackgroundIndex = currentScene->dialogs[0].at_background;
                    }
                }
            }
            if(currentDialog->totalOptions == 0){
                // 檢查是否點擊了對話區域並更新對話索引
                if (y > WINDOW_HEIGHT - DIALOG_HEIGHT) {
                    currentScene->currentDialogIndex++;
                    // 如果對話所屬的背景和當前的不同，則需要切換背景
                    if (currentDialog->at_background != currentScene->currentBackgroundIndex) {
                        currentScene->currentBackgroundIndex = currentDialog->at_background;
                    }
                    // 如果是場景中的最後一句對話，則切換到下一個場景
                    if (currentScene->currentDialogIndex >= currentScene->totalDialogs) {
                        *currentSceneIndex = currentScene->nextSceneIndex;
                        currentScene = &scenes[*currentSceneIndex];
                        currentScene->currentDialogIndex = 0;
                        currentScene->currentBackgroundIndex = currentScene->dialogs[0].at_background;
                    }
                }
            }
        }
        /*
        if(e->type == SDL_KEYDOWN){
            switch (e->key.keysym.sym) {
                case SDLK_b:
                    backpack->visible = !backpack->visible; // 切換背包的可見狀態
                    break;
            }
        }
        */
    }
}

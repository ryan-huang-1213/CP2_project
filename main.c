#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DIALOG_HEIGHT (WINDOW_HEIGHT / 3) // 對話按鈕的高度
#define TEXT_PATH "source/text/cjkfonts_handingwriting4.ttf"
#define MAX_SCENES 3
#define MAX_DIALOGS 4

typedef struct {
    char* path;
    SDL_Texture* texture;
    SDL_Rect rect;
} Asset;

typedef struct {
    char* text;
    SDL_Texture* texture;
    SDL_Rect rect;
    int at_background; // 用來核對台詞應該出現在哪一個背景
    Asset* characters; // 指向需要出現的角色
    int totalCharacters; // 出現角色的總數
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
} Scene;

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
    Scene scenes[MAX_SCENES] = {
        {
            (Asset[2]){{"source/image/maldives.bmp", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}},
                        {"source/image/tmp_background.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}}},
            2,
            (Asset[1]){{"source/character/LG.png", NULL, {0, 0, 0, 0}}}, // 新增角色
            1, // 總角色數量更新為 s
            (Dialog[4]){{"this is maldive, world's best place ", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0},
                         {"this place is famous of its beach and sunsine", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0}/*
                         {"But Lg didn't have enought money.", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 1},
                         {"So, he decided to get some quick money to achieve his dream.", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 1}*/},
            4,
            0,
            0
        },
        // ... Other scenes
    };

    // 初始化對話和角色
    scenes[0].dialogs[2] = (Dialog){
        "But Lg didn't have enought money.",
        NULL,
        {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT},
        1,
        scenes[0].characters, // 指向第一個場景的角色
        1 // 第三句話時有一個角色出現
    };
    scenes[0].dialogs[3] = (Dialog){
        "So, he decided to get some quick money to achieve his dream.",
        NULL,
        {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT},
        1,
        scenes[0].characters, // 指向第一個場景的角色
        1 // 第四句話時有一個角色出現
    };

    // 設置角色的位置
    scenes[0].characters[0].rect.x = 0; // 角色出現在視窗的左側
    scenes[0].characters[0].rect.y = 0; // 根據需要調整 y 值
    scenes[0].characters[0].rect.w = 250;
    scenes[0].characters[0].rect.h = 450;

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

// ... 其他函式的實現 (initSDL, closeSDL, loadMedia, render, handleEvents, updateDialogTexture)

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
/*
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
*/

// 未測試
void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog) {
    SDL_Color textColor = {255, 255, 255}; // 文本顏色為白色
    SDL_Color backgroundColor = {0, 0, 0, 204}; // 背景顏色為 80% 透明度的黑色

    // 釋放先前的紋理（如果存在）
    if (dialog->texture != NULL) {
        SDL_DestroyTexture(dialog->texture);
        dialog->texture = NULL;
    }

    // 使用 TTF_RenderText_Blended_Wrapped 函數來自動換行
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, dialog->text, textColor, WINDOW_WIDTH);
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

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface); // 釋放文本表面

    // 設置對話紋理的位置和大小，使其在按鈕內水平和垂直置中
    dialog->rect.x = (WINDOW_WIDTH - textWidth) / 2; // 水平置中
    dialog->rect.y = WINDOW_HEIGHT - DIALOG_HEIGHT / 2 - textHeight / 2; // 垂直置中
    dialog->rect.w = textWidth; // 設置文本寬度
    dialog->rect.h = textHeight; // 設置文本高度
}

// 使用 80% 透明度的黑色背景更新對話按鈕的紋理
void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog) {
    SDL_Color textColor = {255, 255, 255}; // 文本顏色為白色
    SDL_Color backgroundColor = {0, 0, 0, 204}; // 背景顏色為 80% 透明度的黑色
    SDL_Surface* textSurface = TTF_RenderText_Shaded(font, dialog->text, textColor, backgroundColor);
    dialog->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    // 使用 TTF_RenderText_Blended_Wrapped 函數來自動換行
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, dialog->text, textColor, WINDOW_WIDTH);
    dialog->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    // 設置對話紋理的位置和大小，使其在按鈕內水平和垂直置中
    dialog->rect.x = (WINDOW_WIDTH - textWidth) / 2; // 水平置中
    dialog->rect.y = WINDOW_HEIGHT - DIALOG_HEIGHT / 2 - textHeight / 2; // 垂直置中
    dialog->rect.w = textWidth;
    dialog->rect.h = textHeight;
}

void render(SDL_Renderer* renderer, Scene scenes[], int currentSceneIndex) {
    SDL_RenderClear(renderer);
    Scene* currentScene = &scenes[currentSceneIndex];
    // 渲染當前背景
    SDL_RenderCopy(renderer, currentScene->backgrounds[currentScene->currentBackgroundIndex].texture, NULL, NULL);
    
    Dialog* currentDialog = &currentScene->dialogs[currentScene->currentDialogIndex];
    // 渲染對話中指定的角色
    for (int i = 0; i < currentDialog->totalCharacters; i++) {
        SDL_RenderCopy(renderer, currentDialog->characters[i].texture, NULL, &currentDialog->characters[i].rect);
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
            // 檢查是否點擊了對話區域
            if (y > WINDOW_HEIGHT - DIALOG_HEIGHT) {
                Scene* currentScene = &scenes[*currentSceneIndex];
                currentScene->currentDialogIndex = (currentScene->currentDialogIndex + 1) % currentScene->totalDialogs;
                Dialog* currentDialog = &currentScene->dialogs[currentScene->currentDialogIndex];
                // 如果對話所屬的背景和當前的不同，則需要切換背景
                if (currentDialog->at_background != currentScene->currentBackgroundIndex) {
                    currentScene->currentBackgroundIndex = currentDialog->at_background;
                }
            }
        }
    }
}

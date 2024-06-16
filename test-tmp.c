#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>
#include "uchow.h"

// 函式宣告
bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Scene scenes[], TTF_Font* font, int totalScenes);
bool loadMedia(SDL_Renderer* renderer, Scene scenes[], TTF_Font** font, Backpack** backpackItems);
void render(SDL_Renderer* renderer, Scene scenes[], int currentSceneIndex, TTF_Font* font, bool backpackVisible, Backpack* backpackItems);
void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes, bool* backpackVisible, SDL_Renderer* renderer, Backpack* backpackItems, int totalItems, TTF_Font* font);
void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog);
void printDialogText(const char* dialogText);
void toggleBackpackDisplay(bool* backpackVisible, SDL_Renderer* renderer, Backpack* backpackItems, int totalItems);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;

    Scene scenes[50];
    load_story(scenes);
    Backpack* backpackItems = NULL; // 在 main 函數開始時宣告
    bool backpackVisible = false; // 用於追蹤背包是否顯示
    int totalItems = 4; // 假設您有 4 個背包道具

    int currentSceneIndex = 0;

    if (!initSDL(&window, &renderer)) {
        printf("Failed to initialize!\n");
    } else {
        if (!loadMedia(renderer, scenes, &font, &backpackItems)) {
            printf("Failed to load media!\n");
        } else {
            bool running = true;
            SDL_Event e;
            printf("start run\n");
            while (running) {
                handleEvents(&e, &running, &currentSceneIndex, scenes, MAX_SCENES, &backpackVisible, renderer, backpackItems, totalItems, font);
                render(renderer, scenes, currentSceneIndex, font, backpackVisible, backpackItems);
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
    printf("init finish\n");
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

bool loadMedia(SDL_Renderer* renderer, Scene scenes[], TTF_Font** font, Backpack** backpackItems) {
    // 初始化 PNG 加載
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    printf("img loaded \n");

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
    printf("ttf loaded \n");

    // 為背包道具分配記憶體
    *backpackItems = (Backpack*)malloc(sizeof(Backpack) * 4); // 假設有 4 個道具
    if (!*backpackItems) {
        printf("Failed to allocate memory for backpack items.\n");
        return false;
    }

    // 初始化背包道具的路徑和矩形區域
    // 假設這些路徑已經被定義好
    const char* itemImagePaths[4] = {
        "source/tool/airpods.jpg",
        "source/tool/donpaper.jpg", 
        "source/tool/nopicture.jpg", 
        "source/tool/picture.jpg"
    };
    for (int i = 0; i < 4; i++) {
        (*backpackItems)[i].itemImagePath = itemImagePaths[i];
        // 加載道具圖片
        SDL_Surface* loadedSurface = IMG_Load((*backpackItems)[i].itemImagePath);
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", (*backpackItems)[i].itemImagePath, IMG_GetError());
            return false;
        }
        (*backpackItems)[i].itemTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);

        // 設置道具圖片的矩形區域
        (*backpackItems)[i].itemRect = (SDL_Rect){250 * i, WINDOW_HEIGHT - 250, 250, 250}; // 排列道具圖片
    }

    // 加載每個場景的媒體
    for (int i = 0; i < 33; i++) {
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
        printf("scene: %d loaded\n",i);
    }
    printf("loadMedia finished \n");
    return true;
}

void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog) {
    SDL_Color textColor = {255, 255, 255}; // 文本顏色為白色

    // 釋放先前的紋理（如果存在）
    if (dialog->texture != NULL) {
        SDL_DestroyTexture(dialog->texture);
        dialog->texture = NULL;
    }

    // 設置換行的寬度
    int wrapLength = (int)(WINDOW_WIDTH * 0.8); // 根據您的遊戲設計調整

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

    // 設置對話紋理的位置和大小
    dialog->rect.x = (WINDOW_WIDTH - textWidth) / 2; // 水平置中
    dialog->rect.y = WINDOW_HEIGHT - DIALOG_HEIGHT / 2 - textHeight / 2; // 垂直置中
    dialog->rect.w = textWidth; // 設置文本寬度
    dialog->rect.h = textHeight; // 設置文本高度
}

void updateOptionTexture(SDL_Renderer* renderer, TTF_Font* font, Option* option) {
    SDL_Color textColor = {255, 255, 255}; // 文本顏色為白色

    // 釋放先前的紋理（如果存在）
    if (option->texture != NULL) {
        SDL_DestroyTexture(option->texture);
        option->texture = NULL;
    }

    // 設置換行的寬度
    int wrapLength = 400; // 根據您的遊戲設計調整

    // 使用 TTF_RenderUTF8_Blended_Wrapped 函數來創建文字表面
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, option->text, textColor, wrapLength);
    if (textSurface == NULL) {
        printf("Unable to create text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    option->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (option->texture == NULL) {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // 計算文字的寬度和高度
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface); // 釋放文本表面

    // 設置文字紋理的位置
    option->rect.x = option->rect.x; // 根據選項的背景矩形位置設置
    option->rect.y = option->rect.y + (option->rect.h - textHeight) / 2; // 垂直置中
    option->rect.w = textWidth; // 設置文本寬度
    option->rect.h = textHeight; // 設置文本高度

}

void render(SDL_Renderer* renderer, Scene scenes[], int currentSceneIndex, TTF_Font* font, bool backpackVisible, Backpack* backpackItems) {
    SDL_RenderClear(renderer);
    Scene* currentScene = &scenes[currentSceneIndex];

    // 渲染當前背景
    SDL_RenderCopy(renderer, currentScene->backgrounds[currentScene->currentBackgroundIndex].texture, NULL, NULL);
    
    // 渲染對話中指定的角色
    Dialog* currentDialog = &currentScene->dialogs[currentScene->currentDialogIndex];
    for (int i = 0; i < currentDialog->totalCharacters; i++) {
        SDL_RenderCopy(renderer, currentDialog->characters[i].texture, NULL, &currentDialog->characters[i].rect);
    }

    // 更新並渲染選項
    for (int i = 0; i < currentDialog->totalOptions; i++) {
        updateOptionTexture(renderer, font, &currentDialog->options[i]);

        // 設置選項背景為 80% 黑色透明度
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 204); // 204 是 80% 的透明度
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

    // 如果背包可見，則渲染背包
    if (backpackVisible) {
        for (int i = 0; i < MAX_ITEMS; i++) {
            SDL_RenderCopy(renderer, backpackItems[i].itemTexture, NULL, &backpackItems[i].itemRect);
        }
    }

    SDL_RenderPresent(renderer);
}


/*
void render(SDL_Renderer* renderer, Scene scenes[], int currentSceneIndex, TTF_Font* font, bool backpackVisible, Backpack* backpackItems) {
    SDL_RenderClear(renderer);
    Scene* currentScene = &scenes[currentSceneIndex];

    // 渲染當前背景
    SDL_RenderCopy(renderer, currentScene->backgrounds[currentScene->currentBackgroundIndex].texture, NULL, NULL);
    
    // 渲染對話中指定的角色
    Dialog* currentDialog = &currentScene->dialogs[currentScene->currentDialogIndex];
    for (int i = 0; i < currentDialog->totalCharacters; i++) {
        SDL_RenderCopy(renderer, currentDialog->characters[i].texture, NULL, &currentDialog->characters[i].rect);
    }

    // 更新並渲染選項
    for (int i = 0; i < currentDialog->totalOptions; i++) {
        updateOptionTexture(renderer, font, &currentDialog->options[i]);
        SDL_RenderCopy(renderer, currentDialog->options[i].texture, NULL, &currentDialog->options[i].rect);
    }

    // 渲染對話按鈕背景
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 204); // 設置為半透明黑色
    SDL_Rect dialogRect = {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT};
    SDL_RenderFillRect(renderer, &dialogRect);

    // 渲染對話文本
    SDL_RenderCopy(renderer, currentDialog->texture, NULL, &currentDialog->rect);

    // 如果背包可見，則渲染背包
    if (backpackVisible) {
        for (int i = 0; i < MAX_ITEMS; i++) {
            SDL_RenderCopy(renderer, backpackItems[i].itemTexture, NULL, &backpackItems[i].itemRect);
        }
    }

    SDL_RenderPresent(renderer);
}
*/

// 切換背包顯示狀態的函數
void toggleBackpackDisplay(bool* backpackVisible, SDL_Renderer* renderer, Backpack* backpackItems, int totalItems) {
    *backpackVisible = !*backpackVisible; // 切換顯示狀態
    if (*backpackVisible) {
        // 如果背包顯示，則渲染背包內容
        for (int i = 0; i < totalItems; i++) {
            SDL_RenderCopy(renderer, backpackItems[i].itemTexture, NULL, &backpackItems[i].itemRect);
        }
        SDL_RenderPresent(renderer);
    }
}

void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes, bool* backpackVisible, SDL_Renderer* renderer, Backpack* backpackItems, int totalItems, TTF_Font* font) {
    while (SDL_PollEvent(e) != 0) {
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b) {
            // 切換背包顯示狀態
            toggleBackpackDisplay(backpackVisible, renderer, backpackItems, totalItems);
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            Dialog* currentDialog = &scenes[*currentSceneIndex].dialogs[scenes[*currentSceneIndex].currentDialogIndex];
            
            // 檢查是否點擊了對話區域
            if (!*backpackVisible && y > WINDOW_HEIGHT - DIALOG_HEIGHT) {
                Scene* currentScene = &scenes[*currentSceneIndex];
                currentScene->currentDialogIndex = (currentScene->currentDialogIndex + 1) % currentScene->totalDialogs;

                // 如果對話所屬的背景和當前的不同，則需要切換背景
                if (currentScene->dialogs[currentScene->currentDialogIndex].at_background != currentScene->currentBackgroundIndex) {
                    currentScene->currentBackgroundIndex = currentScene->dialogs[currentScene->currentDialogIndex].at_background;
                }
            }
            
            // 檢查是否點擊了選項
            if (!*backpackVisible) {
                for (int i = 0; i < currentDialog->totalOptions; i++) {
                    SDL_Rect optionRect = currentDialog->options[i].rect;
                    if (x >= optionRect.x && x <= (optionRect.x + optionRect.w) &&
                        y >= optionRect.y && y <= (optionRect.y + optionRect.h)) {
                        // 更新場景索引
                        printf("切換至 %d \n",currentDialog->options[i].nextSceneIndex);
                        *currentSceneIndex = currentDialog->options[i].nextSceneIndex;
                        
                        // 重置當前場景的對話索引，以便從新場景的第一句話開始
                        scenes[*currentSceneIndex].currentDialogIndex = 0;
                        
                        // 更新場景的背景索引
                        scenes[*currentSceneIndex].currentBackgroundIndex = scenes[*currentSceneIndex].dialogs[0].at_background;
                        
                        // 由於場景已經改變，跳出選項檢查循環
                        break;
                    }
                }
            }
        }
    }
}


/*
void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes) {
    while (SDL_PollEvent(e) != 0) {
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            Dialog* currentDialog = &scenes[*currentSceneIndex].dialogs[scenes[*currentSceneIndex].currentDialogIndex];
            
            // 檢查是否點擊了選項
            for (int i = 0; i < currentDialog->totalOptions; i++) {
                SDL_Rect optionRect = currentDialog->options[i].rect;
                if (x >= optionRect.x && x <= (optionRect.x + optionRect.w) &&
                    y >= optionRect.y && y <= (optionRect.y + optionRect.h)) {
                    // 更新場景索引
                    *currentSceneIndex = currentDialog->options[i].nextSceneIndex;
                    // 重置當前場景的對話索引，以便從新場景的第一句話開始
                    scenes[*currentSceneIndex].currentDialogIndex = 0;
                    // 更新場景的背景索引
                    scenes[*currentSceneIndex].currentBackgroundIndex = scenes[*currentSceneIndex].dialogs[0].at_background;
                }
            }

            // 檢查是否點擊了對話區域
            if(currentDialog->totalOptions!=0){
                continue;
            }
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
*/
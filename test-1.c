#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>
#include "uchow.h"

// 函式宣告
bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Scene scenes[], TTF_Font* font, int totalScenes);
bool loadMedia(SDL_Renderer* renderer, Scene scenes[], TTF_Font** font);
void render(SDL_Renderer* renderer, Scene scenes[], int currentSceneIndex, TTF_Font* font);
void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes);
void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog);
void printDialogText(const char* dialogText);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;

    // 後端修改區 開始 

    Scene scenes[50];
    load_story(scenes);

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
            printf("start run\n");
            while (running) {
                handleEvents(&e, &running, &currentSceneIndex, scenes, MAX_SCENES);
                render(renderer, scenes, currentSceneIndex,font);
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

// 加載媒體，包括圖片和對話紋理
bool loadMedia(SDL_Renderer* renderer, Scene scenes[], TTF_Font** font) {
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

void updateOptionTexture(SDL_Renderer* renderer, TTF_Font* font, Option* option) {
    SDL_Color textColor = {255, 255, 255}; // 文本顏色為白色

    // 釋放先前的紋理（如果存在）
    if (option->texture != NULL) {
        SDL_DestroyTexture(option->texture);
        option->texture = NULL;
    }

    // 設置字體大小為24
    TTF_SetFontSize(font, 24);

    // 設置換行的寬度為400像素
    int wrapLength = 400;

    // 使用 TTF_RenderUTF8_Blended_Wrapped 函數來創建文字表面，支持UTF8編碼的中文
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

    // 設置文字紋理的位置，使其在按鈕內靠左並垂直置中
    SDL_Rect textRect;
    textRect.x = option->rect.x; // 靠左對齊，留出邊距
    textRect.y = option->rect.y + (option->rect.h - textHeight) / 2; // 垂直置中
    textRect.w = textWidth; // 設置文本寬度
    textRect.h = textHeight; // 設置文本高度

    // 更新選項的 textRect 為文字的 rect，用於渲染文字
    option->rect = textRect;
}

void render(SDL_Renderer* renderer, Scene scenes[], int currentSceneIndex, TTF_Font* font) {
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
        // 更新選項紋理
        updateOptionTexture(renderer, font, &currentDialog->options[i]);

        // 渲染選項背景
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色背景
        SDL_RenderFillRect(renderer, &currentDialog->options[i].rect);

        // 更新選項紋理
        updateOptionTexture(renderer, font, &currentDialog->options[i]);

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

/*
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
*/

void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes) {
    while (SDL_PollEvent(e) != 0) {
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            Dialog* currentDialog = &scenes[*currentSceneIndex].dialogs[scenes[*currentSceneIndex].currentDialogIndex];
            
            
                // 檢查是否點擊了對話區域
                if (y > WINDOW_HEIGHT - DIALOG_HEIGHT) {
                    Scene* currentScene = &scenes[*currentSceneIndex];
                    currentScene->currentDialogIndex = (currentScene->currentDialogIndex + 1) % currentScene->totalDialogs;

                    // 如果對話所屬的背景和當前的不同，則需要切換背景
                    if (currentScene->dialogs[currentScene->currentDialogIndex].at_background != currentScene->currentBackgroundIndex) {
                        currentScene->currentBackgroundIndex = currentScene->dialogs[currentScene->currentDialogIndex].at_background;
                    }
                }
            
            
            // 檢查是否點擊了選項
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
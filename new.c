#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include "uchow.h"

// 函式宣告
bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Scene scenes[], TTF_Font* font, int totalScenes);
bool loadMedia(SDL_Renderer* renderer, Scene scenes[], TTF_Font** font, Backpack** backpackItems, int fontSize);
void render(SDL_Renderer* renderer, Scene scenes[], int currentSceneIndex, TTF_Font* font, bool backpackVisible, Backpack* backpackItems);
void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes, bool* backpackVisible, SDL_Renderer* renderer, Backpack* backpackItems, TTF_Font* font, GameState* gameState);
void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog, Backpack* backpackItems);
void printDialogText(const char* dialogText);
void toggleBackpackDisplay(bool* backpackVisible, SDL_Renderer* renderer, Backpack* backpackItems);
void loadBackpackItems(Backpack* backpackItems);
bool displayItemDescription(SDL_Renderer* renderer, TTF_Font* font, int mouseX, int mouseY, Backpack* backpackItems);
bool showStartScreen(SDL_Renderer* renderer, TTF_Font** font, GameState* gameState, int* fontSize);
int indexOf(int value, int* array, int size);
void handleDialogTags(char* dialogText, Like likes[], int totalLikes, Backpack* backpackItems);
void transitionEffect(SDL_Renderer* renderer, SDL_Texture* currentTexture, SDL_Texture* nextTexture, int transitionTime);

bool load = false;

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
    GameState gameState = GAME_STATE_START_SCREEN;
    int fontSize = 24;

    Scene scenes[50];
    load_story(scenes);
    Backpack* backpackItems = NULL; 
    bool backpackVisible = false; // 用於追蹤背包是否顯示
    // int totalItems = 0;
    const char *filename = "test.txt";
    int32_t totalScenes = 0;
    if (access(filename, F_OK) != -1)
    {
        loadFromFile(filename, scenes, &totalScenes);
    }
    else
    {
        load_story(scenes);
    }

    int currentSceneIndex = -1;

    if (!initSDL(&window, &renderer)) {
        printf("Failed to initialize!\n");
    } else {
        if (!loadMedia(renderer, scenes, &font, &backpackItems,fontSize)) {
            printf("Failed to load media!\n");
        } else {
            bool running = true;
            SDL_Event e;
            printf("start run\n");
            
            // 顯示開始畫面
            if (!showStartScreen(renderer, &font, &gameState,&fontSize)) {
                closeSDL(window, renderer, scenes, font, MAX_SCENES);
                return 0;
            }
            currentSceneIndex = 0;
            loadMedia(renderer, scenes, &font, &backpackItems,fontSize);
            // 遊戲主循環
            while (running) {
                handleEvents(&e, &running, &currentSceneIndex, scenes, MAX_SCENES, &backpackVisible, renderer, backpackItems, font, &gameState);
                if (gameState == GAME_STATE_RUNNING) {
                    render(renderer, scenes, currentSceneIndex, font, backpackVisible, backpackItems);
                }
            }
        }
    }

    // closeSDL(window, renderer, scenes, font, MAX_SCENES);
    return 0;
}
/*
int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
    GameState gameState = GAME_STATE_START_SCREEN;
    int fontSize = 24; // 預設字體大小

    Scene scenes[50];
    Backpack* backpackItems = NULL;
    bool backpackVisible = false;
    int32_t totalScenes = 0;

    if (!initSDL(&window, &renderer)) {
        printf("Failed to initialize!\n");
    } else {
        // 顯示開始畫面
        if (!showStartScreen(renderer, &font, &gameState, &fontSize)) {
            closeSDL(window, renderer, scenes, font, MAX_SCENES);
            return 0;
        }

        // 加載故事和背包項目
        load_story(scenes);
        loadBackpackItems(backpackItems);

        // 檢查是否有存檔可以加載
        const char *filename = "test.txt";
        if (access(filename, F_OK) != -1) {
            loadFromFile(filename, scenes, &totalScenes);
        }

        // 加載媒體資源
        if (!loadMedia(renderer, scenes, &font, &backpackItems, fontSize)) {
            printf("Failed to load media!\n");
        } else {
            int currentSceneIndex = 0;
            bool running = true;
            SDL_Event e;
            printf("start run\n");

            // 遊戲主循環
            while (running) {
                handleEvents(&e, &running, &currentSceneIndex, scenes, MAX_SCENES, &backpackVisible, renderer, backpackItems, font, &gameState);
                if (gameState == GAME_STATE_RUNNING) {
                    render(renderer, scenes, currentSceneIndex, font, backpackVisible, backpackItems);
                }
            }
        }
    }

    // 關閉SDL並釋放資源
    closeSDL(window, renderer, scenes, font, MAX_SCENES);
    return 0;
}
*/

/*
bool showStartScreen(SDL_Renderer* renderer, TTF_Font** font, GameState* gameState, int* fontSize) {
    bool startGame = false;
    SDL_Event e;
    SDL_Texture* startScreenTexture = IMG_LoadTexture(renderer, "source/image/開始.jpg");
    SDL_Rect startButtonRect = {100, 100, 200, 50}; // 假設按鈕位置和尺寸
    SDL_Rect fontSizeButtonRect = {100, 200, 200, 50}; // 字體大小按鈕位置和尺寸
    const char* fontSizeTexts[] = {"大", "中", "小"};
    int currentFontSizeIndex = 1; // 預設為中

    // 渲染開始畫面背景
    SDL_RenderCopy(renderer, startScreenTexture, NULL, NULL);

    // 渲染 "開始遊戲" 按鈕
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色按鈕
    SDL_RenderFillRect(renderer, &startButtonRect);

    // 渲染按鈕文字
    SDL_Color textColor = {0, 0, 0}; // 黑色文字
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(*font, "開始遊戲", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &startButtonRect);

    // 渲染 "字體大小" 按鈕
    SDL_RenderFillRect(renderer, &fontSizeButtonRect);
    SDL_Surface* fontSizeSurface = TTF_RenderUTF8_Solid(*font, fontSizeTexts[currentFontSizeIndex], textColor);
    SDL_Texture* fontSizeTexture = SDL_CreateTextureFromSurface(renderer, fontSizeSurface);
    SDL_RenderCopy(renderer, fontSizeTexture, NULL, &fontSizeButtonRect);

    SDL_RenderPresent(renderer);

    // 處理開始畫面的事件
    while (!startGame) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                *gameState = GAME_STATE_END_SCREEN;
                startGame = true; // 退出遊戲
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= startButtonRect.x && x <= (startButtonRect.x + startButtonRect.w) &&
                    y >= startButtonRect.y && y <= (startButtonRect.y + startButtonRect.h)) {
                    *gameState = GAME_STATE_RUNNING;
                    startGame = true; // 開始遊戲
                } else if (x >= fontSizeButtonRect.x && x <= (fontSizeButtonRect.x + fontSizeButtonRect.w) &&
                           y >= fontSizeButtonRect.y && y <= (fontSizeButtonRect.y + fontSizeButtonRect.h)) {
                    currentFontSizeIndex = (currentFontSizeIndex + 1) % 3; // 切換字體大小
                    int fontSizes[] = {30, 24, 16};
                    *fontSize = fontSizes[currentFontSizeIndex];
                    // 重新加載字體
                    TTF_CloseFont(*font);
                    *font = TTF_OpenFont("source/text/cjkfonts_handingwriting4.ttf", *fontSize);
                    // 更新 "字體大小" 按鈕的文字
                    SDL_DestroyTexture(fontSizeTexture);
                    SDL_FreeSurface(fontSizeSurface);
                    fontSizeSurface = TTF_RenderUTF8_Solid(*font, fontSizeTexts[currentFontSizeIndex], textColor);
                    fontSizeTexture = SDL_CreateTextureFromSurface(renderer, fontSizeSurface);
                    SDL_RenderCopy(renderer, fontSizeTexture, NULL, &fontSizeButtonRect);
                    SDL_RenderPresent(renderer);
                }
            }
        }
    }

    // 釋放資源
    SDL_DestroyTexture(startScreenTexture);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(fontSizeTexture);
    SDL_FreeSurface(fontSizeSurface);

    return (*gameState != GAME_STATE_END_SCREEN);
}
*/
bool showStartScreen(SDL_Renderer* renderer, TTF_Font** font, GameState* gameState, int* fontSize) {
    bool startGame = false;
    SDL_Event e;
    SDL_Texture* startScreenTexture = IMG_LoadTexture(renderer, "source/image/開始.jpg");
    SDL_Rect startButtonRect = {100, 100, 200, 50}; // 假設按鈕位置和尺寸
    SDL_Rect fontSizeButtonRect = {100, 200, 50, 50}; // 字體大小按鈕位置和尺寸
    const char* fontSizeTexts[] = {"大", "中", "小"};
    int currentFontSizeIndex = 1; // 預設為中

    // 渲染開始畫面背景
    SDL_RenderCopy(renderer, startScreenTexture, NULL, NULL);

    // 渲染 "開始遊戲" 按鈕
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色按鈕
    SDL_RenderFillRect(renderer, &startButtonRect);

    // 渲染按鈕文字
    SDL_Color textColor = {0, 0, 0}; // 黑色文字
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(*font, "開始遊戲", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &startButtonRect);

    // 渲染 "字體大小" 按鈕
    SDL_RenderFillRect(renderer, &fontSizeButtonRect);
    SDL_Surface* fontSizeSurface = TTF_RenderUTF8_Solid(*font, fontSizeTexts[currentFontSizeIndex], textColor);
    SDL_Texture* fontSizeTexture = SDL_CreateTextureFromSurface(renderer, fontSizeSurface);
    SDL_RenderCopy(renderer, fontSizeTexture, NULL, &fontSizeButtonRect);

    SDL_RenderPresent(renderer);

    // 處理開始畫面的事件
    while (!startGame) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                *gameState = GAME_STATE_END_SCREEN;
                startGame = true; // 退出遊戲
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= startButtonRect.x && x <= (startButtonRect.x + startButtonRect.w) &&
                    y >= startButtonRect.y && y <= (startButtonRect.y + startButtonRect.h)) {
                    *gameState = GAME_STATE_RUNNING;
                    startGame = true; // 開始遊戲
                } else if (x >= fontSizeButtonRect.x && x <= (fontSizeButtonRect.x + fontSizeButtonRect.w) &&
                           y >= fontSizeButtonRect.y && y <= (fontSizeButtonRect.y + fontSizeButtonRect.h)) {
                    currentFontSizeIndex = (currentFontSizeIndex + 1) % 3; // 切換字體大小
                    const int fontSizes[] = {30, 24, 16};
                    *fontSize = fontSizes[currentFontSizeIndex];
                    // 重新加載字體
                    TTF_CloseFont(*font);
                    *font = TTF_OpenFont("source/text/cjkfonts_handingwriting4.ttf", *fontSize);
                    // 清除舊的 "字體大小" 按鈕的文字
                    SDL_DestroyTexture(fontSizeTexture);
                    SDL_FreeSurface(fontSizeSurface);
                    // 重新渲染 "字體大小" 按鈕
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色按鈕
                    SDL_RenderFillRect(renderer, &fontSizeButtonRect);
                    // 更新 "字體大小" 按鈕的文字
                    fontSizeSurface = TTF_RenderUTF8_Solid(*font, fontSizeTexts[currentFontSizeIndex], textColor);
                    fontSizeTexture = SDL_CreateTextureFromSurface(renderer, fontSizeSurface);
                    SDL_RenderCopy(renderer, fontSizeTexture, NULL, &fontSizeButtonRect);
                    // 重新渲染整個畫面
                    SDL_RenderPresent(renderer);
                }
            }
        }
    }

    // 釋放資源
    SDL_DestroyTexture(startScreenTexture);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(fontSizeTexture);
    SDL_FreeSurface(fontSizeSurface);

    return (*gameState != GAME_STATE_END_SCREEN);
}

// 輔助函式，用於找出數字在陣列中的索引
int indexOf(int value, int* array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            return i;
        }
    }
    return -1; // 如果找不到，返回-1
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

bool loadMedia(SDL_Renderer* renderer, Scene scenes[], TTF_Font** font, Backpack** backpackItems, int fontSize) {
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
    *font = TTF_OpenFont(TEXT_PATH, fontSize); // 字體大小設為 24px
    if (*font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    printf("ttf loaded \n");
    if(*backpackItems == NULL){
        // 為背包道具分配記憶體
        *backpackItems = (Backpack*)malloc(sizeof(Backpack) * 4); // 假設有 4 個道具
        if (!*backpackItems) {
            printf("Failed to allocate memory for backpack items.\n");
            return false;
        }

        loadBackpackItems(*backpackItems);

        for (int i = 0; i < MAX_ITEMS; i++) {
            SDL_Surface* loadedSurface = IMG_Load((*backpackItems)[i].itemImagePath);
            if (loadedSurface == NULL) {
                printf("Unable to load image %s! SDL_image Error: %s\n", (*backpackItems)[i].itemImagePath, IMG_GetError());
                return false;
            }
            (*backpackItems)[i].itemTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            SDL_FreeSurface(loadedSurface);

            // 設置道具圖片的矩形區域
            (*backpackItems)[i].itemRect = (SDL_Rect){260 * i + (WINDOW_WIDTH * 0.2), (WINDOW_HEIGHT * 0.2) , 250, 250}; // 排列道具圖片
        }
        printf("backpack loaded\n");
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
            updateDialogTexture(renderer, *font, &scene->dialogs[j],*backpackItems);
        }
        printf("scene: %d loaded\n",i);
    }
    printf("loadMedia finished \n");
    load = true;
    return true;
}

void loadBackpackItems(Backpack* backpackItems) {
    DIR *d;
    struct dirent *dir;
    d = opendir("./source/tool");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) { // 確保是一個文件
                // 從文件名中去除擴展名，並將其作為物品名稱
                char* dot = strrchr(dir->d_name, '.');
                if (dot && strcmp(dot, ".jpg") == 0) {
                    *dot = '\0'; // 終止文件名，去除擴展名
                    strcpy(backpackItems[backpackItems[0].totalItems].itemName, dir->d_name);
                    strcpy(backpackItems[backpackItems[0].totalItems].itemImagePath, "./source/tool/");
                    strcat(backpackItems[backpackItems[0].totalItems].itemImagePath, dir->d_name);
                    strcat(backpackItems[backpackItems[0].totalItems].itemImagePath, ".jpg");
                    strcpy(backpackItems[backpackItems[0].totalItems].itemDescriptionPath, "./source/tool/");
                    strcat(backpackItems[backpackItems[0].totalItems].itemDescriptionPath, dir->d_name);
                    strcat(backpackItems[backpackItems[0].totalItems].itemDescriptionPath, ".txt");
                    backpackItems[backpackItems->totalItems].have = 0;
                    printf("Item '%s' loaded, img: %s, txt: %s\n", backpackItems[backpackItems->totalItems].itemName,backpackItems[backpackItems->totalItems].itemImagePath,backpackItems[backpackItems->totalItems].itemDescriptionPath);
                    (backpackItems[0].totalItems)++;
                }
            }
        }
        closedir(d);
    }
}

bool displayItemDescription(SDL_Renderer* renderer, TTF_Font* font, int mouseX, int mouseY, Backpack* backpackItems) {
    bool itemDescriptionDisplayed = false;
    for (int i = 0; i < backpackItems->totalItems; i++) {
        if (backpackItems[i].have && mouseX >= backpackItems[i].itemRect.x && mouseX <= (backpackItems[i].itemRect.x + backpackItems[i].itemRect.w) &&
            mouseY >= backpackItems[i].itemRect.y && mouseY <= (backpackItems[i].itemRect.y + backpackItems[i].itemRect.h)) {
            // 渲染半透明背景
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 102); // 40% 透明度的黑色
            SDL_Rect backgroundRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
            SDL_RenderFillRect(renderer, &backgroundRect);

            // 讀取道具敘述
            char description[9192] = ""; // 初始化為空字符串
            FILE* file = fopen(backpackItems[i].itemDescriptionPath, "r");
            if (file) {
                char line[256];
                while (fgets(line, sizeof(line), file)) {
                    strcat(description, line); // 將每一行添加到描述字符串
                }
                fclose(file);

                SDL_Color textColor = {255, 255, 255}; // 白色文本
                SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, description, textColor, WINDOW_WIDTH * 0.8);
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);

                SDL_Rect textRect = {
                    .x = WINDOW_WIDTH * 0.2,
                    .y = backpackItems[i].itemRect.y + backpackItems[i].itemRect.h + 10, // 文本放在圖片下方
                    .w = textSurface->w,
                    .h = textSurface->h
                };

                // 渲染文字方塊
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
                itemDescriptionDisplayed = true;
            }
        }
    }
    return itemDescriptionDisplayed;
}

void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog, Backpack* backpackItems) {
    // 處理對話中的標籤 <>
    if(load==true){
        //handleDialogTags(dialog->text, likes, totalLikes, backpackItems);
    }
    
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

// 假設您在 main 函數或其他初始化函數中這樣做
Like likes[] = {
    {"Becca", 2, 3},
    {"Police", 5, 3},
    {"Street", 8, 3}
};
const int totalLikes = sizeof(likes) / sizeof(likes[0]);


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
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool itemDescriptionDisplayed = displayItemDescription(renderer, font, mouseX, mouseY, backpackItems);

        // 在這裡設置半透明背景
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 102); // 40% 透明度的黑色
        SDL_Rect fullScreenRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(renderer, &fullScreenRect);

        for (int i = 0; i < backpackItems[0].totalItems; i++) {
            // 渲染每個道具的半透明背景
            // SDL_Rect backgroundRect = backpackItems[i].itemRect;

            // 檢查每個物品的 have 欄位，只渲染 have 大於 0 的物品
            if (backpackItems[i].have > 0) {
                SDL_RenderCopy(renderer, backpackItems[i].itemTexture, NULL, &backpackItems[i].itemRect);
            }
        }
        displayItemDescription(renderer, font, mouseX, mouseY, backpackItems);
        if (!itemDescriptionDisplayed) {
            // 渲染好感度
            SDL_Color textColor = {255, 255, 255}; // 白色文本
            char likeText[1024] = "";
            for (int i = 0; i < totalLikes; i++) {
                char line[256];
                sprintf(line, "%s : %d \n", &likes[i].name[0], likes[i].value);
                strcat(likeText, line);
            }

            SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, likeText, textColor, WINDOW_WIDTH);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);

            SDL_Rect textRect = {
                .x = WINDOW_WIDTH * 0.2,
                .y = WINDOW_HEIGHT * 0.5,
                .w = textSurface->w,
                .h = textSurface->h
            };

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        }
    }
    SDL_RenderPresent(renderer);
}


// 切換背包顯示狀態的函數
void toggleBackpackDisplay(bool* backpackVisible, SDL_Renderer* renderer, Backpack* backpackItems) {
    *backpackVisible = !*backpackVisible; // 切換顯示狀態
    if (*backpackVisible) {
        // 如果背包顯示，則渲染背包內容
        // printf("%d \n",backpackItems->totalItems);
        printf("%d: ", backpackItems[0].totalItems);
        for (int i = 0; i < backpackItems[0].totalItems; i++) {
            printf("%d ",backpackItems[i].have);
            if(backpackItems[i].have){
                SDL_RenderCopy(renderer, backpackItems[i].itemTexture, NULL, &backpackItems[i].itemRect);
            }
        }
        printf("\n");
        SDL_RenderPresent(renderer);
    }
}

void handleDialogTags(char* dialogText, Like likes[], int totalLikes, Backpack* backpackItems) {
    char* start = dialogText;
    char* end = NULL;
    char buffer[256];
    char* context; // 用於strtok_r的上下文指針

    while ((start = strchr(start, '<')) != NULL) {
        end = strchr(start, '>');
        if (end == NULL) {
            break; // 沒有找到匹配的 '>'，終止循環
        }

        // 從標籤中提取內容
        size_t tagLength = end - start - 1;
        if (tagLength < sizeof(buffer)) {
            strncpy(buffer, start + 1, tagLength);
            buffer[tagLength] = '\0'; // 確保字符串結尾

            // 處理標籤內容
            char* token = strtok_r(buffer, " ", &context);
            if (token != NULL) {
                if (strcmp(token, "likes") == 0) {
                    // 增加好感度
                    token = strtok_r(NULL, " ", &context);
                    if (token != NULL) {
                        for (int i = 0; i < totalLikes; i++) {
                            if (strcmp(likes[i].name, token) == 0) {
                                token = strtok_r(NULL, " ", &context);
                                if (token != NULL) {
                                    likes[i].value += atoi(token); // 增加好感度
                                    break;
                                }
                            }
                        }
                    }
                } 
                if (strcmp(token, "backpack") == 0) {
                    token = strtok_r(NULL, " ", &context); // 獲取道具名稱
                    if (token != NULL) {
                        for (int i = 0; i < backpackItems[0].totalItems; i++) {
                            if (strcmp(backpackItems[i].itemName, token) == 0) {
                                backpackItems[i].have += 1; // 增加道具數量
                                printf("get %s \n", backpackItems[i].itemName);
                                break;
                            }
                        }
                    }
                }
            }

            // 移除標籤
            // memmove(start, end + 1, strlen(end + 1) + 1); // 加1以包括結尾的null字符
        }
        start = end + 1; // 更新start指針以繼續搜索下一個標籤
    }
}


void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes, bool* backpackVisible, SDL_Renderer* renderer, Backpack* backpackItems, TTF_Font* font, GameState* gameState) {
    while (SDL_PollEvent(e) != 0) {
        // 檢查是否需要顯示開始畫面
        /*
        if (*currentSceneIndex == -1) {
            *gameState = GAME_STATE_START_SCREEN;
            if (!showStartScreen(renderer, &font, gameState,fontSize)) {
                *running = false; // 如果 showStartScreen 返回 false，則退出遊戲
            } else {
                *currentSceneIndex = 0; // 重置場景索引以重新開始遊戲
            }
        }
        */
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b) {
            // 切換背包顯示狀態
            toggleBackpackDisplay(backpackVisible, renderer, backpackItems);
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            
            // 在處理對話事件之前，調用 handleDialogTags 函式
            Dialog* currentDialog = &scenes[*currentSceneIndex].dialogs[scenes[*currentSceneIndex].currentDialogIndex];
            handleDialogTags(currentDialog->text, likes, totalLikes, backpackItems);
            // 檢查是否點擊了對話區域
            if (!*backpackVisible && y > WINDOW_HEIGHT - DIALOG_HEIGHT) {
                if(scenes[*currentSceneIndex].currentDialogIndex == scenes[*currentSceneIndex].totalDialogs-1){
                    // printf("flag\n");
                    *currentSceneIndex = scenes[*currentSceneIndex].nextSceneIndex;
                }
                updateDialogTexture(renderer, font, currentDialog, backpackItems);
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
                        // 更新場景索引前進行轉場效果
                        int nextSceneIndex = currentDialog->options[i].nextSceneIndex;
                        SDL_Texture* currentTexture = scenes[*currentSceneIndex].backgrounds[0].texture;
                        SDL_Texture* nextTexture = scenes[nextSceneIndex].backgrounds[0].texture;
                        transitionEffect(renderer, currentTexture, nextTexture, 1000); // 1秒的轉場
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

            
            // 如果背包可見，則檢查滑鼠是否靠近任何道具圖片
            if (*backpackVisible) {
                displayItemDescription(renderer, font, x, y, backpackItems);
            }
            
        }
    }
}

void transitionEffect(SDL_Renderer* renderer, SDL_Texture* currentTexture, SDL_Texture* nextTexture, int transitionTime) {
    // 設置初始透明度為完全不透明
    SDL_SetTextureAlphaMod(currentTexture, 255);
    SDL_SetTextureAlphaMod(nextTexture, 0);

    // 計算每一步的透明度變化量
    int alphaStep = 255 / (transitionTime / 10); // 假設每10毫秒更新一次

    // 逐漸減少當前場景的透明度，增加下一個場景的透明度
    for (int alpha = 255; alpha >= 0; alpha -= alphaStep) {
        // 設置當前場景的透明度
        SDL_SetTextureAlphaMod(currentTexture, alpha);
        // 設置下一個場景的透明度
        SDL_SetTextureAlphaMod(nextTexture, 255 - alpha);

        // 清除畫面
        SDL_RenderClear(renderer);

        // 渲染當前場景
        SDL_RenderCopy(renderer, currentTexture, NULL, NULL);
        // 渲染下一個場景
        SDL_RenderCopy(renderer, nextTexture, NULL, NULL);

        // 更新畫面
        SDL_RenderPresent(renderer);

        // 等待一段時間
        SDL_Delay(10);
    }
}
/*
void handleEvents(SDL_Event* e, bool* running, int* currentSceneIndex, Scene scenes[], int totalScenes, bool* backpackVisible, SDL_Renderer* renderer, Backpack* backpackItems, TTF_Font* font, GameState* gameState) {
    while (SDL_PollEvent(e) != 0) {
        // 檢查是否需要顯示開始畫面
        
        if (*currentSceneIndex == -1) {
            *gameState = GAME_STATE_START_SCREEN;
            if (!showStartScreen(renderer, &font, gameState,fontSize)) {
                *running = false; // 如果 showStartScreen 返回 false，則退出遊戲
            } else {
                *currentSceneIndex = 0; // 重置場景索引以重新開始遊戲
            }
        }
        
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b) {
            // 切換背包顯示狀態
            toggleBackpackDisplay(backpackVisible, renderer, backpackItems);
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            
            // 在處理對話事件之前，調用 handleDialogTags 函式
            Dialog* currentDialog = &scenes[*currentSceneIndex].dialogs[scenes[*currentSceneIndex].currentDialogIndex];
            handleDialogTags(currentDialog->text, likes, totalLikes, backpackItems);
            // 檢查是否點擊了對話區域
            if (!*backpackVisible && y > WINDOW_HEIGHT - DIALOG_HEIGHT) {
                if(scenes[*currentSceneIndex].currentDialogIndex == scenes[*currentSceneIndex].totalDialogs-1){
                    // printf("flag\n");
                    *currentSceneIndex = scenes[*currentSceneIndex].nextSceneIndex;
                }
                updateDialogTexture(renderer, font, currentDialog, backpackItems);
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

            
            // 如果背包可見，則檢查滑鼠是否靠近任何道具圖片
            if (*backpackVisible) {
                displayItemDescription(renderer, font, x, y, backpackItems);
            }
            
        }
    }
}
*/
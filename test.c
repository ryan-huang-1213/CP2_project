#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
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
    int nextSceneIndex; // 跳轉至下一個頁面
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
            (Asset[2]){
                {"source/image/maldives.bmp", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}},
                {"source/image/tmp_background.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}}
            },
            2,
            (Asset[1]){{"source/character/LG.png", NULL, {0, 0, 0, 0}}}, // 新增角色
            1, // 總角色數量更新為 1
            (Dialog[4]){{"這是馬爾地夫，是一個眾所周知的旅遊勝地", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0},
                        {"該地以其優美的大海還有沙灘聞名於世", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0},
                        {
                            "L紀的夢想是有朝一日可以去馬爾地夫好好度假。奈何教授薪資對於遊玩馬爾地夫來說，屬實是杯水車薪。這裡臨時增加很多很多很多很多很多的文字這裡臨時增加很多很多很多很多很多的文字這裡臨時增加很多很多很多很多很多的文字",
                            NULL,
                            {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT},
                            1,
                            scenes[0].characters, // 指向第一個場景的角色
                            1 // 第三句話時有一個角色出現
                        },
                        {               
                            "所以，他決定臨時找一些工作去賺更多錢，好讓夢想早日成真。不過L紀要做甚麼工作呢 ? ",
                            NULL,
                            {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT},
                            1,
                            scenes[0].characters, // 指向第一個場景的角色
                            1 // 第四句話時有一個角色出現
                        }
            },
            4,
            0,
            0
        },
        {
            (Asset[2]){
                {"source/image/體育場.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}},
                {"source/image/晚上的公園.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}}
            },
            2,
            (Asset[2]){
                {"source/character/LG.png", NULL, {0, 0, 0, 0}},
                {"source/character/Becca.jpg", NULL, {0, 0, 0, 0}},
            }, // 新增角色
            2, // 總角色數量更新為 2
            (Dialog[2]){
                {"辛苦工作了一天，來看看錢包裡面有多少錢 ( 打開錢包 )", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0},
                {"什麼 !? 忙了這麼久居然只有 10 元，連茶葉蛋都買不起吧 ? ", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 1}
            },
            2,
            0,
            0
        }, 
        {
            (Asset[2]){
                {"source/image/tmp_background.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}},
                {"source/image/生態池.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}}
            },
            2,
            (Asset[2]){
                {"source/character/LG.png", NULL, {0, 0, 0, 0}},
                {"source/character/Jenny.jpg", NULL, {0, 0, 0, 0}},
            }, // 新增角色
            2, // 總角色數量更新為 2
            (Dialog[2]){{"為了馬爾地夫，只能忍氣吞聲來到這裡工作了", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0},
                         {"終於到了午休時段了，來生態池旁邊休息一下吧", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0}
            },
            2,
            0,
            0
        }
        // ... Other scenes
    };

    scenes[1].dialogs[0].at_background = 0; // 第一個對話對應第一張背景圖
    scenes[1].dialogs[1].at_background = 1; // 第二個對話對應第二張背景圖
    scenes[2].dialogs[0].at_background = 0; // 第一個對話對應第一張背景圖
    scenes[2].dialogs[1].at_background = 1; // 第二個對話對應第二張背景圖

    // 設置角色的位置
    scenes[0].characters[0].rect.x = 0; // 角色出現在視窗的左側
    scenes[0].characters[0].rect.y = WINDOW_HEIGHT * 0.3; // 根據需要調整 y 值
    scenes[0].characters[0].rect.w = 250;
    scenes[0].characters[0].rect.h = 450;

    // 初始化選項
    Option options[] = {
        {"去微軟工作", NULL, {WINDOW_WIDTH - 300, 100, 200, 50},2},
        {"掃操場", NULL, {WINDOW_WIDTH - 300, 200, 200, 50},1}
    };
    int totalOptions = sizeof(options) / sizeof(options[0]);

    // 將選項與對話關聯
    scenes[0].dialogs[3].options = options;
    scenes[0].dialogs[3].totalOptions = totalOptions;

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

/*
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

    // 計算並渲染選項
    int optionY = WINDOW_HEIGHT - DIALOG_HEIGHT - 20; // 從對話框上方 20px 開始
    for (int i = 0; i < currentDialog->totalOptions; i++) {
        // 設置選項的位置
        currentDialog->options[i].rect.x = WINDOW_WIDTH - currentDialog->options[i].rect.w - 20; // 靠右對齊，留出邊距
        currentDialog->options[i].rect.y = optionY - (currentDialog->options[i].rect.h + 10) * i; // 每個選項上移並相距 10px
        // 渲染選項
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
                Dialog* currentDialog = &currentScene->dialogs[currentScene->currentDialogIndex];
                // 如果對話所屬的背景和當前的不同，則需要切換背景
                if (currentDialog->at_background != currentScene->currentBackgroundIndex) {
                    currentScene->currentBackgroundIndex = currentDialog->at_background;
                }
            }

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
        }
    }
}
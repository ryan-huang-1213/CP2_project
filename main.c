#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

// 設定視窗寬高
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
// 設定字體路徑
#define TEXT_PATH "source/text/cjkfonts_handingwriting4.ttf" // "./source/text/Arial.ttf"
// 最大圖片數量
#define MAX_IMAGES 3
// 最大對話數量
#define MAX_DIALOGS 3

// 圖片結構定義
typedef struct {
    char* path;
    SDL_Texture* texture;
} Image;

// 對話結構定義
typedef struct {
    char* text;
    char* speaker;
    SDL_Texture* texture;
    SDL_Rect rect;
} Dialog;

// 函式宣告
bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Image images[], Dialog dialogs[], TTF_Font* font);
bool loadMedia(SDL_Renderer* renderer, Image images[], Dialog dialogs[], TTF_Font** font);
void render(SDL_Renderer* renderer, Image images[], Dialog dialogs[], int currentImageIndex, int currentDialogIndex);
void handleEvents(SDL_Event* e, bool* running, int* currentImageIndex, int* currentDialogIndex, int totalImages, int totalDialogs);
void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
    // 初始化圖片和對話陣列
    Image images[MAX_IMAGES] = {
        {"source/image/tmp_background.jpg", NULL},
        {"source/image/maldives.bmp", NULL},
        {"source/image/pure_black.jpg", NULL}
    };
    Dialog dialogs[MAX_DIALOGS] = {
        {"this is text in maldives", "Narrator", NULL, {0, WINDOW_HEIGHT - WINDOW_HEIGHT / 3, WINDOW_WIDTH, WINDOW_HEIGHT / 3}},
        {"you can edit it to change text", "User", NULL, {0, WINDOW_HEIGHT - WINDOW_HEIGHT / 3, WINDOW_WIDTH, WINDOW_HEIGHT / 3}},
        {"this is third 對話 in text", "System", NULL, {0, WINDOW_HEIGHT - WINDOW_HEIGHT / 3, WINDOW_WIDTH, WINDOW_HEIGHT / 3}}
    };
    int currentImageIndex = 0;
    int currentDialogIndex = 0;

    // 初始化 SDL
    if (!initSDL(&window, &renderer)) {
        printf("Failed to initialize!\n");
    } else {
        // 加載媒體
        if (!loadMedia(renderer, images, dialogs, &font)) {
            printf("Failed to load media!\n");
        } else {
            // 主事件迴圈
            bool running = true;
            SDL_Event e;
            while (running) {
                handleEvents(&e, &running, &currentImageIndex, &currentDialogIndex, MAX_IMAGES, MAX_DIALOGS);
                render(renderer, images, dialogs, currentImageIndex, currentDialogIndex);
            }
        }
    }

    // 關閉 SDL 並釋放資源
    closeSDL(window, renderer, images, dialogs, font);
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
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Image images[], Dialog dialogs[], TTF_Font* font) {
    // 銷毀圖片和對話的紋理
    for (int i = 0; i < MAX_IMAGES; i++) {
        SDL_DestroyTexture(images[i].texture);
    }
    for (int i = 0; i < MAX_DIALOGS; i++) {
        SDL_DestroyTexture(dialogs[i].texture);
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
bool loadMedia(SDL_Renderer* renderer, Image images[], Dialog dialogs[], TTF_Font** font) {
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
    // 加載圖片
    for (int i = 0; i < MAX_IMAGES; i++) {
        SDL_Surface* loadedSurface = IMG_Load(images[i].path);
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", images[i].path, IMG_GetError());
            return false;
        }
        images[i].texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    // 打開字體
    *font = TTF_OpenFont(TEXT_PATH, 24); // 字體大小設為 24px
    if (*font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    // 為對話創建紋理
    for (int i = 0; i < MAX_DIALOGS; i++) {
        updateDialogTexture(renderer, *font, &dialogs[i]);
    }
    return true;
}

// 處理事件，包括按鈕點擊以更改對話和圖片
void handleEvents(SDL_Event* e, bool* running, int* currentImageIndex, int* currentDialogIndex, int totalImages, int totalDialogs) {
    while (SDL_PollEvent(e) != 0) {
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            // 檢查是否點擊了對話按鈕
            if (y > WINDOW_HEIGHT - WINDOW_HEIGHT / 3) {
                *currentDialogIndex = (*currentDialogIndex + 1) % totalDialogs;
                if (*currentDialogIndex == 0) { // 如果是最後一個對話
                    *currentImageIndex = (*currentImageIndex + 1) % totalImages;
                }
            }
        }
    }
}

// 使用 80% 透明度的黑色背景渲染當前圖片和對話按鈕
void render(SDL_Renderer* renderer, Image images[], Dialog dialogs[], int currentImageIndex, int currentDialogIndex) {
    // 清除屏幕
    SDL_RenderClear(renderer);

    // 渲染當前圖片
    SDL_RenderCopy(renderer, images[currentImageIndex].texture, NULL, NULL);

    // 設置繪製顏色為 80% 透明度的黑色，用於按鈕背景
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 204); // 80% 透明度
    // 繪製按鈕背景
    SDL_Rect buttonRect = {0, WINDOW_HEIGHT - WINDOW_HEIGHT / 3, WINDOW_WIDTH, WINDOW_HEIGHT / 3};
    SDL_RenderFillRect(renderer, &buttonRect);

    // 渲染對話按鈕文本
    SDL_RenderCopy(renderer, dialogs[currentDialogIndex].texture, NULL, &dialogs[currentDialogIndex].rect);

    // 更新屏幕
    SDL_RenderPresent(renderer);
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

    // 將文本方塊在按鈕內垂直和水平置中
    dialog->rect.x = (WINDOW_WIDTH - textWidth) / 2; // 水平置中
    dialog->rect.y = WINDOW_HEIGHT - (WINDOW_HEIGHT / 3) / 2 - textHeight / 2; // 垂直置中
    dialog->rect.w = textWidth;
    dialog->rect.h = textHeight;
}

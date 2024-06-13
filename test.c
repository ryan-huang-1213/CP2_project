#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TEXT_PATH "./source/text/Arial.ttf"
#define MAX_IMAGES 3
#define MAX_DIALOGS 3

// 圖片結構
typedef struct {
    char* path;
    SDL_Texture* texture;
} Image;

// 台詞結構
typedef struct {
    char* text;
    char* speaker;
    SDL_Texture* texture;
} Dialog;

// 函式原型宣告
bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Image images[], TTF_Font* font);
bool loadMedia(SDL_Renderer* renderer, Image images[], TTF_Font** font, Dialog dialogs[]);
void render(SDL_Renderer* renderer, Image images[], Dialog dialogs[], int currentImageIndex, int currentDialogIndex);
void updateButtonTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
    Image images[MAX_IMAGES] = {
        {"source/image/tmp_background.jpg", NULL},
        {"source/image/maldives.bmp", NULL},
        {"source/image/pure_black.jpg", NULL}
    };
    Dialog dialogs[MAX_DIALOGS] = {
        {"this is text in maldives", "Narrator"},
        {"you can edit it to change text", "User"},
        {"this is third 對話 in text", "System"}
    };
    int currentImageIndex = 0;
    int currentDialogIndex = 0;

    // 初始化 SDL
    if (!initSDL(&window, &renderer)) {
        printf("Failed to initialize!\n");
    } else {
        // 加載媒體
        if (!loadMedia(renderer, images, &font, dialogs)) {
            printf("Failed to load media!\n");
        } else {
            // 主迴圈標誌
            bool running = true;

            // 事件迴圈
            SDL_Event e;
            while (running) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        running = false;
                    }
                    // 在這裡處理其他事件，例如鍵盤輸入或鼠標點擊
                }
                render(renderer, images, dialogs, currentImageIndex, currentDialogIndex);
            }
        }
    }

    // 釋放資源並關閉 SDL 子系統
    closeSDL(window, renderer, images, font);
    return 0;
}
// 初始化 SDL 的函式
bool initSDL(SDL_Window** window, SDL_Renderer** renderer) {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // 初始化 SDL_image
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return false;
    }

    // 初始化 SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // 創建窗口
    *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // 創建渲染器
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF); // 設置渲染器顏色為白色

    return true;
}

// 關閉 SDL 的函式
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Image images[], TTF_Font* font) {
    // 釋放圖片紋理
    for (int i = 0; i < MAX_IMAGES; i++) {
        SDL_DestroyTexture(images[i].texture);
        images[i].texture = NULL;
    }

    // 釋放字體
    TTF_CloseFont(font);
    font = NULL;

    // 釋放渲染器和窗口
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // 關閉 SDL 子系統
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// 加載媒體的函式
bool loadMedia(SDL_Renderer* renderer, Image images[], TTF_Font** font, Dialog dialogs[]) {
    // 加載圖片
    for (int i = 0; i < MAX_IMAGES; i++) {
        SDL_Surface* loadedSurface = IMG_Load(images[i].path);
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", images[i].path, IMG_GetError());
            return false;
        }
        images[i].texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
        if (images[i].texture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", images[i].path, SDL_GetError());
            return false;
        }
    }

    // 加載字體
    *font = TTF_OpenFont(TEXT_PATH, 24);
    if (*font == NULL) {
        printf("Failed to load font %s! SDL_ttf Error: %s\n", TEXT_PATH, TTF_GetError());
        return false;
    }

    // 加載台詞紋理
    for (int i = 0; i < MAX_DIALOGS; i++) {
        updateButtonTexture(renderer, *font, &dialogs[i]);
    }

    return true;
}

// 更新台詞紋理的函式
void updateButtonTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog) {
    // 創建新的帶背景色的紋理
    SDL_Color textColor = {255, 255, 255}; // 白色文字
    SDL_Color backgroundColor = {0, 0, 0, 204}; // 黑色背景，透明度約為 80%
    SDL_Surface* textSurface = TTF_RenderText_Shaded(font, dialog->text, textColor, backgroundColor);
    dialog->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

// 渲染的函式
void render(SDL_Renderer* renderer, Image images[], Dialog dialogs[], int currentImageIndex, int currentDialogIndex) {
    // 清除屏幕
    SDL_RenderClear(renderer);

    // 渲染當前圖片
    SDL_Texture* currentTexture = images[currentImageIndex].texture;
    SDL_RenderCopy(renderer, currentTexture, NULL, NULL);

    // 渲染當前台詞
    SDL_Texture* dialogTexture = dialogs[currentDialogIndex].texture;
    int dialogWidth, dialogHeight;
    SDL_QueryTexture(dialogTexture, NULL, NULL, &dialogWidth, &dialogHeight);
    SDL_Rect renderQuad = {WINDOW_WIDTH / 2 - dialogWidth / 2, WINDOW_HEIGHT - dialogHeight - 10, dialogWidth, dialogHeight};
    SDL_RenderCopy(renderer, dialogTexture, NULL, &renderQuad);

    // 更新屏幕
    SDL_RenderPresent(renderer);
}
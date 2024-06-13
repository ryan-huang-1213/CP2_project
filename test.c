#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TEXT_PATH "./source/text/Arial.ttf"
#define BACKGROUND_IMAGE_PATH "./source/image/tmp_background.jpg"
#define MALDIVES_IMAGE_PATH "./source/image/maldives.bmp"

typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    char* text;
    bool visible;
} Button;

// 函式原型宣告
bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Button* startButton, Button* dialogButton, SDL_Texture* backgroundTexture, SDL_Texture* maldivesTexture, TTF_Font* font);
bool loadMedia(SDL_Renderer* renderer, SDL_Texture** backgroundTexture, SDL_Texture** maldivesTexture, TTF_Font** font, Button* startButton, Button* dialogButton);
void handleEvents(SDL_Event* e, bool* running, bool* showMaldives, Button* startButton, Button* dialogButton);
void render(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Texture* maldivesTexture, Button* startButton, Button* dialogButton, bool showMaldives);
void updateButtonTexture(Button* button, SDL_Renderer* renderer, TTF_Font* font);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* backgroundTexture = NULL;
    SDL_Texture* maldivesTexture = NULL;
    TTF_Font* font = NULL;
    Button startButton = {0};
    Button dialogButton = {0};

    // 初始化 SDL
    if (!initSDL(&window, &renderer)) {
        printf("Failed to initialize!\n");
    } else {
        // 加載媒體
        if (!loadMedia(renderer, &backgroundTexture, &maldivesTexture, &font, &startButton, &dialogButton)) {
            printf("Failed to load media!\n");
        } else {
            // 主迴圈標誌
            bool running = true;
            bool showMaldives = false;

            // 事件迴圈
            SDL_Event e;
            while (running) {
                handleEvents(&e, &running, &showMaldives, &startButton, &dialogButton);
                render(renderer, backgroundTexture, maldivesTexture, &startButton, &dialogButton, showMaldives);
            }
        }
    }

    // 釋放資源並關閉 SDL 子系統
    closeSDL(window, renderer, &startButton, &dialogButton, backgroundTexture, maldivesTexture, font);
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

    return true;
}

// 關閉 SDL 的函式
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Button* startButton, Button* dialogButton, SDL_Texture* backgroundTexture, SDL_Texture* maldivesTexture, TTF_Font* font) {
    // 釋放按鈕資源
    SDL_DestroyTexture(startButton->texture);
    SDL_DestroyTexture(dialogButton->texture);

    // 釋放字體
    TTF_CloseFont(font);

    // 釋放紋理
    SDL_DestroyTexture(maldivesTexture);
    SDL_DestroyTexture(backgroundTexture);

    // 釋放渲染器和窗口
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // 關閉 SDL 子系統
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// 加載媒體的函式
bool loadMedia(SDL_Renderer* renderer, SDL_Texture** backgroundTexture, SDL_Texture** maldivesTexture, TTF_Font** font, Button* startButton, Button* dialogButton) {
    // 加載背景圖片
    SDL_Surface* loadedSurface = IMG_Load(BACKGROUND_IMAGE_PATH);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", BACKGROUND_IMAGE_PATH, IMG_GetError());
        return false;
    }
    *backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    // 加載 Maldives 圖片
    loadedSurface = IMG_Load(MALDIVES_IMAGE_PATH);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", MALDIVES_IMAGE_PATH, IMG_GetError());
        SDL_DestroyTexture(*backgroundTexture);
        return false;
    }
    *maldivesTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    // 加載字體
    *font = TTF_OpenFont(TEXT_PATH, 28);
    if (*font == NULL) {
        printf("Failed to load font %s! SDL_ttf Error: %s\n", TEXT_PATH, TTF_GetError());
        SDL_DestroyTexture(*maldivesTexture);
        SDL_DestroyTexture(*backgroundTexture);
        return false;
    }

    // 創建按鈕
    // ...按鈕創建代碼...

    return true;
}

// 處理事件的函式
void handleEvents(SDL_Event* e, bool* running, bool* showMaldives, Button* startButton, Button* dialogButton) {
    while (SDL_PollEvent(e) != 0) {
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            // ...事件處理代碼...
        }
    }
}

// 渲染的函式
void render(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Texture* maldivesTexture, Button* startButton, Button* dialogButton, bool showMaldives) {
    // 清除屏幕
    SDL_RenderClear(renderer);

    // 根據 showMaldives 的值渲染背景圖片或 Maldives 圖片
    if (!showMaldives) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    } else {
        SDL_RenderCopy(renderer, maldivesTexture, NULL, NULL);
    }

    // 渲染按鈕
    // ...按鈕渲染代碼...

    // 更新屏幕
    SDL_RenderPresent(renderer);
}

// 更新按鈕紋理的函式
void updateButtonTexture(Button* button, SDL_Renderer* renderer, TTF_Font* font) {
    // 釋放舊的紋理
    SDL_DestroyTexture(button->texture);

    // 創建新的紋理
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button->text, (SDL_Color){255, 255, 255});
    button->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TEXT_PATH "./source/text/Arial.ttf"
#define BACKGROUND_IMAGE_PATH "./source/image/tmp_background.jpg"
#define MALDIVES_IMAGE_PATH "./source/image/maldives.bmp"

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* tempSurface = NULL;
    SDL_Texture* backgroundTexture = NULL;
    SDL_Texture* maldivesTexture = NULL;
    TTF_Font* font = NULL;
    SDL_Color textColor = {255, 255, 255, 255}; // 白色
    SDL_Texture* textTexture = NULL;
    SDL_Rect textRect;

    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // 初始化 SDL_image
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    // 初始化 SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // 創建窗口
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // 創建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // 創建按鈕
    Button startButton;
    Button dialogButton;
    initButton(&startButton, renderer, font, "Start", WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 25, 100, 50);
    initButton(&dialogButton, renderer, font, "this is dialog box", 0, WINDOW_HEIGHT - WINDOW_HEIGHT / 3, WINDOW_WIDTH, WINDOW_HEIGHT / 3);

    bool running = true;
    SDL_Event e;
    bool showMaldives = false; // 是否顯示 Maldives 圖片

    while (running) {
        // 處理事件
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (checkButtonClick(&startButton, x, y)) {
                    startButton.visible = false; // 隱藏 Start 按鈕
                    showMaldives = true; // 顯示 Maldives 圖片
                } else if (checkButtonClick(&dialogButton, x, y)) {
                    // 切換對話框文字
                    if (strcmp(dialogButton.text, "this is dialog box") == 0) {
                        dialogButton.text = "you can change text box here";
                    } else {
                        dialogButton.text = "this is dialog box";
                    }
                    // 更新按鈕紋理
                    SDL_DestroyTexture(dialogButton.texture);
                    SDL_Surface* surface = TTF_RenderText_Solid(font, dialogButton.text, (SDL_Color){255, 255, 255, 255});
                    dialogButton.texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);
                }
            }
        }

        // 清除屏幕
        SDL_RenderClear(renderer);

        // 渲染背景圖片
        if (!showMaldives) {
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        }

        // 渲染 Maldives 圖片
        if (showMaldives) {
            SDL_RenderCopy(renderer, maldivesTexture, NULL, NULL);
        }

        // 渲染按鈕
        renderButton(&startButton, renderer);
        renderButton(&dialogButton, renderer);

        // 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // 釋放資源
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyTexture(maldivesTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(startButton.texture);
    SDL_DestroyTexture(dialogButton.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

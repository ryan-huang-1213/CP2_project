#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

// 按鈕結構
typedef struct {
    SDL_Rect rect; // 按鈕的矩形區域
    bool pressed;  // 按鈕是否被按下
    bool shown;    // 圖片是否已顯示
} Button;

int main() {

    uint64_t counter=0;

    // 初始化 SDL，創建視窗和渲染器
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // 設置背景顏色為白色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // 創建按鈕
    Button startButton;
    startButton.rect.x = 350;
    startButton.rect.y = 250;
    startButton.rect.w = 100;
    startButton.rect.h = 50;
    startButton.pressed = false;
    startButton.shown = false;
    
    // 主循環
    bool running = true;
    SDL_Event event;
    while (running) {
        // 處理事件
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT &&
                    event.button.x >= startButton.rect.x &&
                    event.button.x <= (startButton.rect.x + startButton.rect.w) &&
                    event.button.y >= startButton.rect.y &&
                    event.button.y <= (startButton.rect.y + startButton.rect.h)) {
                    if (!startButton.pressed) {
                        startButton.pressed = true;
                    } else if (!startButton.shown) {
                        startButton.shown = true;
                    } else {
                        startButton.pressed = false;
                        startButton.shown = false;
                    }
                }
            }
        }
    
        // 渲染背景
        SDL_RenderClear(renderer);
    
        // 渲染按鈕
        if (!startButton.pressed || startButton.shown) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 藍色按鈕
            SDL_RenderFillRect(renderer, &startButton.rect);
        }
        
        // 如果按鈕被按下且圖片未顯示，則顯示圖片
        if (startButton.pressed && !startButton.shown) {
            // 加載並顯示圖片
            SDL_Surface* bmpSurface = SDL_LoadBMP("source/image/maldives.bmp");
            SDL_Texture* bmpTexture = SDL_CreateTextureFromSurface(renderer, bmpSurface);
            SDL_FreeSurface(bmpSurface); // 釋放表面資源
            SDL_Rect bmpRect = {0, 0, 800, 600}; // 圖片將覆蓋整個視窗
            SDL_RenderCopy(renderer, bmpTexture, NULL, &bmpRect);
            SDL_DestroyTexture(bmpTexture); // 釋放紋理資源
        } else {
            // 設置背景顏色為白色
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
    
        // 渲染按鈕
        if (!startButton.pressed || startButton.shown) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 藍色按鈕
            SDL_RenderFillRect(renderer, &startButton.rect);
            // 設置渲染顏色回背景顏色
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

    
        // 顯示渲染結果
        SDL_RenderPresent(renderer);
        // printf("time: %ld \n",counter);
        // counter++;
    }
    
    // 清理資源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

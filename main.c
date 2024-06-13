#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define TEXT_PATH "./scurce/text/Arial.ttf"

// 定義遊戲狀態結構
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *backgroundTexture;
    SDL_Rect dialogueBox;
    const char *dialogueText;
    int dialogueState;
} GameState;

// 函數原型
void initializeGame(GameState *gameState);
void startGame(GameState *gameState);
void runGame(GameState *gameState);
void endGame(GameState *gameState);
void changeDialogue(GameState *gameState);

int main(int argc, char* argv[]) {
    GameState gameState;

    initializeGame(&gameState);
    startGame(&gameState);
    // 遊戲主循環
    runGame(&gameState);
    endGame(&gameState);

    return 0;
}

// 初始化遊戲，創建窗口和渲染器
void initializeGame(GameState *gameState) {
    SDL_Init(SDL_INIT_VIDEO); // 初始化SDL
    gameState->window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    gameState->renderer = SDL_CreateRenderer(gameState->window, -1, SDL_RENDERER_ACCELERATED);
    gameState->dialogueState = 0; // 對話狀態初始化
}

void startGame(GameState *gameState) {
    // 加載背景圖像
    SDL_Surface *backgroundSurface = IMG_Load("source/image/tmp_background.jpg");
    if (!backgroundSurface) {
        printf("無法加載背景圖像: %s\n", IMG_GetError());
        // 處理錯誤...
    }
    gameState->backgroundTexture = SDL_CreateTextureFromSurface(gameState->renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface); // 不再需要表面

    // 渲染背景紋理
    SDL_RenderCopy(gameState->renderer, gameState->backgroundTexture, NULL, NULL);

    // 設置按鈕位置和大小
    SDL_Rect startButton = {350, 450, 100, 50};
    // 設置按鈕顏色為藍色
    SDL_SetRenderDrawColor(gameState->renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(gameState->renderer, &startButton);

    // 重新設置渲染器的繪製顏色為背景顏色
    SDL_SetRenderDrawColor(gameState->renderer, 255, 255, 255, 255);

    // 初始化TTF並創建文字紋理
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(TEXT_PATH, 24); // 需要有arial.ttf字體文件
    SDL_Color textColor = {255, 255, 255, 255}; // 白色文字
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Start", textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gameState->renderer, textSurface);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);

    // 獲取文字紋理的寬度和高度
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // 設置文字位置（居中於按鈕）
    SDL_Rect textRect = {startButton.x + (startButton.w - textWidth) / 2,
                         startButton.y + (startButton.h - textHeight) / 2,
                         textWidth, textHeight};

    // 渲染文字
    SDL_RenderCopy(gameState->renderer, textTexture, NULL, &textRect);

    // 顯示初始畫面
    SDL_RenderPresent(gameState->renderer);

     // 顯示初始畫面
    SDL_RenderPresent(gameState->renderer);

    // 等待用戶按下按鈕
    SDL_Event event;
    int startButtonPressed = 0;
    while (!startButtonPressed) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                startButtonPressed = 1; // 如果用戶關閉窗口，也結束循環
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                // 檢查是否點擊了開始按鈕
                if (mouseX > startButton.x && mouseX < startButton.x + startButton.w &&
                    mouseY > startButton.y && mouseY < startButton.y + startButton.h) {
                    startButtonPressed = 1;
                    printf("Button pressed (start)\n");
                }
            }
            // 每次循環都更新屏幕
            SDL_RenderPresent(gameState->renderer);
            SDL_Delay(100); // 防止CPU佔用過高
        }
    }

    // 清理
    SDL_DestroyTexture(textTexture);
    TTF_Quit();
}


// 遊戲主循環，處理事件和更新遊戲狀態
void runGame(GameState *gameState) {
    SDL_Event event;
    int gameRunning = 1;

    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = 0;
            }
            // 更多事件處理...
        }

        // 更新遊戲狀態
        changeDialogue(gameState);

        // 渲染遊戲畫面
        SDL_RenderClear(gameState->renderer);
        // ...渲染背景和對話框...
        SDL_RenderPresent(gameState->renderer);

        // 控制遊戲更新的速度
        SDL_Delay(16); // 約60FPS
    }
    // ...遊戲主循環代碼...

    // 渲染對話框和文字
    // 假設對話框紋理已經創建
    SDL_RenderCopy(gameState->renderer, gameState->dialogueText, NULL, &gameState->dialogueBox);
    // ...其他渲染代碼...
    SDL_RenderPresent(gameState->renderer);
}


void endGame(GameState *gameState) {
    // 保持結束畫面直到用戶關閉視窗
    SDL_Event event;
    int gameRunning = 1;
    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = 0;
            }
        }
        // 可以在這裡添加一些結束畫面的渲染代碼
        SDL_RenderPresent(gameState->renderer);
        SDL_Delay(16); // 約60FPS
    }

    // 釋放資源
    SDL_DestroyTexture(gameState->backgroundTexture);
    SDL_DestroyRenderer(gameState->renderer);
    SDL_DestroyWindow(gameState->window);
    SDL_Quit();
}

// 根據用戶互動更改對話框中的文字
void changeDialogue(GameState *gameState) {
    const char *dialogues[] = {"good morning", "good afternoon", "good night"};
    gameState->dialogueText = dialogues[gameState->dialogueState % 3];
    gameState->dialogueState++;

    // 創建文字紋理
    TTF_Font *font = TTF_OpenFont(TEXT_PATH, 24); // 確保字體文件路徑正確
    SDL_Color textColor = {0, 0, 0, 255}; // 黑色文字
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, gameState->dialogueText, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gameState->renderer, textSurface);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);

    // 設置對話框位置和尺寸
    gameState->dialogueBox = (SDL_Rect){100, 500, 600, 100}; // 示例對話框位置和尺寸

    // 渲染對話框和文字紋理
    SDL_RenderCopy(gameState->renderer, textTexture, NULL, &gameState->dialogueBox);

    // 清理文字紋理
    SDL_DestroyTexture(textTexture);
}

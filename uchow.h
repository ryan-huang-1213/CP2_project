#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

void story(int32_t choice);
void END();
void print_out();
void scenes ()

typedef struct _backpack
{
    int32_t nopicture;
    int32_t picture;
    int32_t airpods;
    int32_t donpaper;
}Backpack;

typedef struct _like
{
    int32_t Don;
    int32_t Becca;
    int32_t Street;
    int32_t Police;
    int32_t girl;
}Like;

Like like;
Backpack backpack;

int32_t Becca;
int32_t end;
int32_t wrong;
int32_t line1;



#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define DIALOG_HEIGHT (WINDOW_HEIGHT / 3) // 對話按鈕的高度
#define TEXT_PATH "source/text/cjkfonts_handingwriting4.ttf"
#define MAX_SCENES 3
#define MAX_DIALOGS 4

typedef struct {
    char* path; // 圖片的相對路徑
    SDL_Texture* texture;
    SDL_Rect rect;
} Asset;

typedef struct {
    char* text; // 選項的文字敘述 
    SDL_Texture* texture;
    SDL_Rect rect;
    int nextSceneIndex; // 跳轉至下一個頁面 Scene[?]
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

char background[100];
char character[100];
char caption[1000];
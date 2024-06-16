#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define DIALOG_HEIGHT (WINDOW_HEIGHT / 3) // 對話按鈕的高度
#define TEXT_PATH "source/text/cjkfonts_handingwriting4.ttf"
#define MAX_SCENES 43
#define MAX_DIALOGS 10
#define MAX_ITEMS 4

typedef enum {
    GAME_STATE_START_SCREEN,
    GAME_STATE_RUNNING,
    GAME_STATE_END_SCREEN
} GameState;

typedef struct _backpack {
    char itemName[256];
    char itemImagePath[256]; // 道具圖片的路徑
    char itemDescriptionPath[256]; // 道具描述文本的路徑
    int totalItems;
    int have;
    SDL_Texture* itemTexture; // 道具圖片的紋理
    SDL_Rect itemRect; // 道具圖片的矩形區域
} Backpack;

typedef struct _like {
    char name[256];
    int32_t value;
    int32_t totalLike; // 新增的欄位
} Like;

/*
Like like;
Backpack backpack;
*/
/*
int32_t Becca;
int32_t end;
int32_t wrong;
int32_t line1;
*/

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
    char* speaker;
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
    int nextSceneIndex;
} Scene;
/*
char background[100] = "source/image/maldives.bmp";
char character[100];
char caption[1000];
*/
// void story(int32_t choice ,Scene* scenes);

void load_story(Scene scenes[]);
void loadFromFile(const char *filename, Scene scenes[], int *totalScenes);
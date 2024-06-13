#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TEXT_PATH "Arial.ttf"
#define MAX_IMAGES 3
#define MAX_DIALOGS 3

typedef struct {
    char* path;
    SDL_Texture* texture;
} Image;

typedef struct {
    char* text;
    char* speaker;
    SDL_Texture* texture;
    SDL_Rect rect;
} Dialog;

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

    if (!initSDL(&window, &renderer)) {
        printf("Failed to initialize!\n");
    } else {
        if (!loadMedia(renderer, images, dialogs, &font)) {
            printf("Failed to load media!\n");
        } else {
            bool running = true;
            SDL_Event e;
            while (running) {
                handleEvents(&e, &running, &currentImageIndex, &currentDialogIndex, MAX_IMAGES, MAX_DIALOGS);
                render(renderer, images, dialogs, currentImageIndex, currentDialogIndex);
            }
        }
    }

    closeSDL(window, renderer, images, dialogs, font);
    return 0;
}

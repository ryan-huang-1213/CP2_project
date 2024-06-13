#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TEXT_PATH "source/text/cjkfonts_handingwriting4.ttf" // "./source/text/Arial.ttf"
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

// Initialize SDL
bool initSDL(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    // Create window
    *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    // Create renderer for window
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }
    // Set renderer draw color (for button background)
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 204); // Black with 80% opacity
    return true;
}

// Close SDL and free resources
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Image images[], Dialog dialogs[], TTF_Font* font) {
    // Destroy textures for images and dialogs
    for (int i = 0; i < MAX_IMAGES; i++) {
        SDL_DestroyTexture(images[i].texture);
    }
    for (int i = 0; i < MAX_DIALOGS; i++) {
        SDL_DestroyTexture(dialogs[i].texture);
    }
    // Close and destroy the font
    TTF_CloseFont(font);
    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// Load media including images and dialog textures
bool loadMedia(SDL_Renderer* renderer, Image images[], Dialog dialogs[], TTF_Font** font) {
    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    // Initialize TTF
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    // Load images
    for (int i = 0; i < MAX_IMAGES; i++) {
        SDL_Surface* loadedSurface = IMG_Load(images[i].path);
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", images[i].path, IMG_GetError());
            return false;
        }
        images[i].texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    // Open the font
    *font = TTF_OpenFont(TEXT_PATH, 24); // Font size is set to 24px
    if (*font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    // Create textures for dialogs
    for (int i = 0; i < MAX_DIALOGS; i++) {
        updateDialogTexture(renderer, *font, &dialogs[i]);
    }
    return true;
}

// Handle events, including button clicks to change dialog and images
void handleEvents(SDL_Event* e, bool* running, int* currentImageIndex, int* currentDialogIndex, int totalImages, int totalDialogs) {
    while (SDL_PollEvent(e) != 0) {
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            // Check if the dialog button is clicked
            if (y > WINDOW_HEIGHT - WINDOW_HEIGHT / 3) {
                *currentDialogIndex = (*currentDialogIndex + 1) % totalDialogs;
                if (*currentDialogIndex == 0) { // If it's the last dialog
                    *currentImageIndex = (*currentImageIndex + 1) % totalImages;
                }
            }
        }
    }
}

// Render the current image and dialog button with 80% transparent black background
void render(SDL_Renderer* renderer, Image images[], Dialog dialogs[], int currentImageIndex, int currentDialogIndex) {
    // Clear the screen
    SDL_RenderClear(renderer);

    // Render the current image
    SDL_RenderCopy(renderer, images[currentImageIndex].texture, NULL, NULL);

    // Set the draw color to 80% transparent black for the button background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 204); // 80% opacity
    // Draw the button background
    SDL_Rect buttonRect = {0, WINDOW_HEIGHT - WINDOW_HEIGHT / 3, WINDOW_WIDTH, WINDOW_HEIGHT / 3};
    SDL_RenderFillRect(renderer, &buttonRect);

    // Render the dialog button text
    SDL_RenderCopy(renderer, dialogs[currentDialogIndex].texture, NULL, &dialogs[currentDialogIndex].rect);

    // Update the screen
    SDL_RenderPresent(renderer);
}

// Update the texture for the dialog button with 80% transparent black background
void updateDialogTexture(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog) {
    SDL_Color textColor = {255, 255, 255}; // White color for text
    SDL_Color backgroundColor = {0, 0, 0, 204}; // 80% transparent black for background
    SDL_Surface* textSurface = TTF_RenderText_Shaded(font, dialog->text, textColor, backgroundColor);
    dialog->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    // Center the text box vertically and horizontally within the button
    dialog->rect.x = (WINDOW_WIDTH - textWidth) / 2; // Center horizontally
    dialog->rect.y = WINDOW_HEIGHT - (WINDOW_HEIGHT / 3) / 2 - textHeight / 2; // Center vertically
    dialog->rect.w = textWidth;
    dialog->rect.h = textHeight;
}

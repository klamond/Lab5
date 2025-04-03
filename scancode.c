#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

// PS/2 scan codes from ps2dev.h
typedef enum {
    ESCAPE = 0x76,
    F1 = 0x05,
    F2 = 0x06,
    F3 = 0x04,
    F4 = 0x0c,
    F5 = 0x03,
    F6 = 0x0b,
    F7 = 0x83,
    F8 = 0x0a,
    F9 = 0x01,
    F10 = 0x09,
    F11 = 0x78,
    F12 = 0x07,
    SCROLL_LOCK = 0x7e,
    ACCENT = 0x0e,
    ONE = 0x16,
    TWO = 0x1e,
    THREE = 0x26,
    FOUR = 0x25,
    FIVE = 0x2e,
    SIX = 0x36,
    SEVEN = 0x3d,
    EIGHT = 0x3e,
    NINE = 0x46,
    ZERO = 0x45,
    MINUS = 0x4e,
    EQUAL = 0x55,
    BACKSPACE = 0x66,
    TAB = 0x0d,
    Q = 0x15,
    W = 0x1d,
    E = 0x24,
    R = 0x2d,
    T = 0x2c,
    Y = 0x35,
    U = 0x3c,
    I = 0x43,
    O = 0x44,
    P = 0x4d,
    OPEN_BRACKET = 0x54,
    CLOSE_BRACKET = 0x5b,
    BACKSLASH = 0x5d,
    CAPS_LOCK = 0x58,
    A = 0x1c,
    S = 0x1b,
    D = 0x23,
    F = 0x2b,
    G = 0x34,
    H = 0x33,
    J = 0x3b,
    K = 0x42,
    L = 0x4b,
    SEMI_COLON = 0x4c,
    TICK_MARK = 0x52,
    ENTER = 0x5a,
    LEFT_SHIFT = 0x12,
    Z = 0x1a,
    X = 0x22,
    C = 0x21,
    V = 0x2a,
    B = 0x32,
    N = 0x31,
    M = 0x3a,
    COMMA = 0x41,
    PERIOD = 0x49,
    SLASH = 0x4a,
    RIGHT_SHIFT = 0x59,
    LEFT_CONTROL = 0x14,
    LEFT_ALT = 0x11,
    SPACE = 0x29,
    NUM_LOCK = 0x77,
    ASTERISK = 0x7c,
    NUMPAD_MINUS = 0x7b,
    NUMPAD_SEVEN = 0x6c,
    NUMPAD_EIGHT = 0x75,
    NUMPAD_NINE = 0x7d,
    PLUS = 0x79,
    NUMPAD_FOUR = 0x6b,
    NUMPAD_FIVE = 0x73,
    NUMPAD_SIX = 0x74,
    NUMPAD_ONE = 0x69,
    NUMPAD_TWO = 0x72,
    NUMPAD_THREE = 0x7a,
    NUMPAD_ZERO = 0x70,
    DECIMAL = 0x71
} ScanCodes;

typedef enum {
    LEFT_WIN = 0x1f,
    RIGHT_ALT = 0x11,
    RIGHT_WIN = 0x27,
    MENUS = 0x2f,
    RIGHT_CONTROL = 0x14,
    INSERT = 0x70,
    HOME = 0x6c,
    PAGE_UP = 0x7d,
    DELETE = 0x71,
    END = 0x69,
    PAGE_DOWN = 0x7a,
    UP_ARROW = 0x75,
    LEFT_ARROW = 0x6b,
    DOWN_ARROW = 0x72,
    RIGHT_ARROW = 0x74,
    DIVIDE = 0x4a,
    NUMPAD_ENTER = 0x5a,
    NEXT_TRACK = 0x4d,
    PREVIOUS_TRACK = 0x15,
    STOP = 0x3b,
    PLAY_PAUSE = 0x34,
    MUTE = 0x23,
    VOLUME_UP = 0x32,
    VOLUME_DOWN = 0x21,
    MEDIA_SELECT = 0x50,
    EMAIL = 0x48,
    CALCULATOR = 0x2b,
    MY_COMPUTER = 0x40,
    WWW_SEARCH = 0x10,
    WWW_HOME = 0x3a,
    WWW_BACK = 0x38,
    WWW_FORWARD = 0x30,
    WWW_STOP = 0x28,
    WWW_REFRESH = 0x20,
    WWW_FAVORITES = 0x18,
    POWER = 0x5e,
    SLEEP = 0x5f,
    WAKE = 0x63
} SpecialScanCodes;

typedef struct {
    const char* label;
    int scancode;
    int special;
    SDL_Rect rect;
    SDL_Color color;
} Key;

// Function prototypes
void init_keys(Key* keys, int* key_count);
void render_keyboard(SDL_Renderer* renderer, Key* keys, int key_count);
void handle_click(int x, int y, Key* keys, int key_count);

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("PS/2 Virtual Keyboard Scanner",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize keys
    Key keys[100];
    int key_count = 0;
    init_keys(keys, &key_count);

    // Main loop
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    handle_click(e.button.x, e.button.y, keys, key_count);
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderClear(renderer);

        // Render keyboard
        render_keyboard(renderer, keys, key_count);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void init_keys(Key* keys, int* key_count) {
    // Function keys
    keys[(*key_count)++] = (Key){"Esc", ESCAPE, 0, {10, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F1", F1, 0, {80, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F2", F2, 0, {150, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F3", F3, 0, {220, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F4", F4, 0, {290, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F5", F5, 0, {360, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F6", F6, 0, {430, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F7", F7, 0, {500, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F8", F8, 0, {570, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F9", F9, 0, {640, 10, 60, 60}, {200, 200, 200, 255}};
    keys[(*key_count)++] = (Key){"F10", F10, 0, {710, 10, 60, 60}, {200, 200, 200, 255}};

    // Main keyboard - first row
    keys[(*key_count)++] = (Key){"`", ACCENT, 0, {10, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"1", ONE, 0, {80, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"2", TWO, 0, {150, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"3", THREE, 0, {220, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"4", FOUR, 0, {290, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"5", FIVE, 0, {360, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"6", SIX, 0, {430, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"7", SEVEN, 0, {500, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"8", EIGHT, 0, {570, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"9", NINE, 0, {640, 80, 60, 60}, {255, 255, 255, 255}};
    keys[(*key_count)++] = (Key){"0", ZERO, 0, {710, 80, 60, 60}, {255, 255, 255, 255}};

    // Add more keys here following the same pattern...
    // For brevity, I've only included the first few rows
    
    // Special keys (example)
    keys[(*key_count)++] = (Key){"Up", UP_ARROW, 1, {600, 300, 60, 60}, {200, 200, 255, 255}};
    keys[(*key_count)++] = (Key){"Left", LEFT_ARROW, 1, {530, 370, 60, 60}, {200, 200, 255, 255}};
    keys[(*key_count)++] = (Key){"Down", DOWN_ARROW, 1, {600, 370, 60, 60}, {200, 200, 255, 255}};
    keys[(*key_count)++] = (Key){"Right", RIGHT_ARROW, 1, {670, 370, 60, 60}, {200, 200, 255, 255}};
}

void render_keyboard(SDL_Renderer* renderer, Key* keys, int key_count) {
    SDL_Color textColor = {0, 0, 0, 255};
    
    for (int i = 0; i < key_count; i++) {
        Key key = keys[i];
        
        // Draw key background
        SDL_SetRenderDrawColor(renderer, key.color.r, key.color.g, key.color.b, key.color.a);
        SDL_RenderFillRect(renderer, &key.rect);
        
        // Draw key border
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawRect(renderer, &key.rect);
        
        // Draw key label (simplified - in a real app you'd use SDL_ttf)
        SDL_Surface* surface = SDL_CreateRGBSurface(0, 100, 50, 32, 0, 0, 0, 0);
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, key.color.r, key.color.g, key.color.b));
        // Note: For actual text rendering, you'd need SDL_ttf library
        // This is just a placeholder to show where the label would go
        SDL_Rect textRect = {key.rect.x + 10, key.rect.y + 20, key.rect.w - 20, 20};
        SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderFillRect(renderer, &textRect);
        SDL_FreeSurface(surface);
    }
}

void handle_click(int x, int y, Key* keys, int key_count) {
    for (int i = 0; i < key_count; i++) {
        SDL_Rect rect = keys[i].rect;
        if (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h) {
            if (keys[i].special) {
                printf("Special Key: %s, Scan Code: 0xE0 0x%02X\n", keys[i].label, keys[i].scancode);
            } else {
                printf("Key: %s, Scan Code: 0x%02X\n", keys[i].label, keys[i].scancode);
            }
            break;
        }
    }
}
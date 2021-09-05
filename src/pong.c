#include "pong.h"

const int WIDTH = 640;
const int HEIGHT = 480;

int served = 0;

Ball ball;

Player player1;
Player player2;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main() {
    srand((unsigned int)time(NULL));
    atexit(Shutdown);

    if (!Initialize()) {
        exit(1);
    }

    int quit = 0;
    SDL_Event event;

    Uint32 lastTick = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // get frame information to
        // keep it consistent
        Uint32 curTick = SDL_GetTicks();
        Uint32 diff = curTick - lastTick;
        float elapsed = diff / 1000.0f;
        Update(elapsed);
        lastTick = curTick;
    }

    return 0;
}

int Initialize() {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize the SDL2 library!\n");
        return 0;
    }

    // create window
    window = SDL_CreateWindow("Wow SDL",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Failed to create window\n");
        return 0;
    }

    // create renderer
    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        printf("Failed to create renderer\n");
        return 0;
    }

    ball = MakeBall();
    player1 = MakePlayer();
    player2 = MakePlayer();

    return 1;
}

void Update(float elapsed) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    UpdateBall(served, &ball, elapsed);
    RenderBall(&ball);

    UpdatePlayers(elapsed);
    RenderPlayers();

    SDL_RenderPresent(renderer);
}

void Shutdown() {
    if (renderer) {
        // close and destory the renderer
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        // close and destroy the window
        SDL_DestroyWindow(window);
    }

    // quit SDL
    SDL_Quit();
}

int CoinFlip() {
    return rand() % 2 == 1 ? 1 : 0;
}

void UpdateScore(int player, int points) {
    served = 0;

    if (player == 1) {
        player1.score += points;
    } else {
        player2.score += points;
    }

    char *fmt = "P1 %d | P2 %d";
    int len = snprintf(NULL, 0, fmt, player1.score, player2.score);
    char buf[len + 1];
    snprintf(buf, len + 1, fmt, player1.score, player2.score);
    SDL_SetWindowTitle(window, buf);
}

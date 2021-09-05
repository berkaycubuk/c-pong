#include "pong.h"

const int WIDTH = 640;
const int HEIGHT = 480;
const int BALL_SIZE = 10;

typedef struct Ball {
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;
} Ball;

typedef struct Player {
    int score;
    float yPosition;
} Player;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 75;
const int PLAYER_MARGIN = 10;
const float PLAYER_MOVE_SPEED = 150.0f;

int served = 0;

Ball ball;

Player player1;
Player player2;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int Initialize();
void Update(float elapsed);
void Shutdown();

int CoinFlip();

Ball MakeBall(int size);
void UpdateBall(Ball * ball, float elapsed);
void RenderBall(const Ball *ball);

Player MakePlayer();
void UpdatePlayers(float elapsed);
void RenderPlayers();

void UpdateScore(int player, int points);

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

    ball = MakeBall(BALL_SIZE);
    player1 = MakePlayer();
    player2 = MakePlayer();

    return 1;
}

void Update(float elapsed) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    UpdateBall(&ball, elapsed);
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

Ball MakeBall(int size) {
    const float SPEED = 120;
    Ball ball = {
        .x = WIDTH / 2 - size / 2,
        .y = HEIGHT / 2 - size / 2,
        .size = size,
        .xSpeed = SPEED * (CoinFlip() ? 1 : -1),
        .ySpeed = SPEED * (CoinFlip() ? 1 : -1),
    };

    return ball;
}

void RenderBall(const Ball *ball) {
    int size = ball->size;
    int halfSize = size / 2;
    SDL_Rect rect = {
        .x = ball->x - halfSize,
        .y = ball->y - halfSize,
        .w = size,
        .h = size,
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void UpdateBall(Ball * ball, float elapsed) {
    if (!served) {
        ball->x = WIDTH / 2;
        ball->y = HEIGHT / 2;
        return;
    }

    ball->x += ball->xSpeed * elapsed;
    ball->y += ball->ySpeed * elapsed;

    if (ball->x < BALL_SIZE / 2) {
        // ball->xSpeed = fabs(ball->xSpeed);
        UpdateScore(2, 1);
    }

    if (ball->x > WIDTH - BALL_SIZE / 2) {
        // ball->xSpeed = -fabs(ball->xSpeed);
        UpdateScore(1, 1);
    }

    if (ball->y < BALL_SIZE / 2) {
        ball->ySpeed = fabs(ball->ySpeed);
    }

    if (ball->y > HEIGHT - BALL_SIZE / 2) {
        ball->ySpeed = -fabs(ball->ySpeed);
    }
}

Player MakePlayer() {
    Player player = {
        .yPosition = HEIGHT / 2,
    };
    return player;
}

void UpdatePlayers(float elapsed) {
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_SPACE]) {
        served = 1;
    }

    // player 1 movement
    if (keyboardState[SDL_SCANCODE_W]) {
        player1.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }

    if (keyboardState[SDL_SCANCODE_S]) {
        player1.yPosition += PLAYER_MOVE_SPEED * elapsed;
    }

    if (player1.yPosition < PLAYER_HEIGHT / 2) {
        player1.yPosition = PLAYER_HEIGHT / 2;
    }

    if (player1.yPosition > HEIGHT - PLAYER_HEIGHT / 2) {
        player1.yPosition = HEIGHT - PLAYER_HEIGHT / 2;
    }

    // player 2 movement
    if (keyboardState[SDL_SCANCODE_UP]) {
        player2.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }

    if (keyboardState[SDL_SCANCODE_DOWN]) {
        player2.yPosition += PLAYER_MOVE_SPEED * elapsed;
    }

    if (player2.yPosition < PLAYER_HEIGHT / 2) {
        player2.yPosition = PLAYER_HEIGHT / 2;
    }

    if (player2.yPosition > HEIGHT - PLAYER_HEIGHT / 2) {
        player2.yPosition = HEIGHT - PLAYER_HEIGHT / 2;
    }

    // check if the ball rct overlaps with either player rect
    SDL_Rect ballRect = {
        .x = ball.x - ball.size / 2,
        .y = ball.y - ball.size / 2,
        .w = ball.size,
        .h = ball.size,
    };

    SDL_Rect player1Rect = {
        .x = PLAYER_MARGIN,
        .y = (int)(player1.yPosition) - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };

    if (SDL_HasIntersection(&ballRect, &player1Rect)) {
        // change ball direction to right
        ball.xSpeed = fabs(ball.xSpeed);
    }

    SDL_Rect player2Rect = {
        .x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
        .y = (int)(player2.yPosition) - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };

    if (SDL_HasIntersection(&ballRect, &player2Rect)) {
        // change ball direction to left
        ball.xSpeed = -fabs(ball.xSpeed);
    }
}

void RenderPlayers() {
    // render player 1 | left side
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect player1Rect = {
        .x = PLAYER_MARGIN,
        .y = (int)(player1.yPosition) - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &player1Rect);

    // render player 2 | right side
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect player2Rect = {
        .x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
        .y = (int)(player2.yPosition) - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &player2Rect);
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

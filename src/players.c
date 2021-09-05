#include "pong.h"

extern const int WIDTH;
extern const int HEIGHT;
extern int served;
extern Ball ball;
extern Player player1;
extern Player player2;
extern SDL_Renderer *renderer;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 75;
const int PLAYER_MARGIN = 10;
const float PLAYER_MOVE_SPEED = 150.0f;

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

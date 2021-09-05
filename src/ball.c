#include "pong.h"

extern const int WIDTH;
extern const int HEIGHT;
extern SDL_Renderer *renderer;

const int BALL_SIZE = 10;

Ball MakeBall(int size) {
    const float SPEED = 120;
    Ball ball = {
        .x = WIDTH / 2 - BALL_SIZE / 2,
        .y = HEIGHT / 2 - BALL_SIZE / 2,
        .size = BALL_SIZE,
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

void UpdateBall(int served, Ball * ball, float elapsed) {
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

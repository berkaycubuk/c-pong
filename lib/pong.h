#ifndef CELESTIAL_H
#define CELESTIAL_H

#include <SDL2/SDL.h>

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

int Initialize();
void Update(float elapsed);
void Shutdown();

int CoinFlip();

Ball MakeBall();
void UpdateBall(int served, Ball * ball, float elapsed);
void RenderBall(SDL_Renderer *renderer, const Ball *ball);

Player MakePlayer();
void UpdatePlayers(float elapsed);
void RenderPlayers();

void UpdateScore(int player, int points);

#endif

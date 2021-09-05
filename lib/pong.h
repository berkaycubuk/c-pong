#ifndef PONG_H
#define PONG_H

#include <SDL2/SDL.h>
#include "ball.h"
#include "players.h"

int Initialize();
void Update(float elapsed);
void Shutdown();

int CoinFlip();

void UpdateScore(int player, int points);

#endif

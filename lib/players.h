#ifndef PLAYERS_H
#define PLAYERS_H

typedef struct Player {
    int score;
    float yPosition;
} Player;

Player MakePlayer();
void UpdatePlayers(float elapsed);
void RenderPlayers();

#endif

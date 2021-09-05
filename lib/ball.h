#ifndef BALL_H
#define BALL_H

typedef struct Ball {
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;
} Ball;

Ball MakeBall();
void UpdateBall(int served, Ball * ball, float elapsed);
void RenderBall(const Ball *ball);

#endif

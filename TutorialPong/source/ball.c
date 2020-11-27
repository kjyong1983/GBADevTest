#include <tonc.h>
#include "ball.h"
#include "tool.h"

void setBallDir(Ball* ball){
    while (ball->dirX == 0)
    {
        ball->dirX = rand_range(-1, 2);
    }
    ball->dirY = rand_range(-1, 2);
}

void initBall(Ball* ball, s32 x, s32 y, s32 size, u16 color){
    ball->x = x;
    ball->y = y;
    ball->size = size;
    ball->color = color;
    ball->dirX = ball-> dirY = 0;
    setBallDir(ball);
}

void updateBall(Ball* ball){
    ball->y += ball->dirY;

    if (ball->y < 0)
    {
        ball->y = 0;
        ball->dirY *= -1;
    }
    if (ball->y > SCREEN_HEIGHT - ball->size)
    {
        ball->y = SCREEN_HEIGHT - ball->size;
        ball->dirY *= -1;
    }

    ball->x += ball->dirX;
    if (ball->x < 0 || ball-> x > SCREEN_WIDTH - ball->size)
    {
        resetBall(ball);
    }
}

void resetBall(Ball* ball){
    initBall(ball, SCREEN_WIDTH / 2 - ball->size / 2, SCREEN_HEIGHT / 2 - ball->size / 2, ball->size, ball->color);
}

void clearBall(Ball* ball){
    drawRect(ball->x, ball->y, ball->size, ball->size, color(0,0,0));
}

void drawBall(Ball* ball){
    drawRect(ball->x, ball->y, ball->size, ball->size, ball->color);
}
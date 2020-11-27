#include <tonc.h>
#include "paddle.h"
#include "tool.h"

void initPaddle(Paddle* paddle, s32 x, s32 y, s32 w, s32 h, u16 color){
    paddle->x = x;
    paddle->y = y;
    paddle->w = w;
    paddle->h = h;
    paddle->color = color;
}

void updatePaddle(Paddle* paddle, s32 dy){
    paddle->y += dy;
    if (paddle->y < 0)
    {
        paddle->y = 0;
    }
    if (paddle->y > SCREEN_HEIGHT - paddle->h)
    {
        paddle->y = SCREEN_HEIGHT - paddle->h;
    }
}

void clearPaddle(Paddle* paddle){
    drawRect(paddle->x, paddle->y, paddle->w, paddle->h, color(0,0,0));
}

void drawPaddle(Paddle* paddle){
    drawRect(paddle->x, paddle->y, paddle->w, paddle->h, paddle->color);
}
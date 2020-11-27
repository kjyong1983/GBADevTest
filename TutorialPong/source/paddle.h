#ifndef PADDLE_H
#define PADDLE_H

#include <tonc.h>

typedef struct Paddle{
    s32 x, y, w, h;
    u16 color;
} Paddle;

extern void initPaddle(Paddle* paddle, s32 x, s32 y, s32 w, s32 h, u16 color);
extern void updatePaddle(Paddle* paddle, s32 dy);
extern void clearPaddle(Paddle* paddle);
extern void drawPaddle(Paddle* paddle);

#endif
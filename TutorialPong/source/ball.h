#ifndef BALL_H
#define BALL_H

#include <tonc.h>

typedef struct Ball{
    s32 x, y, dirX, dirY, size;
    u16 color;
} Ball;

extern void setBallDir(Ball* ball);
extern void initBall(Ball* ball, s32 x, s32 y, s32 size, u16 color);
extern void updateBall(Ball* ball);
extern void resetBall(Ball* ball);
extern void clearBall(Ball* ball);
extern void drawBall(Ball* ball);

#endif
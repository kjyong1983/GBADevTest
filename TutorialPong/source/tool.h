#ifndef TOOL_H
#define TOOL_H

#include <tonc.h>

extern s32 abs(s32 val);
extern u16 color(u8 red, u8 green, u8 blue);
extern void plotPixel(u32 x, u32 y, u16 color);
extern void drawRect(u32 left, u32 top, u32 width, u32 height, u16 color);
extern void drawLine(u32 x1, u32 y1, u32 x2, u32 y2, u16 color);
extern s32 seed_rand(s32 val);
extern s32 rand();
extern s32 rand_range(s32 min, s32 max);
extern s32 sign(s32 val);

#endif
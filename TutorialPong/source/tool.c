#include <tonc.h>
#include "tool.h"

s32 abs(s32 val){
    s32 mask = val >> 31;
    return (val ^ mask) - mask;
}

u16 color(u8 red, u8 green, u8 blue){
    // 0x1f == 31
    return (red & 0x1f) | (green & 0x1f) << 5| (blue & 0x1f) << 10;
}

void plotPixel(u32 x, u32 y, u16 color){
    vid_mem[y * SCREEN_WIDTH + x] = color;
}

void drawRect(u32 left, u32 top, u32 width, u32 height, u16 color){
    for (u32 y = 0; y < height; ++y)
    {
        for (u32 x = 0; x < width; ++x)
        {
            vid_mem[(top + y) * SCREEN_WIDTH + left + x] = color;
        }
    }
}

// bresenham's line drawing algorithm
void drawLine(u32 x1, u32 y1, u32 x2, u32 y2, u16 color){
    s32 w = x2 - x1;
    s32 h = y2 - y1;

    s32 dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

    if (w < 0)
        dx1 = dx2 = -1;
    else if (w > 0)
        dx1 = dx2 = 1;

    if (h < 0)
        dy1 = -1;
    else if (h > 0)
        dy1 = 1;

    s32 longest = abs(w);
    s32 shortest = abs(h);
    
    if (shortest > longest)
    {
        // xor swap
        longest ^= shortest;
        shortest ^= longest;
        longest ^= shortest;

        if (h < 0)
            dy2 = -1;
        else if (h > 0)
            dy2 = 1;
        
        dx2 = 0;
    }
    
    s32 numerator = longest >> 1;
    for (s32 i = 0; i <= longest; ++i)
    {
        plotPixel(x1, y1, color);
        numerator += shortest;
        if (numerator > longest)
        {
            x1 += dx1;
            y1 += dy1;
        }
        else
        {
            x1 += dx2;
            y1 += dy2;
        }
    }
}

s32 rand_seed = 42;

s32 seed_rand(s32 val){
    s32 old = rand_seed;
    rand_seed = val;
    return old;
}

// Linear Congruential Generator (LCG) (Wikipedia: https://en.wikipedia.org/wiki/Linear_congruential_generator)
s32 rand(){
    rand_seed = 1664525 * rand_seed + 1013904223;
    // 0x7fff == 2^15 - 1
    return (rand_seed >> 16) & 0x7fff;
}

s32 rand_range(s32 min, s32 max){
    return (rand()*(max - min) >> 15) + min;
}

s32 sign(s32 val){
    return (!(val & 0x80000000) && !val) ? 1 : -1;
}
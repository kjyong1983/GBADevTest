
#include <tonc.h>
#include "tool.h"
#include "ball.h"
#include "paddle.h"

void vsync(){
	while(REG_VCOUNT >= SCREEN_HEIGHT);
	while(REG_VCOUNT < SCREEN_HEIGHT);
}

int main(){

	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

	seed_rand(14);
	Paddle p1, p2;
	initPaddle(&p1, 10, 60, 8, 40, color(0, 0, 31));
	initPaddle(&p2, SCREEN_WIDTH - 10, 60, 8, 40, color(31, 0, 0));

	Ball ball;
	initBall(&ball, SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 - 5, 10, color(31, 31, 31));

	s16 pDir = 0;

	bool paused = false;

	while (1)
	{
		vsync();

		key_poll();

		clearBall(&ball);
		clearPaddle(&p1);
		clearPaddle(&p2);

		if (!paused)
		{
			updateBall(&ball);

			// apply input
			pDir = 0;

			if (key_is_down(KEY_UP))
			{
				pDir = -2;
			}
			
			if (key_is_down(KEY_DOWN))
			{
				pDir = 2;
			}
			
			updatePaddle(&p1, pDir);

			pDir = 0;

			if (key_is_down(KEY_L))
			{
				pDir = -2;
			}
			
			if (key_is_down(KEY_R))
			{
				pDir = 2;
			}
			
			updatePaddle(&p2, pDir);
		}
		
		if (key_hit(KEY_START))
		{
			paused = !paused;
		}

		drawLine(10, 4, 230, 4, color(31, 31, 31));
		drawLine(230, 156, 10, 156, color(31, 31, 31));
		drawBall(&ball);
		drawPaddle(&p1);
		drawPaddle(&p2);
	}
	
	return 0;
}

// int main()
// {
// 	// Init interrupts and VBlank irq.
// 	irq_init(NULL);
// 	irq_add(II_VBLANK, NULL);
	
// 	// Video mode 0, enable bg 0.
// 	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;
	
// 	// Init 4bpp vwf text on bg 0.
// 	tte_init_chr4c(0, 			// BG 0
// 		BG_CBB(0)|BG_SBB(31),	// Charblock 0; screenblock 31
// 		0xF000,					// Screen-entry offset
// 		bytes2word(1,2,0,0),	// Color attributes.
// 		CLR_YELLOW, 			// Yellow text
// 		&verdana9Font,			// Verdana 9 font
// 		NULL					// Use default chr4 renderer
// 		);
	
// 	// Initialize use of stdio.
// 	tte_init_con();
	
// 	// Printf something at 96,72
// 	tte_printf("#{P:96,72}Hello World!");
	
// 	while(1)
// 	{
// 		VBlankIntrWait();
// 	}
	
// 	return 0;
// }

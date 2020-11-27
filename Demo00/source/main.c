/****************************************/
/*	Test program using tile mode	*/
/****************************************/

#include "gba.h"
hword* map	= (hword*) VRAM_MAP(16);	// Map base address
hword* tile	= (hword*) VRAM_TILE(0);	// Tile base address
hword* color	= (hword*) BG_PALETTE;		// Palette
void waiting(int);

int main() {

	int i, j, k = 0, index;			// index: tile index

	// Background palette color
	color[1] = RGB( 0,  0,  0);	// Black
	color[2] = RGB( 0,  0, 31);	// Blue
	color[3] = RGB( 0, 31,  0);	// Green
	color[4] = RGB( 0, 31, 31);	// Aqua
	color[5] = RGB(31,  0,  0);	// Red
	color[6] = RGB(31,  0, 31);	// Purple
	color[7] = RGB(31, 31,  0);	// Yellow
	color[8] = RGB(31, 31, 31);	// White

	// Tile data
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 32; j++)
			tile[i * 32 + j] = ((i + 1) << 8) + (i + 1);
		}

	// Map data
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 15; j++) {
			index = ((k + i + j) % 8) + 1;
			map[(i * 32 + j) * 2	 ] = index;
			map[(i * 32 + j) * 2 + 1 ] = index;
			map[(i * 32 + j) * 2 + 32] = index;
			map[(i * 32 + j) * 2 + 33] = index;
		}
	}
	k++;

	// Background control register & LCD control register & LCD ON
	gba_reg(BG0_CTL) = 	LCD_SIZE00 |		// 32x32tile
				LCD_COLOR256 |		// 256color
				LCD_BGTILE(0) |		// Tile block 0
				LCD_BGMAP(16);		// Map block 16
	gba_reg(LCD_CTL) = 	LCD_MODE0 | LCD_BG0;	// MODE 0 + BG0 ON

	// Redraw LCD
	while (1) {
		//waiting(128);
		waiting(240*160*4);
		for (i = 0; i <10; i++) {
			for (j = 0; j < 15; j++) {
				index = ((k + i + j) % 8) + 1;
				map[(i * 32 + j) * 2	 ] = index;
				map[(i * 32 + j) * 2 + 1 ] = index;
				map[(i * 32 + j) * 2 + 32] = index;
				map[(i * 32 + j) * 2 + 33] = index;
			}
		}
		k = (k < 15) ? k + 1 : 0;
	}
}

/***	Waiting rootine    ***/
void waiting(int t) {
	int i, j;
	for (i = 0; i < t; i++)
		for (j = 0; j < t; j++);
}
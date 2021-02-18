#include <stdio.h>
#include <string.h>
#include <tonc.h>

// bg
#include "brin.h"

// sprite
#include "mygame.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE*)obj_buffer;

//sprite
int oamCount = 0;

// player info
OBJ_ATTR* playerSprite;
int playerX = SCREEN_WIDTH / 2, playerY = SCREEN_HEIGHT / 2;
int playerSizeX = 16, playerSizeY = 16;
int playerSpeed = 2;
u32 playerTileId = 0, playerPalBank = 0;

// bg info
int bgX = 192, bgY = 64;
int bgScrollSpeed = 1;

// enemy info
// TODO: enemy pooling
OBJ_ATTR* enemySprite;
int enemyX = 10, enemyY = 10;
u32 enemyTileId = 4, enemyPalBank = 0;

// bg
void init_bg(){
	REG_DISPCNT |= DCNT_BG0;
	REG_BG0CNT |= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;

	memcpy(pal_bg_mem, brinPal, brinPalLen);
	memcpy(&tile_mem[0][0], brinTiles, brinTilesLen);
	memcpy(&se_mem[30][0], brinMap, brinMapLen);
}

void update_bg(){
	bgX += bgScrollSpeed * key_tri_horz();
	bgY += bgScrollSpeed * key_tri_vert();

	REG_BG0HOFS = bgX;
	REG_BG0VOFS = bgY;
}

void draw_bg(){

}

void init_sprite(){
	REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;

	memcpy(&tile_mem[4][0], mygameTiles, mygameTilesLen);
	memcpy(pal_obj_mem, mygamePal, mygamePalLen);
	oam_init(obj_buffer, 128);
}

// player sprite
void init_player(){
	// oam index
	playerSprite = &obj_buffer[0];

	obj_set_attr(playerSprite, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(playerPalBank) | playerTileId);
	obj_set_pos(playerSprite, playerX, playerY);

	oamCount++;
}

void clamp_player_position(){
	if (playerX > SCREEN_WIDTH - playerSizeX)
	{
		playerX = SCREEN_WIDTH - playerSizeX;
	}

	if (playerX < 0)
	{
		playerX = 0;
	}

	if (playerY > SCREEN_HEIGHT - playerSizeY)
	{
		playerY = SCREEN_HEIGHT - playerSizeY;
	}

	if (playerY < 0)
	{
		playerY = 0;
	}
}

void update_player(){
	playerX += playerSpeed * key_tri_horz();
	playerY += playerSpeed * key_tri_vert();

	clamp_player_position();
}

void draw_player(){
	obj_set_pos(playerSprite, playerX, playerY);
}

// enemy
void init_enemy(){
	// oam index
	enemySprite = &obj_buffer[1];

	obj_set_attr(enemySprite, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(enemyPalBank) | enemyTileId);
	obj_set_pos(enemySprite, enemyX, enemyY);
	oamCount++;
}

void update_enemy(){
	enemyX += enemyX > playerX ? -1 : 1;
	enemyY += enemyY > playerY ? -1 : 1;
}

void draw_enemy(){
	obj_set_pos(enemySprite, enemyX, enemyY);
}

void init_animation_dummy(){
	
}

void init_bullet_pool(){

}

void draw_sprite(){
	oam_copy(oam_mem, obj_buffer, oamCount);	// only need to update one
}

int main()
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	
	REG_DISPCNT = DCNT_MODE0;

	// // Init 4bpp vwf text on bg 0.
	// tte_init_chr4c(0, 			// BG 0
	// 	BG_CBB(0)|BG_SBB(31),	// Charblock 0; screenblock 31
	// 	0xF000,					// Screen-entry offset
	// 	bytes2word(1,2,0,0),	// Color attributes.
	// 	CLR_YELLOW, 			// Yellow text
	// 	&verdana9Font,			// Verdana 9 font
	// 	NULL					// Use default chr4 renderer
	// 	);
	
	// // Initialize use of stdio.
	// tte_init_con();
	
	// // Printf something at 96,72
	// tte_printf("#{P:96,72}Hello World!");



	init_bg();
	
	init_sprite();
	
	init_player();
	init_enemy();

	init_bullet_pool();

	init_animation_dummy();

	while(1)
	{
		VBlankIntrWait();

		key_poll();

		update_bg();
		update_player();
		update_enemy();

		draw_bg();

		draw_player();
		draw_enemy();

		draw_sprite();
	}

	return 0;
}

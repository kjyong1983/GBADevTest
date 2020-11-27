
#include <stdio.h>
#include <tonc.h>
#include <string.h>

// sprite
#include "pika.h"
#include "colorful.h"
#include "sp_food_8b.h"
#include "Food4.h"
#include "guide.h"

// bg
#include "brin.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE*)obj_buffer;

void sprite_test_pika(){
	REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D;
	
	memcpy(&tile_mem[4][0], pikaTiles, pikaTilesLen);
	memcpy(pal_obj_mem, pikaPal, pikaPalLen);
	oam_init(obj_buffer, 128);

	int x = 96, y = 32;
	u32 tid = 0, pb = 0;

	OBJ_ATTR *pika = &obj_buffer[0];
	obj_set_attr(pika, ATTR0_SQUARE, ATTR1_SIZE_64, ATTR2_PALBANK(pb) | tid);

	obj_set_pos(pika, x, y);

	while(1){
		vid_vsync();
		key_poll();

		// move left/right
		x += 2*key_tri_horz();

		// move up/down
		y += 2*key_tri_vert();

		// flip
		if(key_hit(KEY_A))	// horizontally
			pika->attr1 ^= ATTR1_HFLIP;
		if(key_hit(KEY_B))	// vertically
			pika->attr1 ^= ATTR1_VFLIP;

		// make it glow (via palette swapping)
		pb= key_is_down(KEY_SELECT) ? 1 : 0;

		// toggle mapping mode
		if(key_hit(KEY_START))
			REG_DISPCNT ^= DCNT_OBJ_1D;

		// Hey look, it's one of them build macros!
		pika->attr2= ATTR2_BUILD(tid, pb, 0);
		obj_set_pos(pika, x, y);

		oam_copy(oam_mem, obj_buffer, 1);	// only need to update one
	}
}

void sprite_test_food(){
	REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0;

	memcpy(&tile_mem[4][0], sp_food_8bTiles, sp_food_8bTilesLen);
	memcpy(pal_obj_mem, sp_food_8bPal, sp_food_8bPalLen);
	// memcpy(&tile_mem[4][0], Food4Tiles, Food4TilesLen);
	// memcpy(pal_obj_mem, Food4Pal, Food4PalLen);
	oam_init(obj_buffer, 128);

	int x = 120 - 8;
	int y = 80 - 8;

	u32 tileId = 0, palBank = 0;
	OBJ_ATTR* sprite = &obj_buffer[0];
	obj_set_attr(sprite, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(palBank) | tileId);
	
	// set attr0 to match resource color(4BPP, 8BPP)
	sprite->attr0 |= ATTR0_8BPP;

	while (1)
	{
		vid_vsync();
		key_poll();
		
		if(key_hit(KEY_A))
		{
			tileId += 4 << 1;
			tileId = tileId & 0x1FF;
		}

		sprite->attr2 = ATTR2_BUILD(tileId, palBank, 0);
		obj_set_pos(sprite, x, y);
		oam_copy(oam_mem, obj_buffer, 1);
	}
}

void sprite_test_human(){
	REG_DISPCNT =  DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0;
	int x = 120;
	int y = 80;

	memcpy(&tile_mem[4][0], guideTiles, guideTilesLen);
	memcpy(pal_obj_mem, guidePal, guidePalLen);
	oam_init(obj_buffer, 128);

	u32 tileId = 0, palBank = 0;
	OBJ_ATTR* sprite = &obj_buffer[0];
	
	obj_set_attr(sprite, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(palBank) | tileId);
	sprite->attr0 |= ATTR0_8BPP;

	obj_set_pos(sprite, x, y);

	while (1)
	{
		vid_vsync();
		key_poll();
		
		x += 2*key_tri_horz();
		y += 2*key_tri_vert();

		obj_set_pos(sprite, x, y);
		oam_copy(oam_mem, obj_buffer, 1);
	}
}

void bg_test_brin(){
	// Load palette
	memcpy(pal_bg_mem, brinPal, brinPalLen);
	// Load tiles to CBB 0
	memcpy(&tile_mem[0][0], brinTiles, brinTilesLen);
	// Load map into SBB 30
	memcpy(&se_mem[30][0], brinMap, brinMapLen);

	// set up BG0 for a 4bpp 64x32t map, using
	//   using charblock 0 and screenblock 31
	REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

	// Scroll around some
	int x= 192, y= 64;
	while(1)
	{
		vid_vsync();
		key_poll();

		x += key_tri_horz();
		y += key_tri_vert();

		REG_BG0HOFS= x;
		REG_BG0VOFS= y;
	}
}

void game_test_init(){

	// set up BG0 for a 4bpp 64x32t map, using
	//   using charblock 0 and screenblock 31
	REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;

	//// BG
	// Load palette
	memcpy(pal_bg_mem, brinPal, brinPalLen);
	// Load tiles to CBB 0
	memcpy(&tile_mem[0][0], brinTiles, brinTilesLen);
	// Load map into SBB 30
	memcpy(&se_mem[30][0], brinMap, brinMapLen);

	//// sprite
	memcpy(&tile_mem[4][0], sp_food_8bTiles, sp_food_8bTilesLen);
	memcpy(pal_obj_mem, sp_food_8bPal, sp_food_8bPalLen);

	oam_init(obj_buffer, 128);

	int x = 120 - 8;
	int y = 80 - 8;

	u32 tileId = 0, palBank = 0;
	OBJ_ATTR* sprite = &obj_buffer[0];
	obj_set_attr(sprite, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(palBank) | tileId);
	
	// set attr0 to match resource color(4BPP, 8BPP)
	sprite->attr0 |= ATTR0_8BPP;

	while (1)
	{
		vid_vsync();
		key_poll();
		
		if(key_hit(KEY_A))
		{
			tileId += 4 << 1;
			tileId = tileId & 0x1FF;
		}

		sprite->attr2 = ATTR2_BUILD(tileId, palBank, 0);

		x += 2*key_tri_horz();
		y += 2*key_tri_vert();

		obj_set_pos(sprite, x, y);
		oam_copy(oam_mem, obj_buffer, 1);
	}
}

int main()
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	
	// print_test();

	// sprite_test_pika();
	// sprite_test_food();
	// sprite_test_human();

	// bg_test_brin();

	// while(1)
	// {
	// 	vid_vsync();
	// 	key_poll();
	// 	VBlankIntrWait();
	// }

	game_test_init();

	return 0;
}
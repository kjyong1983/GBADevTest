#include <stdio.h>
#include <string.h>
#include <tonc.h>

// bg
#include "brin.h"
// #include "splash_yyt.h"
// #include "logotest.h"
#include "yyt.h"
#include "title.h"
#include "gameover.h"

// 맵으로 넣을 때는 512*256, 4bpp, sbb 포맷으로

// sprite
#include "mygame.h"

typedef enum GAME_SCENE
{
	GAME_SCENE_NONE,
	GAME_SCENE_LOGO,
	GAME_SCENE_TITLE,
	GAME_SCENE_GAME,
	GAME_SCENE_GAMEOVER
} GAME_SCENE;

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

GAME_SCENE prevScene = GAME_SCENE_LOGO;
GAME_SCENE currentScene = GAME_SCENE_LOGO;

// scene management
void ChangeScene(GAME_SCENE newScene);
void EnterScene(GAME_SCENE scene);
void UpdateScene(GAME_SCENE scene);
void ExitScene(GAME_SCENE scene);

// scene
void enter_logo();
void update_logo();
void exit_logo();

void enter_title();
void update_title();
void exit_title();

void enter_game();
void update_game();
void exit_game();

void enter_gameOver();
void update_gameOver();
void exit_gameOver();

// timer
// int gameTimer = 0;
float gameTimer = 0;

// float GetTimer(int index)
// {
// 	switch (index)
// 	{
// 	case 0:
// 		return REG_TM0D;
// 		break;
// 	case 1:
// 		return REG_TM1D;
// 		break;
// 	case 2:
// 		return REG_TM3D;
// 		break;
// 	case 3:
// 		return REG_TM4D;
// 		break;
	
// 	default:
// 		break;
// 	}
// }

// void SetTimer(int index, float time)
// {
// 	switch (index)
// 	{
// 	case 0:
// 		REG_TM0CNT = TM_ENABLE | TM_CASCADE;
// 		return;
// 	case 1:
// 		REG_TM1CNT = TM_ENABLE | TM_CASCADE;
// 		return;

// 	case 2:
// 		REG_TM3CNT = TM_ENABLE | TM_CASCADE;
// 		return;
// 	case 3:
// 		REG_TM4CNT = TM_ENABLE | TM_CASCADE;
// 		return;
	
// 	default:
// 		break;
// 	}
// }

// void ToggleTimer(int index)
// {
// 	switch (index)
// 	{
// 	case 0:
// 		REG_TM0CNT ^= TM_ENABLE;
// 		return;

// 	case 1:
// 		REG_TM1CNT ^= TM_ENABLE;
// 		return;

// 	case 2:
// 		REG_TM3CNT ^= TM_ENABLE;
// 		return;
// 	case 3:
// 		REG_TM4CNT ^= TM_ENABLE;
// 		return;
	
// 	default:
// 		break;
// 	}
// }

// bg
void init_bg()
{
	REG_DISPCNT |= DCNT_BG0;
	REG_BG0CNT |= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;

	memcpy(pal_bg_mem, brinPal, brinPalLen);
	memcpy(&tile_mem[0][0], brinTiles, brinTilesLen);
	memcpy(&se_mem[30][0], brinMap, brinMapLen);
}

void update_bg()
{
	bgX += bgScrollSpeed * key_tri_horz();
	bgY += bgScrollSpeed * key_tri_vert();

	REG_BG0HOFS = bgX;
	REG_BG0VOFS = bgY;
}

void draw_bg()
{

}

void init_sprite()
{
	REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;

	memcpy(&tile_mem[4][0], mygameTiles, mygameTilesLen);
	memcpy(pal_obj_mem, mygamePal, mygamePalLen);
	oam_init(obj_buffer, 128);
}

// player sprite
void init_player()
{
	// oam index
	playerSprite = &obj_buffer[0];

	obj_set_attr(playerSprite, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(playerPalBank) | playerTileId);
	obj_set_pos(playerSprite, playerX, playerY);

	oamCount++;
}

void clamp_player_position()
{
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

void update_player()
{
	playerX += playerSpeed * key_tri_horz();
	playerY += playerSpeed * key_tri_vert();

	clamp_player_position();
}

void draw_player()
{
	obj_set_pos(playerSprite, playerX, playerY);
}

// enemy
void init_enemy()
{
	// oam index
	enemySprite = &obj_buffer[1];

	obj_set_attr(enemySprite, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(enemyPalBank) | enemyTileId);
	obj_set_pos(enemySprite, enemyX, enemyY);
	oamCount++;
}

void update_enemy()
{
	enemyX += enemyX > playerX ? -1 : 1;
	enemyY += enemyY > playerY ? -1 : 1;
}

void draw_enemy()
{
	obj_set_pos(enemySprite, enemyX, enemyY);
}

void init_animation_dummy()
{
	
}

void init_bullet_pool()
{

}

void draw_sprite(){
	oam_copy(oam_mem, obj_buffer, oamCount);	// only need to update one
}

// int main()
// {
// 	// Init interrupts and VBlank irq.
// 	irq_init(NULL);
// 	irq_add(II_VBLANK, NULL);
	
// 	REG_DISPCNT = DCNT_MODE0;

// 	// // Init 4bpp vwf text on bg 0.
// 	// tte_init_chr4c(0, 			// BG 0
// 	// 	BG_CBB(0)|BG_SBB(31),	// Charblock 0; screenblock 31
// 	// 	0xF000,					// Screen-entry offset
// 	// 	bytes2word(1,2,0,0),	// Color attributes.
// 	// 	CLR_YELLOW, 			// Yellow text
// 	// 	&verdana9Font,			// Verdana 9 font
// 	// 	NULL					// Use default chr4 renderer
// 	// 	);
	
// 	// // Initialize use of stdio.
// 	// tte_init_con();
	
// 	// // Printf something at 96,72
// 	// tte_printf("#{P:96,72}Hello World!");



// 	init_bg();
	
// 	init_sprite();
	
// 	init_player();
// 	init_enemy();

// 	init_bullet_pool();

// 	init_animation_dummy();

// 	while(1)
// 	{
// 		VBlankIntrWait();

// 		key_poll();

// 		update_bg();
// 		update_player();
// 		update_enemy();

// 		draw_bg();

// 		draw_player();
// 		draw_enemy();

// 		draw_sprite();
// 	}

// 	return 0;
// }

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

	ChangeScene(GAME_SCENE_LOGO);

	while(1)
	{
		VBlankIntrWait();
		key_poll();

		UpdateScene(currentScene);
	}

	return 0;

}

void ChangeScene(GAME_SCENE newScene)
{
	prevScene = currentScene;
	currentScene = newScene;

	ExitScene(prevScene);
	EnterScene(currentScene);
}

void EnterScene(GAME_SCENE scene)
{
	switch (scene)
	{
	case GAME_SCENE_NONE:
		break;

	case GAME_SCENE_LOGO:
		enter_logo();
		break;

	case GAME_SCENE_TITLE:
		enter_title();
		break;

	case GAME_SCENE_GAME:
		enter_game();
		break;

	case GAME_SCENE_GAMEOVER:
		enter_gameOver();
		break;
	
	default:
		break;
	}
}

void UpdateScene(GAME_SCENE scene)
{
	switch (scene)
	{
	case GAME_SCENE_NONE:
		break;
	
	case GAME_SCENE_LOGO:
		update_logo();
		break;

	case GAME_SCENE_TITLE:
		update_title();
		break;

	case GAME_SCENE_GAME:
		update_game();
		break;

	case GAME_SCENE_GAMEOVER:
		update_gameOver();
		break;
	
	default:
		break;
	}
}

void ExitScene(GAME_SCENE scene)
{
	switch (scene)
	{
	case GAME_SCENE_NONE:
		break;

	case GAME_SCENE_LOGO:
		exit_logo();
		break;

	case GAME_SCENE_TITLE:
		exit_title();
		break;

	case GAME_SCENE_GAME:
		exit_game();
		break;

	case GAME_SCENE_GAMEOVER:
		exit_gameOver();
		break;
	
	default:
		break;
	}
}

void enter_logo()
{
	// 로고 씬 초기화
	REG_DISPCNT |= DCNT_BG0;
	REG_BG0CNT |= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;

	// memcpy(pal_bg_mem, brinPal, brinPalLen);
	// memcpy(&tile_mem[0][0], brinTiles, brinTilesLen);
	// memcpy(&se_mem[30][0], brinMap, brinMapLen);

	memcpy(pal_bg_mem, yytPal, yytPalLen);
	memcpy(&tile_mem[0][0], yytTiles, yytTilesLen);
	memcpy(&se_mem[30][0], yytMap, yytMapLen);
}

void update_logo()
{
	// 2초 보여주고 자동 나가기

	if(key_hit(KEY_A))
		ChangeScene(GAME_SCENE_TITLE);
	
}

void exit_logo()
{
	// 로고 씬 파괴
}

void enter_title()
{
	// 타이틀 씬 초기화
	
	memcpy(pal_bg_mem, titlePal, titlePalLen);
	memcpy(&tile_mem[0][0], titleTiles, titleTilesLen);
	memcpy(&se_mem[30][0], titleMap, titleMapLen);
}


void update_title()
{
	// 타이틀에서 게임 시작 입력 받기

	if(key_hit(KEY_A))
		ChangeScene(GAME_SCENE_GAME);

}


void exit_title()
{
	// 게임 시작 누르면 게임 씬으로 이동
}

void enter_game()
{
	init_bg();
	
	init_sprite();
	
	init_player();
	init_enemy();

	init_bullet_pool();

	init_animation_dummy();
}

void update_game()
{
	update_bg();
	update_player();
	update_enemy();

	draw_bg();
	draw_player();
	draw_enemy();

	// check_collision();

	if(key_hit(KEY_A))
		ChangeScene(GAME_SCENE_GAMEOVER);

	draw_sprite();
}

void exit_game()
{
	// 게임 씬 오브젝트 모두 정리
}

void enter_gameOver()
{
	// 게임 오버 맵 파일 생성
	
	memcpy(pal_bg_mem, gameoverPal, gameoverPalLen);
	memcpy(&tile_mem[0][0], gameoverTiles, gameoverTilesLen);
	memcpy(&se_mem[30][0], gameoverMap, gameoverMapLen);
}


void update_gameOver()
{
	// show game over screen
	// 1초간 입력 무시, 그 이후 a키 누르면 타이틀로 이동

	ChangeScene(GAME_SCENE_TITLE);
}


void exit_gameOver()
{
	// 게임 오버 씬 오브젝트 정리
}



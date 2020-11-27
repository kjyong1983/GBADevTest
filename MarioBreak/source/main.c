//////////////////////////
// gfx header
#include "gfx/gfx.h"



//////////////////////////
// engine/lib headers
#include "code/lib/gba.h"
#include "code/lib/sprite.h"
#include "code/lib/keypad.h"
#include "code/lib/fade.h"



/////////////////////////
// software defines
#define SHOUT		(0)
#define GBAPARTY	(1)
#define	MENU		(2)
#define	GAME_PLAY	(3)



int	 state;
int  done;
int	 menuPointer;

/*
===================================================
=
= gba part
=
===================================================
*/


unsigned short *v_buffer;    // this is the buffer adress
int vframe = 1;              // this is the buffer frame

#define LEVEL_MAX (5)

int level[LEVEL_MAX * 4][12] =
{
	{0,0,0,1,1,1,1,1,1,1,0,0,}, // level 1
	{0,0,0,0,1,2,5,2,1,0,0,0,},
	{0,0,0,0,0,1,3,1,0,0,0,0,},
	{0,0,0,0,0,0,1,0,0,0,0,0,},

	{3,1,5,1,1,1,1,1,1,5,1,3,}, // level 2
	{0,3,3,3,1,2,2,1,3,3,3,0,},
	{0,0,5,0,2,3,3,2,0,5,0,0,},
	{0,1,0,1,0,1,0,1,0,1,0,1,},
	
	{3,3,3,3,3,3,5,3,3,3,3,3,}, // level 3
	{2,4,2,2,2,2,2,2,2,4,2,2,},
	{2,1,2,1,2,4,2,1,2,1,2,1,},
	{1,1,1,5,1,1,1,1,1,1,5,1,},

	{1,2,1,0,3,0,0,3,0,1,2,1,}, // level 4
	{1,5,1,1,1,1,1,1,1,1,5,1,},
	{2,1,2,1,1,4,3,4,1,2,1,2,},
	{1,1,1,3,4,1,4,1,4,1,1,1,},
	
	{1,3,1,3,1,3,1,3,1,3,1,3,}, // level 5
	{3,3,4,3,4,3,4,3,4,3,4,3,},
	{2,5,2,2,2,2,5,2,2,2,5,2,},
	{1,4,1,4,1,4,1,4,1,4,1,4,},
	

};


#define		SCREEN_BORDER 	(192)

#define		LEVEL_GOOMBA	(1)
#define		LEVEL_SHYGUY	(2)
#define		LEVEL_TWEETER	(3)

#define		LEVEL_HARDBOX	(4)
#define		LEVEL_SOFTBOX	(5)

#define		LEVEL_COIN		(6)		// not drawable in level[][], just for collision
#define		LEVEL_SHGREEN	(7) 	// dito..
#define		LEVEL_SHRED		(8)		// dito..
#define		LEVEL_BLOCK		(9)		// dito..
#define		LEVEL_LAVA		(10)	// dito..

int 		OBJECT_MAX = 0;
int			flag_gameover = 0; 
int			flag_level = 0;

int			coins_collected = 0;
int			powerup_collected = 0;

int flag_bomb = 0;
int flag_canon = 0;

typedef struct Point
{
	int		type; 		// enemy,item,etc

	int 	x,y;		// position on screen
	int 	dirx,diry;	// direction it's traveling

	int		anim;		// animation frame

	int		hit;		// how many times it's been hit

}Point;


Point mario;
Point bomb;
Point fireball;

Point objects[96];
Point ground[24]; // 1 x 12 = block, 1 x 12 = lava

/*
=================
= buffer
=================
*/
void buffer(void)
{
	//
	// switch buffers
	// 
    if (vframe == 1)
		v_buffer = (unsigned short*)0x6000000; // buffer 1
    else
    	v_buffer = (unsigned short*)0x600A000; // buffer 0
 
    *(unsigned short*)0x4000000= 0x0404 | (vframe << 4) | 0x1000 | 0x40;
		
    vframe = 1 - vframe;
}



void bgPal(unsigned int * pal)
{
	int 	p;

  	for (p = 0; p < 247; p++) 
		((unsigned short *)0x5000000)[p] = ((unsigned short*)pal)[p]; 

}


void bgPic(unsigned int * pic)
{
	int 	i;

	for (i = 0; i < 19200; i++)
  		((unsigned short*)v_buffer)[i] = ((unsigned short*)pic)[i];

}

void bgPic2Buffer(unsigned int * pic)
{
	int 	i;

	for (i = 0; i < 120*160; i++)
	{
  		((unsigned short*)0x6000000)[i] = ((unsigned short*)pic)[i];
		((unsigned short*)0x600A000)[i] = ((unsigned short*)pic)[i];
	}
}




/*
===================================================
=
= menu part
=
===================================================
*/

/*
=================
= setState
=================
*/
void setState(int s)
{
	flag_gameover = 2;
	coins_collected = 0;
	powerup_collected = 0;

	flag_bomb = 0;
	flag_canon = 0;

	state = s;
	done = 0;

	InitializeSprites();
}

/*
===================================================
=
= state part
=
===================================================
*/

int sx,sy;


#define PARTICLES_MAX (96)
#define ENEMY_OFFSET (32)

#define ENEMY_HEIGHT (0)




int c;



int checkCollision(int x1,int y1)
{
		if(fireball.y == y1 + 16 || fireball.y == y1 + 2)
		{

			if((fireball.x + 8) >= (x1 - 8) && (fireball.x + 8) <= (x1 + 8))
			{
				fireball.dirx = 1;
				fireball.diry = 0;

				return 0;
			}

			if((fireball.x + 8) >= (x1 + 8) && (fireball.x + 8) <= (x1 + 24))
			{
				fireball.dirx = 0;
				fireball.diry = 0;

				return 1;
			}	

		}

		if(bomb.y == y1)
		{

			if((bomb.x + 8) >= (x1 - 8) && (bomb.x + 8) <= (x1 + 8))
			{

				return 0;
			}

			if((bomb.x + 8) >= (x1 + 8) && (bomb.x + 8) <= (x1 + 24))
			{

				return 1;
			}	
		}

		return 3;
}


void drawMario()
{
	mario.anim++;
	MoveSprite(&sprites[0],mario.x,mario.y);
	updateSprite(0,OAM_MARIO_WALK,4,(mario.anim / 4)%4);	
}




void bounce()
{

	int flr;

	int	e;
	int blockcount;

	if(fireball.x <= 0) 					fireball.dirx = 0;
	if(fireball.x >= SCREEN_BORDER - 8) 	fireball.dirx = 1;

	if(fireball.y <= 0) 					fireball.diry = 0;
	if(fireball.y >= 168) 					fireball.diry = 1;

	if(fireball.y >= 160)					flag_gameover = 1;

	//
	// move ball	
	//

	if(fireball.dirx == 0) 
		fireball.x++;
	else if(fireball.dirx == 1) 
		fireball.x--;

	if(fireball.diry == 0)
		fireball.y++;
	else if(fireball.diry == 1)
		fireball.y--;

	//
	// update sprites
	//

	fireball.anim++;
	MoveSprite(&sprites[1],fireball.x,fireball.y);
	updateSprite(1,OAM_ITEM_FIREBALL,1,(fireball.anim / 8)%4);	




	//
	// collision between ball and mario
	//
	if(fireball.y + 8 == mario.y)
	{

		if((fireball.x + 8) >= (mario.x - 8) && (fireball.x + 8) <= (mario.x + 8))
		{
			fireball.dirx = 1;
			fireball.diry = 1;
		}

		if((fireball.x + 8) >= (mario.x + 8) && (fireball.x + 8) <= (mario.x + 24))
		{
			fireball.dirx = 0;
			fireball.diry = 1;
		}	
	}






	flr = 0;


	for(e = 0; e < OBJECT_MAX; e++)
	{

		

		objects[e].anim++;

		switch(objects[e].type)
		{
				

				//
				//		enemies, static objects
				//
				case	LEVEL_GOOMBA:

						updateSprite(e + ENEMY_OFFSET,OAM_ENEMY_GOOMBA,4,(objects[e].anim / 8)%2);

						if(checkCollision(objects[e].x,objects[e].y) != 3)
						{
							objects[e].type = LEVEL_COIN;
				
							updateSprite(e + ENEMY_OFFSET,OAM_ITEM_COIN,2,0);	
					
						}

						break;

				case	LEVEL_SHYGUY:

						updateSprite(e + ENEMY_OFFSET,OAM_ENEMY_SHYGUY,4,(objects[e].anim / 8)%2);

						if(checkCollision(objects[e].x,objects[e].y) != 3)
						{
							objects[e].type = LEVEL_GOOMBA;
				
							updateSprite(e + ENEMY_OFFSET,OAM_ENEMY_GOOMBA,2,0);
	
						}

						break;

				case	LEVEL_TWEETER:

						updateSprite(e + ENEMY_OFFSET,OAM_ENEMY_TWEETER,4,(objects[e].anim / 8)%2);

						if(checkCollision(objects[e].x,objects[e].y) != 3)
						{
							objects[e].type = LEVEL_GOOMBA;
				
							updateSprite(e + ENEMY_OFFSET,OAM_ENEMY_GOOMBA,2,0);	
						
						}

						break;

				case	LEVEL_SOFTBOX:

						if(checkCollision(objects[e].x,objects[e].y) != 3)
						{
							if(srand()%2 == 1)
							{
								objects[e].type = LEVEL_SHRED;
								updateSprite(e + ENEMY_OFFSET,OAM_ITEM_SHRED,2,0);
							}
							else
							{								
							
								objects[e].type = LEVEL_SHRED;
								updateSprite(e + ENEMY_OFFSET,OAM_ITEM_SHGREEN,2,0);
							}
						}


						break;

				case	LEVEL_HARDBOX:
						
						checkCollision(objects[e].x,objects[e].y);					
						
						break;


				//
				//		pickup items
				//
				case	LEVEL_COIN:
				case	LEVEL_SHGREEN:
				case	LEVEL_SHRED:


						if(objects[e].y < 160) 
						{
							objects[e].y++;
							MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);
						}

						if(objects[e].y + 8 == mario.y)
						{

							if((objects[e].x + 8) >= (mario.x - 8) && (objects[e].x + 8) <= (mario.x + 8))
							{
								objects[e].x = 240;
								objects[e].y = 160;

								if(objects[e].type == LEVEL_COIN) coins_collected++;

								MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);

								if(objects[e].type == LEVEL_SHRED) 	 powerup_collected = 1;
								if(objects[e].type == LEVEL_SHGREEN) powerup_collected = 2;

							

							}

							if((objects[e].x + 8) >= (mario.x + 8) && (objects[e].x + 8) <= (mario.x + 24))
							{
								objects[e].x = 240;
								objects[e].y = 160;

								if(objects[e].type == LEVEL_COIN) coins_collected++;

								MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);		

								if(objects[e].type == LEVEL_SHRED) 	 powerup_collected = 1;
								if(objects[e].type == LEVEL_SHGREEN) powerup_collected = 2;

								
				
							}	
						}

						break;

				case	LEVEL_BLOCK:
						
						
						
						if(powerup_collected != 0)
						{
							
							if(objects[e].y == (160 - 16))
							flr++;

							objects[e].hit = 0;


							if(objects[e].y > (160 - 16))
							{
								objects[e].y--;
								MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);
							}
						}
						

						if(objects[e].hit == 1 && objects[e].y < (160 - 12))
						{
							objects[e].y++;
							MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);
						}
						
						
						if(objects[e].hit == 2 && objects[e].y < (160 - 8))
						{
							objects[e].y++;
							MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);
						}
						
						
						if(objects[e].hit == 3 && objects[e].y < (160))
						{
							objects[e].y++;
							MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);
						}
						

						if(fireball.y == objects[e].y)
						{

							if((fireball.x + 8) >= (objects[e].x - 8) && (fireball.x + 8) <= (objects[e].x + 8))
							{
								fireball.dirx = 1;
								fireball.diry = 1;
								
								//if(objects[e].hit < 3)
								objects[e].hit++;
								
							}

							if((fireball.x + 8) >= (objects[e].x + 8) && (fireball.x + 8) <= (objects[e].x + 24))
							{
								fireball.dirx = 0;
								fireball.diry = 1;
								
								//if(objects[e].hit < 3)
								objects[e].hit++;

							}	

						}

						//
						// mario pos update!
						//
				
						if((mario.x) >= (objects[e].x) && (mario.x) <= (objects[e].x + 8))
						{
							mario.y = objects[e].y - 16;
							MoveSprite(&sprites[0],mario.x,mario.y);

							if(objects[e].y >= 159)
							{
								flag_gameover = 1;
								return;
							}
						}

						
						
						break;

				case	LEVEL_LAVA:

						updateSprite(e + ENEMY_OFFSET,OAM_ITEM_LAVA,4,(objects[e].anim / 8)%2);

						break;
		}

		
		
	}

	if(flr >= 12) powerup_collected = 0;


}



void bomber()
{
	int bombpos;
	int y;
	


	
	if(coins_collected >= 2)
	{
		
		if(flag_canon != 1)
		{
			for(y = -32; y < 160; y++)
			{
				MoveSprite(&sprites[4],2,y);
				CopyOAM();
			}
		}

		if(y == 160) 
		{
			MoveSprite(&sprites[4],240,160);
			flag_canon = 1;
		}
	}

	if(coins_collected >= 2)
	{
		if(flag_bomb != 2) flag_bomb = 1;
		bombpos = 2;
		MoveSprite(&sprites[113],240 - 32,bombpos * 16);
	}

	if(coins_collected >= 4)
	{
		if(flag_bomb != 2) flag_bomb = 1;
		bombpos = 1;
		MoveSprite(&sprites[113],240 - 32,bombpos * 16);
	}

	if(coins_collected >= 6)
	{
		if(flag_bomb != 2) flag_bomb = 1;
		bombpos = 0;
		MoveSprite(&sprites[113],240 - 32,bombpos * 16);
	}

	if(flag_bomb == 2)
	{
		bomb.x = 192 - 16;
		bomb.y = bombpos * 16;
		
		coins_collected = 0;

		MoveSprite(&sprites[113],240,160);

		flag_bomb = 0;
	}


	if(bomb.x > (0 - 16))
	{
		bomb.x--;
		bomb.anim++;

		MoveSprite(&sprites[2],bomb.x,bomb.y);
		updateSprite(2,OAM_ENEMY_BULLET,4,(bomb.anim / 4)%2);
	}

}





void checkLevel()
{
	int e;
	int o;

	o = 0;

	for(e = 0; e < OBJECT_MAX; e++)
	{
		switch(objects[e].type)
		{
				case	LEVEL_GOOMBA:
				case	LEVEL_SHYGUY:
				case	LEVEL_TWEETER:
				case	LEVEL_SOFTBOX:
				o++;
				break;
		}
	}

	if(o == 0) 
	{	
		if(flag_level < (LEVEL_MAX - 1))
		{
			flag_level++;
			setState(GAME_PLAY);
		}
		else
		{
			setState(MENU);	
		}
	}
}




/*
=================
= init
=================
*/
void init(int i)
{
	int e;

	int lx,ly;
	
	

	switch(i)
	{
			case	GBAPARTY:

					if(done == 0)
					{


						FadeOut(0);
						bgPal((void*)bg_gbapartyPalette);
						bgPic((void*)bg_gbapartyBitmap);
						FadeIn(1);

						done = 1;
					}

					break;

			case	SHOUT:

					if(done == 0)
					{


						FadeOut(0);
						bgPal((void*)bg_shoutPalette);
						bgPic((void*)bg_shoutBitmap);
						FadeIn(1);

						done = 1;
					}

					break;

			case 	MENU:

				if(done == 0)
				{
					InitializeSprites();

					FadeOut(0);
					bgPal((void*)bg_introPalette);
					bgPic((void*)bg_introBitmap);
					FadeIn(1);
					
					loadSpritePal((void*)item_fireballPalette); 						// counts for all sprites

					loadSpriteGraphics((void*)mario_walkData,MARIO_WALK_SIZE);			// 16x16 	, 4 frames

					initSprite(0,SIZE_16,OAM_MARIO_WALK);

					mario.x = 122;
					mario.y = 134;

					mario.anim = 0;
					
					MoveSprite(&sprites[0],mario.x,mario.y);

					done = 1;
				}

				break;

			case 	GAME_PLAY:

				if(done == 0)
				{
					e = 0;

					InitializeSprites();
					
					FadeOut(0);

					switch(flag_level)
					{
							case	0:

								bgPal((void*)bg_castlePalette);
								bgPic((void*)bg_castleBitmap);

								break;

							case	1:

								bgPal((void*)bg_undergroundPalette);
								bgPic((void*)bg_undergroundBitmap);

								break;

							case	2:

								bgPal((void*)bg_skyPalette);
								bgPic((void*)bg_skyBitmap);

								break;

							case	3:

								bgPal((void*)bg_vollyPalette);
								bgPic((void*)bg_vollyBitmap);

								break;

							case	4:

								bgPal((void*)bg_spacePalette);
								bgPic((void*)bg_spaceBitmap);

								break;

					}

					FadeIn(1);


					loadSpritePal((void*)item_fireballPalette); // counts for all sprites


					//
					// characters
					//
					loadSpriteGraphics((void*)mario_walkData,MARIO_WALK_SIZE);			// 16x16 	, 4 frames

					initSprite(0,SIZE_16,OAM_MARIO_WALK);

					mario.x = 120;
					mario.y = 160 - 32;

					mario.anim = 0;
					
					MoveSprite(&sprites[0],mario.x,mario.y);


					//
					// moveable items
					//
					loadSpriteGraphics((void*)item_fireballData,ITEM_FIREBALL_SIZE);	// 8x8 		, 4 frames

					initSprite(1,SIZE_8,OAM_ITEM_FIREBALL);

					fireball.x = 120;
					fireball.y = 80;

					fireball.dirx = 1;
					fireball.diry = 1;

					fireball.anim = 0;



					//
					// static items
					//
					loadSpriteGraphics((void*)item_hardboxData,ITEM_HARDBOX_SIZE);		// 16x16	, 1 frame
					loadSpriteGraphics((void*)item_softboxData,ITEM_SOFTBOX_SIZE);		// 16x16	, 1 frame

				
					//
					// enemies
					//
					loadSpriteGraphics((void*)enemy_goombaData,ENEMY_GOOMBA_SIZE);		// 16x16	, 2 frames
					loadSpriteGraphics((void*)enemy_bulletData,ENEMY_BULLET_SIZE);		// 16x16	, 2 frames
					loadSpriteGraphics((void*)enemy_shyguyData,ENEMY_SHYGUY_SIZE);		// 16x16	, 2 frames
					loadSpriteGraphics((void*)enemy_tweeterData,ENEMY_TWEETER_SIZE);	// 16x16	, 2 frames


					//
					// pickup items
					//
					loadSpriteGraphics((void*)item_shgreenData,ITEM_SHGREEN_SIZE);		// 16x16	, 1 frame
					loadSpriteGraphics((void*)item_shredData,ITEM_SHRED_SIZE);			// 16x16	, 1 frame
					loadSpriteGraphics((void*)item_coinData,ITEM_COIN_SIZE);			// 16x16	, 1 frame

					loadSpriteGraphics((void*)item_blockData,ITEM_BLOCK_SIZE);			// 16x16	, 1 frame
					loadSpriteGraphics((void*)item_lavaData,ITEM_LAVA_SIZE);			// 16x16	, 2 frame

					loadSpriteGraphics((void*)pointerData,POINTER_SIZE);				// 16x16	, 1 frame
					loadSpriteGraphics((void*)canonData,CANON_SIZE);					// 8x8		, 3 frame
					
					initSprite(4,SIZE_32,OAM_CANON);
					MoveSprite(&sprites[4],240,160); // bonus

					initSprite(2,SIZE_16,OAM_ENEMY_BULLET);

					bomb.x = 0 - 32;
					bomb.y = 0;



					initSprite(113,SIZE_16,OAM_POINTER);

					


					for(ly = 0; ly < 4; ly++)
					{
						for(lx = 0; lx < 12; lx++)
						{

							switch(level[(flag_level * 4) + ly][lx])
							{
									case	LEVEL_GOOMBA:

											initSprite(e + ENEMY_OFFSET,SIZE_16,OAM_ENEMY_GOOMBA);
											
											objects[e].type = LEVEL_GOOMBA;

											objects[e].x = lx * 16;
											objects[e].y = ENEMY_HEIGHT + (ly * 16);

											MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);
											
											e++;

											break;

									case	LEVEL_SHYGUY:

											initSprite(e + ENEMY_OFFSET,SIZE_16,OAM_ENEMY_SHYGUY);

											objects[e].type = LEVEL_SHYGUY;		
	
											objects[e].x = lx * 16;
											objects[e].y = ENEMY_HEIGHT + (ly * 16);

											MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);
											
											e++;

											break;

									case	LEVEL_TWEETER:

											initSprite(e + ENEMY_OFFSET,SIZE_16,OAM_ENEMY_TWEETER);

											objects[e].type = LEVEL_TWEETER;

											objects[e].x = lx * 16;
											objects[e].y = ENEMY_HEIGHT + (ly * 16);

											MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);

											e++;

											break;

									case	LEVEL_SOFTBOX:

											initSprite(e + ENEMY_OFFSET,SIZE_16,OAM_ITEM_SOFTBOX);

											objects[e].type = LEVEL_SOFTBOX;

											objects[e].x = lx * 16;
											objects[e].y = ENEMY_HEIGHT + (ly * 16);

											MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);

											e++;

											break;
			
									case	LEVEL_HARDBOX:

											initSprite(e + ENEMY_OFFSET,SIZE_16,OAM_ITEM_HARDBOX);

											objects[e].type = LEVEL_HARDBOX;

											objects[e].x = lx * 16;
											objects[e].y = ENEMY_HEIGHT + (ly * 16);

											MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);

											e++;

											break;


							}

							
						}
					}
					
					for(lx = 0; lx < 12; lx++)
					{
						
						initSprite(e + ENEMY_OFFSET,SIZE_16,OAM_ITEM_BLOCK);

						objects[e].type = LEVEL_BLOCK;
						objects[e].hit = 0;

						objects[e].x = lx * 16;
						objects[e].y = (160 - 16);

						MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);

						e++;

						initSprite(e +  ENEMY_OFFSET,SIZE_16,OAM_ITEM_LAVA);

						objects[e].type = LEVEL_LAVA;

						objects[e].x = lx * 16;
						objects[e].y = (160 - 16);

						MoveSprite(&sprites[e + ENEMY_OFFSET],objects[e].x,objects[e].y);

						e++;
						
					}

					OBJECT_MAX = e;

					done = 1;
				}

					

				break;
	}
}





/*
=================
= run
=================
*/
void run(int r)
{

	int p;
	switch(r)
	{

			case	GBAPARTY:

					SleepQ(32);

					setState(SHOUT);

					break;

			case	SHOUT:

					SleepQ(64);

					setState(MENU);

					break;

			case	MENU:

					drawMario();

					break;
			case	GAME_PLAY:
					
					
					//updateSprite(0,0,16,(player1.x)%2);			
					
					if(flag_gameover == 1)
					{
						setState(GAME_PLAY);
					}
					

					if(flag_gameover == 2)
					{	
						fireball.x = mario.x + 4;
						fireball.y = mario.y - 8;

						bounce();
					}	

					if(flag_gameover == 3)
					{
						bounce();
						bomber();

						checkLevel();
					}
					
				break;
	}
}


/*
=================
= input
=================
*/
void input(int i)
{
	switch(i)
	{
			case	MENU:
					if(!(*KEYS & KEY_START) || !(*KEYS & KEY_A) || !(*KEYS & KEY_B)) setState(GAME_PLAY);

					break;

			case	GAME_PLAY:

		
				//if(!(*KEYS & KEY_A)) sx++;
				if(!(*KEYS & KEY_START) && !(*KEYS & KEY_L) && !(*KEYS & KEY_R)) 
				{
						if(flag_level < (LEVEL_MAX - 1))
						{
							flag_level++;
							setState(GAME_PLAY);
						}
						else
						{
							setState(MENU);	
						}
				}
		
				
				if(flag_gameover != 1)
				{
						if(!(*KEYS & KEY_LEFT)) 
						{
							if(mario.x > 0) mario.x--;

							if(flag_gameover == 2) fireball.dirx = 1;

							drawMario();
						}
						if(!(*KEYS & KEY_RIGHT)) 
						{
							if(mario.x < SCREEN_BORDER - 16) mario.x++;

							if(flag_gameover == 2) fireball.dirx = 0;

							drawMario();
						}
					
						if(flag_gameover == 2)
						{
							if(!(*KEYS & KEY_A)) flag_gameover = 3;
						}
						else
						{
							if(!(*KEYS & KEY_A)) 
	 						{
								if(coins_collected >= 2)
								{
									if(flag_bomb == 1)
									flag_bomb = 2;
								}
							}
						}
				}

				
				break;
	}
}



/*
===================================================
=
= gba part
=
===================================================
*/



/*
=================
= AgbMain
=================
*/
//void AgbMain(void)
int main(void)
{


	buffer();				//switch buffer

	*(unsigned short*)0x4000000=0x0404 | (vframe <<4) | 0x1000 | 0x40;
  	vframe = 1 - vframe; 

	// set begin state
  	setState(GBAPARTY);

  	while(1)
  	{

		init(state);
		run(state);
		input(state);
		

		WaitForVsync();			//waits for the screen to stop drawing
		CopyOAM();				//copies sprite array into OAM.		
  	}
	
	return 0;
}


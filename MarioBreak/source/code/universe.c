/*
=================
= TVector3
=================
*/
typedef struct TVector3
{

	int 	x,y,z;
	int		r,g,b;
	int		type;

}TVector3,*PVector3;



/*
=================
= Player
=================
*/
typedef struct Player
{

	int 	x,y,z;
	int		points;
	int		action;

}Player;

Player player;		// player class
Player enemy;		// enemy class


//
// player actions
//

#define 	ACTION_COLLISION	(0)
//
// player normal animation
//
#define 	ANIMATION_PLAYER_NORMAL_FR1		(1)
#define 	ANIMATION_PLAYER_NORMAL_FR2		(2)


//
// player hurt animation
//
#define 	ANIMATION_PLAYER_HURT_FR1		(3)
#define 	ANIMATION_PLAYER_HURT_FR2		(4)

#define 	ANIMATION_PLAYER_GET_FR1		(6)
//
// sprite pointers for player and enemy
//
#define		SPR_PLAYER						(0)
#define		SPR_ENEMY						(1)



//
// 	TRACK
//
#define  TRACK_MAXZ  (1024)    		// the race track lenght



//
// 	STAR
//
#define  STAR_MAX    (512)    		// How many stars we want to display
#define  STAR_AREA   (64)     		// Specifies the stars area
#define  STAR_MAXZ   (32)     		// Max z position of a star
#define  STAR_SPEED  (2)      		// Specifies the speed of a star
TVector3 vector_point[STAR_MAX]; 	// create point 



//
// 	SPRITE
//
#define  SPRITE_MAX   (4)			// how many sprites we want to display
#define  SPRITE_COUNT (2)			// how many objects
#define  SPRITE_AREA  (16)			// the area they can be within
#define  SPRITE_MAXZ  (128)			// the max distance
#define  SPRITE_SPEED (2)			// the speed to travel on

#define  TYPE_POWERUP (0)
#define  TYPE_ROCK    (1)
#define  TYPE_RESET   (2)

TVector3 sprite_pos[SPRITE_COUNT * SPRITE_MAX]; // create sprite

int foundCollision;
int foundCollisionType;



//
//	GBA
//
int OAMoffset;


unsigned short *v_buffer;    // this is the buffer adress
int vframe = 1;              // this is the buffer frame



//
// voxels
//
const long int screensize_x = 108;
const long int screensize_y = 160;


short int y_max[400];
short int map[4000];

long int count;
long int count2;


short int preX[128];
short int preZ[96];
short int preZ2[96];

short int preJ[200];
short int preJ1[200];
short int prexmap[120];


/*
===================================================
=
= gba part
=
===================================================
*/



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


void turnScreen(void)
{
	*(unsigned short*)0x4000020 = 0;
	*(unsigned short*)0x4000022 = -256;
	*(unsigned short*)0x4000024 = 137;  
	*(unsigned short*)0x4000026 = 0;
	*(unsigned short*)0x4000028 = 40704; 
}



/*
=================
= plotPixel
=================
*/
void plotPixel(int x,int y,unsigned short int c)
{
	if(y < 0) return;
	//v_buffer[x * 160 + y] = (c); 
	v_buffer[y * 120 + x] = c << 8 | c; 
}



/*
===================================================
=
= voxel part
=
===================================================
*/


void setVoxelMap(int x,int z)
{
	count = -x;
	count2 = z;
}


void setVoxelColor(int r,int g,int b,int grad)
{
	int 	p;
	
	switch(grad)
	{
		case 0:

			for (p = 224; p < 256; p++) 
							((unsigned short *)0x5000000)[p] = (r) | (((u16)(g)) << 5) | (((u16)(b)) << 10); 
			break;
		
		case 1:

				for (p = 224; p < 256; p++) 
							((unsigned short *)0x5000000)[p] = (r + p) | (((u16)(g)) << 5) | (((u16)(b)) << 10); 

			break;
		case 2:

				for (p = 224; p < 256; p++) 
							((unsigned short *)0x5000000)[p] = (r) | (((u16)(g + p)) << 5) | (((u16)(b)) << 10); 

			break;
		case 3:

				for (p = 224; p < 256; p++) 
							((unsigned short *)0x5000000)[p] = (r) | (((u16)(g)) << 5) | (((u16)(b + p)) << 10); 

			break;
	}
	
}
void createhorizon()
{
	

          int xmap1 = 0;
          int xmap2 = 0;
          int j;

		  int pJ,pJ1,pxmap;

          //scroll the map data down so we can put in a new horizon
          for ( j=1;j<99; j++) 
		  {
                for( xmap1=0; xmap1!=20;xmap1++)
                {
				   pJ = preJ[j];
				   pJ1 = preJ1[j];

                   map[(pJ)+xmap1] = map[pJ1+xmap1];
                }
          }

          // create a new horizon at the end of the map
          j = 99;

          for ( xmap1=0; xmap1!=20; xmap1++)
          {
				pJ = preJ[j];
				pxmap = prexmap[xmap1];

	            //get some pre-calced sinus data and create a funky sinus wave with it
	            xmap2 = (sinus_x1[sinus_data[count & 255]] + sinus_data[(pxmap+count2) & 255])/2;
	            //put it into the map
	            map[pJ+xmap1] = xmap2 + (rand() & 1);

          }

		count-= 2;
	    count2+= 2;
}


void prefix(void)
{
	int x, z;

	for(z = 0; z < 64; z+=2)
	{
		for(x = 0; x < (screensize_x-2); x+= 6)
		{
			preX[x] = x / 6;
			preZ[z] = z * 20;
			preZ2[z] = z >> 1;
		}
	}

	for(z = 1; z <= 99; z++)
	{
		for(x = 0; x!=20; x++)
		{
			preJ[z] = z * 20;
			preJ1[z] = (z + 1) * 20;
			prexmap[x] = x * 6;
		}
	}
}

void SetupScreen()
{
		
        long int j, x;
        int max;
        int y, k;
        int z, z2;
        int color;
		int	pX,pZ,pZ2;

         // flip frame buffer to display the OTHER screen.. so we can work on the other
         // buffer
        

         z2=128;

          //copy red sky to background
          //for ( j=0; j<(10240)*2; j++) v_buffer[j]= planet_redBitmap[j];
		createhorizon();

         //Draw mountains
        
         //reset baseline to 0
         for ( j=0;j<(screensize_x>>2);j++) y_max[j] = 0;

         //draw 100 lines
         for ( z=0;z<64;z+=2)
          {
              //Draw a line
              for ( x=0; x<(screensize_x-2); x+= 6)
              {
				
				pX = preX[x];
				pZ = preZ[z];
				pZ2 = preZ2[z];

                //get the max height for the X coord
                max = y_max[pX];

                //get height from map, offset it with z2 to give it a
                // fake depth and divide by 2 then add z/2 to give it perspective
                y = (((map[(pZ)+(pX)])- z2 )>>1)+(pZ2)  ;

                // if current height is greater than the known max height AND
                // the current height is not below the screen
                // then draw the pixel from current height to former max height
                if ( y> max  && (64-y) > 0)
                    {
                        //Draw a column from current Y to former max height at the
                        //current X position
                        for (k=y; k > (max); k-= 4)
                        {
                           //get Y value from map add 80 to brighten it
                           //and subtract Z to
                           //give the illusion of light, convert to 32 shades
                           
						
							//color = (u16) ((map[(pZ)+(pX)])+ (80 - z)) >>3;
                    	 	
						   color = 222 + (32 - (z / 2));
						   if ( color > 256 || color < 222) color = 256;

                        	
                            color = color + (rand() & 1); // roughness
                            
                            //color = color << 5; // 32 shades

							

                            //Draw the pixel on the screen
							
							plotPixel(x + 0,160 - k,color);	plotPixel(x + 0,160 - k + 1,color); plotPixel(x + 0,160 - k + 2,color);	plotPixel(x + 0,160 - k + 3,color);
							plotPixel(x + 1,160 - k,color); plotPixel(x + 1,160 - k + 1,color); plotPixel(x + 1,160 - k + 2,color); plotPixel(x + 1,160 - k + 3,color);
							plotPixel(x + 2,160 - k,color); plotPixel(x + 2,160 - k + 1,color); plotPixel(x + 2,160 - k + 2,color); plotPixel(x + 2,160 - k + 3,color);
							plotPixel(x + 3,160 - k,color); plotPixel(x + 3,160 - k + 1,color); plotPixel(x + 3,160 - k + 2,color); plotPixel(x + 3,160 - k+ 3,color);

							plotPixel(x + 4,160 - k,color); plotPixel(x + 4,160 - k + 1,color); plotPixel(x + 4,160 - k + 2,color); plotPixel(x + 4,160 - k + 3,color);
							plotPixel(x + 5,160 - k,color); plotPixel(x + 5,160 - k + 1,color); plotPixel(x + 5,160 - k + 2,color); plotPixel(x + 5,160 - k + 3,color);

							
                        }
                        //set max height for x coords to the current height
                        y_max[pX]=y;
                    }
              }
           z2--;
        }
}


/*
===================================================
=
= Starfield part
=
===================================================
*/



/*
=================
= drawStar
=================
*/
void drawStar(int star)
{
		int 	screen_x,screen_y;
		int 	color;

		//
		// basic 3d math
		//
		screen_x = (vector_point[star].x << 5) / (vector_point[star].z) + 64;
		screen_y = (vector_point[star].y << 5) / (vector_point[star].z) + 80;
		
		// check if it's out of the screen (so we can throw it back)
		if(vector_point[star].z < 2)
			vector_point[star].z = 1 + (rand()%STAR_MAXZ - 1);

		// star color (if it's far away it's darker, etc)
		if(vector_point[star].z < 16)
			color = 255 - vector_point[star].z;
		else
			color = 239;

		// finally draw it

		if(screen_x > 0 && screen_x < 240 && screen_y > 0 && screen_y < 160)
		plotPixel(screen_x,screen_y,color);
}



/*
=================
= initStarField
=================
*/
void initStarField(int r,int g,int b)
{
		int 	star;
		
		/*
		
		PALETTE LOADER FOR MODE 4 AND OTHER NOT TRUE COLOR MODES
		*/
		int		p;

		for (p = 0; p < 16; p++) ((unsigned short *)0x5000000)[239 + p] = p * 2 << r | p * 2 << g | p * 2 << b; 
		


		//
		// create stars
		//
		for(star = 0; star < STAR_MAX; star++)
		{
			//place them at a random spot
			vector_point[star].x = (rand()%STAR_AREA) - (STAR_AREA / 2);
			vector_point[star].y = (rand()%STAR_AREA) - (STAR_AREA / 2);
			vector_point[star].z = 1 + (rand()%STAR_MAXZ - 1);

			//init the colors for the starfield
			vector_point[star].r = r;
			vector_point[star].g = g;
			vector_point[star].b = b;
		}
}



/*
=================
= drawStarField
=================
*/
void starField(void)
{
	int 		star;

	//draw stars
	for(star = 0; star < STAR_MAX; star++)
	{
		// move the stars
		if(vector_point[star].z > 0)
				vector_point[star].z-= STAR_SPEED;

		//draw star 
		drawStar(star);
	}
}



/*
===================================================
=
= object part
=
===================================================
*/



/*
=================
= drawPlanet
=================
*/
void drawPlanet(int spr)
{	
	int tempz;
	int zoom;


	if(TRACK_MAXZ - player.z <= (512))
	{
		tempz = (TRACK_MAXZ - player.z);
	

		if(tempz > 128)
			zoom = tempz;
		else
			zoom = 128;
	
		RotateSprite(3,0, zoom,zoom);	//rotate the sprite
		MoveSprite(&sprites[3],120 - 32,80 - 32);	// move the player
	}
	else
	{
		MoveSprite(&sprites[3],240,160);	// move the player
	}
}






/*
=================
= draw3DSprite
=================
*/
void draw3DSprite(int sprite)
{
	int 	screen_x, screen_y;
	int		zoom;
	
	int 	tempz;

	tempz = sprite_pos[sprite].z - player.z;
	//
	// basic 3d math
	//
	screen_x = (sprite_pos[sprite].x << 5) / (tempz) + 120;
	screen_y = (sprite_pos[sprite].y << 5) / (tempz) + 60;

	if(screen_x > 0 - 32 && screen_x < 240 && screen_y > 0 - 32 && screen_y < 160)
	{
		zoom = 64 + (tempz << 3);
		
		RotateSprite(OAM_OBJECT_OFFSET + sprite,zoom%360,zoom,zoom);				// zoom the sprite
		MoveSprite(&sprites[OAM_OBJECT_OFFSET + sprite],screen_x,screen_y);	// set the sprite at new position
	}
	else
	{
		MoveSprite(&sprites[OAM_OBJECT_OFFSET + sprite],240,160);	//set the sprite out of the screen
	}
}



/*
=================
= initObject
=================
*/
void initObject(int gfx,int max,int type)
{
	int obj;

	//
	// init all objects
	//
	for(obj = (max - 1); obj >= 0; obj--) 
		initSprite(OAM_OBJECT_OFFSET + OAMoffset + obj,SIZE_32,gfx);

	//
	// place all objects at a random pos
	//
	for(obj = (max - 1); obj >= 0; obj--)
	{
		sprite_pos[OAMoffset + obj].x = ((rand()%3)*40) - 76;
		//sprite_pos[OAMoffset + obj].y = ((rand()%3)*40) - 76;
		sprite_pos[OAMoffset + obj].y = 32;
		sprite_pos[OAMoffset + obj].z = ((rand()%8)*32) + obj * 4;

		sprite_pos[OAMoffset + obj].type = type;
	}

	OAMoffset+=max;	// move the OAMoffset, so we never copy a new sprite over an old one.
}



/*
=================
= object
=================
*/
void object(int max)
{
	int 		object;
	int			tempz;


	

	//
	// move all objects
	//
	for(object = 0; object < max; object++)
	{
		tempz = sprite_pos[object].z - player.z;
		
		if(tempz > 4) 
		{
			draw3DSprite(object); // draw all objects
		}
		else
		{
			//
			// don't create new objects when the track end is in sight.
			//
			if((player.z + 2) < (TRACK_MAXZ - (SPRITE_MAXZ * 3)))
			{
				sprite_pos[object].x = ((rand()%3)*40) - 76;
				//sprite_pos[object].y = ((rand()%3)*40) - 76;
				sprite_pos[object].y = 32;
				sprite_pos[object].z = player.z + SPRITE_MAXZ;
			}		
		}
	}
}


/*
===================================================
=
= sprite collision part
=
===================================================
*/



/*
=================
= setCollision
=================
*/
void setCollision(int c, int type)
{
	foundCollision = c;
	foundCollisionType = type;
}




/*
=================
= detection
=================
*/
void detection(int sprite)
{
	int 	screen_x, screen_y;
	int		screen_z;
	int		tempz;
	

	tempz = sprite_pos[sprite].z - player.z;

	//
	// basic 3d math
	//
	screen_x = (sprite_pos[sprite].x << 5) / (tempz) + 120;
	screen_y = (sprite_pos[sprite].y << 5) / (tempz) + 80;

	//
	// check if it's in the screen, otherwise skip it and say there's no collision
	//
	if(screen_x > 0 - 32 && screen_x < 240 && screen_y > 0 - 32 && screen_y < 160)
	{
		// calc z axe
		screen_z = 64 + (tempz << 3);
		
		// check collision
		if(player.x > screen_x - 64 && player.x < screen_x + 16 &&
		   player.y > screen_y - 72 && player.y < screen_y + 16 &&
		   screen_z <= 256)
		{
			setCollision(1,sprite_pos[sprite].type);
		}
		else
		{
			setCollision(0,TYPE_RESET);
		}
	}
	else
	{
		setCollision(0,TYPE_RESET);
	}	
	
}




/*
=================
= checkCollision
=================
*/
void checkCollision(int max)
{
	int 		object;
	int			tempz;

	setCollision(0,TYPE_RESET); // reset collision

	for(object = 0; object < max; object++)
	{
		tempz = sprite_pos[object].z - player.z;

		if(tempz > 4) 
		{
			if(foundCollision == 0) 
				detection(object); // check for collision
			else 
				return; 		   // if there's already a collision, get outta here
		}
	}

}



/*
===================================================
=
= player part
=
===================================================
*/


// setters/getters for x axe
void movePlayerLeft(int x){ player.x -= x; }
void movePlayerRight(int x){ player.x += x; }
void setPlayerX(int x) { player.x = x; }


// setters/getters for y axe
void movePlayerUp(int y){ player.y -= y; }
void movePlayerDown(int y){ player.y += y; }
void setPlayerY(int y){ player.y = y; }


// setters/getters for z axe
void movePlayerForward(int z){ player.z += z; }
void movePlayerBackward(int z){ player.z -= z; }
void setPlayerZ(int z){ player.z = z; }



/*
=================
= playerAnimations
=================
*/
void playerAnimations(int ani)
{
	int i,j;

	switch(ani)
	{
			case	ANIMATION_PLAYER_NORMAL_FR1:

				updateSprite(SPR_PLAYER,OAM_PLAYER_NORMAL_CENTER,64,0);

				break;

			case	ANIMATION_PLAYER_HURT_FR1:

				updateSprite(SPR_PLAYER,OAM_PLAYER_NORMAL_CENTER,64,1);

				break;

			case	ANIMATION_PLAYER_GET_FR1:

				updateSprite(SPR_PLAYER,OAM_PLAYER_NORMAL_CENTER,64,1);

				break;
	}
}




/*
=================
= playerActions
=================
*/
void playerActions(int action)
{
	switch(action)
	{
			case	ACTION_COLLISION:

					if(foundCollision == 1 && foundCollisionType == TYPE_ROCK)
					{
						playerAnimations(ANIMATION_PLAYER_HURT_FR1);
						movePlayerForward(1);
					}

					if(foundCollision == 1 && foundCollisionType == TYPE_POWERUP)
					{
						playerAnimations(ANIMATION_PLAYER_GET_FR1);
						movePlayerForward(1);
					}

					if(foundCollision != 1)
					{
						playerAnimations(ANIMATION_PLAYER_NORMAL_FR1);
						movePlayerForward(2);
					}

					break;
	}
}



/*
=================
= initPlayer
=================
*/
void initPlayer(void)
{
	loadSpriteGraphics((void*)sprite_player_normal_centerData,4096);

	initSprite(SPR_PLAYER,SIZE_64,OAM_PLAYER_NORMAL_CENTER);

	setPlayerX(120 - (32 + 32));
	setPlayerY(80 - (32 + 32));
}




/*
=================
= drawPlayer
=================
*/
void drawPlayer(void)
{
	int		angle;

	RotateSprite(SPR_PLAYER,0, 256,256);	//rotate the sprite
	MoveSprite(&sprites[SPR_PLAYER],player.x,player.y);	// move the player
}



/*
=================
= doPlayer
=================
*/
void doPlayer(void)
{
	drawPlayer();

	if(player.z <= TRACK_MAXZ)
	{
		playerActions(ACTION_COLLISION);
	}
}



/*
===================================================
=
= enemy part
=
===================================================
*/



// setters/getters for x axe
void moveEnemyLeft(int x){ enemy.x -= x; }
void moveEnemyRight(int x){ enemy.x += x; }
void setEnemyX(int x) { enemy.x = x; }


// setters/getters for y axe
void moveEnemyUp(int y){ enemy.y -= y; }
void moveEnemyDown(int y){ enemy.y += y; }
void setEnemyY(int y){ enemy.y = y; }


// setters/getters for z axe
void moveEnemyForward(int z){ enemy.z += z; }
void moveEnemyBackward(int z){ enemy.z -= z; }
void setEnemyZ(int z){ enemy.z = z; }




/*
=================
= initEnemy
=================
*/
void initEnemy(void)
{
	loadSpriteGraphics((void*)sprite_enemy_normal_centerData,512);
	initSprite(SPR_ENEMY,SIZE_32,OAM_ENEMY_NORMAL_CENTER);

	setEnemyX(0);
	setEnemyY(0);
	setEnemyZ(64);
}


/*
=================
= drawEnemySprite
=================
*/
void drawEnemySprite(int sprite)
{
	int 	screen_x, screen_y;
	int		zoom;
	
	int 	tempz;

	tempz = enemy.z - player.z;
	//
	// basic 3d math
	//
	screen_x = (enemy.x << 5) / (tempz) + 64;
	screen_y = (enemy.y << 5) / (tempz) + 40;

	if(screen_x > 0 - 64 && screen_x < 240 && screen_y > 0 - 64 && screen_y < 160)
	{
		zoom = 64 + (tempz << 3);
		
		RotateSprite(sprite, 0,zoom,zoom);				// zoom the sprite
		MoveSprite(&sprites[sprite],screen_x,screen_y);	// set the sprite at new position
	}
	else
	{
		MoveSprite(&sprites[OAM_OBJECT_OFFSET + sprite],240,160);	//set the sprite out of the screen
	}
}



/*
=================
= drawEnemy
=================
*/
void drawEnemy(void)
{
	int tempz;


	tempz = enemy.z - player.z;

	if(tempz > 4) 
	{
			drawEnemySprite(SPR_ENEMY);
	}
	else
	{
		if(enemy.z < player.z - 2) enemy.z = player.z - 2;

		enemy.x =  ((rand()%3)*32) - 32;
		enemy.y =  ((rand()%3)*32) - 32;

		RotateSprite(SPR_ENEMY, 0,256,256);				// zoom the sprite
		MoveSprite(&sprites[SPR_ENEMY],240,160);	// set the sprite at new position
	}
}


/*
=================
= doEnemy
=================
*/
void doEnemy(void)
{
	drawEnemy();
	
	if(player.z < TRACK_MAXZ && enemy.z < TRACK_MAXZ)
	moveEnemyForward(1);
}

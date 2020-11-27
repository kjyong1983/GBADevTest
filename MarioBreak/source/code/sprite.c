#include "lib/gba.h"
#include "lib/sprite.h"
#include "lib/sincosrad.h"


int		offsetOAM = 0;

u16* OAM = (u16*)0x7000000;

//create the array of sprites (128 is the maximum)
OAMEntry sprites[128];

//create the rotation and scaling array (overlaps the OAMEntry array memory)
pRotData rotData = (pRotData)sprites;

s32 angle = 0;
s32 zoom = 1<<8;

extern void WaitForVsync()
{
	while((volatile u16)REG_VCOUNT != 160){}
}

///Copy our sprite array to OAM
extern void CopyOAM()
{
	u16 loop;
	u16* temp;
	temp = (u16*)sprites;
	for(loop = 0; loop < 128*4; loop++)
	{
		OAM[loop] = temp[loop];
	}
}



//Set sprites to off screen
extern void InitializeSprites()
{
	int loop;
	for(loop = 0; loop < 128; loop++)
	{
		sprites[loop].attribute0 = 160;  //y to > 159
		sprites[loop].attribute1 = 240;  //x to > 239
	}

	offsetOAM = 0;
}



extern void initSprite(int spr,int size,int offset)
{
	int 	x,y;

	x = 240;
	y = 160;

	// set sprite offscreen, and set it up (size,etc)
	sprites[spr].attribute0 = COLOR_256 | SQUARE | x;
	sprites[spr].attribute1 = size | y;
	sprites[spr].attribute2 = 512 + offset; // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
}

extern void initRotSprite(int spr,int size,int offset)
{
	int 	x,y;

	x = 240;
	y = 160;

	// set sprite offscreen, and set it up (size,etc)
	sprites[spr].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | 0x200 | x;
	sprites[spr].attribute1 = size | ROTDATA(spr) | y;
	sprites[spr].attribute2 = 512 + offset; // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
}


extern void updateSprite(int spr,int offset,int sprsize, int ani)
{
	int frame;
	int size;

	size = sprsize * 2;

	frame = size * ani;

	sprites[spr].attribute2 = (512 + offset) + frame; // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
}



//move the sprite
extern void MoveSprite(OAMEntry* sp, int x, int y)
{
	if(x < 0)			//if it is off the left correct
		x = 512 + x;
	if(y < 0)			//if off the top correct
		y = 256 + y;

	sp->attribute1 = sp->attribute1 & 0xFE00;  //clear the old x value
	sp->attribute1 = sp->attribute1 | x;

	sp->attribute0 = sp->attribute0 & 0xFF00;  //clear the old y value
	sp->attribute0 = sp->attribute0 | y;
}

extern void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale)
{

	s32 pa,pb,pc,pd;

	pa = ((x_scale) * (s32)COS[angle])>>8;    //(do my fixed point multiplies and shift back down)
	pb = ((y_scale) * (s32)SIN[angle])>>8;
	pc = ((x_scale) * (s32)-SIN[angle])>>8;
	pd = ((y_scale) * (s32)COS[angle])>>8;

	rotData[rotDataIndex].pa = pa;  //put them in my data struct
	rotData[rotDataIndex].pb = pb;
	rotData[rotDataIndex].pc = pc;
	rotData[rotDataIndex].pd = pd;
}



/////////////////////////////////////////////////////////////
// Hugo's own special part

extern void loadSpritePal(unsigned int * pal)
{
	int 	x;

	for(x = 0; x < 256; x++)
		OBJPaletteMem[x] = ((unsigned short*)pal)[x];
}

extern void loadSpriteGraphics(unsigned int * gfx,int size)
{ 
	int 	x;
	int		offset;

	//offset = size * place;

	for(x = 0; x < size; x++)
      	OAMData[(8192 + offsetOAM) + x] = ((unsigned short*)gfx)[x];

	offsetOAM +=size;
}

extern void resetOffset(void)
{
	offsetOAM = 0;
}




////////////////////////////////////////////////////////////////////////
// File: 	sprite.h                                              	  //
// Description: contains all the definitions and structures necessary //
//		for displaying sprites on the GBA		      				  //
// Author:	dovoto (modified by gbajunkie)	                      	  //
// Date: 	7th February 2002                                    	  //
////////////////////////////////////////////////////////////////////////
//#include "craft.h"

#ifndef SPRITE_H
#define SPRITE_H

//define the screen width and height values to be used
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT   160

//colour convertion (converts a RGB colour to a 15-bit BGR value used by the GBA)
#define RGB16_BGR(r, g, b) ((r)+(g<<5)+(b<<10))

//attribute0 #defines
#define ROTATION_FLAG		0x100
#define SIZE_DOUBLE			0x200
#define MODE_NORMAL			0x0
#define MODE_TRANSPARENT	0x400
#define MODE_WINDOWED		0x800
#define MOSAIC				0x1000
#define COLOR_16			0x0000
#define COLOR_256			0x2000
#define SQUARE				0x0
#define TALL				0x4000
#define WIDE				0x8000

//attribute1 #defines
#define ROTDATA(n)			((n)<<9)
#define HORIZONTAL_FLIP		0x1000
#define VERTICAL_FLIP		0x2000
#define SIZE_8				0x0
#define SIZE_16				0x4000
#define SIZE_32				0x8000
#define SIZE_64				0xC000

//atrribute2 #defines
#define PRIORITY(n)	        ((n)<<10)
#define PALETTE(n)			((n)<<12)

//#define PI 3.14159
//#define RADIAN(n) 		(((float)n)/(float)180*PI)


//sprite structure definitions
typedef struct tagOAMEntry
{

	u16 attribute0;
	u16 attribute1;
	u16 attribute2;
	u16 attribute3;

}OAMEntry, *pOAMEntry;

typedef struct tagRotData
{

	u16 filler1[3];
	u16 pa;
	u16 filler2[3];
	u16 pb;
	u16 filler3[3];
	u16 pc;
	u16 filler4[3];
	u16 pd;

}RotData, *pRotData;

typedef struct
{

	u16 x;
	u16 y;
	u16 OAMSpriteNum;
	u16 *SpriteData;

}Sprite, *pSprite;

//extern u16* OAMData;
extern u16* OAM;
extern OAMEntry sprites[128];
extern pRotData rotData;
//extern pRotData rotData[128];

extern s32 angle;
extern s32 angle2;
extern s32 zoom;

extern void WaitForVsync();
extern void SetSpriteInfo(pSprite spriteName, s16 x, s16 y, int size);
extern void CopyOAM();
extern void InitializeSprites();
extern void MoveSprite(OAMEntry* sp, int x, int y);
extern void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale);

#endif




//////////////////////////
// menu graphics


//////////////////////////
// background graphics

#include "bg/bg_castle.c"
#include "bg/bg_underground.c"
#include "bg/bg_volly.c"
#include "bg/bg_sky.c"
#include "bg/bg_space.c"


#include "bg/bg_intro.c"
#include "bg/bg_gbaparty.c"
#include "bg/bg_shout.c"

//////////////////////////
// sprite graphics


// enemies
#include "sprites/enemy_goomba.h"
#include "sprites/enemy_bullet.h"
#include "sprites/enemy_shyguy.h"
#include "sprites/enemy_tweeter.h"


// pickup items
#include "sprites/item_shgreen.h"
#include "sprites/item_shred.h"
#include "sprites/item_coin.h"

// static items
#include "sprites/item_hardbox.h"
#include "sprites/item_softbox.h"

#include "sprites/item_block.h"
#include "sprites/item_lava.h"

// moveable items
#include "sprites/item_fireball.h"


// characters
#include "sprites/mario_walk.h"

// pointer
#include "sprites/pointer.h"
#include "sprites/canon.h"



//////////////////////////
// palette graphics



//////////////////////////
// OAM defines

#define		OAM_MARIO_WALK		(0)
#define		OAM_ITEM_FIREBALL	(32)

#define		OAM_ITEM_HARDBOX	(40)
#define		OAM_ITEM_SOFTBOX	(48)

#define		OAM_ENEMY_GOOMBA 	(56)
#define		OAM_ENEMY_BULLET	(72)
#define		OAM_ENEMY_SHYGUY	(88)
#define		OAM_ENEMY_TWEETER	(104)

#define		OAM_ITEM_SHGREEN	(120)
#define		OAM_ITEM_SHRED		(128)
#define		OAM_ITEM_COIN		(136)

#define		OAM_ITEM_BLOCK		(144)
#define		OAM_ITEM_LAVA		(152)

#define		OAM_POINTER			(168)

#define		OAM_CANON			(176)
/*
 so how does this work ? easy :

 take the size of the bitmap, divide it by 8; then divide it by 2.
 and that's the OAM point the graphics are stored.

 so for example :


 16x16 = 128	  128 / 8 = 16   , 16 / 2 = 8
 32x32 = 512      512 / 8 = 64   , 64 / 2 = 32
 64x64 = 4096     4096 / 8 = 512 , 512 / 2 = 256
 
*/







//{{BLOCK(gameover)

//======================================================================
//
//	gameover, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 89 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 2848 + 2048 = 5408
//
//	Time-stamp: 2021-06-05, 16:43:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GAMEOVER_H
#define GRIT_GAMEOVER_H

#define gameoverTilesLen 2848
extern const unsigned short gameoverTiles[1424];

#define gameoverMapLen 2048
extern const unsigned short gameoverMap[1024];

#define gameoverPalLen 512
extern const unsigned short gameoverPal[256];

#endif // GRIT_GAMEOVER_H

//}}BLOCK(gameover)

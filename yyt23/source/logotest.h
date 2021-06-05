
//{{BLOCK(logotest)

//======================================================================
//
//	logotest, 512x256@4, 
//	+ palette 256 entries, not compressed
//	+ 80 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x32 
//	Total size: 512 + 2560 + 4096 = 7168
//
//	Time-stamp: 2021-06-05, 16:32:57
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LOGOTEST_H
#define GRIT_LOGOTEST_H

#define logotestTilesLen 2560
extern const unsigned short logotestTiles[1280];

#define logotestMapLen 4096
extern const unsigned short logotestMap[2048];

#define logotestPalLen 512
extern const unsigned short logotestPal[256];

#endif // GRIT_LOGOTEST_H

//}}BLOCK(logotest)

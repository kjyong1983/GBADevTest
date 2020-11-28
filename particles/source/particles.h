
//{{BLOCK(particles)

//======================================================================
//
//	particles, 64x16@4, 
//	+ palette 16 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 32 + 512 = 544
//
//	Time-stamp: 2017-11-08, 11:55:36
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PARTICLES_H
#define GRIT_PARTICLES_H

#include "gba_macros.h"
#include "gba_types.h"

#define particlesTilesLen 512
extern const u16 particlesTiles[256];

#define particlesPalLen 32
extern const u16 particlesPal[16];

#endif // GRIT_PARTICLES_H

//}}BLOCK(particles)

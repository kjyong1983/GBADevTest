#ifndef BN_SPRITE_ITEMS_DINO_H
#define BN_SPRITE_ITEMS_DINO_H

#include "bn_sprite_item.h"

//{{BLOCK(dino_bn_graphics)

//======================================================================
//
//	dino_bn_graphics, 64x64@4, 
//	+ palette 16 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 32 + 2048 = 2080
//
//	Time-stamp: 2020-12-02, 11:02:28
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_DINO_BN_GRAPHICS_H
#define GRIT_DINO_BN_GRAPHICS_H

#define dino_bn_graphicsTilesLen 2048
extern const bn::tile dino_bn_graphicsTiles[512 / (sizeof(bn::tile) / sizeof(uint32_t))];

#define dino_bn_graphicsPalLen 32
extern const bn::color dino_bn_graphicsPal[16];

#endif // GRIT_DINO_BN_GRAPHICS_H

//}}BLOCK(dino_bn_graphics)

namespace bn::sprite_items
{
    constexpr const sprite_item dino(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            span<const tile>(dino_bn_graphicsTiles), 
            span<const color>(dino_bn_graphicsPal, 16), bpp_mode::BPP_4, 1);
}

#endif


#ifndef BN_SPRITE_ITEMS_BLONDE_H
#define BN_SPRITE_ITEMS_BLONDE_H

#include "bn_sprite_item.h"

//{{BLOCK(blonde_bn_graphics)

//======================================================================
//
//	blonde_bn_graphics, 64x64@4, 
//	+ palette 16 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 32 + 2048 = 2080
//
//	Time-stamp: 2020-12-02, 23:23:03
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BLONDE_BN_GRAPHICS_H
#define GRIT_BLONDE_BN_GRAPHICS_H

#define blonde_bn_graphicsTilesLen 2048
extern const bn::tile blonde_bn_graphicsTiles[512 / (sizeof(bn::tile) / sizeof(uint32_t))];

#define blonde_bn_graphicsPalLen 32
extern const bn::color blonde_bn_graphicsPal[16];

#endif // GRIT_BLONDE_BN_GRAPHICS_H

//}}BLOCK(blonde_bn_graphics)

namespace bn::sprite_items
{
    constexpr const sprite_item blonde(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            span<const tile>(blonde_bn_graphicsTiles), 
            span<const color>(blonde_bn_graphicsPal, 16), bpp_mode::BPP_4, 1);
}

#endif


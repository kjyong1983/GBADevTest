#ifndef BN_REGULAR_BG_ITEMS_CLOUDS_H
#define BN_REGULAR_BG_ITEMS_CLOUDS_H

#include "bn_regular_bg_item.h"

//{{BLOCK(clouds_bn_graphics)

//======================================================================
//
//	clouds_bn_graphics, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 111 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 3552 + 2048 = 5632
//
//	Time-stamp: 2020-12-02, 11:02:28
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CLOUDS_BN_GRAPHICS_H
#define GRIT_CLOUDS_BN_GRAPHICS_H

#define clouds_bn_graphicsTilesLen 3552
extern const bn::tile clouds_bn_graphicsTiles[888 / (sizeof(bn::tile) / sizeof(uint32_t))];

#define clouds_bn_graphicsMapLen 2048
extern const bn::regular_bg_map_cell clouds_bn_graphicsMap[1024];

#define clouds_bn_graphicsPalLen 32
extern const bn::color clouds_bn_graphicsPal[16];

#endif // GRIT_CLOUDS_BN_GRAPHICS_H

//}}BLOCK(clouds_bn_graphics)

namespace bn::regular_bg_items
{
    constexpr const regular_bg_item clouds(span<const tile>(clouds_bn_graphicsTiles), 
            span<const color>(clouds_bn_graphicsPal, 16), bpp_mode::BPP_4, 
            clouds_bn_graphicsMap[0], size(32, 32));
}

#endif


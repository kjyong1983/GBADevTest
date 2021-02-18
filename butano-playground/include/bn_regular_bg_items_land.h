#ifndef BN_REGULAR_BG_ITEMS_LAND_H
#define BN_REGULAR_BG_ITEMS_LAND_H

#include "bn_regular_bg_item.h"

//{{BLOCK(land_bn_graphics)

//======================================================================
//
//	land_bn_graphics, 512x512@8, 
//	+ palette 256 entries, not compressed
//	+ 386 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 24704 + 8192 = 33408
//
//	Time-stamp: 2020-12-02, 11:02:28
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LAND_BN_GRAPHICS_H
#define GRIT_LAND_BN_GRAPHICS_H

#define land_bn_graphicsTilesLen 24704
extern const bn::tile land_bn_graphicsTiles[6176 / (sizeof(bn::tile) / sizeof(uint32_t))];

#define land_bn_graphicsMapLen 8192
extern const bn::regular_bg_map_cell land_bn_graphicsMap[4096];

#define land_bn_graphicsPalLen 512
extern const bn::color land_bn_graphicsPal[256];

#endif // GRIT_LAND_BN_GRAPHICS_H

//}}BLOCK(land_bn_graphics)

namespace bn::regular_bg_items
{
    constexpr const regular_bg_item land(span<const tile>(land_bn_graphicsTiles), 
            span<const color>(land_bn_graphicsPal, 96), bpp_mode::BPP_8, 
            land_bn_graphicsMap[0], size(64, 64));
}

#endif



#include <stdio.h>
#include <string.h>
// #include <tonc.h>

#include "gba_macros.h"
#include "gba_types.h"
#include "gba_bios.h"
#include "gba_gfx.h"
#include "gba_drawing.h"
#include "gba_mathUtil.h"
#include "gba_input.h"

#include "BG_Externs.h"

typedef struct{
	u32 data[8];
} HALF_ROW;

void copy64x32MapIntoMemory( const u16* mapData, u16 mapBlockAddress){
	HALF_ROW* src = (HALF_ROW*)mapData;
	HALF_ROW* dst0 = (HALF_ROW*)getBGMapBlock(mapBlockAddress);
	HALF_ROW* dst1 = (HALF_ROW*)getBGMapBlock(mapBlockAddress + 1);

	for (u32 i = 0; i < 32; ++i)
	{
		*dst0++ = *src++;
		*dst0++ = *src++;
		*dst1++ = *src++;
		*dst1++ = *src++;
	}
}

u32 loopCounter = 0;

int main(){
	register_vblank_isr();
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPING_MODE_1D | BGMODE_0 | BGMODE_1;
	memcpy(PAL_BG_MEM, bgPalette, bgPaletteSize);
	u16* tileblock = getBGTileBlock(0);
	memcpy(tileblock, bgTiles, bgTilesSize);
	copy64x32MapIntoMemory(bgMapLayer0, 16);
	copy64x32MapIntoMemory(bgMapLayer1, 18);

	REG_BGCNT[0] = setBGControlRegister(1,0,0,0,16,0,BG_REG_SIZE_64x32);
	REG_BGCNT[1] = setBGControlRegister(0,0,0,0,18,0,BG_REG_SIZE_64x32);

	s32 x = 0;
	s32 y = 0;

	while (1)
	{
		vblank_intr_wait();
		PollKeys();
		
		s32 countVal = vblank_Counter;
		loopCounter++;
		
		x += getAxis(HORIZONTAL);
		y -= getAxis(VERTICAL);

		REG_BG_OFFSET[0].x = x;
		REG_BG_OFFSET[0].y = y;
		REG_BG_OFFSET[1].x = x;
		REG_BG_OFFSET[1].y = y;
	}
	
	return 0;
}


// int main()
// {
// 	// Init interrupts and VBlank irq.
// 	irq_init(NULL);
// 	irq_add(II_VBLANK, NULL);
	
// 	// Video mode 0, enable bg 0.
// 	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;
	
// 	// Init 4bpp vwf text on bg 0.
// 	tte_init_chr4c(0, 			// BG 0
// 		BG_CBB(0)|BG_SBB(31),	// Charblock 0; screenblock 31
// 		0xF000,					// Screen-entry offset
// 		bytes2word(1,2,0,0),	// Color attributes.
// 		CLR_YELLOW, 			// Yellow text
// 		&verdana9Font,			// Verdana 9 font
// 		NULL					// Use default chr4 renderer
// 		);
	
// 	// Initialize use of stdio.
// 	tte_init_con();
	
// 	// Printf something at 96,72
// 	tte_printf("#{P:96,72}Hello World!");
	
// 	while(1)
// 	{
// 		VBlankIntrWait();
// 	}
	
// 	return 0;
// }

#ifndef _GBA_H_
#define _GBA_H_

// Typedef ------------------------------------------------------
typedef	unsigned	short	hword;	// 16bit integer
typedef	unsigned	char	byte;	//  8bit integer

// Address ------------------------------------------------------
#define	VRAM		0x06000000	// VRAM frame buffer address
#define	BG_PALETTE	0x05000000	// BG palette base address
#define	OBJ_PALETTE	0x05000200	// OBJ palette base address
#define IO_BASE	0x04000000		// IO base address
#define VRAM_TILE(n)	(VRAM + n * 0x4000)	// Tile data base address
#define VRAM_MAP(n)		(VRAM + n * 0x0800)	// Map data base address
#define LCD_CTL		(IO_BASE + 0)	// LCD control register address
#define BG0_CTL		(IO_BASE + 8)	// BG0 control register address
#define BG1_CTL		(IO_BASE + 10)	// BG1 control register address
#define BG2_CTL		(IO_BASE + 12)	// BG2 control register address
#define BG3_CTL		(IO_BASE + 14)	// BG3 control register address
#define KEY_STA		(IO_BASE + 304)	// Key status register address
#define	BG0_HOFFSET	(IO_BASE + 16)	// BG0 horizontal offset
#define	BG0_VOFFSET	(IO_BASE + 18)	// BG0 vertical offset
#define	BG1_HOFFSET	(IO_BASE + 20)	// BG0 horizontal offset
#define	BG1_VOFFSET	(IO_BASE + 22)	// BG0 vertical offset
#define	BG2_HOFFSET	(IO_BASE + 24)	// BG0 horizontal offset
#define	BG2_VOFFSET	(IO_BASE + 26)	// BG0 vertical offset
#define	BG3_HOFFSET	(IO_BASE + 28)	// BG0 horizontal offset
#define	BG3_VOFFSET	(IO_BASE + 30)	// BG0 vertical offset
#define OAM			0x07000000		// Object attribute memory address
#define OBJ_DATA0	0x06010000		// Characyer data address of mode 0
#define OBJ_DATA3	0x06014000		// Characyer data address of mode 3
#define TIMER_CTL0	(IO_BASE + 258)		// Timer 0 conrol register address
#define TIMER_CTL1	(IO_BASE + 262)		// Timer 1 conrol register address
#define TIMER_CTL2	(IO_BASE + 266)		// Timer 2 conrol register address
#define TIMER_CTL3	(IO_BASE + 270)		// Timer 3 conrol register address
#define TIMER_CNT0	(IO_BASE + 256)		// Timer 0 data register address
#define TIMER_CNT1	(IO_BASE + 260)		// Timer 1 data register address
#define TIMER_CNT2	(IO_BASE + 264)		// Timer 2 data register address
#define TIMER_CNT3	(IO_BASE + 268)		// Timer 3 data register address

// LCD control -----------------------------------------------
#define	LCD_BG0	0x0100	// Enable BG0 plane
#define	LCD_BG1	0x0200	// Enable BG1 plane
#define	LCD_BG2	0x0400	// Enable BG2 plane
#define	LCD_BG3	0x0800	// Enable BG3 plane
#define	LCD_OBJ	0x1000	// Enable OBJ
#define	LCD_MODE0	0	// LCD mode 0
#define	LCD_MODE1	1	// LCD mode 1
#define	LCD_MODE2	2	// LCD mode 2
#define	LCD_MODE3	3	// LCD mode 3
#define	LCD_MODE4	4	// LCD mode 4
#define	LCD_MODE5	5	// LCD mode 5

#define	LCD_SIZE00	0x0000	// BG size 32x32tiles
#define	LCD_SIZE01	0x4000	// BG size 64x32tiles
#define	LCD_SIZE10	0x8000	// BG size 32x64tiles
#define	LCD_SIZE11	0xC000	// BG size 64x64tiles
#define	LCD_COLOR16		0		// BG 16color x 16 palette
#define	LCD_COLOR256	0x0080	// BG 256color x 1 palette
#define LCD_OBJ_MAP1D	0x0040	// Object mapping method 1D

typedef struct {
	hword* map;		// Map data pointer
	hword* tile;	// Tile data pointer
	int x, y;		// Current position on each plane
} screen;

// Key control -----------------------------------------------
#define	KEY_L		0x200			// L
#define	KEY_R		0x100			// R
#define	KEY_A		0x001			// A
#define	KEY_B		0x002			// B
#define	KEY_DOWN	0x080			// Down
#define	KEY_UP		0x040			// Up
#define	KEY_LEFT	0x020			// Left
#define	KEY_RIGHT	0x010			// Right
#define	KEY_START	0x008			// Start
#define	KEY_SELECT	0x004			// Select
#define	KEY_ALL		0x3FF			// Any key

// Object control --------------------------------------------
#define OBJ_SHAPE_SQ	0x0000		// Shape: square
#define OBJ_SHAPE_HR	0x4000		// Shape: landscape
#define OBJ_SHAPE_VR	0x8000		// Shape: portrait
#define OBJ_COLOR_256	0x2000		// Object color mode: 256 colors
#define OBJ_Y_MASK	0x00FF			// Y coordinate bit mask
#define OBJ_SW_MASK	0x0300		// Object display switch bit mask
#define OBJ_SW_TURNOFF	0x0200		// Turn off object
#define OBJ_X_MASK	0x01FF			// X coordinate bit mask
#define OBJ_SIZE_MASK	0xC000		// Object size bit mask
#define OBJ_CHAR_MASK	0x03FF		// Character# bit mask
#define OBJ_PRIOR_MASK	0x0C00		// Object priority bit mask

#define OBJ_8x8		0				// Object size index
#define OBJ_16x16	1
#define OBJ_32x32	2
#define OBJ_64x64	3
#define OBJ_16x8	4
#define OBJ_32x8	5
#define OBJ_32x16	6
#define OBJ_64x32	7
#define OBJ_8x16	8
#define OBJ_8x32	9
#define OBJ_16x32	10
#define OBJ_32x64	11

#define PRIORITY_1	0			// Priority of object and background
#define PRIORITY_2	1
#define PRIORITY_3	2
#define PRIORITY_4	3

typedef struct {
	hword attr0;					// Object attribute 0
	hword attr1;					// Object attribute 1
	hword attr2;					// Object attribute 2
	hword pacd;					// Rotation, Scaling
} sprite;

// Timer control ---------------------------------------------
#define   TIMER_ENABLE	0x0080		// Timer enable
#define   TIMER_INTREN	0x0040		// Timer interrupt request enable
#define   TIMER_CASCADE	0x0004		// Cascade switch
#define   TIMER_PRE1	0x0000		// 1/1 prescalar
#define   TIMER_PRE64	0x0001		// 1/64 prescalar
#define   TIMER_PRE256	0x0002		// 1/256 prescalar
#define   TIMER_PRE1024	0x0003		// 1/1024 prescalar

// Macro -----------------------------------------------------
#define	gba_reg(p)	*((volatile hword*) p)	// access to conrtol registers
#define	RGB(r, g, b)	(((b) << 10) + ((g) << 5) + (r))	// 15bit RGB
#define	LCD_BGTILE(n)	(n << 2)			// BG tile block start index
#define	LCD_BGMAP(n)	(n << 8)			// BG map block start index

#endif
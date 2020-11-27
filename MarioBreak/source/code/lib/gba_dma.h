#ifndef __GBA_DMA_H
#define __GBA_DMA_H


#define DMA_ENABLE		0x80000000
#define DMA_IMMEDIATE	0x00000000

#define DMA_16			0x00000000
#define DMA_32			0x04000000

void DMACopy16(u8 channel, void *dest, void *source, u32 length);
void DMACopy32(u8 channel, void *dest, void *source, u32 length);

#endif//__GBA_DMA_H

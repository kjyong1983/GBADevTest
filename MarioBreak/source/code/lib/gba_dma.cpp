// DMA Transfer Channels

void DMACopy16(u8 channel, void *dest, void *source, u32 length)
{
	switch (channel)
	{
		case 0:
			REG_DMA0SAD = (u32)source;
			REG_DMA0DAD = (u32)dest;
			REG_DMA0CNT_L = length;
			//REG_DMA0CNT_H = DMA_ENABLE | DMA_16 | DMA_IMMEDIATE;
			REG_DMA0CNT_H = DMA_ENABLE | DMA_16 | DMA_IMMEDIATE;
			break;
		case 1:
			REG_DMA1SAD = (u32)source;
			REG_DMA1DAD = (u32)dest;
			REG_DMA1CNT_L = length;
			//REG_DMA1CNT_H = DMA_ENABLE | DMA_16 | DMA_IMMEDIATE;
			break;
		case 2:
			REG_DMA2SAD = (u32)source;
			REG_DMA2DAD = (u32)dest;
			REG_DMA2CNT_L = length;
			//REG_DMA2CNT_H = DMA_ENABLE | DMA_16 | DMA_IMMEDIATE;
			break;
		case 3:
			REG_DMA3SAD = (u32)source;
			REG_DMA3DAD = (u32)dest;
			REG_DMA3CNT_L = length;
			//REG_DMA3CNT_H = DMA_ENABLE | DMA_16 | DMA_IMMEDIATE;
			break;
	}
}

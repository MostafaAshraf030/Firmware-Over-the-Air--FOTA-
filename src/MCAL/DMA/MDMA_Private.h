#ifndef MDMA_PRIVATE_H
#define MDMA_PRIVATE_H

#define MDMA1_BASE_ADDRESS	0x40026000
#define MDMA2_BASE_ADDRESS	0x40026400	


typedef struct
{
	volatile u32 CR;
	volatile u32 NDTR;
	volatile u32 PAR;
	volatile u32 M0AR;
	volatile u32 M1AR;
	volatile u32 FCR;
}DMA_STREAM_t;

typedef struct
{
	volatile u32 LISR;
	volatile u32 HISR;
	volatile u32 LIFSR;
	volatile u32 HIFSR;
	volatile DMA_STREAM_t STREAMS[8];
}DMA_t;


#define MDMA1 ((volatile DMA_t *)(MDMA1_BASE_ADDRESS))
#define MDMA2 ((volatile DMA_t *)(MDMA2_BASE_ADDRESS))

#endif
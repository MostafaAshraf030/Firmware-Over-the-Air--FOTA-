#include "../../SERVICES/BIT_math.h"
#include "../../SERVICES/STD_TYPES.h"

#include "MDMA_Interface.h"
#include "MDMA_Private.h"
#include "MDMA_Config.h"
#define NULL 0

static void (* Global_MDMA_voidCallBackPtr)(void) = NULL;



void MDMA_voidInit(u8 Copy_u8StreamId, u32 Copy_u32ChannelId)
{
	if (GET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 0) == 1)
	{
		/* Disbale EN bit to configure DMA successfully */
		CLR_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 0);
	}
	else
	{
		// DO NOTHING
	}
	
	/* set the passed Channel */			
	MDMA2->STREAMS[Copy_u8StreamId].CR |= (Copy_u32ChannelId << 25);

/* Set Priority Level -> Very-High */
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 17);
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 16);
	
	/* Set Memory size -> 32-bits */
	CLR_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 14);
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 13);
	
	/* Set Peripheral size -> 32-bits */
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 12);
	CLR_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 11);

/* Set Priority Level -> Very-High */
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 17);
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 16);
	
	/* Enable Circuilar Mode */
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 8);
	
/* Set Data Direction -> Memory-To-Memory */
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 7);
	CLR_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 6);
	
/* Disable Direct Mode */	
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].FCR, 2);
	
/* Use Full FIFO threshold */	
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].FCR, 1);
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].FCR, 0);
	
/* Enable DMA2 Transfer interrupt complete */
MDMA2->STREAMS[Copy_u8StreamId].CR |= (1<<4UL);
	
}

void MDMA_voidSetAddresses(u32 *Copy_u32SrcAddress, u32 *Copy_u32DestAddress, u32 Copy_u32BlockSize, u8 Copy_u8StreamId)
{
	MDMA2->STREAMS[Copy_u8StreamId].PAR = Copy_u32SrcAddress;
	MDMA2->STREAMS[Copy_u8StreamId].M0AR = Copy_u32DestAddress;
	MDMA2->STREAMS[Copy_u8StreamId].NDTR = Copy_u32BlockSize;
}

void MDMA_voidClearFlagChannel0(void)
{
	MDMA2->LIFSR |= (1<<0UL) | (1<<2UL) | (1<<3UL) | (1<<4UL) | (1<<5UL);
}

void MDMA_voidStreamEnable(u32 Copy_u8StreamId)
{
	/* clear Stream 0 Flags */
	MDMA_voidClearFlagChannel0();
	
	/* Enable Stream on DMA  */
	SET_BIT(MDMA2->STREAMS[Copy_u8StreamId].CR, 0);
}

void MDMA_voidCallBack(void(*Copy_voidPtrFunc)(void))
{
	Global_MDMA_voidCallBackPtr = Copy_voidPtrFunc;
}


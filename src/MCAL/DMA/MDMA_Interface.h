#ifndef MDMA_INTERFACE_H
#define MDMA_INTERFACE_H

/* Function to init stream number */
void MDMA_voidInit(u8 Copy_u8StreamId, u32 Copy_u32ChannelId);

/* Function to set addresses of soucre and destination and block size */
void MDMA_voidSetAddresses(u32 *Copy_u32SrcAddress, u32 *Copy_u32DestAddress, u32 Copy_u32BlockSize, u8 Copy_u8StreamId);

/* Function to Clear flag of channel 0 */
void MDMA_voidClearFlagChannel0(void);

/* Function To enable stream with its ID */
void MDMA_voidStreamEnable(u32 Copy_u8StreamId);

/* MDMA Call-Back function */
void MDMA_voidCallBack(void(*Copy_voidPtrFunc)(void));

#endif
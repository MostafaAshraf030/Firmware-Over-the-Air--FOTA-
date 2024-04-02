/*
 * FOTA.c
 *
 *  Created on: Jun 16, 2023
 *      Author: abdullah
 */

#include"../SERVICES/BIT_math.h"
#include"../SERVICES/STD_TYPES.h"
#include"../MCAL/MRCC/MRCC_Inteface.h"
#include"../MCAL/DIO/MGPIO_Interface.h"
#include"../MCAL/MUSART/MUSART_Interface.h"
#include"../MCAL/BootLoader/APARSER_Interface.h"
#include"../MCAL/STK/MSTK_Interface.h"
#include"../MCAL/NVIC/MNVIC_Interface.h"
#include"../MCAL/BootLoader/MFDI_Interface.h"

#include"../HAL/HESP/ESP_interface.h"
#include"FOTA_interface.h"
extern u8 HEX_u8chars[30000];
extern u32 HEX_u32charCounter;
extern u8 endOFrecordFlag;
extern u8 eraseFLAG;
u8 EndOfFileFlag =0;
void App_voidTest(void)
{
#define NULL 0
	void(*jumptoResetISR)(void)=NULL;

	/*Move Vector Table*/
#define SCB_VTOR       *((volatile u32*)0xE000ED08)

	SCB_VTOR = 0x08004000 ; //Starting of Sector 1 [Application]

	/*Set Address to call with Reset Handeler_ISR [startup code of Application]*/
	jumptoResetISR= 0x08004004 ;

	jumptoResetISR() ; //jump Reset handler [startup code]
}

void main (void)
{
	FOTA_init();

	while(1)
	{
		if(EndOfFileFlag==0)
		{
			MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN0,MGPIO_HIGH_VALUE) ;
		connect_parseONErecord();
		}
		else if(EndOfFileFlag==1)
		{
			MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN0,MGPIO_LOW_VALUE) ;
			/*Wait 1 sec then jump to Application*/
			MSTK_voidSetIntervalSingle(1000000,App_voidTest) ;
			/*Indicate to Successful Flashing*/
			MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN0,MGPIO_LOW_VALUE) ;




		}


	}




}





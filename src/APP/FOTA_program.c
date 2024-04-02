/*
 * FOTA_program.c
 *
 *  Created on: Jun 17, 2023
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
u8 HEX_u8chars[30000]={0};
u32 HEX_u32charCounter=0;
u8 endOFrecordFlag=0;
u8 eraseFLAG=0;
extern  u8 EndOfFileFlag;





void FOTA_init(void)
{
	MRCC_voidInitSystemClk();
	/*Step 2 : Enable GPIO Peripherial Clock For Port A*/
	MRCC_voidEnablePeripheralClock(AHB1 ,0UL);
	MGPIO_u8SetPinMode(MGPIOA_PORT,MGPIO_PIN0,MGPIO_MODE_OUTPUT);
	MGPIO_u8SetOutputSpeed(MGPIOA_PORT,MGPIO_PIN0,MGPIO_SPEED_LOW);
	MGPIO_u8SetOutputType(MGPIOA_PORT,MGPIO_PIN0,MGPIO_OUTPUT_PUSH_PULL);

	/*Step 3 : Enable USART1 Peripherial Clock */
	MRCC_voidEnablePeripheralClock(APB2,4);
	/*Step 4 : Set Pin Mode For Pins 9 , 10 -> Alternative Function */
	MGPIO_u8SetPinMode(MGPIOA_PORT,MGPIO_PIN9,MGPIO_MODE_ALTF) ;    //TX-> USART1
	MGPIO_u8SetPinMode(MGPIOA_PORT,MGPIO_PIN10,MGPIO_MODE_ALTF) ;   //RX-> USART1


	/*Step 5 : Set Pin Alternative function For Pins 9 , 10  */
	MGPIO_voidSetPinAltFun(MGPIOA_PORT,MGPIO_PIN9,7) ;    //TX-> USART1
	MGPIO_voidSetPinAltFun(MGPIOA_PORT,MGPIO_PIN10,7) ;   //RX-> USART1
	/* Enable NVIC For USART1 */
	/*Step 6 : Enable USART From MNVIC*/
	MNVIC_voidSetInterruptGroupMode(MNVIC_GROUPMODE_G16S0)   ;
	MNVIC_voidSetInterruptPriority( 37 , MNVIC_GROUPMODE_G16S0 , 0 , 0 ) ;
	MNVIC_voidEnableInterrupt(37) ;//for uart
	MSTK_voidInit();
	MUSART_voidInit();
	MUSART_voidEnable() ;
	/*we need call back for systick*/

	//MSTK_voidInit() ; /*disable STK , Interrupt , Tick = 0.5 Micro second --> HSI/8 --> 2MHz*/

	//MSTK_voidSetIntervalSingle(300000000,App_voidTest) ;
	//------

	ESP8266_voidInit();
	ESP8266_voidConnectToWiFi( (u8 *)"Abumsalam" , (u8 *)"abomosallam" );
	/////////////////////init function
}

void connect_parseONErecord(void)
{
	u8 loc_u8HEXdata=0;

	while(endOFrecordFlag==0)
	{
		/* Connecting To hamada.freevar.com */
		ESP8266_voidConnectToServerTcp( (u8 *)"fawzi.freetzi.com" , (u8 *)"80" );

		/* Connecting To hamada.freevar.com  change input paramters for wifi and website &the number &txt file*/
		loc_u8HEXdata = ESP8266_u8ReceiveHttpReq( (u8 *)"fawzi.freetzi.com" , (u8 *)"41" );
		HEX_u8chars[HEX_u32charCounter]=loc_u8HEXdata;
		HEX_u32charCounter++;
		ESP8266_voidClearBuffer();
		if (loc_u8HEXdata=='/n')/* check over end of record */
		{
			endOFrecordFlag=1;
			if(eraseFLAG==0)
			{
				MFDI_voidEraseAppArea();
				eraseFLAG=1;
			}

			APARSER_voidParseRecord(HEX_u8chars);
			/*check the end of file*/
			if(HEX_u8chars[8]==1)
			{

				EndOfFileFlag=1;
				MNVIC_voidDisableInterrupt(37);
			}

			// clear record array to be able to receive a new record
			for(u32 i=0;i<HEX_u32charCounter;i--)
			{

				HEX_u8chars[i]=0;

			}
			HEX_u32charCounter=0;

		}

	}



}



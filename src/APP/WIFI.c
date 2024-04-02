/*
 * WIFI.c
 *
 *  Created on: Jun 13, 2023
 *      Author: abdullah
 */



#include"../SERVICES/BIT_math.h"
#include"../SERVICES/STD_TYPES.h"
#include"../MCAL/DIO/MGPIO_Interface.h"
#include"../MCAL/MRCC/MRCC_Inteface.h"
#include"../MCAL/NVIC/MNVIC_Interface.h"
#include"../HAL/HESP/ESP_interface.h"
void main()
{

	/* Create Variable To Receive Data From Server */
	u8 LOC_u8Data = 0;
		/*Step 1 : System Clock is 16 MHz From HSI*/
	MRCC_voidInitSystemClk();

	/*Step 2 : Enable GPIO Peripherial Clock For Port A*/
	MRCC_voidEnablePeripheralClock(AHB1,0);

	/*Step 3 : Enable USART1 Peripherial Clock */
	MRCC_voidEnablePeripheralClock(APB2,4);


	MGPIO_u8SetPinMode( MGPIOA_PORT , MGPIO_PIN0 , MGPIO_MODE_OUTPUT );
	MGPIO_u8SetOutputType( MGPIOA_PORT , MGPIO_PIN0 , MGPIO_OUTPUT_PUSH_PULL );
	MGPIO_u8SetOutputSpeed( MGPIOA_PORT , MGPIO_PIN0 , MGPIO_SPEED_MEDUIM);


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


	MNVIC_voidEnableInterrupt(37) ;//for uart global interrupt

	/*Step 7 : MUSART Initialization*/
	MUSART_voidInit() ; /*115200bps , 1 stop bits  , no parity , 8  bit data*/

	MUSART_voidEnable() ;

	/* Step 8 :Setting ESP8266 Mode */
	ESP8266_voidInit();

	/* Step 9 :Connecting To WIFI Network */
	ESP8266_voidConnectToWiFi( (u8 *)"Abumsalam" , (u8 *)"abomosallam" );





	while(1){

		/* Connecting To hamada.freevar.com */
		ESP8266_voidConnectToServerTcp( (u8 *)"armiti.freetzi.com" , (u8 *)"80" );

		/* Connecting To hamada.freevar.com */
		LOC_u8Data = ESP8266_u8ReceiveHttpReq( (u8 *)"armiti.freetzi.com" , (u8 *)"41" );



		if (LOC_u8Data=='1')
		{/* We Receive ASCII So We Will Subtract 48 From Data Come */
		MGPIO_u8SetPinValue( MGPIOA_PORT , 0 , MGPIO_HIGH_VALUE );

		}
		else if (LOC_u8Data=='0')
		{		MGPIO_u8SetPinValue( MGPIOA_PORT , 0 , MGPIO_LOW_VALUE );
}

	}

}

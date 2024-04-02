/****************************************************************/
/* Author    : Ahmed Tarek                                      */
/* Date      : 28 SEP 2020                                      */
/* Version   : V01                                              */
/****************************************************************/


#include"../../SERVICES/BIT_math.h"
#include"../../SERVICES/STD_TYPES.h"
#include "../../MCAL/STK/MSTK_Interface.h"
#include "../../MCAL/MUSART/MUSART_Interface.h"

#include "ESP_interface.h"
u8 Global_u8Response[200] = {0};
u8 	Iterator     = 0 ;


void MUSART_CallBack ( void ){

	/* Receive ESP8266 Response */
	Global_u8Response[ Iterator ] = MUSART_u8ReadDataAsynch();
	/* ------------------ */
	Iterator++;

}

void ESP8266_voidInit(void)
{
	/* Set USART1 CallBack To Receive The Response Of The ESP8266 */
		MUSART_voidCallBack( MUSART_CallBack );

		/* Sending AT Command To Check ESP8266 Is Working Or Not  */
		MUSART_voidSendData( (u8 *)"AT\r\n",4 );//just 4 char because i do not care about the response

		MSTK_voidDelayMs( 3000);

		ESP8266_voidClearBuffer();



		/* Clear ESP8266 Buffer */

		MUSART_voidSendData( (u8 *)"AT+CWMODE=1\r\n", 13);
		MSTK_voidDelayMs( 3000);

		ESP8266_voidClearBuffer();


}


void ESP8266_voidConnectToWiFi ( u8 * SSID , u8 * Password ){

	ESP8266_voidClearBuffer();

	MUSART_voidSendString( (u8 *) "AT+CWJAP_CUR=\"" );
	MUSART_voidSendString( (u8 *) SSID );
	MUSART_voidSendString( (u8 *) "\",\"" );
	MUSART_voidSendString( (u8 *) Password);
	MUSART_voidSendString( (u8 *) "\"\r\n");
	MSTK_voidDelayMs( 8000);

}


void ESP8266_voidConnectToServerTcp ( u8 * Copy_u8Domain , u8 * Copy_u8Port ){

	ESP8266_voidClearBuffer();

	MUSART_voidSendString( (u8 *) "AT+CIPSTART=\"TCP\",\"" );
	MUSART_voidSendString( (u8 *) Copy_u8Domain );
	MUSART_voidSendString( (u8 *) "\"," );
	MUSART_voidSendString( (u8 *) Copy_u8Port );
	MUSART_voidSendString( (u8 *) "\r\n" );
	MSTK_voidDelayMs( 2000);

}

void ESP8266_VidSendHttpReq( u8 * Copy_u8Key , u8 * Copy_u8Data , u8 * Copy_u8Length ){

	ESP8266_voidClearBuffer();

	MUSART_voidSendString( (u8 *) "AT+CIPSEND=" );
	MUSART_voidSendString( (u8 *) Copy_u8Length );
	MUSART_voidSendString( (u8 *) "\r\n" );
	MSTK_voidDelayMs( 4000);

	ESP8266_voidClearBuffer();

	// with thing speak channel
	MUSART_voidSendString( (u8 *) "GET /update?api_key=" );
	MUSART_voidSendString( (u8 *) Copy_u8Key );
	MUSART_voidSendString( (u8 *) "&field1=" );
	MUSART_voidSendString( (u8 *) Copy_u8Data );
	MUSART_voidSendString( (u8 *) "\r\n" );
	MSTK_voidDelayMs( 20000);

}



u8   ESP8266_u8ReceiveHttpReq( u8 * Copy_u8Server , u8 * Copy_u8Length ){
u8 Local_u8Receive=0;
	ESP8266_voidClearBuffer();

	MUSART_voidSendString( (u8 *) "AT+CIPSEND=" );
	MUSART_voidSendString( (u8 *) Copy_u8Length );
	MUSART_voidSendString( (u8 *) "\r\n" );
	MSTK_voidDelayMs( 4000);

	ESP8266_voidClearBuffer();

	MUSART_voidSendString( (u8 *) "GET http://" );
	MUSART_voidSendString( (u8 *) Copy_u8Server );
	/*name of file needs to be changed*/
	MUSART_voidSendString( (u8 *) "/system.php\r\n" );
	MSTK_voidDelayMs( 5000);//20000

	Local_u8Receive=Global_u8Response[37];
	ESP8266_voidConnectToServerTcp( (u8 *)Copy_u8Server , (u8 *)"80" );

	return  Local_u8Receive ;

}

void ESP8266_voidClearBuffer ( void ){

	u8 LOC_u8Iterator1 = 0 ;
	Iterator     = 0 ;

	for( LOC_u8Iterator1 = 0 ; LOC_u8Iterator1 < 200 ; LOC_u8Iterator1++ ){

		Global_u8Response[ LOC_u8Iterator1 ] = 0 ;

	}

}

u8 voidEspValidateCmd(void)
{
	u8 i = 0;
	u8 Dummy = 0;
	u8 Result = 0;
	
	while (Dummy != 255)
	{
		Dummy = MUSART_u8ReadData();
		Global_u8Response[i] = Dummy;
		i++;
	}
	
	/* Check if Ok */
	if (Global_u8Response[0] == 'O' && Global_u8Response[1] == 'K')
	{
		Result = 1;
	}
	
	return Result;
}

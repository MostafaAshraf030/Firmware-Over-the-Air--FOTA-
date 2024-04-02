/****************************************************************/
/* Author    : Ahmed Tarek                                      */
/* Date      : 28 SEP 2020                                      */
/* Version   : V01                                              */
/****************************************************************/
#ifndef ESP_INTERFACE_H
#define ESP_INTERFACE_H

void ESP8266_voidInit(void);
void ESP8266_voidConnectToWiFi ( u8 * SSID , u8 * Password );

void ESP8266_voidConnectToServerTcp ( u8 * Copy_u8Domain , u8 * Copy_u8Port );

void ESP8266_VidSendHttpReq( u8 * Copy_u8Key , u8 * Copy_u8Data , u8 * Copy_u8Length );

u8   ESP8266_u8ReceiveHttpReq( u8 * Copy_u8Server , u8 * Copy_u8Length );

u8 voidEspValidateCmd(void);

void ESP8266_voidClearBuffer ( void );

#endif

#include "../SERVICES/BIT_math.h"
#include "../SERVICES/STD_TYPES.h"
#include "../MCAL/MRCC/MRCC_Inteface.h"
#include "../MCAL/DIO/MGPIO_Interface.h"
#include "../MCAL/MUSART/MUSART_Interface.h"
#include "../MCAL/STK/MSTK_Interface.h"

#include "../MCAL/BootLoader/MFDI_Interface.h"
#include "../MCAL/BootLoader/APARSER_Interface.h"



volatile u8 TimeOutFlag = 0 ;
volatile u8 ReceiveBuffer[100] ;
volatile u8 u8RecCounter = 0 ;
volatile u8 u8WriteReq = 1  ;

typedef void (*APP_Call)(void) ;

APP_Call Add_To_Call = 0 ; /*void (*Add_To_Call)(void)*/

void App_voidTest(void)
{

	TimeOutFlag = 1 ;

    /*Move Vector Table*/
#define SCB_VTOR       *((volatile u32*)0xE000ED08)

	SCB_VTOR = 0x08004000 ; //Starting of Sector 1 [Application]

    /*Set Address to call with Reset Handeler_ISR [startup code of Application]*/
	Add_To_Call = *(APP_Call*) 0x08004004 ;

	Add_To_Call() ; //jump Reset handler [startup code]
}

void main(void)
{

	u8 LOC_u8RecStatus = 0 ;
	/*Step 1 : System Clock is 16 MHz From HSI*/
	MRCC_voidInitSystemClk();

	/*Step 2 : Enable GPIO Peripherial Clock For Port A*/
	MRCC_voidEnablePeripheralClock(AHB,MRCC_GPIOA);

	/*Step 3 : Enable USART1 Peripherial Clock */
	MRCC_voidEnablePeripheralClock(APB2,4);

	/*Step 4 : Enable FDI Peripherial Clock */
	MRCC_voidEnablePeripheralClock(AHB,15);

	/*Step 5 : Set Pin Direction ->> Output*/
	MGPIO_u8SetPinMode(MGPIOA_PORT,MGPIO_PIN0,MGPIO_MODE_OUTPUT) ;
	/*Set Pin Speed*/
	MGPIO_u8SetOutputSpeed(MGPIOA_PORT,MGPIO_PIN0,MGPIO_SPEED_MEDUIM) ;
	/*Pull Output Type*/
	MGPIO_u8SetOutputType(MGPIOA_PORT,MGPIO_PIN0,MGPIO_OUTPUT_PUSH_PULL) ;

	/*Step 6 : indicate to i am in Boot loader */
	MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN0,MGPIO_HIGH_VALUE) ;


	/*Step 5 : Set Pin Mode For Pins 9 , 10 -> Alternative Function */
	MGPIO_u8SetPinMode(MGPIOA_PORT,MGPIO_PIN9,MGPIO_MODE_ALTF) ;    //TX-> USART1
	MGPIO_u8SetPinMode(MGPIOA_PORT,MGPIO_PIN10,MGPIO_MODE_ALTF) ;   //RX-> USART1

	/*Step 6 : Set Pin Alternative function For Pins 9 , 10  */
	MGPIO_voidSetPinAltFun(MGPIOA_PORT,MGPIO_PIN9,7) ;                //TX-> USART1
	MGPIO_voidSetPinAltFun(MGPIOA_PORT,MGPIO_PIN10,7) ;               //RX-> USART1
	/*Step 8 : MUSART Initialization*/
	MUSART_voidInit() ; /*9600bps , 1 stop bits  , no parity , 8  bit data*/

	/*Step 9 : Enable MUSART */
	MUSART_voidEnable() ;
	/*Step 10: init Systick*/
	MSTK_voidInit() ; /*disable STK , Interrupt , Tick = 0.5 Micro second --> HSI/8 --> 2MHz*/

	MSTK_voidSetIntervalSingle(15000000,App_voidTest) ;

	/* Loop forever */
	while(TimeOutFlag == 0)
	{
		LOC_u8RecStatus = MUSART_u8ReadDataSynch(&ReceiveBuffer[u8RecCounter]) ;
		if(LOC_u8RecStatus == 1)
		{
			/*Stop Timer Because Application Code In Processing*/
			MSTK_voidInit()   ; //Stop Timer
			/*indicate The end Of the Record*/
			if(ReceiveBuffer[u8RecCounter] == '\n')
			{
				if(u8WriteReq == 1)
				{
                /*Erase The Application Code*/
				MFDI_voidEraseAppArea() ;
				u8WriteReq = 0 ;
				}
				/*Parse The Record and Write it to flash*/
				APARSER_voidParseRecord(ReceiveBuffer) ;
				/*Send ok to tool to send the next Record */
				MUSART_voidSendData((u8*)"ok",2) ;
				/*Reset Buffer Counter*/
				u8RecCounter = 0 ;
			}
			else{

				u8RecCounter++ ;

			}

	        /*End Of File*/
			if(ReceiveBuffer[8] == '1')
			{
				/*Wait 1 sec then jump to Application*/
				MSTK_voidSetIntervalSingle(1000000,App_voidTest) ;
				/*Indicate to Successful Flashing*/
				MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN0,MGPIO_LOW_VALUE) ;
			}


		}


	}

}


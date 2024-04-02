/*
 * IR_program.c
 *
 *  Created on: Jun 7, 2023
 *      Author: abdullah
 */
#include"../../SERVICES/BIT_math.h"
#include"../../SERVICES/STD_TYPES.h"
#include"../../MCAL/DIO/MGPIO_Interface.h"
#include"../../MCAL/MRCC/MRCC_Inteface.h"
#include"../../MCAL/NVIC/MNVIC_Interface.h"
#include"../../MCAL/STK/MSTK_Interface.h"
#include"../../MCAL/EXTI/MEXTI_Interface.h"

#include"IR_Interface.h"

u8 flag=0;
u32 timingARR[50]={0};
u8 counter=0;
u8 GLO_u8Data ;
void IR_voidInit(void)
{
	//rcc
	MRCC_voidInitSystemClk();
	MRCC_voidEnablePeripheralClock(AHB1,MRCC_GPIOA);
	MRCC_voidEnablePeripheralClock(APB2,14);
	//exti for remote
	MGPIO_u8SetPinMode(MGPIOA_PORT,MGPIO_PIN0,MGPIO_MODE_INPUT);
	MGPIO_u8SetPullState(MGPIOA_PORT,MGPIO_PIN0,MGPIO_PULL_UP);

	MEXTI0_VoidCallBack(getFrameTiming);
	MEXTI_voidSetTriggerSource(MEXTI_LINE_0,MEXTI_FALLING_EDGE);
	MEXTI_voidSetExtiConfig(MEXTI_LINE_0,MEXTI_PORTA);

	MEXTI_voidEnableEXTI(MEXTI_LINE_0);

	MNVIC_voidEnableInterrupt(6);


for (u8 i=1;i<7;i++)
{ //3LEDS
	MGPIO_u8SetPinMode(MGPIOA_PORT,i,MGPIO_MODE_OUTPUT);
	MGPIO_u8SetOutputType(MGPIOA_PORT,i,MGPIO_OUTPUT_PUSH_PULL);
	MGPIO_u8SetOutputSpeed(MGPIOA_PORT,i,MGPIO_SPEED_LOW);

}

//MSTK_voidInit();


}

void getFrameTiming(void)
{
	if(flag == 0 )//frame is at start bits
{
	  MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN5,MGPIO_HIGH_VALUE);  //led oN
	  /*Start Timer*/
	 MSTK_voidSetIntervalSingle(25000,GetData) ;   //250 msec
	 flag = 1 ;
}
else
{
	timingARR[counter] = ( (MSTK_u32GetTime())); //??
	  MSTK_voidSetIntervalSingle(25000,GetData) ;  // reload 250 mSec
	  counter++;
	  MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN5,MGPIO_LOW_VALUE);  //led off
}


	}



void GetData(void)
{

	u8 i ;

		 /*Check if Start bit Sended */
		 if((timingARR[0]>=10000) && (timingARR[0] <= 14000))
		 {
			 for(i = 0 ; i < 8 ; i++ )
			  {
			 	 if((timingARR[17+i]>=2000) && (timingARR[17+i] <= 2500))
			 	 {
			 		/*Logic One*/
			 		 SET_BIT(GLO_u8Data,i) ;
			 	 }
			 	 else
			 	 {
			 		 /*Logic zero*/
			 		 CLR_BIT(GLO_u8Data,i) ;
			 	 }
			  }
			 TakeAction() ;
		 }
		 else
		 {
			 /*Invalid Frame*/
		 }
		 flag = 0 ;
		 timingARR[0] = 0 ;
		 counter = 0  ;



}
void TakeAction(void)
{
	u8 x=0;
	switch(GLO_u8Data)
	  {
	  case 12: MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN1,MGPIO_HIGH_VALUE);
	  break ;
	  case 24: MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN2,MGPIO_HIGH_VALUE);
	   break ;
	  case 94: MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN3,MGPIO_HIGH_VALUE);
	   break ;
	  case 8: MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN1,MGPIO_LOW_VALUE);
	   break ;
	  case 28: MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN2,MGPIO_LOW_VALUE);
	   break ;
	  case 90: MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN3,MGPIO_LOW_VALUE);
	   break ;
	  default :TOG_BIT(x,0);
	  MGPIO_u8SetPinValue(MGPIOA_PORT,MGPIO_PIN6,x) ;  break ;
	  }



}


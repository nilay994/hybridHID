/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_usb_hid_device.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_adc.h"

/*Connections
PD6=Mouse/Keyboard mode switch
PD7= left/enter
PD8= right/backspace
PD10= keyboard special
PA2= x axis 
PA1= y axis
*/

int main()
{	
	uint8_t state1= 0;
	uint8_t state2= 0;
	uint8_t state3= 0;
	uint8_t state4= 0;
	uint8_t state5= 0;
	uint8_t state6= 0;
	uint8_t counter= 0;
	uint8_t counter1=0;
	TM_USB_HIDDEVICE_Keyboard_t Keyboard;
	TM_USB_HIDDEVICE_Mouse_t Mouse;

	GPIO_InitTypeDef GPIO_InitDef;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitDef);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(GPIOD, &GPIO_InitDef);

	
	SystemInit();
	
	TM_ADC_Init(ADC1, ADC_Channel_1);   //for accx
	TM_ADC_Init(ADC1, ADC_Channel_2);   //for accy
	
	TM_DISCO_LedInit();
	
	TM_DELAY_Init();

	TM_USB_HIDDEVICE_Init();

	TM_USB_HIDDEVICE_MouseStructInit(&Mouse);
	TM_USB_HIDDEVICE_KeyboardStructInit(&Keyboard);	
	GPIO_ResetBits(GPIOD, GPIO_Pin_11);

	while (1)
	{		  
		if (TM_USB_HIDDEVICE_GetStatus() == TM_USB_HIDDEVICE_Status_Connected) 
		{

			TM_DISCO_LedOn(LED_GREEN);

			
//////////////////////////Mouse Accelero////////////////////////////////////////////
			if(TM_ADC_Read(ADC1, ADC_Channel_2)<=1700&& GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))
			{
				Mouse.XAxis=-15;
				TM_DISCO_LedOn(LED_BLUE);
				TM_USB_HIDDEVICE_MouseSend(&Mouse);
			}
			else if(TM_ADC_Read(ADC1, ADC_Channel_2)>2300&& GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))
			{
				Mouse.XAxis=15;
				TM_DISCO_LedOn(LED_BLUE);
				TM_USB_HIDDEVICE_MouseSend(&Mouse);
			}
			if(TM_ADC_Read(ADC1, ADC_Channel_1)<=1700&& GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))
			{
				Mouse.YAxis=-15;
				TM_DISCO_LedOn(LED_BLUE);
				TM_USB_HIDDEVICE_MouseSend(&Mouse);
			}
			else if(TM_ADC_Read(ADC1, ADC_Channel_1)>2300&& GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))
			{
				Mouse.YAxis=15;
				TM_DISCO_LedOn(LED_BLUE);
				TM_USB_HIDDEVICE_MouseSend(&Mouse);
			}
			Mouse.XAxis=0;

			Mouse.YAxis=0;
			TM_DISCO_LedOff(LED_BLUE);
			TM_USB_HIDDEVICE_MouseSend(&Mouse);	
////////////////////////////////////////////////////////////////////////

///////////////////////Keyboard Accelero///////////////////////////////////////////////
			if(TM_ADC_Read(ADC1, ADC_Channel_2)<=1700&& (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)))
			{
				Keyboard.Key1=0x50;
				TM_DISCO_LedOn(LED_ORANGE);	
				TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
			}

			else if(TM_ADC_Read(ADC1, ADC_Channel_2)>2300&& (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)))
			{
				Keyboard.Key1=0x4F;
				TM_DISCO_LedOn(LED_ORANGE);	
				TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
			}

			if(TM_ADC_Read(ADC1, ADC_Channel_1)<=1700&& (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)))
			{
				Keyboard.Key1=0x51;
				TM_DISCO_LedOn(LED_ORANGE);	
				TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
			}

			else if(TM_ADC_Read(ADC1, ADC_Channel_1)>2300&& (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)))
			{
				Keyboard.Key1=0x52;
				TM_DISCO_LedOn(LED_ORANGE);	
				TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
			}

			Keyboard.Key1=0x00;
			TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
			TM_DISCO_LedOff(LED_ORANGE);	

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

			if ((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7)) && state1 == 0 && GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))
			{ 
				state1 = 1;
				GPIO_SetBits(GPIOD, GPIO_Pin_11);
				TM_DISCO_LedOn(LED_BLUE);	
				Mouse.LeftButton = 1;
				TM_USB_HIDDEVICE_MouseSend(&Mouse);
			}

			else if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) && state1 == 1 && GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)) 
			{ 
				state1 = 0;
				Mouse.LeftButton = 0;
				GPIO_ResetBits(GPIOD, GPIO_Pin_11);
				TM_USB_HIDDEVICE_MouseSend(&Mouse);
				TM_DISCO_LedOff(LED_BLUE);	
			}
			if ((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7)) && state2== 0 && (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)))
			{ 
				state2 = 1;
				GPIO_SetBits(GPIOD, GPIO_Pin_11);
				TM_DISCO_LedOn(LED_ORANGE);	
  Keyboard.Key1 = 0x28;   //enter
  TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
} 
else if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) && state2 == 1 && (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))) 
{ 
	state2 = 0;
	Keyboard.Key1=0x00;
	GPIO_ResetBits(GPIOD, GPIO_Pin_11);
	TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
	TM_DISCO_LedOff(LED_ORANGE);	
}
if ((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)) && state3 == 0 && GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))
{ 
	state3 = 1;
	GPIO_SetBits(GPIOD, GPIO_Pin_11);
	TM_DISCO_LedOn(LED_BLUE);	
	Mouse.RightButton = 1;
	TM_USB_HIDDEVICE_MouseSend(&Mouse);
} 
else if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8) && state3 == 1 && GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)) 
{ 
	state3 = 0;
	Mouse.RightButton = 0;
	TM_USB_HIDDEVICE_MouseSend(&Mouse);
	GPIO_ResetBits(GPIOD, GPIO_Pin_11);
	TM_DISCO_LedOff(LED_BLUE);	
}
if ((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)) && state4== 0 && (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)))
{ 
	state4 = 1;
	GPIO_SetBits(GPIOD, GPIO_Pin_11);
	TM_DISCO_LedOn(LED_ORANGE);	
  Keyboard.Key1 = 0x2A;   //backspace
  TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
} 
else if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8) && state4 == 1 && (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))) 
{ 
	state4 = 0;
	Keyboard.Key1=0x00;
	TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
	TM_DISCO_LedOff(LED_ORANGE);	
	GPIO_ResetBits(GPIOD, GPIO_Pin_11);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

if((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9)) && state6 == 0)
{
	while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9))
	{
	 //start the motor from PinD11
		GPIO_SetBits(GPIOD, GPIO_Pin_11);
		counter1++;
		Delayms(500);
	 //stop the motor
		GPIO_ResetBits(GPIOD, GPIO_Pin_11);	
		Delayms(500);
	}
	state6=1;
}
else if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9) && state6 == 1 )	
{
	if(counter1==1)
	{
				Keyboard.L_GUI = 1;	/* Win button */
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
		Delayms(100);
		Keyboard.L_GUI=0;
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
		
		Keyboard.Key1 = 0x11;    //n
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
		
		Keyboard.Key1 = 0x04;    //a
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
		
		Keyboard.Key1 = 0x28;   //enter
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);		
	}
	
	if(counter1==2)
	{
		Keyboard.L_CTRL=1;
		Keyboard.L_SHIFT=1;
		Keyboard.Key1=0x19;     //pdf reader mode
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
	}
	if(counter1==3)
	{
				Keyboard.L_GUI = 1;	/* Win button */
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
		Delayms(100);
		Keyboard.L_GUI=0;
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
		
		Keyboard.Key1 = 0x16;    //s
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
		
		Keyboard.Key1 = 0x13;    //p
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
		
		Keyboard.Key1 = 0x08;    //e
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
		
		Keyboard.Key1 = 0x28;   //enter
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);	
	}		
	if(counter1==4)
	{
		Keyboard.L_CTRL=1;
		Keyboard.L_ALT=1;		
		Keyboard.Key1 = 0x0A;  // G
		TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
	}
	
	
	state6=0;
	counter1=0;
	Keyboard.L_GUI=0;
	Keyboard.L_CTRL=0;
	Keyboard.L_SHIFT=0;
	Keyboard.L_ALT=0;
	Keyboard.Key1=0x00;
	TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
	TM_DISCO_LedOff(LED_ORANGE);
}

/////////////////////////////////////////////////////////////////////////////

if((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)) && state5 == 0)
{
	while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10))
	{
	 //start the motor from PinD11
		GPIO_SetBits(GPIOD, GPIO_Pin_11);
		counter++;
		Delayms(500);
	 //stop the motor
		GPIO_ResetBits(GPIOD, GPIO_Pin_11);	
		Delayms(500);
	}
	state5=1;
}
else if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10) && state5 == 1 )	
{
	if(counter==1)
	{
		//function1-alt tab 
		Keyboard.L_ALT=1;
		Keyboard.Key1=0x2B;
	}
	if(counter==2)
	{
		//function2-alt f4
		Keyboard.L_ALT=1;
		Keyboard.Key1=0x3D;     //f4
	}
	
	if(counter==3)
	{
		//function3 - desktop
		Keyboard.L_GUI=1;
		Keyboard.Key1=0x07;     //d
	}
	
	if(counter==4)
	{
		//function4- my computer
		Keyboard.L_GUI=1;
		Keyboard.Key1=0x08;     //d
	}
	
	TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
	state5=0;
	counter=0;
	Keyboard.L_GUI=0;
	Keyboard.L_CTRL=0;
	Keyboard.L_ALT=0;
	Keyboard.Key1=0x00;
	TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
	TM_DISCO_LedOff(LED_ORANGE);
}
///////////////////////////////////////////////////////////////////////

}
else 
{
	TM_DISCO_LedOff(LED_GREEN);
}

}
}

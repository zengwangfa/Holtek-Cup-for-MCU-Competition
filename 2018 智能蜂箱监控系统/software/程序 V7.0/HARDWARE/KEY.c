#include "KEY.h"

extern int pwm1_value;

u8 send_buffer1[]={0xAA,0X55,0X0C,0X00,0X01,0x04,0x00,0x9c,0XFF,0XFF,0X02,0XD5,0X1B,0X1B,0XB7,0X00};
u8 zt_CMD[]={0x7A,0x74,0x2E,0x76,0x61,0x6C,0x3D,0x30,0xff,0xff,0xff};

void KEY_Init(void) //IO初始化   A8  A9
{ 
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_12, AFIO_MODE_DEFAULT);		 //DAT(A6)
	GPIO_DirectionConfig(HT_GPIOB, AFIO_PIN_12, GPIO_DIR_IN);	 //输入方向
	GPIO_PullResistorConfig(HT_GPIOB, AFIO_PIN_12, GPIO_PR_UP); //上拉输入
	
	AFIO_GPxConfig(GPIO_PD, AFIO_PIN_6, AFIO_MODE_DEFAULT);		 //DAT(A6)
	GPIO_DirectionConfig(HT_GPIOD, AFIO_PIN_6, GPIO_DIR_IN);	 //输入方向
	GPIO_PullResistorConfig(HT_GPIOD, AFIO_PIN_6, GPIO_PR_UP); //上拉输入
	
  GPIO_InputConfig(HT_GPIOB, AFIO_PIN_12, ENABLE);  		       //使能输入
	GPIO_InputConfig(HT_GPIOD, AFIO_PIN_6, ENABLE);  		       //使能输入
}


/*
       pwm1_value     	 19			18			16			14			12			10			8    (*3000)
				
   [实测]占空比（%）		61.2   58.6    53.7    48.3     38     32.6    6.6
*/
void KYE1_SHOW(void)
{
	if(KEY1 == 0)
	{
		delay_ms(30);
		if(KEY1 == 0)
		{
			pwm1_value--;
			delay_ms(50);			
			//USART_SendData(HT_USART0,pwm1_value); 
			if(pwm1_value<1)pwm1_value=20;
		}
	//	while(!KEY1);
	}
}

void KYE2_SHOW(void)//设置蜂王状态
{
	if(KEY2 == 0)
	{
		delay_ms(30);
		if(KEY2 == 0)
		{	
			zt_CMD[7]=zt_CMD[7]+0x01;
			if(zt_CMD[7]>0x34)zt_CMD[7]=0x31;		
			USART_Sends(HT_USART1,zt_CMD,sizeof(zt_CMD)); 

		}
	//	while(!KEY2);
	}

	
}
	


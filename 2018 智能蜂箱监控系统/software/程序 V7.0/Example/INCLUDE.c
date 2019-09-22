#include "INCLUDE.h"


void All_Init()
{
	
	System_Init();      	//系统时钟初始化
  USART0_Init(115200);  //用于WiFi与上位机通信 TX.(PA2)    RX.(PA3)115200
  USART1_Init(9600);  	//用于串口屏通讯         TX.(PA4)    RX.(PA5)
	Time0_Init();
 // Time1_Init();
	Adc_Init();
	delay_init();       	//延时函数初始
	HX711_Init1();       	//HX711模块初始化 DAT.(PC0)  SCK.(PC1)
	HX711_Init2();       	//HX711模块初始化 DAT.(PC2)  SCK.(PC3)
	CTR_IO_Init();      	//外部控制电磁阀和加热片引脚初始化(电磁阀包括 喂食与清洗)
	Motor_Init();         //步进电机初始化
	LED_Init();        		//LED1 (C9)  LED2 (C11)
	KEY_Init();         	//KEY1 (B0)  KEY2 (B1)
 	delay_ms(100);

	PWM1_Init();
	PWM2_Init();

	while(DS18B20_Init())	//18B20串行总线      Serial Bus(PA0)
	{
		
	}
		
}

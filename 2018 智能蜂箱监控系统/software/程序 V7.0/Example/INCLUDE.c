#include "INCLUDE.h"


void All_Init()
{
	
	System_Init();      	//ϵͳʱ�ӳ�ʼ��
  USART0_Init(115200);  //����WiFi����λ��ͨ�� TX.(PA2)    RX.(PA3)115200
  USART1_Init(9600);  	//���ڴ�����ͨѶ         TX.(PA4)    RX.(PA5)
	Time0_Init();
 // Time1_Init();
	Adc_Init();
	delay_init();       	//��ʱ������ʼ
	HX711_Init1();       	//HX711ģ���ʼ�� DAT.(PC0)  SCK.(PC1)
	HX711_Init2();       	//HX711ģ���ʼ�� DAT.(PC2)  SCK.(PC3)
	CTR_IO_Init();      	//�ⲿ���Ƶ�ŷ��ͼ���Ƭ���ų�ʼ��(��ŷ����� ιʳ����ϴ)
	Motor_Init();         //���������ʼ��
	LED_Init();        		//LED1 (C9)  LED2 (C11)
	KEY_Init();         	//KEY1 (B0)  KEY2 (B1)
 	delay_ms(100);

	PWM1_Init();
	PWM2_Init();

	while(DS18B20_Init())	//18B20��������      Serial Bus(PA0)
	{
		
	}
		
}

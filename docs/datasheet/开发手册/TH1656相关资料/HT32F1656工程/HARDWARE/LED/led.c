#include "led.h"
#include "delay.h"

void LED_Init(void)
{
	/* 使能时钟       	 						*/
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PA         = 1;			//GPIOA时钟打开
	CKCUClock.Bit.PD         = 1;			//GPIOD时钟打开
  CKCUClock.Bit.AFIO       = 1;			//复用时钟
  CKCU_PeripClockConfig(CKCUClock, ENABLE);		//时钟使能

  /* 配置LED1,LED2,LED3管脚功能 	*/

  /* 配置管脚复用功能						*/
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_7, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_8, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_10, AFIO_MODE_DEFAULT);

  /* 配置GPIO为输出							*/
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_7, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_8, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10, GPIO_DIR_OUT);
	
}


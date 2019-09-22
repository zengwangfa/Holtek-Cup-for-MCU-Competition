#include "led.h"
#include "delay.h"

void LED_Init(void)
{
	/* ʹ��ʱ��       	 						*/
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PA         = 1;			//GPIOAʱ�Ӵ�
	CKCUClock.Bit.PD         = 1;			//GPIODʱ�Ӵ�
  CKCUClock.Bit.AFIO       = 1;			//����ʱ��
  CKCU_PeripClockConfig(CKCUClock, ENABLE);		//ʱ��ʹ��

  /* ����LED1,LED2,LED3�ܽŹ��� 	*/

  /* ���ùܽŸ��ù���						*/
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_7, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_8, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_10, AFIO_MODE_DEFAULT);

  /* ����GPIOΪ���							*/
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_7, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_8, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10, GPIO_DIR_OUT);
	
}


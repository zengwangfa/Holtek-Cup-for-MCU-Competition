#ifndef __KEY_H
#define __KEY_H	 

#include "INCLUDE.h"

#define KEY1  GPIO_ReadInBit(HT_GPIOD,GPIO_PIN_6)//��ȡ����1
#define KEY2  GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12)//��ȡ����0


#define KEY1_PRES	  1 //KEY1����
#define KEY2_PRES	  2	//KEY2����


void KEY_Init(void); //IO��ʼ��    B0  B1
void KYE1_SHOW(void);
void KYE2_SHOW(void);


#endif

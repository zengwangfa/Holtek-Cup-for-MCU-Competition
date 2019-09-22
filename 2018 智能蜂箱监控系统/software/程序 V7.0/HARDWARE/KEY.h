#ifndef __KEY_H
#define __KEY_H	 

#include "INCLUDE.h"

#define KEY1  GPIO_ReadInBit(HT_GPIOD,GPIO_PIN_6)//读取按键1
#define KEY2  GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12)//读取按键0


#define KEY1_PRES	  1 //KEY1按下
#define KEY2_PRES	  2	//KEY2按下


void KEY_Init(void); //IO初始化    B0  B1
void KYE1_SHOW(void);
void KYE2_SHOW(void);


#endif

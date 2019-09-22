#ifndef _HX711_H_
#define _HX711_H_

#include "INCLUDE.h"

#define  HX711_AFIO_PORT         GPIO_PC      //PORT
#define  HX711_GPIO_PORT         HT_GPIOC    

#define  HX711_DOUT1_AFIO_PIN          AFIO_PIN_0  //��DAT1
#define  HX711_DOUT1_GPIO_PIN          GPIO_PIN_0  

#define  HX711_SCK1_AFIO_PIN           AFIO_PIN_1  //�� SCK1
#define  HX711_SCK1_GPIO_PIN           GPIO_PIN_1

#define  HX711_DOUT2_AFIO_PIN          AFIO_PIN_2  //�� DAT2
#define  HX711_DOUT2_GPIO_PIN          GPIO_PIN_2  

#define  HX711_SCK2_AFIO_PIN           AFIO_PIN_3  //�� SCK2
#define  HX711_SCK2_GPIO_PIN           GPIO_PIN_3

#define ADDO1 PCin(0)  
#define ADSK1 PCout(1)

#define ADDO2 PCin(2)  
#define ADSK2 PCout(3)

void HX711_Init1(void);     //HX711ģ��1���ų�ʼ��
void HX711_Init2(void);     //HX711ģ��2���ų�ʼ��

unsigned long HX711_Read1(void);//��ȡHX711ģ��ɼ�����1
unsigned long HX711_Read2(void);//��ȡHX711ģ��ɼ�����2

u8 *Replace_Weight_Array(u8 array[],u8 num1,u8 num2);//��������ֵ

void UART_SEND_Weight(void);

#endif

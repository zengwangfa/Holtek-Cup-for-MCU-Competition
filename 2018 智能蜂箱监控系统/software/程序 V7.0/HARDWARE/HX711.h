#ifndef _HX711_H_
#define _HX711_H_

#include "INCLUDE.h"

#define  HX711_AFIO_PORT         GPIO_PC      //PORT
#define  HX711_GPIO_PORT         HT_GPIOC    

#define  HX711_DOUT1_AFIO_PIN          AFIO_PIN_0  //①DAT1
#define  HX711_DOUT1_GPIO_PIN          GPIO_PIN_0  

#define  HX711_SCK1_AFIO_PIN           AFIO_PIN_1  //① SCK1
#define  HX711_SCK1_GPIO_PIN           GPIO_PIN_1

#define  HX711_DOUT2_AFIO_PIN          AFIO_PIN_2  //② DAT2
#define  HX711_DOUT2_GPIO_PIN          GPIO_PIN_2  

#define  HX711_SCK2_AFIO_PIN           AFIO_PIN_3  //② SCK2
#define  HX711_SCK2_GPIO_PIN           GPIO_PIN_3

#define ADDO1 PCin(0)  
#define ADSK1 PCout(1)

#define ADDO2 PCin(2)  
#define ADSK2 PCout(3)

void HX711_Init1(void);     //HX711模块1引脚初始化
void HX711_Init2(void);     //HX711模块2引脚初始化

unsigned long HX711_Read1(void);//读取HX711模块采集重量1
unsigned long HX711_Read2(void);//读取HX711模块采集重量2

u8 *Replace_Weight_Array(u8 array[],u8 num1,u8 num2);//更新重量值

void UART_SEND_Weight(void);

#endif

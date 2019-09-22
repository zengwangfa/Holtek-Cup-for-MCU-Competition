/* 重要声明：Kaven技术有限，如果用了本代码而出现问题的，概不负责。所以发布前请先进行测试。 */

#ifndef __ZH_KEY_H__
#define __ZH_KEY_H__

#include "ht32.h"


#define KEY_R1_AFIO_PORT                  GPIO_PA
#define KEY_R1_AFIO_PIN                   AFIO_PIN_0
#define KEY_R1_PORT                       HT_GPIOA
#define KEY_R1_PIN                        GPIO_PIN_0

#define KEY_R2_AFIO_PORT                  GPIO_PA
#define KEY_R2_AFIO_PIN                   AFIO_PIN_1
#define KEY_R2_PORT                       HT_GPIOA
#define KEY_R2_PIN                        GPIO_PIN_1

#define KEY_R3_AFIO_PORT                  GPIO_PA
#define KEY_R3_AFIO_PIN                   AFIO_PIN_2
#define KEY_R3_PORT                       HT_GPIOA
#define KEY_R3_PIN                        GPIO_PIN_2

#define KEY_R4_AFIO_PORT                  GPIO_PA
#define KEY_R4_AFIO_PIN                   AFIO_PIN_3
#define KEY_R4_PORT                       HT_GPIOA
#define KEY_R4_PIN                        GPIO_PIN_3

#define KEY_C1_AFIO_PORT                  GPIO_PA
#define KEY_C1_AFIO_PIN                   AFIO_PIN_4
#define KEY_C1_PORT                       HT_GPIOA
#define KEY_C1_PIN                        GPIO_PIN_4

#define KEY_C2_AFIO_PORT                  GPIO_PA
#define KEY_C2_AFIO_PIN                   AFIO_PIN_5
#define KEY_C2_PORT                       HT_GPIOA
#define KEY_C2_PIN                        GPIO_PIN_5

#define KEY_C3_AFIO_PORT                  GPIO_PA
#define KEY_C3_AFIO_PIN                   AFIO_PIN_6
#define KEY_C3_PORT                       HT_GPIOA
#define KEY_C3_PIN                        GPIO_PIN_6

//#define KEY_C4_AFIO_PORT                  GPIO_PB
//#define KEY_C4_AFIO_PIN                   AFIO_PIN_10
//#define KEY_C4_PORT                       HT_GPIOB
//#define KEY_C4_PIN                        GPIO_PIN_10


#define ZH_KEY_AFIO_MODE                  AFIO_MODE_1

typedef enum 
{
  RELEASE = 0,    //释放
  PRESS ,         //第一次按下
  KEEP            //保持
} ZH_KeyStatus;

typedef enum
{
  ZH_KEYR1 = 1,
  ZH_KEYR2,
  ZH_KEYR3,
  ZH_KEYR4,
  ZH_KEYRMAX
}ZH_KEY_R_Enum ;

typedef enum
{
  ZH_KEYC1 = 1,
  ZH_KEYC2,
  ZH_KEYC3,
//  ZH_KEYC4,
  ZH_KEYCMAX
}ZH_KEY_C_Enum ;

/*****************************************
矩阵键盘中每个按键都有自己的名称,
下列就是从第一个按键开始对所有按键命名.
******************************************/
typedef enum
{
  ZH_KEY1 = 0,
  ZH_KEY2 ,
  ZH_KEY3 ,
  ZH_KEY4 ,
  
  ZH_KEY5 ,
  ZH_KEY6 ,
  ZH_KEY7 ,
  ZH_KEYZH ,
  
  ZH_KEY_JIA ,
  ZH_KEY_JIAN ,
  ZH_KEY_SHENG ,
  ZH_KEY_JIANG ,
  
  ZH_KEYNMAX
  
}ZH_KEY_Enum ;


extern ZH_KeyStatus volatile ZH_Key_s[ZH_KEYNMAX];   //所有按键的状态




void ZH_Key_Init(void);
void ZH_Key_WriteOutBit(ZH_KEY_C_Enum KEY, FlagStatus Status);
void ZH_Key_WriteOutC(ZH_KEY_C_Enum KEY, FlagStatus Status);
void ZH_Key_JianCe(void);








#endif



#ifndef __MOTOR_H
#define __MOTOR_H

#include "INCLUDE.h"

#define STEP_AFIO_PORT   GPIO_PB      //步进电机的控制端口为B端口
#define STEP_GPIO_PORT 	 HT_GPIOB

#define STEP_A_AFIO   	  AFIO_PIN_8   //对应步进电机的A相
#define STEP_B_AFIO 			AFIO_PIN_9   //对应步进电机的B相
#define STEP_C_AFIO 			AFIO_PIN_10  //对应步进电机的C相
#define STEP_D_AFIO 			AFIO_PIN_11  //对应步进电机的D相

#define STEP_A_GPIO   	  GPIO_PIN_8   //对应步进电机的A相
#define STEP_B_GPIO 			GPIO_PIN_9   //对应步进电机的B相
#define STEP_C_GPIO 			GPIO_PIN_10  //对应步进电机的C相
#define STEP_D_GPIO 			GPIO_PIN_11  //对应步进电机的D相


void Motor_Init(void);
void Motor_Stop(void);	
void SetMotor(unsigned char InputData); //根据传送的数据Inputdata设置引脚输出

void Motor_NCircle(int n,bool position);//n为步进电机转动的圈数，position为正转或者反转

void Motor_NAngle(int n,bool position); //n为步进电机转动的角度为n*120，position为正转或者反转







#endif

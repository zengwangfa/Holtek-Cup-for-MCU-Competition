#ifndef __MOTOR_H
#define __MOTOR_H

#include "INCLUDE.h"

#define STEP_AFIO_PORT   GPIO_PB      //��������Ŀ��ƶ˿�ΪB�˿�
#define STEP_GPIO_PORT 	 HT_GPIOB

#define STEP_A_AFIO   	  AFIO_PIN_8   //��Ӧ���������A��
#define STEP_B_AFIO 			AFIO_PIN_9   //��Ӧ���������B��
#define STEP_C_AFIO 			AFIO_PIN_10  //��Ӧ���������C��
#define STEP_D_AFIO 			AFIO_PIN_11  //��Ӧ���������D��

#define STEP_A_GPIO   	  GPIO_PIN_8   //��Ӧ���������A��
#define STEP_B_GPIO 			GPIO_PIN_9   //��Ӧ���������B��
#define STEP_C_GPIO 			GPIO_PIN_10  //��Ӧ���������C��
#define STEP_D_GPIO 			GPIO_PIN_11  //��Ӧ���������D��


void Motor_Init(void);
void Motor_Stop(void);	
void SetMotor(unsigned char InputData); //���ݴ��͵�����Inputdata�����������

void Motor_NCircle(int n,bool position);//nΪ�������ת����Ȧ����positionΪ��ת���߷�ת

void Motor_NAngle(int n,bool position); //nΪ�������ת���ĽǶ�Ϊn*120��positionΪ��ת���߷�ת







#endif

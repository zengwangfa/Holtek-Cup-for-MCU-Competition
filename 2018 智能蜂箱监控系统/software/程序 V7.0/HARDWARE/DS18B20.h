#ifndef __DSB18B20_H
#define __DSB18B20_H

#include "INCLUDE.h"

#define DS18B20_PIN     AFIO_PIN_12
//IO��������
#define DS18B20_IO_IN()  {	AFIO_GPxConfig(GPIO_PE, DS18B20_PIN, AFIO_MODE_DEFAULT);GPIO_DirectionConfig(HT_GPIOE, DS18B20_PIN, GPIO_DIR_IN);GPIO_InputConfig(HT_GPIOE, DS18B20_PIN, ENABLE); }
#define DS18B20_IO_OUT() {	AFIO_GPxConfig(GPIO_PE, DS18B20_PIN, AFIO_MODE_DEFAULT);GPIO_DirectionConfig(HT_GPIOE, DS18B20_PIN, GPIO_DIR_OUT);}
////IO��������											   
#define	DS18B20_DQ_OUT PEout(12) //���ݶ˿�	PE8
#define	DS18B20_DQ_IN  PEin(12)  //���ݶ˿�	PE8


u8 DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20    
u8 *Replace_Temp_Array(u8 array[],u8 num1,u8 num2);//�����¶�ֵ
void UART_Send_Temp(void);

#endif





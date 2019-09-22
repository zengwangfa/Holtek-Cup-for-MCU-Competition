#ifndef __DSB18B20_H
#define __DSB18B20_H

#include "INCLUDE.h"

#define DS18B20_PIN     AFIO_PIN_12
//IO方向设置
#define DS18B20_IO_IN()  {	AFIO_GPxConfig(GPIO_PE, DS18B20_PIN, AFIO_MODE_DEFAULT);GPIO_DirectionConfig(HT_GPIOE, DS18B20_PIN, GPIO_DIR_IN);GPIO_InputConfig(HT_GPIOE, DS18B20_PIN, ENABLE); }
#define DS18B20_IO_OUT() {	AFIO_GPxConfig(GPIO_PE, DS18B20_PIN, AFIO_MODE_DEFAULT);GPIO_DirectionConfig(HT_GPIOE, DS18B20_PIN, GPIO_DIR_OUT);}
////IO操作函数											   
#define	DS18B20_DQ_OUT PEout(12) //数据端口	PE8
#define	DS18B20_DQ_IN  PEin(12)  //数据端口	PE8


u8 DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20    
u8 *Replace_Temp_Array(u8 array[],u8 num1,u8 num2);//更新温度值
void UART_Send_Temp(void);

#endif





/* Includes ------------------------------------------------------------------------------------------------*/
#include "INCLUDE.h"

#define FLASH_WriteAddress     0x100000
#define FLASH_ReadAddress      FLASH_WriteAddress
#define FLASH_SectorToErase    FLASH_WriteAddress
#define BufferSize  256

//------------------定时器中断中进行的函数-------------------------------------//
extern u16 temperature,temp_positive,temp_ten,temp_unit,temp_decimal;//调用温度形参
extern u8 Temp_Ten_Cmd[],Temp_decimal_Cmd[];////调用所发送的数据
extern void UART_Send_Temp(void);//调用发送温度函数
extern void UART_Send_Weight(void);//调用发送重量函数
extern bool          HEAT_START_FLAG;                   //开始控制加热丝加热标志位
extern bool          FEED_START_FLAG;                   //开始喂食蜂箱标志位
extern bool          START_WASHING_FLAG;                //开始清洗蜂箱标志位
unsigned long HX711AD_Value_out=0;
u8 SECOND_Count=0;       
u8 DataBuffer[8];   //蜂箱数据暂存数组，包括2字节的重量+2字节的饲料重量+2字节的温度+1字节的(清洗、加热、喂食)标志位+1字节的蜂王状态
u8 TEXT_Buffer[2]={0X04};    //第一个字节为蜂箱的开口状态，0x04表示开口最大，0x02居中，0x01最小
u8 temp; 
vu32 FLASH_ID = 0;
u32 FLASH_Init_OK;
float Volet;

void DataBuffer_Deal(void);

int main(void)
{
	All_Init();     //系统以及外围设备初始化
	
//      /* Initialize the SPI_FLASH driver                                                                        */
//		FLASH_Init_OK = SPI_FLASH_Init();
//	
//		if (FLASH_Init_OK == TRUE)LED1=1;
//		else LED1=0;

//		FLASH_ID = SPI_FLASH_ReadJEDECID();        //读FLASH ID
//		SPI_FLASH_WriteStatus(0x00);               //清除保护位
//		SPI_FLASH_SectorErase(FLASH_SectorToErase);//擦除 FLASH 扇区
//	
//		SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,sizeof(TEXT_Buffer));	//写入蜂箱开口状态
//	  SPI_FLASH_BufferDualRead((u16*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer)/2);	//读取蜂箱开口状态,放入数组当中
//	
//	  if(TEXT_Buffer[1]!=0x55)	
//		{
//			TEXT_Buffer[0]=0x04;
//			TEXT_Buffer[1]=0x55;
//			SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,sizeof(TEXT_Buffer));	//写入蜂箱开口状态
//		}	
			TEXT_Buffer[0]=0x04;
			TEXT_Buffer[1]=0x55;
	while(1)
	{
		//Volet = Get_AD_Value()*8.02;//电压采集
		Uart0DataAnalyzing(); //串口数据解析
		KYE1_SHOW();  //切换查看蜂王状态
		KYE2_SHOW();
		PWM_Control(); 
		DataBuffer_Deal();
	}
}

void DataBuffer_Deal()
{
		
			
		DataBuffer[6]=HEAT_START_FLAG;                     //将加热标志位存入数组，具体看通信数据格式
		DataBuffer[6]=DataBuffer[6]<<1;                    //00000010代表正在加热
		DataBuffer[6]=DataBuffer[6]+FEED_START_FLAG;        //将喂食标志位存入数组，具体看通信数据格式
		temp=START_WASHING_FLAG;
		DataBuffer[6]=DataBuffer[6]+(temp<<2);
		
//		if(SECOND_Count == 1)
//		{
			temperature=DS18B20_Get_Temp();	
			DataBuffer[5]=(temperature+500);                     //2字节的温度数据放入数组中
			DataBuffer[4]=(temperature+500)>>8;
//		}
//		else if(SECOND_Count == 2)
//		{
			DataBuffer[0]=HX711_Read1()>>16;                  //2字节的蜂箱重量数据放入数组
			DataBuffer[1]=HX711_Read1()>>8;
			DataBuffer[2]=0xff;                               //2字节的饲料重量数据放入数组(ffff代表饲料充足)
			DataBuffer[3]=0xff;
			SECOND_Count=0;
//		}
		
}



//------------------UART.c---------------------//
#include "UART.h"

#define MAX_DATA_LENS 127
#define USART_TIMEOUT_ON          ((u32)0x00000080)

extern unsigned char temperature;             //蜂箱内的温度值
unsigned char recv_buffer[(MAX_DATA_LENS+4)]; //串口1接收数据缓冲变量,
unsigned char send_buffer[(MAX_DATA_LENS+4)]; //串口1发送数据缓冲变量,
unsigned char recv1_data_p=0x00;              //串口1接收数据指针
unsigned char recv1_data_ok=0;                //接收到正确的数据包
unsigned char STEER_START_RUN_FLAG=0;         //舵机开始运转标志位,0表示停止运转，1表示开始运转
extern     u8 TEXT_Buffer[2];                  //第一个字节为蜂箱的开口状态，0x01表示开口最大，0x02居中，0x03最小
bool          HEAT_START_FLAG=FALSE;          //开始控制加热丝加热标志位
bool          START_WASHING_FLAG=FALSE;       //开始清洗蜂箱标志位
bool          FEED_START_FLAG=FALSE;          //开始喂食蜂箱标志位
extern unsigned char DataBuffer[8];           //蜂箱数据暂存数组，包括2字节的重量+2字节的温度+1字节的蜂王状态
unsigned  int Washing_Time=0;                 //清洗时间，由上位机给定
unsigned  int Heat_Time=0;                    //加热时间，由上位机给定，单位秒
unsigned  int Feed_Time=0;                    //喂食时间，由上位机给定，单位秒
unsigned char sum_check=0;
u8 Gate_Motor_flag=0x03;    //舵机打开标志�
u8 Gate_Motor=0x00;    //舵机打开标志位�
u8 dz_CMD[]={0x64,0x7A,0x2E,0x76,0x61,0x6C,0x3D,0x31,0xff,0xff,0xff};
 //74 31 39 2E 74 78 74 3D 22 32 31 22 ff ff ff         t19.txt="21"


void USART_Sends(HT_USART_TypeDef* USARTx,u8 *Data,u16 len)
{
	u16 i=0;
  for(;i<len;i++)
  USART_SendData(USARTx,Data[i]);
}


void USART1_IRQHandler(void)  //串口1 接收中断   接收串口屏蜂门控制
{
  /* Rx, move data from USART FIFO to buffer            */
  if (((HT_USART1->IIR) & USART_IID_RDA ) == USART_IID_RDA ) //接收中断
  {
		Gate_Motor=0x01;
		Gate_Motor_flag = USART_ReceiveData(HT_USART1);
		if(Gate_Motor_flag == 0xA6 && Gate_Motor==0x01 )  //开启蜂门
		{
			LED3=0;
			dz_CMD[7]=0x36;
		  USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //串口屏上显示 .蜂门最大
			if(TEXT_Buffer[0]==0x02)                                  //蜂箱开口居中，顺时针转动120度，回到开口最大
			{		
				Motor_NAngle(1,FALSE);                                 //顺时针转动120度
				Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
			}
			else if(TEXT_Buffer[0]==0x01)                             //蜂箱开口最小，顺时针转动240度，回到开口最大
			{	
				Motor_NAngle(2,FALSE);                                //顺时针转动240度
				Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
			}
			TEXT_Buffer[0]=0x04;
			Gate_Motor=0x00;
		}
		else if(Gate_Motor_flag == 0xA5 && Gate_Motor==0x01)   //关闭
		{
			LED3=1;
			dz_CMD[7]=0x34;
		  USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //串口屏上显示 .蜂门关闭
			if(TEXT_Buffer[0]==0x04)                                  //蜂箱开口居中，顺时针转动120度，回到开口最大
			{		
				Motor_NAngle(2,TRUE);                                 //顺时针转动120度
				Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
			}
			else if(TEXT_Buffer[0]==0x02)                             //蜂箱开口最小，顺时针转动240度，回到开口最大
			{	
				Motor_NAngle(1,TRUE);                                //顺时针转动240度
				Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
			}
			TEXT_Buffer[0]=0x01;
			Gate_Motor=0x00;
   	}
		else {Gate_Motor=0x00;}
	}
}


void USART0_IRQHandler(void)  //串口0 接收中断
{
	u8 i=0,j=0;
  /* Rx, move data from USART FIFO to buffer            */
	
  if (((HT_USART0->IIR) & USART_IID_RDA ) == USART_IID_RDA ) //接收中断
  {
		
		recv_buffer[recv1_data_p] =USART_ReceiveData(HT_USART0);	 //读取串口1接收到的数据
		recv1_data_p++; 

			if(recv1_data_p<(MAX_DATA_LENS+4))    //定义数据长度未包括包头和包长3个字节,+4
			 {

				if(recv_buffer[0]==0xaa)  //数据包包头字节
					{

						if(recv1_data_p>3)
							{

								if(recv_buffer[1]==0x55)
									{	
												
										if(recv1_data_p>=(recv_buffer[2]+3))  //接收完数据包（第三个字节为数据长度，数据长度不包含开头和校验字）
											{
												
													for(i=0;i<=(recv1_data_p-2);i++)  //接收到数据包的最后一个字节为校验字；校验方法对整包数据进行累加（不包含校验字）
													{
														j +=recv_buffer[i];	
													}
													
													if(j==recv_buffer[recv1_data_p-1])     //判断校验是否成功
													{	
														recv1_data_ok=1;  
																										//接收到正确完整数据包标志位置位
													}													
													else
													{
														recv1_data_p=0;
													}
													recv1_data_p=0;
													j=0;			
											}	
									}					
									else
									recv1_data_p=0;
							}
					}
					else
					recv1_data_p=0;
				}
				else
				recv1_data_p=0;
		}
}



void Uart0DataAnalyzing(void)       //串口0数据解析
{
	//unsigned char datatemp[1];
	u8 i;
  if(recv1_data_ok==1)      //串口1接收一帧数据完成
	{
	  if(((recv_buffer[4]==FX_NUMBER%255)&&(recv_buffer[3]==FX_NUMBER/255))||((recv_buffer[4]==0xff)&&(recv_buffer[3]==0xff)))  //接收到的蜂箱编号与本地编号相同,或者为广播信号。才进行解码
	  {
			LED2=!LED2;	
			switch(recv_buffer[5])                                   //接收数组的第6个字节为命令字
			{	
				case SET_WASHING :                                     //执行清洗蜂箱动作  
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x32;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //串口屏上显示 清洗中...
						if(recv_buffer[8]==0x01)                             //上位机发送清洗指令,开
						{
							Washing_Time=recv_buffer[6];                       //清洗时间低字节
							Washing_Time=(Washing_Time<<8)+recv_buffer[7];     //清洗时间
						  GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_12,SET);	
							START_WASHING_FLAG=TRUE;                           //开始清洗蜂箱标志位
						}
						else
						{
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_12, RESET);	// 输出低，控制电磁阀1，停止进水
							START_WASHING_FLAG=FALSE; 
						}
				}		
				break;			
				case SET_FEED :                                     	 //进行喂食
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x31;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //串口屏上显示 喂食中...
						if(recv_buffer[8]==0x01)                          	 //上位机发送喂食开
						{
							Feed_Time=recv_buffer[6];                      		 //喂食时间低字节
							Feed_Time=(Feed_Time<<8)+recv_buffer[7];       	 	 //喂食时间
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, SET);   	 //拉高引脚开始喂食            
							FEED_START_FLAG=TRUE;                           	 //开始喂食时间计数
						}
						else
						{
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, RESET); 	 //  输出低，停止喂食           
							FEED_START_FLAG=FALSE;                          	 //开始喂食时间计数关闭
						}
				}
					break;
				
				case SET_TEMPTURE :                                  	 //执行加热
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x33;
				  LED4 = 0;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //串口屏上显示 加热中...
						if(recv_buffer[8]==0x01)                            	//上位机发送加热开
						{
							Heat_Time=recv_buffer[6];                          //加热时间低字节
							Heat_Time=(Heat_Time<<8)+recv_buffer[7];        	 //加热时间
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9 , SET);    //拉高PC9引脚开始加热  
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_10, SET);    //拉高PC10引脚开始加热    
							HEAT_START_FLAG=TRUE;                              //开始加热时间计数
						}
						else
						{
							LED4 = 1;
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);     //拉低引脚停止加热   
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_10, RESET);     //拉低引脚停止加热           
							HEAT_START_FLAG=FALSE;                                //开始加热时间计数关闭
						}                                           //开始加热时间计数关闭
				}	
					break;
				
				case RETRUN_MESSAGE :
				{
				  DataBuffer[7]=TEXT_Buffer[0];                             //存入数组  
					send_buffer[0]=0xAA;                                   //数据包 开头
					send_buffer[1]=0x55;
					send_buffer[2]=0x0C;                                   //数据包数据长度
					send_buffer[3]=FX_NUMBER/255;                          //蜂箱编号
					send_buffer[4]=FX_NUMBER%255;   
					send_buffer[5]=0x04;                                   //命令字节返回
					for(i=0;i<8;i++)                                       //将重量、温度、开口状态存入发送数组
			    send_buffer[i+6]=DataBuffer[i];         
    
					  for(i=0;i<14;i++)                                    //计算校验字
			    sum_check+=send_buffer[i];
					send_buffer[14]=sum_check;                                   //数据包数据长度
					USART_Sends(HT_USART0,send_buffer,sizeof(send_buffer));  
					sum_check=0;
				}
				break;
				
				case CONTR_BEE_STATE1 :                                  //开口打开最大全部蜜蜂都可通过
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x36;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //串口屏上显示 ...					
					//SPI_FLASH_BufferRead((u8*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer));	//读取蜂箱开口状态					
					if(TEXT_Buffer[0]==0x02)                                  //蜂箱开口居中，顺时针转动120度，回到开口最大
					{		
						Motor_NAngle(1,FALSE);                                 //顺时针转动120度
						Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
					}
					else if(TEXT_Buffer[0]==0x01)                             //蜂箱开口最小，顺时针转动240度，回到开口最大
					{	
						Motor_NAngle(2,FALSE);                                //顺时针转动240度
						Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
					}
	       	TEXT_Buffer[0]=0x04;
					//SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,1);           //写入蜂箱开口状态
				}
				break;
				
				case CONTR_BEE_STATE2 :                                //蜂王禁止通行
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x35;					
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //串口屏上显示 ...
					
			  	//SPI_FLASH_BufferRead((u8*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer));	//读取蜂箱开口状态
				
					if(TEXT_Buffer[0]==0x04)                                  //蜂箱开口居中，顺时针转动120度，回到开口最大
					{		
						Motor_NAngle(1,TRUE);                                 //顺时针转动120度
						Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
					}
					else if(TEXT_Buffer[0]==0x01)                             //蜂箱开口最小，顺时针转动240度，回到开口最大
					{	
						Motor_NAngle(1,FALSE);                                //顺时针转动240度
						Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
					}
	       	TEXT_Buffer[0]=0x02;
					//SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,1);           //写入蜂箱开口状态	
				}
				break;
				
				case CONTR_BEE_STATE3 :                                //蜂王禁止通行且开口最小
				{
					dz_CMD[7]=0x34;
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //串口屏上显示 ...
					
					//SPI_FLASH_BufferRead((u8*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer));	//读取蜂箱开口状态
					
					if(TEXT_Buffer[0]==0x04)                                  //蜂箱开口居中，顺时针转动120度，回到开口最大
					{		
						Motor_NAngle(2,TRUE);                                 //顺时针转动120度
						Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
					}
					else if(TEXT_Buffer[0]==0x02)                             //蜂箱开口最小，顺时针转动240度，回到开口最大
					{	
						Motor_NAngle(1,TRUE);                                //顺时针转动240度
						Motor_Stop();		//输出低(步进电机在控制完后拉低，保证不工作)
					}
	       	TEXT_Buffer[0]=0x01;
					//SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,1);           //写入蜂箱开口状态			
				}
					break;
				default:
					break;	
			}
		
	  }
	   recv1_data_ok=0;    //接收标志位清0，等待下一次接收

	}
}


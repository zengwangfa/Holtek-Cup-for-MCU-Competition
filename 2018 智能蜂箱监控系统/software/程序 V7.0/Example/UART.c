//------------------UART.c---------------------//
#include "UART.h"

#define MAX_DATA_LENS 127
#define USART_TIMEOUT_ON          ((u32)0x00000080)

extern unsigned char temperature;             //�����ڵ��¶�ֵ
unsigned char recv_buffer[(MAX_DATA_LENS+4)]; //����1�������ݻ������,
unsigned char send_buffer[(MAX_DATA_LENS+4)]; //����1�������ݻ������,
unsigned char recv1_data_p=0x00;              //����1��������ָ��
unsigned char recv1_data_ok=0;                //���յ���ȷ�����ݰ�
unsigned char STEER_START_RUN_FLAG=0;         //�����ʼ��ת��־λ,0��ʾֹͣ��ת��1��ʾ��ʼ��ת
extern     u8 TEXT_Buffer[2];                  //��һ���ֽ�Ϊ����Ŀ���״̬��0x01��ʾ�������0x02���У�0x03��С
bool          HEAT_START_FLAG=FALSE;          //��ʼ���Ƽ���˿���ȱ�־λ
bool          START_WASHING_FLAG=FALSE;       //��ʼ��ϴ�����־λ
bool          FEED_START_FLAG=FALSE;          //��ʼιʳ�����־λ
extern unsigned char DataBuffer[8];           //���������ݴ����飬����2�ֽڵ�����+2�ֽڵ��¶�+1�ֽڵķ���״̬
unsigned  int Washing_Time=0;                 //��ϴʱ�䣬����λ������
unsigned  int Heat_Time=0;                    //����ʱ�䣬����λ����������λ��
unsigned  int Feed_Time=0;                    //ιʳʱ�䣬����λ����������λ��
unsigned char sum_check=0;
u8 Gate_Motor_flag=0x03;    //����򿪱�־�
u8 Gate_Motor=0x00;    //����򿪱�־λ�
u8 dz_CMD[]={0x64,0x7A,0x2E,0x76,0x61,0x6C,0x3D,0x31,0xff,0xff,0xff};
 //74 31 39 2E 74 78 74 3D 22 32 31 22 ff ff ff         t19.txt="21"


void USART_Sends(HT_USART_TypeDef* USARTx,u8 *Data,u16 len)
{
	u16 i=0;
  for(;i<len;i++)
  USART_SendData(USARTx,Data[i]);
}


void USART1_IRQHandler(void)  //����1 �����ж�   ���մ��������ſ���
{
  /* Rx, move data from USART FIFO to buffer            */
  if (((HT_USART1->IIR) & USART_IID_RDA ) == USART_IID_RDA ) //�����ж�
  {
		Gate_Motor=0x01;
		Gate_Motor_flag = USART_ReceiveData(HT_USART1);
		if(Gate_Motor_flag == 0xA6 && Gate_Motor==0x01 )  //��������
		{
			LED3=0;
			dz_CMD[7]=0x36;
		  USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //����������ʾ .�������
			if(TEXT_Buffer[0]==0x02)                                  //���俪�ھ��У�˳ʱ��ת��120�ȣ��ص��������
			{		
				Motor_NAngle(1,FALSE);                                 //˳ʱ��ת��120��
				Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
			}
			else if(TEXT_Buffer[0]==0x01)                             //���俪����С��˳ʱ��ת��240�ȣ��ص��������
			{	
				Motor_NAngle(2,FALSE);                                //˳ʱ��ת��240��
				Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
			}
			TEXT_Buffer[0]=0x04;
			Gate_Motor=0x00;
		}
		else if(Gate_Motor_flag == 0xA5 && Gate_Motor==0x01)   //�ر�
		{
			LED3=1;
			dz_CMD[7]=0x34;
		  USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //����������ʾ .���Źر�
			if(TEXT_Buffer[0]==0x04)                                  //���俪�ھ��У�˳ʱ��ת��120�ȣ��ص��������
			{		
				Motor_NAngle(2,TRUE);                                 //˳ʱ��ת��120��
				Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
			}
			else if(TEXT_Buffer[0]==0x02)                             //���俪����С��˳ʱ��ת��240�ȣ��ص��������
			{	
				Motor_NAngle(1,TRUE);                                //˳ʱ��ת��240��
				Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
			}
			TEXT_Buffer[0]=0x01;
			Gate_Motor=0x00;
   	}
		else {Gate_Motor=0x00;}
	}
}


void USART0_IRQHandler(void)  //����0 �����ж�
{
	u8 i=0,j=0;
  /* Rx, move data from USART FIFO to buffer            */
	
  if (((HT_USART0->IIR) & USART_IID_RDA ) == USART_IID_RDA ) //�����ж�
  {
		
		recv_buffer[recv1_data_p] =USART_ReceiveData(HT_USART0);	 //��ȡ����1���յ�������
		recv1_data_p++; 

			if(recv1_data_p<(MAX_DATA_LENS+4))    //�������ݳ���δ������ͷ�Ͱ���3���ֽ�,+4
			 {

				if(recv_buffer[0]==0xaa)  //���ݰ���ͷ�ֽ�
					{

						if(recv1_data_p>3)
							{

								if(recv_buffer[1]==0x55)
									{	
												
										if(recv1_data_p>=(recv_buffer[2]+3))  //���������ݰ����������ֽ�Ϊ���ݳ��ȣ����ݳ��Ȳ�������ͷ��У���֣�
											{
												
													for(i=0;i<=(recv1_data_p-2);i++)  //���յ����ݰ������һ���ֽ�ΪУ���֣�У�鷽�����������ݽ����ۼӣ�������У���֣�
													{
														j +=recv_buffer[i];	
													}
													
													if(j==recv_buffer[recv1_data_p-1])     //�ж�У���Ƿ�ɹ�
													{	
														recv1_data_ok=1;  
																										//���յ���ȷ�������ݰ���־λ��λ
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



void Uart0DataAnalyzing(void)       //����0���ݽ���
{
	//unsigned char datatemp[1];
	u8 i;
  if(recv1_data_ok==1)      //����1����һ֡�������
	{
	  if(((recv_buffer[4]==FX_NUMBER%255)&&(recv_buffer[3]==FX_NUMBER/255))||((recv_buffer[4]==0xff)&&(recv_buffer[3]==0xff)))  //���յ��ķ������뱾�ر����ͬ,����Ϊ�㲥�źš��Ž��н���
	  {
			LED2=!LED2;	
			switch(recv_buffer[5])                                   //��������ĵ�6���ֽ�Ϊ������
			{	
				case SET_WASHING :                                     //ִ����ϴ���䶯��  
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x32;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //����������ʾ ��ϴ��...
						if(recv_buffer[8]==0x01)                             //��λ��������ϴָ��,��
						{
							Washing_Time=recv_buffer[6];                       //��ϴʱ����ֽ�
							Washing_Time=(Washing_Time<<8)+recv_buffer[7];     //��ϴʱ��
						  GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_12,SET);	
							START_WASHING_FLAG=TRUE;                           //��ʼ��ϴ�����־λ
						}
						else
						{
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_12, RESET);	// ����ͣ����Ƶ�ŷ�1��ֹͣ��ˮ
							START_WASHING_FLAG=FALSE; 
						}
				}		
				break;			
				case SET_FEED :                                     	 //����ιʳ
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x31;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //����������ʾ ιʳ��...
						if(recv_buffer[8]==0x01)                          	 //��λ������ιʳ��
						{
							Feed_Time=recv_buffer[6];                      		 //ιʳʱ����ֽ�
							Feed_Time=(Feed_Time<<8)+recv_buffer[7];       	 	 //ιʳʱ��
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, SET);   	 //�������ſ�ʼιʳ            
							FEED_START_FLAG=TRUE;                           	 //��ʼιʳʱ�����
						}
						else
						{
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, RESET); 	 //  ����ͣ�ֹͣιʳ           
							FEED_START_FLAG=FALSE;                          	 //��ʼιʳʱ������ر�
						}
				}
					break;
				
				case SET_TEMPTURE :                                  	 //ִ�м���
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x33;
				  LED4 = 0;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //����������ʾ ������...
						if(recv_buffer[8]==0x01)                            	//��λ�����ͼ��ȿ�
						{
							Heat_Time=recv_buffer[6];                          //����ʱ����ֽ�
							Heat_Time=(Heat_Time<<8)+recv_buffer[7];        	 //����ʱ��
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9 , SET);    //����PC9���ſ�ʼ����  
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_10, SET);    //����PC10���ſ�ʼ����    
							HEAT_START_FLAG=TRUE;                              //��ʼ����ʱ�����
						}
						else
						{
							LED4 = 1;
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);     //��������ֹͣ����   
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_10, RESET);     //��������ֹͣ����           
							HEAT_START_FLAG=FALSE;                                //��ʼ����ʱ������ر�
						}                                           //��ʼ����ʱ������ر�
				}	
					break;
				
				case RETRUN_MESSAGE :
				{
				  DataBuffer[7]=TEXT_Buffer[0];                             //��������  
					send_buffer[0]=0xAA;                                   //���ݰ� ��ͷ
					send_buffer[1]=0x55;
					send_buffer[2]=0x0C;                                   //���ݰ����ݳ���
					send_buffer[3]=FX_NUMBER/255;                          //������
					send_buffer[4]=FX_NUMBER%255;   
					send_buffer[5]=0x04;                                   //�����ֽڷ���
					for(i=0;i<8;i++)                                       //���������¶ȡ�����״̬���뷢������
			    send_buffer[i+6]=DataBuffer[i];         
    
					  for(i=0;i<14;i++)                                    //����У����
			    sum_check+=send_buffer[i];
					send_buffer[14]=sum_check;                                   //���ݰ����ݳ���
					USART_Sends(HT_USART0,send_buffer,sizeof(send_buffer));  
					sum_check=0;
				}
				break;
				
				case CONTR_BEE_STATE1 :                                  //���ڴ����ȫ���۷䶼��ͨ��
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x36;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //����������ʾ ...					
					//SPI_FLASH_BufferRead((u8*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer));	//��ȡ���俪��״̬					
					if(TEXT_Buffer[0]==0x02)                                  //���俪�ھ��У�˳ʱ��ת��120�ȣ��ص��������
					{		
						Motor_NAngle(1,FALSE);                                 //˳ʱ��ת��120��
						Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
					}
					else if(TEXT_Buffer[0]==0x01)                             //���俪����С��˳ʱ��ת��240�ȣ��ص��������
					{	
						Motor_NAngle(2,FALSE);                                //˳ʱ��ת��240��
						Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
					}
	       	TEXT_Buffer[0]=0x04;
					//SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,1);           //д����俪��״̬
				}
				break;
				
				case CONTR_BEE_STATE2 :                                //������ֹͨ��
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x35;					
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //����������ʾ ...
					
			  	//SPI_FLASH_BufferRead((u8*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer));	//��ȡ���俪��״̬
				
					if(TEXT_Buffer[0]==0x04)                                  //���俪�ھ��У�˳ʱ��ת��120�ȣ��ص��������
					{		
						Motor_NAngle(1,TRUE);                                 //˳ʱ��ת��120��
						Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
					}
					else if(TEXT_Buffer[0]==0x01)                             //���俪����С��˳ʱ��ת��240�ȣ��ص��������
					{	
						Motor_NAngle(1,FALSE);                                //˳ʱ��ת��240��
						Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
					}
	       	TEXT_Buffer[0]=0x02;
					//SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,1);           //д����俪��״̬	
				}
				break;
				
				case CONTR_BEE_STATE3 :                                //������ֹͨ���ҿ�����С
				{
					dz_CMD[7]=0x34;
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //����������ʾ ...
					
					//SPI_FLASH_BufferRead((u8*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer));	//��ȡ���俪��״̬
					
					if(TEXT_Buffer[0]==0x04)                                  //���俪�ھ��У�˳ʱ��ת��120�ȣ��ص��������
					{		
						Motor_NAngle(2,TRUE);                                 //˳ʱ��ת��120��
						Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
					}
					else if(TEXT_Buffer[0]==0x02)                             //���俪����С��˳ʱ��ת��240�ȣ��ص��������
					{	
						Motor_NAngle(1,TRUE);                                //˳ʱ��ת��240��
						Motor_Stop();		//�����(��������ڿ���������ͣ���֤������)
					}
	       	TEXT_Buffer[0]=0x01;
					//SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,1);           //д����俪��״̬			
				}
					break;
				default:
					break;	
			}
		
	  }
	   recv1_data_ok=0;    //���ձ�־λ��0���ȴ���һ�ν���

	}
}


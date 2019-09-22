//------------------UART.c---------------------//
#include "UART.h"

#define MAX_DATA_LENS 127
#define USART_TIMEOUT_ON          ((u32)0x00000080)

extern unsigned char temperature;             //·äÏäÄÚµÄÎÂ¶ÈÖµ
unsigned char recv_buffer[(MAX_DATA_LENS+4)]; //´®¿Ú1½ÓÊÕÊı¾İ»º³å±äÁ¿,
unsigned char send_buffer[(MAX_DATA_LENS+4)]; //´®¿Ú1·¢ËÍÊı¾İ»º³å±äÁ¿,
unsigned char recv1_data_p=0x00;              //´®¿Ú1½ÓÊÕÊı¾İÖ¸Õë
unsigned char recv1_data_ok=0;                //½ÓÊÕµ½ÕıÈ·µÄÊı¾İ°ü
unsigned char STEER_START_RUN_FLAG=0;         //¶æ»ú¿ªÊ¼ÔË×ª±êÖ¾Î»,0±íÊ¾Í£Ö¹ÔË×ª£¬1±íÊ¾¿ªÊ¼ÔË×ª
extern     u8 TEXT_Buffer[2];                  //µÚÒ»¸ö×Ö½ÚÎª·äÏäµÄ¿ª¿Ú×´Ì¬£¬0x01±íÊ¾¿ª¿Ú×î´ó£¬0x02¾ÓÖĞ£¬0x03×îĞ¡
bool          HEAT_START_FLAG=FALSE;          //¿ªÊ¼¿ØÖÆ¼ÓÈÈË¿¼ÓÈÈ±êÖ¾Î»
bool          START_WASHING_FLAG=FALSE;       //¿ªÊ¼ÇåÏ´·äÏä±êÖ¾Î»
bool          FEED_START_FLAG=FALSE;          //¿ªÊ¼Î¹Ê³·äÏä±êÖ¾Î»
extern unsigned char DataBuffer[8];           //·äÏäÊı¾İÔİ´æÊı×é£¬°üÀ¨2×Ö½ÚµÄÖØÁ¿+2×Ö½ÚµÄÎÂ¶È+1×Ö½ÚµÄ·äÍõ×´Ì¬
unsigned  int Washing_Time=0;                 //ÇåÏ´Ê±¼ä£¬ÓÉÉÏÎ»»ú¸ø¶¨
unsigned  int Heat_Time=0;                    //¼ÓÈÈÊ±¼ä£¬ÓÉÉÏÎ»»ú¸ø¶¨£¬µ¥Î»Ãë
unsigned  int Feed_Time=0;                    //Î¹Ê³Ê±¼ä£¬ÓÉÉÏÎ»»ú¸ø¶¨£¬µ¥Î»Ãë
unsigned char sum_check=0;
u8 Gate_Motor_flag=0x03;    //¶æ»ú´ò¿ª±êÖ¾Î
u8 Gate_Motor=0x00;    //¶æ»ú´ò¿ª±êÖ¾Î»»
u8 dz_CMD[]={0x64,0x7A,0x2E,0x76,0x61,0x6C,0x3D,0x31,0xff,0xff,0xff};
 //74 31 39 2E 74 78 74 3D 22 32 31 22 ff ff ff         t19.txt="21"


void USART_Sends(HT_USART_TypeDef* USARTx,u8 *Data,u16 len)
{
	u16 i=0;
  for(;i<len;i++)
  USART_SendData(USARTx,Data[i]);
}


void USART1_IRQHandler(void)  //´®¿Ú1 ½ÓÊÕÖĞ¶Ï   ½ÓÊÕ´®¿ÚÆÁ·äÃÅ¿ØÖÆ
{
  /* Rx, move data from USART FIFO to buffer            */
  if (((HT_USART1->IIR) & USART_IID_RDA ) == USART_IID_RDA ) //½ÓÊÕÖĞ¶Ï
  {
		Gate_Motor=0x01;
		Gate_Motor_flag = USART_ReceiveData(HT_USART1);
		if(Gate_Motor_flag == 0xA6 && Gate_Motor==0x01 )  //¿ªÆô·äÃÅ
		{
			LED3=0;
			dz_CMD[7]=0x36;
		  USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //´®¿ÚÆÁÉÏÏÔÊ¾ .·äÃÅ×î´ó
			if(TEXT_Buffer[0]==0x02)                                  //·äÏä¿ª¿Ú¾ÓÖĞ£¬Ë³Ê±Õë×ª¶¯120¶È£¬»Øµ½¿ª¿Ú×î´ó
			{		
				Motor_NAngle(1,FALSE);                                 //Ë³Ê±Õë×ª¶¯120¶È
				Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
			}
			else if(TEXT_Buffer[0]==0x01)                             //·äÏä¿ª¿Ú×îĞ¡£¬Ë³Ê±Õë×ª¶¯240¶È£¬»Øµ½¿ª¿Ú×î´ó
			{	
				Motor_NAngle(2,FALSE);                                //Ë³Ê±Õë×ª¶¯240¶È
				Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
			}
			TEXT_Buffer[0]=0x04;
			Gate_Motor=0x00;
		}
		else if(Gate_Motor_flag == 0xA5 && Gate_Motor==0x01)   //¹Ø±Õ
		{
			LED3=1;
			dz_CMD[7]=0x34;
		  USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //´®¿ÚÆÁÉÏÏÔÊ¾ .·äÃÅ¹Ø±Õ
			if(TEXT_Buffer[0]==0x04)                                  //·äÏä¿ª¿Ú¾ÓÖĞ£¬Ë³Ê±Õë×ª¶¯120¶È£¬»Øµ½¿ª¿Ú×î´ó
			{		
				Motor_NAngle(2,TRUE);                                 //Ë³Ê±Õë×ª¶¯120¶È
				Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
			}
			else if(TEXT_Buffer[0]==0x02)                             //·äÏä¿ª¿Ú×îĞ¡£¬Ë³Ê±Õë×ª¶¯240¶È£¬»Øµ½¿ª¿Ú×î´ó
			{	
				Motor_NAngle(1,TRUE);                                //Ë³Ê±Õë×ª¶¯240¶È
				Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
			}
			TEXT_Buffer[0]=0x01;
			Gate_Motor=0x00;
   	}
		else {Gate_Motor=0x00;}
	}
}


void USART0_IRQHandler(void)  //´®¿Ú0 ½ÓÊÕÖĞ¶Ï
{
	u8 i=0,j=0;
  /* Rx, move data from USART FIFO to buffer            */
	
  if (((HT_USART0->IIR) & USART_IID_RDA ) == USART_IID_RDA ) //½ÓÊÕÖĞ¶Ï
  {
		
		recv_buffer[recv1_data_p] =USART_ReceiveData(HT_USART0);	 //¶ÁÈ¡´®¿Ú1½ÓÊÕµ½µÄÊı¾İ
		recv1_data_p++; 

			if(recv1_data_p<(MAX_DATA_LENS+4))    //¶¨ÒåÊı¾İ³¤¶ÈÎ´°üÀ¨°üÍ·ºÍ°ü³¤3¸ö×Ö½Ú,+4
			 {

				if(recv_buffer[0]==0xaa)  //Êı¾İ°ü°üÍ·×Ö½Ú
					{

						if(recv1_data_p>3)
							{

								if(recv_buffer[1]==0x55)
									{	
												
										if(recv1_data_p>=(recv_buffer[2]+3))  //½ÓÊÕÍêÊı¾İ°ü£¨µÚÈı¸ö×Ö½ÚÎªÊı¾İ³¤¶È£¬Êı¾İ³¤¶È²»°üº¬¿ªÍ·ºÍĞ£Ñé×Ö£©
											{
												
													for(i=0;i<=(recv1_data_p-2);i++)  //½ÓÊÕµ½Êı¾İ°üµÄ×îºóÒ»¸ö×Ö½ÚÎªĞ£Ñé×Ö£»Ğ£Ñé·½·¨¶ÔÕû°üÊı¾İ½øĞĞÀÛ¼Ó£¨²»°üº¬Ğ£Ñé×Ö£©
													{
														j +=recv_buffer[i];	
													}
													
													if(j==recv_buffer[recv1_data_p-1])     //ÅĞ¶ÏĞ£ÑéÊÇ·ñ³É¹¦
													{	
														recv1_data_ok=1;  
																										//½ÓÊÕµ½ÕıÈ·ÍêÕûÊı¾İ°ü±êÖ¾Î»ÖÃÎ»
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



void Uart0DataAnalyzing(void)       //´®¿Ú0Êı¾İ½âÎö
{
	//unsigned char datatemp[1];
	u8 i;
  if(recv1_data_ok==1)      //´®¿Ú1½ÓÊÕÒ»Ö¡Êı¾İÍê³É
	{
	  if(((recv_buffer[4]==FX_NUMBER%255)&&(recv_buffer[3]==FX_NUMBER/255))||((recv_buffer[4]==0xff)&&(recv_buffer[3]==0xff)))  //½ÓÊÕµ½µÄ·äÏä±àºÅÓë±¾µØ±àºÅÏàÍ¬,»òÕßÎª¹ã²¥ĞÅºÅ¡£²Å½øĞĞ½âÂë
	  {
			LED2=!LED2;	
			switch(recv_buffer[5])                                   //½ÓÊÕÊı×éµÄµÚ6¸ö×Ö½ÚÎªÃüÁî×Ö
			{	
				case SET_WASHING :                                     //Ö´ĞĞÇåÏ´·äÏä¶¯×÷  
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x32;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //´®¿ÚÆÁÉÏÏÔÊ¾ ÇåÏ´ÖĞ...
						if(recv_buffer[8]==0x01)                             //ÉÏÎ»»ú·¢ËÍÇåÏ´Ö¸Áî,¿ª
						{
							Washing_Time=recv_buffer[6];                       //ÇåÏ´Ê±¼äµÍ×Ö½Ú
							Washing_Time=(Washing_Time<<8)+recv_buffer[7];     //ÇåÏ´Ê±¼ä
						  GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_12,SET);	
							START_WASHING_FLAG=TRUE;                           //¿ªÊ¼ÇåÏ´·äÏä±êÖ¾Î»
						}
						else
						{
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_12, RESET);	// Êä³öµÍ£¬¿ØÖÆµç´Å·§1£¬Í£Ö¹½øË®
							START_WASHING_FLAG=FALSE; 
						}
				}		
				break;			
				case SET_FEED :                                     	 //½øĞĞÎ¹Ê³
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x31;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //´®¿ÚÆÁÉÏÏÔÊ¾ Î¹Ê³ÖĞ...
						if(recv_buffer[8]==0x01)                          	 //ÉÏÎ»»ú·¢ËÍÎ¹Ê³¿ª
						{
							Feed_Time=recv_buffer[6];                      		 //Î¹Ê³Ê±¼äµÍ×Ö½Ú
							Feed_Time=(Feed_Time<<8)+recv_buffer[7];       	 	 //Î¹Ê³Ê±¼ä
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, SET);   	 //À­¸ßÒı½Å¿ªÊ¼Î¹Ê³            
							FEED_START_FLAG=TRUE;                           	 //¿ªÊ¼Î¹Ê³Ê±¼ä¼ÆÊı
						}
						else
						{
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, RESET); 	 //  Êä³öµÍ£¬Í£Ö¹Î¹Ê³           
							FEED_START_FLAG=FALSE;                          	 //¿ªÊ¼Î¹Ê³Ê±¼ä¼ÆÊı¹Ø±Õ
						}
				}
					break;
				
				case SET_TEMPTURE :                                  	 //Ö´ĞĞ¼ÓÈÈ
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x33;
				  LED4 = 0;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //´®¿ÚÆÁÉÏÏÔÊ¾ ¼ÓÈÈÖĞ...
						if(recv_buffer[8]==0x01)                            	//ÉÏÎ»»ú·¢ËÍ¼ÓÈÈ¿ª
						{
							Heat_Time=recv_buffer[6];                          //¼ÓÈÈÊ±¼äµÍ×Ö½Ú
							Heat_Time=(Heat_Time<<8)+recv_buffer[7];        	 //¼ÓÈÈÊ±¼ä
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9 , SET);    //À­¸ßPC9Òı½Å¿ªÊ¼¼ÓÈÈ  
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_10, SET);    //À­¸ßPC10Òı½Å¿ªÊ¼¼ÓÈÈ    
							HEAT_START_FLAG=TRUE;                              //¿ªÊ¼¼ÓÈÈÊ±¼ä¼ÆÊı
						}
						else
						{
							LED4 = 1;
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);     //À­µÍÒı½ÅÍ£Ö¹¼ÓÈÈ   
							GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_10, RESET);     //À­µÍÒı½ÅÍ£Ö¹¼ÓÈÈ           
							HEAT_START_FLAG=FALSE;                                //¿ªÊ¼¼ÓÈÈÊ±¼ä¼ÆÊı¹Ø±Õ
						}                                           //¿ªÊ¼¼ÓÈÈÊ±¼ä¼ÆÊı¹Ø±Õ
				}	
					break;
				
				case RETRUN_MESSAGE :
				{
				  DataBuffer[7]=TEXT_Buffer[0];                             //´æÈëÊı×é  
					send_buffer[0]=0xAA;                                   //Êı¾İ°ü ¿ªÍ·
					send_buffer[1]=0x55;
					send_buffer[2]=0x0C;                                   //Êı¾İ°üÊı¾İ³¤¶È
					send_buffer[3]=FX_NUMBER/255;                          //·äÏä±àºÅ
					send_buffer[4]=FX_NUMBER%255;   
					send_buffer[5]=0x04;                                   //ÃüÁî×Ö½Ú·µ»Ø
					for(i=0;i<8;i++)                                       //½«ÖØÁ¿¡¢ÎÂ¶È¡¢¿ª¿Ú×´Ì¬´æÈë·¢ËÍÊı×é
			    send_buffer[i+6]=DataBuffer[i];         
    
					  for(i=0;i<14;i++)                                    //¼ÆËãĞ£Ñé×Ö
			    sum_check+=send_buffer[i];
					send_buffer[14]=sum_check;                                   //Êı¾İ°üÊı¾İ³¤¶È
					USART_Sends(HT_USART0,send_buffer,sizeof(send_buffer));  
					sum_check=0;
				}
				break;
				
				case CONTR_BEE_STATE1 :                                  //¿ª¿Ú´ò¿ª×î´óÈ«²¿ÃÛ·ä¶¼¿ÉÍ¨¹ı
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x36;
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //´®¿ÚÆÁÉÏÏÔÊ¾ ...					
					//SPI_FLASH_BufferRead((u8*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer));	//¶ÁÈ¡·äÏä¿ª¿Ú×´Ì¬					
					if(TEXT_Buffer[0]==0x02)                                  //·äÏä¿ª¿Ú¾ÓÖĞ£¬Ë³Ê±Õë×ª¶¯120¶È£¬»Øµ½¿ª¿Ú×î´ó
					{		
						Motor_NAngle(1,FALSE);                                 //Ë³Ê±Õë×ª¶¯120¶È
						Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
					}
					else if(TEXT_Buffer[0]==0x01)                             //·äÏä¿ª¿Ú×îĞ¡£¬Ë³Ê±Õë×ª¶¯240¶È£¬»Øµ½¿ª¿Ú×î´ó
					{	
						Motor_NAngle(2,FALSE);                                //Ë³Ê±Õë×ª¶¯240¶È
						Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
					}
	       	TEXT_Buffer[0]=0x04;
					//SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,1);           //Ğ´Èë·äÏä¿ª¿Ú×´Ì¬
				}
				break;
				
				case CONTR_BEE_STATE2 :                                //·äÍõ½ûÖ¹Í¨ĞĞ
				{
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					dz_CMD[7]=0x35;					
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //´®¿ÚÆÁÉÏÏÔÊ¾ ...
					
			  	//SPI_FLASH_BufferRead((u8*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer));	//¶ÁÈ¡·äÏä¿ª¿Ú×´Ì¬
				
					if(TEXT_Buffer[0]==0x04)                                  //·äÏä¿ª¿Ú¾ÓÖĞ£¬Ë³Ê±Õë×ª¶¯120¶È£¬»Øµ½¿ª¿Ú×î´ó
					{		
						Motor_NAngle(1,TRUE);                                 //Ë³Ê±Õë×ª¶¯120¶È
						Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
					}
					else if(TEXT_Buffer[0]==0x01)                             //·äÏä¿ª¿Ú×îĞ¡£¬Ë³Ê±Õë×ª¶¯240¶È£¬»Øµ½¿ª¿Ú×î´ó
					{	
						Motor_NAngle(1,FALSE);                                //Ë³Ê±Õë×ª¶¯240¶È
						Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
					}
	       	TEXT_Buffer[0]=0x02;
					//SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,1);           //Ğ´Èë·äÏä¿ª¿Ú×´Ì¬	
				}
				break;
				
				case CONTR_BEE_STATE3 :                                //·äÍõ½ûÖ¹Í¨ĞĞÇÒ¿ª¿Ú×îĞ¡
				{
					dz_CMD[7]=0x34;
					USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));  
					USART_Sends(HT_USART1,dz_CMD,sizeof(dz_CMD));				 //´®¿ÚÆÁÉÏÏÔÊ¾ ...
					
					//SPI_FLASH_BufferRead((u8*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer));	//¶ÁÈ¡·äÏä¿ª¿Ú×´Ì¬
					
					if(TEXT_Buffer[0]==0x04)                                  //·äÏä¿ª¿Ú¾ÓÖĞ£¬Ë³Ê±Õë×ª¶¯120¶È£¬»Øµ½¿ª¿Ú×î´ó
					{		
						Motor_NAngle(2,TRUE);                                 //Ë³Ê±Õë×ª¶¯120¶È
						Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
					}
					else if(TEXT_Buffer[0]==0x02)                             //·äÏä¿ª¿Ú×îĞ¡£¬Ë³Ê±Õë×ª¶¯240¶È£¬»Øµ½¿ª¿Ú×î´ó
					{	
						Motor_NAngle(1,TRUE);                                //Ë³Ê±Õë×ª¶¯240¶È
						Motor_Stop();		//Êä³öµÍ(²½½øµç»úÔÚ¿ØÖÆÍêºóÀ­µÍ£¬±£Ö¤²»¹¤×÷)
					}
	       	TEXT_Buffer[0]=0x01;
					//SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,1);           //Ğ´Èë·äÏä¿ª¿Ú×´Ì¬			
				}
					break;
				default:
					break;	
			}
		
	  }
	   recv1_data_ok=0;    //½ÓÊÕ±êÖ¾Î»Çå0£¬µÈ´ıÏÂÒ»´Î½ÓÊÕ

	}
}


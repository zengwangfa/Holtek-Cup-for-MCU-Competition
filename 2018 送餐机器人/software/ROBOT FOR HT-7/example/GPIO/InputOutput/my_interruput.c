
/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "usart_int.h"
#include "controll_main.h"


unsigned char recv_buffer[(MAX_DATA_LENS+4)]; //串口1接收数据缓冲变量,
unsigned char lastrecv_buffer[(MAX_DATA_LENS+4)];
unsigned char recv1_data_p=0x00;              //串口1接收数据指针
unsigned char recv1_data_ok=0;                //接收到正确的数据包
unsigned char recv1_data_ok1=0;                //接收到正确的数据包
unsigned char recvl_data_err[9]={0xAA,0x55,0x06,0x00,0x01,0xc1,0x00,0x01,0xc8};
extern unsigned char answer;

unsigned char curve_number=0;     //路径中弯道数量

unsigned char food_buffer[(MAX_DATA_LENS+4)]={0};
unsigned char walk_route[(MAX_DATA_LENS+4)]={0};

extern int back_flag;          //返回标志位
extern int timer_5ms_count;   //中断计时标志



void UART_other(void);
/* Private define ------------------------------------------------------------------------------------------*/
#define USART_TIMEOUT_ON          ((u32)0x00000080)

/* Private variables ---------------------------------------------------------------------------------------*/

/* USART0 definitions                                                                                       */
u8 UR0TxBuf[USART_BUFSIZE];
u8 UR0RxBuf[USART_BUFSIZE];
u32 UR0TxReadIndex = 0;
vu32 UR0TxWriteIndex = 0;
u32 UR0RxReadIndex = 0;
vu32 UR0RxWriteIndex = 0;

/* USART1 definitions                                                                                       */
u8 UR1TxBuf[USART_BUFSIZE];
u8 UR1RxBuf[USART_BUFSIZE];
u32 UR1TxReadIndex = 0;
vu32 UR1TxWriteIndex = 0;
u32 UR1RxReadIndex = 0;
vu32 UR1RxWriteIndex = 0;
/*user-define*/


/*********************************************************************************************************//**
 * @brief   This function handles USART0 interrupt.
 * @retval  None
 ************************************************************************************************************/
void USART0_IRQHandler(void)
{
	unsigned char k = 0,m=0;
	unsigned char i=0,j=0;
  /* Rx, move data from USART FIFO to buffer                                                                */
  if (((HT_USART0->IIR) & USART_IID_RDA ) == USART_IID_RDA ) //接受中断
  {
		recv_buffer[recv1_data_p] =USART_ReceiveData(HT_USART0);	 //读取串口1接收到的数据
		recv1_data_p++; 
		if(recv1_data_p<(MAX_DATA_LENS+4))                     //定义数据长度未包括包头和包长3个字节,+4
		 {
			if(recv_buffer[0]==0xaa)//数据包包头字节
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
													recv1_data_ok=1;                 //接收到正确完整数据包标志位置位
													recv1_data_ok1=1; 
													answer =1;
													
													for( k = 0;k<recv1_data_p;k++)
													{
														USART_SendData(HT_USART0,recv_buffer[k]); //接收到正确数据，返回接受到的数据	
														if(recv_buffer[5]==0x01)               //保存送餐命令
															food_buffer[k]=recv_buffer[k];
														if((recv_buffer[5]==0x07)&&((k+6)<(recv1_data_p-1)))               //保存路径，需在送餐命令前发送路径信息
														{
															walk_route[k]=recv_buffer[k+6];   //从数据包中解析路径
															curve_number = (recv1_data_p-7);   //弯道数量
														}	
														if((recv_buffer[5]!=0x03)&&(recv_buffer[5]!=0x07))  //命令不为查询和路径时，保存命令
														{
															lastrecv_buffer[k]=recv_buffer[k]; 
														}
													}
												}
												else
												{
													//recv1_data_ok1=0; 
													recv1_data_p=0;
													for(m=0;m<9;m++)
													USART_SendData(HT_USART0,recvl_data_err[m]);   //接受错误，返回代表错误的数据
												}
												j=0;
												recv1_data_p=0;			
												//recv1_data_ok1=0; 														
										}	
							}					
							else
							{
								//recv1_data_ok1=0; 
								recv1_data_p=0;
								for(m=0;m<9;m++)
									USART_SendData(HT_USART0,recvl_data_err[m]);   //接受错误，返回代表错误的数据
							}
					}
			}
			else
			{
				//recv1_data_ok1=0; 
				recv1_data_p=0;
				for(m=0;m<9;m++)
					USART_SendData(HT_USART0,recvl_data_err[m]);   //接受错误，返回代表错误的数据
			}
	}
	else
	{
		//recv1_data_ok1=0; 
		recv1_data_p=0;
		for(m=0;m<9;m++)
			USART_SendData(HT_USART0,recvl_data_err[m]);   //接受错误，返回代表错误的数据
	}
		
  }
  UART_other();
}





void USART_Interrupt_Init(HT_USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStructure, USART_INT_Mode Mode)
{
  /* Configures USARTx                                                                                      */
  USART_Init(USARTx, USART_InitStructure);
  USART_FIFOReset(USARTx, USART_FIFO_TX | USART_FIFO_RX);

  if (Mode == USART_TX_INT)
  {
    /* Set TX FIFO Level, empty in FIFO                                                                     */
    USART_TFITLConfig(USARTx, USART_TFITL_00);
    USART_TxCmd(USARTx, ENABLE);
  }
  else if (Mode == USART_RX_INT)
  {
    /* Set RX FIFO Level, 1 byte in FIFO                                                                    */
    USART_RFITLConfig(USARTx, USART_RFITL_01);
    /* Enable RX FIFO Interrupt                                                                             */
    USART_IntConfig(USARTx, USART_IER_RDAIE, ENABLE);
    /* Set RX Timeout                                                                                       */
    USARTx->TPR = USART_TIMEOUT_ON | 40;
    USART_RxCmd(USARTx, ENABLE);
  }
  else if (Mode == USART_ALL_INT)
  {
    USART_TFITLConfig(USARTx, USART_TFITL_00);
    USART_TxCmd(USARTx, ENABLE);
    USART_RFITLConfig(USARTx, USART_RFITL_01);
    USART_IntConfig(USARTx, USART_IER_ALL, ENABLE);
    USARTx->TPR = USART_TIMEOUT_ON | 40;
    USART_RxCmd(USARTx, ENABLE);
  }

  if (USARTx == HT_USART0)
  {
    NVIC_EnableIRQ(USART0_IRQn);
  }
  else
  {
    NVIC_EnableIRQ(UART0_IRQn);
  }
}

void UART_other(void)
{
	 u32 i;  //发送中断数据长度记录
	/* Tx, move data from buffer to USART FIFO                                                                */
  if (((HT_USART0->IIR) & USART_IID_THRE ) == USART_IID_THRE )//发送中断
  {
    if (IS_BUFFER_EMPTY(UR0TxReadIndex, UR0TxWriteIndex))
    {
      USART_IntConfig(HT_USART0, USART_IER_THREIE, DISABLE);
    }
    else
    {
      for (i = 0; i < 1; i++)
      {
        USART_SendData(HT_USART0, UR0TxBuf[UR0TxReadIndex]);//将发送缓存器的内容放在UR0TxBuf[128]数组中				
        UR0TxReadIndex = (UR0TxReadIndex + 1) % USART_BUFSIZE;//数组长度累计
        //HT32F_DVB_LEDOff(HT_LED1);//发送灯亮
        if (IS_BUFFER_EMPTY(UR0TxReadIndex, UR0TxWriteIndex))
        {
          break;
        }
      }
    }
  }

}





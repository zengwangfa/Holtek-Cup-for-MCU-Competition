
/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "usart_int.h"
#include "controll_main.h"


unsigned char recv_buffer[(MAX_DATA_LENS+4)]; //����1�������ݻ������,
unsigned char lastrecv_buffer[(MAX_DATA_LENS+4)];
unsigned char recv1_data_p=0x00;              //����1��������ָ��
unsigned char recv1_data_ok=0;                //���յ���ȷ�����ݰ�
unsigned char recv1_data_ok1=0;                //���յ���ȷ�����ݰ�
unsigned char recvl_data_err[9]={0xAA,0x55,0x06,0x00,0x01,0xc1,0x00,0x01,0xc8};
extern unsigned char answer;

unsigned char curve_number=0;     //·�����������

unsigned char food_buffer[(MAX_DATA_LENS+4)]={0};
unsigned char walk_route[(MAX_DATA_LENS+4)]={0};

extern int back_flag;          //���ر�־λ
extern int timer_5ms_count;   //�жϼ�ʱ��־



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
  if (((HT_USART0->IIR) & USART_IID_RDA ) == USART_IID_RDA ) //�����ж�
  {
		recv_buffer[recv1_data_p] =USART_ReceiveData(HT_USART0);	 //��ȡ����1���յ�������
		recv1_data_p++; 
		if(recv1_data_p<(MAX_DATA_LENS+4))                     //�������ݳ���δ������ͷ�Ͱ���3���ֽ�,+4
		 {
			if(recv_buffer[0]==0xaa)//���ݰ���ͷ�ֽ�
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
													recv1_data_ok=1;                 //���յ���ȷ�������ݰ���־λ��λ
													recv1_data_ok1=1; 
													answer =1;
													
													for( k = 0;k<recv1_data_p;k++)
													{
														USART_SendData(HT_USART0,recv_buffer[k]); //���յ���ȷ���ݣ����ؽ��ܵ�������	
														if(recv_buffer[5]==0x01)               //�����Ͳ�����
															food_buffer[k]=recv_buffer[k];
														if((recv_buffer[5]==0x07)&&((k+6)<(recv1_data_p-1)))               //����·���������Ͳ�����ǰ����·����Ϣ
														{
															walk_route[k]=recv_buffer[k+6];   //�����ݰ��н���·��
															curve_number = (recv1_data_p-7);   //�������
														}	
														if((recv_buffer[5]!=0x03)&&(recv_buffer[5]!=0x07))  //���Ϊ��ѯ��·��ʱ����������
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
													USART_SendData(HT_USART0,recvl_data_err[m]);   //���ܴ��󣬷��ش�����������
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
									USART_SendData(HT_USART0,recvl_data_err[m]);   //���ܴ��󣬷��ش�����������
							}
					}
			}
			else
			{
				//recv1_data_ok1=0; 
				recv1_data_p=0;
				for(m=0;m<9;m++)
					USART_SendData(HT_USART0,recvl_data_err[m]);   //���ܴ��󣬷��ش�����������
			}
	}
	else
	{
		//recv1_data_ok1=0; 
		recv1_data_p=0;
		for(m=0;m<9;m++)
			USART_SendData(HT_USART0,recvl_data_err[m]);   //���ܴ��󣬷��ش�����������
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
	 u32 i;  //�����ж����ݳ��ȼ�¼
	/* Tx, move data from buffer to USART FIFO                                                                */
  if (((HT_USART0->IIR) & USART_IID_THRE ) == USART_IID_THRE )//�����ж�
  {
    if (IS_BUFFER_EMPTY(UR0TxReadIndex, UR0TxWriteIndex))
    {
      USART_IntConfig(HT_USART0, USART_IER_THREIE, DISABLE);
    }
    else
    {
      for (i = 0; i < 1; i++)
      {
        USART_SendData(HT_USART0, UR0TxBuf[UR0TxReadIndex]);//�����ͻ����������ݷ���UR0TxBuf[128]������				
        UR0TxReadIndex = (UR0TxReadIndex + 1) % USART_BUFSIZE;//���鳤���ۼ�
        //HT32F_DVB_LEDOff(HT_LED1);//���͵���
        if (IS_BUFFER_EMPTY(UR0TxReadIndex, UR0TxWriteIndex))
        {
          break;
        }
      }
    }
  }

}





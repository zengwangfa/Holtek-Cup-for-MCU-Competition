#include "usart_int.h"


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



  /*
        USARTx configured as follow:
            - BaudRate = 115200 baud
            - Word Length = 8 Bits
            - One Stop Bit
            - None parity bit
  */
void usart_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	/* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPAConfig(AFIO_PIN_2 | AFIO_PIN_3, AFIO_MODE_6);

  /* Config AFIO mode as UART0_Rx and UART0_Tx function.                                                    */
  AFIO_GPCConfig(AFIO_PIN_9 | AFIO_PIN_10, AFIO_MODE_6);

  /* Config AFIO mode as USARTx function                                                                    */
  USART_InitStructure.USART_BaudRate = 115200;    //波特率设置
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;    //8位数据位
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;     //一位停止位
  USART_InitStructure.USART_Parity = USART_PARITY_NO;       //无校验位
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;        //正常收发模式

  /* Configure USART0 & UART0 interrupt                                                                     */
  USART_Interrupt_Init(HT_USART0, &USART_InitStructure, USART_ALL_INT);   //USART0中断
  USART_Interrupt_Init(HT_UART0, &USART_InitStructure, USART_ALL_INT);    //UART中断
}


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Initialize the USARTx peripheral in interrupt mode.
  * @param  USARTx: where USARTx is USART to select the USART peripheral, x can be 0 or 1.
  * @param  USART_InitStructure: pointer to a USART_InitTypeDef structure that contains the configuration information
  *         for the specified USART peripheral.
  * @param  Mode: where Mode is USART to select the USART interrupt initial setting.
  * @retval None.
  ***********************************************************************************************************/
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




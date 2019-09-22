//------------------INIT.C---------------------//
#include "INIT.h"     

__IO uint32_t wMctmUpdateDownCounter = 0;  //Used to save the numbers of Update Event occurred of MCTM1 
uint32_t wCRR = 43999, wPSCR = 0;// wBeeIndex = 0

#define USART_TIMEOUT_ON          ((u32)0x00000080)


/* USART0 definitions   ���ò���          */
u8 UR0TxBuf[USART_BUFSIZE];
u8 UR0RxBuf[USART_BUFSIZE];
u32 UR0TxReadIndex = 0;
vu32 UR0TxWriteIndex = 0;
u32 UR0RxReadIndex = 0;
vu32 UR0RxWriteIndex = 0;

/* USART1 definitions                    */
u8 UR1TxBuf[USART_BUFSIZE];
u8 UR1RxBuf[USART_BUFSIZE];
u32 UR1TxReadIndex = 0;
vu32 UR1TxWriteIndex = 0;
u32 UR1RxReadIndex = 0;
vu32 UR1RxWriteIndex = 0;


void System_Init(void)
{
	//FlagStatus TmpStatus = RESET;
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.BFTM0      = 1;
	CKCUClock.Bit.BFTM1      = 1;
	CKCUClock.Bit.PA         = 1;
  CKCUClock.Bit.PB         = 1;
  CKCUClock.Bit.PC         = 1;
	CKCUClock.Bit.PD         = 1;
	CKCUClock.Bit.PE         = 1;
  CKCUClock.Bit.AFIO       = 1;
	CKCUClock.Bit.ADC        = 1;
  CKCUClock.Bit.MCTM1      = 1;
	CKCUClock.Bit.USART0     = 1;
	CKCUClock.Bit.USART1     = 1;
  //CKCUClock.Bit.UART0      = 1;
	CKCUClock.Bit.GPTM0      = 1;
  CKCUClock.Bit.BKP        = 1;
  //CKCUClock.Bit.SPI0     = 1;
  //CKCUClock.Bit.SPI1     = 1;                                                                                 */

  BUZZER_TM_CLK(CKCUClock) = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);


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
    NVIC_EnableIRQ(USART0_IRQn);//�����ж��� 
  }
  else
  {
    NVIC_EnableIRQ(UART0_IRQn); //�����ж��� 
  }
}


void CTR_IO_Init()  //�ⲿ���Ƶ�ŷ��ͼ���Ƭ���ų�ʼ��(��ŷ����� ιʳ����ϴ)
{
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_9, AFIO_MODE_DEFAULT);   //�˿�����
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_10, AFIO_MODE_DEFAULT);  
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_11, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_12, AFIO_MODE_DEFAULT);	
 	
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_9, GPIO_DIR_OUT);//�������
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_10, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_11, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_12, GPIO_DIR_OUT);
	
	GPIO_OpenDrainConfig(HT_GPIOC,GPIO_PIN_9,DISABLE);				//�������
	GPIO_OpenDrainConfig(HT_GPIOC,GPIO_PIN_10,DISABLE);				//�������
	GPIO_OpenDrainConfig(HT_GPIOC,GPIO_PIN_11,DISABLE);				//�������
	GPIO_OpenDrainConfig(HT_GPIOC,GPIO_PIN_12,DISABLE);				//�������//D5 ����ͣ����Ƽ���˿

	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);  	//C9  ����ͣ�����MOS,���Ƽ���
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_10, RESET); 	//C10 ����ͣ�����MOS,���Ƽ���
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, RESET); 	//C11 ����ͣ����Ƶ�ŷ�1������ιʳ
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_12, RESET);  //C12 �����, ���Ƶ�ŷ�2��������ϴ
	
	AFIO_GPxConfig(GPIO_PD, AFIO_PIN_11, AFIO_MODE_DEFAULT);	//��λ����GATE����
	GPIO_DirectionConfig(HT_GPIOD, AFIO_PIN_11, GPIO_DIR_IN);	 //���뷽��
	GPIO_PullResistorConfig(HT_GPIOD, AFIO_PIN_11, GPIO_PR_UP); //��������
  GPIO_InputConfig(HT_GPIOD, AFIO_PIN_11, ENABLE);  		       //ʹ������

				      
}
void USART0_Init(u32 bound)   //����WiFi����λ��ͨ�� TX.(PA2)    RX.(PA3)   57600
{

	USART_InitTypeDef USART_InitStructure;

	/* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPAConfig(AFIO_PIN_2 | AFIO_PIN_3, AFIO_MODE_6);   //USART0 IO��ʼ��


  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;	

  	//USART_Init(HT_USART0, &USART_InitStructure);
    USART_TxCmd(HT_USART0, ENABLE);
    USART_RxCmd(HT_USART0, ENABLE);

  /* Configure USART0 & UART0 interrupt                                                                     */
  USART_Interrupt_Init(HT_USART0, &USART_InitStructure, USART_RX_INT);//ѡ������ж�    

	NVIC_EnableIRQ(USART0_IRQn);//�����ж���
}


void USART1_Init(u32 bound)  //���ڴ�����ͨѶ       TX.(PA4)    RX.(PA5)
{

  USART_InitTypeDef USART_InitStructure;

  /* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPAConfig(AFIO_PIN_4 | AFIO_PIN_5, AFIO_MODE_6);    

  /* USART0 configuration ----------------------------------------------------------------------------------*/

  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

  USART_Init(HT_USART1, &USART_InitStructure);
  USART_TxCmd(HT_USART1, ENABLE);
  USART_RxCmd(HT_USART1, ENABLE);
	USART_Interrupt_Init(HT_USART1, &USART_InitStructure, USART_RX_INT);//ѡ������ж�    
	NVIC_EnableIRQ(USART1_IRQn);//�����ж���
	
}
void Time0_Init()
{
		/********  ��ʱ��0�жϳ�ʼ�� *********/
	NVIC_EnableIRQ(BFTM0_IRQn);     //��ʱ���ж�0ʹ��
	BFTM_SetCompare(HT_BFTM0, SystemCoreClock/1000 * 5);//10ms��ʱ�ж�
  BFTM_SetCounter(HT_BFTM0, 0);     
  BFTM_IntConfig(HT_BFTM0, ENABLE);//ENABLEΪ1
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
}

void Time1_Init()
{
	NVIC_EnableIRQ(BFTM1_IRQn);     //��ʱʹ��
	BFTM_SetCompare(HT_BFTM1, SystemCoreClock/100 );//����1.5Khz���壬�ö�ʱ��1�����ȼ����ڶ�ʱ��0
  BFTM_SetCounter(HT_BFTM1, 0);  
	
	BFTM_IntConfig(HT_BFTM1, ENABLE);//ENABLEΪ1
	BFTM_EnaCmd(HT_BFTM1, ENABLE);
}


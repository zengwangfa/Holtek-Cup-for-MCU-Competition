#include "ht32.h"
#include "ht32_board.h"
#include "config.h"
#include "controll_main.h"
#include "usart_int.h"
#include "stdlib.h"

void usart0_init(void);
void usart1_init(void);

void USART_INT(void)
{
	USART_InitTypeDef USART_InitStructure;
	  /* Config AFIO mode as USARTx function  
	*/
	//System_Init();
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;	

	   /* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPAConfig(AFIO_PIN_2 | AFIO_PIN_3, AFIO_MODE_6);   //USART0 IO初始化

  /* Config AFIO mode as UART0_Rx and UART0_Tx function.                                                    */
  //AFIO_GPCConfig(AFIO_PIN_9 | AFIO_PIN_10, AFIO_MODE_6);    //UART0  IO初始化

  /* Configure USART0 & UART0 interrupt                                                                     */
  USART_Interrupt_Init(HT_USART0, &USART_InitStructure, USART_ALL_INT);//选择双向模式 PC9 TX   PC1O  RX
  //USART_Interrupt_Init(HT_UART0, &USART_InitStructure, USART_ALL_INT);   //PA2 TX   PA3 RX
}


void usart0_init(void)
{
	USART_InitTypeDef USART_InitStructure;

	/* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPAConfig(AFIO_PIN_2 | AFIO_PIN_3, AFIO_MODE_6);   //USART0 IO初始化

	
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;	

//	USART_Init(HT_USART0, &USART_InitStructure);
//  USART_TxCmd(HT_USART0, ENABLE);

  /* Configure USART0 & UART0 interrupt                                                                     */
  USART_Interrupt_Init(HT_USART0, &USART_InitStructure, USART_ALL_INT);//选择双向模式 PC9 TX   PC1O  RX

}

void usart1_init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  USART_InitTypeDef USART_InitStructure;

  /* Enable peripheral clock of AFIO, USART0                                                                */
  CKCUClock.Bit.AFIO   = 1;
  CKCUClock.Bit.USART1 = 1;
	//CKCUClock.Bit.USART1 = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPAConfig(AFIO_PIN_4 | AFIO_PIN_5, AFIO_MODE_6);     // TX (PA2), RX (PA3)

  /* USART0 configuration ----------------------------------------------------------------------------------*/
  /* USART0 configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - None parity bit
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

  USART_Init(HT_USART1, &USART_InitStructure);
  USART_TxCmd(HT_USART1, ENABLE);
  USART_RxCmd(HT_USART1, ENABLE);
	
	

	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_1, AFIO_MODE_DEFAULT);    //用于音乐播放模块状态检测，处于播放或是空闲
  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_1, GPIO_DIR_IN);  
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_1, GPIO_PR_UP);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_1, ENABLE);
	
}




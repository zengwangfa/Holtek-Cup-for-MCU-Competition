#include "ht32.h"
#include "ht32_board.h"
#include "config.h"
#include "action.h"
#include "controll_main.h"
#include "key.h"
#include "usart_int.h"
#include "stdlib.h"
#include "ht32f1655_56.h"
/*可用通信助手实现所有功能*/
 u8 full = 0;
 u8 empty = 12;
 u8 t = 0;//产生随机数
 u8 f = 1;//产生密码 

int main(void)
{ 
	USART_InitTypeDef USART_InitStructure;
	  /* Config AFIO mode as USARTx function  
	*/
	System_Init();
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

  /* Configure USART0 & UART0 interrupt                                                                     */
  USART_Interrupt_Init(HT_USART0, &USART_InitStructure, USART_ALL_INT);//选择双向模式
  USART_Interrupt_Init(HT_UART0, &USART_InitStructure, USART_ALL_INT);

	Port_Init();
	Init();
  showgarage(full,empty);
  while (1)
  { 
   PCexchange();
		//KeyDriver();     
	}
}

#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
  */

  while (1)
  {
  }
}
#endif


void delay(void)
{
 	unsigned int i = 100;
	while(i--);
}

void delay_wt(void)
{
   	unsigned int i,j;
	for(j=0;j<1;j++)
	{
	for(i=0;i<20000;i++);
	} 
}
void delayX(void)
{
	unsigned int i,j;
	for(j=0;j<12;j++)
	{
	for(i=0;i<50000;i++);
	}
}
 void ZMoveback(void)
{
	
 	  Zred = 0;   
		Zblack = 1; 
	  delayY();
    while(Zb == 1);
	  Zblack = 0;  	 

}

void delayY(void)
{
    unsigned int i,j;
	  for(j=0;j<20;j++)
	  {
			  for(i=0;i<50000;i++);
	  }
}

void ZMovefront(void)
{
    Zred = 1;   
		Zblack = 0;  
	  while(Zf == 1);
	  Zred = 0; 
}


void Init(void)
{
    Cx = 1;     //x,y轴片选0断开，1为断开
	  Cy = 1;
	  Zred = 0;   //Z轴运动控制位
	  Zblack = 0;
	
}
 void System_Init(void)
 {
	
	FlagStatus TmpStatus = RESET;
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.BFTM0      = 1;
	CKCUClock.Bit.BFTM1      = 1;
	CKCUClock.Bit.PA         = 1;
  CKCUClock.Bit.PB         = 1;
  CKCUClock.Bit.PC         = 1;
	CKCUClock.Bit.PD         = 1;
	CKCUClock.Bit.PE         = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.BKP        = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
  /* Enable peripheral clock of AFIO, USART0, USART1 */
	CKCUClock.Bit.USART0     = 1;
  CKCUClock.Bit.UART0      = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
//  vu8 temp[10] = {0}, temp1[10] = {0};
   /* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPAConfig(AFIO_PIN_2 | AFIO_PIN_3, AFIO_MODE_6);

  /* Config AFIO mode as UART0_Rx and UART0_Tx function.                                                    */
  AFIO_GPCConfig(AFIO_PIN_9 | AFIO_PIN_10, AFIO_MODE_6);


}
void Port_Init(void)
{	
/*定义输入限位PC(6 7)*/
  /* Configure AFIO mode of input pins                                                                      */
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_0, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_2, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_3, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_4, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_5, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_6, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_7, AFIO_MODE_DEFAULT);
 

  /* Configure GPIO direction of input pins                                                                 */
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_IN); 
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_2, GPIO_DIR_IN);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_3, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_4, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_5, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_6, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_7, GPIO_DIR_IN);
 

  /* Configure GPIO pull resistor of input pins                                                             */
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_0, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_1, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_2, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_3, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_4, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_5, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_6, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_7, GPIO_PR_UP);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_0, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_1, ENABLE);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_2, ENABLE);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_3, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_4, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_5, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_6, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_7, ENABLE);
	
/*定义前进后退控制引脚*/	
	  /* Configure AFIO mode of output pins                                                                     */
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_0, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_1, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_2, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_3, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_4, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_MODE_DEFAULT);
	
  

  /* Configure GPIO direction of output pins                                                                */
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_0, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_2, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_4, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_5, GPIO_DIR_OUT);
	
	/*定义键盘端口PA输入*/
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_14, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_12, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_13, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_10, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_11, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_8, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_9, AFIO_MODE_DEFAULT);
	
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_14, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_IN); 
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_12, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_13, GPIO_DIR_IN);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_11, GPIO_DIR_OUT); 
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_8, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_9, GPIO_DIR_OUT);
	
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_14, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_12, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_13, GPIO_PR_UP);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_14, ENABLE);
  GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_12, ENABLE);
  GPIO_InputConfig(HT_GPIOA, GPIO_PIN_13, ENABLE);
	
	/*测试程序是否运行*/
	/*AFIO_GPxConfig(GPIO_PD, AFIO_PIN_4, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_OUT);
		AFIO_GPxConfig(GPIO_PD, AFIO_PIN_5, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_5, GPIO_DIR_OUT);
		AFIO_GPxConfig(GPIO_PD, AFIO_PIN_6, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_6, GPIO_DIR_OUT);*/
		AFIO_GPxConfig(GPIO_PD, AFIO_PIN_7, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_7, GPIO_DIR_OUT);
	
	HT32F_DVB_LEDInit(HT_LED1);   //产生脉冲初始化
	//HT32F_DVB_LEDInit(HT_LED2);

	

	NVIC_EnableIRQ(BFTM1_IRQn);     //定时使能
	BFTM_SetCompare(HT_BFTM1, SystemCoreClock/15000 * 5);//产生1.5Khz脉冲，用定时器1，优先级低于定时器0
  BFTM_SetCounter(HT_BFTM1, 0);  
	
 BFTM_IntConfig(HT_BFTM1, ENABLE);//ENABLE为1
 BFTM_EnaCmd(HT_BFTM1, ENABLE);
	
	NVIC_EnableIRQ(BFTM0_IRQn);     //定时使能
	BFTM_SetCompare(HT_BFTM0, SystemCoreClock/10000 * 5);//0.5ms定时
  BFTM_SetCounter(HT_BFTM0, 0);                        
  BFTM_IntConfig(HT_BFTM0, ENABLE);//ENABLE为1
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
}

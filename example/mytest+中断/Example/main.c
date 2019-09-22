
#include "ht32.h"
#include "ht32_board.h"
#include "zh_key.h"
#include "ZH_GPTM.h"
#include "ZH_BFTM.h"
#include "usart_int.h"


/* Private variables ---------------------------------------------------------------------------------------*/
static vu32 DelayTime;
/* Private function prototypes -----------------------------------------------------------------------------*/
void Delay(u32 nTime);
void TimingDelay(void);

void NVIC_Configuration(void);
void CKCU_Configuration(void);
void GPIO_Configuration(void);


void CKOUTConfig(void);

int main(void)
{
	 
	//usart_init();   //串口初始化
	//USART_SendData(HT_USART0,0x01);
	
	NVIC_Configuration();                                           /* 中断向量表配置  */
  CKCU_Configuration();                                           /* 时钟单元配置 */
  GPIO_Configuration();                                           /* IO口配置 */
	
	//ZH_Key_Init();                                                  /* 键盘初始化 */
  ZH_BFTM_Init();                                                 /* 基本功能定时器初始化 */
  
	
	ZH_GPTM_Init();                                                 /* 通用定时器初始化 */
  ZH_GPTM_Switch(ENABLE);                                         /* 通用定时器总开关 */
  ZH_GPTMCH_Switch(DISABLE);                                      /* 通用定时器通道开关 */
	
/*	
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};  //开启时钟
  CKCUClock.Bit.PA         = 1;    //GPIOA时钟打开
  CKCUClock.Bit.PC         = 1;		 //GPIOC时钟打开
  CKCUClock.Bit.PD         = 1;		 //GPIOD时钟打开
  CKCUClock.Bit.PE         = 1;		 //GPIOE时钟打开
  CKCUClock.Bit.AFIO       = 1;    //时钟复用
  //CKCUClock.Bit.BKP        = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);   //开启外设时钟*/

  /* Check if the backup domain is ready for access so as to disable isolation.
     Because some GPIO pins are located in backup domain. */
//  if (PWRCU_CheckReadyAccessed() != PWRCU_OK)
//  {
//    while (1);
//  }

	  /* SYSTICK configuration */
//  SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK); // Default : 9MHz on chip
//  SYSTICK_SetReloadValue(9000);                 // (9k/9MHz) = 1ms on chip
//  SYSTICK_IntConfig(ENABLE);                    // Enable SYSTICK Interrupt



//  /* Infinite loop to read data from input pin and then output to LED                                       */
  while (1)
  {
		
    /* Read WEAKUP and then output to LED1                                                                  */
    //TmpStatus = GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15);
   // GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_7, TmpStatus);
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, SET);
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, RESET);
		
		Delay(1000000);
		
		//GPIO_ClearOutBits(HT_GPIOC,GPIO_PIN_9);
		//USART_SendData(HT_USART0,0x01);
		
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, SET);
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);
		Delay(1000000);
		//GPIO_ClearOutBits(HT_GPIOC,GPIO_PIN_9);
    /* Read KEY1 and then output to LED2                                                                    */
//    TmpStatus = GPIO_ReadInBit(HT_GPIOE, GPIO_PIN_1);
//    GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_8, TmpStatus);

//    /* Read KEY2 and then output to LED3                                                                    */
//    TmpStatus = GPIO_ReadInBit(HT_GPIOE, GPIO_PIN_2);
//    GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_10, TmpStatus);
  }
}






/*********************************************************************************************************//**
  * @brief  Configure the GPIO ports.
  * @retval None
  ***********************************************************************************************************/
void GPIO_Configuration(void)
{
  //AFIO_GPxConfig(GPIO_PC, GPIO_PIN_9 | GPIO_PIN_11, AFIO_MODE_1);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_9 | GPIO_PIN_11, AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_9 | GPIO_PIN_11, GPIO_DIR_OUT);
  GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_9 | GPIO_PIN_11);
  
  //  GPIO_DriveConfig(HT_GPIOA, GPIO_PIN_7, GPIO_DV_8MA);
	
	  /* Infinite loop */
	
	
  /* Configure WEAKUP, KEY1, KEY2 pins as the input function                                                */

  /* Configure AFIO mode of input pins                                                                      */
  //  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_15, AFIO_MODE_1);
  //  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_1, AFIO_MODE_DEFAULT);
  //  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_2, AFIO_MODE_DEFAULT);

  //  /* Configure GPIO direction of input pins                                                                 */
  //  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_15, GPIO_DIR_IN);
  //  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_1, GPIO_DIR_IN);
  //  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_2, GPIO_DIR_IN);

  //  /* Configure GPIO pull resistor of input pins                                                             */
  //  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_15, GPIO_PR_DOWN);
  //  GPIO_PullResistorConfig(HT_GPIOE, GPIO_PIN_1, GPIO_PR_UP);
  //  GPIO_PullResistorConfig(HT_GPIOE, GPIO_PIN_2, GPIO_PR_UP);

  //  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_15, ENABLE);
  //  GPIO_InputConfig(HT_GPIOE, GPIO_PIN_1, ENABLE);
  //  GPIO_InputConfig(HT_GPIOE, GPIO_PIN_2, ENABLE);


//  /* Configure LED1, LED2, LED3 pins as output function                                                     */

//  /* Configure AFIO mode of output pins                                                                     */
//  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_7, AFIO_MODE_DEFAULT);
//	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_9, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_8, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_10, AFIO_MODE_DEFAULT);

//  /* Configure GPIO direction of output pins                                                                */
//  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_7, GPIO_DIR_OUT);
//	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_9, GPIO_DIR_OUT);
//  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_8, GPIO_DIR_OUT);
//  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10, GPIO_DIR_OUT);
	
}




/*********************************************************************************************************//**
  * @brief  Configure the NVIC vector table.
  * @retval None
  ***********************************************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 0x0);     /* Set the Vector Table base location at 0x00000000   */
}



void CKCU_Configuration(void)
{
/*
//<e0> Enable Peripheral Clock
//  <h> Communication
//    <q5> EBI
//    <q12> I2C0   <q13> I2C1
//    <q23> I2S
//    <q22> SCI
//    <q14> SPI0   <q15> SPI1
//    <q18> UART0  <q19> UART1
//    <q16> USART0 <q17> USART1
//    <q3>  USB
//  </h>
//  <h> IO
//    <q7> GPIO Port A <q8>  GPIO Port B <q9>  GPIO Port C <q10>  GPIO Port D <q11>  GPIO Port E
//    <q20> AFIO
//    <q21> EXTI
//  </h>
//  <h> System
//    <q34> ADC
//    <q4>  CKREF
//    <q6>  CRC
//    <q32> OPA/CMP0 <q33> OPA/CMP1
//    <q2>  PDMA
//  </h>
//  <h> Timer
//    <q30> BFTM0 <q31> BFTM1
//    <q28> GPTM0 <q29> GPTM1
//    <q24> MCTM0 <q25> MCTM1
//    <q27> RTC   <q26> WDT
//  </h>
//</e>
*/

  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.PDMA       = 0;
  CKCUClock.Bit.USBD       = 0;
  CKCUClock.Bit.CKREF      = 0;
  CKCUClock.Bit.EBI        = 0;
  CKCUClock.Bit.CRC        = 0;
  CKCUClock.Bit.PA         = 0;
  CKCUClock.Bit.PB         = 0;
  CKCUClock.Bit.PC         = 1;
  CKCUClock.Bit.PD         = 0;
  CKCUClock.Bit.PE         = 0;
  CKCUClock.Bit.I2C0       = 0;
  CKCUClock.Bit.I2C1       = 0;
  CKCUClock.Bit.SPI0       = 0;
  CKCUClock.Bit.SPI1       = 0;
  CKCUClock.Bit.USART0     = 1;
  CKCUClock.Bit.USART1     = 0;
  CKCUClock.Bit.UART0      = 1;
  CKCUClock.Bit.UART1      = 0;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.EXTI       = 0;
  CKCUClock.Bit.SCI        = 0;
  CKCUClock.Bit.I2S        = 0;
  CKCUClock.Bit.MCTM0      = 0;
  CKCUClock.Bit.MCTM1      = 0;
  CKCUClock.Bit.WDT        = 0;
  CKCUClock.Bit.RTC        = 0;
  CKCUClock.Bit.GPTM0      = 1;
  CKCUClock.Bit.GPTM1      = 0;
  CKCUClock.Bit.BFTM0      = 0;
  CKCUClock.Bit.BFTM1      = 0;
  CKCUClock.Bit.OPA0       = 0;
  CKCUClock.Bit.OPA1       = 0;
  CKCUClock.Bit.ADC        = 0;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

#if (ENABLE_CKOUT == 1)
  CKOUTConfig();
#endif
}



void TimingDelay(void)
{
  if(DelayTime != 0)
  {
    DelayTime--;
  }
}

void Delay(u32 nTime)
{
    /* Enable the SysTick Counter */
    //SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);

    DelayTime = nTime;

    while(DelayTime != 0)
		{
			TimingDelay();
		}

    /* Disable SysTick Counter */
   // SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
    /* Clear SysTick Counter */
   // SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
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


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */



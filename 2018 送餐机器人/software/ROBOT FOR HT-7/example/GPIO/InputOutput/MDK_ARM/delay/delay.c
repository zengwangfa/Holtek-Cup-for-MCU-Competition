#include "delay.h"

void delay_init(void)
{
     /* SYSTICK configuration */
  SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK); // Default : 9MHz on chip
  SYSTICK_SetReloadValue(9000);                 // (9k/9MHz) = 1ms on chip
  SYSTICK_IntConfig(ENABLE);                    // Enable SYSTICK Interrupt
		
}

/*********************************************************************************************************//**
  * @brief  Insert Delay time.
  * @param  nTime: Insert Delay time.
  * @retval None
  ***********************************************************************************************************/
void Delay(u32 nTime)
{
    /* Enable the SysTick Counter */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);

    DelayTime = nTime;

    while(DelayTime != 0);

    /* Disable SysTick Counter */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
    /* Clear SysTick Counter */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}

/*********************************************************************************************************//**
  * @brief  Decrements the TimingDelay variable.
  * @retval None
  ***********************************************************************************************************/
void TimingDelay(void)
{
  if(DelayTime != 0)
  {
    DelayTime--;
  }
}




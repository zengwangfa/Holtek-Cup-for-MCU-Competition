
/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"

void pwm1_init(void);
void pwm2_init(void);


///* Private variables ---------------------------------------------------------------------------------------*/
__IO uint32_t wMctmUpdateDownCounter = 0;   /*!< Used to save the numbers of Update Event occurred of MCTM1 */
  uint32_t wCRR = 23999, wPSCR = 0;//, wBeeIndex = 0
void delayX(void);
int main(void)
{

	pwm1_init();
	pwm2_init();
	
  while (1)
	{
		delayX();
		wCRR-=100;
		MCTM_SetCounterReload(HT_MCTM1, wCRR);
		GPTM_SetCounterReload(HT_GPTM0, wCRR);
		if(wCRR<14999)wCRR=63999;
		//wMctmUpdateDownCounter
	}
}



void pwm1_init(void)
{
	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  MCTM_TimeBaseInitTypeDef MCTM_TimeBaseInitStructure;
  MCTM_OutputInitTypeDef MCTM_OutputInitStructure;


  /* Enable PCLK of BUZZER and AFIO                                                                         */
  BUZZER_TM_CLK(CKCUClock) = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Configure the BUZZER_GPIO_PIN as MCTM channel output AFIO function                                     */
  HT32F_DVB_GPxConfig(BUZZER_GPIO_ID, BUZZER_AFIO_PIN|BUZZER_AFIO_PIN2, BUZZER_AFIO_MODE);

  /* Init BUZZER MCTM time-base                                                                             */
  MCTM_TimeBaseInitStructure.CounterReload = wCRR;
  MCTM_TimeBaseInitStructure.Prescaler = 0;
  MCTM_TimeBaseInitStructure.RepetitionCounter = 0;
  MCTM_TimeBaseInitStructure.CounterMode = MCTM_CNT_MODE_UP;
  MCTM_TimeBaseInitStructure.PSCReloadTime = MCTM_PSC_RLD_IMMEDIATE;
  MCTM_TimeBaseInit(BUZZER_TM, &MCTM_TimeBaseInitStructure);

  /* Clear Update Event Interrupt flag                                                                      */
  //MCTM_ClearFlag(BUZZER_TM, MCTM_FLAG_UEV1);

  /* Init BUZZER MCTM Channel x to output PWM waveform with 33% duty                                        */
  MCTM_OutputInitStructure.Channel = BUZZER_TM_CHANNEL;
  MCTM_OutputInitStructure.OutputMode = MCTM_OM_PWM2;
  MCTM_OutputInitStructure.Control = MCTM_CHCTL_ENABLE;
  MCTM_OutputInitStructure.ControlN = MCTM_CHCTL_DISABLE;//MCTM_CHCTL_ENABLE;
  MCTM_OutputInitStructure.Polarity = MCTM_CHP_NONINVERTED;
  MCTM_OutputInitStructure.PolarityN = MCTM_CHP_NONINVERTED;
  MCTM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
  MCTM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
  MCTM_OutputInitStructure.Compare = ((wCRR + 1) * (100 - 50)) / 100;
  MCTM_OutputInit(BUZZER_TM, &MCTM_OutputInitStructure);

  /* BUZZER MCTM Channel Main Output enable                                                                 */
  MCTM_CHMOECmd(BUZZER_TM, ENABLE);
  /* BUZZER MCTM counter enable                                                                             */
  MCTM_Cmd(BUZZER_TM, ENABLE);

}



void pwm2_init(void)
{
	
	  

	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  GPTM_TimeBaseInitTypeDef TimeBaseInit;
  GPTM_OutputInitTypeDef OutInit;
  u32 wCRR = 0;

  //RETARGET_Configuration();           /* Retarget Related configuration                                     */

  CKCUClock.Bit.PA         = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.GPTM0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);


  /* Config AFIO mode of GT0_CH1 and GT1_CH1 as GPTM function                                               */
  AFIO_GPAConfig(AFIO_PIN_1, AFIO_MODE_4);


  /* GPTM0 Time-Base configuration                                                                          */
  wCRR = (SystemCoreClock / 2000) - 1;
  GPTM_TimeBaseStructInit(&TimeBaseInit);
  TimeBaseInit.CounterMode = GPTM_CNT_MODE_UP;
  TimeBaseInit.CounterReload = 23999;
  TimeBaseInit.Prescaler = 0;
  TimeBaseInit.PSCReloadTime = GPTM_PSC_RLD_UPDATE;
  GPTM_TimeBaseInit(HT_GPTM0, &TimeBaseInit);

  /* GPTM0 Channel 1 output configuration                                                                   */
  GPTM_OutputStructInit(&OutInit);
  OutInit.Channel = GPTM_CH_1;
  OutInit.OutputMode = GPTM_OM_PWM1;
  OutInit.Control = GPTM_CHCTL_ENABLE;
  OutInit.Polarity = GPTM_CHP_NONINVERTED;
  OutInit.Compare = ((wCRR + 1) * 33) / 100;
  GPTM_OutputInit(HT_GPTM0, &OutInit);

  /* Enable GPTM0                                                                                           */
  GPTM_Cmd(HT_GPTM0, ENABLE);

}


void delayX(void)
{
	unsigned int i,j;
	for(j=0;j<12;j++)
	{
	for(i=0;i<50000;i++);
	}
}

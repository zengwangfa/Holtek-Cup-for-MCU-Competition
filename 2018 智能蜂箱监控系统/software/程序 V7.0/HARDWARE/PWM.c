#include "PWM.h"

int pwm1_value = 20;
int pwm2_value = 20;

u32 pwm1_truevalue=0;
u32 pwm2_truevalue=0;

extern uint32_t wCRR;



void PWM1_Init(void)  //PE3   
{

	//CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  MCTM_TimeBaseInitTypeDef MCTM_TimeBaseInitStructure;
  MCTM_OutputInitTypeDef MCTM_OutputInitStructure;

  /* Enable PCLK of BUZZER and AFIO                                                                         */
//  BUZZER_TM_CLK(CKCUClock) = 1;
//  CKCUClock.Bit.AFIO       = 1;
//  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Configure the BUZZER_GPIO_PIN as MCTM channel output AFIO function                                     */
  HT32F_DVB_GPxConfig(BUZZER_GPIO_ID, BUZZER_AFIO_PIN, BUZZER_AFIO_MODE);

  /* Init BUZZER MCTM time-base                                                                             */
  MCTM_TimeBaseInitStructure.CounterReload = wCRR;
  MCTM_TimeBaseInitStructure.Prescaler = 0;
  MCTM_TimeBaseInitStructure.RepetitionCounter = 0;
  MCTM_TimeBaseInitStructure.CounterMode = MCTM_CNT_MODE_UP;
  MCTM_TimeBaseInitStructure.PSCReloadTime = MCTM_PSC_RLD_IMMEDIATE;
  MCTM_TimeBaseInit(BUZZER_TM, &MCTM_TimeBaseInitStructure);


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



void PWM2_Init(void)   //PA6
{
	  
	//CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  GPTM_TimeBaseInitTypeDef TimeBaseInit;
  GPTM_OutputInitTypeDef OutInit;

  /* Config AFIO mode of GT0_CH1 and GT1_CH1 as GPTM function                                               */
  AFIO_GPAConfig(AFIO_PIN_6, AFIO_MODE_4);

  /* GPTM0 Time-Base configuration                                */
	//wCRR0 = (SystemCoreClock / 2000) - 1;
  GPTM_TimeBaseStructInit(&TimeBaseInit);
  TimeBaseInit.CounterMode = GPTM_CNT_MODE_UP;
  TimeBaseInit.CounterReload = wCRR;
  TimeBaseInit.Prescaler = 0;
  TimeBaseInit.PSCReloadTime = GPTM_PSC_RLD_UPDATE;
  GPTM_TimeBaseInit(HT_GPTM0, &TimeBaseInit);

  /* GPTM0 Channel 1 output configuration                                                                   */
  GPTM_OutputStructInit(&OutInit);
  OutInit.Channel = GPTM_CH_2;
  OutInit.OutputMode = GPTM_OM_PWM1;
  OutInit.Control = GPTM_CHCTL_ENABLE;
  OutInit.Polarity = GPTM_CHP_NONINVERTED;
  OutInit.Compare = ((wCRR + 1) * 33) / 100;
  GPTM_OutputInit(HT_GPTM0, &OutInit);

  /* Enable GPTM0                                                                                           */
  GPTM_Cmd(HT_GPTM0, ENABLE);

}

/*
       pwm1_value     	 19			18			16			14			12			10			8    (*3000)
				
   [Êµ²â]Õ¼¿Õ±È£¨%£©		61.2   58.6    53.7    48.3     38     32.6    6.6
*/
void PWM_Control(void)//
{

	pwm1_truevalue=pwm1_value*3000;

  MCTM_SetCounterReload(HT_MCTM1, pwm1_truevalue); //PE3
	GPTM_SetCounterReload(HT_GPTM0, 50000); //PA6

} 





#include "ht32.h"
#include "ht32_board.h"
#include "config.h"
#include "controll_main.h"
#include "usart_int.h"
#include "stdlib.h"
#include "ht32f1655_56.h"
#include "mfrc522.h"
#include "app_com.h"
#include "delay.h"
#include "ht32f1655_56_bftm.h"



__IO uint32_t wMctmUpdateDownCounter = 0;   /*!< Used to save the numbers of Update Event occurred of MCTM1 */
uint32_t wCRR = 43999, wPSCR = 0;//, wBeeIndex = 0

unsigned char data = 0;
unsigned char IO_data;

int err; 

unsigned char step_control(void)
{
	
		
	  IO_data = ReadData();

		if((IO_data == 0x80)||(IO_data == 0xc0))		//只要第八个检测到
			err = -6;
		else if(IO_data == 0x80)     //第七个检测到
			err = -5;
		else if(IO_data == 0x60)		//第六七个检测到
			err = -4;
		else if(IO_data == 0x30)		//第五六个检测到
			err = -3;
		else if(IO_data == 0x38)		//第四五六个检测到
			err = -2;			
		
		else if(IO_data == 0x18)   //第四五个检测到，传感器的中间位置
			err = 1;
		
		else if(IO_data == 0x1c)		//第三四五个检测到
			err = 2;		
		else if(IO_data == 0x0c)   //第三四个检测到
			err = 3;		
		else if(IO_data == 0x0e)		//第二三四个检测到
			err = 4;	
		else if(IO_data == 0x06)		//第二三个检测到
			err = 5;		
		else if((IO_data == 0x01)||(IO_data == 0x03))
			err = 6;
		else err = 0;
			
		return err;
}


unsigned char ReadData()
{
   data = GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_7);//8位
   data = data << 1;
   data |= GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_6);//7位
   data = data << 1;
   data |= GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_5);//6位
   data = data << 1;
   data |= GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_4);//5位
   data = data << 1;
   data |= GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_3);//4位
   data = data << 1;
   data |= GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_2);//3位
   data = data << 1;
   data |= GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_1);//2位
   data = data << 1;
   data |= GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_0);//1位
	
	 led_display(); 
	
   return  data;
}

void led_display(void)
{
	if(GPIO_ReadInBit(HT_GPIOC , GPIO_PIN_7)==1){
	  LED7=0;
	}
	else{
		LED7=1;
	}
	
	if(GPIO_ReadInBit(HT_GPIOC , GPIO_PIN_6)==1){
	  LED6=0;
	}

	else{
		LED6=1;
	}
	
	if(GPIO_ReadInBit(HT_GPIOC , GPIO_PIN_5)==1){
	  LED5=0;
	}
	else{
		LED5=1;
	}

	if(GPIO_ReadInBit(HT_GPIOC , GPIO_PIN_4)==1){
	  LED4=0;
	}
	else{
		LED4=1;
	}
	
	if(GPIO_ReadInBit(HT_GPIOC , GPIO_PIN_3)==1){
	  LED3=0;
	}
	else{
		LED3=1;
	}
	
	if(GPIO_ReadInBit(HT_GPIOC , GPIO_PIN_2)==1){
	  LED2=0;
	}
	else{
		LED2=1;
	}
	
	if(GPIO_ReadInBit(HT_GPIOC , GPIO_PIN_1)==1){
	  LED1=0;
	}
	else{
		LED1=1;
	}
	
	if(GPIO_ReadInBit(HT_GPIOC , GPIO_PIN_0)==1){
	  LED0=0;
	}
	else{
		LED0=1;
	}
	
}




void pwm1_init(void)  //PE3
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



void pwm2_init(void)   //PA6
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


void adc_init(void)    //PA0
{
	/* ADCLK frequency is set to 72/64 MHz = 1.125MHz                                                         */
  CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);

  /* Config AFIO mode as ADC function                                                                       */
  AFIO_GPAConfig(AFIO_PIN_0 , AFIO_MODE_2);

  /* Continuous Mode, Length 1, SubLength 1                                                                 */
  ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 1, 1);

  /* ADC Channel n, Rank 0, Sampling clock is (1.5 + 0) ADCLK
     Conversion time = (sampling clock + 12.5) / ADCLK = 12.4 uS */
  ADC_RegularChannelConfig(HT_ADC, ADC_CH_0, 1, 0);

  /* Use Software Trigger as ADC trigger source                                                             */
  ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);

  /* Software trigger to start continuous mode                                                              */
  ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);
}


void System_Init(void)
{

	//FlagStatus TmpStatus = RESET;
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.BFTM0      = 1;
	//CKCUClock.Bit.BFTM1      = 1;
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
  //CKCUClock.Bit.SPI0       = 1;
  //CKCUClock.Bit.SPI1       = 1;
  BUZZER_TM_CLK(CKCUClock) = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
  /* Enable peripheral clock of AFIO, USART0, USART1 */


  //CKCU_PeripClockConfig(CKCUClock, ENABLE);

}
void INPUT_Line_Init(void)
{	
	
	//PC[7:0] 为磁条传感器的检测
  /* Configure AFIO mode of input pins            配置GPIO为输入模式                                         */
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_0, AFIO_MODE_DEFAULT);	
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_2, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_3, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_4, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_5, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_6, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_7, AFIO_MODE_DEFAULT);
 

  /* Configure GPIO direction of input pins      配置GPIO方向为输入                                            */
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_IN); 
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_2, GPIO_DIR_IN);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_3, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_4, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_5, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_6, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_7, GPIO_DIR_IN);
 

  /* Configure GPIO pull resistor of input pins       配置为上拉输入                                            */
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_0, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_1, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_2, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_3, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_4, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_5, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_6, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_7, GPIO_PR_UP);
	
	//使能输入
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_0, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_1, ENABLE);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_2, ENABLE);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_3, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_4, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_5, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_6, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_7, ENABLE);

}

void DIR_Init(void)  //电机使能及方向
{
	/*定义前进后退控制引脚*/	
	  /* Configure AFIO mode of output pins                                                                     */
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_2, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_3, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_4, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_MODE_DEFAULT);
	
  /* Configure GPIO direction of output pins                                                                */
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_2, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_4, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_5, GPIO_DIR_OUT);
	
	
	
	/************* LED 测试*********************/
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_2, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_3, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(GPIO_PD, AFIO_PIN_4, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_5, AFIO_MODE_DEFAULT);

	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_2, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_3, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_5, GPIO_DIR_OUT);
	
	/************ LED 显示传感器检测状态  **************/
	AFIO_GPxConfig(GPIO_PD, AFIO_PIN_8|AFIO_PIN_9|AFIO_PIN_10|AFIO_PIN_11|AFIO_PIN_12|AFIO_PIN_13|AFIO_PIN_14|AFIO_PIN_15, AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_DIR_OUT);
	
	
	/**************** 用于仓门闭合控制*******************************/
	  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_6|AFIO_PIN_7, AFIO_MODE_DEFAULT);
		GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_6|GPIO_PIN_7, GPIO_DIR_OUT);
		
		
	/*定义键盘端口PA输入*/
//	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_14, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_12, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_13, AFIO_MODE_DEFAULT);
//	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_10, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_11, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_8, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_9, AFIO_MODE_DEFAULT);
//	
//	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_14, GPIO_DIR_IN);
//  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_IN); 
//	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_12, GPIO_DIR_IN);
//  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_13, GPIO_DIR_IN);
//	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10, GPIO_DIR_OUT);
//  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_11, GPIO_DIR_OUT); 
//	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_8, GPIO_DIR_OUT);
//  GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_9, GPIO_DIR_OUT);
//	
//	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_14, GPIO_PR_UP);
//  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);
//  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_12, GPIO_PR_UP);
//	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_13, GPIO_PR_UP);
//	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_14, ENABLE);
//  GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);
//	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_12, ENABLE);
//  GPIO_InputConfig(HT_GPIOA, GPIO_PIN_13, ENABLE);
	
	/*测试程序是否运行*/
	/*AFIO_GPxConfig(GPIO_PD, AFIO_PIN_4, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_OUT);
		AFIO_GPxConfig(GPIO_PD, AFIO_PIN_5, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_5, GPIO_DIR_OUT);
		AFIO_GPxConfig(GPIO_PD, AFIO_PIN_6, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_6, GPIO_DIR_OUT);*/
//	AFIO_GPxConfig(GPIO_PD, AFIO_PIN_7, AFIO_MODE_DEFAULT);
//	GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_7, GPIO_DIR_OUT);
}



void infrared_init(void)   //红外对管IO初始化
{
	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_11|AFIO_PIN_12|AFIO_PIN_15, AFIO_MODE_DEFAULT);  // AFIO_PIN_9|AFIO_PIN_10|
//  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_12, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_10, AFIO_MODE_DEFAULT);
//  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_11, AFIO_MODE_DEFAULT);
//	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_15, AFIO_MODE_DEFAULT);
	
	GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_DIR_IN); // GPIO_PIN_9|GPIO_PIN_10|
//	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_12, GPIO_DIR_IN);
//  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_10, GPIO_DIR_IN);
//  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_11, GPIO_DIR_IN); 

	
	GPIO_PullResistorConfig(HT_GPIOC,GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PR_UP);  // GPIO_PIN_9|GPIO_PIN_10|
//  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_12, GPIO_PR_UP);
//	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_10, GPIO_PR_UP);
//	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_11, GPIO_PR_UP);
	
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, ENABLE);   //使能输入端口  //GPIO_PIN_9|GPIO_PIN_10|
//	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_12, ENABLE);
//	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_10, ENABLE);
//	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_11, ENABLE);
}

void other_gpio(void)
{
	AFIO_GPxConfig(GPIO_PE, AFIO_PIN_8, AFIO_MODE_DEFAULT);   //PEB检测是否在充电
	GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_8, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOE, GPIO_PIN_8, GPIO_PR_UP);
	GPIO_InputConfig(HT_GPIOE, GPIO_PIN_8, ENABLE);
}


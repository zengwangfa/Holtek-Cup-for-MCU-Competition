#include "ADC.h"

u16 adc_var;
float adc_value[10] = {0};
float adc_real = 0;

void Adc_Init(void)    //PA0
{
	/* ADCLK frequency is set to 72/64 MHz = 1.125MHz                                                         */
  CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);

  /* Config AFIO mode as ADC function                                                                       */
  AFIO_GPAConfig(AFIO_PIN_0 , AFIO_MODE_2);   //PA0
 
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

float Bubble(float *v)     
{
	u8 i,j;
	float k,temp;
	for(j=0;j<10;j++)
	{			
		for(i=0;i<9-j;i++)
		{
				if(v[i]>v[i+1])
				{
					temp=v[i];
					v[i]=v[i+1];
					v[i+1]=temp;
				}
		}
	}
	k=(v[4]+v[5]+v[6])/3;  //取中间三个值平均
	return k;
	
}


float Get_AD_Value()
{
	u8 i;

	for(i=0;i<10;i++)
	{
		adc_var = ((HT_ADC->DR[0]&0x0FFF));
		adc_value[i]=(float)adc_var*(3.3/4096) ;   //temp=(float)adcx*(3.3/4096);
	}
	
	adc_real = Bubble(adc_value);
	return adc_real;
}




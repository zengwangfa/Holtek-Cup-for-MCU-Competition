#include "delay.h"
#include "led.h"
#include "ht32.h"

#define NVIC_PRIORITY_GROUP_3          3 /* Preemption: 4 bits / Subpriority: 0 bits */

int main(void)
{
	u8 i,j,k;
	delay_init();          //系统延时初始化
	NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP_3);
	LED_Init();
	
	while(1)
	{
		delay_ms(10);
		i++;j++;k++;
		if(i==10)
		{
			i = 0;
			HT32F_DVB_LEDToggle(HT_LED1);
		}
		if(j==20)
		{
			j = 0;
			HT32F_DVB_LEDToggle(HT_LED2);
		}
		if(k==50)
		{
			k = 0;
			HT32F_DVB_LEDToggle(HT_LED3);
		}
	}
	return 0;
}


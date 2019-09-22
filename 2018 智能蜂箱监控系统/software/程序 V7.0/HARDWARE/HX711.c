//------------------HX711.c---------------------//
#include "HX711.h"
u32 i,j,k,x,y;

u8 Weight1_Ten_Cmd[]    ={0x74,0x31,0x39,0x2E,0x74,0x78,0x74,0x3D,0x22,/*重量十位*/0x31,/*重量个位*/0x31,0x22,0xff,0xff,0xff};//  31 31   ASC值实际为1 1℃  
u8 Weight1_decimal_Cmd[]={0x74,0x32,0x30,0x2E,0x74,0x78,0x74,0x3D,0x22,/*重量小数*/0x31,0x22,0xff,0xff,0xff};  //所以每一位需要加+30

u8 Weight2_Ten_Cmd[]    ={0x74,0x32,0x31,0x2E,0x74,0x78,0x74,0x3D,0x22,/*重量十位*/0x31,/*重量个位*/0x31,0x22,0xff,0xff,0xff};//  31 31   ASC值实际为1 1℃  
u8 Weight2_decimal_Cmd[]={0x74,0x32,0x32,0x2E,0x74,0x78,0x74,0x3D,0x22,/*重量小数*/0x31,0x22,0xff,0xff,0xff};  //所以每一位需要加+30

void HX711_Init1(void)
{
	
	AFIO_GPxConfig(HX711_AFIO_PORT, HX711_DOUT1_AFIO_PIN, AFIO_MODE_DEFAULT);		 //DAT(C0)
	GPIO_DirectionConfig(HX711_GPIO_PORT, HX711_DOUT1_GPIO_PIN, GPIO_DIR_IN);	 //输入方向
	GPIO_PullResistorConfig(HX711_GPIO_PORT, HX711_DOUT1_GPIO_PIN, GPIO_PR_UP); //上拉输入
  GPIO_InputConfig(HX711_GPIO_PORT, HX711_DOUT1_GPIO_PIN, ENABLE);  		       //使能输入
	
	AFIO_GPxConfig(HX711_AFIO_PORT, HX711_SCK1_AFIO_PIN, AFIO_MODE_DEFAULT);   //SCK(C1)
	GPIO_DirectionConfig(HX711_GPIO_PORT, HX711_SCK1_GPIO_PIN, GPIO_DIR_OUT);//输出方向
	GPIO_OpenDrainConfig(HX711_GPIO_PORT,HX711_SCK1_GPIO_PIN,DISABLE);				//推挽输出
	GPIO_WriteOutBits(HX711_GPIO_PORT, HX711_SCK1_GPIO_PIN, SET);        		//PA7->1
}

void HX711_Init2(void)
{
	
	AFIO_GPxConfig(HX711_AFIO_PORT, HX711_DOUT2_AFIO_PIN, AFIO_MODE_DEFAULT);		 //DAT(C0)
	GPIO_DirectionConfig(HX711_GPIO_PORT, HX711_DOUT2_GPIO_PIN, GPIO_DIR_IN);	 //输入方向
	GPIO_PullResistorConfig(HX711_GPIO_PORT, HX711_DOUT2_GPIO_PIN, GPIO_PR_UP); //上拉输入
  GPIO_InputConfig(HX711_GPIO_PORT, HX711_DOUT2_GPIO_PIN, ENABLE);  		       //使能输入
	
	AFIO_GPxConfig(HX711_AFIO_PORT, HX711_SCK2_AFIO_PIN, AFIO_MODE_DEFAULT);   //SCK(C1)
	GPIO_DirectionConfig(HX711_GPIO_PORT, HX711_SCK2_GPIO_PIN, GPIO_DIR_OUT);//输出方向
	GPIO_OpenDrainConfig(HX711_GPIO_PORT,HX711_SCK2_GPIO_PIN,DISABLE);				//推挽输出
	GPIO_WriteOutBits(HX711_GPIO_PORT, HX711_SCK2_GPIO_PIN, SET);        		//PA7->1
}


unsigned long HX711_Read1()
{
	
	unsigned long Count;
  unsigned char i;

	ADSK1 = 0;                 //使能AD(PD_SCK 拉低)
  Count= 0; 
  while(ADDO1);              //AD转换未结束则等待,否则开始读取

  for(i=0;i<24;i++)
   {
      ADSK1 = 1;           //PD_SCK 置高(发送脉冲)
      Count=Count<<1;     //下降沿来临时变量Count左移一位,右则补零
      ADSK1 = 0;            //PD_SCK 拉低
      if(ADDO1) 
			Count++;
   }
  ADSK1=1;
  Count=Count^0x800000;   //第25个脉冲下降沿来时,转换数据完成
  ADSK1=0;
  return(Count);
		 
}


unsigned long HX711_Read2()
{
	
	unsigned long Count;
  unsigned char i;

	ADSK2 = 0;                 //使能AD(PD_SCK 拉低)
  Count= 0; 
  while(ADDO2);              //AD转换未结束则等待,否则开始读取

  for(i=0;i<24;i++)
   {
      ADSK2 = 1;           //PD_SCK 置高(发送脉冲)
      Count=Count<<1;     //下降沿来临时变量Count左移一位,右则补零
      ADSK2 = 0;            //PD_SCK 拉低
      if(ADDO2) 
			Count++;
   }
  ADSK2=1;
  Count=Count^0x800000;   //第25个脉冲下降沿来时,转换数据完成
  ADSK2=0;
  return(Count);
		 
}

u8 *Replace_Weight_Array(u8 array[],u8 num1,u8 num2)//更新重量值
{
  array[9] = num1;
	array[10] =num2;
  return array;
}

u8 BCD2Val(u8 x)   //BCD转十进制
{
return ((x>>4)&0x0f)*10+(x&0x0f);
} 


void UART_SEND_Weight()
{
	
	  i = HX711_Read1();       //重量采集
		j = (i-10198500)/4200;   //(i-10225500)/4300;
		k = ((j-2.72)/2.3285);
		if(j<=0)k=0;
		//k = k*100;
	
	
	  x =  k/5;
	  y = (k/5)*10;
		delay_ms(50);
		//USART_Sends(HT_USART1,Replace_Weight_Array(Weight1_Ten_Cmd,(0)+0x30,(0)+0x30),15);//发送正数部分
		//delay_ms(50);
		USART_Sends(HT_USART1,Replace_Weight_Array(Weight1_decimal_Cmd,((x/10))+0x30,  0x22),14);//发送小数部分
		delay_ms(50);
}


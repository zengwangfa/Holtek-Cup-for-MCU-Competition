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
#include "voice.h"


#define check_bit2 0x00


float distance = 0;     //超声波检测的距离

unsigned char time_count=0;   ////电压检测时间计时
u8 send_dis=0;
u8 play_music_flag=0;  //音乐播放标志位
//u8 music_busy=0;

extern u8 status_flag;  //机器人状态标志位

extern unsigned char  data;
extern int val_err_last;
extern int err;
extern int val_err_L;
extern int val_err_R;
extern int turn_mount;
unsigned char turn_flag = 0;
unsigned char OLED_flag = 1;
unsigned char Fre_div_val = 10;
unsigned char Data_value = 0;
unsigned char stop_flag = 0;

extern int cover_time;   //遮挡时间
unsigned char cur_landmark=0;  //弯道地标
unsigned char current_state2[7]={0xaa,0x55,0x04,0x00,rob_number,0xff,check_bit2};
 
unsigned char  data2[4]  = {0x07,0x07,0x07,0x07};
unsigned char  DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  //默认密码
unsigned char  g_ucTempbuf[20];    
unsigned char  MLastSelectedSnr[4]={1,1,1,1};                         //序列号数组
unsigned char  RevBuffer[30]={0};  
unsigned char  CardSendBuffer[16]={0x01, 0xff, 0x22, 0x2, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0xff, 0xff, 0x01};  //要发送的数据
unsigned char  CardRecBuffer[16]={0},cardrec2[16]={0},CardRecBuffer2[16]={0};   //卡接收的数据
unsigned char  atq[2]={0xff,0xff}; 
unsigned char	 ControlBlockAddr = 7;    //卡控制模块地址
unsigned char	 DataBlockAddr = 4;       //卡读写数据块的地址

u16 gPotentiometerLevel;
float adc_value=0;


unsigned char hhh=1;

void RFID_read(void);  //红外对管检测
void usart_status1(unsigned char np);//发送状态函数1

int main(void)
{ 
	System_Init();  //系统时钟初始化
  pwm1_init();  //PE3
	pwm2_init();  //PA6
	
	adc_init();  //电压采集  PA0
	
	usart0_init();   //用于WiFi通讯，PA2TX， PA3 RX
	usart1_init();   //用于声音播放，PA4TX， PA5 RX
	
	INPUT_Line_Init();  //磁条检测IO初始化
	DIR_Init();    //电机使能与方向IO初始化
	infrared_init();  //红外对管检测
	
	/********  定时器0中断初始化 *********/
	NVIC_EnableIRQ(BFTM0_IRQn);     //定时器中断0使能
	BFTM_SetCompare(HT_BFTM0, SystemCoreClock/1000 * 5);//10ms定时中断
  BFTM_SetCounter(HT_BFTM0, 0);     
  BFTM_IntConfig(HT_BFTM0, ENABLE);//ENABLE为1
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
	
	delay_init();  //延时函数初始化
	
	RC522_Control_IO_Init();      //REID控制引脚初始化
	PcdReset();
	M500PcdConfigISOType('A');
	
	chose_music(2,1);   //播放开机欢迎词
	
		/*******  初始化电机状态  **********/
	ena1_status = 1;    //为 1 时电机使能  为0时不使能
	dir1_status = 0;    //为 0 时正转      为1时反转
	ena2_status = 1;
	dir2_status = 0;
	//chose_music(1,3);
	
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_9|AFIO_PIN_10, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_9|GPIO_PIN_10, GPIO_DIR_OUT);
	
	//RfidCardWrite(DataBlockAddr,CardSendBuffer); 
  while (1)
  { 
		if(((status_flag==1)||(status_flag==3))&&(play_music_flag==1)&&(busy_music==0)) //busy_music==1时，正在播放，为0时没有播放
		{
			play_music_flag=0;
		  chose_music(1,1);
		}
		if(time_count>60)   //一分钟检测一次电量
		{
			time_count = 0;
			gPotentiometerLevel = ((HT_ADC->DR[0]&0x0FFF));
		  adc_value=(float)gPotentiometerLevel*(3.3/4096) ;//temp=(float)adcx*(3.3/4096);
		}
		if(charging_status==1)//正在充电 （PE8）
		{}
		else //没有充电
		{}

		RFID_read();        //RFID读卡
		
	  IRLED();           //红外对管
		test_led1=!test_led1;
		test_led2=!test_led2;
	}
}


void RFID_read(void)   //RFID读卡
{
		if(RfidCardRead(DataBlockAddr,CardRecBuffer) == MI_OK)
		{
				if((CardRecBuffer[0]==0x01)&&(CardRecBuffer[1]==0xff))  //RFID读卡
				{
					cur_landmark  = 1;    //弯道地标
				}
				if((CardRecBuffer[0]==0xff)&&(CardRecBuffer[1]==0xff))  //RFID读卡
				{
					usart_status1(0xff);   //发送距离信息
				}
				led4=0;
		}
		else
		{
			unsigned char clear_flag=0;
			if((CardRecBuffer[0] != 0x00)&&(CardRecBuffer[1] != 0x00)&&(CardRecBuffer[2] != 0x00)&&(CardRecBuffer[3] != 0x00))
			{
				for(clear_flag=0;clear_flag<16;clear_flag++)    //清除RFID数据缓冲区
				  CardRecBuffer[clear_flag]=0x00;
			}
			send_dis=0;
			led4=1; 
		}
		led1=!led1;
}

void IRLED(void)   //红外对管检测及超声波距离判断
{
		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_11) == 0) ///////////////////////////////////---------------------------
		{
			if(stop_flag == 0)
			{
				insert_filemusic(2,2);  //插播提示音乐
			}
			stop_flag = 1;
		}
			
		else  
		{
			stop_flag = 0;
			cover_time = 0;
		}
}

void usart_status1(unsigned char np)   //状态发送函数，输入参数为当前状态
{
	if(send_dis == 0)
	{
	  	unsigned char mp,n=0;
		  mp=((0xaa+0x55+0x04+0x00+rob_number+np)&(0xff));
		  for(n=0;n<7;n++)   //发送当前状态
		 {
			  current_state2[5] = np;
			  current_state2[6] = mp;
			  USART_SendData(HT_USART0,current_state2[n]);
		 }
			send_dis =1;
   }
}






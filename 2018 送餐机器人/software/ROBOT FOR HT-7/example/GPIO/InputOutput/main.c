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


float distance = 0;     //���������ľ���

unsigned char time_count=0;   ////��ѹ���ʱ���ʱ
u8 send_dis=0;
u8 play_music_flag=0;  //���ֲ��ű�־λ
//u8 music_busy=0;

extern u8 status_flag;  //������״̬��־λ

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

extern int cover_time;   //�ڵ�ʱ��
unsigned char cur_landmark=0;  //����ر�
unsigned char current_state2[7]={0xaa,0x55,0x04,0x00,rob_number,0xff,check_bit2};
 
unsigned char  data2[4]  = {0x07,0x07,0x07,0x07};
unsigned char  DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  //Ĭ������
unsigned char  g_ucTempbuf[20];    
unsigned char  MLastSelectedSnr[4]={1,1,1,1};                         //���к�����
unsigned char  RevBuffer[30]={0};  
unsigned char  CardSendBuffer[16]={0x01, 0xff, 0x22, 0x2, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0xff, 0xff, 0x01};  //Ҫ���͵�����
unsigned char  CardRecBuffer[16]={0},cardrec2[16]={0},CardRecBuffer2[16]={0};   //�����յ�����
unsigned char  atq[2]={0xff,0xff}; 
unsigned char	 ControlBlockAddr = 7;    //������ģ���ַ
unsigned char	 DataBlockAddr = 4;       //����д���ݿ�ĵ�ַ

u16 gPotentiometerLevel;
float adc_value=0;


unsigned char hhh=1;

void RFID_read(void);  //����Թܼ��
void usart_status1(unsigned char np);//����״̬����1

int main(void)
{ 
	System_Init();  //ϵͳʱ�ӳ�ʼ��
  pwm1_init();  //PE3
	pwm2_init();  //PA6
	
	adc_init();  //��ѹ�ɼ�  PA0
	
	usart0_init();   //����WiFiͨѶ��PA2TX�� PA3 RX
	usart1_init();   //�����������ţ�PA4TX�� PA5 RX
	
	INPUT_Line_Init();  //�������IO��ʼ��
	DIR_Init();    //���ʹ���뷽��IO��ʼ��
	infrared_init();  //����Թܼ��
	
	/********  ��ʱ��0�жϳ�ʼ�� *********/
	NVIC_EnableIRQ(BFTM0_IRQn);     //��ʱ���ж�0ʹ��
	BFTM_SetCompare(HT_BFTM0, SystemCoreClock/1000 * 5);//10ms��ʱ�ж�
  BFTM_SetCounter(HT_BFTM0, 0);     
  BFTM_IntConfig(HT_BFTM0, ENABLE);//ENABLEΪ1
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
	
	delay_init();  //��ʱ������ʼ��
	
	RC522_Control_IO_Init();      //REID�������ų�ʼ��
	PcdReset();
	M500PcdConfigISOType('A');
	
	chose_music(2,1);   //���ſ�����ӭ��
	
		/*******  ��ʼ�����״̬  **********/
	ena1_status = 1;    //Ϊ 1 ʱ���ʹ��  Ϊ0ʱ��ʹ��
	dir1_status = 0;    //Ϊ 0 ʱ��ת      Ϊ1ʱ��ת
	ena2_status = 1;
	dir2_status = 0;
	//chose_music(1,3);
	
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_9|AFIO_PIN_10, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_9|GPIO_PIN_10, GPIO_DIR_OUT);
	
	//RfidCardWrite(DataBlockAddr,CardSendBuffer); 
  while (1)
  { 
		if(((status_flag==1)||(status_flag==3))&&(play_music_flag==1)&&(busy_music==0)) //busy_music==1ʱ�����ڲ��ţ�Ϊ0ʱû�в���
		{
			play_music_flag=0;
		  chose_music(1,1);
		}
		if(time_count>60)   //һ���Ӽ��һ�ε���
		{
			time_count = 0;
			gPotentiometerLevel = ((HT_ADC->DR[0]&0x0FFF));
		  adc_value=(float)gPotentiometerLevel*(3.3/4096) ;//temp=(float)adcx*(3.3/4096);
		}
		if(charging_status==1)//���ڳ�� ��PE8��
		{}
		else //û�г��
		{}

		RFID_read();        //RFID����
		
	  IRLED();           //����Թ�
		test_led1=!test_led1;
		test_led2=!test_led2;
	}
}


void RFID_read(void)   //RFID����
{
		if(RfidCardRead(DataBlockAddr,CardRecBuffer) == MI_OK)
		{
				if((CardRecBuffer[0]==0x01)&&(CardRecBuffer[1]==0xff))  //RFID����
				{
					cur_landmark  = 1;    //����ر�
				}
				if((CardRecBuffer[0]==0xff)&&(CardRecBuffer[1]==0xff))  //RFID����
				{
					usart_status1(0xff);   //���;�����Ϣ
				}
				led4=0;
		}
		else
		{
			unsigned char clear_flag=0;
			if((CardRecBuffer[0] != 0x00)&&(CardRecBuffer[1] != 0x00)&&(CardRecBuffer[2] != 0x00)&&(CardRecBuffer[3] != 0x00))
			{
				for(clear_flag=0;clear_flag<16;clear_flag++)    //���RFID���ݻ�����
				  CardRecBuffer[clear_flag]=0x00;
			}
			send_dis=0;
			led4=1; 
		}
		led1=!led1;
}

void IRLED(void)   //����Թܼ�⼰�����������ж�
{
		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_11) == 0) ///////////////////////////////////---------------------------
		{
			if(stop_flag == 0)
			{
				insert_filemusic(2,2);  //�岥��ʾ����
			}
			stop_flag = 1;
		}
			
		else  
		{
			stop_flag = 0;
			cover_time = 0;
		}
}

void usart_status1(unsigned char np)   //״̬���ͺ������������Ϊ��ǰ״̬
{
	if(send_dis == 0)
	{
	  	unsigned char mp,n=0;
		  mp=((0xaa+0x55+0x04+0x00+rob_number+np)&(0xff));
		  for(n=0;n<7;n++)   //���͵�ǰ״̬
		 {
			  current_state2[5] = np;
			  current_state2[6] = mp;
			  USART_SendData(HT_USART0,current_state2[n]);
		 }
			send_dis =1;
   }
}







/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "config.h"
#include "ht32_board.h"
#include "usart_int.h"
#include "delay.h"
#include "controll_main.h"
#include "voice.h"
#include "fuzzy.h"



#define start_dodge 0x01            //��ʼ����
#define complete_dodge_back 0x02    //��ɱ��ò�����
#define dodge_direct_right 0x01     //���ұ���
#define dodge_direct_left  0x02			//�������




#define what_status 0x00
#define check_bit 0x00
#define waiting_send 0x01                //�ȴ��Ͳ�
#define Room_service_route 0x02          //���Ͳ�·��
#define Arrive_designated_table 0x03     //����ָ������
#define back_to_wait_road 0x04           //�ڷ��صȴ�����·��
#define Low_electricity 0x06             //��������
#define covered_long_time 0x07           //����ʱ�䵲ס
#define dodge_ok 0x08                    //�ѱܿ�
#define back_line 0x09                   //���ú��ֻص�����·��
#define charge_area 0x0a                 //�ڳ������
#define Turn_around_finish   0x0d        //ת�����

u8 status_flag=0;  //״̬��־λ
u8 look_charge=0;

u32 pwm1_truevalue=0;
u32 pwm2_truevalue=0;

int pwm1_value = 3;
int pwm2_value = 3;
int val_err_L = 0;
int val_err = 0;
int val_err_R = 0;
int turn_mount = 0;			//������Ƶ��
int waiting_time = 1000;    //���ڵ���ʱ��  5s
int back_flag = 0;          //���ر�־λ
int timer_5ms_count = 0;   //�жϼ�ʱ��־
int cover_time = 0;   //�ڵ�ʱ��
int turn_time = 0;    //ת��ʱ��ʱ

float fuzzy_value;  //ģ���㷨�õ���ֵ
float err1;   //ƫ��
float err1_last;   //ƫ��
float err_ec;   //ƫ��仯��

unsigned char task_ok = 0;   //������ɱ�־

unsigned char USART_send_flag = 0;
unsigned char turn_back_complete_flag = 0;
unsigned char complete_dodge_back_flag = 0;   //������ɲ����ر�־
unsigned char complete_dodge_flag=0;

extern uint32_t wCRR;
extern u8 play_music_flag;

extern int err;
extern unsigned char turn_flag;
extern unsigned char stop_flag;
extern unsigned char Fre_div_val;
unsigned char turn_f = 0;
uint16_t PSCReloadMod = 5;   //��Ƶ��
unsigned char answer = 1;
unsigned char check_bit1 = 0;


//void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode)
													// PrescalerΪ10��ʱ����1kHz
												  //Ϊ1��ʱ����5.2kHz
													//Ϊ5ʱΪ 1.8kHz
													//Ϊ6ʱΪ 1.4kHz
													//Ϊ 11 ʱ  800Hz



unsigned char current_state[7]={0xaa,0x55,0x04,0x00,rob_number,what_status,check_bit};  //���ص�״̬
//check_bit1=((0xaa+0x55+0x04+0x00+rob_number+0x03)&&(0xff));
extern unsigned char  CardRecBuffer[16],cardrec2[16],CardRecBuffer2[16];   //�����յ�����

extern unsigned char  data;   //IO�ڼ�⵽������
extern unsigned char time_count;  //��ѹ���ʱ���ʱ
unsigned char time5ms=0;
extern unsigned char IO_data;

extern unsigned char recv_buffer[(MAX_DATA_LENS+4)];  //����1�������ݻ������,
extern unsigned char lastrecv_buffer[(MAX_DATA_LENS+4)];
extern unsigned char recv1_data_ok;                //���յ���ȷ�����ݰ�
extern unsigned char recv1_data_ok1;
extern unsigned char food_buffer[10];
extern unsigned char recv1_data_p;
extern unsigned char curve_number;   //�������
extern unsigned char walk_route[(MAX_DATA_LENS+4)];    //·��

unsigned char number_x = 0;    //�ڼ������

extern unsigned char cur_landmark;   //RFID��⵽0x01��˵����������ڸ���·��ת�䡣

/********************** �������� ****************************/
int my_abs(int a);
void control_line(void);
void stop(void);
void dodge(unsigned char do_status, unsigned char direct);
void Start_serving(void);
void turn_RI(unsigned char RI);
void turn_back(void);
void usart_status(unsigned char np);  //npΪ״̬
void backwait(void);       //�ص��ȴ���
void charging(void);    //ȥ���
void status_res(void);    //״̬�ظ�
/*************************************************************/



int my_abs(int a)
{
  int b=a>0.0?a:-a;
  return b;
}



void BFTM0_IRQHandler(void)   //��ʱ��0�ж�
{

	BFTM_ClearFlag(HT_BFTM0);
  
	time5ms++;
	
//	ena1_status = 1;    //Ϊ 1 ʱ���ʹ��  Ϊ0ʱ��ʹ��
//	dir1_status = 0;    //Ϊ 0 ʱ��ת      Ϊ1ʱ��ת
//	ena2_status = 1;
//	dir2_status = 0;
//	TIM_PrescalerConfig(TIM1, 3, PSCReloadMod);
//  TIM_PrescalerConfig(TIM2, 3, PSCReloadMod);
//	
	//control_line();
	
	if(time5ms==200)  //1s
	{
		LED2=!LED2;
		time_count++;
	}
	if(stop_flag == 1)    //��⵽�ϰ���
  {
			 cover_time ++;   //����ס��ʱ��
			 stop();  //С��ֹͣ
			 if(cover_time >= waiting_time )  //���ͱ���ʱ���ڵ���״̬
			 {
				 cover_time = 0;
				 usart_status(covered_long_time);
			 }
  }
	else
	{
			step_control();   //�������
			if(recv1_data_ok==1)
			{
				turn_time = 0;
				timer_5ms_count = 0;
				back_flag = 0;
				recv1_data_ok = 0;
				USART_send_flag = 0;
				play_music_flag = 0;
			}
			if(task_ok == 1)  //ÿ��������ɣ�����������ݣ�
			{
				unsigned char clear_flag=0;
				for(clear_flag=0;clear_flag<curve_number+12;clear_flag++)
					recv_buffer[clear_flag]=0x00;
				task_ok = 0;
			}
			
			if((rob_number == recv_buffer[4])&&(recv1_data_ok1==1) )  //���ҵ���Ӧ������ִ������
			{
				switch(recv_buffer[5])
				{
					case Food_delivery:      Start_serving() ;  break;    //�Ͳ� 0x01
					case back_to_wait:       backwait() ;       break;    //���صȴ���  0x02
					case status_inquiry:     status_res();      break;    //״̬��ѯ 0x03
					case go_charge:          charging() ;       break;    //ȥ���   0x04
					case dodge_command:      dodge(recv_buffer[6],recv_buffer[7]);break;  //����ָ��  0x05
					case turnback:           turn_back();       break;    //ת������   0x06           
					case Unconditional_stop: stop();            break;    //������ͣ������  x08
					default:{stop();status_flag =0;} break;
				}
			}
			else //δ���յ��κ�����
			{
			   stop();
			}
			
	}


}



void control_line(void)
{
	step_control();
	val_err = my_abs(err);
	val_err_L = val_err + 10;
	val_err_R = val_err;
	
	err1=(float)IO_data;
	err_ec=err1-err1_last;
	fuzzy_value = Fuzzy(&err1,&err_ec);
	err1_last=err1;
	
  if(cur_landmark == 1)  //�������
	{
		turn_RI(walk_route[number_x]);
	}
	else if(err > 1)  //  ��ת
	{
			//if(val_err == 6)
		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_0)==1) //ֻҪ��һλ��⵽
			{
				dir1_status = 1; 
				dir2_status = 0;
				ena1_status = 1;
				ena2_status = 1;
				pwm1_value = 4;
				pwm2_value = 4;
			}
			else if(val_err == 5)
			{
				dir1_status = 0; 
				dir2_status = 0;
				ena1_status = 0;
				ena2_status = 1;
				pwm1_value = 10000;
				pwm2_value = 2;
			}
			else if(val_err == 4)
			{
				dir1_status = 0; 
				dir2_status = 0;
				ena1_status = 1;
				ena2_status = 1;
				pwm1_value = 100;
				pwm2_value = 2;
			}
			
			else
			{
				dir1_status = 0; 
				dir2_status = 0;
				ena1_status = 1;
				ena2_status = 1;
				pwm1_value = val_err_L;
				pwm2_value = 3;
			}
	}
	
	else if(err < -1)  //  ��ת
	{
			//if(val_err == 6)
			if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_7)==1)  //ֻҪ�ڰ�λ��⵽
			{
				dir1_status = 0; 
				dir2_status = 1;
				ena1_status = 1;
				ena2_status = 1;
				pwm1_value = 3;
				pwm2_value = 3;
			}
			else if(val_err == 5)
			{
				dir1_status = 0; 
				dir2_status = 0;
				ena1_status = 1;
				ena2_status = 0;
				pwm1_value = 2;
				pwm2_value = 10000;
			}
			else if(val_err == 4)
			{
				dir1_status = 0; 
				dir2_status = 0;
				ena1_status = 1;
				ena2_status = 1;
				pwm1_value = 2;
				pwm2_value = 100;
			}
			else
			{
				dir1_status = 0; 
				dir2_status = 0;
				ena1_status = 1;
				ena2_status = 1;
				pwm1_value = 4;
				pwm2_value = val_err_R;
			}
	}
	else if (err == 1)     //ֱ��
	{
		ena1_status = 1;
		ena2_status = 1;
		dir1_status = 0;
		dir2_status = 0;
		pwm1_value = 3;
		pwm2_value = 3;
	}
	else   //δ��⵽   ���ϴε�ֵ����
	{
			ena1_status = ena1_status;
			ena2_status = ena2_status;
			dir1_status = dir1_status;
			dir2_status = dir2_status;
			pwm1_value = pwm1_value;
			pwm2_value = pwm2_value;
	}
  pwm1_truevalue=wCRR+pwm1_value*200;
  pwm2_truevalue=wCRR+pwm2_value*100;
//  pwm1_truevalue=fuzzy_value*100 ;
//	pwm2_truevalue=(118-fuzzy_value)*100;
  MCTM_SetCounterReload(HT_MCTM1, pwm1_truevalue);
	GPTM_SetCounterReload(HT_GPTM0, pwm2_truevalue);
} 


void status_res(void)   //״̬
{
	unsigned char lr = 0;
	if(status_flag == 4)
	{
		usart_status(waiting_send);  //�ȴ��Ͳ�
	}
	else if(status_flag == 1)
	{
		usart_status(Room_service_route);  //���Ͳ�·��
	}
	else if(status_flag == 2)   //����ָ��λ��   ֹͣ
	{
	  usart_status(Arrive_designated_table);  //����ָ������
	}
	else if(status_flag == 3)
	{
		usart_status(back_to_wait_road);  //�ڷ��صȴ�����·��
	}
	else if(status_flag == 10)
	{
		usart_status(Low_electricity);  //��������
	}
	else if(stop_flag == 1)
	{
		usart_status(covered_long_time);  //����ס
	}
	else if(status_flag == 10)
	{
		usart_status(charge_area);  //�ڳ������
	}
	for(lr=0;lr<20;lr++)              //״̬����������ִ���ϴ�����
	  recv_buffer[lr]=lastrecv_buffer[lr];
	USART_send_flag = 0;
}	



void Start_serving(void)    //ִ���Ͳ�ָ��
{
		if((CardRecBuffer[0]==recv_buffer[7])&&(CardRecBuffer[1]==0x00))   //����ָ��λ��   ֹͣ
		{
		  stop();
			usart_status(Arrive_designated_table); //���͵������״̬
			if(status_flag==1)
			{
				insert_filemusic(2,3);
			}
			task_ok = 1;
			status_flag=2;  //����ָ���ͲͲ���
		}
		else 
		{
			control_line();
			status_flag=1;  //���Ͳ͵�·��
		}
}


void backwait(void)     //���صȴ���
{
  if((CardRecBuffer[0]==recv_buffer[7])&&(CardRecBuffer[1]==0xfe))   //����ָ��λ��   ֹͣ
		{
			status_flag=4;   //���Ͳ����ȴ�
		  stop();
			usart_status(waiting_send); //����״̬
			task_ok = 1;
		}
		else 
		{
			status_flag=3;  //�ڷ��صȴ��Ͳ�����·��
			control_line();
		}
}


void charging(void)    //ȥ���
{
	if((CardRecBuffer[0]==recv_buffer[7])&&(CardRecBuffer[1]==0xfd))   //����ָ�����λ��   ֹͣ
		{
			status_flag=6;   //����������
		  stop();
			usart_status(charge_area); //����״̬
			task_ok = 1;
		}
	else 
		{
			status_flag=5;    //ȥ������·��
			control_line();
		}  
		
	if(look_charge==1)  //Ѱ�ҳ���־λ
	{
	}
		
}

void stop(void)   //ͣ��
{
		ena1_status = 0;  //�����ʹ��
	//dir1_status = 1;
	  ena2_status = 0;
	//dir2_status = 1;
}

void turn_RI(unsigned char RI)   //������ת��
{
	turn_time++;
	if(turn_time >505) turn_time=505;
	if(turn_time>100)
	{
	if((turn_time>=100)&&(turn_time <= 450))  //ʱ��ԼΪ2s
	{
		if(RI == 0x01)  //��ת
		{
			ena1_status = 0;
			ena2_status = 1;
		}
		if(RI == 0x02)  //��ת
		{
			ena1_status = 1;
			ena2_status = 0;
		}
		if(RI == 0x03)   //ֱ��
		{
			ena1_status = 1;
			ena2_status = 1;
		}
		dir2_status = 0;    
	  dir1_status = 0; 
	  MCTM_SetCounterReload(HT_MCTM1, wCRR);
		GPTM_SetCounterReload(HT_GPTM0, wCRR);
	}
	else
	{
		if(RI == 0x01)  //��ת
		{
			ena1_status = 0;
			ena2_status = 1;
		}
		if(RI == 0x02)  //��ת
		{
			ena1_status = 1;
			ena2_status = 0;
		}
		if(RI == 0x03)   //ֱ��
		{
			ena1_status = 1;
			ena2_status = 1;
		}
		dir2_status = 0;    
	  dir1_status = 0; 
	  MCTM_SetCounterReload(HT_MCTM1, wCRR);
		GPTM_SetCounterReload(HT_GPTM0, wCRR);
		if((((data==0x30)||(data==0x0c)||(data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))&&(turn_time > 300)))  //ת����ɲ��Ҵ�������Ŵ����м�λ��
		{
			turn_back_complete_flag=1;
			number_x ++;
			cur_landmark=0;  //�����־λ����
			turn_time = 0;
			if(number_x == curve_number )
				number_x =0;
		}
	}
 }
	else
	{
		ena1_status = 1;
		ena2_status = 1;
		dir2_status = 0;    
	  dir1_status = 0; 
	  MCTM_SetCounterReload(HT_MCTM1, wCRR);
		GPTM_SetCounterReload(HT_GPTM0, wCRR);
	}
}

void turn_back(void)  //ԭ��ת��
{
	turn_time++;
	if(turn_time > 700) turn_time = 700;
	if(recv_buffer[7]==0x01)  //��ת
	{
		if(recv_buffer[6]==0x02)   //ת180��
		{
			if((turn_time <= 200))   //ʱ��ԼΪ500ms
			{
				ena1_status = 1;  //���ʹ��
				ena2_status = 1;
				dir2_status = 0;
				dir1_status = 1;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR);
			}
			else
			{
				if((((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))&&(turn_time > 200)))  //ת����ɲ��Ҵ�������Ŵ����м�λ��
				{
					usart_status(Turn_around_finish);
					stop();
					turn_back_complete_flag=1;
					task_ok = 1;
				}
				else
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
				}
			}
	  }
		else if(recv_buffer[6]==0x01)  //ת90��
		{
			if(turn_time>600)
			{
				usart_status(Turn_around_finish);
				stop();
				turn_back_complete_flag=1;
				task_ok = 1;
			}
			else
			{
				ena1_status = 1;  //���ʹ��
				ena2_status = 1;
				dir2_status = 0;
				dir1_status = 1;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR);
			}
		}
		else if(recv_buffer[6]==0x03)   //ʻ�ع��
		{
			if((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))  //ת����ɲ��Ҵ�������Ŵ����м�λ��
			{
				usart_status(Turn_around_finish);
				stop();
				turn_back_complete_flag=1;
				task_ok = 1;
			}
			else
			{
				ena1_status = 1;  //���ʹ��
				ena2_status = 1;
				dir2_status = 0;
				dir1_status = 1;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR);
			}
		}
	}
	if(recv_buffer[7]==0x02)  //��ת
	{
		if(recv_buffer[6]==0x02)  //ת180��
		{
			if((turn_time <= 200))   //ʱ��ԼΪ500ms
			{
				ena1_status = 1;  //���ʹ��
				ena2_status = 1;
				dir2_status = 1;     
				dir1_status = 0;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR);
			}
			else
			{
				if((((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))&&(turn_time > 200)))  //ת����ɲ��Ҵ�������Ŵ����м�λ��
				{
					usart_status(Turn_around_finish);
					stop();
					turn_back_complete_flag=1;
					task_ok = 1;
				}
				else
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
				}
			}
	  }
		else if(recv_buffer[6]==0x01)  //ת90��
		{
			if(turn_time>600)
			{
				usart_status(Turn_around_finish);
				stop();
			  turn_back_complete_flag=1;
				task_ok = 1;
			}
			else
			{
				ena1_status = 1;  //���ʹ��
				ena2_status = 1;
				dir2_status = 1;
				dir1_status = 0;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR); 
			}
		}
		else if(recv_buffer[6]==0x03)   //ʻ�ع��
		{
			if((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))  //ת����ɲ��Ҵ�������Ŵ����м�λ��
			{
				usart_status(Turn_around_finish);
				stop();
				turn_back_complete_flag=1;
				task_ok = 1;
			}
			else
			{
				ena1_status = 1;  //���ʹ��
				ena2_status = 1;
				dir2_status = 1;
				dir1_status = 0;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR);
			}
		}
		  
	}
}



/******************************************

���������recv_buffer[6]����ʼ����ɱ��ÿ���
         recv_buffer[7]�����÷������
*********************************************/
void dodge(unsigned char do_status, unsigned char direct)    //���ó�����ָ������ת90��������ǰ��һ���ľ���
{

	if(recv_buffer[5] == 0x05)   //ȷ��Ϊ����ָ��
	{
		if(do_status == start_dodge)  //��ʼ����  0x01
		{
			timer_5ms_count++;
	    if(timer_5ms_count >2100) timer_5ms_count=2100;
			
			if(direct == dodge_direct_left)   //����� 0x02
			{
				if(timer_5ms_count < 600) 				//���ʱ���ڣ�������ԭ��ת��
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
					dir2_status = 1;    //��ߵ����ӷ�ת  
					dir1_status = 0;    //�ұߵ�������ת
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else if((timer_5ms_count >= 600) && (timer_5ms_count < 2000))   //֮��ֱ����ʻһ�ξ���
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
					dir2_status = 0;    //��ߵ����ӷ�ת
					dir1_status = 0;    //�ұߵ�������ת
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else         //������ɣ�ֹͣ�ȴ�
				{
					stop();
				  usart_status(dodge_ok);
					complete_dodge_flag = 1;
				}
					
			}
			if(direct == dodge_direct_right )   //�ұ��� 0x01
			{
				if(timer_5ms_count < 600) 				//���ʱ���ڣ�������ԭ��ת��
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
					dir2_status = 0;    //��ߵ�������ת  
					dir1_status = 1;    //�ұߵ����ӷ�ת
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else if((timer_5ms_count >= 600) && (timer_5ms_count < 2000))   //֮��ֱ����ʻһ�ξ���
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
					dir2_status = 0;    
					dir1_status = 0;    
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				} 
				else         //������ɣ�ֹͣ�ȴ�
				{
					stop();
				  usart_status(dodge_ok);
					complete_dodge_flag = 1;
					task_ok = 1;
				}
			}
		}
		
		if(do_status == complete_dodge_back)  //��������˻�ԭ��,������ǰ�� 0x02
		{
			back_flag ++;
			if(back_flag >2100) back_flag=2100;
			if(direct == dodge_direct_left)   //���˻� 0x02
			{
				if(back_flag < 1500) 				
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
					dir2_status = 1;    //��ߵ����ӷ�ת   �˻�
					dir1_status = 1;    //�ұߵ����ӷ�ת
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else if((back_flag >= 1500) && (back_flag < 1700))  
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
					dir2_status = 0;    //��ߵ�������ת
					dir1_status = 1;    //�ұߵ����ӷ�ת
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
					
				}
				else     //������ɣ�����ǰ��
				{
					if((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))  //ת����ɲ��Ҵ�������Ŵ����м�λ��
					{
					  unsigned char j=0;
						complete_dodge_back_flag = 1;
						stop();
						usart_status(back_line);
						for(j=0;j<10;j++)
						recv_buffer[j]=food_buffer[j];
					}
					else
					{
						ena1_status = 1;  //���ʹ��
					  ena2_status = 1;
					}
					
				}
					
			}
			if(direct == dodge_direct_right)   //���˻� 0x01
			{
				if(back_flag < 1500) 				//���ʱ���ڣ�������ԭ��ת��
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
					dir2_status = 1;    //��ߵ����ӷ�ת   �˻�
					dir1_status = 1;    //�ұߵ����ӷ�ת
          MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else if((back_flag >= 1500) && (back_flag < 1700))   //֮��ֱ����ʻһ�ξ���
				{
					ena1_status = 1;  //���ʹ��
					ena2_status = 1;
					dir2_status = 1;    //��ߵ����ӷ�ת
					dir1_status = 0;    //�ұߵ�������ת
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else     //������ɣ�����ǰ��
				{
					//ena1_status = 1;  //���ʹ��
					//ena2_status = 1;
					if((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))  //ת����ɲ��Ҵ�������Ŵ����м�λ��
					{
						unsigned char j=0;
						complete_dodge_back_flag = 1;
						stop();
						usart_status(back_line);
						for(j=0;j<10;j++)
						recv_buffer[j]=food_buffer[j];
					}
					else
					{
						ena1_status = 1;  //���ʹ��
					  ena2_status = 1;
					}
				}
			}
		}
  }
}

void usart_status(unsigned char np)   //״̬���ͺ������������Ϊ��ǰ״̬
{
	if(USART_send_flag == 0)
	{
	  	unsigned char mp,n=0;
		  check_bit1=((0xaa+0x55+0x04+0x00+rob_number+np)&(0xff));
		  mp = check_bit1;
		  for(n=0;n<7;n++)   //���͵�ǰ״̬
		 {
			  current_state[5] = np;
			  current_state[6] = mp;
				USART_SendData(HT_USART0,current_state[n]);
		 }
			USART_send_flag =1;
   }
}









void SysTick_Handler(void) //��ʱ�ж�
{
  TimingDelay();
}

/*********************************************************************************************************//**
 * @brief   This function handles MCTM1 UP interrupt.
 * @retval  None
 ************************************************************************************************************/
//void MCTM1UP_IRQHandler(void)
//{
//  extern __IO uint32_t wMctmUpdateDownCounter;
//  MCTM_ClearFlag(BUZZER_TM, MCTM_INT_UEV1);
//  if (wMctmUpdateDownCounter)
//    wMctmUpdateDownCounter--;
//}

//void BFTM1_IRQHandler(void)  //��ʱ��1�ж�
//{
//	USART_SendData(HT_UART0,0x02);
//	  t++;
//	  HT32F_DVB_LEDToggle(HT_LED1);		//��ת��ƽ�ĺ���

//   	BFTM_ClearFlag(HT_BFTM1);
//  
//}









/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup GPIO_Examples GPIO
  * @{
  */

/** @addtogroup InputOutput
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
//void NMI_Handler(void)
//{
//}

///*********************************************************************************************************//**
// * @brief   This function handles Hard Fault exception.
// * @retval  None
// ************************************************************************************************************/
//void HardFault_Handler(void)
//{
//  while (1);
//}

///*********************************************************************************************************//**
// * @brief   This function handles Memory Manage exception.
// * @retval  None
// ************************************************************************************************************/
//void MemManage_Handler(void)
//{
//  while (1);
//}

///*********************************************************************************************************//**
// * @brief   This function handles Bus Fault exception.
// * @retval  None
// ************************************************************************************************************/
//void BusFault_Handler(void)
//{
//  while (1);
//}

///*********************************************************************************************************//**
// * @brief   This function handles Usage Fault exception.
// * @retval  None
// ************************************************************************************************************/
//void UsageFault_Handler(void)
//{
//  while (1);
//}

///*********************************************************************************************************//**
// * @brief   This function handles SVCall exception.
// * @retval  None
// ************************************************************************************************************/
//void SVC_Handler(void)
//{
//}

///*********************************************************************************************************//**
// * @brief   This function handles Debug Monitor exception.
// * @retval  None
// ************************************************************************************************************/
//void DebugMon_Handler(void)
//{
//}

///*********************************************************************************************************//**
// * @brief   This function handles PendSVC exception.
// * @retval  None
// ************************************************************************************************************/
//void PendSV_Handler(void)
//{
//}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "config.h"
#include "ht32_board.h"
#include "usart_int.h"
#include "delay.h"
#include "controll_main.h"
#include "voice.h"
#include "fuzzy.h"



#define start_dodge 0x01            //开始避让
#define complete_dodge_back 0x02    //完成避让并返回
#define dodge_direct_right 0x01     //向右避让
#define dodge_direct_left  0x02			//向左避让




#define what_status 0x00
#define check_bit 0x00
#define waiting_send 0x01                //等待送餐
#define Room_service_route 0x02          //在送餐路上
#define Arrive_designated_table 0x03     //到达指定餐桌
#define back_to_wait_road 0x04           //在返回等待区的路上
#define Low_electricity 0x06             //电量过低
#define covered_long_time 0x07           //被长时间挡住
#define dodge_ok 0x08                    //已避开
#define back_line 0x09                   //避让后又回到行走路线
#define charge_area 0x0a                 //在充电区域
#define Turn_around_finish   0x0d        //转向完成

u8 status_flag=0;  //状态标志位
u8 look_charge=0;

u32 pwm1_truevalue=0;
u32 pwm2_truevalue=0;

int pwm1_value = 3;
int pwm2_value = 3;
int val_err_L = 0;
int val_err = 0;
int val_err_R = 0;
int turn_mount = 0;			//电机打角频率
int waiting_time = 1000;    //被遮挡的时间  5s
int back_flag = 0;          //返回标志位
int timer_5ms_count = 0;   //中断计时标志
int cover_time = 0;   //遮挡时间
int turn_time = 0;    //转弯时计时

float fuzzy_value;  //模糊算法得到的值
float err1;   //偏差
float err1_last;   //偏差
float err_ec;   //偏差变化量

unsigned char task_ok = 0;   //任务完成标志

unsigned char USART_send_flag = 0;
unsigned char turn_back_complete_flag = 0;
unsigned char complete_dodge_back_flag = 0;   //避让完成并返回标志
unsigned char complete_dodge_flag=0;

extern uint32_t wCRR;
extern u8 play_music_flag;

extern int err;
extern unsigned char turn_flag;
extern unsigned char stop_flag;
extern unsigned char Fre_div_val;
unsigned char turn_f = 0;
uint16_t PSCReloadMod = 5;   //分频数
unsigned char answer = 1;
unsigned char check_bit1 = 0;


//void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode)
													// Prescaler为10的时候是1kHz
												  //为1的时候是5.2kHz
													//为5时为 1.8kHz
													//为6时为 1.4kHz
													//为 11 时  800Hz



unsigned char current_state[7]={0xaa,0x55,0x04,0x00,rob_number,what_status,check_bit};  //返回的状态
//check_bit1=((0xaa+0x55+0x04+0x00+rob_number+0x03)&&(0xff));
extern unsigned char  CardRecBuffer[16],cardrec2[16],CardRecBuffer2[16];   //卡接收的数据

extern unsigned char  data;   //IO口检测到的数据
extern unsigned char time_count;  //电压检测时间计时
unsigned char time5ms=0;
extern unsigned char IO_data;

extern unsigned char recv_buffer[(MAX_DATA_LENS+4)];  //串口1接收数据缓冲变量,
extern unsigned char lastrecv_buffer[(MAX_DATA_LENS+4)];
extern unsigned char recv1_data_ok;                //接收到正确的数据包
extern unsigned char recv1_data_ok1;
extern unsigned char food_buffer[10];
extern unsigned char recv1_data_p;
extern unsigned char curve_number;   //弯道数量
extern unsigned char walk_route[(MAX_DATA_LENS+4)];    //路径

unsigned char number_x = 0;    //第几个弯道

extern unsigned char cur_landmark;   //RFID检测到0x01则说明是弯道，在根据路径转弯。

/********************** 函数声明 ****************************/
int my_abs(int a);
void control_line(void);
void stop(void);
void dodge(unsigned char do_status, unsigned char direct);
void Start_serving(void);
void turn_RI(unsigned char RI);
void turn_back(void);
void usart_status(unsigned char np);  //np为状态
void backwait(void);       //回到等待区
void charging(void);    //去充电
void status_res(void);    //状态回复
/*************************************************************/



int my_abs(int a)
{
  int b=a>0.0?a:-a;
  return b;
}



void BFTM0_IRQHandler(void)   //定时器0中断
{

	BFTM_ClearFlag(HT_BFTM0);
  
	time5ms++;
	
//	ena1_status = 1;    //为 1 时电机使能  为0时不使能
//	dir1_status = 0;    //为 0 时正转      为1时反转
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
	if(stop_flag == 1)    //检测到障碍物
  {
			 cover_time ++;   //被挡住的时间
			 stop();  //小车停止
			 if(cover_time >= waiting_time )  //发送被长时间遮挡的状态
			 {
				 cover_time = 0;
				 usart_status(covered_long_time);
			 }
  }
	else
	{
			step_control();   //磁条检测
			if(recv1_data_ok==1)
			{
				turn_time = 0;
				timer_5ms_count = 0;
				back_flag = 0;
				recv1_data_ok = 0;
				USART_send_flag = 0;
				play_music_flag = 0;
			}
			if(task_ok == 1)  //每项任务完成，清除命令数据，
			{
				unsigned char clear_flag=0;
				for(clear_flag=0;clear_flag<curve_number+12;clear_flag++)
					recv_buffer[clear_flag]=0x00;
				task_ok = 0;
			}
			
			if((rob_number == recv_buffer[4])&&(recv1_data_ok1==1) )  //先找到对应机器人执行命令
			{
				switch(recv_buffer[5])
				{
					case Food_delivery:      Start_serving() ;  break;    //送餐 0x01
					case back_to_wait:       backwait() ;       break;    //返回等待区  0x02
					case status_inquiry:     status_res();      break;    //状态查询 0x03
					case go_charge:          charging() ;       break;    //去充电   0x04
					case dodge_command:      dodge(recv_buffer[6],recv_buffer[7]);break;  //避让指令  0x05
					case turnback:           turn_back();       break;    //转身命令   0x06           
					case Unconditional_stop: stop();            break;    //无条件停车命令  x08
					default:{stop();status_flag =0;} break;
				}
			}
			else //未接收到任何命令
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
	
  if(cur_landmark == 1)  //到了弯道
	{
		turn_RI(walk_route[number_x]);
	}
	else if(err > 1)  //  左转
	{
			//if(val_err == 6)
		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_0)==1) //只要第一位检测到
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
	
	else if(err < -1)  //  右转
	{
			//if(val_err == 6)
			if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_7)==1)  //只要第八位检测到
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
	else if (err == 1)     //直走
	{
		ena1_status = 1;
		ena2_status = 1;
		dir1_status = 0;
		dir2_status = 0;
		pwm1_value = 3;
		pwm2_value = 3;
	}
	else   //未检测到   按上次的值运行
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


void status_res(void)   //状态
{
	unsigned char lr = 0;
	if(status_flag == 4)
	{
		usart_status(waiting_send);  //等待送餐
	}
	else if(status_flag == 1)
	{
		usart_status(Room_service_route);  //在送餐路上
	}
	else if(status_flag == 2)   //到达指定位置   停止
	{
	  usart_status(Arrive_designated_table);  //到达指定餐桌
	}
	else if(status_flag == 3)
	{
		usart_status(back_to_wait_road);  //在返回等待区的路上
	}
	else if(status_flag == 10)
	{
		usart_status(Low_electricity);  //电量过低
	}
	else if(stop_flag == 1)
	{
		usart_status(covered_long_time);  //被挡住
	}
	else if(status_flag == 10)
	{
		usart_status(charge_area);  //在充电区域
	}
	for(lr=0;lr<20;lr++)              //状态发送完后继续执行上次命令
	  recv_buffer[lr]=lastrecv_buffer[lr];
	USART_send_flag = 0;
}	



void Start_serving(void)    //执行送餐指令
{
		if((CardRecBuffer[0]==recv_buffer[7])&&(CardRecBuffer[1]==0x00))   //到达指定位置   停止
		{
		  stop();
			usart_status(Arrive_designated_table); //发送到达餐桌状态
			if(status_flag==1)
			{
				insert_filemusic(2,3);
			}
			task_ok = 1;
			status_flag=2;  //到达指定送餐餐桌
		}
		else 
		{
			control_line();
			status_flag=1;  //在送餐的路上
		}
}


void backwait(void)     //返回等待区
{
  if((CardRecBuffer[0]==recv_buffer[7])&&(CardRecBuffer[1]==0xfe))   //到达指定位置   停止
		{
			status_flag=4;   //在送餐区等待
		  stop();
			usart_status(waiting_send); //发送状态
			task_ok = 1;
		}
		else 
		{
			status_flag=3;  //在返回等待送餐区的路上
			control_line();
		}
}


void charging(void)    //去充电
{
	if((CardRecBuffer[0]==recv_buffer[7])&&(CardRecBuffer[1]==0xfd))   //到达指定充电位置   停止
		{
			status_flag=6;   //到达充电区域
		  stop();
			usart_status(charge_area); //发送状态
			task_ok = 1;
		}
	else 
		{
			status_flag=5;    //去往充电的路上
			control_line();
		}  
		
	if(look_charge==1)  //寻找充电标志位
	{
	}
		
}

void stop(void)   //停车
{
		ena1_status = 0;  //电机不使能
	//dir1_status = 1;
	  ena2_status = 0;
	//dir2_status = 1;
}

void turn_RI(unsigned char RI)   //机器人转弯
{
	turn_time++;
	if(turn_time >505) turn_time=505;
	if(turn_time>100)
	{
	if((turn_time>=100)&&(turn_time <= 450))  //时间约为2s
	{
		if(RI == 0x01)  //右转
		{
			ena1_status = 0;
			ena2_status = 1;
		}
		if(RI == 0x02)  //左转
		{
			ena1_status = 1;
			ena2_status = 0;
		}
		if(RI == 0x03)   //直走
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
		if(RI == 0x01)  //右转
		{
			ena1_status = 0;
			ena2_status = 1;
		}
		if(RI == 0x02)  //左转
		{
			ena1_status = 1;
			ena2_status = 0;
		}
		if(RI == 0x03)   //直走
		{
			ena1_status = 1;
			ena2_status = 1;
		}
		dir2_status = 0;    
	  dir1_status = 0; 
	  MCTM_SetCounterReload(HT_MCTM1, wCRR);
		GPTM_SetCounterReload(HT_GPTM0, wCRR);
		if((((data==0x30)||(data==0x0c)||(data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))&&(turn_time > 300)))  //转身完成并且传感器大概处于中间位置
		{
			turn_back_complete_flag=1;
			number_x ++;
			cur_landmark=0;  //弯道标志位清零
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

void turn_back(void)  //原地转向
{
	turn_time++;
	if(turn_time > 700) turn_time = 700;
	if(recv_buffer[7]==0x01)  //右转
	{
		if(recv_buffer[6]==0x02)   //转180度
		{
			if((turn_time <= 200))   //时间约为500ms
			{
				ena1_status = 1;  //电机使能
				ena2_status = 1;
				dir2_status = 0;
				dir1_status = 1;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR);
			}
			else
			{
				if((((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))&&(turn_time > 200)))  //转身完成并且传感器大概处于中间位置
				{
					usart_status(Turn_around_finish);
					stop();
					turn_back_complete_flag=1;
					task_ok = 1;
				}
				else
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
				}
			}
	  }
		else if(recv_buffer[6]==0x01)  //转90度
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
				ena1_status = 1;  //电机使能
				ena2_status = 1;
				dir2_status = 0;
				dir1_status = 1;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR);
			}
		}
		else if(recv_buffer[6]==0x03)   //驶回轨道
		{
			if((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))  //转身完成并且传感器大概处于中间位置
			{
				usart_status(Turn_around_finish);
				stop();
				turn_back_complete_flag=1;
				task_ok = 1;
			}
			else
			{
				ena1_status = 1;  //电机使能
				ena2_status = 1;
				dir2_status = 0;
				dir1_status = 1;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR);
			}
		}
	}
	if(recv_buffer[7]==0x02)  //左转
	{
		if(recv_buffer[6]==0x02)  //转180度
		{
			if((turn_time <= 200))   //时间约为500ms
			{
				ena1_status = 1;  //电机使能
				ena2_status = 1;
				dir2_status = 1;     
				dir1_status = 0;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR);
			}
			else
			{
				if((((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))&&(turn_time > 200)))  //转身完成并且传感器大概处于中间位置
				{
					usart_status(Turn_around_finish);
					stop();
					turn_back_complete_flag=1;
					task_ok = 1;
				}
				else
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
				}
			}
	  }
		else if(recv_buffer[6]==0x01)  //转90度
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
				ena1_status = 1;  //电机使能
				ena2_status = 1;
				dir2_status = 1;
				dir1_status = 0;
				MCTM_SetCounterReload(HT_MCTM1, wCRR);
		    GPTM_SetCounterReload(HT_GPTM0, wCRR); 
			}
		}
		else if(recv_buffer[6]==0x03)   //驶回轨道
		{
			if((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))  //转身完成并且传感器大概处于中间位置
			{
				usart_status(Turn_around_finish);
				stop();
				turn_back_complete_flag=1;
				task_ok = 1;
			}
			else
			{
				ena1_status = 1;  //电机使能
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

输入参数：recv_buffer[6]：开始或完成避让控制
         recv_buffer[7]：避让方向控制
*********************************************/
void dodge(unsigned char do_status, unsigned char direct)    //避让程序，向指定方向转90度左右在前行一定的距离
{

	if(recv_buffer[5] == 0x05)   //确定为避让指令
	{
		if(do_status == start_dodge)  //开始避让  0x01
		{
			timer_5ms_count++;
	    if(timer_5ms_count >2100) timer_5ms_count=2100;
			
			if(direct == dodge_direct_left)   //左避让 0x02
			{
				if(timer_5ms_count < 600) 				//这个时间内，机器人原地转向
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
					dir2_status = 1;    //左边的轮子反转  
					dir1_status = 0;    //右边的轮子正转
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else if((timer_5ms_count >= 600) && (timer_5ms_count < 2000))   //之后直线行驶一段距离
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
					dir2_status = 0;    //左边的轮子反转
					dir1_status = 0;    //右边的轮子正转
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else         //避让完成，停止等待
				{
					stop();
				  usart_status(dodge_ok);
					complete_dodge_flag = 1;
				}
					
			}
			if(direct == dodge_direct_right )   //右避让 0x01
			{
				if(timer_5ms_count < 600) 				//这个时间内，机器人原地转向
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
					dir2_status = 0;    //左边的轮子正转  
					dir1_status = 1;    //右边的轮子反转
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else if((timer_5ms_count >= 600) && (timer_5ms_count < 2000))   //之后直线行驶一段距离
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
					dir2_status = 0;    
					dir1_status = 0;    
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				} 
				else         //避让完成，停止等待
				{
					stop();
				  usart_status(dodge_ok);
					complete_dodge_flag = 1;
					task_ok = 1;
				}
			}
		}
		
		if(do_status == complete_dodge_back)  //避让完成退回原地,并继续前行 0x02
		{
			back_flag ++;
			if(back_flag >2100) back_flag=2100;
			if(direct == dodge_direct_left)   //左退回 0x02
			{
				if(back_flag < 1500) 				
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
					dir2_status = 1;    //左边的轮子反转   退回
					dir1_status = 1;    //右边的轮子反转
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else if((back_flag >= 1500) && (back_flag < 1700))  
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
					dir2_status = 0;    //左边的轮子正转
					dir1_status = 1;    //右边的轮子反转
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
					
				}
				else     //避让完成，继续前行
				{
					if((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))  //转身完成并且传感器大概处于中间位置
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
						ena1_status = 1;  //电机使能
					  ena2_status = 1;
					}
					
				}
					
			}
			if(direct == dodge_direct_right)   //左退回 0x01
			{
				if(back_flag < 1500) 				//这个时间内，机器人原地转向
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
					dir2_status = 1;    //左边的轮子反转   退回
					dir1_status = 1;    //右边的轮子反转
          MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else if((back_flag >= 1500) && (back_flag < 1700))   //之后直线行驶一段距离
				{
					ena1_status = 1;  //电机使能
					ena2_status = 1;
					dir2_status = 1;    //左边的轮子反转
					dir1_status = 0;    //右边的轮子正转
					MCTM_SetCounterReload(HT_MCTM1, wCRR);
		      GPTM_SetCounterReload(HT_GPTM0, wCRR);
				}
				else     //避让完成，继续前行
				{
					//ena1_status = 1;  //电机使能
					//ena2_status = 1;
					if((data==0x18)||(data==0x1c)||(data==0x38)||((data>=0x18)&&(data<=0x38)))  //转身完成并且传感器大概处于中间位置
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
						ena1_status = 1;  //电机使能
					  ena2_status = 1;
					}
				}
			}
		}
  }
}

void usart_status(unsigned char np)   //状态发送函数，输入参数为当前状态
{
	if(USART_send_flag == 0)
	{
	  	unsigned char mp,n=0;
		  check_bit1=((0xaa+0x55+0x04+0x00+rob_number+np)&(0xff));
		  mp = check_bit1;
		  for(n=0;n<7;n++)   //发送当前状态
		 {
			  current_state[5] = np;
			  current_state[6] = mp;
				USART_SendData(HT_USART0,current_state[n]);
		 }
			USART_send_flag =1;
   }
}









void SysTick_Handler(void) //延时中断
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

//void BFTM1_IRQHandler(void)  //定时器1中断
//{
//	USART_SendData(HT_UART0,0x02);
//	  t++;
//	  HT32F_DVB_LEDToggle(HT_LED1);		//翻转电平的函数

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

#include "ht32f1655_56.h" 
#ifndef _controll_main_h
#define _controll_main_h


#define rob_number  0x01      //该机器人编号为 1 号

#define Food_delivery 0x01    //送餐命令
#define back_to_wait 0x02    //返回等待区
#define status_inquiry 0x03   //状态查询命令
#define go_charge 0x04        //去往充电区域
#define dodge_command 0x05    //避让命令
#define turnback 0x06      //转身
#define Unconditional_stop  0x08  //无条件停车


#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
#define MAX_DATA_LENS 127



#define rob1  0x01  //定义机器人号



#define tab1  0x01   //定义餐桌号
#define tab2  0x02
#define tab3  0x03
#define tab4  0x04
#define tab5  0x05
#define tab6  0x06
#define tab7  0x07
#define tab8  0x08
#define tab9  0x09



#define work  0x01    //开始工作
#define back  0x02		//回到起点




#define ena1_status PBout(2)   //电机使能及方向控制
#define dir1_status PBout(3)
#define ena2_status PBout(4)
#define dir2_status PBout(5)
#define busy_music  PAin(1)
#define charging_status PEin(8)


#define led1 PDout(2)   //电机使能及方向控制
#define led2 PDout(3)
#define led3 Pout(4)
#define led4 PDout(5)

#define test_led1 PCout(9)
#define test_led2 PCout(10)

#define G_PWM1 PDout(6)
#define G_PWM2 PDout(7)


#define LED0 PDout(8)  //LED显示传感器状态
#define LED1 PDout(9)
#define LED2 PDout(10)
#define LED3 PDout(11)
#define LED4 PDout(12)
#define LED5 PDout(13)
#define LED6 PDout(14)
#define LED7 PDout(15)





void INPUT_Line_Init(void);
void System_Init(void);
void delay(int i);
void USART_INT(void);
void pwm1_init(void);
void pwm2_init(void);
void DIR_Init(void);
unsigned char ReadData(void);
void infrared_init(void);
void IRLED(void);
unsigned char step_control(void);
void adc_init(void);
void other_gpio(void);
void led_display(void);



#endif

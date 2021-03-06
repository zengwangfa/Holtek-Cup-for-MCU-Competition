//------------------DS18B20.c---------------------//

#include "DS18B20.h"


//向串口屏所发送数据
u8 Temp_Ten_Cmd[]    ={0x74,0x31,0x36,0x2E,0x74,0x78,0x74,0x3D,0x22,/*温度十位*/0x31,/*温度个位*/0x31,0x22,0xff,0xff,0xff};//  31 31   ASC值实际为1 1℃  
u8 Temp_decimal_Cmd[]={0x74,0x31,0x37,0x2E,0x74,0x78,0x74,0x3D,0x22,/*温度小数*/0x31,0x22,0xff,0xff,0xff};  //所以每一位需要加+30
//温度形参

u16 temperature,temp_positive,temp_ten,temp_unit,temp_decimal;  //蜂箱内的温度值

extern u8 DataBuffer[8];
extern u8 recv1_data_ok;

void UART_Send_Temp(void);
//复位DS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PA0 OUTPUT
  DS18B20_DQ_OUT=0; //拉低DQ
  delay_us(750);    //拉低750us
  DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}
//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//从DS18B20读取一个位
//返回值：1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
  u8 data;
	DS18B20_IO_OUT();//SET PA0 OUTPUT
  DS18B20_DQ_OUT=0; 
	delay_us(2);
  DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();//SET PA0 INPUT
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}
//从DS18B20读取一个字节
//返回值：读到的数据
u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}
//开始温度转换
void DS18B20_Start(void)// ds1820 start convert
{   						               
  DS18B20_Rst();	   
	DS18B20_Check();	 
  DS18B20_Write_Byte(0xcc);// skip rom
  DS18B20_Write_Byte(0x44);// convert
} 
//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init(void)     //-------------------------------------------------------------------------------
{	

	AFIO_GPxConfig(GPIO_PA, DS18B20_PIN, AFIO_MODE_DEFAULT);   //PA8
	GPIO_DirectionConfig(HT_GPIOA, DS18B20_PIN, GPIO_DIR_OUT);//输出方向
	GPIO_OpenDrainConfig(HT_GPIOA,DS18B20_PIN,DISABLE);//推挽输出
	GPIO_WriteOutBits(HT_GPIOA, DS18B20_PIN, SET);        //PA8->1

	DS18B20_Rst();

	return DS18B20_Check();
}  
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	  short tem;
    DS18B20_Start ();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    TL=DS18B20_Read_Byte(); // LSB   
    TH=DS18B20_Read_Byte(); // MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(float)tem*0.625;//转换     
	if(temp)return tem; //返回温度值
	else return -tem;    
} 

u8 *Replace_Temp_Array(u8 array[],u8 num1,u8 num2)//更新温度值
{
  array[9] = num1;
	array[10] =num2;
  return array;
}

void UART_Send_Temp()  //向串口屏发送温度数据
{
			temperature=DS18B20_Get_Temp();	

			temp_positive = temperature/10;  		//显示正数部分	 如27 ℃  
		  temp_ten    = temp_positive/10;     //十位  如2
	   	temp_unit   = temp_positive%10;    	//个位  如7
			temp_decimal  = temperature%10; 		//显示小数部分   如0.3 ℃
	
//			DataBuffer[4]=(temperature+500)>>8;
//		  DataBuffer[5]=(temperature+500);                     //2字节的温度数据放入数组中

			//delay_ms(20); //不延时 发送数据不稳定（待解决）
			USART_Sends(HT_USART1,Replace_Temp_Array(Temp_Ten_Cmd,temp_ten+0x30,temp_unit+0x30),15);//发送正数部分
			delay_ms(20);	
		  USART_Sends(HT_USART1,Replace_Temp_Array(Temp_decimal_Cmd,temp_decimal+0x30,  0x22),14);//发送小数部�
			delay_ms(20);	
			//delay_ms(50);

}


//------------------DS18B20.c---------------------//

#include "DS18B20.h"


//�򴮿�������������
u8 Temp_Ten_Cmd[]    ={0x74,0x31,0x36,0x2E,0x74,0x78,0x74,0x3D,0x22,/*�¶�ʮλ*/0x31,/*�¶ȸ�λ*/0x31,0x22,0xff,0xff,0xff};//  31 31   ASCֵʵ��Ϊ1 1��  
u8 Temp_decimal_Cmd[]={0x74,0x31,0x37,0x2E,0x74,0x78,0x74,0x3D,0x22,/*�¶�С��*/0x31,0x22,0xff,0xff,0xff};  //����ÿһλ��Ҫ��+30
//�¶��β�

u16 temperature,temp_positive,temp_ten,temp_unit,temp_decimal;  //�����ڵ��¶�ֵ

extern u8 DataBuffer[8];
extern u8 recv1_data_ok;

void UART_Send_Temp(void);
//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PA0 OUTPUT
  DS18B20_DQ_OUT=0; //����DQ
  delay_us(750);    //����750us
  DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
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
//��DS18B20��ȡһ��λ
//����ֵ��1/0
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
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
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
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
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
//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{   						               
  DS18B20_Rst();	   
	DS18B20_Check();	 
  DS18B20_Write_Byte(0xcc);// skip rom
  DS18B20_Write_Byte(0x44);// convert
} 
//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)     //-------------------------------------------------------------------------------
{	

	AFIO_GPxConfig(GPIO_PA, DS18B20_PIN, AFIO_MODE_DEFAULT);   //PA8
	GPIO_DirectionConfig(HT_GPIOA, DS18B20_PIN, GPIO_DIR_OUT);//�������
	GPIO_OpenDrainConfig(HT_GPIOA,DS18B20_PIN,DISABLE);//�������
	GPIO_WriteOutBits(HT_GPIOA, DS18B20_PIN, SET);        //PA8->1

	DS18B20_Rst();

	return DS18B20_Check();
}  
//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
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
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    tem=(float)tem*0.625;//ת��     
	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
} 

u8 *Replace_Temp_Array(u8 array[],u8 num1,u8 num2)//�����¶�ֵ
{
  array[9] = num1;
	array[10] =num2;
  return array;
}

void UART_Send_Temp()  //�򴮿��������¶�����
{
			temperature=DS18B20_Get_Temp();	

			temp_positive = temperature/10;  		//��ʾ��������	 ��27 ��  
		  temp_ten    = temp_positive/10;     //ʮλ  ��2
	   	temp_unit   = temp_positive%10;    	//��λ  ��7
			temp_decimal  = temperature%10; 		//��ʾС������   ��0.3 ��
	
//			DataBuffer[4]=(temperature+500)>>8;
//		  DataBuffer[5]=(temperature+500);                     //2�ֽڵ��¶����ݷ���������

			//delay_ms(20); //����ʱ �������ݲ��ȶ����������
			USART_Sends(HT_USART1,Replace_Temp_Array(Temp_Ten_Cmd,temp_ten+0x30,temp_unit+0x30),15);//������������
			delay_ms(20);	
		  USART_Sends(HT_USART1,Replace_Temp_Array(Temp_decimal_Cmd,temp_decimal+0x30,  0x22),14);//����С�����
			delay_ms(20);	
			//delay_ms(50);

}


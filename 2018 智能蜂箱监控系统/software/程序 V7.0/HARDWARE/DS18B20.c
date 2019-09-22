//------------------DS18B20.c---------------------//

#include "DS18B20.h"


//Ïò´®¿ÚÆÁËù·¢ËÍÊı¾İ
u8 Temp_Ten_Cmd[]    ={0x74,0x31,0x36,0x2E,0x74,0x78,0x74,0x3D,0x22,/*ÎÂ¶ÈÊ®Î»*/0x31,/*ÎÂ¶È¸öÎ»*/0x31,0x22,0xff,0xff,0xff};//  31 31   ASCÖµÊµ¼ÊÎª1 1¡æ  
u8 Temp_decimal_Cmd[]={0x74,0x31,0x37,0x2E,0x74,0x78,0x74,0x3D,0x22,/*ÎÂ¶ÈĞ¡Êı*/0x31,0x22,0xff,0xff,0xff};  //ËùÒÔÃ¿Ò»Î»ĞèÒª¼Ó+30
//ÎÂ¶ÈĞÎ²Î

u16 temperature,temp_positive,temp_ten,temp_unit,temp_decimal;  //·äÏäÄÚµÄÎÂ¶ÈÖµ

extern u8 DataBuffer[8];
extern u8 recv1_data_ok;

void UART_Send_Temp(void);
//¸´Î»DS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PA0 OUTPUT
  DS18B20_DQ_OUT=0; //À­µÍDQ
  delay_us(750);    //À­µÍ750us
  DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}
//µÈ´ıDS18B20µÄ»ØÓ¦
//·µ»Ø1:Î´¼ì²âµ½DS18B20µÄ´æÔÚ
//·µ»Ø0:´æÔÚ
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
//´ÓDS18B20¶ÁÈ¡Ò»¸öÎ»
//·µ»ØÖµ£º1/0
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
//´ÓDS18B20¶ÁÈ¡Ò»¸ö×Ö½Ú
//·µ»ØÖµ£º¶Áµ½µÄÊı¾İ
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
//Ğ´Ò»¸ö×Ö½Úµ½DS18B20
//dat£ºÒªĞ´ÈëµÄ×Ö½Ú
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
//¿ªÊ¼ÎÂ¶È×ª»»
void DS18B20_Start(void)// ds1820 start convert
{   						               
  DS18B20_Rst();	   
	DS18B20_Check();	 
  DS18B20_Write_Byte(0xcc);// skip rom
  DS18B20_Write_Byte(0x44);// convert
} 
//³õÊ¼»¯DS18B20µÄIO¿Ú DQ Í¬Ê±¼ì²âDSµÄ´æÔÚ
//·µ»Ø1:²»´æÔÚ
//·µ»Ø0:´æÔÚ    	 
u8 DS18B20_Init(void)     //-------------------------------------------------------------------------------
{	

	AFIO_GPxConfig(GPIO_PA, DS18B20_PIN, AFIO_MODE_DEFAULT);   //PA8
	GPIO_DirectionConfig(HT_GPIOA, DS18B20_PIN, GPIO_DIR_OUT);//Êä³ö·½Ïò
	GPIO_OpenDrainConfig(HT_GPIOA,DS18B20_PIN,DISABLE);//ÍÆÍìÊä³ö
	GPIO_WriteOutBits(HT_GPIOA, DS18B20_PIN, SET);        //PA8->1

	DS18B20_Rst();

	return DS18B20_Check();
}  
//´Óds18b20µÃµ½ÎÂ¶ÈÖµ
//¾«¶È£º0.1C
//·µ»ØÖµ£ºÎÂ¶ÈÖµ £¨-550~1250£© 
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
        temp=0;//ÎÂ¶ÈÎª¸º  
    }else temp=1;//ÎÂ¶ÈÎªÕı	  	  
    tem=TH; //»ñµÃ¸ß°ËÎ»
    tem<<=8;    
    tem+=TL;//»ñµÃµ×°ËÎ»
    tem=(float)tem*0.625;//×ª»»     
	if(temp)return tem; //·µ»ØÎÂ¶ÈÖµ
	else return -tem;    
} 

u8 *Replace_Temp_Array(u8 array[],u8 num1,u8 num2)//¸üĞÂÎÂ¶ÈÖµ
{
  array[9] = num1;
	array[10] =num2;
  return array;
}

void UART_Send_Temp()  //Ïò´®¿ÚÆÁ·¢ËÍÎÂ¶ÈÊı¾İ
{
			temperature=DS18B20_Get_Temp();	

			temp_positive = temperature/10;  		//ÏÔÊ¾ÕıÊı²¿·Ö	 Èç27 ¡æ  
		  temp_ten    = temp_positive/10;     //Ê®Î»  Èç2
	   	temp_unit   = temp_positive%10;    	//¸öÎ»  Èç7
			temp_decimal  = temperature%10; 		//ÏÔÊ¾Ğ¡Êı²¿·Ö   Èç0.3 ¡æ
	
//			DataBuffer[4]=(temperature+500)>>8;
//		  DataBuffer[5]=(temperature+500);                     //2×Ö½ÚµÄÎÂ¶ÈÊı¾İ·ÅÈëÊı×éÖĞ

			//delay_ms(20); //²»ÑÓÊ± ·¢ËÍÊı¾İ²»ÎÈ¶¨£¨´ı½â¾ö£©
			USART_Sends(HT_USART1,Replace_Temp_Array(Temp_Ten_Cmd,temp_ten+0x30,temp_unit+0x30),15);//·¢ËÍÕıÊı²¿·Ö
			delay_ms(20);	
		  USART_Sends(HT_USART1,Replace_Temp_Array(Temp_decimal_Cmd,temp_decimal+0x30,  0x22),14);//·¢ËÍĞ¡Êı²¿·
			delay_ms(20);	
			//delay_ms(50);

}


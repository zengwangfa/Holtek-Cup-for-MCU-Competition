#ifndef __GPS_H
#define __GPS_H	 
#include "sys.h"  
		
//********************************************************************************
//V2.0 �޸�˵�� 20140208
//1,���Ublox_Cfg_Cfg_Save����
//2,���Ublox_Cfg_Msg����
//3,���Ublox_Cfg_Prt����.				  
////////////////////////////////////////////////////////////////////////////////// 	   
 									   						    
//GPS NMEA-0183Э����Ҫ�����ṹ�嶨�� 
//������Ϣ
__packed typedef struct  
{										    
 	u8 num;		//���Ǳ��
	u8 eledeg;	//��������
	u16 azideg;	//���Ƿ�λ��
	u8 sn;		//�����		   
}nmea_slmsg;  
//UTCʱ����Ϣ
__packed typedef struct  
{										    
 	u16 year;	//���
	u8 month;	//�·�
	u8 date;	//����
	u8 hour; 	//Сʱ
	u8 min; 	//����
	u8 sec; 	//����
}nmea_utc_time;   	   
//NMEA 0183 Э����������ݴ�Žṹ��
__packed typedef struct  
{										    
 	u8 svnum;					//�ɼ�������
	nmea_slmsg slmsg[12];		//���12������
	nmea_utc_time utc;			//UTCʱ��
	u32 latitude;				//γ�� ������100000��,ʵ��Ҫ����100000
	u8 nshemi;					//��γ/��γ,N:��γ;S:��γ				  
	u32 longitude;			    //���� ������100000��,ʵ��Ҫ����100000
	u8 ewhemi;					//����/����,E:����;W:����
	u8 gpssta;					//GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.				  
 	u8 posslnum;				//���ڶ�λ��������,0~12.
 	u8 possl[12];				//���ڶ�λ�����Ǳ��
	u8 fixmode;					//��λ����:1,û�ж�λ;2,2D��λ;3,3D��λ
	u16 pdop;					//λ�þ������� 0~500,��Ӧʵ��ֵ0~50.0
	u16 hdop;					//ˮƽ�������� 0~500,��Ӧʵ��ֵ0~50.0
	u16 vdop;					//��ֱ�������� 0~500,��Ӧʵ��ֵ0~50.0 

	int altitude;			 	//���θ߶�,�Ŵ���10��,ʵ�ʳ���10.��λ:0.1m	 
	u16 speed;					//��������,�Ŵ���1000��,ʵ�ʳ���10.��λ:0.001����/Сʱ	 
}nmea_msg; 
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//UBLOX NEO-6M ����(���,����,���ص�)�ṹ��
__packed typedef struct  
{										    
 	u16 header;					//cfg header,�̶�Ϊ0X62B5(С��ģʽ)
	u16 id;						//CFG CFG ID:0X0906 (С��ģʽ)
	u16 dlength;				//���ݳ��� 12/13
	u32 clearmask;				//�������������(1��Ч)
	u32 savemask;				//�����򱣴�����
	u32 loadmask;				//�������������
	u8  devicemask; 		  	//Ŀ������ѡ������	b0:BK RAM;b1:FLASH;b2,EEPROM;b4,SPI FLASH
	u8  cka;		 			//У��CK_A 							 	 
	u8  ckb;			 		//У��CK_B							 	 
}_ublox_cfg_cfg; 

//UBLOX NEO-6M ��Ϣ���ýṹ��
__packed typedef struct  
{										    
 	u16 header;					//cfg header,�̶�Ϊ0X62B5(С��ģʽ)
	u16 id;						//CFG MSG ID:0X0106 (С��ģʽ)
	u16 dlength;				//���ݳ��� 8
	u8  msgclass;				//��Ϣ����(F0 ����NMEA��Ϣ��ʽ)
	u8  msgid;					//��Ϣ ID 
								//00,GPGGA;01,GPGLL;02,GPGSA;
								//03,GPGSV;04,GPRMC;05,GPVTG;
								//06,GPGRS;07,GPGST;08,GPZDA;
								//09,GPGBS;0A,GPDTM;0D,GPGNS;
	u8  iicset;					//IIC���������    0,�ر�;1,ʹ��.
	u8  uart1set;				//UART1�������	   0,�ر�;1,ʹ��.
	u8  uart2set;				//UART2�������	   0,�ر�;1,ʹ��.
	u8  usbset;					//USB�������	   0,�ر�;1,ʹ��.
	u8  spiset;					//SPI�������	   0,�ر�;1,ʹ��.
	u8  ncset;					//δ֪�������	   Ĭ��Ϊ1����.
 	u8  cka;			 		//У��CK_A 							 	 
	u8  ckb;			    	//У��CK_B							 	 
}_ublox_cfg_msg; 

//UBLOX NEO-6M UART�˿����ýṹ��
__packed typedef struct  
{										    
 	u16 header;					//cfg header,�̶�Ϊ0X62B5(С��ģʽ)
	u16 id;						//CFG PRT ID:0X0006 (С��ģʽ)
	u16 dlength;				//���ݳ��� 20
	u8  portid;					//�˿ں�,0=IIC;1=UART1;2=UART2;3=USB;4=SPI;
	u8  reserved;				//����,����Ϊ0
	u16 txready;				//TX Ready��������,Ĭ��Ϊ0
	u32 mode;					//���ڹ���ģʽ����,��żУ��,ֹͣλ,�ֽڳ��ȵȵ�����.
 	u32 baudrate;				//����������
 	u16 inprotomask;		 	//����Э�鼤������λ  Ĭ������Ϊ0X07 0X00����.
 	u16 outprotomask;		 	//���Э�鼤������λ  Ĭ������Ϊ0X07 0X00����.
 	u16 reserved4; 				//����,����Ϊ0
 	u16 reserved5; 				//����,����Ϊ0 
 	u8  cka;			 		//У��CK_A 							 	 
	u8  ckb;			    	//У��CK_B							 	 
}_ublox_cfg_prt; 

//UBLOX NEO-6M ʱ���������ýṹ��
__packed typedef struct  
{										    
 	u16 header;					//cfg header,�̶�Ϊ0X62B5(С��ģʽ)
	u16 id;						//CFG TP ID:0X0706 (С��ģʽ)
	u16 dlength;				//���ݳ���
	u32 interval;				//ʱ��������,��λΪus
	u32 length;				 	//������,��λΪus
	signed char status;			//ʱ����������:1,�ߵ�ƽ��Ч;0,�ر�;-1,�͵�ƽ��Ч.			  
	u8 timeref;			   		//�ο�ʱ��:0,UTCʱ��;1,GPSʱ��;2,����ʱ��.
	u8 flags;					//ʱ���������ñ�־
	u8 reserved;				//����			  
 	signed short antdelay;	 	//������ʱ
 	signed short rfdelay;		//RF��ʱ
	signed int userdelay; 	 	//�û���ʱ	
	u8 cka;						//У��CK_A 							 	 
	u8 ckb;						//У��CK_B							 	 
}_ublox_cfg_tp; 

//UBLOX NEO-6M ˢ���������ýṹ��
__packed typedef struct  
{										    
 	u16 header;					//cfg header,�̶�Ϊ0X62B5(С��ģʽ)
	u16 id;						//CFG RATE ID:0X0806 (С��ģʽ)
	u16 dlength;				//���ݳ���
	u16 measrate;				//����ʱ��������λΪms�����ٲ���С��200ms��5Hz��
	u16 navrate;				//�������ʣ����ڣ����̶�Ϊ1
	u16 timeref;				//�ο�ʱ�䣺0=UTC Time��1=GPS Time��
 	u8  cka;					//У��CK_A 							 	 
	u8  ckb;					//У��CK_B							 	 
}_ublox_cfg_rate; 
				 
int NMEA_Str2num(u8 *buf,u8*dx);
void GPS_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGGA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPRMC_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPVTG_Analysis(nmea_msg *gpsx,u8 *buf);
u8 Ublox_Cfg_Cfg_Save(void);
u8 Ublox_Cfg_Msg(u8 msgid,u8 uart1set);
u8 Ublox_Cfg_Prt(u32 baudrate);
u8 Ublox_Cfg_Tp(u32 interval,u32 length,signed char status);
u8 Ublox_Cfg_Rate(u16 measrate,u8 reftime);
#endif  

 




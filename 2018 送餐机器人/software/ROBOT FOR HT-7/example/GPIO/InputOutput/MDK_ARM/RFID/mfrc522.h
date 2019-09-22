/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
#ifndef __MFRCC522_H
#define __MFRCC522_H


#include "stdint.h"


void RFID_read(void);

int8_t PcdReset(void);
void PcdAntennaOn(void);
void PcdAntennaOff(void);
int8_t PcdRequest(unsigned char req_code,unsigned char *pTagType);   
int8_t PcdAnticoll(unsigned char *pSnr);
int8_t PcdSelect(unsigned char *pSnr);         
int8_t PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);     
int8_t PcdRead(unsigned char addr,unsigned char *pData);     
int8_t PcdWrite(unsigned char addr,unsigned char *pData);    
int8_t PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue);   
int8_t PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr);                                 
int8_t PcdHalt(void);
int8_t PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit);
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData);
void WriteRawRC(unsigned char Address,unsigned char value);
unsigned char ReadRawRC(unsigned char Address); 
void SetBitMask(unsigned char reg,unsigned char mask); 
void ClearBitMask(unsigned char reg,unsigned char mask); 
signed char M500PcdConfigISOType(unsigned char type);
void delay_10ms(unsigned int _10ms);
//void iccardcode();
int8_t PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr);
int8_t PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue);
void RC522_Control_IO_Init(void );                 //RFID-RC522控制引脚初始化
/////////////////////////////////////////////////////////////////////
//MF522命令字
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

/////////////////////////////////////////////////////////////////////
//Mifare_One卡片命令字
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

/////////////////////////////////////////////////////////////////////
//MF522 FIFO长度定义
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

/////////////////////////////////////////////////////////////////////
//MF522寄存器定义
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		  0x3F

/////////////////////////////////////////////////////////////////////
//和MF522通讯时返回的错误代码
/////////////////////////////////////////////////////////////////////
#define MI_OK                          0
#define MI_NOTAGERR                    (-1)
#define MI_ERR                         (-2)
//////////////////////////////////
//端口定义
/////////////////////////////////////////////////////////////////////
//MFRC522

#define MF522_SCK_0            	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, RESET)	 
#define MF522_SCK_1            	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, SET)	 	
#define MF522_NSS_0							GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6, RESET)	  
#define MF522_NSS_1							GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6, SET)		  
#define MF522_MOSI_0						GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8, RESET)		
#define MF522_MOSI_1						GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8, SET)		
#define MF522_MISO_0R1					GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_10)

#define MF522_RST_1							GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_9, SET)		
#define MF522_RST_0							GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_9, RESET)	

#define RCC522_PORT      HT_GPIOB	                                    //定义RCC522的端口为PB端口


//#define MF522_SCK_Pin   GPIO_PIN_5                                   //RCC522的SCK引脚接到PA5
//#define MF522_NSS_Pin   GPIO_Pin_4                                   //RCC522的SDA引脚接到PA4
//#define MF522_MOSI_Pin  GPIO_Pin_6                                  //RCC522的MOSI引脚接到PA7
//#define MF522_MISO_Pin  GPIO_Pin_7                                   //RCC522的MISO引脚接到PA6
//#define MF522_RST_Pin   GPIO_Pin_4                                   //RCC522的RST引脚接到PA3
//sbit     MF522_RST  =    P1^4;                   //RC500片选
//sbit     MF522_NSS  =    P1^2;
//sbit     MF522_SCK  =    P1^1;
//sbit     MF522_SI   =    P1^0;
//sbit     MF522_SO   =    P1^3;
//指示灯
//sbit     LED_GREEN  =    P1^5;     
/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
//void InitializeSystem();                                   
#define BAUD_115200             256 - (OSC_FREQ/192L)/115200L   // 255
#define BAUD_57600              256 - (OSC_FREQ/192L)/57600L    // 254
#define BAUD_38400              256 - (OSC_FREQ/192L)/38400L    // 253
#define BAUD_28800              256 - (OSC_FREQ/192L)/28800L    // 252
#define BAUD_19200              256 - (OSC_FREQ/192L)/19200L    // 250
#define BAUD_14400              256 - (OSC_FREQ/192L)/14400L    // 248
#define BAUD_9600               256 - (OSC_FREQ/192L)/9600L     // 244                                   
#define OSC_FREQ                22118400L
//#define  OSC_FREQ        7372800L
#define OSC_FREQ          22118400L
#define  RCAP2_50us      65536L - OSC_FREQ/40417L
#define  RCAP2_1ms       65536L - OSC_FREQ/2000L
#define  RCAP2_10ms      65536L - OSC_FREQ/1200L
#define  TIME0_500us     65536L - OSC_FREQ/8000L
#define  TIME0_10ms      65536L - OSC_FREQ/200


#endif




#include "ht32f1655_56.h" 
#ifndef _config_h
#define _config_h


/*�������Ͷ���*/
typedef  signed    char    int8;    //8λ�з����ַ���
typedef  signed    int     int16;   //16λ�з�������
typedef  signed    long    int32;   //32λ�з��ų�����
typedef  unsigned  char    uint8;   //8�޷����ַ���
typedef  unsigned  int     uint16;  //16�޷�������
typedef  unsigned  long    uint32;  //32�޷��ų�����
/*********����51����I/O��*********************/
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOA_ODR_Addr    (HT_GPIOA_BASE+32) //0x4001080C        
#define GPIOB_ODR_Addr    (HT_GPIOB_BASE+32) //0x40010C0C 
#define GPIOC_ODR_Addr    (HT_GPIOC_BASE+32) //0x4001100C 
#define GPIOD_ODR_Addr    (HT_GPIOD_BASE+32) //0x4001140C 
#define GPIOE_ODR_Addr    (HT_GPIOE_BASE+32) //0x4001180C 
#define GPIOF_ODR_Addr    (HT_GPIOF_BASE+32) //0x40011A0C    
#define GPIOG_ODR_Addr    (HT_GPIOG_BASE+32) //0x40011E0C    

#define GPIOA_IDR_Addr    (HT_GPIOA_BASE+28) //0x40010808    
#define GPIOB_IDR_Addr    (HT_GPIOB_BASE+28) //0x40010C08 
#define GPIOC_IDR_Addr    (HT_GPIOC_BASE+28) //0x40011008 
#define GPIOD_IDR_Addr    (HT_GPIOD_BASE+28) //0x40011408 
#define GPIOE_IDR_Addr    (HT_GPIOE_BASE+28) //0x40011808 
#define GPIOF_IDR_Addr    (HT_GPIOF_BASE+28) //0x40011A08 
#define GPIOG_IDR_Addr    (HT_GPIOG_BASE+28) //0x40011E08 
 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)   

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)   

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)   

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n) 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  
/*****************�˿ڶ���****************/
#define KEY_IN_1   PAin(14)
#define KEY_IN_2   PAin(15)
#define KEY_IN_3   PAin(12)
#define KEY_IN_4   PAin(13)
#define KEY_OUT_1  PAout(10)
#define KEY_OUT_2  PAout(11)
#define KEY_OUT_3  PAout(8)
#define KEY_OUT_4  PAout(9)
/*****������ŷ���******/
#define Cx    PBout(0)   //X���ѻ�ѡ��Ϊ1���Ͽ�Ϊ0 
#define Cy    PBout(1)   //Y���ѻ�
#define dir   PBout(2)   //�������
#define CPx   PBout(3)   //ʱ������
#define Zred  PBout(4)   //Z��ǰ������
#define Zblack PBout(5)  //Z����˲���
/*****����Ӧ�����Ŷ������*****/
#define Xl  PCin(0)      //X������λ     
#define Xm  PCin(1)      //X���м�й���
#define Xr  PCin(2)      //X������λ
#define Yu  PCin(3)      //Y������λ
#define Ym  PCin(4)      //Y���м�й���
#define Yd  PCin(5)      //Y������λ
#define Zf  PCin(6)      //Z��ǰ��λ
#define Zb  PCin(7)      //Z�����λ

/*#define L1  PDout(4)      //Y���м�й���
#define L2  PDout(5)      //Y������λ
#define L3  PDout(6)      //Z��ǰ��λ*/
#define L4  PDout(7)      //Z�����λ
#endif

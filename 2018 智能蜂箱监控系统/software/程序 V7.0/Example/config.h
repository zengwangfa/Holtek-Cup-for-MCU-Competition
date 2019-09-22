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



#endif

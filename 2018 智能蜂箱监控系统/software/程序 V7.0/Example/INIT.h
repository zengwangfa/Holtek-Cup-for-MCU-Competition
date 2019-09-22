#ifndef _INIT_H_
#define _INIT_H_

#include "INCLUDE.h"

#define GATE PDin(11)  
void System_Init(void);  //系统时钟初始化
void CTR_IO_Init(void); 

void USART0_Init(u32 banud);
void USART1_Init(u32 banud);
void Time0_Init(void);
void Time1_Init(void);


#endif

#include "ht32f1655_56.h" 
#ifndef _controll_main_h
#define _controll_main_h

void Port_Init(void);
void System_Init(void);
void ZMoveback(void);
void ZMovefront(void);
void delay(void);
void XMoveback(void);
void XMovefront(void);
void Init(void);
void delayX(void); //����X��λ�õ�ƫ��
void delayY(void); //����T��λ�õ�ƫ��
void delay_wt(void);

#endif

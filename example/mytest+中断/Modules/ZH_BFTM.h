/* ��Ҫ������Kaven�������ޣ�������˱��������������ģ��Ų��������Է���ǰ���Ƚ��в��ԡ� */

#ifndef __ZH_BFTM_H
#define __ZH_BFTM_H

#include "ht32.h"

#define ZH_BFTM_CLK(CK)                 (CK.Bit.BFTM0)
#define ZH_BFTM_N                       HT_BFTM0
#define ZH_BFTM_IRQEN                   TRUE
#define ZH_BFTM_IRQ                     BFTM0_IRQn                              // BFTM0 ���ж�

#define ZH_BFTM_CMP                     (72000000/200)                          // 32-bit BFTM �������ıȽ�ֵ   5ms�ж�
#define ZH_BFTM_CNTR                    0                                       // 32-bit BFTM ��������ֵ
#define ZH_BFTM_MIEN                    ENABLE                                  // BFTM �Ƚ�ƥ���ж�ʹ�ܿ���  ENABLE/DISABLE
#define ZH_BFTM_CEN                     ENABLE                                  // BFTM ������ʹ�ܿ���  ENABLE/DISABLE
#define ZH_BFTM_OSM                     DISABLE                                 // BFTM ����ģʽѡ��    ENABLE/DISABLE  �������ظ�ģʽ


#define ZH_BFTM_CMP32K                  (32000/100)                             // 10ms �ж�
#define ZH_BFTM_CMP8M                   (8000000/1000)                          // 1ms �ж�
#define ZH_BFTM_CMP48M                  (48000000/1000)                         // 1ms �ж�








void ZH_BFTM_Init(void);
void ZH_BFTM_SetCMP(u32 Num);
void ZH_BFTM_Switch(ControlStatus NewState);








#endif

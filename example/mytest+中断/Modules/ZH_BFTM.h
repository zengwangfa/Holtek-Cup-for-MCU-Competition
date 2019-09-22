/* 重要声明：Kaven技术有限，如果用了本代码而出现问题的，概不负责。所以发布前请先进行测试。 */

#ifndef __ZH_BFTM_H
#define __ZH_BFTM_H

#include "ht32.h"

#define ZH_BFTM_CLK(CK)                 (CK.Bit.BFTM0)
#define ZH_BFTM_N                       HT_BFTM0
#define ZH_BFTM_IRQEN                   TRUE
#define ZH_BFTM_IRQ                     BFTM0_IRQn                              // BFTM0 总中断

#define ZH_BFTM_CMP                     (72000000/200)                          // 32-bit BFTM 计数器的比较值   5ms中断
#define ZH_BFTM_CNTR                    0                                       // 32-bit BFTM 计数器的值
#define ZH_BFTM_MIEN                    ENABLE                                  // BFTM 比较匹配中断使能控制  ENABLE/DISABLE
#define ZH_BFTM_CEN                     ENABLE                                  // BFTM 计数器使能控制  ENABLE/DISABLE
#define ZH_BFTM_OSM                     DISABLE                                 // BFTM 单次模式选项    ENABLE/DISABLE  工作于重复模式


#define ZH_BFTM_CMP32K                  (32000/100)                             // 10ms 中断
#define ZH_BFTM_CMP8M                   (8000000/1000)                          // 1ms 中断
#define ZH_BFTM_CMP48M                  (48000000/1000)                         // 1ms 中断








void ZH_BFTM_Init(void);
void ZH_BFTM_SetCMP(u32 Num);
void ZH_BFTM_Switch(ControlStatus NewState);








#endif

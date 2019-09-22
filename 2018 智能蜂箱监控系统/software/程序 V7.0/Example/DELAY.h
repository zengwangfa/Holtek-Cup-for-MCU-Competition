#ifndef __DELAY_H
#define __DELAY_H

#include "ht32.h"
#include "ht32_board.h"

#define us 0
#define ms 1

//#ifndef DelayTime
//  static vu32 DelayTime;
//#endif

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif

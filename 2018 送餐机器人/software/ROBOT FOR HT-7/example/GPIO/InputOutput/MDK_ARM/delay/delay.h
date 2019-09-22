#ifndef __DELAY_H
#define __DELAY_H

#include "ht32.h"
#include "ht32_board.h"

/* Private variables ---------------------------------------------------------------------------------------*/
static vu32 DelayTime;
/* Private function prototypes -----------------------------------------------------------------------------*/
void Delay(u32 nTime);
void TimingDelay(void);
void delay_init(void);

#endif



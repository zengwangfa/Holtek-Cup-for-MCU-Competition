#include "config.h"
#include "ht32f1655_56.h" 

#ifndef _ACTION_H
#define _ACTION_H


#ifndef _ACTION_C
/*在复位状态下 Ym=1,Xm=0 */
#endif
void XMove(uint8 num,uint8 model); //x轴运动
void ResetShaft(void);  //复位
void Get(uint8 get);  //取货物
void Store(uint8 store); //存货物
void KeyAction(uint8 keycode); //按键扫描
void floors(uint8 flo);  //层数判断
void storeaction(u8 Room1);
void getaction(u8 Room2);
#endif

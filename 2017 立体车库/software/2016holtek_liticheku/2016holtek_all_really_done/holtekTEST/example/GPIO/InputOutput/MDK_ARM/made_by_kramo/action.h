#include "config.h"
#include "ht32f1655_56.h" 

#ifndef _ACTION_H
#define _ACTION_H


#ifndef _ACTION_C
/*�ڸ�λ״̬�� Ym=1,Xm=0 */
#endif
void XMove(uint8 num,uint8 model); //x���˶�
void ResetShaft(void);  //��λ
void Get(uint8 get);  //ȡ����
void Store(uint8 store); //�����
void KeyAction(uint8 keycode); //����ɨ��
void floors(uint8 flo);  //�����ж�
void storeaction(u8 Room1);
void getaction(u8 Room2);
#endif

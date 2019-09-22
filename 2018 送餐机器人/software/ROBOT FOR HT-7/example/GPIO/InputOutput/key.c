#define  _KEY_C
#include "config.h"
#include "action.h"
#include "key.h"
#include "ht32_board.h"

const uint8 KeyCodeMap[4][4] = { 
    { 0x0a, 0x0b, 0x0c, 0x27 }, 
    { 0x07, 0x08, 0x09, 0x28 }, 
    { 0x04, 0x05, 0x06, 0x25 }, 
    { 0x01, 0x02, 0x03, 0x26 }  
	};//���󰴼���ŵ���׼���̵�ӳ���
    
	
	//���ּ�10 ���ּ�11 ���ּ�12
	//���ּ�4 ���ּ�5 ���ּ�6  ȡ
	//���ּ�7 ���ּ�8 ���ּ�9  ��
//���ּ�1 ���ּ�2 ���ּ�3 ��λ��
uint8 KeySta[4][4] = {  
    {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};//ȫ�����󰴼��ĵ�ǰ״̬

/*����������������ⰴ��������������Ӧ����������������ѭ���е���*/
void KeyDriver()
{
    uint8 i, j;
    static uint8 backup[4][4] = {  //����ֵ���ݣ�����ǰһ�ε�ֵ
        {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
    };
    
    for (i=0; i<4; i++)  //ѭ�����4*4�ľ��󰴼� ,i����out,j����In
    {
        for (j=0; j<4; j++)
        {
            if (backup[i][j] != KeySta[i][j])    //��ⰴ������
            {
                if (backup[i][j] == 0)           //��������ʱִ�ж���
                {
									  
                    KeyAction(KeyCodeMap[i][j]); //���ð�����������
                }
                backup[i][j] = KeySta[i][j];     //ˢ��ǰһ�α���ֵ
            }
        }
    }
}
/* ����ɨ�躯��,��Ҫ�ڶ�ʱ�ж��е���,�Ƽ����ü��1ms */
void KeyScan()
{
    uint8 i;
    static uint8 keyout = 0;   //���󰴼�ɨ���������
    static uint8 keybuf[4][4] = {  //���󰴼�ɨ�軺����
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF}
    };

    //��һ�е�4������ֵ���뻺����
    keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;
    keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
    keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
    keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;
    //��������°���״̬	
    for (i=0; i<4; i++)  //ÿ��4������,����ѭ��4��
    {
        if ((keybuf[keyout][i] & 0x0F) == 0x00)
        {   //����4��ɨ��ֵΪ0,��4*4ms�ڶ��ǰ���״̬ʱ,������Ϊ�������ȶ��İ���
             KeySta[keyout][i] = 0;
        }
        else if ((keybuf[keyout][i] & 0x0F) == 0x0F)
        {   //����4��ɨ��ֵΪ1,��4*4ms�ڶ��ǵ���״̬ʱ,������Ϊ�������ȶ��ĵ���
             KeySta[keyout][i] = 1;
        }
    }
    //ִ����һ�ε�ɨ�����
    keyout++;        //����������� 
    keyout &= 0x03;  //����������ӵ�4������ 
    switch (keyout)  //��������ֵ,�ͷŵ�ǰ�������,�����´ε��������
    {
        case 0: KEY_OUT_4 = 1; KEY_OUT_1 = 0; break;
        case 1: KEY_OUT_1 = 1; KEY_OUT_2 = 0; break;
        case 2: KEY_OUT_2 = 1; KEY_OUT_3 = 0; break;
        case 3: KEY_OUT_3 = 1; KEY_OUT_4 = 0; break;
        default: break;
    }
}

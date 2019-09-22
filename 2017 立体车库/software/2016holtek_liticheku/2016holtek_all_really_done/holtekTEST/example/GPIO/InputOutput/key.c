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
	};//矩阵按键编号到标准键盘的映射表
    
	
	//数字键10 数字键11 数字键12
	//数字键4 数字键5 数字键6  取
	//数字键7 数字键8 数字键9  存
//数字键1 数字键2 数字键3 复位键
uint8 KeySta[4][4] = {  
    {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};//全部矩阵按键的当前状态

/*按键驱动函数，检测按键动作，调度相应动作函数，需在主循环中调用*/
void KeyDriver()
{
    uint8 i, j;
    static uint8 backup[4][4] = {  //按键值备份，保存前一次的值
        {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
    };
    
    for (i=0; i<4; i++)  //循环检测4*4的矩阵按键 ,i代表out,j代表In
    {
        for (j=0; j<4; j++)
        {
            if (backup[i][j] != KeySta[i][j])    //检测按键动作
            {
                if (backup[i][j] == 0)           //按键按下时执行动作
                {
									  
                    KeyAction(KeyCodeMap[i][j]); //调用按键动作函数
                }
                backup[i][j] = KeySta[i][j];     //刷新前一次备份值
            }
        }
    }
}
/* 按键扫描函数,需要在定时中断中调用,推荐调用间隔1ms */
void KeyScan()
{
    uint8 i;
    static uint8 keyout = 0;   //矩阵按键扫描输出索引
    static uint8 keybuf[4][4] = {  //矩阵按键扫描缓冲区
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF}
    };

    //将一行的4个按键值移入缓冲区
    keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;
    keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
    keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
    keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;
    //消抖后更新按键状态	
    for (i=0; i<4; i++)  //每行4个按键,所以循环4次
    {
        if ((keybuf[keyout][i] & 0x0F) == 0x00)
        {   //连续4次扫描值为0,即4*4ms内都是按下状态时,可以认为按键已稳定的按下
             KeySta[keyout][i] = 0;
        }
        else if ((keybuf[keyout][i] & 0x0F) == 0x0F)
        {   //连续4次扫描值为1,即4*4ms内都是弹起状态时,可以认为按键已稳定的弹起
             KeySta[keyout][i] = 1;
        }
    }
    //执行下一次的扫描输出
    keyout++;        //输出索引递增 
    keyout &= 0x03;  //输出索引增加到4即归零 
    switch (keyout)  //根据索引值,释放当前输出引脚,拉低下次的输出引脚
    {
        case 0: KEY_OUT_4 = 1; KEY_OUT_1 = 0; break;
        case 1: KEY_OUT_1 = 1; KEY_OUT_2 = 0; break;
        case 2: KEY_OUT_2 = 1; KEY_OUT_3 = 0; break;
        case 3: KEY_OUT_3 = 1; KEY_OUT_4 = 0; break;
        default: break;
    }
}

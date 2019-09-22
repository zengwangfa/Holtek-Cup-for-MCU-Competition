#include "zh_key.h"


ZH_KeyStatus volatile ZH_Key_s[ZH_KEYNMAX];


/*******************************************************************************
函数名   ：void ZH_Key_Init
功能     ：矩阵键盘初始化函数
输入参数 ：无
返回值   ：无
完成时间 ：2017.03.14
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_Key_Init(void)
{
  /***************** IN **************/
  AFIO_GPxConfig(KEY_R1_AFIO_PORT, KEY_R1_AFIO_PIN, ZH_KEY_AFIO_MODE);
//	GPIO_PullResistorConfig(KEY_R1_PORT, KEY_R1_PIN,GPIO_PR_UP);
  GPIO_DirectionConfig(KEY_R1_PORT, KEY_R1_PIN, GPIO_DIR_IN);
	GPIO_InputConfig(KEY_R1_PORT, KEY_R1_PIN, ENABLE);
  
  AFIO_GPxConfig(KEY_R2_AFIO_PORT, KEY_R2_AFIO_PIN, ZH_KEY_AFIO_MODE);
//	GPIO_PullResistorConfig(KEY_R2_PORT, KEY_R2_PIN,GPIO_PR_UP);
  GPIO_DirectionConfig(KEY_R2_PORT, KEY_R2_PIN, GPIO_DIR_IN);
	GPIO_InputConfig(KEY_R2_PORT, KEY_R2_PIN, ENABLE);
  
  AFIO_GPxConfig(KEY_R3_AFIO_PORT, KEY_R3_AFIO_PIN, ZH_KEY_AFIO_MODE);
//	GPIO_PullResistorConfig(KEY_R3_PORT, KEY_R3_PIN,GPIO_PR_UP);
  GPIO_DirectionConfig(KEY_R3_PORT, KEY_R3_PIN, GPIO_DIR_IN);
	GPIO_InputConfig(KEY_R3_PORT, KEY_R3_PIN, ENABLE);
  
  AFIO_GPxConfig(KEY_R4_AFIO_PORT, KEY_R4_AFIO_PIN, ZH_KEY_AFIO_MODE);
//	GPIO_PullResistorConfig(KEY_R4_PORT, KEY_R4_PIN,GPIO_PR_UP);
  GPIO_DirectionConfig(KEY_R4_PORT, KEY_R4_PIN, GPIO_DIR_IN);
	GPIO_InputConfig(KEY_R4_PORT, KEY_R4_PIN, ENABLE);
  
  /***************** OUT **************/
  AFIO_GPxConfig(KEY_C1_AFIO_PORT, KEY_C1_AFIO_PIN, ZH_KEY_AFIO_MODE);
  GPIO_DirectionConfig(KEY_C1_PORT, KEY_C1_PIN, GPIO_DIR_OUT);
  GPIO_SetOutBits(KEY_C1_PORT, KEY_C1_PIN);
  
  AFIO_GPxConfig(KEY_C2_AFIO_PORT, KEY_C2_AFIO_PIN, ZH_KEY_AFIO_MODE);
  GPIO_DirectionConfig(KEY_C2_PORT, KEY_C2_PIN, GPIO_DIR_OUT);
  GPIO_SetOutBits(KEY_C2_PORT, KEY_C2_PIN);
  
  AFIO_GPxConfig(KEY_C3_AFIO_PORT, KEY_C3_AFIO_PIN, ZH_KEY_AFIO_MODE);
  GPIO_DirectionConfig(KEY_C3_PORT, KEY_C3_PIN, GPIO_DIR_OUT);
  GPIO_SetOutBits(KEY_C3_PORT, KEY_C3_PIN);
//  
//  AFIO_GPxConfig(KEY_C4_AFIO_PORT, KEY_C4_AFIO_PIN, ZH_KEY_AFIO_MODE);
//  GPIO_DirectionConfig(KEY_C4_PORT, KEY_C4_PIN, GPIO_DIR_OUT);
//  GPIO_SetOutBits(KEY_C4_PORT, KEY_C4_PIN);
}

/*******************************************************************************
函数名   ：void ZH_Key_WriteOutBit
功能     ：矩阵键盘某行输出电平控制
输入参数 ：KEY：要控制的是哪一行，参数（ZH_KEYCn）   ; 
           Status : SET/RESET 输出高或低
返回值   ：无
完成时间 ：2017.03.14
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_Key_WriteOutBit(ZH_KEY_C_Enum KEY, FlagStatus Status)
{
  switch (KEY)
  {
    case ZH_KEYC1:
    {
      GPIO_WriteOutBits(KEY_C1_PORT, KEY_C1_PIN, Status);
      break;
    }
    case ZH_KEYC2:
    {
      GPIO_WriteOutBits(KEY_C2_PORT, KEY_C2_PIN, Status);
      break;
    }
    case ZH_KEYC3:
    {
      GPIO_WriteOutBits(KEY_C3_PORT, KEY_C3_PIN, Status);
      break;
    }
//    case ZH_KEYC4:
//    {
//      GPIO_WriteOutBits(KEY_C4_PORT, KEY_C4_PIN, Status);
//      break;
//    }
    default:	break;
  }
}


/*******************************************************************************
函数名   ：void ZH_Key_WriteOutC
功能     ：矩阵键盘扫描行输出电平控制
输入参数 ：KEY：要扫描的是哪一行，参数（ZH_KEYCn）   ; 
           Status : SET/RESET 该行输出高或低，其它行则反之
返回值   ：无
完成时间 ：2017.03.14
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_Key_WriteOutC(ZH_KEY_C_Enum KEY, FlagStatus Status)
{
  ZH_KEY_C_Enum i;
  for(i=ZH_KEYC1;i<ZH_KEYCMAX;i++)
  {
    if(i==KEY)
      ZH_Key_WriteOutBit(i, Status);
    else
      ZH_Key_WriteOutBit(i, !Status);
  }
}

/***********************************

***********************************/
/*******************************************************************************
函数名   ：void ZH_Key_WriteOutC
功能     ：矩阵键盘行扫描检测实时更新按键状态,
           将最终的按键状态存于 ZH_Key_s 数组中,
           每个按键有3个状态,分别为：
               RELEASE-->释放或者没按下,
               PRESS-->第一次检测按下到时,只有一次,但不会漏检,
               KEEP-->保持.
输入参数 ：无
返回值   ：无
完成时间 ：2017.03.14
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_Key_JianCe(void)
{
  ZH_KEY_C_Enum keyC;
  u8 key_num = 0;
  
  for(keyC = ZH_KEYC1;keyC<ZH_KEYCMAX;keyC++)
  {
    ZH_Key_WriteOutC(keyC, RESET);
    
    if(GPIO_ReadInBit(KEY_R1_PORT,KEY_R1_PIN) == RESET)
    {
      if(ZH_Key_s[key_num])
        ZH_Key_s[key_num] = KEEP;
      else
        ZH_Key_s[key_num] = PRESS;
    }
    else
      ZH_Key_s[key_num] = RELEASE;
    key_num++;
    
    if(GPIO_ReadInBit(KEY_R2_PORT,KEY_R2_PIN) == RESET)
    {
      if(ZH_Key_s[key_num])
        ZH_Key_s[key_num] = KEEP;
      else
        ZH_Key_s[key_num] = PRESS;
    }
    else
      ZH_Key_s[key_num] = RELEASE;
    key_num++;
    
    if(GPIO_ReadInBit(KEY_R3_PORT,KEY_R3_PIN) == RESET)
    {
      if(ZH_Key_s[key_num])
        ZH_Key_s[key_num] = KEEP;
      else
        ZH_Key_s[key_num] = PRESS;
    }
    else
      ZH_Key_s[key_num] = RELEASE;
    key_num++;
    
    if(GPIO_ReadInBit(KEY_R4_PORT,KEY_R4_PIN) == RESET)
    {
      if(ZH_Key_s[key_num])
        ZH_Key_s[key_num] = KEEP;
      else
        ZH_Key_s[key_num] = PRESS;
    }
    else
      ZH_Key_s[key_num] = RELEASE;
    key_num++;
  }
  
}















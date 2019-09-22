#include "ZH_BFTM.h"


/*******************************************************************************
函数名   ：void ZH_BFTM_Init
功能     ：BFTM初始化函数
输入参数 ：无
返回值   ：无
完成时间 ：2017.03.14
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_BFTM_Init(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  
  ZH_BFTM_CLK(CKCUClock)      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
  
  BFTM_SetCompare(ZH_BFTM_N, ZH_BFTM_CMP);
  BFTM_SetCounter(ZH_BFTM_N, ZH_BFTM_CNTR);
  BFTM_IntConfig(ZH_BFTM_N, ZH_BFTM_MIEN);
  BFTM_OneShotModeCmd(ZH_BFTM_N, ZH_BFTM_OSM);
  BFTM_EnaCmd(ZH_BFTM_N, ZH_BFTM_CEN);
  
  #if (ZH_BFTM_IRQEN == ENABLE)
  NVIC_EnableIRQ(ZH_BFTM_IRQ);
  #else
  NVIC_DisableIRQ(ZH_BFTM_IRQ);
  #endif
}


/*******************************************************************************
函数名   ：void ZH_BFTM_SetCMP
功能     ：BFTM 定时器使能控制
输入参数 ：Num： 计数器的比较值
返回值   ：无
完成时间 ：2017.03.14
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_BFTM_SetCMP(u32 Num)
{
  BFTM_SetCompare(ZH_BFTM_N, Num);
  BFTM_IntConfig(ZH_BFTM_N, ZH_BFTM_MIEN);
  BFTM_SetCounter(ZH_BFTM_N, ZH_BFTM_CNTR);
}

/*******************************************************************************
函数名   ：void ZH_BFTM_Switch
功能     ：BFTM 定时器使能控制
输入参数 ：NewState： ENABLE or DISABLE.
返回值   ：无
完成时间 ：2017.03.14
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_BFTM_Switch(ControlStatus NewState)
{
  BFTM_EnaCmd(ZH_BFTM_N, NewState);
}

























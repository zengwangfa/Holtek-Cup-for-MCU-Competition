/* Includes ------------------------------------------------------------------------------------------------*/
#include "ZH_GPTM.h"


ZH_GPTM_TypeDef ZH_gGPTM;


/*******************************************************************************
函数名   ：void ZH_GPTM_Init
功能     ：GPTM初始化函数
输入参数 ：无
返回值   ：无
完成时间 ：2017.03.13
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_GPTM_Init(void)
{
	GPTM_TimeBaseInitTypeDef TimeBaseInit;
  GPTM_OutputInitTypeDef Output_Init;
  
  AFIO_GPxConfig(ZH_GPTM_GPIO_ID, ZH_GPTM_AFIO_PIN, ZH_GPTM_AFIO_MODE);
  
  ZH_gGPTM.pscr = ZH_GPTM_PSCR_PSCV;
  ZH_gGPTM.crr = ZH_GPTM_CRR_CRV;
  ZH_gGPTM.ccr = ZH_GPTM_CCR_CCV;
  
  TimeBaseInit.CounterMode 			  = ZH_GPTM_CNTCFR_CMSEL;
  TimeBaseInit.CounterReload 		  = ZH_gGPTM.crr;
  TimeBaseInit.Prescaler 				  = ZH_gGPTM.pscr;
  TimeBaseInit.PSCReloadTime 		  = ZH_GPTM_EVGR_UEVG;
  GPTM_TimeBaseInit(ZH_GPTM_N, &TimeBaseInit);
  ZH_gGPTM.clock = SystemCoreClock/(ZH_gGPTM.pscr+1);
  
  Output_Init.Channel 						= ZH_GPTM_CH;
  Output_Init.Compare 						= ZH_gGPTM.ccr;
  Output_Init.Control 						= ZH_GPTM_CHCTR;
  Output_Init.OutputMode 					= ZH_GPTM_CHNOCFR_OM;
  Output_Init.Polarity 						= ZH_GPTM_CHPOLR;
  GPTM_OutputInit(ZH_GPTM_N, &Output_Init);
  
  GPTM_Cmd(ZH_GPTM_N, DISABLE);
}

/*******************************************************************************
函数名   ：void ZH_GPTM_CCR_Num
功能     ：修改GPTM 的CCR寄存器的值
输入参数 ：Num：CCR的值，控制占空比 范围：0~ZH_GPTM_CRR_CRV
返回值   ：无
完成时间 ：2017.03.13
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_GPTM_CCR_Num(u16 Num)
{
  if(Num<ZH_gGPTM.crr)
    ZH_GPTM_CCR = Num;
  else
    ZH_GPTM_CCR = ZH_gGPTM.crr;
  
  ZH_gGPTM.ccr = ZH_GPTM_CCR;
}

/*******************************************************************************
函数名   ：void ZH_GPTM_CRR_Num
功能     ：修改GPTM 的CCR寄存器的值
输入参数 ：Num：CRR的值，控制占空比 范围：0~ZH_GPTM_CRR_CRV
返回值   ：无
完成时间 ：2017.03.13
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_GPTM_CRR_Num(u16 Num)
{
  ZH_GPTM_N->CRR = Num;
  ZH_gGPTM.crr = Num;
}

/*******************************************************************************
函数名   ：void ZH_GPTM_Switch
功能     ：GPTM 定时器使能控制
输入参数 ：NewState： ENABLE or DISABLE.
返回值   ：无
完成时间 ：2017.03.13
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_GPTM_Switch(ControlStatus NewState)
{
  GPTM_Cmd(ZH_GPTM_N, NewState);
}

/*******************************************************************************
函数名   ：void ZH_GPTMCH_Switch
功能     ：GPTM 定时器通道使能控制
输入参数 ：NewState： ENABLE or DISABLE.
返回值   ：无
完成时间 ：2017.03.13
作者     ：Kaven
版本     ：第一版
修改时间 ：
********************************************************************************/
void ZH_GPTMCH_Switch(ControlStatus NewState)
{
  ZH_GPTM_N->CHCTR &= ~(u32)(0x1 << (ZH_GPTM_CH << 1));
  ZH_GPTM_N->CHCTR |= NewState<<(ZH_GPTM_CH << 1);
}












#include "ZH_BFTM.h"


/*******************************************************************************
������   ��void ZH_BFTM_Init
����     ��BFTM��ʼ������
������� ����
����ֵ   ����
���ʱ�� ��2017.03.14
����     ��Kaven
�汾     ����һ��
�޸�ʱ�� ��
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
������   ��void ZH_BFTM_SetCMP
����     ��BFTM ��ʱ��ʹ�ܿ���
������� ��Num�� �������ıȽ�ֵ
����ֵ   ����
���ʱ�� ��2017.03.14
����     ��Kaven
�汾     ����һ��
�޸�ʱ�� ��
********************************************************************************/
void ZH_BFTM_SetCMP(u32 Num)
{
  BFTM_SetCompare(ZH_BFTM_N, Num);
  BFTM_IntConfig(ZH_BFTM_N, ZH_BFTM_MIEN);
  BFTM_SetCounter(ZH_BFTM_N, ZH_BFTM_CNTR);
}

/*******************************************************************************
������   ��void ZH_BFTM_Switch
����     ��BFTM ��ʱ��ʹ�ܿ���
������� ��NewState�� ENABLE or DISABLE.
����ֵ   ����
���ʱ�� ��2017.03.14
����     ��Kaven
�汾     ����һ��
�޸�ʱ�� ��
********************************************************************************/
void ZH_BFTM_Switch(ControlStatus NewState)
{
  BFTM_EnaCmd(ZH_BFTM_N, NewState);
}

























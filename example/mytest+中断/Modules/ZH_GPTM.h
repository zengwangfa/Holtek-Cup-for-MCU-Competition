/* ��Ҫ������Kaven�������ޣ�������˱��������������ģ��Ų��������Է���ǰ���Ƚ��в��ԡ� */
#ifndef __ZH_GPTM_H
#define __ZH_GPTM_H

#include "ht32.h"


#define STRCAT2_(a, b)       a##b
#define STRCAT2(a, b)        STRCAT2_(a, b)
#define STRCAT3_(a, b, c)    a##b##c
#define STRCAT3(a, b, c)     STRCAT3_(a, b, c)


#define ZH_GPTM_N                      HT_GPTM0
#define ZH_GPTM_CLK(CK)                (CK.Bit.GPTM0)
#define ZH_GPTMCH                      3
#define ZH_GPTM_PORT                   A
#define ZH_GPTM_PIN                    7

#define ZH_GPTM_CNTCFR_CMSEL            GPTM_CNT_MODE_UP
#define ZH_GPTM_CHPOLR                  GPTM_CHP_NONINVERTED      // ͨ����׽�����Ƚ�������ԵĿ��� �� ������Ϊ����ʱ����׽�¼�������ͨ�������� �� ������Ϊ���ʱ��ͨ������ߵ�ƽ��Ч
#define ZH_GPTM_CHNOCFR_OM              GPTM_OM_PWM1              // ͨ�����ģʽ���ã�ѡ��Ƚ�ƥ��ʱ��ת�����и���Ƶ�ʱ仯��  �� ѡ��PWM ģʽ 1  �� �����ϼ����ڼ䣬�� CNTR < CCR��ͨ��������Ч��ƽ�����򽫴�����Ч��ƽ�� �����¼����ڼ䣬�� CNTR > CCR��ͨ��������Ч��ƽ�����򽫴�����Ч��ƽ��
#define ZH_GPTM_CHCTR                   GPTM_CHCTL_ENABLE         // ͨ������  ; ͨ��ʹ�ܣ�����Ϊ����ʱ���벶׽ģʽʹ�ܣ�����Ϊ���ʱ����ź�����Ӧ����Ų���
#define ZH_GPTM_EVGR_UEVG               GPTM_PSC_RLD_IMMEDIATE    // �����¼����� ; 0���޶��� , 1�����ؼ����� 

#define ZH_GPTM_CH                     STRCAT2(GPTM_CH_,  ZH_GPTMCH)
#define ZH_GPTM_CCR                    STRCAT3(ZH_GPTM_N->CH, ZH_GPTMCH, CCR)
#define ZH_GPTM_GPIO_ID                STRCAT2(GPIO_P,  ZH_GPTM_PORT)
#define ZH_GPTM_AFIO_PIN               STRCAT2(AFIO_PIN_, ZH_GPTM_PIN)

#define ZH_GPTM_AFIO_MODE               AFIO_MODE_4

typedef struct 
{
  u16 pscr;
  u32 clock;
  u16 crr;
  u16 ccr;
} ZH_GPTM_TypeDef;
extern ZH_GPTM_TypeDef ZH_gGPTM;

#define ZH_GPTM_PSCR_PSCV              71                       // Ԥ��Ƶ����ֵ  ; 16-bit ��������ʱ��Ƶ�� f(CK_CNT) =  Ԥ��Ƶ��ʱ��Դf(CK_PSC) / (PSCV[15:0] + 1) = 72M / (71 + 1) = 1M
#define ZH_GPTM_CRR_CRV                1000                     // ����������ֵ  ; 16-bit   f(PWM) = f(CK_CNT) / ZH_GPTM_CRR_CRV = 1K Hz
#define ZH_GPTM_CCR_CCV                0                       // ��׽ / �Ƚ�ֵ  ; 16-bit  ռ�ձ��� 0/100


void ZH_GPTM_Init(void);                              // GPTM��ʼ������
void ZH_GPTM_CCR_Num(u16 Num);                        // �޸�GPTM ��CCR�Ĵ�����ֵ  �ɸı�ռ�ձȣ���Χ��0~ZH_GPTM_CRR_CRV
void ZH_GPTM_CRR_Num(u16 Num);                        // �޸�GPTM ��CRR�Ĵ�����ֵ
void ZH_GPTM_Switch(ControlStatus NewState);          // GPTM ��ʱ��ʹ�ܿ���
void ZH_GPTMCH_Switch(ControlStatus NewState);        // GPTM ��ʱ��ͨ��ʹ�ܿ���


#endif


/* 重要声明：Kaven技术有限，如果用了本代码而出现问题的，概不负责。所以发布前请先进行测试。 */
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
#define ZH_GPTM_CHPOLR                  GPTM_CHP_NONINVERTED      // 通道捕捉输入或比较输出极性的控制 ； （配置为输入时）捕捉事件发生在通道上升沿 或 （配置为输出时）通道输出高电平有效
#define ZH_GPTM_CHNOCFR_OM              GPTM_OM_PWM1              // 通道输出模式设置（选择比较匹配时翻转可以有更多频率变化）  ； 选择PWM 模式 1  ： 在向上计数期间，当 CNTR < CCR，通道处于有效电平，否则将处于无效电平。 在向下计数期间，当 CNTR > CCR，通道处于无效电平，否则将处于有效电平。
#define ZH_GPTM_CHCTR                   GPTM_CHCTL_ENABLE         // 通道控制  ; 通道使能，配置为输入时输入捕捉模式使能，配置为输出时输出信号在相应输出脚产生
#define ZH_GPTM_EVGR_UEVG               GPTM_PSC_RLD_IMMEDIATE    // 更新事件发生 ; 0：无动作 , 1：重载计数器 

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

#define ZH_GPTM_PSCR_PSCV              71                       // 预分频器的值  ; 16-bit 计数器的时钟频率 f(CK_CNT) =  预分频器时钟源f(CK_PSC) / (PSCV[15:0] + 1) = 72M / (71 + 1) = 1M
#define ZH_GPTM_CRR_CRV                1000                     // 计数器重载值  ; 16-bit   f(PWM) = f(CK_CNT) / ZH_GPTM_CRR_CRV = 1K Hz
#define ZH_GPTM_CCR_CCV                0                       // 捕捉 / 比较值  ; 16-bit  占空比是 0/100


void ZH_GPTM_Init(void);                              // GPTM初始化函数
void ZH_GPTM_CCR_Num(u16 Num);                        // 修改GPTM 的CCR寄存器的值  可改变占空比，范围：0~ZH_GPTM_CRR_CRV
void ZH_GPTM_CRR_Num(u16 Num);                        // 修改GPTM 的CRR寄存器的值
void ZH_GPTM_Switch(ControlStatus NewState);          // GPTM 定时器使能控制
void ZH_GPTMCH_Switch(ControlStatus NewState);        // GPTM 定时器通道使能控制


#endif


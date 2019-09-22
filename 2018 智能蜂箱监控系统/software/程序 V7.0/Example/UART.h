#ifndef _UART_H_
#define _UART_H_

#include "INCLUDE.h"
// 串口0-USART0   引脚定义配置


//----------------------定义蜂箱指令信息---------------------------------//
#define FX_NUMBER          0x0001                      //蜂箱编号

#define SET_WASHING        0x01                         //清洗蜂箱
#define SET_FEED           0x02                         //喂食蜜蜂
#define SET_TEMPTURE       0x03                         //设置温度，加热丝加热
#define RETRUN_MESSAGE     0x04                         //返回蜂箱的信息
//#define RETURN_TEMPTURE  0x05                         //返回蜂箱温度值
#define CONTR_BEE_STATE1   0x06                         //开口打开最大全部蜜蜂都可通过
#define CONTR_BEE_STATE2   0x07                         //蜂王禁止通行
#define CONTR_BEE_STATE3   0x08                         //蜂王禁止通行且开口最小


void USART_Sends(HT_USART_TypeDef* USARTx,u8 *Data,u16 len);//发送字符串
void Uart0DataAnalyzing(void);

#endif

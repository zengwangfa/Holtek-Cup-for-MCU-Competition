#ifndef _UART_H_
#define _UART_H_

#include "INCLUDE.h"
// ����0-USART0   ���Ŷ�������


//----------------------�������ָ����Ϣ---------------------------------//
#define FX_NUMBER          0x0001                      //������

#define SET_WASHING        0x01                         //��ϴ����
#define SET_FEED           0x02                         //ιʳ�۷�
#define SET_TEMPTURE       0x03                         //�����¶ȣ�����˿����
#define RETRUN_MESSAGE     0x04                         //���ط������Ϣ
//#define RETURN_TEMPTURE  0x05                         //���ط����¶�ֵ
#define CONTR_BEE_STATE1   0x06                         //���ڴ����ȫ���۷䶼��ͨ��
#define CONTR_BEE_STATE2   0x07                         //������ֹͨ��
#define CONTR_BEE_STATE3   0x08                         //������ֹͨ���ҿ�����С


void USART_Sends(HT_USART_TypeDef* USARTx,u8 *Data,u16 len);//�����ַ���
void Uart0DataAnalyzing(void);

#endif

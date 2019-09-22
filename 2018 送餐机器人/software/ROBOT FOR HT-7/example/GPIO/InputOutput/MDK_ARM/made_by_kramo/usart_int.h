
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __USART_INT_H
#define __USART_INT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/* Exported types ------------------------------------------------------------------------------------------*/
typedef enum
{
  USART_TX_INT = 1,
  USART_RX_INT = 2,
  USART_ALL_INT = 3
}USART_INT_Mode;

/* Exported constants --------------------------------------------------------------------------------------*/
#define USART_BUFSIZE   128

extern u8 UR0TxBuf[USART_BUFSIZE];
extern u8 UR0RxBuf[USART_BUFSIZE];
extern u32 UR0TxReadIndex;
extern vu32 UR0TxWriteIndex;
extern u32 UR0RxReadIndex;
extern vu32 UR0RxWriteIndex;
extern u8 UR1TxBuf[USART_BUFSIZE];
extern u8 UR1RxBuf[USART_BUFSIZE];
extern u32 UR1TxReadIndex;
extern vu32 UR1TxWriteIndex;
extern u32 UR1RxReadIndex;
extern vu32 UR1RxWriteIndex;

/* Exported macro ------------------------------------------------------------------------------------------*/
#define IS_BUFFER_FULL(ridx, widx)    ((widx + 1) % USART_BUFSIZE == ridx)
#define IS_BUFFER_EMPTY(ridx, widx)   (widx == ridx)

/* Exported functions --------------------------------------------------------------------------------------*/
void USART_Interrupt_Init(HT_USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStructure, USART_INT_Mode Mode);

/*user-define */
//void PCexchange(void);
//void showgarage(u8 fu,u8 em);
//void storage(void);
//void random_data(u8 w);//生成随机数
//void tack(void);
//void showpassword(u8 b);
void USART_INT(void);
void usart0_init(void);
void usart1_init(void);


#endif /* __USART_INT_H ------------------------------------------------------------------------------------*/

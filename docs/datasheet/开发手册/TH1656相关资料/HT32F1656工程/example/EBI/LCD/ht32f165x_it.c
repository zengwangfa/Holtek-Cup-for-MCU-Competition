#include "ht32.h"
#include "ht32_board.h"
#include "usart_int.h"
#define USART_TIMEOUT_ON          ((u32)0x00000080)
/* USART0 definitions                                                                                       */
u8 UR0TxBuf[USART_BUFSIZE];
u8 UR0RxBuf[USART_BUFSIZE];
u32 UR0TxReadIndex = 0;
vu32 UR0TxWriteIndex = 0;
u32 UR0RxReadIndex = 0;
vu32 UR0RxWriteIndex = 0;

/* USART1 definitions                                                                                       */
u8 UR1TxBuf[USART_BUFSIZE];
u8 UR1RxBuf[USART_BUFSIZE];
u32 UR1TxReadIndex = 0;
vu32 UR1TxWriteIndex = 0;
u32 UR1RxReadIndex = 0;
vu32 UR1RxWriteIndex = 0;

void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Memory Manage exception.
 * @retval  None
 ************************************************************************************************************/
void MemManage_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Bus Fault exception.
 * @retval  None
 ************************************************************************************************************/
void BusFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Usage Fault exception.
 * @retval  None
 ************************************************************************************************************/
void UsageFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Debug Monitor exception.
 * @retval  None
 ************************************************************************************************************/
void DebugMon_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}



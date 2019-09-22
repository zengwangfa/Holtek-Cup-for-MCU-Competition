/*********************************************************************************************************//**
 * @file    GPIO/InputOutput/ht32f165x_it.c
 * @version $Rev:: 929          $
 * @date    $Date:: 2015-09-16 #$
 * @brief   This file provides all interrupt service routine.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "config.h"
#include "ht32_board.h"
#include "key.h"
#include "usart_int.h"

/* user-define*/
extern u8 f;
extern u8 t;
/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup GPIO_Examples GPIO
  * @{
  */

/** @addtogroup InputOutput
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
//void NMI_Handler(void)
//{
//}

///*********************************************************************************************************//**
// * @brief   This function handles Hard Fault exception.
// * @retval  None
// ************************************************************************************************************/
//void HardFault_Handler(void)
//{
//  while (1);
//}

///*********************************************************************************************************//**
// * @brief   This function handles Memory Manage exception.
// * @retval  None
// ************************************************************************************************************/
//void MemManage_Handler(void)
//{
//  while (1);
//}

///*********************************************************************************************************//**
// * @brief   This function handles Bus Fault exception.
// * @retval  None
// ************************************************************************************************************/
//void BusFault_Handler(void)
//{
//  while (1);
//}

///*********************************************************************************************************//**
// * @brief   This function handles Usage Fault exception.
// * @retval  None
// ************************************************************************************************************/
//void UsageFault_Handler(void)
//{
//  while (1);
//}

///*********************************************************************************************************//**
// * @brief   This function handles SVCall exception.
// * @retval  None
// ************************************************************************************************************/
//void SVC_Handler(void)
//{
//}

///*********************************************************************************************************//**
// * @brief   This function handles Debug Monitor exception.
// * @retval  None
// ************************************************************************************************************/
//void DebugMon_Handler(void)
//{
//}

///*********************************************************************************************************//**
// * @brief   This function handles PendSVC exception.
// * @retval  None
// ************************************************************************************************************/
//void PendSV_Handler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
}
void BFTM1_IRQHandler(void)
{
	  t++;
	  HT32F_DVB_LEDToggle(HT_LED1);		//��ת��ƽ�ĺ���

   	BFTM_ClearFlag(HT_BFTM1);
  
}
void BFTM0_IRQHandler(void)
{
  //HT32F_DVB_LEDToggle(HT_LED2);
	f++;
	if(f == 7)
		f = 0;
	KeyScan();
	BFTM_ClearFlag(HT_BFTM0);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

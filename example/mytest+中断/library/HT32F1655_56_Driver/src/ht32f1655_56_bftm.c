/*********************************************************************************************************//**
 * @file    ht32f1655_56_bftm.c
 * @version V1.00
 * @date    2014-06-30
 * @brief   This file provides all the BFTM firmware functions.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1655_56_bftm.h"

/** @addtogroup HT32F1655_56_Peripheral_Driver HT32F1655/1656 Peripheral Driver
  * @{
  */

/** @defgroup BFTM BFTM
  * @brief BFTM driver modules
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/
/** @defgroup BFTM_Exported_Functions BFTM exported functions
  * @{
  */
/*********************************************************************************************************//**
 * @brief Deinitialize the specified BFTM registers to their default values.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @retval None
 ************************************************************************************************************/
void BFTM_DeInit(HT_BFTM_TypeDef* BFTMx)
{
  RSTCU_PeripReset_TypeDef RSTCUReset = {{0}};

  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));

  if (BFTMx == HT_BFTM0)
  {
    RSTCUReset.Bit.BFTM0 = 1;
  }
  else
  {
    RSTCUReset.Bit.BFTM1 = 1;
  }
  RSTCU_PeripReset(RSTCUReset, ENABLE);
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the specified BFTM.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @param NewState: new state of the BFTMx peripheral.
 *        This parameter can be: ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void BFTM_EnaCmd(HT_BFTM_TypeDef* BFTMx, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    BFTMx->CR |= (1UL << 2);
  }
  else
  {
    BFTMx->CR &= ~(1UL << 2);
  }
}

/*********************************************************************************************************//**
 * @brief Configure the CMP register value of the specified BFTM.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @param wCompare: Specify a value to the CMP register.
 * @retval None
 ************************************************************************************************************/
void BFTM_SetCompare(HT_BFTM_TypeDef* BFTMx, u32 wCompare)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));

  BFTMx->CMP = wCompare;
}

/*********************************************************************************************************//**
 * @brief Get the CMP register value of the specified BFTM.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @retval The value of the CMP register
 ************************************************************************************************************/
u32 BFTM_GetCompare(HT_BFTM_TypeDef* BFTMx)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));

  return BFTMx->CMP;
}

/*********************************************************************************************************//**
 * @brief Set the CNTR register value of the specified BFTM.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @param wCounter: Specify a new value to the CNTR register.
 * @retval None
 ************************************************************************************************************/
void BFTM_SetCounter(HT_BFTM_TypeDef* BFTMx, u32 wCounter)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));

  BFTMx->CNTR = wCounter;
}

/*********************************************************************************************************//**
 * @brief Get the CNTR register value of the specified BFTM.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @retval The value of the CNTR register
 ************************************************************************************************************/
u32 BFTM_GetCounter(HT_BFTM_TypeDef* BFTMx)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));

  return BFTMx->CNTR;
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the one shot mode of the specified BFTM.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @param NewState: new state of the BFTMx one shot mode.
 *        This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void BFTM_OneShotModeCmd(HT_BFTM_TypeDef* BFTMx, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    BFTMx->CR |= (1UL << 1);
  }
  else
  {
    BFTMx->CR &= ~(1UL << 1);
  }
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the specified BFTM interrupt.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @param NewState: new state of the BFTMx interrupt.
 *        This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void BFTM_IntConfig(HT_BFTM_TypeDef* BFTMx, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    BFTMx->CR |= BFTM_INT_MATCH;
  }
  else
  {
    BFTMx->CR &= ~BFTM_INT_MATCH;
  }
}

/*********************************************************************************************************//**
 * @brief Get the flag status of the specified BFTM.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @retval SET or RESET
 ************************************************************************************************************/
FlagStatus BFTM_GetFlagStatus(HT_BFTM_TypeDef* BFTMx)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));

  if (BFTMx->SR & BFTM_FLAG_MATCH)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*********************************************************************************************************//**
 * @brief Clear the interrupt flag of the specified BFTM.
 * @param BFTMx: where the BFTMx is the selected BFTM peripheral, x can be 0 or 1.
 * @retval None
 ************************************************************************************************************/
void BFTM_ClearFlag(HT_BFTM_TypeDef* BFTMx)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_BFTM(BFTMx));

  BFTMx->SR &= ~BFTM_FLAG_MATCH;

  /*--------------------------------------------------------------------------------------------------------*/
  /* DSB instruction is added in this function to ensure the write operation which is for clearing interrupt*/
  /* flag is actually completed before exiting ISR. It prevents the NVIC from detecting the interrupt again */
  /* since the write register operation may be pended in the internal write buffer of Cortex-M3 when program*/
  /* has exited interrupt routine. This DSB instruction may be masked if this function is called in the     */
  /* beginning of ISR and there are still some instructions before exiting ISR.                             */
  /*--------------------------------------------------------------------------------------------------------*/
  __DSB();
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

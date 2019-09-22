/*********************************************************************************************************//**
 * @file    ht32f1655_56_bftm.h
 * @version $Rev:: 921          $
 * @date    $Date:: 2015-09-16 #$
 * @brief   The header file of the BFTM library.
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

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1655_56_BFTM_H
#define __HT32F1655_56_BFTM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1655_56.h"

/** @addtogroup HT32F1655_56_Peripheral_Driver HT32F1655/1656 Peripheral Driver
  * @{
  */

/** @addtogroup BFTM
  * @{
  */


/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup BFTM_Exported_Constants BFTM exported constants
  * @{
  */
#define IS_BFTM(x) ((x == HT_BFTM0) || (x == HT_BFTM1))

#define BFTM_FLAG_MATCH           (1UL << 0)
#define BFTM_INT_MATCH            (1UL << 0)
/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup BFTM_Exported_Functions BFTM exported functions
  * @{
  */
void BFTM_DeInit(HT_BFTM_TypeDef* BFTMx);
void BFTM_EnaCmd(HT_BFTM_TypeDef* BFTMx, ControlStatus NewState);
void BFTM_SetCompare(HT_BFTM_TypeDef* BFTMx, u32 wCompare);
u32 BFTM_GetCompare(HT_BFTM_TypeDef* BFTMx);
void BFTM_SetCounter(HT_BFTM_TypeDef* BFTMx, u32 wCounter);
u32 BFTM_GetCounter(HT_BFTM_TypeDef* BFTMx);
void BFTM_OneShotModeCmd(HT_BFTM_TypeDef* BFTMx, ControlStatus NewState);
void BFTM_IntConfig(HT_BFTM_TypeDef* BFTMx, ControlStatus NewState);
FlagStatus BFTM_GetFlagStatus(HT_BFTM_TypeDef* BFTMx);
void BFTM_ClearFlag(HT_BFTM_TypeDef* BFTMx);
/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif

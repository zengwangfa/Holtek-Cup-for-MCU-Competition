/*********************************************************************************************************//**
 * @file    ht32f1655_56_cmp_op.h
 * @version V1.00
 * @date    2014-06-30
 * @brief   The header file of the CMP_OP library.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1655_56_CMP_OP_H
#define __HT32F1655_56_CMP_OP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1655_56.h"

/** @addtogroup HT32F1655_56_Peripheral_Driver HT32F1655/1656 Peripheral Driver
  * @{
  */

/** @addtogroup CMP_OP
  * @{
  */


/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup CMP_OP_Exported_Constants CMP_OP exported constants
  * @{
  */

/* Definitions of CMP_OP modes                                                                              */
#define OP_MODE                       (0x00000000ul)
#define CMP_MODE                      (0x00000002ul)

#define IS_CMP_OP_MODE(MODE) ((MODE == OP_MODE) || (MODE == CMP_MODE))

/* Definitions the cancelation reference input of CMP_OP                                                    */
#define CMP_OP_NEGATIVE_INPUT         (0x00000000ul)
#define CMP_OP_POSITIVE_INPUT         (0x00000008ul)

#define IS_CMP_OP_REF(REF) ((REF == CMP_OP_NEGATIVE_INPUT ) || (REF == CMP_OP_POSITIVE_INPUT))

/* Definitions of CMP_OP input offset                                                                       */
#define CMP_OP_IOVC_MIN               (0x00000000ul)
#define CMP_OP_IOVC_CENTER            (0x00000020ul)
#define CMP_OP_IOVC_MAX               (0x0000003Ful)

#define IS_CMP_OP_IOVC(IOVC)          (IOVC <= 63 )

/* Definitions of CMP_OP interrupts                                                                         */
#define CMP_OP_INT_FALLING            (0x00000001ul)
#define CMP_OP_INT_RISING             (0x00000002ul)

#define IS_CMP_OP_INT(CMP_OP_INT)     (((CMP_OP_INT & 0xFFFFFFFC) == 0x0) && \
                                       (CMP_OP_INT != 0x0))

/* Definitions of CMP_OP interrupt flags                                                                    */
#define CMP_OP_FLAG_FALLING           (0x00000001ul)
#define CMP_OP_FLAG_RISING            (0x00000002ul)

#define IS_CMP_OP_FLAG(FLAG)          (((FLAG & 0xFFFFFFFC) == 0x0) && \
                                       (FLAG != 0x0))

/* check parameter of the CMP_OPx                                                                           */
#define IS_CMP_OP_ALL_PERIPH(PERIPH)  ((PERIPH == HT_CMP_OP0) || (PERIPH == HT_CMP_OP1))
/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup CMP_OP_Exported_Functions CMP_OP exported functions
  * @{
  */
void CMP_OP_DeInit(HT_CMP_OP_TypeDef* CMP_OPx);
void CMP_OP_Config(HT_CMP_OP_TypeDef* CMP_OPx, u32 mode, u32 cancellation);
void CMP_OP_Cmd(HT_CMP_OP_TypeDef* CMP_OPx, ControlStatus NewState);
void CMP_OP_CancellationModeConfig(HT_CMP_OP_TypeDef* CMP_OPx, u16 CMP_OP_REF_INPUT);
void CMP_OP_SetCancellationVaule(HT_CMP_OP_TypeDef* CMP_OPx, u32 cancellation);
u32 CMP_OP_GetCancellationVaule(HT_CMP_OP_TypeDef* CMP_OPx);
void CMP_OP_IntConfig(HT_CMP_OP_TypeDef* CMP_OPx, u32 CMP_OP_INT, ControlStatus NewState);
FlagStatus CMP_OP_GetIntStatus(HT_CMP_OP_TypeDef* CMP_OPx, u32 CMP_OP_INT);
FlagStatus CMP_OP_GetFlagStatus(HT_CMP_OP_TypeDef* CMP_OPx, u32 CMP_OP_FLAG);
void CMP_OP_ClearIntPendingBit(HT_CMP_OP_TypeDef* CMP_OPx, u32 CMP_OP_INT);
FlagStatus CMP_OP_GetOutputStatus(HT_CMP_OP_TypeDef* CMP_OPx);
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

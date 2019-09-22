/*********************************************************************************************************//**
 * @file    ht32f1655_56_ebi.h
 * @version V1.00
 * @date    2014-06-30
 * @brief   The header file of the EBI library.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1655_56_EBI_H
#define __HT32F1655_56_EBI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1655_56.h"

/** @addtogroup HT32F1655_56_Peripheral_Driver HT32F1655/1656 Peripheral Driver
  * @{
  */

/** @addtogroup EBI
  * @{
  */


/* Exported types ------------------------------------------------------------------------------------------*/
/** @defgroup EBI_Exported_Types EBI exported types
  * @{
  */
typedef struct
{
  u32 EBI_Bank;
  u32 EBI_Mode;
  u32 EBI_ByteLane;
  u32 EBI_IdleCycle;
  u32 EBI_AsynchronousReady;
  u32 EBI_ARDYTimeOut;
  u32 EBI_ChipSelectPolarity;
  u32 EBI_AddressLatchPolarity;
  u32 EBI_WriteEnablePolarity;
  u32 EBI_ReadEnablePolarity;
  u32 EBI_ByteLanePolarity;
  u32 EBI_ReadySignalPolarity;
  u32 EBI_IdleCycleTime;
  u32 EBI_AddressSetupTime;
  u32 EBI_AddressHoldTime;
  u32 EBI_WriteSetupTime;
  u32 EBI_WriteStrobeTime;
  u32 EBI_WriteHoldTime;
  u32 EBI_ReadSetupTime;
  u32 EBI_ReadStrobeTime;
  u32 EBI_ReadHoldTime;
  u32 EBI_PageMode;
  u32 EBI_PageLength;
  u32 EBI_PageHitMode;
  u32 EBI_PageAccessTime;
  u32 EBI_PageOpenTime;
}EBI_InitTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup EBI_Exported_Constants EBI exported constants
  * @{
  */
#define EBI_BANK_0                                  ((u32)0x00000000)
#define EBI_BANK_1                                  ((u32)0x00000001)
#define EBI_BANK_2                                  ((u32)0x00000002)
#define EBI_BANK_3                                  ((u32)0x00000003)

#define IS_EBI_BANK(BANK)                           ((BANK == EBI_BANK_0) || \
                                                     (BANK == EBI_BANK_1) || \
                                                     (BANK == EBI_BANK_2) || \
                                                     (BANK == EBI_BANK_3))


#define EBI_MODE_D8A8                               ((u32)0x00000000)
#define EBI_MODE_D16A16ALE                          ((u32)0x00000001)
#define EBI_MODE_D8A24ALE                           ((u32)0x00000002)
#define EBI_MODE_D16                                ((u32)0x00000003)

#define IS_EBI_MODE(MODE)                           ((MODE == EBI_MODE_D8A8) || \
                                                     (MODE == EBI_MODE_D16A16ALE) || \
                                                     (MODE == EBI_MODE_D8A24ALE) || \
                                                     (MODE == EBI_MODE_D16))


#define EBI_BYTELANE_ENABLE                         ((u32)0x01000000)
#define EBI_BYTELANE_DISABLE                        ((u32)0x00000000)

#define IS_EBI_BYTELANE(BYTELANE)                   ((BYTELANE == EBI_BYTELANE_ENABLE) || \
                                                     (BYTELANE == EBI_BYTELANE_DISABLE))


#define EBI_IDLECYCLE_ENABLE                        ((u32)0x00000000)
#define EBI_IDLECYCLE_DISABLE                       ((u32)0x00001000)

#define IS_EBI_IDLECYCLE(IDLECYCLE)                 ((IDLECYCLE == EBI_IDLECYCLE_ENABLE) || \
                                                     (IDLECYCLE == EBI_IDLECYCLE_DISABLE))


#define EBI_ASYNCHRONOUSREADY_ENABLE                ((u32)0x00010000)
#define EBI_ASYNCHRONOUSREADY_DISABLE               ((u32)0x00000000)

#define IS_EBI_ARDY(ARDY)                           ((ARDY == EBI_ASYNCHRONOUSREADY_ENABLE) || \
                                                     (ARDY == EBI_ASYNCHRONOUSREADY_DISABLE))


#define EBI_ARDYTIMEOUT_ENABLE                      ((u32)0x00000000)
#define EBI_ARDYTIMEOUT_DISABLE                     ((u32)0x00020000)

#define IS_EBI_ARDY_TIMEOUT(TIMEOUT)                ((TIMEOUT == EBI_ARDYTIMEOUT_ENABLE) || \
                                                     (TIMEOUT == EBI_ARDYTIMEOUT_DISABLE))


#define EBI_CHIPSELECTPOLARITY_LOW                  ((u32)0x00000000)
#define EBI_CHIPSELECTPOLARITY_HIGH                 ((u32)0x00000001)

#define IS_EBI_CS_POLARITY(POLARITY)                ((POLARITY == EBI_CHIPSELECTPOLARITY_LOW) || \
                                                     (POLARITY == EBI_CHIPSELECTPOLARITY_HIGH))


#define EBI_ADDRESSLATCHPOLARITY_LOW                ((u32)0x00000000)
#define EBI_ADDRESSLATCHPOLARITY_HIGH               ((u32)0x00000001)

#define IS_EBI_ALE_POLARITY(POLARITY)               ((POLARITY == EBI_ADDRESSLATCHPOLARITY_LOW) || \
                                                     (POLARITY == EBI_ADDRESSLATCHPOLARITY_HIGH))


#define EBI_WRITEENABLEPOLARITY_LOW                 ((u32)0x00000000)
#define EBI_WRITEENABLEPOLARITY_HIGH                ((u32)0x00000001)

#define IS_EBI_WE_POLARITY(POLARITY)                ((POLARITY == EBI_WRITEENABLEPOLARITY_LOW) || \
                                                     (POLARITY == EBI_WRITEENABLEPOLARITY_HIGH))


#define EBI_READENABLEPOLARITY_LOW                  ((u32)0x00000000)
#define EBI_READENABLEPOLARITY_HIGH                 ((u32)0x00000001)

#define IS_EBI_RE_POLARITY(POLARITY)                ((POLARITY == EBI_READENABLEPOLARITY_LOW) || \
                                                     (POLARITY == EBI_READENABLEPOLARITY_HIGH))


#define EBI_BYTELANEPOLARITY_LOW                    ((u32)0x00000000)
#define EBI_BYTELANEPOLARITY_HIGH                   ((u32)0x00000001)

#define IS_EBI_BL_POLARITY(POLARITY)                ((POLARITY == EBI_BYTELANEPOLARITY_LOW) || \
                                                     (POLARITY == EBI_BYTELANEPOLARITY_HIGH))


#define EBI_READYSIGNALPOLARITY_LOW                 ((u32)0x00000000)
#define EBI_READYSIGNALPOLARITY_HIGH                ((u32)0x00000001)

#define IS_EBI_ARDY_POLARITY(POLARITY)              ((POLARITY == EBI_READYSIGNALPOLARITY_LOW) || \
                                                     (POLARITY == EBI_READYSIGNALPOLARITY_HIGH))


#define EBI_PAGEMODE_ENABLE                         ((u32)0x01000000)
#define EBI_PAGEMODE_DISABLE                        ((u32)0x00000000)

#define IS_EBI_PAGE_MODE(MODE)                      ((MODE == EBI_PAGEMODE_ENABLE) || \
                                                     (MODE == EBI_PAGEMODE_DISABLE))


#define EBI_PAGELENGTH_4                            ((u32)0x00000000)
#define EBI_PAGELENGTH_8                            ((u32)0x00000001)
#define EBI_PAGELENGTH_16                           ((u32)0x00000002)
#define EBI_PAGELENGTH_32                           ((u32)0x00000003)

#define IS_EBI_PAGE_LENGTH(LENGTH)                  ((LENGTH == EBI_PAGELENGTH_4) || \
                                                     (LENGTH == EBI_PAGELENGTH_8) || \
                                                     (LENGTH == EBI_PAGELENGTH_16) || \
                                                     (LENGTH == EBI_PAGELENGTH_32))


#define EBI_PAGEHITMODE_ADDINC                      ((u32)0x00000010)
#define EBI_PAGEHITMODE_INTRPAGE                    ((u32)0x00000000)

#define IS_EBI_PAGE_HIT_MODE(MODE)                  ((MODE == EBI_PAGEHITMODE_ADDINC) || \
                                                     (MODE == EBI_PAGEHITMODE_INTRPAGE))


#define EBI_INT_TOUT                                ((u32)0x00000001)
#define EBI_INT_ACCDIS                              ((u32)0x00000002)
#define EBI_INT_ACCERR                              ((u32)0x00000002)
#define EBI_INT_ACCRES                              ((u32)0x00000004)
#define EBI_INT_ALL                                 ((u32)0x00000007)

#define IS_EBI_INT(INT)                             (((INT & 0xFFFFFFF8) == 0x0) && (INT != 0x0))

#define IS_EBI_INT_FLAG(FLAG)                       (((FLAG & 0xFFFFFFFC) == 0x0) && (FLAG != 0x0))



#define IS_EBI_IDLE_CYCLE_TIME(TIME)                (TIME < 0x10)

#define IS_EBI_ADDRESS_SETUP_TIME(TIME)             (TIME < 0x10)

#define IS_EBI_ADDRESS_HOLD_TIME(TIME)              (TIME < 0x10)

#define IS_EBI_WRITE_SETUP_TIME(TIME)               (TIME < 0x10)

#define IS_EBI_WRITE_STROBE_TIME(TIME)              (TIME < 0x40)

#define IS_EBI_WRITE_HOLD_TIME(TIME)                (TIME < 0x10)

#define IS_EBI_READ_SETUP_TIME(TIME)                (TIME < 0x10)

#define IS_EBI_READ_STROBE_TIME(TIME)               (TIME < 0x40)

#define IS_EBI_READ_HOLD_TIME(TIME)                 (TIME < 0x10)

#define IS_EBI_PAGE_ACCESS_TIME(TIME)               (TIME < 0x10)

#define IS_EBI_PAGE_OPEN_TIME(TIME)                 (TIME < 0x100)
/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup EBI_Exported_Functions EBI exported functions
  * @{
  */
void EBI_DeInit(void);
void EBI_Init(EBI_InitTypeDef* EBI_InitStruct);
void EBI_StructInit(EBI_InitTypeDef* EBI_InitStruct);
void EBI_Cmd(u32 EBI_Bank, ControlStatus NewState);
void EBI_IntConfig(u32 EBI_Int, ControlStatus NewState);
FlagStatus EBI_GetIntStatus(u32 EBI_Int);
void EBI_ClearIntFlag(u32 EBI_Int);
FlagStatus EBI_GetBusyStatus(void);
FlagStatus EBI_GetARDYStatus(void);
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

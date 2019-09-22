/*********************************************************************************************************//**
 * @file    ht32_serial.h
 * @version V1.00
 * @date    2014-06-30
 * @brief   The header file of the Serial library.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32_SERIAL_H
#define __HT32_SERIAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#ifdef RETARGET_IS_USB
#include "ht32_usbd_core.h"
#endif

/** @addtogroup HT32_Peripheral_Driver HT32 Peripheral Driver
  * @{
  */

/** @addtogroup SERIAL
  * @brief Serial related functions
  * @{
  */


/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup SERIAL_Exported_Functions Serial exported functions
  * @{
  */
u32 SERIAL_GetChar(void);
u32 SERIAL_PutChar(u32 ch);
#ifdef RETARGET_IS_USB
void SERIAL_USBDClass_Request(USBDCore_Device_TypeDef *pDev);
void SERIAL_USBDClass_RXHandler(USBD_EPTn_Enum EPTn);
void SERIAL_USBDInit(void);
#endif
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

#endif /* __HT32_SERIAL_H ----------------------------------------------------------------------------------*/

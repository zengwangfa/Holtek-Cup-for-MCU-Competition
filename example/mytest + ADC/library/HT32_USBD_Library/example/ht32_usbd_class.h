/*********************************************************************************************************//**
 * @file    example/ht32_usbd_class.h
 * @version V1.00
 * @date    2014-06-30
 * @brief   The header file of USB Device Class.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32_USBD_CLASS_H
#define __HT32_USBD_CLASS_H

/* Includes ------------------------------------------------------------------------------------------------*/

/** @addtogroup HT32_USBD_Library HT32 USB Device Library
  * @{
  */

/** @defgroup USBDClass USB Device Class
  * @brief USB Device Class
  * @{
  */


/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup USBDClass_Private_Define USB Device Class private definitions
  * @{
  */
/* For ht32_usbd_descriptor.c                                                                               */
#define CLASS_INF_CLASS                   (DESC_CLASS_03_XXX)
#define CLASS_INF_SUBCLASS                (HID_SUBCLASS_00_NONE)
#define CLASS_INF_PTCO                    (HID_PROTOCOL_00_NONE)

/* HID related definition                                                                                   */
#define DESC_LEN_XXX                      ((u32)(9))
#define DESC_LEN_XXX                      ((u16)(47))

#define DESC_TYPE_01_XXX                  (0x01)
#define DESC_TYPE_02_XXX                  (0x02)

#define HID_SUBCLASS_00_NONE              (0x00)
#define HID_SUBCLASS_01_BOOT              (0x01)

#define HID_PROTOCOL_00_NONE              (0x00)
/**
  * @}
  */


/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup USBDClass_Exported_Functions USB Device Class exported functions
  * @{
  */
void USBDClass_Init(USBDCore_Class_TypeDef *pClass);
/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

#endif /* __HT32_USBD_CLASS_H ------------------------------------------------------------------------------*/

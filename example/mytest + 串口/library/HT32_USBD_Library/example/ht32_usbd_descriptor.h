/*********************************************************************************************************//**
 * @file    example/ht32_usbd_descriptor.h
 * @version V1.00
 * @date    2014-06-30
 * @brief   The USB descriptor.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32_USBD_DESCRIPTOR_H
#define __HT32_USBD_DESCRIPTOR_H

/* Includes ------------------------------------------------------------------------------------------------*/

/** @addtogroup HT32_USBD_Library HT32 USB Device Library
  * @{
  */

/** @defgroup USBDDescriptor USB Descriptor
  * @brief USB descriptor
  * @{
  */


/* Exported constants --------------------------------------------------------------------------------------*/
#define DESC_LEN_CONFN_T          (u16)(DESC_LEN_CONFN + DESC_LEN_INF + DESC_LEN_XXX + DESC_LEN_EPT * 2)

/* Exported functions --------------------------------------------------------------------------------------*/
void USBDDesc_Init(USBDCore_Desc_TypeDef *pDesc);


/**
  * @}
  */

/**
  * @}
  */

#endif /* __HT32_USBD_DESCRIPTOR_H -------------------------------------------------------------------------*/

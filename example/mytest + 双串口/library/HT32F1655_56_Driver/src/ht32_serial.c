/*********************************************************************************************************//**
 * @file    ht32_serial.c
 * @version V1.00
 * @date    2014-06-30
 * @brief   This file provides all the Low level serial routines for HT32.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_usbd_core.h"

#if (_RETARGET == 1)

/** @addtogroup HT32_Peripheral_Driver HT32 Peripheral Driver
  * @{
  */

/** @defgroup SERIAL SERIAL
  * @brief Serial related functions
  * @{
  */


/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup SERIAL_Exported_Functions Serial exported functions
  * @{
  */

#ifdef RETARGET_IS_UART
/*********************************************************************************************************//**
  * @brief  Put char to USART.
  * @param  ch: The char put to USART.
  * @retval The char put to USART.
  ***********************************************************************************************************/
u32 SERIAL_PutChar(u32 ch)
{
  USART_SendData(RETARGET_USART_PORT, (u8)ch);
  while (USART_GetLineStatus(RETARGET_USART_PORT, USART_LSR_TE) == RESET)
  {
  }
  return ch;
}

/*********************************************************************************************************//**
  * @brief  Get char from USART.
  * @retval The char got from USART.
  ***********************************************************************************************************/
u32 SERIAL_GetChar(void)
{
  while (USART_GetLineStatus(RETARGET_USART_PORT, USART_LSR_RFDR) == RESET)
  {
  }
  return USART_ReceiveData(RETARGET_USART_PORT);
}
#endif


#ifdef RETARGET_IS_USB
/* Private types -------------------------------------------------------------------------------------------*/
/** @defgroup SERIAL_Private_TypesDefinitions Serial private types definitions
  * @{
  */
typedef struct _VCP_LINE_CODING
{
  u32 dwDTERate;      //Bit rate;
  u8  bCharFormat;    //Stop bits:
                      //0 = 1   Stop bit
                      //1 = 1.5 Stop bit
                      //2 = 2   Stop bit
  u8  bParityType;    //parity:
                      //0 = None
                      //1 = Odd
                      //2 = Even
                      //3 = Mark
                      //4 = Space
  u8  bDataBits;      //Number of data bits (7, 8, 9)
} USBDClass_VCP_LINE_CODING;
/**
  * @}
  */

/* Private constants ---------------------------------------------------------------------------------------*/
/** @defgroup SERIAL_Private_Define Serial private definitions
  * @{
  */
#define CLASS_REQ_20_SET_LINE_CODING        (0x20)
#define CLASS_REQ_21_GET_LINE_CODING        (0x21)
#define CLASS_REQ_22_SET_CONTROL_LINE_STATE (0x22)
/**
  * @}
  */

/* Private macro -------------------------------------------------------------------------------------------*/
/** @defgroup SERIAL_Private_Macro Serial private macros
  * @{
  */
#define IS_BUFFER_FULL()        ((uWriteIndex + 1) % RETARGET_BUFFER_SIZE == uReadIndex)
#define IS_BUFFER_EMPTY()       (uReadIndex == uWriteIndex)
/**
  * @}
  */

/* Private variables ---------------------------------------------------------------------------------------*/
/** @defgroup SERIAL_Private_Variable Serial private variables
  * @{
  */
static USBDClass_VCP_LINE_CODING USBDClassVCPLineCoding;
__ALIGN4 static u8 uSerialBuffer[RETARGET_BUFFER_SIZE];
static u32 uReadIndex;
static vu32 uWriteIndex;
vu32 uIsTerminalOpened = 0;
/**
  * @}
  */

#if (RETARGET_RX_EPT == RETARGET_TX_EPT)
  #error USB Endpoint of retarget Rx and Tx must different. Please check RETARGET_RX_EPT/RETARGET_TX_EPT "ht32_retarget_usbdconf.h".
#endif

#if (RETARGET_CTRL_EPT == RETARGET_TX_EPT)
  #error USB Endpoint of retarget Control and Tx must different. Please check RETARGET_CTRL_EPT/RETARGET_TX_EPT "ht32_retarget_usbdconf.h".
#endif

#if (RETARGET_CTRL_EPT == RETARGET_RX_EPT)
  #error USB Endpoint of retarget Control and Rx must different. Please check RETARGET_CTRL_EPT/RETARGET_RX_EPT "ht32_retarget_usbdconf.h".
#endif

#ifdef _RERATGET1_ERR
  #error Endpoint 1 already used by other USB class. Retarget can not overwrite it.
#endif

#ifdef _RERATGET2_ERR
  #error Endpoint 2 already used by other USB class. Retarget can not overwrite it.
#endif

#ifdef _RERATGET3_ERR
  #error Endpoint 3 already used by other USB class. Retarget can not overwrite it.
#endif

#ifdef _RERATGET4_ERR
  #error Endpoint 4 already used by other USB class. Retarget can not overwrite it.
#endif

#ifdef _RERATGET5_ERR
  #error Endpoint 5 already used by other USB class. Retarget can not overwrite it.
#endif

#ifdef _RERATGET6_ERR
  #error Endpoint 6 already used by other USB class. Retarget can not overwrite it.
#endif

#ifdef _RERATGET7_ERR
  #error Endpoint 7 already used by other USB class. Retarget can not overwrite it.
#endif

/*********************************************************************************************************//**
  * @brief  Put char to USB.
  * @param  ch: The char put to USB.
  * @retval The char put to USB.
  ***********************************************************************************************************/
u32 SERIAL_PutChar(u32 ch)
{
  while(uIsTerminalOpened == 0);

  while (USBDCore_EPTGetTransferCount((USBD_EPTn_Enum)RETARGET_TX_EPT, USBD_TCR_0));
  USBDCore_EPTWriteINData((USBD_EPTn_Enum)RETARGET_TX_EPT, (u32 *)&ch, 1);

  return ch;
}

/*********************************************************************************************************//**
  * @brief  Get char from USB.
  * @retval The char got from USB.
  ***********************************************************************************************************/
u32 SERIAL_GetChar(void)
{
  u32 value = 0;

  while(uIsTerminalOpened == 0);

  while (IS_BUFFER_EMPTY());
  value = uSerialBuffer[uReadIndex];
  uReadIndex = (uReadIndex + 1) % RETARGET_BUFFER_SIZE;

  return value;
}

#ifdef NON_USB_IN_APP
#include "ht32_retarget_desc.c"
__ALIGN4 USBDCore_TypeDef gUSBCore;
USBD_Driver_TypeDef gUSBDriver;

/*********************************************************************************************************//**
 * @brief   This function handles USB interrupt.
 * @retval  None
 ************************************************************************************************************/
void USB_IRQHandler(void)
{
  USBDCore_IRQHandler(&gUSBCore);
}

/*********************************************************************************************************//**
  * @brief  USB Class initialization.
  * @param  pClass: pointer of USBDCore_Class_TypeDef
  * @retval None
  ***********************************************************************************************************/
void USBDClass_Init(USBDCore_Class_TypeDef *pClass)
{
  pClass->CallBack_ClassRequest = SERIAL_USBDClass_Request;
  pClass->CallBack_EPTn[RETARGET_RX_EPT] = SERIAL_USBDClass_RXHandler;
  return;
}

/*********************************************************************************************************//**
  * @brief  Configure USB for retarget.
  * @retval None
  ***********************************************************************************************************/
void SERIAL_USBDInit(void)
{
  u32 uPLL;

  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.USBD = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  uPLL = CKCU_GetPLLFrequency();

  #if 0
  switch (uPLL)
  {
    case 48000000:
    {
      CKCU_SetUSBPrescaler(CKCU_USBPRE_DIV1);
      break;
    }
    case 96000000:
    {
      CKCU_SetUSBPrescaler(CKCU_USBPRE_DIV2);
      break;
    }
    case 144000000:
    {
      CKCU_SetUSBPrescaler(CKCU_USBPRE_DIV3);
      break;
    }
  }
  #endif
  CKCU_SetUSBPrescaler((CKCU_USBPRE_TypeDef)((uPLL / 48000000) - 1));

  gUSBCore.pDriver = (u32 *)&gUSBDriver;                /* Initiate memory pointer of USB driver            */
  USBDDesc_Init(&gUSBCore.Device.Desc);                 /* Initiate memory pointer of descriptor            */
  USBDClass_Init(&gUSBCore.Class);                      /* Initiate USB Class layer                         */
  USBDCore_Init(&gUSBCore);                             /* Initiate USB Core layer                          */
  NVIC_EnableIRQ(USB_IRQn);                             /* Enable USB device interrupt                      */
  USBDCore_MainRoutine(&gUSBCore);                      /* USB core main routine                            */

 // while (USBDCore_GetStatus() != USB_STATE_CONFIGURED);
}
#endif

/*********************************************************************************************************//**
  * @brief  USB Device Class Request for USB retarget
  * @param  pDev: pointer of USB Device
  * @retval None
  ***********************************************************************************************************/
void SERIAL_USBDClass_Request(USBDCore_Device_TypeDef *pDev)
{
  u8 USBCmd = *((u8 *)(&(pDev->Request.bRequest)));
  u16 len = *((u16 *)(&(pDev->Request.wLength)));
  u32 inf = pDev->Request.wIndex;
  u32 uIsCmdOK = 0;

  if(inf != 11)
  {
    return;
  }
  if(USBCmd == CLASS_REQ_22_SET_CONTROL_LINE_STATE)
  {
    if (len == 0)
    {
      pDev->Transfer.pData = 0;
      pDev->Transfer.sByteLength = 0;
      pDev->Transfer.Action = USB_ACTION_DATAOUT;
    }
  }
  else
  {
    if(USBCmd == CLASS_REQ_20_SET_LINE_CODING)
    {
      pDev->Transfer.Action = USB_ACTION_DATAOUT;
      uIsTerminalOpened = 1;
      uIsCmdOK = 1;
    }
    else if(USBCmd == CLASS_REQ_21_GET_LINE_CODING)
    {
      pDev->Transfer.Action = USB_ACTION_DATAIN;
      uIsCmdOK = 1;
    }
    if(uIsCmdOK == 1)
    {
      pDev->Transfer.pData = (uc8*)&USBDClassVCPLineCoding;
      pDev->Transfer.sByteLength = (sizeof(USBDClassVCPLineCoding) > pDev->Request.wLength) ? (pDev->Request.wLength) : (sizeof(USBDClassVCPLineCoding));
    }
  }

  #if 0
  switch (USBCmd)
  {
    case CLASS_REQ_22_SET_CONTROL_LINE_STATE:
    {
      if (len == 0)
      {
        pDev->Transfer.pData = 0;
        pDev->Transfer.sByteLength = 0;
        pDev->Transfer.Action = USB_ACTION_DATAOUT;
      }
      break;
    }
    case CLASS_REQ_20_SET_LINE_CODING:
    {
      pDev->Transfer.pData = (uc8*)&USBDClassVCPLineCoding;
      pDev->Transfer.sByteLength = (sizeof(USBDClassVCPLineCoding) > pDev->Request.wLength) ? (pDev->Request.wLength) : (sizeof(USBDClassVCPLineCoding));
      pDev->Transfer.Action = USB_ACTION_DATAOUT;
      uIsTerminalOpened = 1;
      break;
    }
    case CLASS_REQ_21_GET_LINE_CODING:
    {
      pDev->Transfer.pData = (uc8*)&USBDClassVCPLineCoding;
      pDev->Transfer.sByteLength = (sizeof(USBDClassVCPLineCoding) > pDev->Request.wLength) ? (pDev->Request.wLength) : (sizeof(USBDClassVCPLineCoding));
      pDev->Transfer.Action = USB_ACTION_DATAIN;
      break;
    }
  }
  #endif
  return;
}

/*********************************************************************************************************//**
  * @brief  USB Class Received handler
  * @param  EPTn: USB Endpoint number
  *         @arg USBD_EPT0 ~ USBD_EPT7
  * @retval None
  ***********************************************************************************************************/
void SERIAL_USBDClass_RXHandler(USBD_EPTn_Enum EPTn)
{
  u32 uLen;

  /* Read Receive data                                                                                      */
  uLen = USBDCore_EPTGetTransferCount(EPTn, USBD_TCR_0);
  uLen = USBDCore_EPTReadOUTData(EPTn, (u32*)&uSerialBuffer[uWriteIndex], uLen);
  uWriteIndex = (uWriteIndex + (uLen)) % RETARGET_BUFFER_SIZE;

  return;
}

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
#endif

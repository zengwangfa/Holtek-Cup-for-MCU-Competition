/*********************************************************************************************************//**
 * @file    ht32f1655_56_sci.c
 * @version V1.00
 * @date    2014-06-30
 * @brief   This file provides all the SCI firmware functions.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1655_56_sci.h"

/** @addtogroup HT32F1655_56_Peripheral_Driver HT32F1655/1656 Peripheral Driver
  * @{
  */

/** @defgroup SCI SCI
  * @brief SCI driver modules
  * @{
  */


/* Private constants ---------------------------------------------------------------------------------------*/
/** @defgroup SCI_Private_Define SCI private definitions
  * @{
  */
/* SCI ENSCI mask                                                                                           */
#define CR_ENSCI_SET             ((u32)0x00000020)
#define CR_ENSCI_RESET           ((u32)0xFFFFFFDF)

/* SCI WTEN mask                                                                                            */
#define CR_WTEN_SET              ((u32)0x00000004)
#define CR_WTEN_RESET            ((u32)0xFFFFFFFB)
/**
  * @}
  */

/* Global functions ----------------------------------------------------------------------------------------*/
/** @defgroup SCI_Exported_Functions SCI exported functions
  * @{
  */
/*********************************************************************************************************//**
  * @brief  Deinitializes the SCI peripheral registers to their default reset values.
  * @retval None
  ***********************************************************************************************************/
void SCI_DeInit(void)
{
  RSTCU_PeripReset_TypeDef RSTCUReset = {{0}};

  RSTCUReset.Bit.SCI = 1;
  RSTCU_PeripReset(RSTCUReset, ENABLE);
}

/*********************************************************************************************************//**
  * @brief  Initializes the SCI peripheral according to the specified parameters in the SCI_InitStruct.
  * @param  SCI_InitStruct: pointer to a SCI_InitTypeDef structure that contains the configuration
  *   information for the specified SCI peripheral.
  * @retval None
  ***********************************************************************************************************/
void SCI_Init(SCI_InitTypeDef* SCI_InitStruct)
{
  u32 tmpreg;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_MODE(SCI_InitStruct->SCI_Mode));
  Assert_Param(IS_SCI_RETRY(SCI_InitStruct->SCI_Retry));
  Assert_Param(IS_SCI_CONVENTION(SCI_InitStruct->SCI_Convention));
  Assert_Param(IS_SCI_CARD_POLARITY(SCI_InitStruct->SCI_CardPolarity));
  Assert_Param(IS_SCI_CLOCK_PRESCALER(SCI_InitStruct->SCI_ClockPrescale));


  /*------------------------- SCI Control Register Configuration -------------------------------------------*/
  tmpreg = HT_SCI->CR;
  tmpreg &= 0xFFFFFFA4;

  tmpreg |= SCI_InitStruct->SCI_Mode | SCI_InitStruct->SCI_Retry | SCI_InitStruct->SCI_Convention |
            SCI_InitStruct->SCI_CardPolarity;

  HT_SCI->CR = tmpreg;

  /*------------------------- SCI Prescaler Register Configuration -----------------------------------------*/
  HT_SCI->PSC = SCI_InitStruct->SCI_ClockPrescale;
}

/*********************************************************************************************************//**
  * @brief  Initializes the SCI peripheral according to the specified parameters in the SCI_InitStruct.
  * @param  SCI_InitStruct: pointer to a SCI_InitTypeDef structure that contains the configuration
  *   information for the specified SCI peripheral.
  * @retval None
  ***********************************************************************************************************/
void SCI_StructInit(SCI_InitTypeDef* SCI_InitStruct)
{
  /* Initialize the SCI_Mode member                                                                         */
  SCI_InitStruct->SCI_Mode = SCI_MODE_MANUAL;

  /* Initialize the SCI_Retry member                                                                        */
  SCI_InitStruct->SCI_Retry = SCI_RETRY_NO;

  /* Initialize the SCI_Convention member                                                                   */
  SCI_InitStruct->SCI_Convention = SCI_CONVENTION_DIRECT;

  /* Initialize the SCI_CardPolarity member                                                                 */
  SCI_InitStruct->SCI_CardPolarity = SCI_CARDPOLARITY_LOW;

  /* Initialize the SCI_ClockPrescale member                                                                */
  SCI_InitStruct->SCI_ClockPrescale = SCI_CLKPRESCALER_1;
}

/*********************************************************************************************************//**
  * @brief  Enables or Disables the specified SCI peripheral.
  * @param  NewState: new state of the SCI peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  ***********************************************************************************************************/
void SCI_Cmd(ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SCI peripheral                                                                   */
    HT_SCI->CR |= CR_ENSCI_SET;
  }
  else
  {
    /* Disable the selected SCI peripheral                                                                  */
    HT_SCI->CR &= CR_ENSCI_RESET;
  }
}

/*********************************************************************************************************//**
  * @brief  This function is used to configure the Elementary Time Unit.
  * @param  SCI_ETU: specifies the SCI Elementary Time Unit.
  * @param  SCI_Compensation: Enables or Disables the Compensation mode.
  *   This parameter can be one of the following values:
  *     @arg SCI_COMPENSATION_ENABLE          : Compensation mode enabled
  *     @arg SCI_COMPENSATION_DISABLE         : Compensation mode disabled
  * @retval None
  ***********************************************************************************************************/
void SCI_ETUConfig(u32 SCI_ETU, u32 SCI_Compensation)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_ETU(SCI_ETU));
  Assert_Param(IS_SCI_ETU_COMPENSATION(SCI_Compensation));

  HT_SCI->ETU = SCI_ETU | SCI_Compensation;
}

/*********************************************************************************************************//**
  * @brief  This function is used to set the value of SCI GuardTime.
  * @param  SCI_GuardTime: specifies the value of SCI GuardTime value.
  * @retval None
  ***********************************************************************************************************/
void SCI_SetGuardTimeValue(u16 SCI_GuardTime)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_GUARDTIME(SCI_GuardTime));

  HT_SCI->GT = SCI_GuardTime;
}

/*********************************************************************************************************//**
  * @brief  This function is used to set the value of SCI Waiting Time.
  * @param  SCI_WaitingTime: specifies the value of SCI Waiting Time value.
  * @retval None
  ***********************************************************************************************************/
void SCI_SetWaitingTimeValue(u32 SCI_WaitingTime)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_WAITING_TIME(SCI_WaitingTime));

  HT_SCI->WT = SCI_WaitingTime;
}

/*********************************************************************************************************//**
  * @brief  Enables or Disables the Waiting Time Counter.
  * @param  NewState: new state of the SCI peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  ***********************************************************************************************************/
void SCI_WaitingTimeCounterCmd(ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_SCI->CR |= CR_WTEN_SET;
  }
  else
  {
    HT_SCI->CR &= CR_WTEN_RESET;
  }
}

/*********************************************************************************************************//**
  * @brief  Sends a data byte through the SCI peripheral.
  * @param  SCI_Data: byte to be transmitted.
  * @retval None
  ***********************************************************************************************************/
void SCI_SendData(u8 SCI_Data)
{
  HT_SCI->TXB = SCI_Data;
}

/*********************************************************************************************************//**
  * @brief  Returns the received data through the SCI peripheral
  * @retval The value of the received data.
  ***********************************************************************************************************/
u8 SCI_ReceiveData(void)
{
  return (u8)HT_SCI->RXB;
}

/*********************************************************************************************************//**
  * @brief  Determines the SCI output clock signal is driven by hardware or software.
  * @param  SCI_CLKMode: specifies the SCI clock pin mode.
  *   This parameter can be one of the following values:
  *     @arg SCI_CLK_SOFTWARE       : SCI output clock is controlled by software
  *     @arg SCI_CLK_HARDWARE       : SCI output clock is controlled by hardware
  * @retval None
  ***********************************************************************************************************/
void SCI_ClockModeConfig(u32 SCI_CLKMode)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_CLK_MODE(SCI_CLKMode));

  if (SCI_CLKMode != SCI_CLK_SOFTWARE)
  {
    HT_SCI->CCR |= SCI_CLK_HARDWARE;
  }
  else
  {
    HT_SCI->CCR &= SCI_CLK_SOFTWARE;
  }
}

/*********************************************************************************************************//**
  * @brief  Output the SCI clock pin low or high by software.
  * @param  SCI_CLK: specifies if the SCI clock pin to be high or low.
  *   This parameter can be one of the following values:
  *     @arg SCI_CLK_HIGH           : Software drive SCI output clock high
  *     @arg SCI_CLK_LOW            : Software drive SCI output clock low
  * @retval None
  ***********************************************************************************************************/
void SCI_SoftwareClockCmd(u32 SCI_CLK)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_CLK(SCI_CLK));

  if (SCI_CLK != SCI_CLK_LOW)
  {
    HT_SCI->CCR |= SCI_CLK_HIGH;
  }
  else
  {
    HT_SCI->CCR &= SCI_CLK_LOW;
  }
}

/*********************************************************************************************************//**
  * @brief  Output the SCI DIO pin low or high by software.
  * @param  SCI_DIO: specifies if the SCI DIO pin to be high or low.
  *   This parameter can be one of the following values:
  *     @arg SCI_DIO_HIGH           : Drive SCI DIO signal high
  *     @arg SCI_DIO_LOW            : Drive SCI DIO signal low
  * @retval None
  ***********************************************************************************************************/
void SCI_OutputDIO(u32 SCI_DIO)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_DIO(SCI_DIO));

  if (SCI_DIO != SCI_DIO_LOW)
  {
    HT_SCI->CCR |= SCI_DIO_HIGH;
  }
  else
  {
    HT_SCI->CCR &= SCI_DIO_LOW;
  }
}

/*********************************************************************************************************//**
  * @brief  Enables or Disables the specified SCI interrupt.
  * @param  SCI_Int: specifies the SCI interrupt source to be enabled or disable.
  *   This parameter can be any combination of the following values:
  *     @arg SCI_INT_PAR             : SCI parity error interrupt
  *     @arg SCI_INT_RXC             : SCI received character interrupt
  *     @arg SCI_INT_TXC             : SCI transmitted character interrupt
  *     @arg SCI_INT_WT              : SCI waiting timer interrupt
  *     @arg SCI_INT_CARD            : SCI card insert/remove interrupt
  *     @arg SCI_INT_TXBE            : SCI transmit buffer empty interrupt
  * @param  NewState: new state of the SCI interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  ***********************************************************************************************************/
void SCI_IntConfig(u32 SCI_Int, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_INT(SCI_Int));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_SCI->IER |= SCI_Int;
  }
   else
  {
    HT_SCI->IER &= ~SCI_Int;
  }
}

/*********************************************************************************************************//**
  * @brief  Get the status of specified SCI flag.
  * @param  SCI_Flag: specifies the flag that is to be check.
  *   This parameter can be one of the following values:
  *     @arg SCI_FLAG_PAR              : SCI parity error flag
  *     @arg SCI_FLAG_RXC              : SCI received character flag
  *     @arg SCI_FLAG_TXC              : SCI transmitted character flag
  *     @arg SCI_FLAG_WT               : SCI waiting timer flag
  *     @arg SCI_FLAG_CARD             : SCI card insert/remove flag
  *     @arg SCI_FLAG_TXBE             : SCI transmit buffer empty flag
  * @retval The new state of SCI_Flag (SET or RESET).
  ***********************************************************************************************************/
FlagStatus SCI_GetFlagStatus(u32 SCI_Flag)
{
  u32 statusreg = 0;
  FlagStatus bitstatus = RESET;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_FLAG(SCI_Flag));

  statusreg = HT_SCI->SR;

  if ((statusreg & SCI_Flag) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  return bitstatus;
}

/*********************************************************************************************************//**
  * @brief  Clears the flag status of specified SCI flag.
  * @param  SCI_Flag: specifies the flag to be cleared.
  *   This parameter can be one of the following values:
  *     @arg SCI_FLAG_PAR               : SPI write collision flag
  *     @arg SCI_FLAG_TXC               : SPI read overrun flag
  *     @arg SCI_FLAG_WT                : SPI mode fault flag
  * @retval None
  ***********************************************************************************************************/
void SCI_ClearFlag(u32 SCI_Flag)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_CLEAR_FLAG(SCI_Flag));

  if (SCI_Flag != SCI_FLAG_WT)
  {
    HT_SCI->SR &= ~SCI_Flag;
  }
  else
  {
    HT_SCI->CR &= CR_WTEN_RESET;
    HT_SCI->CR |= CR_WTEN_SET;
  }
}

/*********************************************************************************************************//**
  * @brief  Enables or disables the SCI PDMA interface.
  * @param  SCI_PDMAREQ: specifies the SCI PDMA transfer request to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg SCI_PDMAREQ_TX             : Tx PDMA transfer request
  *     @arg SCI_PDMAREQ_RX             : Rx PDMA transfer request
  * @param  NewState: new state of the SCI PDMA request.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  ***********************************************************************************************************/
void SCI_PDMACmd(u32 SCI_PDMAREQ, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_SCI_PDMA_REQ(SCI_PDMAREQ));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_SCI->CR |= SCI_PDMAREQ;
  }
  else
  {
    HT_SCI->CR &= ~SCI_PDMAREQ;
  }
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

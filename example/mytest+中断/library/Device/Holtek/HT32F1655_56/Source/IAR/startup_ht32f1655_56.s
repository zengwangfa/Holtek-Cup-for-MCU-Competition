;/*---------------------------------------------------------------------------------------------------------*/
;/* Holtek Semiconductor Inc.                                                                               */
;/*                                                                                                         */
;/* Copyright (c) 2014 by Holtek Semiconductor Inc.                                                         */
;/* All rights reserved.                                                                                    */
;/*                                                                                                         */
;/*-----------------------------------------------------------------------------------------------------------
;  File Name        : startup_ht32f1655_56.s
;  Version          : V1.00
;  Date             : 2014-06-30
;  Description      : Startup code.
;-----------------------------------------------------------------------------------------------------------*/

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

;*******************************************************************************
; Fill-up the Vector Table entries with the exceptions ISR address
;*******************************************************************************
        DATA

__vector_table

        DCD  sfe(CSTACK)                        ; Top address of Stack
        DCD  Reset_Handler                      ; Reset Handler
        DCD  NMI_Handler                        ; NMI Handler
        DCD  HardFault_Handler                  ; Hard Fault Handler
        DCD  MemManage_Handler                  ; Memory Management Fault Handler
        DCD  BusFault_Handler                   ; Bus Fault Handler
        DCD  UsageFault_Handler                 ; Usage Fault Handler
        DCD  0                                  ; Reserved
        DCD  0                                  ; Reserved
        DCD  0                                  ; Reserved
        DCD  0                                  ; Reserved
        DCD  SVC_Handler                        ; SVC Handler
        DCD  DebugMon_Handler                   ; Debug Monitor Handler
        DCD  0                                  ; Reserved
        DCD  PendSV_Handler                     ; PendSV Handler
        DCD  SysTick_Handler                    ; SysTick Handler

        ; External Interrupt Handler
        DCD  CKRDY_IRQHandler
        DCD  LVD_IRQHandler
        DCD  BOD_IRQHandler
        DCD  WDT_IRQHandler
        DCD  RTC_IRQHandler
        DCD  FLASH_IRQHandler
        DCD  EVWUP_IRQHandler
        DCD  LPWUP_IRQHandler
        DCD  EXTI0_IRQHandler
        DCD  EXTI1_IRQHandler
        DCD  EXTI2_IRQHandler
        DCD  EXTI3_IRQHandler
        DCD  EXTI4_IRQHandler
        DCD  EXTI5_IRQHandler
        DCD  EXTI6_IRQHandler
        DCD  EXTI7_IRQHandler
        DCD  EXTI8_IRQHandler
        DCD  EXTI9_IRQHandler
        DCD  EXTI10_IRQHandler
        DCD  EXTI11_IRQHandler
        DCD  EXTI12_IRQHandler
        DCD  EXTI13_IRQHandler
        DCD  EXTI14_IRQHandler
        DCD  EXTI15_IRQHandler
        DCD  COMP_IRQHandler
        DCD  ADC_IRQHandler
        DCD  0
        DCD  MCTM0BRK_IRQHandler
        DCD  MCTM0UP_IRQHandler
        DCD  MCTM0TR_IRQHandler
        DCD  MCTM0CC_IRQHandler
        DCD  MCTM1BRK_IRQHandler
        DCD  MCTM1UP_IRQHandler
        DCD  MCTM1TR_IRQHandler
        DCD  MCTM1CC_IRQHandler
        DCD  GPTM0_IRQHandler
        DCD  GPTM1_IRQHandler
        DCD  0
        DCD  0
        DCD  0
        DCD  0
        DCD  BFTM0_IRQHandler
        DCD  BFTM1_IRQHandler
        DCD  I2C0_IRQHandler
        DCD  I2C1_IRQHandler
        DCD  SPI0_IRQHandler
        DCD  SPI1_IRQHandler
        DCD  USART0_IRQHandler
        DCD  USART1_IRQHandler
        DCD  UART0_IRQHandler
        DCD  UART1_IRQHandler
        DCD  SCI_IRQHandler
        DCD  I2S_IRQHandler
        DCD  USB_IRQHandler
        DCD  0
        DCD  PDMA_CH0_IRQHandler
        DCD  PDMA_CH1_IRQHandler
        DCD  PDMA_CH2_IRQHandler
        DCD  PDMA_CH3_IRQHandler
        DCD  PDMA_CH4_IRQHandler
        DCD  PDMA_CH5_IRQHandler
        DCD  PDMA_CH6_IRQHandler
        DCD  PDMA_CH7_IRQHandler
        DCD  0
        DCD  0
        DCD  0
        DCD  0
        DCD  0
        DCD  EBI_IRQHandler

        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER(1)
SysTick_Handler
        B SysTick_Handler


        PUBWEAK CKRDY_IRQHandler
        SECTION .text:CODE:REORDER(1)
CKRDY_IRQHandler
        B CKRDY_IRQHandler

        PUBWEAK LVD_IRQHandler
        SECTION .text:CODE:REORDER(1)
LVD_IRQHandler
        B LVD_IRQHandler

        PUBWEAK BOD_IRQHandler
        SECTION .text:CODE:REORDER(1)
BOD_IRQHandler
        B BOD_IRQHandler

        PUBWEAK WDT_IRQHandler
        SECTION .text:CODE:REORDER(1)
WDT_IRQHandler
        B WDT_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:REORDER(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:REORDER(1)
FLASH_IRQHandler
        B FLASH_IRQHandler

        PUBWEAK EVWUP_IRQHandler
        SECTION .text:CODE:REORDER(1)
EVWUP_IRQHandler
        B EVWUP_IRQHandler

        PUBWEAK LPWUP_IRQHandler
        SECTION .text:CODE:REORDER(1)
LPWUP_IRQHandler
        B LPWUP_IRQHandler

        PUBWEAK EXTI0_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI0_IRQHandler
        B EXTI0_IRQHandler

        PUBWEAK EXTI1_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI1_IRQHandler
        B EXTI1_IRQHandler

        PUBWEAK EXTI2_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI2_IRQHandler
        B EXTI2_IRQHandler

        PUBWEAK EXTI3_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI3_IRQHandler
        B EXTI3_IRQHandler

        PUBWEAK EXTI4_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI4_IRQHandler
        B EXTI4_IRQHandler

        PUBWEAK EXTI5_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI5_IRQHandler
        B EXTI5_IRQHandler

        PUBWEAK EXTI6_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI6_IRQHandler
        B EXTI6_IRQHandler

        PUBWEAK EXTI7_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI7_IRQHandler
        B EXTI7_IRQHandler

        PUBWEAK EXTI8_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI8_IRQHandler
        B EXTI8_IRQHandler

        PUBWEAK EXTI9_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI9_IRQHandler
        B EXTI9_IRQHandler

        PUBWEAK EXTI10_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI10_IRQHandler
        B EXTI10_IRQHandler

        PUBWEAK EXTI11_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI11_IRQHandler
        B EXTI11_IRQHandler

        PUBWEAK EXTI12_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI12_IRQHandler
        B EXTI12_IRQHandler

        PUBWEAK EXTI13_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI13_IRQHandler
        B EXTI13_IRQHandler

        PUBWEAK EXTI14_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI14_IRQHandler
        B EXTI14_IRQHandler

        PUBWEAK EXTI15_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTI15_IRQHandler
        B EXTI15_IRQHandler

        PUBWEAK COMP_IRQHandler
        SECTION .text:CODE:REORDER(1)
COMP_IRQHandler
        B COMP_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER(1)
ADC_IRQHandler
        B ADC_IRQHandler

        PUBWEAK MCTM0BRK_IRQHandler
        SECTION .text:CODE:REORDER(1)
MCTM0BRK_IRQHandler
        B MCTM0BRK_IRQHandler

        PUBWEAK MCTM0UP_IRQHandler
        SECTION .text:CODE:REORDER(1)
MCTM0UP_IRQHandler
        B MCTM0UP_IRQHandler

        PUBWEAK MCTM0TR_IRQHandler
        SECTION .text:CODE:REORDER(1)
MCTM0TR_IRQHandler
        B MCTM0TR_IRQHandler

        PUBWEAK MCTM0CC_IRQHandler
        SECTION .text:CODE:REORDER(1)
MCTM0CC_IRQHandler
        B MCTM0CC_IRQHandler

        PUBWEAK MCTM1BRK_IRQHandler
        SECTION .text:CODE:REORDER(1)
MCTM1BRK_IRQHandler
        B MCTM1BRK_IRQHandler

        PUBWEAK MCTM1UP_IRQHandler
        SECTION .text:CODE:REORDER(1)
MCTM1UP_IRQHandler
        B MCTM1UP_IRQHandler

        PUBWEAK MCTM1TR_IRQHandler
        SECTION .text:CODE:REORDER(1)
MCTM1TR_IRQHandler
        B MCTM1TR_IRQHandler

        PUBWEAK MCTM1CC_IRQHandler
        SECTION .text:CODE:REORDER(1)
MCTM1CC_IRQHandler
        B MCTM1CC_IRQHandler

        PUBWEAK GPTM0_IRQHandler
        SECTION .text:CODE:REORDER(1)
GPTM0_IRQHandler
        B GPTM0_IRQHandler

        PUBWEAK GPTM1_IRQHandler
        SECTION .text:CODE:REORDER(1)
GPTM1_IRQHandler
        B GPTM1_IRQHandler

        PUBWEAK BFTM0_IRQHandler
        SECTION .text:CODE:REORDER(1)
BFTM0_IRQHandler
        B BFTM0_IRQHandler

        PUBWEAK BFTM1_IRQHandler
        SECTION .text:CODE:REORDER(1)
BFTM1_IRQHandler
        B BFTM1_IRQHandler

        PUBWEAK I2C0_IRQHandler
        SECTION .text:CODE:REORDER(1)
I2C0_IRQHandler
        B I2C0_IRQHandler

        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:REORDER(1)
I2C1_IRQHandler
        B I2C1_IRQHandler

        PUBWEAK SPI0_IRQHandler
        SECTION .text:CODE:REORDER(1)
SPI0_IRQHandler
        B SPI0_IRQHandler

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:REORDER(1)
SPI1_IRQHandler
        B SPI1_IRQHandler

        PUBWEAK USART0_IRQHandler
        SECTION .text:CODE:REORDER(1)
USART0_IRQHandler
        B USART0_IRQHandler

        PUBWEAK USART1_IRQHandler
        SECTION .text:CODE:REORDER(1)
USART1_IRQHandler
        B USART1_IRQHandler

        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:REORDER(1)
UART0_IRQHandler
        B UART0_IRQHandler

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:REORDER(1)
UART1_IRQHandler
        B UART1_IRQHandler

        PUBWEAK SCI_IRQHandler
        SECTION .text:CODE:REORDER(1)
SCI_IRQHandler
        B SCI_IRQHandler

        PUBWEAK I2S_IRQHandler
        SECTION .text:CODE:REORDER(1)
I2S_IRQHandler
        B I2S_IRQHandler

        PUBWEAK USB_IRQHandler
        SECTION .text:CODE:REORDER(1)
USB_IRQHandler
        B USB_IRQHandler

        PUBWEAK PDMA_CH0_IRQHandler
        SECTION .text:CODE:REORDER(1)
PDMA_CH0_IRQHandler
        B PDMA_CH0_IRQHandler

        PUBWEAK PDMA_CH1_IRQHandler
        SECTION .text:CODE:REORDER(1)
PDMA_CH1_IRQHandler
        B PDMA_CH1_IRQHandler

        PUBWEAK PDMA_CH2_IRQHandler
        SECTION .text:CODE:REORDER(1)
PDMA_CH2_IRQHandler
        B PDMA_CH2_IRQHandler

        PUBWEAK PDMA_CH3_IRQHandler
        SECTION .text:CODE:REORDER(1)
PDMA_CH3_IRQHandler
        B PDMA_CH3_IRQHandler

        PUBWEAK PDMA_CH4_IRQHandler
        SECTION .text:CODE:REORDER(1)
PDMA_CH4_IRQHandler
        B PDMA_CH4_IRQHandler

        PUBWEAK PDMA_CH5_IRQHandler
        SECTION .text:CODE:REORDER(1)
PDMA_CH5_IRQHandler
        B PDMA_CH5_IRQHandler

        PUBWEAK PDMA_CH6_IRQHandler
        SECTION .text:CODE:REORDER(1)
PDMA_CH6_IRQHandler
        B PDMA_CH6_IRQHandler

        PUBWEAK PDMA_CH7_IRQHandler
        SECTION .text:CODE:REORDER(1)
PDMA_CH7_IRQHandler
        B PDMA_CH7_IRQHandler

        PUBWEAK EBI_IRQHandler
        SECTION .text:CODE:REORDER(1)
EBI_IRQHandler
        B EBI_IRQHandler


        END

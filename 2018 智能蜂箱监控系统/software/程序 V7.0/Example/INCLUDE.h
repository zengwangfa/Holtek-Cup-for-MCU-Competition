#ifndef _INCLUDE_H
#define _INCLUDE_H

#include "ht32.h"
#include "ht32_board.h"
#include "usart_int.h"
#include "stdlib.h"
#include "config.h"
#include "ADC.h"
#include "LED.h"
#include "KEY.h"
#include "PWM.h"
#include "UART.h"
#include "MOTOR.h"
#include "DELAY.h"
#include "INIT.h"
#include "HX711.h"
#include "DS18B20.h"
#include "spi_flash.h"
#include "ht32f1655_56.h" 
#include "ht32f1655_56_bftm.h"

#define FLASH_WriteAddress     0x100000
#define FLASH_ReadAddress      FLASH_WriteAddress
#define FLASH_SectorToErase    FLASH_WriteAddress
#define BufferSize  256

void All_Init(void);
void PART_Init(void);

#endif


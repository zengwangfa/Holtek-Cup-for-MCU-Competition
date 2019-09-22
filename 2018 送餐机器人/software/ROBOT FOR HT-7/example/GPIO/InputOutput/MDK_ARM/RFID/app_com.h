/* Copyright (C) ARM Ltd., 1999 */
/* All rights reserved */

/*
 * RCS $Revision: 137748 $
 * Checkin $Date: 2008-09-11 17:34:24 +0100 (Thu, 11 Sep 2008) $
 * Revising $Author: agrant $
 */
#include "stdint.h"
#ifndef __app_com_h
#define __app_com_h

void SPI1_Init(void);
void RequestA(void);
unsigned char RC500Main(void);
void NVIC_Configuration(void);
void RCC_Configuration(void);
int8_t RfidCardWrite(unsigned char addr,unsigned char *pData);
int8_t RfidCardRead(unsigned char addr,unsigned char *pData);
void USART1_Config(void);
void GPIO_Configuration(void);

#endif


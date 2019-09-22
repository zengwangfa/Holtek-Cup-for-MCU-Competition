

#include "mfrc522.h"
#include "ht32f1655_56_spi.h"
#include "stdint.h"
#include "app_com.h"
//#include "misc.h"


extern unsigned char  data2[4];
extern unsigned char  DefaultKey[6] ; 
extern unsigned char  g_ucTempbuf[20];    
extern unsigned char  MLastSelectedSnr[4];
extern unsigned char  RevBuffer[30];  
extern unsigned char  SerBuffer[20]; 
extern unsigned char  atq[2]; 
extern unsigned char	 ControlBlockAddr;
extern unsigned char	 DataBlockAddr;

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

 void RequestA()
{
	 char status;

  	do{
			status = PcdRequest(0x52, atq);	 
		}
	while(status);		
} 

unsigned char RC500Main()
{
//	unsigned char i;
	
	if (MI_OK != PcdAnticoll(MLastSelectedSnr))	   			//防冲撞	 
	return 01;											  
//  for(i=0;i<4;i++)
//	{
//	  g_cReceBuf[6+i]=g_cSNR[i];  						  
//	}
	if (MI_OK != PcdSelect(MLastSelectedSnr) )           //选择卡片  
	return 02;	

	if (MI_OK != PcdAuthState(PICC_AUTHENT1A,7,DefaultKey,MLastSelectedSnr)) 	  
	return 03;							  
	return MI_OK;						  
}


//void RCC_Configuration(void)
//{
//  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
//     initialize the PLL and update the SystemFrequency variable. */
//  SystemInit();
//  
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
//  /* Enable USART1, GPIOA, GPIOx and AFIO clocks */
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);
//  /* Enable USART2 clock */

//}

////void NVIC_Configuration(void)
////{
// // NVIC_InitTypeDef NVIC_InitStructure;

//  /* Configure the NVIC Preemption Priority Bits */  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//  
//  /* Enable the USART1 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//  
//}


void test()
{
		 RequestA();
		if(RC500Main() == MI_OK)
		{
			
		}

}
	   


//void USART1_Config(void)
//{
//	USART_InitTypeDef USART_InitStructure;
//  USART_InitStructure.USART_BaudRate = 9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

//	USART_Init(USART1, &USART_InitStructure); 
////	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
////	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
//	USART_Cmd(USART1, ENABLE);
//}
//void GPIO_Configuration(void)
//{ /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
//     initialize the PLL and update the SystemFrequency variable. */
// 	GPIO_InitTypeDef GPIO_InitStructure;
//  
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
//  /* Enable USART1, GPIOA, GPIOx and AFIO clocks */
// 	SystemInit();
//  /* Enable USART2 clock */


// RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);
//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2,ENABLE);
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
//                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
//    	
// 
//  /* Enable SPI2 GPIOB clocks */

//  //PA5--C PA6--Q  PA7--D  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_3;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  /* Configure PB.12 as Output push-pull, used as Flash Chip select */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);	

//}

/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 	
int8_t RfidCardRead(unsigned char addr,unsigned char *pData)
{
		char status;
		int8_t i = 0x0a;
	
//	RequestA();

		do{
			status = PcdRequest(0x52, atq);																				//寻卡
			i--;
		}		while((status != MI_OK) && (i != 0));	

		if(PcdAnticoll(MLastSelectedSnr) == MI_OK)		                          //防冲撞MLastSelectedSnr[4]序列号存储单元
		{
			if(PcdSelect(MLastSelectedSnr) == MI_OK)															//选卡
			{
				if(PcdAuthState(0x60,ControlBlockAddr,DefaultKey,MLastSelectedSnr) == 	MI_OK)	       //验证A密匙
				{
					if(PcdRead(DataBlockAddr,pData) == MI_OK)																										//读取数据
					{
						return MI_OK;
					}

			   }

			}
		}	
		return MI_ERR;
}		
/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////     	

int8_t RfidCardWrite(unsigned char addr,unsigned char *pData)
{
		char status;
		int8_t i = 0x0a;

		do{
			status = PcdRequest(0x52, atq);																				//寻卡
			i--;
		}		while((status != MI_OK) && (i != 0));	

		if(PcdAnticoll(MLastSelectedSnr) == MI_OK)		                           //MLastSelectedSnr[4]序列号存储单元
		{
			if(PcdSelect(MLastSelectedSnr) == MI_OK)
			{
				if(PcdAuthState(0x60,ControlBlockAddr,DefaultKey,MLastSelectedSnr) == 	MI_OK)	       //A密匙
				{
					if(PcdWrite(DataBlockAddr,pData) == MI_OK)
					{
						return MI_OK;
					}
			   }

			}
		}	
		return MI_ERR;
}




/* Includes ------------------------------------------------------------------------------------------------*/
#include "INCLUDE.h"

#define FLASH_WriteAddress     0x100000
#define FLASH_ReadAddress      FLASH_WriteAddress
#define FLASH_SectorToErase    FLASH_WriteAddress
#define BufferSize  256

//------------------��ʱ���ж��н��еĺ���-------------------------------------//
extern u16 temperature,temp_positive,temp_ten,temp_unit,temp_decimal;//�����¶��β�
extern u8 Temp_Ten_Cmd[],Temp_decimal_Cmd[];////���������͵�����
extern void UART_Send_Temp(void);//���÷����¶Ⱥ���
extern void UART_Send_Weight(void);//���÷�����������
extern bool          HEAT_START_FLAG;                   //��ʼ���Ƽ���˿���ȱ�־λ
extern bool          FEED_START_FLAG;                   //��ʼιʳ�����־λ
extern bool          START_WASHING_FLAG;                //��ʼ��ϴ�����־λ
unsigned long HX711AD_Value_out=0;
u8 SECOND_Count=0;       
u8 DataBuffer[8];   //���������ݴ����飬����2�ֽڵ�����+2�ֽڵ���������+2�ֽڵ��¶�+1�ֽڵ�(��ϴ�����ȡ�ιʳ)��־λ+1�ֽڵķ���״̬
u8 TEXT_Buffer[2]={0X04};    //��һ���ֽ�Ϊ����Ŀ���״̬��0x04��ʾ�������0x02���У�0x01��С
u8 temp; 
vu32 FLASH_ID = 0;
u32 FLASH_Init_OK;
float Volet;

void DataBuffer_Deal(void);

int main(void)
{
	All_Init();     //ϵͳ�Լ���Χ�豸��ʼ��
	
//      /* Initialize the SPI_FLASH driver                                                                        */
//		FLASH_Init_OK = SPI_FLASH_Init();
//	
//		if (FLASH_Init_OK == TRUE)LED1=1;
//		else LED1=0;

//		FLASH_ID = SPI_FLASH_ReadJEDECID();        //��FLASH ID
//		SPI_FLASH_WriteStatus(0x00);               //�������λ
//		SPI_FLASH_SectorErase(FLASH_SectorToErase);//���� FLASH ����
//	
//		SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,sizeof(TEXT_Buffer));	//д����俪��״̬
//	  SPI_FLASH_BufferDualRead((u16*)TEXT_Buffer, FLASH_WriteAddress,sizeof(TEXT_Buffer)/2);	//��ȡ���俪��״̬,�������鵱��
//	
//	  if(TEXT_Buffer[1]!=0x55)	
//		{
//			TEXT_Buffer[0]=0x04;
//			TEXT_Buffer[1]=0x55;
//			SPI_FLASH_BufferWrite(TEXT_Buffer,FLASH_WriteAddress,sizeof(TEXT_Buffer));	//д����俪��״̬
//		}	
			TEXT_Buffer[0]=0x04;
			TEXT_Buffer[1]=0x55;
	while(1)
	{
		//Volet = Get_AD_Value()*8.02;//��ѹ�ɼ�
		Uart0DataAnalyzing(); //�������ݽ���
		KYE1_SHOW();  //�л��鿴����״̬
		KYE2_SHOW();
		PWM_Control(); 
		DataBuffer_Deal();
	}
}

void DataBuffer_Deal()
{
		
			
		DataBuffer[6]=HEAT_START_FLAG;                     //�����ȱ�־λ�������飬���忴ͨ�����ݸ�ʽ
		DataBuffer[6]=DataBuffer[6]<<1;                    //00000010�������ڼ���
		DataBuffer[6]=DataBuffer[6]+FEED_START_FLAG;        //��ιʳ��־λ�������飬���忴ͨ�����ݸ�ʽ
		temp=START_WASHING_FLAG;
		DataBuffer[6]=DataBuffer[6]+(temp<<2);
		
//		if(SECOND_Count == 1)
//		{
			temperature=DS18B20_Get_Temp();	
			DataBuffer[5]=(temperature+500);                     //2�ֽڵ��¶����ݷ���������
			DataBuffer[4]=(temperature+500)>>8;
//		}
//		else if(SECOND_Count == 2)
//		{
			DataBuffer[0]=HX711_Read1()>>16;                  //2�ֽڵķ����������ݷ�������
			DataBuffer[1]=HX711_Read1()>>8;
			DataBuffer[2]=0xff;                               //2�ֽڵ������������ݷ�������(ffff�������ϳ���)
			DataBuffer[3]=0xff;
			SECOND_Count=0;
//		}
		
}



#include "MOTOR.h"

unsigned char F_Rotation[8]={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09} ;  //��˫8�ķ�ת��
unsigned char B_Rotation[8]={0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08} ;  //��˫8����ת��

void Motor_Init()   //���������ʼ��
{
	
	AFIO_GPxConfig(STEP_AFIO_PORT, STEP_A_AFIO, AFIO_MODE_DEFAULT);   //�޸��ù���
	AFIO_GPxConfig(STEP_AFIO_PORT, STEP_B_AFIO, AFIO_MODE_DEFAULT);  
	AFIO_GPxConfig(STEP_AFIO_PORT, STEP_C_AFIO, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(STEP_AFIO_PORT, STEP_D_AFIO, AFIO_MODE_DEFAULT);

	GPIO_DirectionConfig(STEP_GPIO_PORT, STEP_A_GPIO, GPIO_DIR_OUT);
	GPIO_DirectionConfig(STEP_GPIO_PORT, STEP_B_GPIO, GPIO_DIR_OUT);
	GPIO_DirectionConfig(STEP_GPIO_PORT, STEP_C_GPIO, GPIO_DIR_OUT);
	GPIO_DirectionConfig(STEP_GPIO_PORT, STEP_D_GPIO, GPIO_DIR_OUT);
	
	GPIO_OpenDrainConfig(STEP_GPIO_PORT,STEP_A_GPIO,DISABLE);				//�������
	GPIO_OpenDrainConfig(STEP_GPIO_PORT,STEP_B_GPIO,DISABLE);				//�������
	GPIO_OpenDrainConfig(STEP_GPIO_PORT,STEP_C_GPIO,DISABLE);				//�������
	GPIO_OpenDrainConfig(STEP_GPIO_PORT,STEP_D_GPIO,DISABLE);				//�������

	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_A_GPIO, RESET); 
	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_B_GPIO, RESET); 
	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_C_GPIO, RESET); 
	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_D_GPIO, RESET); 
}

void Motor_Stop()
{
	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_A_GPIO, RESET); 
	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_B_GPIO, RESET); 
	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_C_GPIO, RESET); 
	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_D_GPIO, RESET); 
}


void SetMotor(unsigned char InputData) //���ݴ��͵�����Inputdata�����������
{
    if(InputData & 0x08)
    {
       	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_A_GPIO, RESET); 
    }
    else
    {
        GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_A_GPIO, SET); 
    }
    if(InputData & 0x04)
    {
       	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_B_GPIO, RESET); 
    }
   else
    {
        GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_B_GPIO, SET); 
    }
    if(InputData & 0x02)
    {
       	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_C_GPIO, RESET); 
    }
    else
    {
        GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_C_GPIO, SET); 
    }
   if(InputData & 0x01)
    {
       	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_D_GPIO, RESET); 
    }
   else
   {
       	GPIO_WriteOutBits(STEP_GPIO_PORT, STEP_D_GPIO, SET); 
   }
}


void Motor_NCircle(int n,bool position)//nΪ�������ת����Ȧ����positionΪ��ת���߷�ת
{
   int i=0;
   int j=0;
   int k=0;
   for(j=0;j<n;j++)
   {
      for(i=0;i<64*8;i++)     
      {
          for(k=0;k<8;k++)
          {
              if(TRUE == position)
                 SetMotor(F_Rotation[k]);  
             else
                 SetMotor(B_Rotation[k]);  
								 delay_us(1000);       
          }
      }
    }
}



void Motor_NAngle(int n,bool position)        //nΪ�������ת���ĽǶ�Ϊn*120��positionΪ��ת���߷�ת
{
   int i=0;
   int j=0;
   int k=0;
   for(j=0;j<n;j++)
   {
      for(i=0;i<170;i++)     
      {
          for(k=0;k<8;k++)
          {
              if(TRUE == position)
                 SetMotor(F_Rotation[k]);  
             else
                 SetMotor(B_Rotation[k]);  
             	delay_us(1000);       
          }
      }
    }
}



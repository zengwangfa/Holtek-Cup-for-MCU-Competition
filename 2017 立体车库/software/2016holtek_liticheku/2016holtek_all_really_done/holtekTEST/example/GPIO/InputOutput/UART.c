#include "ht32.h"
#include "ht32_board.h"
#include "config.h"
#include "action.h"
#include "controll_main.h"
#include "usart_int.h"
#include "stdlib.h"
unsigned int a;
u16 oldword[13];//����ԭֵ
u16 newword[4];//�û�����������ݴ�
u8 password[5];

extern u8 f;
extern u8 t;
extern u8 full;
extern u8 empty;
 u8 garagenews[13][6]={
	  0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a,
    0,0,0,0,0,0x0a};//��0��Ϊ�ֿ��ȡ״̬��־λ����1-4��Ϊ����
                    // ��5��Ϊ����ָ��
void PCexchange(void)
{
	u8 tx;//����ָ���жϺ�
	    if (!IS_BUFFER_EMPTY(UR0RxReadIndex, UR0RxWriteIndex))//����ǰ��
    {
      tx = UR0RxBuf[UR0RxWriteIndex -1];
		 UR0RxReadIndex = (UR0RxReadIndex + 1) % USART_BUFSIZE;
			switch(tx)
			{
				case 0x46: ResetShaft();break;//���յ�F����λϵͳ			           
				case 0x53: storage();break; //���յ�S,����ֿ�
				case 0x47: tack();break; //���յ�G����������ȡ
				default:break;
			}
     
		}
}

void showgarage(u8 fu,u8 em) //��ʾ�ֿ�״̬
{

	u8 station[17]= {0x46,0x55,0x4c,0x4c,0x3a,0,0,0x0a,
		                    0x45,0x4d,0x50,0x54,0x59,0x3a,0,0,0x0a};
	u8 sta;
	em = 12-fu;											
	BFTM_IntConfig(HT_BFTM1, ENABLE);//ENABLEΪ1
	BFTM_IntConfig(HT_BFTM0,  ENABLE);//ENABLEΪ1											
	empty = 12 - full;
	station[5] = fu/10 + 0x30;
	station[6] = fu%10 + 0x30;
  station[14] = em/10 + 0x30;
  station [15] = em%10 + 0x30;											
	 for (sta = 0; sta < 17; sta++)
    {
      if (!IS_BUFFER_FULL(UR0TxReadIndex, UR0TxWriteIndex))//����ǰ��
      {
        /* 0x41 = ASCII "0"                                                                                 */
        UR0TxBuf[UR0TxWriteIndex] = station[sta];
        UR0TxWriteIndex = (UR0TxWriteIndex + 1) % USART_BUFSIZE;
      }
    }
    USART_IntConfig(HT_USART0, USART_IER_THREIE, ENABLE);	
	}
void showpassword(u8 b)
{
	u8 sta;
		 for (sta = 1; sta < 6; sta++)
    {
      if (!IS_BUFFER_FULL(UR0TxReadIndex, UR0TxWriteIndex))//����ǰ��
      {
        /* 0x41 = ASCII "0"                                                                                 */
        UR0TxBuf[UR0TxWriteIndex] = garagenews[b][sta];
        UR0TxWriteIndex = (UR0TxWriteIndex + 1) % USART_BUFSIZE;
      }
    }
    USART_IntConfig(HT_USART0, USART_IER_THREIE, ENABLE);	
	}

void storage(void)
{
  u8 i;//�ֿ����λ
	u8 g=0; //�����ֿ��

	for(i=1;i<13;i++)
	{
		if(garagenews[i][0] == 0)
		 {
			 garagenews[i][0]= 1;	
       g = i;			 
       i = 13;			 
		 }		
		  
		  
	 }
	  	full += 1;
			 showgarage(full,empty);
			 random_data(g);//��������	
	}
void tack(void)
{
	u8 pass[22]={0x50,0x6c,0x65,0x61,0x73,0x65,0x20,//Please
	             0x49,0x6e,0x70,0x75,0x74,0x20,   //Input
	             0x50,0x61,0x73,0x73,0x77,0x6f,0x72,0x64,0x0a};//Password
	u16 match = 0;//����ƥ��ֵ
	u8 h = 0;//�ֿ�����ԭֵ������
	u8 m = 0;//
	u8 k = 1;
	u8 i;
		for(i=0;i<22;i++)
	 {
      if (!IS_BUFFER_FULL(UR0TxReadIndex, UR0TxWriteIndex))//����ǰ��
      {
        /* 0x41 = ASCII "0"                                                                                 */
        UR0TxBuf[UR0TxWriteIndex] = pass[i];//��ʾ�û��������������־
        UR0TxWriteIndex = (UR0TxWriteIndex + 1) % USART_BUFSIZE;
      }
       USART_IntConfig(HT_USART0, USART_IER_THREIE, ENABLE);
		}
    while(k)
		{			
			    if (!IS_BUFFER_EMPTY(UR0RxReadIndex, UR0RxWriteIndex))//����ǰ��
       {
           newword[m] = UR0RxBuf[UR0RxWriteIndex -1];
				     m++;
		       UR0RxReadIndex = (UR0RxReadIndex + 1) % USART_BUFSIZE;
			 }
			    if(m == 4)//��ʾ�ѽ��յ��ĸ�����
			   {
				    match = newword[0]*1000+newword[1]*100+newword[2]*10+newword[3];
					 for(h =0;h<13;h++)
					 {
					   if(match ==oldword[h] )
						 {
							  garagenews[h][0]= 0;  
							 	  	full -= 1;
			        showgarage(full,empty);
							 getaction(h);   
			         h  = 13;
						 }
			     }
					 k = 0;
         }
     }
}	
void random_data(u8 w)//���ݲֿ�Ž���
{
	u8 k = 1;//ѭ��λ
	u8 m = 1;
	while(k)
	{
		while(m)
		{
//	  srand(t);
//		 a = (rand()%(255-50+1)+50);
		  garagenews[w][1] = t%f + 0X30;
		  garagenews[w][2] = t%(f+1)+ 0X30;
		  garagenews[w][3] = t%(f+2)+0X30;
		  garagenews[w][4] = t%(f+3)+0X30;
 oldword[w] = garagenews[w][1]*1000+garagenews[w][2]*100+garagenews[w][3]*10+garagenews[w][4];
		if(oldword[w] != 0)
		{
			 m = 0;
		}
	}
		 showpassword(w);
     storeaction(w);
		 k = 0;
	}
}


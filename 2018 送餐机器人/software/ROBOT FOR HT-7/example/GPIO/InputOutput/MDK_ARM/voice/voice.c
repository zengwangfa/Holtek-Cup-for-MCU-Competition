#include "voice.h"
#include "usart_int.h"

u8 music_dis=0;

unsigned char music_num[7]={0x7e,0x05,0x42,0x00,0x00,0x00,0xef};   //����ָ���ļ����еĸ���
unsigned char insert_music[7]={0x7e,0x05,0x44,0x00,0x00,0x00,0xef};  //�岥�ļ����и���

void chose_music(unsigned char file,unsigned char number)   //ָ���ļ����е���Ŀ����
{
	
		//if(music_dis == 0)
	  {
	  	unsigned char mp,n=0;
			music_num[3] = file;
			music_num[4] = number;
		  mp= music_num[1] ^ music_num[2] ^ music_num[3] ^ music_num[4];  //У��λ����
			music_num[5] = mp;
		  for(n=0;n<7;n++)   
		  {
				USART_SendData(HT_USART1,music_num[n]);
		  }
			//music_dis =1;
    }
}

void insert_filemusic(unsigned char file,unsigned char number)  //�岥�ļ����и���
{
  unsigned char mp,n=0;
	insert_music[3] = file;
	insert_music[4] = number;
	mp= insert_music[1] ^ insert_music[2] ^ insert_music[3] ^ insert_music[4];  //У��λ����
	insert_music[5] = mp;
	for(n=0;n<7;n++)   
	{
		USART_SendData(HT_USART1,insert_music[n]);
	}
}



#include "voice.h"
#include "usart_int.h"

u8 music_dis=0;

unsigned char music_num[7]={0x7e,0x05,0x42,0x00,0x00,0x00,0xef};   //播放指定文件夹中的歌曲
unsigned char insert_music[7]={0x7e,0x05,0x44,0x00,0x00,0x00,0xef};  //插播文件夹中歌曲

void chose_music(unsigned char file,unsigned char number)   //指定文件夹中的曲目播放
{
	
		//if(music_dis == 0)
	  {
	  	unsigned char mp,n=0;
			music_num[3] = file;
			music_num[4] = number;
		  mp= music_num[1] ^ music_num[2] ^ music_num[3] ^ music_num[4];  //校验位计算
			music_num[5] = mp;
		  for(n=0;n<7;n++)   
		  {
				USART_SendData(HT_USART1,music_num[n]);
		  }
			//music_dis =1;
    }
}

void insert_filemusic(unsigned char file,unsigned char number)  //插播文件夹中歌曲
{
  unsigned char mp,n=0;
	insert_music[3] = file;
	insert_music[4] = number;
	mp= insert_music[1] ^ insert_music[2] ^ insert_music[3] ^ insert_music[4];  //校验位计算
	insert_music[5] = mp;
	for(n=0;n<7;n++)   
	{
		USART_SendData(HT_USART1,insert_music[n]);
	}
}



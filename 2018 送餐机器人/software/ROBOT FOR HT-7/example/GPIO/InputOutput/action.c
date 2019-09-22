#define _ACTION_C
#include "config.h"
#include "key.h"
#include "controll_main.h"
#include "action.h"
uint8 No;
uint8 Room;
_Bool model;  //为0 store模式 为1是get模式
void XMove(uint8 num,uint8 model)
{  
	    uint8 N;
	    uint8 k = 1;
	    uint8 count = 0;
	    if(num == 0) //指向预备仓
	    {
	        N = num+1;
	    }
	    else
	    {
		      N = num%3;
	        if(N == 0)
		      N = N+3;
	        if(model == 1) 
		      N = N+1;
	    }

	    dir = 0;
	    Cy = 0;
	    Cx = 1;
	    while(k)
	    {
		      if(Xm == 1)
		      {
						   delay_wt();
			         while(Xm == 1);
			         count += 1;
			         if(count == N)
				       {
					         delayX(); 
					         Cy = 1;
					         k = 0;
				       }
		      }
	    }

}

void ResetShaft(void)
{
   Init();
	 dir = 1;   
	 if(Zb == 1)  
   {
		  ZMoveback();
	    delay(100);
   }
   if(Yd == 0)   
   {  
      Cx = 0; 
      Cy = 1;		 
  	  while(Yd == 0) 
      {
			}				
      Cx = 1;   

   }

   if(Xl == 0)  
   {
	    Cy = 0; 
      Cx = 1;		 
		  while(Xl == 0)
      {
			
			}				
		  Cy = 1;      
   }
}
 
void floors(uint8 flo) 
 { 
	 	 if((flo >=1)&&(flo<=3))
		 No  =1;
	   else if((flo >= 4)&&(flo <= 6))
	   No = 2;
	   else if((flo >= 7)&&(flo <= 9))
		 No = 3;
	   else if((flo >= 10)&&(flo <= 12))
     No = 4;
 }
 
void Get(uint8 get)
{
	   uint8 k = 1;
		 uint8 count = 0;
		 Init();
	   dir = 0;
	   floors(get);
	   Cx = 0;
	   while(k)
	   {
		     if(Ym == 1)
		     {
					   delay_wt();
					   if(Ym == 1)
						 {	 
			        count += 1;
			        delay_wt();
			        if(count == No)
				      {
					     Cx = 1;
					     ZMovefront();
					     dir = 0;
					     Cx = 0;
					     delayY();
					     while(Ym == 1);
					     Cx = 1;
					     ZMoveback();
					     k = 0;
				      }
		           else 
		  	       while(Ym == 1);
						 }
		     }
	  } 
}

void Store(uint8 store)
{
	   uint8 k = 1;
		 uint8 count = 0;
	   Init();
	   dir = 0;
	   floors(store); 
	   Cx = 0;
	   delay(100);
//   if(No == 4)
//	 {
//		        while(Yu == 0);	 
//		 					     Cx = 1;
//					        ZMovefront();
//					        dir = 1;
//					        Cx = 0;
//								  Cy = 1;
//					        delayY();
//					        while(Ym == 1);
//					        Cx = 1;
//					        ZMoveback();
//					        k = 0;
//	 }
//   else
//	 {		
  		 while(k)
	   {
		   if(Ym == 1) 
		     {
					  delayY();
					     if(Ym == 1)
							 {GPIO_WriteOutBits(HT_GPIOD,GPIO_PIN_7,SET);
					    
		          while(Ym == 1)
							{
							    
							}
						  GPIO_WriteOutBits(HT_GPIOD,GPIO_PIN_7,RESET);   
			        count += 1;
			      if(count == No )
				      {
					        Cx = 1;
					        ZMovefront();
					        dir = 1;
					        Cx = 0;
								  Cy = 1;
					        delayY();
					        while(Ym == 1);
					        Cx = 1;
					        ZMoveback();
					        k = 0;
							}
						}
		     }
			 
	   } 	
}
void storeaction(u8 Room1)
{
                XMove(0x00,0);
                Get(0x01);
                if(Yd == 0) 
                { 
                    dir = 1;
                    Cx = 0;
                    while(Yd == 0);
                    Cx = 1;
                 }
                    XMove(Room1,0);
                    Store(Room1);
                    ResetShaft();
}
void getaction(u8 Room2)
{
	         	XMove(Room2,1);
	        Get(Room2);
	        ResetShaft();
	        XMove(0x00,0);
                Store(0x01);
                ResetShaft();
}
void KeyAction(uint8 keycode)
{  
   	 if  ((keycode>=0x01) && (keycode<=0x0c))  
     {
          Room = keycode;
     }
     switch(keycode)
     {
      case 0x25: storeaction(Room); break;   
     case 0x28: getaction(Room) ;break;    
     case 0x26:  ResetShaft(); break;                
     case 0x27:     break;           
     default: 
     break;
     }


}


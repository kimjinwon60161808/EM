#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "led.h"

static int fd = 0;
static int ledStatus = 0;
int ledLibInit(void)// 이 라이브러리는 led를 켜기 전에 반드시 한번 불러주어야한다. 
{
   fd = open (LED_DRV_NODE, O_RDWR);
   if(fd < 0){
      printf("Device Open Error\r\n");
      exit(1);
   }
   ledStatus = 0;
   ledLibRaw(ledStatus);
   return 1;
}
int ledLibOnOff(int ledNumber, int onOff)//이 함수는 ledNumber에 0~7 사이의 값을 넣으면 그에 해당하는 led를 켜거나 끄게 해준다.
{
   int lamp = 1;
   lamp = lamp << ledNumber; // 0001000 해당하는 전구비트만 1로 바꾼다.
   ledStatus = (ledStatus &(~lamp));// 해당하는 전구비트만 0으로 바꾼다.
   if (onOff)
      ledStatus = ledStatus | lamp;//해당하는 전구비트만 1로 바꾸고 나머지는 그대로 유지한다.
   ledLibRaw(ledStatus);
   return 1;
}
int ledLibStatus(void)//이 함수를 호출하면 현재 led상태가 어떤지 확인해 준다.
{
   return ledStatus;
}
int ledLibExit(void)//이 함수를 호출하면 led를 전부 끄고 driver를 close한다.
{
   ledLibRaw(0); // 전부 끔
   close(fd);
}
int ledLibRaw(char ledwannabe)//이 함수를 호출하면 ledwannabe에 맞게 8개의 led를 전부 제어한다.
{
   if (fd > 0)
   {
      write(fd,&ledwannabe,1);
   }
   else{
   printf("WRONG! you must open device node\r\n");
   exit(2);
   }
   ledStatus = ledwannabe;
   return 1;
}
int ledLiblink(int ledNumber, int nth, int  msec)//1~8까지의 led를 하나 골라서 msec 주기로 깜빡인다.
{
	ledLibMorseBlink(ledNumber, nth, msec, msec);
}
int ledLibMorseBlink (int ledNumber ,int nth, int msecOn, int msecoff)
{
	int i=0;
	for (i=0; i<nth; i++)
	{
		ledLibOnOff(ledNumber, 1); //led on
		usleep(1000*msecOn);
		ledLibOnOff(ledNumber, 0); //led off
		usleep(1000*msecoff);
		
	}
}

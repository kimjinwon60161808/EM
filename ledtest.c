#include <stdio.h>
#include "led.h"

int main (void)
{
	ledLibInit();
	
	ledLibRaw(0xFF);
	sleep(1);
	ledLibRaw(0xAA);
	sleep(1);
	ledLibRaw(0x55);
	sleep(1);
	ledLibRaw(0x0);
	int i=0;
	
	for (i=0; i<8; i++)
	{
		ledLibOnOff(i, 1);
		printf ("0x%02X--ok? \r\n",ledLibStatus());
		usleep(200*1000);
		ledLibOnOff(i, 0);
		usleep(200*1000);
		
	}
	
	ledLibBlink(1, 10, 500);
	
	ledLibMorseBlink(2, 3, 500, 500);	//oN 0.5, off 0.5 3번 깜빡임 == S
	ledLibMorseBlink(2, 3, 1500, 500);	//On 1.5, off 0.5 3번 깜빡임 == O
	ledLibMorseBlink(2, 3, 500, 500);	//On 0.5, off 0.5 3번 깜빡임 == S
	
	sleep(2);	//We should check.
	ledLibExit();
}

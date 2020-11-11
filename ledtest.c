#include <stdio.h>
#include "led.h"

int main(void){
   ledLibInit(); //ledLibInit을 확인한다.
   
   ledLibRaw(0xFF);
   sleep(1);
   ledLibRaw(0xAA);
   sleep(1);
   ledLibRaw(0x55);
   sleep(1);
   ledLibRaw(0x0);
   int i=0;
   for(i = 0; i < 8; i++){
         ledLibOnOff(i, 1); //ledLibOnOff을 확인한다.
         printf("0x%02X---- ok?\r\n",ledLibStatus());//ledLibStatus가 동작하는지 확인한다.
         usleep(200*1000);
         ledLibOnOff(i, 0);
         usleep(200*1000);
   }
   ledLiblink(1, 10, 500);//ledLibblink() 확인 5번 led 10번 100ms간격으로 깜빡인다.
   
   ledLibMorseBlink(2, 3, 500, 500); // on 0.5 off 0.5 3번 깜빡인다 == S
   ledLibMorseBlink(2, 3, 1500, 500); // on 1.5 off 0.5 3번 깜빡인다 == O
   ledLibMorseBlink(2, 3, 500, 500); // on 0.5 off 0.5 3번 깜빡인다 == S
   sleep(2); //we whould check 
   ledLibExit();
}

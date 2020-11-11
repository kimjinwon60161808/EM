#ifndef _LED_LIBRARY_H_
#define _LED_LIBRARY_H_

int ledLibInit(void); // 이 lib는 led를 켜기 전 반드시 한번 불러 주어야 함 
int ledLibOnOff(int ledNumber, int onOff); //이 함수는 ledNumber에 0~7 사이의 값을 넣으면 그에 해당하는 led를 켜거나 끄게 해준다.
int ledLibStatus(void); //이 함수를 호출하면 현재 led상태가 어떤지 확인해 준다. 만약 모든 led가 켜져 있으면 0xff리턴 다 꺼져있으면 0x00리턴
int ledLibExit(void); //이 함수를 호출하면 led를 전부 끄고 driver를 close한다.
int ledLibRaw(char ledwannabe); //이 함수를 호출하면 ledwannabe에 맞게 8개의 로led를 전부 제어한다. ex)ledLibRaw(0x80): 한개만 켜짐
int ledLiblink(int ledNumber,int nth, int  msec); //1~8까지의 led를 하나 골라서 n번 깜빡이는데, msec 주기
int ledLibMorseBlink(int ledNumber, int nth, int msecOn, int msecoff);


#define LED_DRV_NODE "/dev/periled"

#endif

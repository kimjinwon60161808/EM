#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include "button.h"

#define INPUT_DEVICE_LIST "/dev/input/event"
//실제 디바이스 드라이버 노드파일: 뒤에 숫자붙음! ex)event5
#define PROBE_FILE "/proc/bus/input/devices"
//ppt에 제시된 event?의 숫자를 볼 수 있는 그 파일!
static int msgID;
struct input_event stEvent;
int main(int argc, char *argv[])
{
	msgID=buttonInit();
	int returnValue = 0;
	BUTTON_MSG_T msgrecieve;
	msgrecieve.messageNum = 1;
	returnValue = msgrcv(msgID, &msgrecieve, sizeof(int), 0 , IPC_NOWAIT);
	if(returnValue<0) printf("receive fail\r\n");
	else{
	if(stEvent.type == EV_KEY)
		printf("EV_KEY:(");
		switch(stEvent.code)
		{
			case KEY_VOLUMEUP:      printf("Volume up key):");      break;
			case KEY_HOME:            printf("Home key):");            break;
			case KEY_SEARCH:         printf("Search key):");         break;
			case KEY_BACK:            printf("Back key):");            break;
			case KEY_MENU:            printf("Menu key):");            break;
			case KEY_VOLUMEDOWN:   printf("Volume down key):");   break;
		}
	}
		printf("\n\n"); 
		buttonExit();
		return 0;
}

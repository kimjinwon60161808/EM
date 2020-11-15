#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/msg.h>
#include <pthread.h>
#include "button.h"
//first read input device
#define INPUT_DEVICE_LIST "/dev/input/event"
#define INPUT_PATH "/dev/input/event5/"
#define PROBE_FILE "/proc/bus/input/devices"
#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=kbd event"

static int fd =0;
pthread_t buttonTh_id;
char inputPath[200]={0,};
int probeButtonPath(char *newPath)
{
	int returnValue = 0;
	int number =0;
	FILE *fp = fopen(PROBE_FILE,"rt");
//define HAVE_TO_FIND_1 "N: Name=\"ecube-button\n"
//	#define HAVE_TO_FIND_2 "H: Handlers=kbd event"
	while(!feof(fp))
	{
		char tmpStr[200];
		fgets(tmpStr,200 ,fp);
		//printf("%s",tmpStr);
		if(strcmp(tmpStr, HAVE_TO_FIND_1) ==0)
		{
			printf("yes i found!: %s\r\n",tmpStr);
			returnValue =1; // 찾음
		}
		if((returnValue ==1) && (strncasecmp(tmpStr, HAVE_TO_FIND_2, strlen(HAVE_TO_FIND_2)) ==0))
		{
			printf("--->%s",tmpStr);
			printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
			number = tmpStr[strlen(tmpStr)-3] - '0';
			break;
		}
	}
	fclose(fp);
	if(returnValue ==1)
		sprintf (newPath,"%s%d",INPUT_DEVICE_LIST, number);
	return returnValue;
}

void* buttonThFunc(void *arg)
{
	BUTTON_MSG_T buttonsend;
	buttonsend.messageNum = 1;
	struct input_event Event[100];
	int returnval=0;
	int readval=0;
	while(1)
	{
		read(fd, Event, sizeof(Event));
	//	if(readval ==-1) printf("read error!\r\n");
		if( Event[100].type == EV_KEY)
		{	
			buttonsend.keyInput = Event[100].code;
			if(Event[100].value)
			{
				buttonsend.pressed = Event[100].value;
				returnval =msgsnd(1122,&buttonsend, sizeof(int),0);
				printf("%d\r\n",returnval);
				if(returnval<0) printf("send fail\r\n");
			}
		}
	}
}

int buttonInit(void)
{ 
	//char *buttonPath=0;
	int msgID;
	int buttonTh_id;
	int err;
	
	if (probeButtonPath(inputPath) ==0)
		return 0;
	fd = open ("/dev/input/event5/", O_RDONLY); //장치 열기
	if(fd<0) printf("open error\n");
	msgID = msgget(1122, IPC_CREAT|0666); //message 초기화
	printf("msgget return %d\n",msgID);
	err = pthread_create(&buttonTh_id, NULL, &buttonThFunc, NULL); //thread 생성
	if(err!=0) printf("thread careat error\r\n");
	
	return msgID;
}
int buttonExit(void)
{
	pthread_cancel(buttonTh_id);
	close(fd);
	printf("test finish!\r\n");
}

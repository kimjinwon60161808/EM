#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include "button.h"

#define INPUT_DEVICE_LIST "/dev/input/event" //실제 디바이스 드라이버 노드파일 : 뒤에 숫자가 붙음
#define PROBE_FILE "/proc/bus/input/devices" //PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알 수 있다"는 그파일
#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=kbd event"
static int fd = 0;
pthread_t buttonTh_id;
static int msgID;

int probeButtonPath(char *newPath)
{
   int returnValue = 0;
   int number =0;
   FILE *fp = fopen(PROBE_FILE,"rt");

   while(!feof(fp))
   {
      char tmpStr[200];
      fgets(tmpStr,200 ,fp);
      if(strcmp(tmpStr, HAVE_TO_FIND_1) ==0)
      {
         printf("yes i found!: %s\r\n",tmpStr);
         returnValue =1; 
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
   printf("%s\n",newPath);
   return returnValue;
}


void *buttonThFunc(char *arg)
{
   int error=0;;
   BUTTON_MSG_T send;
   struct input_event stEvent;
   printf("start!\n");
      //while(1)
      //{
      read(fd, &stEvent, sizeof(stEvent));
      if(stEvent.type == EV_KEY && stEvent.value == 0)
      {
         send.messageNum = 1;
         send.pressed = stEvent.value;//buttonsend.pressed가 해당 버튼이 눌렸다라는 것을 알려줌 1나옴.
         send.keyInput = stEvent.code;//buttonkeyInput이 해당 버튼의 값 출력해줬다.
            switch(send.keyInput)
			{
            case KEY_VOLUMEUP: printf("Volume up key : "); break;
            case KEY_HOME: printf("home key : "); break;
            case KEY_SEARCH: printf("search key : "); break;
            case KEY_BACK: printf("back key : "); break;
            case KEY_MENU: printf("menu key : "); break;
            case KEY_VOLUMEDOWN: printf("Volume down key : "); break;
			}
         
         error=msgsnd(msgID, &send, sizeof(int), 0);
         if(error == -1){
         printf("send error\n");
         }
       //     printf("%d \r\n",send.keyInput);
         //   break;
      }
   //}
}

int buttonInit(void){
   int err;
   int buttonTh_id;
   char inputDevPath[200]={0,};
   if (probeButtonPath(inputDevPath) == 0)
   {
      printf("probe error!\n");
   }
   fd = open(inputDevPath,O_RDONLY);
 
   msgID = msgget ((key_t)MESSAGE_ID, IPC_CREAT|0666);
   err = pthread_create(&buttonTh_id, NULL,  buttonThFunc,  NULL);
   return 1;
}

int buttonExit(void)
{
   pthread_cancel(buttonTh_id);
   close(fd);
   printf("finish!\n");   
}

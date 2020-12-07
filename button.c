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

      
int probebuttonPath(char *newPath)
{   
   int returnValue = 0;
   int number = 0;
   #define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
   #define HAVE_TO_FIND_2 "H: Handlers=kbd event"
   
   FILE *fp = fopen(PROBE_FILE, "rt");
   while(!feof(fp)) //끝까지 읽어들인다.
  {
    char tmpStr[200]; //200자를 읽을 수 있게 버퍼
    fgets(tmpStr,200,fp); //최대 200자를 읽어봄
    //printf ("%s",tmpStr);

    if (strcmp(tmpStr,HAVE_TO_FIND_1) == 0)
    {
      printf("YES! I found!: %s\r\n", tmpStr);
      returnValue = 1; //찾음
    }

   if (
      (returnValue == 1) && //찾은 상태에서
      (strncasecmp(tmpStr, HAVE_TO_FIND_2, strlen(HAVE_TO_FIND_2)) == 0) //Event??을 찾았으면
    )
    {
      printf ("-->%s",tmpStr);
      printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
      number = tmpStr[strlen(tmpStr)-3] - '0'; 
      //Ascii character '0'-'9' (0x30-0x39) 
      //to interger(0)
      break;
    }
  }
  fclose(fp);
  if (returnValue == 1)
  sprintf (newPath,"%s%d",INPUT_DEVICE_LIST,number);
  return returnValue;
}

static pthread_t buttonTh_id;
static int fd = 0;
static int msgID;

void *buttonThFunc(void *args)
{
   struct input_event stEvent;
   BUTTON_MSG_T sendMsg;
   sendMsg.messageNum = 1;
   printf ("button Thread Ready\r\n");
   while (1)
   {
      read(fd, &stEvent, sizeof (stEvent));
      if ((stEvent.value == 0) && (stEvent.type == EV_KEY) )
      {
         //printf ("Key Unpressed: %d\r\n",stEvent.code);
         BUTTON_MSG_T newMsg;
         newMsg.messageNum = 1;
         newMsg.keyInput = stEvent.code;
         msgsnd(msgID, &newMsg, sizeof(BUTTON_MSG_T) - sizeof(long int), 0);
         //printf("\t send press : %d \r\n",newMsg.keyInput);
      }
   }
}


int buttonInit(void)
{
   char buttonPath[1024];
   if (probebuttonPath(buttonPath) == 0)
   {
      printf ("Device button Open Failed\r\n");
      return 0;
   }
   fd=open (buttonPath, O_RDONLY);
   msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
   BUTTON_MSG_T trashCan;
   while (   msgrcv (msgID, &trashCan, sizeof(BUTTON_MSG_T) - sizeof(long int), 0, IPC_NOWAIT) >= 0   )
   {
      printf ("Cleaning Messages...\r\n");
   }
   pthread_create(&buttonTh_id, NULL, buttonThFunc, NULL);
   return 1;
}

int buttonExit(void)
{
   pthread_cancel(buttonTh_id);
}

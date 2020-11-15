#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>//for open/close
#include <fcntl.h> //for O_RDWR
#include <sys/ioctl.h>//for ioctl
//first read input device
#include <sys/msg.h>
#include "button.h"
static int msgID = 0;
int main(int argc, char *argv[]){
   BUTTON_MSG_T rcv;
   rcv.messageNum = 1;
   msgID = msgget (1122, IPC_CREAT|0666);
   if(msgID == -1){
   printf("ID get error\n");
   }
   else{
   printf("get ID!!\n");
   }
   int returnValue = 0;
   buttonInit();  
   while(1){
      returnValue =  msgrcv(msgID, &rcv,sizeof(int),0,IPC_NOWAIT);
       //printf("%d",rcv.keyInput);
       if(returnValue >=0)
       {
         buttonInit();  
      }
   }
      printf("\n");
      buttonExit();
      return 0;
}
   

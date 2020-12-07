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
int main(int argc, char *argv[]){
   buttonInit();

   int msgID = msgget( MESSAGE_ID, IPC_CREAT|0666);
   BUTTON_MSG_T recvMsg;
   while (1)
   {
      msgrcv(msgID, &recvMsg, sizeof (recvMsg)-sizeof (long int), 0, 0);
      switch (recvMsg.keyInput)
      {
            case KEY_VOLUMEUP: printf("Volume up key : %d\n",recvMsg.keyInput); break;
            case KEY_HOME: printf("home key : %d\n",recvMsg.keyInput); break;
            case KEY_SEARCH: printf("search key : %d\n",recvMsg.keyInput); break;
            case KEY_BACK: printf("back key : %d\n",recvMsg.keyInput); break;
              case KEY_MENU: printf("menu key : %d\n",recvMsg.keyInput); break;
            case KEY_VOLUMEDOWN: printf("Volume down key : %d\n",recvMsg.keyInput); break;
      }
   }
}
   

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>
#include <linux/input.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include "textlcddrv.h"


int main(int argc , char **argv){
   textinit();
   textwrite("1", "Hi jungmin!");
   textwrite("2", "embeddedstystem");
   textexit();
}

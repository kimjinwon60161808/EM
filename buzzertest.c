#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"
#define MAX_SCALE_STEP 8
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/”
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"
char gBuzzerBaseSysDir[128]; ///sys/bus/platform/devices/peribuzzer.XX 가 결정됨
const int musicScale[MAX_SCALE_STEP]=
{
	262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};

main(int argc , char **argv)
{
	int freIndex;
	if (argc < 2 || findBuzzerSysPath() )
	{
	printf("Error!\n");
	doHelp();
	return 1;
	}
	freIndex = atoi(argv[1]);
	printf("freIndex :%d \n",freIndex);
	if ( freIndex > MAX_SCALE_STEP )
	{
	   printf(" <buzzerNo> over range \n");
	   doHelp();
	   return 1;
	  }
	  if ( freIndex == 0)// disable
	  {
      buzzerEnable(0);
   }
   else
   {
      setFrequency(musicScale[freIndex-1]);
      buzzerEnable(1);
   }
   return 0;
}

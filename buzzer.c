#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_SCALE_STEP		12*3	//three Octaves.
#define BUZZER_BASE_SYS_PATH	"/sys/bus/platform/devices/"
#define BUZZER_FILENAME  "peribuzzer"
#define BUZZER_ENABLE_NAME		"enable"
#define BUZZER_FREQUENCY_NAME	"frequency"

const char gBuzzerBaseSysDir[1024];  ///sys/bus/platform/devices/peribuzzer.XX 가 결정됨

const int musicScale[MAX_SCALE_STEP] =
{
	
	262/2, 277/2, 294/2, 311/2, 330/2, 349/2, 370/2, 392/2, 415/2, 440/2, 466/2, 494/2,
	262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
	262*2, 277*2, 294*2, 311*2, 330*2, 349*2, 370*2, 392*2, 415*2, 440*2, 466*2, 494*2, 	
};
static int fdEn = 0;
static int fdFq = 0;
int ifBuzz = 0;

int findBuzzerSysPath()
{
	DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
	int ifNotFound = 1;
	if (dir_info != NULL)
	{
		while (1)
		{	
			struct dirent  *dir_entry;
			dir_entry = readdir (dir_info);
			if (dir_entry == NULL)	break;
			if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0){
				ifNotFound = 0;
				sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name);
			}
		}
	}
	printf("find %s\n",gBuzzerBaseSysDir);
	return ifNotFound; 
}


int buzzerInit(void)
{
	if ( findBuzzerSysPath() )
	{
		printf ("Buzzer driver failed!\r\n");
		exit(-1);
	}
	char path[200];
	sprintf (path, "%s%s",gBuzzerBaseSysDir, BUZZER_ENABLE_NAME);
	fdEn = open(path,O_WRONLY);
	if (fdEn <= 0)
	{
		printf ("Buzzer Drv Open Failed\r\n");
		exit(-1);
	}
	write(fdEn,&"0",1);
	ifBuzz = 0;
	
	sprintf (path, "%s%s",gBuzzerBaseSysDir, BUZZER_FREQUENCY_NAME);
	fdFq = open(path,O_WRONLY);
	if (fdFq <= 0)
	{
		printf ("Buzzer Fq. Open Failed\r\n");
		exit(-1);
	}
}
int buzzerPlaySong(int scale)	//0~35.0~11: lowOct, 12~23: MidOct, 24~35: HighOct.
{
	//printf ("Now Play scale %d\r\n", musicScale[scale]);
	dprintf(fdFq, "%d", musicScale[scale]);
	if ( ifBuzz == 0 )
	{
		write(fdEn,&"1",1);
		ifBuzz = 1;
	}	
}
int buzzerStopSong(void)
{
	write(fdEn, &"0",1);
	ifBuzz = 0;
}
int buzzerExit(void)
{
	buzzerStopSong();
	close(fdEn);
	close(fdFq);
}

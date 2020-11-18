#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include  <unistd.h>
#include <dirent.h>

#define MAX_SCALE_STEP
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

char gBuzzerBaseSysDir[128];

const int musicScale[MAX_SCALE_STEP]=
{
	262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};

int findBuzzerSysPath()
{
	DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
	int ifNotFound =1;
	if (dir_info != NULL)
	{
		while(1)
		{
			struct dirent *dir_entry;
			dir_entry = readdir (dir_info);
			if (dir_entry ==NULL)break;
			if(strncasecmp(BUZZER_FILENAME, dir_entry ->d_name, strlen(BUZZER_FILENAME)) ==0)
			{
				ifNotFound =0;
				sprintf(gBuzzerBaseSysDir, "%s%s/",BUZZER_BASE_SYS_PATH, dir_entry ->d_name);
			}
		}
	}
	printf("fined %s\n",gBuzzerBaseSysDir);
	return ifNotFound;
}

void doHelp(void)
{
	printf("usage:\n");
	printf("buzzertest <buzzerNo> \n");
	printf("buzzerNo:\n");
	printf("do(1),re(2),mi(3),fa(4),sol(5),ra(6),si(7),do(8)");
	printf("off(0)\n");
}
void buzzerEnable(int bEnable)
{
	char path[200];
	sprintf(path, "%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
	int fd =open(path, O_WRONLY);
	if (bEnable)	write(fd, &"1", 1);
	else
		write(fd,&"0",1);
		close(fd);
}

void setFrequency(int frequency)
{
	char path[200];
	sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
	int fd= open(path,O_WRONLY);
	dprintf(fd, "%d", frequency);
	close(fd);
}


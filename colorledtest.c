#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "colorled.h"


int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf ("colorledtest.elf 0-100 0-100 0-100\r\n");
		printf("ex) colorledtest.elf 100 100 100 ----> full white color\r\n");
		return 0;
	}
	pwmLedInit();
	pwmSetPercent(atoi(argv[1]),0);
	pwmSetPercent(atoi(argv[2]),1);
	pwmSetPercent(atoi(argv[3]),2);
	pwmInactiveAll();
	return 0;
}

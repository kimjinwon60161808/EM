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
#include <errno.h>
#include "fnddrv.h"


int main(int argc, char **argv)
{
	int number;
	number = atoi("999999");
	
	fndDisp(number,0);
	
	sleep(1);
	number = atoi("888888");
	
	fndDisp(number,0);
	sleep(1);
	number = atoi("777777");
	
	fndDisp(number,0);
	sleep(1);
	number = atoi("666666");
	
	fndDisp(number,0);
	sleep(1);
	number = atoi("555555");
	
	fndDisp(number,0);
	sleep(1);
	number = atoi("444444");
	
	fndDisp(number,0);
	sleep(1);
	number = atoi("333333");
	
	fndDisp(number,0);
	
	sleep(1);
	number = atoi("222222");
	
	fndDisp(number,0);
	
	sleep(1);
	number = atoi("111111");
	
	fndDisp(number,0);
	sleep(1);
	number = atoi("000000");
	
	fndDisp(number,0);
	
}

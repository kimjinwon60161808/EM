#ifndef _BUTTON_H_
#define _BUTTON_H_

#define MESSAGE_ID 1122
#define INPUT_PATH "/dev/input/event5/"
typedef struct
{
	long int messageNum;
	int keyInput;
	int pressed;
} BUTTON_MSG_T;

int buttonInit(void); //thread를 creat
int buttonExit(void); //thread를 exit

#endif

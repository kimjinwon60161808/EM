#ifndef _BUTTON_H_
#define _BUTTON_H_
#define MESSAGE_ID 1122
typedef struct
{
   long int messageNum;
   int keyInput;
   int pressed;
} BUTTON_MSG_T;

int probebuttonPath(char *newPath);
void *buttonThFunc(void* args);
int buttonInit(void);
int buttonExit(void);

#endif

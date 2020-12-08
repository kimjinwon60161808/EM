#ifndef _COLORLED_H__
#define _COLORLED_H_

int pwmActiveAll(void);
int pwmInactiveAll(void);
int pwmSetDuty(int dutyCycle, int pwmIndex);
int pwmSetPeriod(int Period, int pwmIndex);
int pwmSetPercent(int percent, int ledColor);
int pwmStartAll(void);
int pwmLedInit(void);
int colorled_bon(void); 
int colorled_gon(void);
int colorled_ron(void);
int colorled_off(void);

#endif


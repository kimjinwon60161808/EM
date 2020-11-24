CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a

libMyPeri.a: button.o led.o buzzer.o fnd.o
   $(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o

   
fnd.o: fnddrv.c fnddrv.h
   $(CC) fnddrv.c -o fnd.o -c

new_fndtest: fnddrvtest.c fnddrv.h libMyPeri.a
   $(CC) fnddrvtest.c -l MyPeri -L. -o new_fndtest 
   scp new_fndtest ecube@192.168.0.117:/home/ecube

clean:
   rm *.o
   rm *.a
   rm new_fndtest

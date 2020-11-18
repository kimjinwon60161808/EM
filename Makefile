CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a ledtest buttontest

libMyPeri.a : button.o led.o
	$(AR) rc libMyPeri.a led.o button.o
   
button.o  : button.h button.c
	$(CC) button.c -o button.o -c
   
led.o : led.h led.c
	$(CC) led.c -o led.o -c
	
ledtest: ledtest.c led.h libMyPeri.a
	$(CC) ledtest.c -o ledtest -l MyPeri -L.
   
buttontest : buttontest.c button.h libMyPeri.a
	$(CC) buttontest.c -o buttontest -l MyPeri -L. -lpthread
			scp butttontest ecube@192.168.0.117:/home/ecube
   
clean :
	rm *.o -rf
	rm *.a -rf
	rm buttontest -rf

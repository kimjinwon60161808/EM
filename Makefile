CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a

libMyPeri.a: button.o led.o buzzer.o fnd.o textlcd.o temp.o touch.o colorled.o acmgma.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o textlcd.o temp.o touch.o colorled.o acmgma.o

	
button.o: button.c button.h
	$(CC) button.c -o button.o -c

nbuttontest: buttontest.c button.h libMyPeri.a
	$(CC) buttontest.c -l MyPeri -L. -o nbuttontest -lpthread 
	scp nbuttontest ecube@192.168.0.117:/home/ecube

clean:
	rm *.a
	rm nbuttontest

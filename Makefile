CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a

libMyPeri.a: button.o led.o buzzer.o fnd.o textlcd.o temp.o colorled.o acmgma.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o textlcd.o temp.o colorled.o acmgma.o

	
acmgma.o: acmgma.c acmgma.h
	$(CC) acmgma.c -o acmgma.o -c

acmgmatest: acmgmatest.c acmgma.h libMyPeri.a
	$(CC) acmgmatest.c -l MyPeri -L. -o acmgmatest 
	scp acmgmatest ecube@192.168.0.117:/home/ecube

clean:
	rm *.a
	rm acmgmatest

CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a project
	
project: project.c libMyPeri.a
	$(CC) project.c -o project -l MyPeri -L. -lpthread
	scp  project ecube@192.168.0.117:/home/ecube

libMyPeri.a: button.o led.o buzzer.o fnd.o textlcd.o temp.o touch.o colorled.o acmgma.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o textlcd.o temp.o touch.o colorled.o acmgma.o
	
fnd.o: fnddrv.c fnddrv.h
	$(CC) fnddrv.c -o fnd.o -c
	
textlcd.o: textlcddrv.c textlcddrv.h
	$(CC) textlcddrv.c -o textlcd.o -c	
	
	
clean:
	rm *.o -rf
	rm *.a -rf
	rm jukebox -rf

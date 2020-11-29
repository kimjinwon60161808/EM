CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a

libMyPeri.a: button.o led.o buzzer.o fnd.o textlcd.o temp.o colorled.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o textlcd.o temp.o colorled.o

	
colorled.o: colorled.c colorled.h
	$(CC) colorled.c -o colorled.o -c

new_colortest: colorledtest.c colorled.h libMyPeri.a
	$(CC) colorledtest.c -l MyPeri -L. -o new_colortest 
	scp new_colortest ecube@192.168.0.117:/home/ecube

clean:
	rm *.a
	rm new_colortest

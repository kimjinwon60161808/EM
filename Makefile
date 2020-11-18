CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a ledtest buttontest buzzertest

libMyPeri.a: button.o led.o buzzer.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o 

button.o: button.h button.c
	$(CC) button.c -o button.o -c

led.o: led.h led.c
	$(CC) button.c -o button.o -c

buzzer.o: buzzer.h buzzer.c
	$(CC) buzzer.c -o buzzer.o -c


buzzertest: buzzertest.c buzzer.h libMyPeri.a
	$(CC) buzzertest.c -o buzzertest -l MyPeri -L.
	scp buzzertest ecube@192.168.0.117:/home/ecube

clean:
	rm *,o
	rm *.a
	rm buttuntest

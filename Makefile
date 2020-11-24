CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a

libMyPeri.a: button.o led.o buzzer.o fnd.o textlcd.o
   $(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o textlcd.o

   
textlcd.o: textlcddrv.c textlcddrv.h
   $(CC) textlcddrv.c -o textlcd.o -c

new_lcdtest: textlcdtest.c textlcddrv.h libMyPeri.a
   $(CC) textlcdtest.c -l MyPeri -L. -o new_lcdtest 
   scp new_lcdtest ecube@192.168.0.117:/home/ecube

clean:
   rm *.a
   rm new_textlcd

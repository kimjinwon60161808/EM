CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a

libMyPeri.a: button.o led.o buzzer.o fnd.o textlcd.o temp.o
   $(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o textlcd.o temp.o

   
temp.o: temp.c temp.h
   $(CC) temp.c -o temp.o -c

new_temptest: temptest.c temp.h libMyPeri.a
   $(CC) temptest.c -l MyPeri -L. -o new_temptest 
   scp new_temptest ecube@192.168.0.117:/home/ecube

clean:
   rm *.a
   rm new_temptest

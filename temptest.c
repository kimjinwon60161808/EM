#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "temp.h"

char gbuf[10];

int main(int argc, char **argv)
{
   char *buffer; int file;
   file= spi_init("/dev/spidev1.0"); 
   buffer = (char *)spi_read_lm74(file);
   close(file);
   
   int value = 0;
   value = ( buffer[1] >>3);
   value += (buffer[0] ) << 5 ;      
   if ( buffer[0] &0x80)
   {    
      int i =0;
      for (i=   31; i>12; i--)
         value |= (1<<i);
   }
   double temp = (double)value*0.0625;
   printf("currunt temp %lf \n", temp);
   return 0;
}

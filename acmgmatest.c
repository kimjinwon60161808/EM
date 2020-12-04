#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/"
#define MAGNEPATH "/sys/class/misc/FreescaleMagnetometer/"
#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"
#include "acmgma.h"


int main (void)
{
   Accel();
   Magnet();
   Gyro();
   return 0;
}

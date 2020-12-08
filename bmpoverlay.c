#include <stdio.h>
#include <stdlib.h>     // for exit
#include "bmpoverlay.h"
#include "libfbdev.h"
#include "libbmp.h"

//Newly added.

void usage(void)
{
    printf("====================================================\n");
    printf("\nUsage: ./bitmap [FILE.bmp] [char.bmp] \n");
    printf("====================================================\n");
}


int main (int argc, char **argv)
{
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
	char *data;


    printf("=================================\n");
    printf("Frame buffer Application - Bitmap\n");
    printf("=================================\n\n");

    if(argc != 2) {
        usage();
        return  0;
    }

	//FrameBuffer init
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
	{
		printf ("FrameBuffer Init Failed\r\n");
		return 0;
	}

	//Clear FB.
	fb_clear();

	//overlay framebuffer init
	fb_init2();
	fb_clear2(0,0,1024,600);

	//FileRead
    if (read_bmp(argv[1], &data, &cols, &rows) < 0)
	{
		printf ("File open failed\r\n");
		return 0;
	}
//    close_bmp();
    printf("\tBitmapFile: %dx%d pixels\n", cols, rows);
    printf("\tFB Screen: %dx%d\n", screen_width, screen_height);
    printf("\tFB bits_per_pixel: %d, FB line_length: %d\n", bits_per_pixel, line_length);
	
	//FileWrite
	fb_write(data, cols,rows);
	printf ("DO this seg.Fault?\r\n");
 	close_bmp();

	printf ("Try to read New BMP\r\n");

	char *newData;
	int newCols;
	int newRows;


	read_bmp("promusic.bmp", &newData, &newCols, &newRows);
	printf ("%d %d bmp read\r\n",newCols, newRows);

	while(1)
{
	int xpos = 0;
	for (xpos = 100; xpos<200;xpos++)
	{
		fb_write2(newData, newCols, newRows, xpos, 200);
		usleep(500);
		fb_clear2(xpos,200, 1, newRows);	//딱 한라인만 지우쟈...
		//fb_clear2(xpos+newCols-1, 200, 1, newRows);
	}
	int ypos = 0;
	for (ypos = 200; ypos>0; ypos--)
	{
		//printf ("YPOs:%d\r\n",ypos);
		fb_write2(newData, newCols, newRows, xpos, ypos);
		usleep(500);
		fb_clear2( xpos, ypos+newRows, newCols, 1);
	}
	sleep(1);
	for (xpos = 200; xpos>100;xpos--)
        {
                fb_write2(newData, newCols, newRows, xpos, 0);
                usleep(100);
                fb_clear2(xpos+newCols-1, ypos, 1, newRows);        //딱 한라인만 지우쟈...
                //fb_clear2(xpos+newCols-1, 200, 1, newRows);
        }

	for (ypos = 0; ypos<200; ypos++)
        {
                //printf ("YPOs:%d\r\n",ypos);
                fb_write2(newData, newCols, newRows, xpos, ypos);
                usleep(500);
                fb_clear2( xpos, ypos, newCols, 1);
                usleep(1000);   
        }     
}
	close_bmp();

	
	fb_close();
	fb_close2();
    return 0;
}

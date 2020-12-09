#include <stdio.h>
#include "led.h"
#include "textlcddrv.h"
#include "button.h"
#include "buzzer.h"
#include "touch.h"
#include "fnddrv.h"
#include "colorled.h"
#include "acmgma.h"
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <pthread.h>
#include <sys/msg.h>
#include <string.h>

//C도,D레,E미,F파,G솔,A라,B시: 계이름
//C+, C-: 옥타브
//C+#, C-@: 임시 조표
//C$, : 내추랄. 퍼마조표를 뗀다.
//C+#1, C+#2, C+#4, C+#8, C+#6: 16분음표
//C+#1., C+#2.: 점음표
//P: 쉼표
//P1: 1분쉼표
//P1.: 점 1분쉼표
//[#]: 상시조표
//[#F]: 라 장조
//[@BE]: 내림마장조

const char song_note [] = 
"[#F]" //바장조 파(F)에 샾
"D4 G4 G8 F#8 E4 G4 D4 B-4 D4 G4 A8 B8 C+4 C+8 B8 A2 A4 P"
"D+4 D+8 C+8 B4 A4 G4 F#8 E8 D4 B-4 D4 G4 A8 A8 B4 G2 G4 P"
"F#4 F#8 G8 A4 F#4 B4 B8 C+8 D+4 B4 A4 G4 F#4 G4 A2 A4 P"
"D+4 D+8 C+8 B4 A4 G4 F#8 E8 D4 B-4 D4 G4 A8 A8 B4 G4 G8"
;


enum
{
	PARSE_TYPE_INITIAL,
	PARSE_TYPE_NOTE,		//음표/쉼표
	PARSE_TYPE_SPEED,	//속도
	PARSE_TYPE_MAJOR,	//장단조/
};
enum
{
	PARSE_STAGE_UNKNOWN,
	PARSE_STAGE_NOTE,		//계이름
	PARSE_STAGE_OCT,
	PARSE_STAGE_TEMP,		//임시조표
	PARSE_STAGE_SUSTAIN,	//지속시간
	PARSE_STAGE_POINT,		//점
	
	PARSE_STAGE_MAJOR_MINOR,
	PARSE_STAGE_MAJOR_NOTE,
	PARSE_STAGE_MAJOR_CLOSE,
};

#define DEFAULT_SUS		8			//32분음표가 기본.

static pthread_t button_id2;
static pthread_t button_id3;
static pthread_t fnd_id;
static pthread_t tlcd_id;
static pthread_t gyro_id;

int main (int argc, char *argv[])
{
	pid_t pid;
	ledLibInit(); 
	buttonInit();
	buzzerInit();
	touchInit();
	pwmLedInit();
	textinit();
	
	int songPtr = 0;
	int oneTickPerMs = DEFAULT_SUS;	//1분음표=1.024초, 2분음표=512ms, 4분음표 = 256ms, 8분음표 = 128ms, 16분음표 = 16ms; 
	int msCnt = 0;
	int waitToNextTick = DEFAULT_SUS;
	int prevWaitToNextTick = waitToNextTick;
	int parseType = PARSE_TYPE_INITIAL;
	int parseStage = PARSE_STAGE_UNKNOWN;;
	int nearByScale = 0;	//영구조표때문.
	
	int permaMajor[8] = {0,0,0,0,0,0,0,0,};	//영구조표 기록
	int majorMinor = 0;
	
	int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
	BUTTON_MSG_T recvdMsg;

	int msgID2 = msgget( MESSAGE_ID2, IPC_CREAT|0666);
	TOUCH_MSG_T recvMsg;
	ledLibRaw(0xFF);
	int ifPlay = 0;
	int cnt=0;
	int pcnt=0;
	int status =0;
	int number;
	pthread_create(&button_id2, NULL, buttonInit, NULL);
	pid = fork();
	if(pid ==0)
	{
		while(1)
	{
		if(msgrcv(msgID2, &recvMsg, sizeof (recvMsg)-sizeof (long int), 0, 0)>0)
		{
		//이떄는 터치가 일어나거나 아니면 터리가 끝날때만 여기에 들어옴!
		switch (recvMsg.keyInput)
		{
			case 999:
				if (recvMsg.pressed == 1)
				{
					if (recvMsg.x >600 && recvMsg.x< 900 &&  recvMsg.y > 0 && recvMsg.y < 250)
					{
						printf ("You are smart!!\r\n");
						colorled_bon();
						break;
					}
					
					else
					{
						printf ("You touched fail!!\r\n");
						colorled_ron();
						ledLibOnOff(cnt,0);
						cnt++;
									break;
					}
				}
		}
	}
	if(cnt ==7)
	{
		printf("game over!\r\n");
		textwrite("1", "");
		textwrite("2", "");
		colorled_off();
		int number = atoi("000000");
		fndDisp(number,0);
		break;
	}
	int i = Gyro();
	if(i>150)
	{
		printf("Danger!!!\r\n");
		textwrite("1", "");
		textwrite("2", "");
		colorled_off();
		int number = atoi("000000");
		fndDisp(number,0);
		break;
	}

}	//End of while.
	ledLibExit();
	buttonExit();
	buzzerExit();
	

}
else if(pid >0)
{
	
	ledLibInit(); 
	buttonInit();
	buzzerInit();
	touchInit();
   int msgID1 = msgget( MESSAGE_ID, IPC_CREAT|0666);
   BUTTON_MSG_T recvdMsg;
   int ifPlay =0;
   while ( !waitpid(pid, &status, WNOHANG) )	
   {
	   			
	if ( msgrcv (msgID, &recvdMsg, sizeof(BUTTON_MSG_T) - sizeof(long int), 0, IPC_NOWAIT) >= 0	)
		{
			//Button!
			if (recvdMsg.keyInput == 158){
				ifPlay =0;
				textwrite("1", "");
				textwrite("2", "");
				colorled_off();
				int number = atoi("000000");
				fndDisp(number,0);
				buzzerExit();
				break;	//프로그램 종료.
			}
			else if (recvdMsg.keyInput == 102){ //제일 왼쪽거 Play
				ifPlay = 1;
				pthread_create(&fnd_id, NULL, fndcount , NULL);
				pthread_create(&tlcd_id, NULL, tlcdwrite , NULL);
			}
			else if (recvdMsg.keyInput == 114) 
				ifPlay = 0;
			else if (recvdMsg.keyInput == 217) 
			{
				ifPlay 	= 0;
				songPtr	= 0;
			}
		}
		usleep(1000);	//Every 1ms.
		msCnt++;
		if (msCnt >= oneTickPerMs)
		{	//매 틱마다.
			msCnt = 0;
			//oneTick Reached.
			//16분음표 하나 추가.
			if (waitToNextTick == 0)
			{
				if (songPtr == strlen(song_note) )
				{
					buzzerStopSong(); //끝.
				}
				
				int newNote = 0;
				if (ifPlay == 0)	//현재 플레이상태가 아니면.
				{
					
					ledLibRaw(0);
					buzzerStopSong();
					waitToNextTick = DEFAULT_SUS;	//그냥 기달리기.
				}
				else 
				{
					while ( songPtr <= strlen(song_note) ) //while of Parse.
					{
						if (parseType == PARSE_TYPE_INITIAL)	
						{
							//if ( (song_note[songPtr] == 0) || (song_note[songPtr] == ' ')	songPtr++;	//이건 공백은 해석하지 않기.
	
							switch (song_note[songPtr])
							{
								case 'A': newNote = 21;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'A'; songPtr++; break;
								case 'B': newNote = 23;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT; nearByScale = 'B'; songPtr++; break;
								case 'C': newNote = 12;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'C'; songPtr++; break;
								case 'D': newNote = 14;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'D'; songPtr++; break;
								case 'E': newNote = 16;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'E'; songPtr++; break;
								case 'F': newNote = 17;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'F'; songPtr++; break;
								case 'G': newNote = 19;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'G'; songPtr++; break;
								case 'P': newNote = -1;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_SUSTAIN;	nearByScale = '-'; songPtr++; break;
								case '[': parseType = PARSE_TYPE_MAJOR; 	parseStage = PARSE_STAGE_MAJOR_MINOR; songPtr++; memset(permaMajor, 0, sizeof(permaMajor));		break;
								default: songPtr++;	break;	//공백등은 개무시.
								//case ' ':
								//default:
							}
						}
						
						else if (parseType == PARSE_TYPE_NOTE)
						{
							//waitToNextTick = DEFAULT_SUS;
							waitToNextTick = prevWaitToNextTick;
							if (parseStage == PARSE_STAGE_OCT)
							{
								if (song_note[songPtr] == '+')		//높은 옥타브...
								{
									//printf ("HighOct!\r\n");
									newNote+=12;
									songPtr++;
								}
								else if (song_note[songPtr] == '-')	//낮은 옥타브...
								{
									newNote-=12;
									songPtr++;
								}
								parseStage = PARSE_STAGE_TEMP;	//Try Again.
							}
							else if (parseStage == PARSE_STAGE_TEMP)
							{
								if (newNote != -1)
								{	//쉼표는 적용하면 안됨.
									newNote = newNote + permaMajor[nearByScale-'A'] ;	//고정조표 반영
								
									if (song_note[songPtr] == '#') 
									{
										songPtr++;
										if (permaMajor[nearByScale-'A']!=1)			//PermaMajor == 1:Sharp이 붙은것.
											newNote+=1;
									}
									else if (song_note[songPtr] == '@') 
									{
										songPtr++;
										if (permaMajor[nearByScale-'A']!=-1)	//PermaMajor == -1:Flat이 붙은것.
											newNote-=1;
									}
									else if (song_note[songPtr] == '$')
									{
										
										songPtr++;
										newNote = newNote - permaMajor[nearByScale-'A'];
									}
								}
								
								parseStage = PARSE_STAGE_SUSTAIN;
							}
							else if (parseStage == PARSE_STAGE_SUSTAIN)
							{
							
								
								if (song_note[songPtr] == '1')
								{
									waitToNextTick = DEFAULT_SUS*32;	//32가 minimum Resolution.
									songPtr++;
								}
								else if (song_note[songPtr] == '2')
								{
									waitToNextTick = DEFAULT_SUS*16;	//32가 minimum Resolution.
									songPtr++;
								}
								else if (song_note[songPtr] == '4')
								{				
									waitToNextTick = DEFAULT_SUS*8;	//32가 minimum Resolution.
									songPtr++;
								}
								else if (song_note[songPtr] == '8')
								{
									waitToNextTick = DEFAULT_SUS*4;
									songPtr++;
								}
								else if (song_note[songPtr] == '6')
								{
									waitToNextTick = DEFAULT_SUS*2;
									songPtr++;
								}
								prevWaitToNextTick = waitToNextTick;
								parseStage = PARSE_STAGE_POINT;
							}	
							else if ( parseStage == PARSE_STAGE_POINT )
							{
								if (song_note[songPtr] == '.')
								{
									waitToNextTick = waitToNextTick*3/2;	//1.5
									songPtr++;
								}
								if (newNote == -1)
								{	
									buzzerStopSong();
								}
								else
								{
								
									buzzerPlaySong(newNote);
								}
								parseType = PARSE_TYPE_INITIAL;
								break;	
							}
						}	
						
						else if (parseType == PARSE_TYPE_MAJOR)
						{
							//printf("ParseTypeMajor!\r\n");
							if ( parseStage == PARSE_STAGE_MAJOR_MINOR )
							{
								if ( song_note[songPtr] == '#' )
								{
									majorMinor = 1;
									songPtr++;
									parseStage = PARSE_STAGE_MAJOR_NOTE;
								}
								else if (song_note[songPtr] == '@' )
								{
									majorMinor = -1;
									songPtr++;
									parseStage = PARSE_STAGE_MAJOR_NOTE;
								}
								else
								{
									parseStage = PARSE_STAGE_MAJOR_CLOSE;
								}
							}
							else if (parseStage == PARSE_STAGE_MAJOR_NOTE)
							{
								//printf ("%c!\r\n",song_note[songPtr]);
								if ( song_note[songPtr] >= 'A' && song_note[songPtr] <= 'G' )
								{
									permaMajor[song_note[songPtr] - 'A'] = majorMinor;
									songPtr++;
								}
								else
								{
									//printf ("This should be Here!\r\n");
									parseStage = PARSE_STAGE_MAJOR_CLOSE;
								}
							}
							else if (parseStage == PARSE_STAGE_MAJOR_CLOSE)
							{
								if ( song_note[songPtr] != ']' )
									//printf ("Major Minor Open/Close err!\r\n");
								songPtr++;
								//printf ("%d,%d,%d,%d,%d,%d,%d,%d\r\n",permaMajor[0],permaMajor[1],permaMajor[2],permaMajor[3],permaMajor[4],permaMajor[5],permaMajor[6],permaMajor[7]);
								parseType = PARSE_TYPE_INITIAL;
							}
						}
					}
				}	//Play 끝.
				
				if ( songPtr > strlen(song_note) )
				{
					printf ("Song is ended, successfully!\r\n");
					//Song is ended.
					textwrite("1", "");
					textwrite("2", "");
					colorled_off();
					ledLibRaw(0x00);
					int number = atoi("000000");
					fndDisp(number,0);
					ifPlay = 0;
					songPtr = 0;	//초기화.
					break;
				}
			}
			else
			{
				//이번틱은 넘기기.
				waitToNextTick --;
			}
		}
	}
}
}



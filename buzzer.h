#ifndef _BUZZER_H_
#define _BUZZER_H_
	int buzzerInit(void);
	int buzzerPlaySong(int scale);	//0~35.0~11: lowOct, 12~23: MidOct, 24~35: HighOct
	int buzzerStopSong(void);
	int buzzerExit(void);
#endif


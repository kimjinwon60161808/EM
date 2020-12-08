#ifndef __FND_DRV_H__
#define __FND_DRV_H__

#define MAX_FND_NUM 6
#define FND_DATA_BUFF_LEN (MAX_FND_NUM + 2) // 자리수와 관련된 define
#define FND_DRIVER_NAME "/dev/perifnd"
#define MODE_STATIC_DIS 0
#define MODE_TIME_DIS 1
#define MODE_COUNT_DIS 2


int fndDisp(int num, int dotflag);
int fndAllOff();
int fndcount(int argr, char **argl);

typedef struct FNDWriteDataForm_tag
{
   char DataNumeric[FND_DATA_BUFF_LEN]; // 숫자 0~9 자리수
   char DataDot[FND_DATA_BUFF_LEN];// 숫자 0 or 1 각 자리수의 점을 킬지 끌지
   char DataValid[FND_DATA_BUFF_LEN];//  숫자 0 or 1 숫자를 쓰거나 안쓰거나
}stFndWriteForm, *pStFndWriteForm;

#endif //__FND_DRV_H__



#ifndef __FND_DRV_H__
#define __FND_DRV_H__

#define MAX_FND_NUM 6
#define FND_DATA_BUFF_LEN (MAX_FND_NUM +2)

typedef struct FNDWriteData_tag
{
	char DataNumeric[FND_DATA_BUFF_LEN];
	char DataDot[FND_DATA_BUFF_LEN];
	char DataValid[FND_DATA_BUFF_LEN];
	
}stFndWriteForm, *pStFndWriteForm;

#endif// __FND_DRV_H__

#ifndef _time_h
#define _time_h

#include "base.h"

void time_init(void);

#pragma pack(1)
typedef struct
{
	uint8_t	hour;
	uint8_t min;
	uint8_t sec;
	uint16_t us;
	uint8_t flag;
}_TIME;
	
extern _TIME	tim0;
#endif



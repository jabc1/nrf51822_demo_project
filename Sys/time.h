#ifndef _time_h
#define _time_h

#include "base.h"

void time_init(void);
void led_function(void);
void start_led(u16 t);
#pragma pack(1)
typedef struct
{
	uint8_t	hour;
	uint8_t min;
	uint8_t sec;
	uint16_t us;
	uint8_t flag;
}_TIME;
typedef struct
{
	u8		Flag;
	u16		star;
	u16		count;
}_COUNT;

extern _TIME	tim0;
#endif



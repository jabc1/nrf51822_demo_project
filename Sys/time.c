#include "time.h"
#include "nrf.h"
#include "Thread.h"
#include "runflag.h"
#include "nrf_gpio.h"
#include "gpio.h"

volatile unsigned long uwTick;
_TIME	tim0;
_COUNT	LLED;
void time_init(void)
{
	NRF_TIMER0->PRESCALER =4;
	NRF_TIMER0->MODE =0;
	NRF_TIMER0->BITMODE =3;
	NRF_TIMER0->CC[0]=1000;
	NRF_TIMER0->INTENSET =1<<16;
	NRF_TIMER0->SHORTS=1;
	NRF_TIMER0->TASKS_START=1;
	NVIC_SetPriority(TIMER0_IRQn,3);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);
}

void TIMER0_IRQHandler()//1ms²âÊÔ×¼È·
{
	if(NRF_TIMER0->EVENTS_COMPARE[0]==1)
	{
		NRF_TIMER0->EVENTS_COMPARE[0]=0;
		uwTick++;
		Thread_RunCheck();
		led_function();
//		if(tim0.us % 680 == 0)
//			tim0.flag = true;
//		if(tim0.us++ >= 999)
//		{
//			tim0.us = 0;
//			if(tim0.sec++ >= 59)
//			{
//				tim0.sec = 0;
//				if(tim0.min ++ >= 59)
//				{
//					tim0.min = 0;
//				}
//			}
//		}
	}
}
void start_led(u16 t)
{
	nrf_gpio_pin_set(LED);
	LLED.Flag = 1;
	LLED.star = t;
}
void led_function()
{
	if(LLED.Flag)
	{
		if(LLED.count++ >= LLED.star)
		{
			LLED.Flag = 0;
			nrf_gpio_pin_clear(LED);
		}
	}
	else
	{
		LLED.count = 0;
	}
}





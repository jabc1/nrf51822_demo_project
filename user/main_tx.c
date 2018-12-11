#include <stdint.h>
#include "nrf.h"
#include "radio_config.h"
#include "simple_uart.h"
#include "boards.h"
#include "nrf_esb.h"
#include "gpio.h"
#include "time.h"
#include "Sys.h"
#include "uart.h"

uint8_t PacketBuffer[8];
void Nrf51822_Rx(void);
void test(void);

int main(void)
{
	sysinit();	
	usart_init();
	gpio_inti();
	time_init();
	radio_configure();
	NRF_RADIO->PACKETPTR = (uint32_t)PacketBuffer;//指针指向数据缓冲packet
	radio();
	while(1)
	{
		if(tim0.flag)
		{
			tim0.flag = false;
			printf("test\r\n");
		}
	}
}




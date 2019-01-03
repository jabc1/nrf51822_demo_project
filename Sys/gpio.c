/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : gpio.c
Description: set gpio High Low Input
Platform   : MDK V5.26.0.0
Version    : V1.0
Author	   : Jason
Create Time: 2018-12-3
Modify	   : 
Modify Time: 
******************************************************************************/
#include "gpio.h"
#include "nrf_delay.h"

void gpio_inti()
{
	nrf_gpio_cfg_output(LED);
}
void test_gpio()
{
	nrf_gpio_pin_toggle(LED);
}





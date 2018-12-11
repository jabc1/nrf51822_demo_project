#include "gpio.h"
#include "nrf_delay.h"

void gpio_inti()
{
	nrf_gpio_cfg_output(LED);
}
void test_gpio()
{
	nrf_gpio_pin_toggle(LED);
	nrf_delay_ms(200);
}





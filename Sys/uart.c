#include "uart.h"
#include "nrf_gpio.h"

void usart_init(void)
{
	/** @snippet [Configure UART RX and TX pin] */
	nrf_gpio_cfg_output(TX_PIN_NUMBER);             //设置usart-io
	nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);  

	NRF_UART0->PSELTXD =TX_PIN_NUMBER;//0x200ul;//0x01ul<<TX_PIN_NUMBER;  //选择tx管脚
	NRF_UART0->PSELRXD =RX_PIN_NUMBER;//0x800ul;//0x01ul<<RX_PIN_NUMBER;


	NRF_UART0->BAUDRATE         = (UART_BAUDRATE_BAUDRATE_Baud115200 << UART_BAUDRATE_BAUDRATE_Pos);   //波特率
	NRF_UART0->ENABLE           = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);            //打开usart
	NRF_UART0->TASKS_STARTTX    = 1;
	NRF_UART0->TASKS_STARTRX    = 1;
	NRF_UART0->EVENTS_RXDRDY    = 0;
#if uart_exit     //usart中断设置
     NRF_UART0->INTENSET = UART_INTENSET_RXDRDY_Enabled << UART_INTENSET_RXDRDY_Pos;
    NVIC_SetPriority(UART0_IRQn, 1);
    NVIC_EnableIRQ(UART0_IRQn);
#endif
}
void UART0_IRQHandler(void)  //usart中断处理
{
	u8 res;
	while (NRF_UART0->EVENTS_RXDRDY != 1)
	{
		// Wait for RXD data to be received
	}
	NRF_UART0->EVENTS_RXDRDY = 0;
	res = (uint8_t)NRF_UART0->RXD;
	
}

int fputc(int ch, FILE *f)//重定义putc函数，这样可以使用printf函数从串口1打印输出
{
	/* Place your implementation of fputc here */
	usart_send((uint8_t)ch);
	return ch;
}
void usart_send(u8 tx)  //串口发送
{
  NRF_UART0->TXD = tx;

  while (NRF_UART0->EVENTS_TXDRDY!=1)
  {
    // Wait for TXD data to be sent
  }
  NRF_UART0->EVENTS_TXDRDY=0;
}






#ifndef _uart_h
#define _uart_h

#include "base.h"
#include "stdio.h"


#define RX_PIN  11  // UART RX pin number.
#define TX_PIN  9  // UART TX pin number.
#define  uart_exit     1   //1¿ªÖÐ¶Ï
void usart_init(void);
void send_uart_byte(uint8_t data);
#endif


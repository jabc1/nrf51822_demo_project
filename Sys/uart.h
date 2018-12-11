#ifndef _uart_h
#define _uart_h

#include "base.h"
#include "stdio.h"

#define RX_PIN_NUMBER  9  // UART RX pin number.
#define TX_PIN_NUMBER  8  // UART TX pin number.
#define  uart_exit     1   //1¿ªÖÐ¶Ï
void usart_init(void);
void usart_send(u8 tx);

#endif


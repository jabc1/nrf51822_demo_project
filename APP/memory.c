/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : memory.c
Description: get cmd frame from uart fifo
Platform   : MDK V5.26.0.0
Version    : V1.0
Author	   : Jason
Create Time: 2018-12-3
Modify	   : 
Modify Time: 
******************************************************************************/
#include "memory.h"
#include "fifo.h"

#define			Uart0Tx_len				1024*4+1//Uart0 memory 2k
#define			Uart0Rx_len				1024*1+1//Uart0 memory 1k
#define			Radiofifo_len			1024*4+1//radio memory 4k

u8 Uart0Rx_buff[Uart0Rx_len];
u8 Uart0Tx_buff[Uart0Tx_len];
u8 Radiofifo_buff[Radiofifo_len];

FIFO_t	Uart0Tx;
FIFO_t	Uart0Rx;
FIFO_t	Radiofifo;

void Init_fifo()
{
	fifo_Init(&Uart0Tx,Uart0Tx_buff,Uart0Tx_len);
	fifo_Init(&Uart0Rx,Uart0Rx_buff,Uart0Rx_len);
	fifo_Init(&Radiofifo,Radiofifo_buff,Radiofifo_len);
}










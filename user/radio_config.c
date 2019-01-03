/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : radio_config.c
Description: 对RF发送和接收进行配置
Platform   : MDK V5.26.0.0
Version    : V1.0
Author	   : Jason
Create Time: 2018-12-3
Modify	   : 
Modify Time: 
******************************************************************************/
#include "radio_config.h"
#include "nrf_delay.h"
#include "base.h"
#include "fifo.h"
#include "memory.h"
#include "time.h"

static u8 PacketBuffer[7]={0,};

void radio_configure()//无线配置，准备和nrf24L01通讯
{
	NRF_RADIO->TXPOWER = (0xf8<<0);//无线功率4分贝
	NRF_RADIO->FREQUENCY = 10;//无线频率30MHz+2400MHz=2430MHz
	//无线速率：00：1Mbit，01：2Mbit，02：250Kbit，03：1Mbit（蓝牙）
	NRF_RADIO->MODE = (01<<1);//速率1MHz
	//无线地址设置
	NRF_RADIO->PREFIX0 = 0xff00ffA5;  // 通道3 到 0 的低1字节
	NRF_RADIO->BASE0   = 0x55AA0880;  // 通道0的高字节
//	NRF_RADIO->PREFIX1 = 0x23C343A5;  // 通道7 到 4 的低1字节
//	NRF_RADIO->BASE1   = 0x55AA0880;  // 通道1-7的高字节
	NRF_RADIO->TXADDRESS = 0x00UL;      // 发射使用的通道号：1通道
	NRF_RADIO->RXADDRESSES = 0x01UL;    // 接收的通道号：0通道
	//配置包0的设置
	NRF_RADIO->PCNF0 = (0<<16)| //S1领域的长度
					 (0<<8) | //S0场的长度
					 (0<<0);  //长度字段中的比特数

	//配置包1的设置
	NRF_RADIO->PCNF1 = (0<<25)| //效验位（0关，1开）
					  (1<<24)| //数据大小端（高低字节哪个先发 0低字节，1高字节）
					  (2<<16)| //通道1~7高字节长度（ nrf24高低字节5字节：4个高+1个低）
					  (6<<8) | //数据字节长度（255~1）3字节QxC
					  (6<<0);  //硬件传输字节长度（255~1）3字节QxC
	// CRC 校验长度配置
	NRF_RADIO->CRCCNF = 2; // 校验长度 2个char
	if ((NRF_RADIO->CRCCNF & 0x03)== 2 )
	{
		NRF_RADIO->CRCINIT = 0xFFFFUL;      // 校验初始值
		NRF_RADIO->CRCPOLY = 0x11021UL;     // CRC poly: x^16+x^12^x^5+1
	}
	else if ((NRF_RADIO->CRCCNF & 0x03) == 1 )
	{
		NRF_RADIO->CRCINIT = 0xFFUL;        // 校验初始值
		NRF_RADIO->CRCPOLY = 0x107UL;       // CRC poly: x^8+x^2^x^1+1
	}
	nrf_delay_ms(5);
}
void radio()
{
	NRF_RADIO->SHORTS |= RADIO_SHORTS_ADDRESS_RSSISTART_Msk;
	NRF_RADIO->INTENSET = RADIO_INTENSET_READY_Msk | RADIO_INTENSET_END_Msk ;//| RADIO_INTENSET_DISABLED_Msk;
	NVIC_EnableIRQ(RADIO_IRQn);
	NRF_RADIO->PACKETPTR = (uint32_t)PacketBuffer;//指针指向数据缓冲packet
	NRF_RADIO->TASKS_RXEN   = 1U; //接收模式	
}

void RADIO_IRQHandler()
{
	if(NRF_RADIO->EVENTS_READY)
	{
		NRF_RADIO->EVENTS_READY = 0;
		NRF_RADIO->TASKS_START = 1;
	}
	if(NRF_RADIO->EVENTS_END)
	{
		NRF_RADIO->EVENTS_END = 0;       
		if(NRF_RADIO->CRCSTATUS == 1)
		{
			while(NRF_RADIO->EVENTS_RSSIEND == 0)
			{}
			PacketBuffer[6] = NRF_RADIO->RSSISAMPLE;
			NRF_RADIO->EVENTS_RSSIEND = 0;
			fifo_puts(&Radiofifo,&PacketBuffer[0],7);
			start_led(20);
		}
		NRF_RADIO->TASKS_START = 1;
	}
}






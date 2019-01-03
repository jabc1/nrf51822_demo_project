/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : radio_config.c
Description: ��RF���ͺͽ��ս�������
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

void radio_configure()//�������ã�׼����nrf24L01ͨѶ
{
	NRF_RADIO->TXPOWER = (0xf8<<0);//���߹���4�ֱ�
	NRF_RADIO->FREQUENCY = 10;//����Ƶ��30MHz+2400MHz=2430MHz
	//�������ʣ�00��1Mbit��01��2Mbit��02��250Kbit��03��1Mbit��������
	NRF_RADIO->MODE = (01<<1);//����1MHz
	//���ߵ�ַ����
	NRF_RADIO->PREFIX0 = 0xff00ffA5;  // ͨ��3 �� 0 �ĵ�1�ֽ�
	NRF_RADIO->BASE0   = 0x55AA0880;  // ͨ��0�ĸ��ֽ�
//	NRF_RADIO->PREFIX1 = 0x23C343A5;  // ͨ��7 �� 4 �ĵ�1�ֽ�
//	NRF_RADIO->BASE1   = 0x55AA0880;  // ͨ��1-7�ĸ��ֽ�
	NRF_RADIO->TXADDRESS = 0x00UL;      // ����ʹ�õ�ͨ���ţ�1ͨ��
	NRF_RADIO->RXADDRESSES = 0x01UL;    // ���յ�ͨ���ţ�0ͨ��
	//���ð�0������
	NRF_RADIO->PCNF0 = (0<<16)| //S1����ĳ���
					 (0<<8) | //S0���ĳ���
					 (0<<0);  //�����ֶ��еı�����

	//���ð�1������
	NRF_RADIO->PCNF1 = (0<<25)| //Ч��λ��0�أ�1����
					  (1<<24)| //���ݴ�С�ˣ��ߵ��ֽ��ĸ��ȷ� 0���ֽڣ�1���ֽڣ�
					  (2<<16)| //ͨ��1~7���ֽڳ��ȣ� nrf24�ߵ��ֽ�5�ֽڣ�4����+1���ͣ�
					  (6<<8) | //�����ֽڳ��ȣ�255~1��3�ֽ�QxC
					  (6<<0);  //Ӳ�������ֽڳ��ȣ�255~1��3�ֽ�QxC
	// CRC У�鳤������
	NRF_RADIO->CRCCNF = 2; // У�鳤�� 2��char
	if ((NRF_RADIO->CRCCNF & 0x03)== 2 )
	{
		NRF_RADIO->CRCINIT = 0xFFFFUL;      // У���ʼֵ
		NRF_RADIO->CRCPOLY = 0x11021UL;     // CRC poly: x^16+x^12^x^5+1
	}
	else if ((NRF_RADIO->CRCCNF & 0x03) == 1 )
	{
		NRF_RADIO->CRCINIT = 0xFFUL;        // У���ʼֵ
		NRF_RADIO->CRCPOLY = 0x107UL;       // CRC poly: x^8+x^2^x^1+1
	}
	nrf_delay_ms(5);
}
void radio()
{
	NRF_RADIO->SHORTS |= RADIO_SHORTS_ADDRESS_RSSISTART_Msk;
	NRF_RADIO->INTENSET = RADIO_INTENSET_READY_Msk | RADIO_INTENSET_END_Msk ;//| RADIO_INTENSET_DISABLED_Msk;
	NVIC_EnableIRQ(RADIO_IRQn);
	NRF_RADIO->PACKETPTR = (uint32_t)PacketBuffer;//ָ��ָ�����ݻ���packet
	NRF_RADIO->TASKS_RXEN   = 1U; //����ģʽ	
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






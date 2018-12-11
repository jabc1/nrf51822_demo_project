#include "radio_config.h"
#include "nrf_delay.h"
#include "string.h"
#define PACKET0_S1_SIZE                  (0UL)  //!< S1 size in bits
#define PACKET0_S0_SIZE                  (0UL)  //!< S0 size in bits
#define PACKET0_PAYLOAD_SIZE             (0UL)  //!< payload size in bits
#define PACKET1_BASE_ADDRESS_LENGTH      (4UL)  //!< base address length in bytes
#define PACKET1_STATIC_LENGTH            (1UL)  //!< static length in bytes
#define PACKET1_PAYLOAD_SIZE             (1UL)  //!< payload size in bytes

static uint8_t nrf_rx_packet[7]={0};

#define  LEN   512
#define  PLEN  4
static uint8_t rx_fifo[LEN][PLEN];
uint32_t timer_cnt =0;
uint16_t  instart;
uint16_t  inend;
_Bool    inempty;
uint8_t checksum=0;

void radio_configure()//�������ã�׼����nrf24L01ͨѶ
{
	NRF_RADIO->TXPOWER = (0xf8<<0);//���߹���4�ֱ�
	NRF_RADIO->FREQUENCY = 10;//����Ƶ��30MHz+2400MHz=2430MHz
	//�������ʣ�00��1Mbit��01��2Mbit��02��250Kbit��03��1Mbit��������
	NRF_RADIO->MODE = (00<<0);//����1MHz
	//0x01,0x01,0x01,0x01,0x01(24L01���͵�ַ)
	//���ߵ�ַ����
	NRF_RADIO->PREFIX0 = 0x23C343A5;  // ͨ��3 �� 0 �ĵ�1�ֽ�
	NRF_RADIO->PREFIX1 = 0x23C343A5;  // ͨ��7 �� 4 �ĵ�1�ֽ�
	NRF_RADIO->BASE0   = 0x55AA0880;  // ͨ��0�ĸ��ֽ�
	NRF_RADIO->BASE1   = 0x55AA0880;  // ͨ��1-7�ĸ��ֽ�
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

	//���ռĴ�����  NRF_RADIO->PACKETPTR    
}

void radio()
{
	NRF_RADIO->SHORTS|= RADIO_SHORTS_ADDRESS_RSSISTART_Msk;
	NRF_RADIO->INTENSET = RADIO_INTENSET_READY_Msk | RADIO_INTENSET_END_Msk ;//| RADIO_INTENSET_DISABLED_Msk;
	NVIC_EnableIRQ(RADIO_IRQn);
	NRF_RADIO->PACKETPTR = (uint32_t)nrf_rx_packet;//ָ��ָ�����ݻ���packet
	NRF_RADIO->TASKS_RXEN   = 1U; //����ģʽ	
}
unsigned char key[2] = {0x41,0x55};
void data_decrypt(unsigned char *buf,unsigned char *data)
{
	unsigned char *p;
	unsigned char temp[6] = {0,},i;
	p = buf;
	for(i=5;i>=1;i--)
	{
		temp[i] = *(p+i) ^ *(p+(i-1)) ^ key[i&0x01];
	}
	temp[0] = *(p) ^ key[0];
	
	memcpy(data,temp,6);
}
void RADIO_IRQHandler()
{		
	int i;
	unsigned char temp[6];
	// The very first time the radio is powered up we have to trigger the START task after getting the READY event
    // (note, this could also have been handled by a shortcut)
    if(NRF_RADIO->EVENTS_READY)
    {
        NRF_RADIO->EVENTS_READY = 0;       
        // Start listening and wait for the END event
        NRF_RADIO->TASKS_START = 1;
    }    
    // Every time the END event is triggered we have received a packet on the radio
    // Check the CRC status on the packet, do whatever we want with it, and trigger the START task again to enable the receiver for new data
    if(NRF_RADIO->EVENTS_END)
    {
        NRF_RADIO->EVENTS_END = 0;       
        if(NRF_RADIO->CRCSTATUS == 1)
        {
			while(NRF_RADIO->EVENTS_RSSIEND == 0)
			{}
			nrf_rx_packet[6]=NRF_RADIO->RSSISAMPLE;	
			NRF_RADIO->EVENTS_RSSIEND = 0;	
			data_decrypt(nrf_rx_packet,&temp[0]);
			for(i=0;i<6;i++)
				printf("%02x,",temp[i]);
				printf("-%03d,",nrf_rx_packet[6]);
				printf("\r\n");
			//if(nrf_rx_packet[0]==0xf4)
			{	
				if (instart + LEN != inend) 
				{
					//inbuf[hspi->inend++ & (ILEN-1)] =hspi->Instance->DR;
					//1B length +11B payload +1B RSSI +4B timer+1B checksum
					checksum=0;
					for(i=0; i<(PLEN); i++)
					{
						rx_fifo[inend & (LEN-1)][i] = nrf_rx_packet[i+2];
					}
					inend+=1;
					inempty = 0;
				}
			}
        }        
        // Start listening and wait for the END event
        NRF_RADIO->TASKS_START = 1;				
    }
}





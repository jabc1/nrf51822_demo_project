#include "Sys.h"
#include "base.h"
#include "radio_config.h"
#include "system_nrf51.h"
void sysinit(void)
{
#if 1
	 //�жϵ�Դʱ��״̬
	 if((((*(uint32_t *)0xF0000FE0) & 0xFF) == 1) && (((*(uint32_t *)0xF0000FE4) & 0xF) == 0))
     {
        if ((((*(uint32_t *)0xF0000FE8) & 0xF0) == 0x0) && (((*(uint32_t *)0xF0000FEC) & 0xF0) == 0x0))
        {
			//���radio�޷�д��  
          *(uint32_t *)0x40000504 = 0xC007FFDF;//��Դʱ������
          *(uint32_t *)0x40006C18 = 0x00008000;//GPIOTE����    
        }
		else if ((((*(uint32_t *)0xF0000FE8) & 0xF0) == 0x10) && (((*(uint32_t *)0xF0000FEC) & 0xF0) == 0x0))
        {
          //���radio�޷�д��  
          *(uint32_t *)0x40000504 = 0xC007FFDF;//��Դʱ������
          *(uint32_t *)0x40006C18 = 0x00008000;//GPIOTE����  
        }
        else if ((((*(uint32_t *)0xF0000FE8) & 0xF0) == 0x30) && (((*(uint32_t *)0xF0000FEC) & 0xF0) == 0x0))
        {
          //���radio�޷�д��  
          *(uint32_t *)0x40000504 = 0xC007FFDF;//��Դʱ������
          *(uint32_t *)0x40006C18 = 0x00008000;//GPIOTE����    
        }
    }
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;//HFCLK��������״̬0�رգ�1����
	NRF_CLOCK->TASKS_HFCLKSTART = 1;//���⾧��HFCLK 0�رգ�1����
	while(!NRF_CLOCK->EVENTS_HFCLKSTARTED);//�ȴ�HFCLK��������״̬Ϊ1
#else
	/* Start 16 MHz crystal oscillator */
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;

	/* Wait for the external oscillator to start up */
	while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) 
	{
	}

	// Set radio configuration parameters
	radio_configure();

	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);

	// Set payload pointer
	NRF_RADIO->PACKETPTR = (uint32_t)packet;  
#endif
}






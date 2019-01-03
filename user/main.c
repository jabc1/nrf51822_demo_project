/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : main.c
Description: ʵ��mcu�����ʼ�����̳߳�ʼ��
Platform   : MDK V5.26.0.0
Version    : V1.0
Author	   : Jason
Create Time: 2018-12-13
Modify	   : 
Modify Time: 
******************************************************************************/
#include "radio_config.h"
#include "Sys.h"
#include "time.h"
#include "uart.h"
#include "gpio.h"
#include "memory.h"
#include "task.h"
#include "runflag.h"
#include "SPI.h"
#include "Flash.h"

int main(void)
{
	Thread_Init();
	Thread_Login(FOREVER, 0, 500, &main_task);//������
	Thread_Login(FOREVER, 0, 200, &setting_radio);//����radio���ù���
	Thread_Login(FOREVER, 0, 10, &RF_function);//����radio�������ݹ���
	Thread_Login(FOREVER, 0, 50, &send_function);//�����Ͷ���
	Thread_Login(FOREVER, 0, 1000, &spis_process);//spiͨ����Ϣ������
	sysinit();
	time_init();
	Init_fifo();
	RunFlagInit();
	gpio_inti();
	spis_gpio_init();
	usart_init();
	radio_configure();
	radio();
	flash_init();
//	flash_test();
	printf("System start!\r\n");
	while(1)
	{
		RunFlagHandler();
		Thread_Process();
	}
}

/**
 *@}
 **/









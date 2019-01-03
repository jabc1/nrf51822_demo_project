/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : main.c
Description: 实现mcu外设初始化及线程初始化
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
	Thread_Login(FOREVER, 0, 500, &main_task);//主任务
	Thread_Login(FOREVER, 0, 200, &setting_radio);//设置radio配置功能
	Thread_Login(FOREVER, 0, 10, &RF_function);//处理radio接收数据功能
	Thread_Login(FOREVER, 0, 50, &send_function);//处理发送队列
	Thread_Login(FOREVER, 0, 1000, &spis_process);//spi通信消息处理功能
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









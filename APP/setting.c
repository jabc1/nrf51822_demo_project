/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : setting.c
Description: 实现设置radio功能
Platform   : MDK V5.26.0.0
Version    : V1.0
Author	   : Jason
Create Time: 2018-12-23
Modify	   : 
Modify Time: 
******************************************************************************/
#include "setting.h"
#include "fifo.h"
#include "task.h"
#include "stdio.h"
/*
设置    频率：7e 06 01 1a a0 10 5a 74
设置  地址码：7e 0a 01 1a a1 10 11 12 13 14 76 ff
设置    速率：7e 06 01 1a a2 01 db b5
设置接收通道：7e 06 01 1a a3 00 1a 75
*/
u8 set_fun(u8 datatype,u8 *data)
{
	switch(datatype)
	{
		case setfer:
			set_fer(data);
			break;
		case setmac:
			set_mac(data);
			break;
		case setspeed:
			set_speed(data);
			break;
		case setch:
			set_ch(data);
			break;
		case setsendch:
			break;
		case setsendpow:
			break;
		default:
			break;
	}
	return 1;
}
u8 sel_fun(u8 datatype,u8 *data)
{
	switch(datatype)
	{
		case 0x00:
			break;
		case selinfo:
			break;
		default:
			break;
	}
	return 1;
}

u8 set_fer(u8 *data)
{
	u8 temp;
	temp = *data;
	switch(temp)
	{
		case 0x10:
			printf("setting succ!!!\r\n");
			break;
		case 0x20:
			break;
		default:
			break;
	}
	printf("setting num = 0x%02x\r\n",temp);
	return 1;
}
u8 set_mac(u8 *data)
{
	_Mac *Mac;
	Mac = (_Mac *)data;
	printf("mac test %s\r\n",Mac->mbuf);
	return 1;
}
u8 set_speed(u8 *data)
{
	u8 temp;
	temp = *data;
	printf("speed=%02d\r\n",temp);
	return 1;
}
u8 set_ch(u8 *data)
{
	u8 temp;
	temp = *data;
	printf("ch = %02d\r\n",temp);
	return 1;
}





















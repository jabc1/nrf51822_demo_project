/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : task.c
Description: 实现各个函数功能
Platform   : MDK V5.26.0.0
Version    : V1.0
Author	   : Jason
Create Time: 2018-12-3
Modify	   : 
Modify Time: 
******************************************************************************/
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "memory.h"
#include "decrypt.h"
#include "runflag.h"
#include "Analysis.h"
#include "setting.h"
#include "Analysis.h"

static u8 rfdata[7];
_DATA		dtemp;
_CMD		ctemp;

void setting_radio()
{
//	printf("setting_radio\r\n");
}

void RF_function()
{
	u8 temp[7]={0,};
	if(!fifo_empty(&Radiofifo))
	{
		fifo_gets(&Radiofifo,temp,7);//出队
		data_decrypt(temp,&rfdata[0]);//软件解密
		rfdata[6] = temp[6];
		fifo_puts(&Uart0Tx,rfdata,7);//入队操作
	}
}
void send_function()
{
	u8 temp[7]={0,},i;
	if(!fifo_empty(&Uart0Tx))
	{
		fifo_gets(&Uart0Tx,temp,7);
		for(i=1;i<6;i++)
		{
			printf("%02X",temp[i]);
		}
		printf("-%03d",temp[6]);
	}
}
void main_task()
{
	u8 len_temp=0;
//	u8 temp[10]={0,};
	if(!fifo_empty(&Uart0Rx))
	{
//		fifo_gets(&Uart0Rx,temp,10);
//		printf("%s\r\n",temp);
		memset(dtemp.fbuff,0,sizeof(dtemp.fbuff));
		if(info_out_fifo(&Uart0Rx,&len_temp,&dtemp.fbuff[0]))
		{
			Analysis_function(&ctemp.cmdtype,&ctemp.datatype,&dtemp.bbuff[0],dtemp.fbuff,&len_temp);
			if(ctemp.cmdtype == settype)
			{
				set_fun(ctemp.datatype,dtemp.bbuff);
			}
			else if(ctemp.cmdtype == seltype)
			{
				sel_fun(ctemp.datatype,dtemp.bbuff);
			}
		}
		
	}
}















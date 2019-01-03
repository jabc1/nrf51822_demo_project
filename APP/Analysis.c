/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : Analysis.c
Description: 实现设置radio功能
Platform   : MDK V5.26.0.0
Version    : V1.0
Author	   : Jason
Create Time: 2018-12-3
Modify	   : 
Modify Time: 
******************************************************************************/
#include "Analysis.h"
#include "task.h"
#include "string.h"
#include "crc.h"

u8 Analysis_function(u8 *cmdtype,u8 *datatype,u8 *outdata,u8 *indata,u8 *len)
{
	_CMD *pbuff;
	u16 crc_temp;
	u16 *crc;
	pbuff = (_CMD *)indata;
	if((pbuff->head != HEAD)||(pbuff->softver != Soft_ver))
	{
		return false;
	}
	crc_temp = check_crc(&(pbuff->len),(pbuff->len-2),0xffff);
	crc = (u16 *)((u8 *)pbuff+2+(pbuff->len-2));
	if(*crc != crc_temp)
	{
		return false;
	}
	*cmdtype = pbuff->cmdtype;
	*datatype = pbuff->datatype;
	memcpy(outdata,&(pbuff->dbuff),(pbuff->len - 5));
	return true;
}
u8 Unpack_function()
{
	;
	return 1;
}

u8 info_out_fifo(FIFO_t *fifoytpe,u8 *len,u8 *data)
{
	u8 length,head_temp;
	do{
		if (!fifo_getc(fifoytpe, &head_temp))
		{
			return FALSE;
		}
	}while(head_temp != 0x7e);
	
	*data++ = head_temp;
	if(fifo_getc(fifoytpe, &length))
	{
		*data++ = length;
		if(!fifo_gets(fifoytpe,data,length))
		{
			return false;
		}
	}
	*len = length;
	return true;
}

























/************Copyright(C) Kaikai Technology 2018-05-14***********************
File name  : decrypt.c
Description: 实现软件信息解密功能
Platform   : MDK V5.26.0.0
Version    : V1.0
Author	   : Jason
Create Time: 2018-12-3
Modify	   : 
Modify Time: 
******************************************************************************/
#include "decrypt.h"
#include "string.h"
#include "stdio.h"
#include <stdlib.h>

u8 key[2] = {0x41,0x55};
u8 data_decrypt(u8 *buf,u8 *data)
{
	u8 *p;
	u8 temp[6] = {0,},i;
	p = (u8 *) malloc(15);
	p = buf;
	for(i=5;i>=1;i--)
	{
		temp[i] = *(p+i) ^ *(p+(i-1)) ^ key[i&0x01];
	}
	temp[0] = *(p) ^ key[0];
	
	memcpy(data,temp,6);
	free(p);
	return 1;
}























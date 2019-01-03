#ifndef _task_h
#define _task_h

#include "Thread.h"
//设置字节长度为一个字节主要是因为通信内容不太长
#define			HEAD			0x7e
#define			Soft_ver		0x01//软件版本

#define			settype			0x1a//设置命令类型
#define			seltype			0x2a//查询命令类型

#define			setfer			0xa0//设置频率
#define			setmac			0xa1//设置地址码
#define			setspeed		0xa2//设置速度
#define			setch			0xa3//设置通道

#define			setsendch		0xb0//设置发送通道
#define			setsendpow		0xb1//设置发送功率

#define			selinfo			0xc0//查询标签信息

void setting_radio(void);
void RF_function(void);
void send_function(void);
void main_task(void);


#pragma	pack(push,1)
//typedef struct{
//	u8 fercmd;
//	u8 fer;
//}_Fer;//设置频率
//typedef struct{
//	u8 Maccmd;
//	u8 mbuff[5];
//}_Mac;//设置地址码
//typedef struct{
//	u8 Speedcmd;
//	u8 speed;
//}_Speed;//设置速度
//typedef struct{
//	u8 Chcmd;
//	u8 ch;
//}_Ch;//设置接收通道
//typedef struct{
//	u8 Sendcmd;
//	u8 SendCh;
//}_Send;//设置发送通道
//typedef struct{
//	u8 powercmd;
//	u8 power;
//}_Power;//设置发送功率
//typedef struct{
//	_Fer	ffer;
//	_Mac	mmac;
//	_Speed	sspeed;
//	_Ch		cch;
//	_Send	ssend;
//	_Power	ppower;
//}_SET;//设置
typedef struct{
	u8 mbuf[5];
}_Mac;
typedef struct{
	u8 head;
	u8 len;
	u8 softver;
	u8 cmdtype;
	u8 datatype;
	u8 dbuff[240];//命令内容最大支持240byte
	u16 crc;
}_CMD;
typedef struct{
	u8 fbuff[255];
	u8 bbuff[255];
}_DATA;
#pragma	pack(pop)

extern _DATA		dtemp;
extern _CMD			ctemp;

#endif





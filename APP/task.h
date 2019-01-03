#ifndef _task_h
#define _task_h

#include "Thread.h"
//�����ֽڳ���Ϊһ���ֽ���Ҫ����Ϊͨ�����ݲ�̫��
#define			HEAD			0x7e
#define			Soft_ver		0x01//����汾

#define			settype			0x1a//������������
#define			seltype			0x2a//��ѯ��������

#define			setfer			0xa0//����Ƶ��
#define			setmac			0xa1//���õ�ַ��
#define			setspeed		0xa2//�����ٶ�
#define			setch			0xa3//����ͨ��

#define			setsendch		0xb0//���÷���ͨ��
#define			setsendpow		0xb1//���÷��͹���

#define			selinfo			0xc0//��ѯ��ǩ��Ϣ

void setting_radio(void);
void RF_function(void);
void send_function(void);
void main_task(void);


#pragma	pack(push,1)
//typedef struct{
//	u8 fercmd;
//	u8 fer;
//}_Fer;//����Ƶ��
//typedef struct{
//	u8 Maccmd;
//	u8 mbuff[5];
//}_Mac;//���õ�ַ��
//typedef struct{
//	u8 Speedcmd;
//	u8 speed;
//}_Speed;//�����ٶ�
//typedef struct{
//	u8 Chcmd;
//	u8 ch;
//}_Ch;//���ý���ͨ��
//typedef struct{
//	u8 Sendcmd;
//	u8 SendCh;
//}_Send;//���÷���ͨ��
//typedef struct{
//	u8 powercmd;
//	u8 power;
//}_Power;//���÷��͹���
//typedef struct{
//	_Fer	ffer;
//	_Mac	mmac;
//	_Speed	sspeed;
//	_Ch		cch;
//	_Send	ssend;
//	_Power	ppower;
//}_SET;//����
typedef struct{
	u8 mbuf[5];
}_Mac;
typedef struct{
	u8 head;
	u8 len;
	u8 softver;
	u8 cmdtype;
	u8 datatype;
	u8 dbuff[240];//�����������֧��240byte
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





#include <STC12C.H>
#include <stdio.h>
#include <stdlib.h>
#include <intrins.h>
#include <proce.h>
#include <main.h>
#include <figer.h>
//#include <Time.h>
#include <ADC.h>
//#include <KEY.h>
#include <IAP.h>
#include <OS.h>
#include <string.h>
#include <math.h>

#define INT8U unsigned char        //�޷���8λ��
#define INT8S signed char          //�з���8λ��
#define INT16U unsigned int        //�޷���16λ��
#define INT16S signed int          //�з���16λ��
#define INT32U unsigned long       //�޷���32λ��
#define INT32S signed long         //�з���32λ��
#define FP32 float                 //�����ȸ�����
#define FP64 double                //˫���ȸ�����

//************************** 8051�ж�Դ���� ***********************
#define		INT_EX0		0		//�ⲿ�ж�0Դ,P3.2
#define		INT_EX1		2		//�ⲿ�ж�1Դ,P3.3
#define		INT_T0		1		//��ʱ��0�ж�Դ,Timer0
#define		INT_T1		3		//��ʱ��1�ж�Դ,Timer1
#define		INT_SER		4		//�����ж�Դ,P3.0,P3.1
#define   INT_ADC   5   //ADC�ж�Դ
//****************************************************************
//***********************����LED������I/O���ƶ˿�*******************

sbit PIN_IA        =P3^4;
sbit PIN_IB        =P3^5;
sbit PIN_PWR       =P3^2;
//sbit PIN_FPWR      =P3^3;
sbit PIN_RLED      =P1^1;
sbit PIN_GLED      =P1^2;
sbit PIN_BUZ       =P3^7;

//sbit Pin_NC        =P1^4;
//sbit Pin_SP        =P2^5;
//sbit Pin_CLOSE     =P2^4;
//**************************���尴���˿�***************************
//sbit Pin_K1=P2^0;
sbit Pin_K2=P1^7;
sbit Pin_K3=P1^3;
sbit Pin_K4=P1^4;
sbit Pin_K5=P1^5;
//sbit Pin_K6=P2^2;
sbit Pin_K7=P3^3;
sbit Pin_KBo=P1^6;
//***************************************************************

#define BZ_ON         1              //���������ض���
#define BZ_OFF        0
#define POWER_OFF     0              //
#define POWER_ON      1

//***************************************************************
#define LED_FLASH  1
#define LED_NORMAL 0

#define red   0
#define green 1
#define on    1
#define off   0
#define FALSE 0
#define TURE  1
#define key_login 1
#define key_del   2
#define key_clear_all  3

#define land   1 //��½
#define NoLand 0 //δ��½

sbit Add_Key=P0^7;
sbit Del_Key=P0^6;

//sbit PIN_LED0=P1^3;
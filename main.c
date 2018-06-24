//****************************************************************
//  项目名称:指纹锁
//  作    者:张俊锋
//  芯片名称:STC12LE5616AD
//  V1.0日期:2008-05-20
//  版    本:V2.0
//  备    注:方程式指纹模块
//  修改日期:2011-09-14
//****************************************************************
#define MAIN_GLOBALS
#include <includes.h>
#define CODE_K0 0x10
#define CODE_K1 0x08
#define CODE_K2 0x04
#define CODE_K3 0x02
#define DELAY_TIM 200

#define FUN_00 0x00
#define FUN_01 0x01
#define FUN_02 0x02
#define FUN_03 0x03
#define FUN_04 0x04

//#define CODE_K6 0x20
//#define CODE_K7 0x40
unsigned char key_number;
unsigned char key();
unsigned char key_pwr();
unsigned char key_fun();
unsigned char FlagBuf;
unsigned char T0cnt=0;
unsigned char ScanPwrBuf[8];
unsigned char fPwrOk;
unsigned char fDkey;
unsigned char fAdCir;
unsigned char fSec;
unsigned int mainTimer;
//unsigned char MK;
INT8U UsePwrBuf[6][8];  //用户密码数据
INT8U SuperPwrBuf[6];   //厂家密码数据

INT16U code ADR_PWRDATA[]={0x0800,0x0810,0x0820,0x0830,0x0840,0x0850};
//密码存放地址 1个管理密码和6个用户密码

//------------------------------------------------------------------------------------------------
//以下是
//------------------------------------------------------------------------------------------------
static void MainTxdByte(unsigned char i)
{

    TI = 0;
    SBUF = i;
    while(!TI);
   TI = 0;
}

void  mem_ini()                   //内存初始化程序
{
  INT8U i,j,k;
  INT16U i_16u;
  //INT8U pwr_ran[6];
  i=0;
	j=0;
	i_16u=0;
	clk0=0;
	clk1=0;
	clk2=0;
	Second=0;
  Minute=0;
  doorflag=0;
  fDkey=0;
  fAdCir=0;
  PointADC=0;
  SearchNumber=0xffff;//搜索到的指纹序列号
  //SaveNumber=0xffff;
	P1M0=0x01;//0000 0001b
	P1M1=0x00;//0100 0000b
	P2M0=0x00;//
	P2M1=0x00;
	P3M0=0x00;//0000 0000b
	P3M1=0xBc;//1011 1100b
	P1=0xff;
	P2=0xff;
	P3=0x4b;  //0100 1011b
	PIN_IA=0;
	PIN_IB=0;
	PIN_PWR=1;
	//PIN_FPWR=1;
	flag_com_rx_finish=0;
	//Module_power(off);
	AUXR=0x30;
	ADCrackDat=182;
	i=IAP_read(IAP_ADR_DEFAULT);
//------------------------------------------------------------------
//------------------------------------------------------------------
	for(k=0;k<6;k++)  //读取用户密码数据
	{
	  for(i_16u=ADR_PWRDATA[k];i_16u<(ADR_PWRDATA[k]+8);i_16u++)
	  {
	  	j=i_16u-ADR_PWRDATA[k];
	  	UsePwrBuf[k][j]=IAP_read(i_16u);
	  }
	}
	for(i_16u=0;i_16u<58;i_16u++)   //读取指纹模板表
	{
		userFigerTab[i_16u]=IAP_read(ADR_FTAB+i_16u);
	}
}

//*******************************************************************************************************
unsigned char keytest()
{
	unsigned char key_temp;
  key_temp=0x00;
//************************键盘扫描************************************
  //Pin_K1=1;
  Pin_K2=1;
  Pin_K3=1;
  Pin_K4=1;
  Pin_K5=1;
  //Pin_K6=1;
  //Pin_K7=1;
	//if(Pin_K1)key_temp|=0x01;else key_temp&=0xfe;
	if(!Pin_K2)key_temp|=0x02;else key_temp&=0xfd;
	if(!Pin_K3)key_temp|=0x04;else key_temp&=0xfb;
	if(!Pin_K4)key_temp|=0x08;else key_temp&=0xf7;
	if(!Pin_K5)key_temp|=0x10;else key_temp&=0xef;
	//if(!Pin_K6)key_temp|=0x20;else key_temp&=0xdf;
	//if(Pin_K7)key_temp|=0x40;else key_temp&=0xbf;
	//if(!Pin_K8)key_temp|=0x80;else key_temp&=0x7f;
	return key_temp;
}
unsigned char key()
{
	unsigned char key_buf00=0;
	unsigned char key_buf01=0;
	LED_delay_time(10);
  key_buf00=keytest();
  LED_delay_time(10);
  key_buf01=keytest();
  if(key_buf00!=key_buf01)return 0;
  else
  {
  	return key_buf00;
  }
}

unsigned char ScanOpenWrd()
{
	unsigned char key_number;
	unsigned char l=0;
	Clear_Timer();  //清定时器
	while(l<8)  //8位密码
	{
		key_number=key();
		WDG_CONTR=0x3d;  //喂狗 1.13s
  	switch(key_number)
  	{
  		case CODE_K0:
  		{
  			ScanPwrBuf[l]=2;
  			BuzzerCTL(1,150);
  			l++;
  			Clear_Timer();
  		}break;
  		case CODE_K1:
  		{
  			ScanPwrBuf[l]=3;
  			BuzzerCTL(1,150);
  			l++;
  			Clear_Timer();
  		}break;
  		case CODE_K2:
  		{
  			ScanPwrBuf[l]=4;
  			BuzzerCTL(1,150);
  			l++;
  			Clear_Timer();
  		}break;
  		case CODE_K3:
  		{
  			ScanPwrBuf[l]=5;
  			BuzzerCTL(1,150);
  			l++;
  			Clear_Timer();
  		}break;
  		default:break;
  	}
  	if(Second>=10)
		{
			GreenOff();
			RedOn();
			BuzzerCTL(2,150);
			RedOff();
			power(off);
		}
  }
  return(1);
}

//*******************************************************************************************************
void Open_Door_Program()                  //开门程序流程
{
  unsigned char i=0;
  //Module_power(off);                      //关闭模块电源
  GreenOn();//RedOn();
  BuzzerCTL(1,200);
  OpenDoor();
  RedOff();
  Clear_Timer();
  for(i=0;i<6;i++)
  {
  	LED_delay_time(633);
  	WDG_CONTR=0x3d;  //喂狗 1.13s
  }
  CloseDoor();
  return;    //如果5秒内没有按门把手  直接关门
}
//*******************************************************************************************************

void	T0_Ini() reentrant using 0		//定时器0初始化,5ms中断一次
{
	TR0	= 0;
	TF0	= 0;
	TH0	= 0xdc;
	TL0	= 0x31;
	TR0	= 1;
	ET0	= 1;
}

void	Ser_Ini()  reentrant using 0
{
	PCON &= 0x7f;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xbf;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xfe;		//串口1选择定时器1为波特率发生器
	//TMOD &= 0x0f;		//清除定时器1模式位
	//TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TMOD = 0x21;
	TL1 = 0xFA;		//设定定时初值
	TH1 = 0xFA;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	ES=1;
}

//------------------------------------------------------------------------------------------------
//以下是主函数
//------------------------------------------------------------------------------------------------
void main()
{
	INT8U i,i_buf[3],j,k,l,mSel;
  INT8S i_s,j_s;
  unsigned char err;
  INT32U i_32u,j_32u;
  INT16U i_16u,j_16u;
	//bit low_power_flag=0;
  //unsigned char i;
  unsigned char MK1,MK2,MK3;
  unsigned char Compare_Number=0;
  //MK=0;
  
  MK1=0;
  MK2=0;
  MK3=0;
  //Module_power(off);
  i=0;
  j=0;
  k=0;
  l=0;
  i_s=0;
  j_s=0;
  i_32u=0;
  j_32u=0;
  i_16u=0;
  j_16u=0;
  //T0_Ini();
	//Ser_Ini();
	//EA=1;
	mem_ini();
	WDG_CONTR=0x3d;  //喂狗 1.13s
	Pin_K7=1;
	Pin_KBo=1;
	BuzzerCTL(1,150);
	//PIN_PWR=1;
	//Pin_SP=0;
  //-------------------------test
	//PIN_GLED=0;
	//LED_delay_time(633);
	//PIN_GLED=1;
	//WDG_CONTR=0x3d;  //喂狗 1.13s
	//while(1){;}
	//while(1)
	//{
	//	LED_delay_time(633);  //1000=790ms  633=0.5s
	//	PIN_GLED=!PIN_GLED;
	//}
	//--------------------------
	T0_Ini();
	Ser_Ini();
	ADC_Init();
	fAdCir=0;
	
	//-----------------------------------指纹开门或者芯片开门--------------------------
	
	systeminit();
	//LED_delay_time(10);
	Clear_Timer();
  
  //
  //Open_Door_Program();
  //while(1)
  //{
  //	i=key();
  //	if(i!=0)MainTxdByte(i);
  //	WDG_CONTR=0x3d;  //喂狗 1.13s
  //}
  
  GreenOn();
  //LED_Red_Flash(3,400);
  //power(off);                       //关闭电源
  ADC_CONTR=ADC_CONTR | 0x08;               //启动AD转换
  if (VefPSW())
  {
  	Clear_Timer();  //清定时器
  	if(!Pin_K7)
  	{
    	while(!Pin_K7) //后功能键按下,长按10秒则清除指纹与密码
    	{
    		LED_delay_time(10);
    		if(Second>=10)
    		{
    			IAP_erase(ADR_USERPWR);   //清除密码
	  	    for(k=0;k<6;k++)  //读取用户密码数据
	        {
	          for(i_16u=ADR_PWRDATA[k];i_16u<(ADR_PWRDATA[k]+8);i_16u++)
	          {
	          	j=i_16u-ADR_PWRDATA[k];
	          	UsePwrBuf[k][j]=IAP_read(i_16u);
	          }
	        }
	        delete_all();                     //删除全部指纹程序
	        power(off);                       //关闭电源
    		}
    	}
    	power(off);                       //关闭电源
    }
    if(!Pin_KBo)
    {
    	LED_delay_time(10);
    	if(!Pin_KBo)
    	{
    		GreenOn();//RedOn();
    		BuzzerCTL(1,200);
    		OpenDoor();
    		RedOff();
    		Clear_Timer();
    		power(off);                       //关闭电源
    	}
    }
  	mainTimer=0;
    fSec=0;
    while(fSec==0)
    {
    	WDG_CONTR=0x3d;  //喂狗 1.13s
    	i=key();
    	if(i!=0)
    	{
    		BuzzerCTL(1,150);
    		break;
    	}
    }
    switch(i)
    {
    	case CODE_K0:
    		mSel=FUN_01;
    		break;
    	case CODE_K1:
    		mSel=FUN_02;
    		break;
    	case CODE_K2:
    		mSel=FUN_03;
    		break;
    	case CODE_K3:
    		mSel=FUN_04;
    		break;
    	default:
    		mSel=FUN_00;
    		break;
    }
    GreenOff();
  	while(1)
	  {
      WDG_CONTR=0x3d;  //喂狗 1.13s
      switch (mSel)
      {
      	case FUN_01:                        //登记指纹
        {
        	GreenOn();
          //BuzzerCTL(1,200);
          login();                          //登记指纹程序
	        if (doorflag==1){CloseDoor();}
          power(off);                       //关闭电源
        }break;
        case FUN_02:                       //删除指纹
        {
        	GreenOn();
          //BuzzerCTL(1,200);
          deletef();                         //删除指纹程序
	        if (doorflag==1){CloseDoor();}
          power(off);                       //关闭电源
        }break;
        case FUN_03:                       //密码开门
        {
        	GreenOn();
          //BuzzerCTL(1,200);
        	if(ScanOpenWrd())
			    {
		      	for(i=0;i<6;i++)
		      	{
		      		if(ArrCompare(UsePwrBuf[i],ScanPwrBuf,8))
		      		{
		      			Open_Door_Program();	    //开门程序流程
                power(off);               //关闭电源
		      		}
		      	}
		      	GreenOff();
		      	RedOn();
		      	BuzzerCTL(2,150);
		      	RedOff();
		      	power(off);
		      }
        }break;
        case FUN_04:                       //增加密码
        {
        	GreenOn();
          //LED_delay_time(10);
				  Clear_Timer();  //清定时器
				  i=AdminFiger_chek();
				  GreenBuzzerCTL(1,150);
				  GreenOn();
				  ScanOpenWrd();
				  for(i=0;i<6;i++)
				  {
				  	if(UsePwrBuf[i][0]==0||UsePwrBuf[i][0]>=6)  //密码为空
				  	{
				  		for(j=0;j<8;j++)
				  		{
				  			UsePwrBuf[i][j]=ScanPwrBuf[j];
				  		}
				  		IAP_erase(ADR_USERPWR);
				  		for(k=0;k<6;k++)  //写入用户密码数据
	            {
	              for(i_16u=ADR_PWRDATA[k];i_16u<(ADR_PWRDATA[k]+8);i_16u++)
	              {
	              	j=i_16u-ADR_PWRDATA[k];
	              	IAP_pro_ver(i_16u,UsePwrBuf[k][j]);
	              }
	            }
				  		RedOff();
				      GreenOn();
				  		BuzzerCTL(1,1000);
				  		GreenOff();
				  		power(off);
				  	}
				  }
				  //密码已满
				  GreenOff();
				  RedOn();
				  BuzzerCTL(3,150);
				  RedOff();
				  power(off);
				  BuzzerCTL(1,1000);
				  power(off);
        }break;
        default:                            //指纹开门
        {
        	WDG_CONTR=0x3d;  //喂狗 1.13s
        	//Pin_SP=1;
        	if(MK1==0)
        	{
        		FlagBuf=ReadUserInfo();
        		MK1=1;
            i_buf[0]=3;
            i_buf[1]=0;
            i_buf[2]=57;
            flag_com_rx_finish=0;
            Command(i_buf);
            OSSemPend(&flag_com_rx_finish,500,&err);
        	}
          switch (FlagBuf)
          {
            //-------------------------------------------------------------------------
            case 1:                         //指纹库空 开门 进行 出厂设置
            {
            	if (doorflag==0){OpenDoor();doorflag=1; clk2=0;Second=0;}
            	if (Second>=5)                //如果超过5秒  关闭电源
					    {
					    	RedOn();
                BuzzerCTL(2,150);
						    RedOff();
					 	    if (doorflag==1){CloseDoor();}
		            //LowVolAlarm();//电压检测报警
                power(off);                 //关闭电源
              }
              // delay(5);
            }break;
            //-------------------------------------------------------------------------
            case 2:
            {
            	//if(Pin_NC)   //如果检测到芯片信号
            	//{
            	//	Module_power(off);
            	//	Open_Door_Program();	    //开门程序流程
							//  power(off);               //关闭电源
            	//}
            	i=Main_searchfinger();
            	if(i==0xaa)
            	{
            		//Module_power(off);
            		Open_Door_Program();	    //开门程序流程
							  power(off);               //关闭电源
							}
              else if(i==0x55)            //比对失败，则重试
	            {
                Compare_Number++;
							  RedOn();
                BuzzerCTL(3,150);         //delay(20);
							  RedOff();
                if ((Compare_Number)>=3)  //如果3次验证不通过,关闭电源
	              {
	              	//Module_power(off);
	              	//LowVolAlarm();//电压检测报警
	              	power(off);             //关闭电源
	              }
	              clk2=0;
                Second=0;
                i_buf[0]=3;
                i_buf[1]=0;
                i_buf[2]=57;
                flag_com_rx_finish=0;
                Command(i_buf);
                OSSemPend(&flag_com_rx_finish,500,&err);
              }
              else if(i==0)
              {
              	if (Second>=8)             //如果超过8秒  关闭电源
              	{
              		RedOn();
              		BuzzerCTL(3,150);
              		RedOff();
              		if (doorflag==1){CloseDoor();}
              		//LowVolAlarm();//电压检测报警
              		power(off);               //关闭电源
              	}
              	delay(3);
              }
              else
              {
              	RedOn();
                BuzzerCTL(3,150);         //delay(20);
							  RedOff();
							  power(off);               //关闭电源
              }
            }break;
            //-------------------------------------------------------------------------
            default:
            {
            	RedGreenBuzzerCTL(3,200);     //红灯，绿灯，蜂鸣器同时短闪叫3次
            	//LowVolAlarm();//电压检测报警
            	power(off);                   //关闭电源
            }
            //-------------------------------------------------------------------------
          }
        }
        //----end of default
      }
    }
  }
  //LowVolAlarm();//电压检测报警
  power(off);                               //关闭电源
}


//定时器0中断,不够8个就在此发送
void T0_int(void) interrupt INT_T0		//5ms进入中断一次.
{
	TH0	= 0xdc;
	TL0	= 0x31;
	//Pin_test=!Pin_test;
	clk2++;
	clk0++;
	clk1++;
	mainTimer++;
	if(mainTimer>=DELAY_TIM)
	{
		mainTimer=DELAY_TIM+1;
		fSec=1;
	}
 //ADC_CONTR=ADC_CONTR | 0x08;           //启动AD转换
 if (clk2==200)
 {
   clk2=0;
   Second++;
   if(Second>=60){Minute++;Second=0;}
 }
	cntSemPend++;
	if(cntComRxTimeout>=5)
	{
		cntComRxTimeout=5;
		if(cntComRxIndex!=0)
		{
			comRxLength=cntComRxIndex;
			flag_com_rx_finish=1;
		}
		cntComRxIndex=0;
	}
	cntComRxTimeout++;
	//CntCanTxTimeout++;
}
//串口接收中断函数 
void serial () interrupt 4 using 1 
{
	unsigned char ch;
  if(RI)
  {
  	RI = 0;
    ch=SBUF;
  	comRxDataBuf[cntComRxIndex++] = ch;
  	cntComRxTimeout=0;
  }
}

void ADC_int(void) interrupt INT_ADC		//ADC中断
{
	//INT16U i_16u,j_16u;
  
  INT8U ADC_buf;                      //AD转换寄存器
  ADC_CONTR = ADC_CONTR & 0xef;         //清转换结束标志
  ADC_buf = ADC_DATA;
	ADBuf[PointADC]=ADC_DATA;
	PointADC++;
	if(PointADC>=ADC_BUF_SIZE)fAdCir=1;
	PointADC%=ADC_BUF_SIZE;
  ADC_CONTR=ADC_CONTR | 0x08;           //启动AD转换
}
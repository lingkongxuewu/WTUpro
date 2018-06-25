/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Init.c
* 版　本：  v1.0
* 描  述：  100T双轴 整合后的版本
* 创建人：   
* 日  期：   
--------------------------------------------------------------------------
*当前版本:	v1.0
*修 改 人:
*日　  期:
----------------------------------修改履历--------------------------------
1、
2、
3、
4、
------------------------------------------------------------------------*/
#define   __INIT_C__
#include  "_Include.h"

void Init_SystemPara (void)
{
	SendActPosi=1;
	SendSpeedNum=1;		        			//????????????????????????
    MainTime4Cnt = SendSpeed[SendSpeedNum];	

	TempA=2500;
	Braud_Num=3;
	Addr=1;
	AngRange=30;

}

static void Sys_ClkInit (void)
{	
	int8u SfrPage_ = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	OSCICN = 0xD7;//0xD5;		//6M					  

	CLKSEL = 0X00;				//	选择内部振荡器作为系统时钟
	while((OSCICN & BIT4) == 0);//	等待时钟稳定
	VDM0CN = 0xA0;				//	Enable VDD monitor and high threshold
	RSTSRC = 0x02;				//	Enable VDD monitor as a reset source

	SFRPAGE = SfrPage_;
}
//***********************************************************************/
// *功    能： 初始化I/O口操作
// *入    口： 无
// *出    口：无
// *备    注：
// *函数性质：
//*说明：I/O口设置为模拟时，PXMIN相应位设置为0，I/O口设置为数字时，PXMIN相应为设置为1（默认状态为1）
//*		 I/O口设置为输出时，PXMDOUT相应位设置为1，I/O口为输入PXMDOUT相应位设置为0（默认状态为0）
//*		 I/O口设置为模拟，通用I/O口时以及特殊功能时PXSKIP相应为设置为1（默认状态为0）
//***********************************************************************/
void PortInit(void)
{
	int8u SfrPage = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	
	P0SKIP  |= 0x0F;	//skip P0.0 P0.1 p0.2 p0.3 
	P0MDIN  &= ~0x01;   //set P0.0 analog input(VREF)
	P0MDOUT |= 0x50;    //set P0.4 P0.6 push-pull
	P1MDOUT |= 0x2D;    //set P1.0 P1.2 P1.3 P1.5 push-pull
	P1SKIP  |= 0x10;	//skip P1.4
	P2MDOUT |= 0x0B;    //for sensor spi
	XBR0 = 0x07;  		//enable SPI I/O,CAN I/O,UART I/O
	//XBR1 = 0x02 ;		//SYSCLK output
	XBR2 = 0x40;		//enable crossbar
	SFRPAGE = SfrPage;
}


void Init_All (void)
{
	PortInit();			// Initialize Port I/O
	Sys_ClkInit();			// Initialize Oscillator 
	Comm_HardwareInit();
	Comm_SoftwareInit();
	Timer2_Init();
	Init_SystemPara();
	SPI_init();
	CalibMtInit();
	AD7176_Init();
#ifndef TIMER3_ENABLE
	Timer3_Init();
#endif
	AngInit();
	Int0_Init();
	waiting=0;
	coun=0;
}

//***********************************************************************/

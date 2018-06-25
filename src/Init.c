/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Init.c
* �桡����  v1.0
* ��  ����  100T˫�� ���Ϻ�İ汾
* �����ˣ�   
* ��  �ڣ�   
--------------------------------------------------------------------------
*��ǰ�汾:	v1.0
*�� �� ��:
*�ա�  ��:
----------------------------------�޸�����--------------------------------
1��
2��
3��
4��
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

	CLKSEL = 0X00;				//	ѡ���ڲ�������Ϊϵͳʱ��
	while((OSCICN & BIT4) == 0);//	�ȴ�ʱ���ȶ�
	VDM0CN = 0xA0;				//	Enable VDD monitor and high threshold
	RSTSRC = 0x02;				//	Enable VDD monitor as a reset source

	SFRPAGE = SfrPage_;
}
//***********************************************************************/
// *��    �ܣ� ��ʼ��I/O�ڲ���
// *��    �ڣ� ��
// *��    �ڣ���
// *��    ע��
// *�������ʣ�
//*˵����I/O������Ϊģ��ʱ��PXMIN��Ӧλ����Ϊ0��I/O������Ϊ����ʱ��PXMIN��ӦΪ����Ϊ1��Ĭ��״̬Ϊ1��
//*		 I/O������Ϊ���ʱ��PXMDOUT��Ӧλ����Ϊ1��I/O��Ϊ����PXMDOUT��Ӧλ����Ϊ0��Ĭ��״̬Ϊ0��
//*		 I/O������Ϊģ�⣬ͨ��I/O��ʱ�Լ����⹦��ʱPXSKIP��ӦΪ����Ϊ1��Ĭ��״̬Ϊ0��
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

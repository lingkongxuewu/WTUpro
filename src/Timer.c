/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Time.c
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
#define   __TIMER_C__
#include "_Include.h"

static xdata int8u count;
static xdata int8u count_in;
int16u xdata sum_temp;
int8u  xdata vas;



//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Timer2_Init (void)
{	
	int8u SfrPage_ = SFRPAGE;
	SFRPAGE = ACTIVE_PAGE;	
	TMR2CN  = 0x00;							// Stop Timer2; Clear TF2;	
	CKCON  |= 0x10;							// Select SYSCLK for timer 2 source		0x30
	TMR2RL  = -(SYSCLK/1000);				// Init reload value for 1ms			65536-(SYSCLK/10000)--100us
	TMR2	= 0xFFFF;						// Set to reload immediately
	IP |= 0x40;
	ET2	  	= 1;							// Enable Timer2 interrupts
	TR2	  	= 1;							// Start Timer2
	SFRPAGE = SfrPage_;
	MainTime4Cnt=SendSpeed[SendSpeedNum];
}	


#if TIMER3_ENABLE
void Timer3_Init (void)
{	
	int8u SfrPage_ = SFRPAGE;
	SFRPAGE = ACTIVE_PAGE;	
	TMR3CN  = 0x00;							// Stop Timer3; Clear TF3;	
	CKCON  |= 0x40;							// Select SYSCLK for timer 3 source		
	TMR3RLL = 0x36;							// 50KHz
	TMR3RLH	= 0xF8;//0xE8;
    TMR3H = 0xFF;
    TMR3L = 0xFF;						
	EIE1   |= 0x40;							// Enable Timer3 interrupts
	TMR3CN |= 0x04;							// Start Timer3
	SFRPAGE = SfrPage_;
}

void Timer3_ISR (void) interrupt INTERRUPT_TIMER3
{                      
    TMR3CN &= ~0x80;				
    AD7176_RdRegData(0x04,4);
//	Comm_TimerProc();    
}
#endif


/***********************************************************************/
// *���ܣ�
// *��ڣ���������ʱ�����ڣ���ѯCOMM��ʱ������
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/
#if 1
void Timer2_ISR (void) interrupt 5
{
	TF2H = 0;								// Clear Timer2 interrupt flag
	Timer2_TaskInIsr();
	if(++count>=200)
	{
		vas=SensorSPI_Write(0x08);
		sum_temp+=vas;
		count=0;
		if(++count_in>=10)
		{
			temp_adc.ftval=sum_temp/10;
			count_in=0;
			sum_temp=0;
		}
	}
	if(++coun==2300)
	{
		waiting=1;
	}
}

/***********************************************************************/

#define	MAIN_TIME1_INIT	(1000/100)
#define	MAIN_TIME2_INIT	(1000/8)
#define	MAIN_TIME3_INIT	(1000/2)
#define	MAIN_TIME4_INIT	(1000/1)

static   int8u MainTime4En  = OFF;

/***********************************************************************/
// *���ܣ���ʱ��2 ������������ģʽ��Ƶ�ʺ�ʱ������
// *��ڣ�
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/

void Timer2_TaskInIsr (void)
{				
	Comm_TimerProc();						// CommTimer=10 ��ѯCOMM��ʱ������
	MainTime4Cnt--;	   						//����ʱ������
	if(MainTime4Cnt==0)
	{
		MainTime4Cnt=SendSpeed[SendSpeedNum];  //����ʱ������
		MainTime4En=ON;						   //����ʹ��
		/****** ��������Ƶ��bps����***********/
	}
}
/***********************************************************************/
// *���ܣ���������ģʽ ����XY�ĽǶ�
// *��ڣ�MainTime4En SendActPosi
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/
void Timer2_TaskInMain (void)
{
	if(MainTime4En==ON)
	{	
		MainTime4En=OFF;	
		if(SendActPosi==0)
		Sub_SendAngXY();				   //��������ģʽ ����XY�ĽǶ�
		/******��������***********/
	}
}
#endif



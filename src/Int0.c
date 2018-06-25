


/***********************************************************************/
#define		__INT0_C__
#include "_Include.h"


/*************************************************************************************************/
 #if 1
int8u a[1] = {0X44};
int8u tem = 0;
 
void Int0_Enable(void)					//INT0 input is active low
{
	IE0	= 0;						//clear int0 interrupt flag
    EX0 = 1;						//enable INT0
}

/*************************************************************************************************
* ��������
* ���ܣ�	��ʼ��INT0�ж�
* ��ڣ�
* ���ڣ�
* ��ע��
* �������ʣ�
*************************************************************************************************/

void Int0_Init(void)					
{
	int8u	page_temp;
	page_temp = SFRPAGE;
	SFRPAGE	  = CONFIG_PAGE;
    IT0	= 1;						//INT0 is edge triggered
	IT01CF &= ~0x0E;
	IT01CF |= 0x01;
    SFRPAGE   = page_temp;
	Int0_Enable();
}

/*************************************************************************************************/
void Int0_Disable(void)					//INT0 input is active low
{
    EX0 = 0;							//disable INT0
}

/*************************************************************************************************/

void Int0_ISR (void) interrupt INTERRUPT_INT0
{
	IE0	= 0;
	Int0_Disable();						
	AD7176_IntProc();
	Int0_Enable();
//	NSSMD0 = 0;
}
#endif




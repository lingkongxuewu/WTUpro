
#define   __MAIN_C__
#include "_Include.h"

void main (void)
{
    SFRPAGE = ACTIVE_PAGE;    		// Set for PCA0MD
    PCA0MD &= ~0x40;          		// WDTE = 0 (clear watchdog timer enable)
    Init_All();
	Flash_CalDataInit();
	MenuId=MOD_MOD0;
	delay_ms(250);
	EA = 1;

	 while(1)
	{
	#if AD_DEBUG
//		AD7176_RdRegData(0x04,4);
		Debug_Displayad(0x04,4);
		delay_ms(500);
	#endif
		while(waiting==1) 
		{
			ES0=1;
			waiting=0;
		 }
		AD7176_InMainProc();
		SnProc();
		Timer2_TaskInMain();
		Send_TimeProc();
	}
	
}
//***********************************************************************/
		 
/*--------------------------------文*件*信*息-----------------------------
* 文件名：  AngApp.h
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
#include "_Type.h"
#ifndef __ANGAPP_H__
#define __ANGAPP_H__
	/*-----宏定义---------------------*/
	#define		Alarm_X_Enb			0	 //X轴	报警编译使能
	#define		Alarm_Y_Enb			0	 //Y轴	报警编译使能
	#define		Alarm_P_Enb			0	 //P轴	报警编译使能
	sbit		LED_X 	=	P2^7;
	sbit		LED_Y 	=	P2^6;
	sbit		LED_S 	=	P2^5;
	
	#define		LED_X_ON() 	LED_X=1
	#define		LED_X_OFF() LED_X=0
	#define		LED_Y_ON() 	LED_Y=1
	#define		LED_Y_OFF() LED_Y=0
	#define		LED_S_ON()  LED_S=1
	#define		LED_S_OFF() LED_S=0
	/*-----结构体数据类型定义---------*/
	struct sensor;


	/*-----常量（表格等）声明---------*/
	
	/*-----函数声明-------------------*/
	void AngInit(void);
	
	int32s GetAngP(void);
	int32s GetAngXSysZero(void);
	int32s GetAngYSysZero(void);
	int32s GetAngXUsrZero(void);
	int32s GetAngYUsrZero(void);
	int32s GetAngXInst(void);
	int32s GetAngYInst(void);
	int32s GetAngXSet(void);
	int32s GetAngYSet(void);
	int32s GetAngPSet(void);
	void SetAngXSysZero(int32s zore);
	void SetAngYSysZero(int32s zore);
	void SetAngXUsrZero(int32s zore);
	void SetAngYUsrZero(int32s zore);
	void SetAngXInst(int32s ang_inst);
	void SetAngYInst(int32s ang_inst);
	void SetAngXSet(int32s ang_set);
	void SetAngYSet(int32s ang_set);
	void SetAngPSet(int32s ang_set);
	int32s GetXAngAllSetVal(void);
	int32s GetYAngAllSetVal(void);
	void AngProc(void);
	int32s GetAngXZeroUsr(void);
	int32s GetAngYZeroUsr(void);
//	int32s GetAngXZeroSys(void);		//函数名修改为下面两组
//	int32s GetAngYZeroSys(void);		//函数名修改为下面两组
	int32s GetAngXRelSys(void);
	int32s GetAngYRelSys(void);	
	 
//**************************************************************************
#endif
/***********************************************************************/
#ifdef   __CAL_C__
#define  CAL_EXT
#else
#define	 CAL_EXT  extern
#endif


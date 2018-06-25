/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Main.h
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
#include	"_Type.h"
//***********************************************************************/
#ifndef __MAIN_H__

	#define __MAIN_H__
	/*-----宏定义---------------------*/
	#define		SYSCLK 				24000000		// SYSCLK frequency in Hz
	#define		MOD_MOD0			0
	#define		MOD_X_CAL			1
	#define		MOD_Y_CAL			2
	#define		MOD_T_CAL			3
	#define		TEMP_S				1		//0 = 传感器温度 1 = MCU温度
	
	#define     COM_DEBUG			0	  	//for uart debug
	#define     AD_DEBUG			0			//for adc debug
	#define     TIMER3_ENABLE       0

	sbit P1_4 = P1^4 ;


	/*-----结构体数据类型定义---------*/
	/*-----常量（表格等）声明---------*/	  
	/*-----函数声明-------------------*/
#endif
//***********************************************************************/
#ifdef   __MAIN_C__
#define  MAIN_EXT
#else
#define  MAIN_EXT  extern
#endif
/*-------------变量声明--------------*/
MAIN_EXT	xdata	int8u	MenuId;
MAIN_EXT	xdata	int16u	TempA;

MAIN_EXT	xdata	int16u	AlarmHighLow;
MAIN_EXT    xdata   int8u waiting;
MAIN_EXT	xdata	int16u coun;
//***********************************************************************/

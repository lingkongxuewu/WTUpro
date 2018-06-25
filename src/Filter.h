/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Filter.h
* 版　本：  v1.0
* 描  述：  滤波函数 100T双轴 整合后的版本
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
//***********************************************************************/
#ifndef __FILTER_H__
#define __FILTER_H__
#include	"_Type.h"

	/*-----宏定义---------------------*/
//#define FQUEUE_LONG 9

	/*-----结构体数据类型定义---------*/
	/*
typedef struct
{
	int8u front;
	int8u rear;
	int32 qf_arr[FQUEUE_LONG];
}QF_SILWIN;
*/


	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	int8u ValInRange(int32u ref_val,int32u new_val,int32u range) reentrant;
	int8u ValOutRange(int32u old_val,int32u new_val,int32u range) reentrant;
	int32u SlideWinFilter (int8u *pbufpos, int8u *pbufcnt, int32u *pbuf, int32u val, int16u ftcnt) reentrant;
	float Filter_1st (float ref_val,int32u new_val, int8u filtercnt);
//	void Init_QF(void);		   
	
#endif
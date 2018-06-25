/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Filter.c
* 版　本：  v1.0
* 描  述：  滤波函数   100T双轴 整合后的版本
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
#include "Filter.h"

//***********************************************************************/
// *功    能：限幅滤波
// *入    口：old_val，前一次的值；new_val，新值；range，范围
// *出    口：当新值在幅度内时返回1，否则返回0
// *备    注：幅度为old_val-range<new_val<old_val+range
// *函数性质：
//***********************************************************************/
int8u ValInRange(int32u old_val,int32u new_val,int32u range) reentrant
{
	if((new_val>(old_val+range))||((new_val+range)<old_val))
		return 0;
	else
		return 1;
}

#if 0
int8u ValOutRange(int32u old_val,int32u new_val,int32u range) reentrant
{
	if((new_val>(old_val+range))||((new_val+range)<old_val))
		return 1;
	else
		return 0;
}
#endif
//***********************************************************************/
// *功    能：滑动窗口滤波
// *入    口：pbufpos,缓存位置指针;pbufcnt,缓存中元素个数指针;pbuf,缓存指针;
//						val,新值;ftcnt，窗口长度（元素个数）
// *出    口：当窗口数据填满之后返回平均值，否则返回传入值
// *备    注：保证ftcnt小于等于缓存指针所指向的数组的大小
// *函数性质：
//***********************************************************************/
int32u SlideWinFilter (int8u *pbufpos, int8u *pbufcnt, int32u *pbuf, int32u val, int16u ftcnt) reentrant
{
	int32u data_ram=0;
	int8u i;
	*(pbuf+*pbufpos)=val;    		//替换光标（位置）所在的元素
	(*pbufpos)++;           		//光标后移
	if(ftcnt==*(pbufpos))
		*pbufpos=0;			     	//光标到达窗口末尾，重新回到起始位置
	if((*pbufcnt)!=ftcnt)
	{	
		(*pbufcnt)++;				//窗口数据未满
		return val;
	}
	else 
	{
		for(i=0;i<ftcnt;i++)
		{	
			data_ram+=*(pbuf+i);
		}
		data_ram=data_ram/ftcnt;
		return ((int32u)data_ram);			
	}
}
//***********************************************************************/
// *功    能：一阶滤波
// *入    口：old_val，前一次滤波值；new_val，新值；filtercnt，滤波系数
// *出    口：一阶滤波计算后的值
// *备    注：0<filtercnt<256
// *函数性质：
//***********************************************************************/

float Filter_1st (float old_val,int32u new_val, int8u filtercnt)
{	
	return ((old_val*(filtercnt-1)+new_val)/filtercnt);
}

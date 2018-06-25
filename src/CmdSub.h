/*--------------------------------文*件*信*息-----------------------------
* 文件名：  CmdSub.h
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
#ifndef _CMDSUB_H_
#define _CMDSUB_H_

/***********************************************************************/
	/*-----宏定义---------------------*/
	#define SystemVerNum	16  

	/*-----结构体数据类型定义---------*/
	struct sensor;
	/*-----常量（表格等）声明---------*/
	
	/*-----函数声明-------------------*/
	void Temp_CalValWr (int8u id,int16u temp,int32s x,int32s y);
	
	int8u Temp_CalSta (void);
	int8u Temp_CalStop (void);
	int8u Temp_CalSamp (int8u id,int16u temp,int32s x,int32s y);
	
	int8u Temp_CalTRefSet (int8u id);
	
	int8u Adc_XCalSta (void);
	int8u Adc_XCalProc (int32s ang,int32u adc);
	int8u Adc_XCalStop (void);
	
	int8u Adc_YCalSta (void);
	int8u Adc_YCalProc (int32s ang,int32u adc);
	int8u Adc_YCalStop (void);
	int8u AngChkDiff(int32s ang,int32u adc,int8u num,int32s* angarr,int32u* adcarr);


#endif
	/***************变量声明******************/
#ifdef   __CMDSUB_C__
#define  CMDSUB_EXT
#else
#define	 CMDSUB_EXT  extern
#endif
	CMDSUB_EXT	int8u  xdata	ACalXId;
	CMDSUB_EXT	int8u  xdata	ACalYId;

	CMDSUB_EXT int8u  xdata  SendActPosi;             //SendActPosi=0:主动发送模式，=1：应答模式
	CMDSUB_EXT int8u  xdata  SendSpeedNum;
	CMDSUB_EXT int8u  xdata  SystemVer[SystemVerNum]; //产品编号

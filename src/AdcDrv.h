/*--------------------------------文*件*信*息-----------------------------
* 文件名：  AdcDrv.h
* 版　本：  v1.0
* 描  述：  ADC底层驱动模块	100T双轴 整合后的版本
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
	#ifndef __ADC_DRV_H__
	#define __ADC_DRV_H__
	#include <compiler_defs.h>
	#include <c8051F500_defs.h>

//#include "_Include.h"
	
	

	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	void Adc_Init (void);
	void Adc_Enable (void);	
	int32u Adc_GetXVal(void);
	int32u Adc_GetYVal(void);
//	int16u Adc_GetTemp(void);
//	int16u Adc_GetVdd(void);


/***********************************************************************/
#ifdef   __ADC_DRV_C__
#define  ADC_DRV_EXT
#else
#define  ADC_DRV_EXT  extern
#endif
ADC_DRV_EXT int8u idata	AdcEndTag;
ADC_DRV_EXT int8u idata	AdcEndId;
#endif

	

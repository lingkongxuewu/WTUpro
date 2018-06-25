/*--------------------------------文*件*信*息-----------------------------
* 文件名：  
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
#ifndef __CALMCUT_H__
#define __CALMCUT_H__

/***********************************************************************/
/*-----------------------------宏定义----------------------------------*/

	struct mcut_calib{
		int8u calcnt;    				            //温度校准点个数
		int16u *ptempadc;    			            //温度 AD值缓存
		int16s mcu_temp[TEMP_CAL_NUM];              //温度缓存数组
		float prm_a[TEMP_CAL_NUM-1];                //参数A缓存数组
		float prm_k[TEMP_CAL_NUM-1];                //参数K缓存数组
		void (*SetCalNum)(int8u num);               //设置校准个数
		int16s (*RdTempVal)( int8u id);             //读设置数据
		int16u (*RdAdcVal)(int8u id);               //读数据
		void (*WrTempVal)(int16s val, int8u id);    //写数据
		void (*WrAdcVal)(int16u val, int8u id);     //写返回数据
//		void (*CalcPrm)(void);                      //计算参数A,K
	}; 




		
/*----------------常量（表格等）声明---------*/
		#define	MTEMP_CAL_NUM 	10	

/*----------------外部函数声明-------------------*/
		
    	void  CalibMtInit(void);									//MCU温度校准结构体初始化
    	void  SetMcuTCalCnt(int8u);									//设置MCU温度校准点数
		int8u GetMcuTCalCnt();
		int8u McuT_CalSta (void);									//MCU温度校准开始，
		int8u McuT_CalStop (void);									//MCU温度校准结束，算K A
		int8u Mcut_CalSamp (int8u id,int16s temp,int16u tadc);		//MCU温度校准数据采集
		void MTCalDataInit();		 								//MCU的温度校准数据初始化
		float GetMt(int16u tadc);	 								//获取MCU的温度值
		int16s GetMcuTCalTempVal(int8u pos);						//获取MCU校准设定的温度值
		int16u GetMcuTCalAdcVal(int8u pos);							//获取MCU校准的温度AD值
		void SetMcuTCalTempVal(int16s val, int8u pos);
		void SetMcuTCalAdcVal(int16u val, int8u pos);
#endif

#ifdef   __CALMCUT_C__
#define  CALMCUT_EXT
#else
#define	 CALMCUT_EXT  extern
#endif

CALMCUT_EXT struct mcut_calib xdata McuTcal;	 //hxl mcu温度校准


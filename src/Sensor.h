/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Sensor.h
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
#ifndef __SENSOR_H__
#define __SENSOR_H__


/***********************************************************************/

	/*-----宏定义---------------------*/

	/*-----结构体数据类型定义---------*/
	
	struct sensor{
		int32u val;									//原始值（窗口滤波后）
		float  ftval_f;								//滤波之后的值(中间变量，仅用于一阶滤波，Filter_1st)
		int32u ftval;								//滤波之后的AD值
		int32s oftval;   							//温补之后的AD值
		int32s ang;   								//角度(计算温补后）
		struct temp_calib *tempcal;				    //指向temp_calib结构的指针
		struct ang_calib *angcal;					//指向ang_calib结构的指针
		void (*DataInit)();						    //函数指针
		void (*Filter)(int32u val, int8u ftcnt);	//指向滤波函数
		int32s (*GetTempOffset)(int16u temp);		// 指向获取温补的角度值函数
		void (*AngCalc)();							//函数指针指向角度计算函数		 
	};

	/*-----常量（表格等）声明---------*/
	
	/*-----函数声明-------------------*/
	void SnInit(void);    //
	
	void SnProc(void);    //
	
	int8u GetFilterCnt(void);
	void SetFilterCnt(int8u cnt);
	
	int16u Get_McuTemp(void);
	
	int32u GetXFtVal(void);    //一阶滤波后的值
	int32u GetYFtVal(void);    //
	
	int32s GetXOftVal(void);   //温补之后的值
	int32s GetYOftVal(void);
	
	int32s GetXAng(void);     //角度值
	int32s GetYAng(void);


#endif
/***********************************************************************/
#ifdef   __SENSOR_C__
#define  SENSOR_EXT
#else
#define	 SENSOR_EXT  extern
#endif
SENSOR_EXT struct sensor xdata accel_x;		  //加速度传感器X
SENSOR_EXT struct sensor xdata accel_y;		  //加速度传感器Y
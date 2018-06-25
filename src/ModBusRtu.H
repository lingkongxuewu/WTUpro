/*--------------------------------文*件*信*息-----------------------------
* 文件名：  ModBusRtu.h
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
/***********************************************************************/
#ifndef __MODBUSRTU_H__
	#define __MODBUSRTU_H__
	#define MODBUS_CHECK_ENB	1
	#define	MAXSIZE		32
/*******命令字定义*************/
	#define READ		0x03	//读  
	#define	WRITE		0x10	//写
	
	#define	ANGX_ADDR				0x00		//r
	#define ANGY_ADDR				0x01		//r
	#define	ANGP_ADDR				0x02		//r
	#define	ANGX_OFFSET_ADDR		0x03		//r/w
	#define	ANGY_OFFSET_ADDR		0x04		//r/w	
	#define	ANGX_ALARM_ADDR			0x05		//r/w
	#define	ANGY_ALARM_ADDR			0x06		//r/w
	#define ANGP_ALARM_ADDR			0x07		//r/w
	#define SELF_ADDR				0x08		//r/w
	#define BAUD_ADDR				0x09		//r/w
	#define	ZERO_ADDR				0x0A		//w
//	#define RST_ADDR				0x0B		//w
//	#define FLASH
	
	#define	MR_READ_REGH(temp)	temp>>8
	#define MR_READ_REGL(temp)	temp
	#define	MR_WRITE_REGH(temp)	temp>>8
	#define MR_WRITE_REGL(temp)	temp
//	#define MR_READ_ADDRH()
	

/*********函数声明*********************/	
void ModRtu_RxProc (int8u* buf, int8u n);	//对外接口函数
#endif

/*-----------------------------------*/
#ifdef   __MODBUSRTU_C__
#define  MODBUSRTU_EXT
#else
#define	 MODBUSRTU_EXT  extern
#endif
/*-------------变量声明--------------*/
MODBUSRTU_EXT	xdata	int8u 	Addr;			//本机通讯地址
MODBUSRTU_EXT	int32u 	xdata	AngRange;		//角度测量范围 5 10 15 30
MODBUSRTU_EXT	int32s xdata  	AngAlarmOffset;
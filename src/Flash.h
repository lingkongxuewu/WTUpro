/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Flash.h
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
#ifndef __FLASH_H__

	#define __FLASH_H__
	/*-----宏定义---------------------*/
//	#define		FLASH_DATA_CHK		0x85	



//	#define FLASH_SYS       0x7800L            // For 32K Flash devices	
//	#define FLASH_USER      0x7A00L            // For 32K Flash devices
//	#define	FLASH_TEMP      0x7C00L			   // For 32K Flash devices
	#define FLASH_SYS       0xE800L            // For 64K Flash devices	
	#define FLASH_USER      0xEA00L            // For 64K Flash devices
	#define	FLASH_TEMP      0xEC00L				// For 64K Flash devices
	
	
	#define	FLASH_DATA_CHK	0x85			   //校验字
// 	#define	NUM_PAGE0     	0x200// 1D0//          //Flash page=0 CheckSum 的累加次数
//	#define	NUM_PAGE1     	0x200// 1fc//          //Flash page=1 CheckSum 	累加次数
	#define	FLASH_BUF_NUM   0x200//         	//FLASH BUF 数



	/*-----结构体数据类型定义---------*/
	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	void Flash_CalDataInit (void);
	void Flash_CalDataUpdate (void);
	void Flash_UserDataUpdate(void);
#endif

//***********************************************************************/
#ifdef   __FLASH_C__
#define  FLASH_EXT
#else
#define  FLASH_EXT  extern
#endif
/*-------------变量声明--------------*/
FLASH_EXT	xdata	char	DispBuf[FLASH_BUF_NUM];
//***********************************************************************/
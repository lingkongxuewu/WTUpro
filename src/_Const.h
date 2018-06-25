/*--------------------------------文*件*信*息-----------------------------
* 文件名：  _Const.h
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
    #ifndef _CONST_H_

	#define _CONST_H_
	
	#define BIT0 0x01
	#define BIT1 0x02
	#define BIT2 0x04
	#define BIT3 0x08
	#define BIT4 0x10
	#define BIT5 0x20
	#define BIT6 0x40
	#define BIT7 0x80
	#define BIT8 0x0100
	#define BIT9 0x0200
	#define BIT10 0x0400
	#define BIT11 0x0800
	#define BIT12 0x1000
	#define BIT13 0x2000
	#define BIT14 0x4000
	#define BIT15 0x8000
	
	#define	DATE_POS	//xdata
	#define	PI 3.1415926
	
	#define TRUE 1
	#define FALSE 0
	
	#define ON TRUE
	#define OFF FALSE
	#define OK TRUE
	#define ERR FALSE
	#define ESC FALSE
	#define CYC TRUE
	#define END FALSE
	#define T24H TRUE
	#define T12H FALSE
	
	#define CONST_GET_HIGH_BYTE(temp) (char)(temp>>8)
	#define CONST_GET_LOW_BYTE(temp)  (char)(temp)
		
	#define PORT_SET_DIRECT(_port,bit)		((_port)->DIR=bit)
	#define PORT_SET_OUTPUT(_port,bit)		((_port)->DIRSET=bit)
	#define PORT_SET_INPUT(_port,bit)		((_port)->DIRCLR=bit)
	
	#define PORT_OUTPUT_VAL(_port,val)		((_port)->OUT=val)
	#define PORT_BIT_SET(_port,bit)			((_port)->OUTSET=bit)
	#define PORT_BIT_CLR(_port,bit)			((_port)->OUTCLR=bit)


#endif

#ifndef NUL
	#define NUL 0x00
#endif
/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Sub.h
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
#ifndef __SUB_H__

	#define __SUB_H__
	/*-----宏定义---------------------*/
	/*-----结构体数据类型定义---------*/
	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/

	void Comm_PutInt32u (int32u val);	
	void Comm_PutInt32s (int32s val);
	void Comm_PutInt16u (int16u val);
	void Comm_PutInt16s (int16s val);
	void Comm_PutInt8u (int8u val);
	int8s Sub_Str2Int8s (int8u* str);
	
	void Sub_SysRst (void);
	void Sub_BufBubble (int32s* p1,int32u* p2,int8u n);
	void Sub_BufBubble16s(int16s* id,int16u* x,int8u n);
//	void Sub_AngCalValGet (float* a,float* k,int16u adc,int8u tag);
//	void Sub_AngCalValCnt (int16u adc0,int16u adc1,int32s ang0,int32s ang1,float* a,float* k);	 //HXL 12/8/28
	
	int8u Sub_Str2Int32sAng (int32s *val,int8u* str);
	int8u Sub_Str2Int8u (int8u* str);
	int8u Sub_Str2Int32s (int32s *outdata,int8u* str);
//	int8u Sub_Str2Int16s (int16s *outdata,int8u* str);
	int8u Sub_Str2Int16u (int16u *temp,int8u* str);
	void  Sub_Int32s2Str (char* s,int32s ang);
	void Sub_Int32u2Str (char* s,int32u val);
	void  Sub_Int16u2Str (char* s,int16u adc);
	void  Sub_Int16s2Str (char* s,int16s temp);
	void  Sub_Int8u2Str (char* s,int8u val);
	
	void Sub_SendOk (void);
	void Sub_SendErr (void);
	void Sub_SendAngXY (void);
	void Sub_SendAngXYP (void);
	void Sub_SendAngXCal (int8u i);
	void Sub_SendAngYCal (int8u i);
	void Sub_SendTempCal (int8u i);
	void Sub_SendID (void);
	void Sub_SendAdcTXY (int16u temp,int32s x,int32s y);
	void Sub_SendSoftVer (void);
	void Sub_SendSysNum (void);
	void Sub_SendTCalTest (void);
	void Sub_SendOffset (int32s val);
	void Sub_SendPrmtAll(void);
	void Sub_BufBubble2 (int16u* id,int32s* x,int32s* y,int8u n);	  
	int8u Sub_StralarmCheck(int32s *val,int8u* str);
	void Comm_PutNewLine(void);
	
	void Sub_SendMcuT(int16s temp,int16u tadc);		
#endif
/*-----------------------------------*/
#ifdef   __SUB_C__
#define  SUB_EXT
#else
#define	 SUB_EXT  extern
#endif
/*-------------变量声明--------------*/



/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Sub.c
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
#define   __SUB_C__
#include  "_Include.h"
//***********************************************************************/
void Sub_16SBufSwitch (int16s* buf,int8u i,int8u j);
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：32有符号位角度转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt32sAd (int32s val)
{		
	if(val<0)
	{	val=-val;
		DispBuf[0]='-';
	}
	else
	{	DispBuf[0]='+';
	}
	Sub_Int32u2Str(&DispBuf[1],val);
	Comm_PutStr((int8u*)DispBuf,9);	// 从DisBuf[]将XY的角度值存到发送缓冲区
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：32有符号位转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt32s (int32s val)
{
	Sub_Int32s2Str(DispBuf,val);	// 将角度值转BCD ，存于DisBuf[]
	Comm_PutStr((int8u*)DispBuf,8);	// 从DisBuf[]将XY的角度值存到发送缓冲区
}
//***********************************************************************/
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：32无符号位转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt32u (int32u val)
{
	Sub_Int32u2Str(DispBuf,val);
	Comm_PutStr((int8u*)(DispBuf+2),6);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：16有符号位转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt16s (int16s val)
{
	Sub_Int16s2Str(DispBuf,val);
	Comm_PutStr((int8u*)DispBuf,6);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：16无符号位转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt16u (int16u val)
{
	Sub_Int16u2Str(DispBuf,val);
	Comm_PutStr((int8u*)DispBuf,5);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：8位无符号位转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt8u (int8u val)
{
	Sub_Int8u2Str(DispBuf,val);
	Comm_PutStr((int8u*)DispBuf,3);
}
//***********************************************************************/
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：将回车换行的ASCII值存TX缓冲区
//***********************************************************************/
void Comm_PutNewLine (void)
{
	Comm_PutChar(0x0D);
	Comm_PutChar(0x0A);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：将回车换行的ASCII值存TX缓冲区
//***********************************************************************/
void Comm_PutNewLineSta (void)
{
	Comm_PutChar(0x0D);
	Comm_PutChar(0x0A);
	Comm_SendSta();
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：Send "ok"
//***********************************************************************/
void Sub_SendOk (void)
{	
	char* str={",OK"};
	Comm_PutStr((int8u*)str,3);
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendErr (void)
{	char* str={"ERR"};
	
	
	Comm_PutStr((int8u*)str,3);
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendAngXY (void)
{
	Comm_PutStr("X=",2);						 //将字符存入发生缓冲区RingBufTx[COMM_TX_BUF_SIZE]
	Comm_PutInt32s(GetXAngAllSetVal());			//(温补后的角度+系统、用户、安装角后的角度）转BCD
	Comm_PutStr("  ",2);
//	Comm_PutNewLine();							//回车换行
	Comm_PutStr("Y=",2);
	Comm_PutInt32s(GetYAngAllSetVal());			//

	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendAngXYP (void)
{	
	Comm_PutStr("X=",2);
	Comm_PutInt32s(GetXAngAllSetVal());	
	Comm_PutStr("  ",2);	
//	Comm_PutNewLine();
	Comm_PutStr("Y=",2);
	Comm_PutInt32s(GetYAngAllSetVal());	
	Comm_PutStr("  ",2);	
//	Comm_PutNewLine();
	Comm_PutStr("P=",2);
	Comm_PutInt32s(GetAngP());
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendAdcTXY (int16u temp,int32s x,int32s y)
{	
	Comm_PutStr("t,",2);	Comm_PutInt16u(temp);
	Comm_PutStr(",x,",3);	Comm_PutInt32u(x);
	Comm_PutStr(",y,",3);	Comm_PutInt32u(y);
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendMcuT(int16s temp,int16u tadc)
{	
	Comm_PutStr("mt,",3);	Comm_PutInt16s(temp);
	Comm_PutStr(",adc,",5);	Comm_PutInt16u(tadc);
	Comm_PutNewLineSta();
} 
//***********************************************************************/
void Sub_SendPrmtAll(void)
{
	Comm_PutStr("ST,",3);	Comm_PutInt8u(Get_McuTemp());
	
	Comm_PutStr(",x,",3);	
							Comm_PutInt32u(GetXFtVal());		//	滤波后的值 
	Comm_PutChar(',');		
							Comm_PutInt32u(GetXOftVal());		//Comm_PutInt32u(Adc_GetXVal());测试用  
	Comm_PutChar(',');		
							Comm_PutInt32s(GetXAng());				   

	Comm_PutStr(",y,",3);	
							Comm_PutInt32u(GetYFtVal());		//
	Comm_PutChar(',');		
							Comm_PutInt32u(GetYOftVal());		//Comm_PutInt32u(Adc_GetYVal());测试用		  
	Comm_PutChar(',');		
							Comm_PutInt32s(GetYAng());
							
	Comm_PutChar(',');		
							Comm_PutInt8u(GetAngXCalCnt());
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendAngXCal (int8u i)
{	
	Comm_PutStr("X,",2);	Comm_PutInt8u(i);
	Comm_PutStr(",",1);	   Comm_PutInt32u(GetAngXCalAdcVal(i+0));		 

	Comm_PutStr(",K,",3);	Comm_PutInt16s(100000000*Ang_Xcal.prm_k[i+0]); 
	Comm_PutStr(",A,",3);	Comm_PutInt16s(100000000*Ang_Xcal.prm_a[i+0]); 
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendAngYCal (int8u i)
{	
	Comm_PutStr("Y,",2);	Comm_PutInt8u(i);
	Comm_PutChar(',');		Comm_PutInt32u(GetAngYCalAdcVal(i+0));        
	Comm_PutStr(",K,",3);	Comm_PutInt16s(100000000*Ang_Ycal.prm_a[i+0]); 
	Comm_PutStr(",A,",3);	Comm_PutInt16s(100000000*Ang_Ycal.prm_a[i+0]); 

	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendTempCal (int8u i)
{	
	Comm_PutStr("t,",2);	Comm_PutInt8u(i);
	Comm_PutChar(',');		Comm_PutInt16u(GetTempXCalTempVal(i+0)); 
	
	Comm_PutStr(",x,",3);	Comm_PutInt32sAd(GetTempXCalAdcVal(i+0));
	Comm_PutStr(",y,",3);	Comm_PutInt32sAd(GetTempYCalAdcVal(i+0));	  

	Comm_PutStr(",XK,",4);	Comm_PutInt32sAd(1000*Temp_Xcal.prm_k[i+0]);	//Comm_PutInt16s(1000*Temp_Xcal.prm_k[i+0]);
	Comm_PutStr(",A,",3);	Comm_PutInt32sAd(Temp_Xcal.prm_a[i+0]);
	Comm_PutStr(",YK,",4);	Comm_PutInt32sAd(1000*Temp_Ycal.prm_k[i+0]);		//Comm_PutInt16s(1000*Temp_Ycal.prm_k[i+0]);
	Comm_PutStr(",A,",3);	Comm_PutInt32sAd(Temp_Ycal.prm_a[i+0]);

	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendOffset (int32s val)
{	
	Comm_PutInt32s(val);
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendID (void)
{
	Comm_PutInt16u(MenuId);			Comm_PutChar(',');
	Comm_PutInt16u(GetAngXCalCnt());Comm_PutChar(',');	  		//HXL	整合更新 12/8/26 ？
	Comm_PutInt16u(ACalXId);		Comm_PutChar(',');			//?
	Comm_PutInt16u(ACalYId);		Comm_PutChar(',');			//?
	Comm_PutInt16u(GetTempXCalCnt());							//HXL	整合更新 12/8/26 
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendSoftVer (void)
{		
	Comm_PutStr("224,0114,20171124",17);	
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendSysNum (void)
{			
	Comm_PutStr("sn,",3);
	Comm_PutStr(SystemVer,SystemVerNum);	
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendTCalTest (void)
{	
	int32s	xdata TempX;
	int32s	xdata TempY;

	TempX = accel_x.GetTempOffset(Get_McuTemp());	//温度补偿
	TempY = accel_y.GetTempOffset(Get_McuTemp());	//温度补偿	  HXL 12/8/27

	Comm_PutInt16u(TempA);	Comm_PutChar(',');
	Comm_PutInt32sAd(TempX);	Comm_PutChar(',');
	Comm_PutInt32sAd(TempY);
	Comm_PutNewLine();
	Comm_SendSta();
	
	TempA++;
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u Sub_Str2Int32sAng (int32s *val,int8u* str)
{	
	int32s xdata temp;
	
	if((str[1]<'0')||(str[1]>'9'))return FALSE;
	if((str[2]<'0')||(str[2]>'9'))return FALSE;
	if(str[3]!='.')				  return FALSE;
	if((str[4]<'0')||(str[4]>'9'))return FALSE;
	if((str[5]<'0')||(str[5]>'9'))return FALSE;
	if((str[6]<'0')||(str[6]>'9'))return FALSE;
	if((str[7]<'0')||(str[7]>'9'))return FALSE;
	temp=(int32s)(str[1]-'0')*100000+(int32s)(str[2]-'0')*10000+(str[4]-'0')*1000+(str[5]-'0')*100+(str[6]-'0')*10+(str[7]-'0'); //yyc17-12-13修改str[2]有可能越界的问题
	if(str[0]=='+')
	{	*val = temp;
		return TRUE;
	}
	else if(str[0]=='-')
	{	*val = -temp;
		return TRUE;
	}
	return FALSE;
}
//***********************************************************************/
int8u Sub_Str2Int32s (int32s *val,int8u* str)
{	int32u xdata temp;
	int8u xdata i;


	for(i=1;i<9;i++)
	{	if((str[i]<'0')||(str[i]>'9'))
		return FALSE;
	}
//	temp=(str[1]-'0')*10000000+(str[2]-'0')*1000000+(str[3]-'0')*100000+(int32u)(str[4]-'0')*10000+(int32u)(str[5]-'0')*1000
//		 +(int32u)(str[6]-'0')*100+(int32u)(str[7]-'0')*10+(int32u)(str[8]-'0');
		 
	temp=((str[1]-'0')*1000+(str[2]-'0')*100+(str[3]-'0')*10+(str[4]-'0'))*(int32u)10000+
	      (str[5]-'0')*1000+(str[6]-'0')*100+(str[7]-'0')*10+(str[8]-'0');
		 
		 
		 
//测试用
//	temp = (str[1]-'0')*10000000;
//	temp +=(str[2]-'0')*1000000;
//	temp +=(str[3]-'0')*100000;
//	temp +=(str[4]-'0')*10000;
//	temp +=(str[5]-'0');
//	temp +=(str[6]-'0');
//	temp +=(str[7]-'0');
//	temp +=(str[8]-'0');	
//测试结束		 
	
	if(str[0]=='+')
	{	*val = temp;
		return TRUE;
	}
	else if(str[0]=='-')
	{	*val = -temp;
		return TRUE;
	}
	return FALSE;
}
//***********************************************************************/
//int8u Sub_Str2Int16s (int16s *val,int8u* str)
//{	int16u temp;
//	if(FALSE==Sub_Str2Int16u(&temp,&str[1]))return (FALSE);
//	
//	if(str[0]=='+')
//	{	*val = temp;
//		return TRUE;
//	}
//	else if(str[0]=='-')
//	{	*val = -temp;
//		return TRUE;
//	}
//	return FALSE;
//}
//***********************************************************************/
int8u Sub_Str2Int16u (int16u *val,int8u* str)
{	int8u xdata i;
	
	
	for(i=0;i<5;i++)
	{	if((str[i]<'0')||(str[i]>'9'))
		return FALSE;
	}
	*val=(str[0]-'0')*10000+(str[1]-'0')*1000+(str[2]-'0')*100+(str[3]-'0')*10+(str[4]-'0');
	return TRUE;
}
//***********************************************************************/
int8u Sub_Str2Int8u (int8u* str)
{	int8u xdata val;	
	
	if((str[0]<'0')||(str[0]>'9'))return(100);
	if((str[1]<'0')||(str[1]>'9'))return(100);
	val=(str[0]-'0')*10+(str[1]-'0');
	return  val;
}
//***********************************************************************/
int8s Sub_Str2Int8s (int8u* str)
{
	int8s xdata temp;
	temp = Sub_Str2Int8u(&str[1]);
	
	if(str[0]=='-')
	{	
		temp = -temp;
	}
	return temp;
}
//***********************************************************************/
void Sub_Int32s2Str (char* s,int32s val)
{
	if(val<0)
	{	
		val=-val;
		s[0]='-';
	}
	else
	{	
		s[0]='+';
	}
	if(val>999999)val-=1000000;
	
	s[1] = val/100000+'0';
	val  = val%100000;
	s[2] = val/10000+'0';
	val  = val%10000;
	s[3] = '.';
	s[4] = val/1000+'0';
	val  = val%1000;	
	s[5] = val/100+'0';
	val  = val%100;
	s[6] = val/10+'0';
	s[7] = val%10+'0';
}
//***********************************************************************/
//void Sub_Int32u2Str (char* s,int32u val,int8u num)
void Sub_Int32u2Str (char* s,int32u val)
{	
	if(val>16777215)val = 16777215; 	//数值溢出2013-7-15 11:56AM修改增加
	s[0] = val/10000000+'0';
	val  = val%10000000;
	s[1] = val/1000000+'0';
	val  = val%1000000;	
	s[2] = val/100000+'0';
	val  = val%100000;	
	s[3] = val/10000+'0';
	val  = val%10000;
	s[4] = val/1000+'0';
	val  = val%1000;
	s[5] = val/100+'0';
	val  = val%100;
	s[6] = val/10+'0';
	s[7] = val%10+'0';
}
//***********************************************************************/
void Sub_Int16s2Str (char* s,int16s val)
{
	if(val<0)
	{	val=-val;
		s[0]='-';
	}
	else
	{	s[0]='+';
	}
	Sub_Int16u2Str(&s[1],val);
}
//***********************************************************************/
void Sub_Int16u2Str (char* s,int16u val)
{
	s[0] = val/10000+'0';
	val  = val%10000;
	s[1] = val/1000+'0';
	val  = val%1000;
	s[2] = val/100+'0';
	val  = val%100;
	s[3] = val/10+'0';
	s[4] = val%10+'0';
}
//***********************************************************************/
void Sub_Int8u2Str (char* s,int8u val)
{
	s[0] = val/100+'0';
	val  = val%100;
	s[1] = val/10+'0';
	s[2] = val%10+'0';
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
/*
void Sub_TVCalValCnt (int16u temp0,int16u temp1,int16s adc0,int16s adc1,float* a,float* k)
{
	*k = ((float)adc1-adc0)/((float)temp1-temp0);
	*a = (float)adc0- *k * temp0;
}
*/
//***********************************************************************/
/*
void Sub_AngCalValCnt (int16u adc0,int16u adc1,int32s ang0,int32s ang1,float* a,float* k)
{	float _ang0=(PI*ang0)/180000.0;
	float _ang1=(PI*ang1)/180000.0;
	
	*k = (sin(_ang1)-sin(_ang0))/(int16s)(adc1-adc0);
	*a = sin(_ang0) - *k * adc0;
}
*/   //HXL 12/8/28
//***********************************************************************/



/*
void Sub_AngCalValGet (float* a,float* k,int16u adc,int8u tag)
{	int8u i;
	
	
	if(tag==0)
	{	//for(i=0;i<ACalN-2;i++)				//HXL 12/8/26  原來的
		for(i=0;i<(GetAngXCalCnt()-2);i++)		//HXL 12/8/26  整合更新
		{
//			if(adc<ACalXAdc[i+1])break;		
			if(adc<GetAngXCalAdcVal(i+1))break;	//HXL 12/8/27  整合
			
			更新
		}
//		*a=APrmtXA[i];	
//		*k=APrmtXK[i];
		*a=Ang_Xcal.prm_a[i]; //HXL 		12/8/27
		*k=Ang_Xcal.prm_k[i]; //HXL 		12/8/27
	}
	else
	{	//for(i=0;i<ACalN-2;i++)
		for(i=0;i<(GetAngYCalCnt()-2);i++)		//HXL 12/8/26  整合更新
		{
//			if(adc<ACalYAdc[i+1])break;
			if(adc<GetAngYCalAdcVal(i+1))break;	//HXL 12/8/27  整合更新	
		}
//		*a=APrmtYA[i];	
//		*k=APrmtYK[i];
		*a=Ang_Ycal.prm_a[i]; //HXL 		12/8/27
		*k=Ang_Ycal.prm_k[i]; //HXL 		12/8/27
				
	}
}
*/
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Sub_SBufSwitch (int32s* buf,int8u i,int8u j)
{	int32s xdata temp;
	
	
	temp=buf[i];
	buf[i]=buf[j];
	buf[j]=temp;
}
//***********************************************************************/

void Sub_32uBufSwitch (int32u* buf,int8u i,int8u j)
{	int32u xdata temp;
	
	
	temp=buf[i];
	buf[i]=buf[j];
	buf[j]=temp;
}
 //HXL 12/8/28 2013-6-13 11:28AM孙娜修改增加
//***********************************************************************/

void Sub_16SBufSwitch (int16s* buf,int8u i,int8u j)
{	int16s xdata temp;
	
	
	temp=buf[i];
	buf[i]=buf[j];
	buf[j]=temp;
}
 //HXL 12/8/28 
//***********************************************************************/
void Sub_UBufSwitch (int16u* buf,int8u i,int8u j)
{	int16u xdata temp;
	
	
	temp=buf[i];
	buf[i]=buf[j];
	buf[j]=temp;
}
//***********************************************************************/
void Sub_BufBubble (int32s* id,int32u* x,int8u n)
{	int8u xdata  i,j;
	
	
	for(i=0;i<n;i++)
	{	for(j=n-1;j>i;j--)
		{	if(id[i]>id[j])
			{	Sub_SBufSwitch(id,i,j);
				Sub_32uBufSwitch(x,i,j);
			}
		}
	}
}
//***********************************************************************/
void Sub_BufBubble2 (int16u* id,int32s* x,int32s* y,int8u n)
//void Sub_BufBubble2 (int16u* id,int32s* x,int8u n)
{	int8u xdata i,j;


	for(i=0;i<n;i++)
	{	for(j=n-1;j>i;j--)
		{	if(id[i]>id[j])
			{	Sub_UBufSwitch(id,i,j);
				Sub_SBufSwitch(x,i,j);
				Sub_SBufSwitch(y,i,j);
			}
		}
	}
}
  //HXL 12/8/28
//***********************************************************************/
void Sub_BufBubble16s(int16s* id,int16u* x,int8u n)
{	int8u xdata  i,j;
	
	
	for(i=0;i<n;i++)
	{	for(j=n-1;j>i;j--)
		{	if(id[i]>id[j])
			{	Sub_16SBufSwitch(id,i,j);
				Sub_UBufSwitch(x,i,j);
			}
		}
	}
}  
//***********************************************************************/
void Sub_SysRst (void)
{	int8u SfrPage_ = SFRPAGE;
	
	
	SFRPAGE = ACTIVE_PAGE;
	RSTSRC|=0x10;
	SFRPAGE = SfrPage_;
}
//***********************************************************************/
int8u Sub_StralarmCheck(int32s *val,int8u* str)
{
	if((str[0]=='H')||(str[0]=='L'))
	{
		*val=str[0];
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

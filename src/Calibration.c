/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Calibration.c
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
#define __CALIBR_C__
#include "_Include.h"
//struct temp_calib xdata Temp_Xcal;    //X轴温度补偿
//struct temp_calib xdata Temp_Ycal;    //Y轴温度补偿

//struct ang_calib xdata Ang_Xcal;    //X轴角度校准
//struct ang_calib xdata Ang_Ycal;    //Y轴角度校准

static int16u xdata temp_buf[TEMP_CAL_NUM];    //21

static float    Ang_RdXAVal(struct ang_calib *p_cal, int8u posid);

//***********************************************************************/
// *功    能：设置温度补偿点个数
// *入    口：p_cal,temp_calib结构体指针(X,Y);num,点数
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Temp_SetCalCnt(struct temp_calib *p_cal, int8u num)
{
    p_cal->calcnt=num;
}
//***********************************************************************/
// *功    能：读温度校准模块中温度数组的某个元素
// *入    口：p_cal,temp_calib结构体指针(X,Y);posid,位置ID号
// *出    口：温度值
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static int16u   Temp_RdTempVal(struct temp_calib *p_cal, int8u posid)
{
    return (p_cal->ptempval[posid]);
}

//***********************************************************************/
// *功    能：读温度校准模块中ADC数组的某个元素
// *入    口：p_cal,temp_calib结构体指针(X,Y);posid,位置ID号
// *出    口：角度0时的ADC值
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static int32s   Temp_RdAdcVal(struct temp_calib *p_cal, int8u posid)
{
    return (p_cal->adcarr[posid]);
}

//***********************************************************************/
// *功    能：设置温度校准模块中温度数组的某个元素
// *入    口：p_cal,temp_calib结构体指针(X,Y);val,温度AD值;posid,位置ID号
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static void Temp_WrTempVal(struct temp_calib *p_cal,int16u val, int8u posid)
{
    p_cal->ptempval[posid]=val;
}

//***********************************************************************/
// *功    能：设置温度校准模块中ADC数组的某个元素
// *入    口：p_cal,temp_calib结构体指针(X,Y);val,写入的角度0时的AD值;posid,位置ID号
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static void Temp_WrAdcVal(struct temp_calib *p_cal,int32s val, int8u posid)
{
    p_cal->adcarr[posid]=val;
}

//***********************************************************************/
// *功    能：0角度温度漂移A,K参数计算//
// *入    口：p_cal,temp_calib结构体指针(X,Y);
// *出    口：
// *备    注：K=(ADC1-ADC0)/(TEMP1-TEMP0);A=ADC0-K*TEMP0;
// *函数性质：在角度为0，以某一个温度为参考点的前提下，角度AD值随温度变化的关系来计算其中的ad值
//***********************************************************************/
static void Temp_CalcPrm(struct temp_calib *p_cal)
{
    int8u xdata i;
    for(i=0;i<(p_cal->calcnt-1);i++)    						//计算所有的A,K值
    {
        p_cal->prm_k[i]=((float)(p_cal->adcarr[i+1]-p_cal->adcarr[i])/(float)(p_cal->ptempval[i+1]-p_cal->ptempval[i]));
        p_cal->prm_a[i] = (float)(p_cal->adcarr[i])-p_cal->prm_k[i] * (p_cal->ptempval[i]);
    }
}

//***********************************************************************/
// *功    能：设置角度标定点个数
// *入    口：p_cal,ang_calib结构体指针(X,Y);num,点数
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Ang_SetCalCnt(struct ang_calib *p_cal, int8u num)
{
    p_cal->calcnt=num;
}
//***********************************************************************/
// *功    能：读取角度标定模块中的角度值数组中的某个元素
// *入    口：p_cal,ang_calib结构体指针(X,Y);posid,位置ID号
// *出    口：角度值
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static int32s   Ang_RdAngVal(struct ang_calib *p_cal, int8u posid)
{
    return (p_cal->angarr[posid]);
}

//***********************************************************************/
// *功    能：读取角度标定模块中的角度AD值数组中的某个元素
// *入    口：p_cal,ang_calib结构体指针(X,Y);posid,位置ID号
// *出    口：角度ADC值
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static int32u   Ang_RdAdcVal(struct ang_calib *p_cal, int8u posid)
{
    return (p_cal->adcarr[posid]);
}
//***********************************************************************/
// *功    能：设置角度标定模块中的角度值数组中的某个元素
// *入    口：p_cal,ang_calib结构体指针(X,Y);val,角度值;posid,位置ID号
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static void Ang_WrAngVal(struct ang_calib *p_cal,int32s val, int8u posid)
{
    p_cal->angarr[posid]=val;
}

//***********************************************************************/
// *功    能：设置角度标定模块中的角度AD值数组中的某个元素
// *入    口：p_cal,ang_calib结构体指针(X,Y);val,角度ADC值;posid,位置ID号
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static void Ang_WrAdcVal(struct ang_calib *p_cal,int32u val, int8u posid)
{
    p_cal->adcarr[posid]=val;
}

//***********************************************************************/
// *功    能：角度A,K参数计算
// *入    口：p_cal,ang_calib结构体指针(X,Y);
// *出    口：
// *备    注：K=(sin(ang1)-sin(ang0))/(adc1-adc0);A=sin(ang0)-K*adc0;
// *函数性质：
//***********************************************************************/
static void Ang_CalcPrm(struct ang_calib *p_cal)
{
    float xdata ang0;
    float xdata ang1;

    int8u xdata i;
    for(i=0;i<(p_cal->calcnt-1);i++)
    {
        ang0=(PI*(p_cal->angarr[i]))/1800000.0;    //角度换算成弧度
        ang1=(PI*(p_cal->angarr[i+1]))/1800000.0;
        p_cal->prm_k[i] = (sin(ang1)-sin(ang0))/(int32s)(p_cal->adcarr[i+1]-p_cal->adcarr[i]);
        p_cal->prm_a[i] = sin(ang0)- p_cal->prm_k[i] * (p_cal->adcarr[i]);
    }
}


//***********************************************************************/
// *功    能：获取X轴温度补偿点个数
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u GetTempXCalCnt(void)
{
    return (Temp_Xcal.calcnt);    //获取X轴温度补偿点个数
}

int8u GetAngXCalCnt(void)
{
    return (Ang_Xcal.calcnt);    //获取X轴角度校准点个数
}
/*
int8u GetTempYCalCnt(void)
{
    return (Temp_Ycal.calcnt);    //获取Y轴温度补偿点个数 (没有用到注释起来）
}
*/
int8u GetAngYCalCnt(void)
{
    return (Ang_Ycal.calcnt);    //获取Y轴角度校准点个数
}


//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void SetTempXCalCnt(int8u cnt)
{
     Temp_SetCalCnt(&Temp_Xcal,cnt);    //设置X轴温度补偿点个数
}

void SetAngXCalCnt(int8u cnt)
{
     Ang_SetCalCnt(&Ang_Xcal,cnt);     //设置X轴角度校准点个数
}

void SetTempYCalCnt(int8u cnt)
{
     Temp_SetCalCnt(&Temp_Ycal,cnt);    //设置Y轴温度补偿点个数
}

void SetAngYCalCnt(int8u cnt)
{
     Ang_SetCalCnt(&Ang_Ycal,cnt);    //设置Y轴角度校准点个数
}

void SetTempCalCnt(int8u cnt)         //设置温度补偿点个数
{
     SetTempXCalCnt(cnt);
     SetTempYCalCnt(cnt);
}

void SetAngCalCnt(int8u cnt)         //设置角度校准点个数
{
     SetAngXCalCnt(cnt);
     SetAngYCalCnt(cnt);
}



//***********************************************************************/
// *功    能：读温度校准模块中X轴温度数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：温度AD值
// *备    注：保证位置在数组的范围内    X,Y共用温度数组ptempval[]
// *函数性质：
//***********************************************************************/
int16u GetTempXCalTempVal(int8u pos)
{
    return (Temp_RdTempVal(&Temp_Xcal,pos));
}
//***********************************************************************/
// *功    能：读取角度标定模块中的X轴角度值数组中的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度值
// *备    注：保证位置ID在数组大小的范围内
//***********************************************************************/
int32s GetAngXCalAngVal(int8u pos)
{
    return (Ang_RdAngVal(&Ang_Xcal,pos));
}
//***********************************************************************/
// *功    能：读温度校准模块中Y轴温度数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：温度AD值
// *备    注：保证位置在数组的范围内
// *函数性质：
//***********************************************************************/
int16u GetTempYCalTempVal(int8u pos)
{
    return (Temp_RdTempVal(&Temp_Ycal,pos));
}
//***********************************************************************/
// *功    能：读取角度标定模块中的Y轴角度值数组中的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度值
// *备    注：保证位置ID在数组大小的范围内
//***********************************************************************/
int32s GetAngYCalAngVal(int8u pos)
{
    return (Ang_RdAngVal(&Ang_Ycal,pos));
}

//***********************************************************************/
// *功    能：读温度校准模块中X轴ADC数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度0时的ADC值
// *备    注：保证位置在数组的范围内
//***********************************************************************/
int32u GetTempXCalAdcVal(int8u pos)
{
    return (Temp_RdAdcVal(&Temp_Xcal,pos));
}
//***********************************************************************/
// *功    能：读角度标定模块中X轴ADC数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度ADC值
// *备    注：保证位置在数组的范围内
//***********************************************************************/
int32u GetAngXCalAdcVal(int8u pos)
{
    return (Ang_RdAdcVal(&Ang_Xcal,pos));
}
//***********************************************************************/
// *功    能：读温度校准模块中Y轴ADC数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度0时的ADC值
// *备    注：保证位置在数组的范围内
//***********************************************************************/
int32u GetTempYCalAdcVal(int8u pos)
{
    return (Temp_RdAdcVal(&Temp_Ycal,pos));
}
//***********************************************************************/
// *功    能：读角度标定模块中Y轴ADC数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度ADC值
// *备    注：保证位置在数组的范围内
//***********************************************************************/
int32u GetAngYCalAdcVal(int8u pos)
{
    return (Ang_RdAdcVal(&Ang_Ycal,pos));
}
//***********************************************************************/
// *功    能：设置温度校准模块中X轴温度数组的某个元素
// *入    口：val，温度AD值;pos,数组中的位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetTempXCalTempVal(int16u val, int8u pos)
{
    Temp_WrTempVal(&Temp_Xcal,val,pos);
}
//***********************************************************************/
// *功    能：设置角度标定模块中的X轴角度值数组中的某个元素
// *入    口：val,角度值;pos,位置
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
//***********************************************************************/
void SetAngXCalAngVal(int32s val, int8u pos)
{
    Ang_WrAngVal(&Ang_Xcal,val,pos);
}
//***********************************************************************/
// *功    能：设置温度校准模块中Y轴温度数组的某个元素
// *入    口：val，温度AD值;pos,数组中的位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetTempYCalTempVal(int32s val, int8u pos)
{
    Temp_WrTempVal(&Temp_Ycal,val,pos);
}
//***********************************************************************/
// *功    能：设置角度标定模块中的Y轴角度值数组中的某个元素
// *入    口：val,角度值;pos,位置
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
//***********************************************************************/
void SetAngYCalAngVal(int32s val, int8u pos)
{
    Ang_WrAngVal(&Ang_Ycal,val,pos);
}
//***********************************************************************/
// *功    能：设置温度校准模块中X轴ADC数组的某个元素
// *入    口：val,写入的角度0时的AD值;pos,位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetTempXCalAdcVal(int32s val, int8u pos)			//2013-6-9 03:21PM数据类型有修改int32s改为int32u
{
    Temp_WrAdcVal(&Temp_Xcal,val,pos);
}
//***********************************************************************/
// *功    能：设置角度标定模块中的X轴角度AD值数组中的某个元素
// *入    口：val,角度ADC值;pos,位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetAngXCalAdcVal(int32u val, int8u pos)
{
    Ang_WrAdcVal(&Ang_Xcal,val,pos);
}
//***********************************************************************/
// *功    能：设置温度补偿模块中Y轴ADC数组的某个元素
// *入    口：val,写入的角度0时的AD值;pos,位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetTempYCalAdcVal(int32u val, int8u pos)		//2013-6-9 03:21PM修改int32s改为int32u
{
    Temp_WrAdcVal(&Temp_Ycal,val,pos);
}
//***********************************************************************/
// *功    能：设置角度标定模块中的Y轴角度AD值数组中的某个元素
// *入    口：val,角度ADC值;pos,位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetAngYCalAdcVal (int32u val, int8u pos)
{
    Ang_WrAdcVal(&Ang_Ycal,val,pos);
}
//***********************************************************************/
// *功    能：X轴温度补偿参数计算
// *备    注：
//***********************************************************************/
void TempXCalcPrm (void)
{
    Temp_CalcPrm(&Temp_Xcal);
}
//***********************************************************************/
// *功    能：Y轴温度补偿参数计算
//***********************************************************************/
void TempYCalcPrm (void)
{
    Temp_CalcPrm(&Temp_Ycal);
}
//***********************************************************************/
// *功    能：X轴角度标定参数计算
// *备    注：
//***********************************************************************/
void AngXCalcPrm (void)
{
    Ang_CalcPrm(&Ang_Xcal);
}
//***********************************************************************/
// *功    能：Y轴角度标定参数计算
// *备    注：
//***********************************************************************/
void AngYCalcPrm (void)
{
    Ang_CalcPrm(&Ang_Ycal);
}
//***********************************************************************/
// *功    能：温补默认数据初始化
// *入    口：
// *出    口：
// *备    注：Flash 初始化的时候调用
// *函数性质：
//***********************************************************************/
void TempCalDataInit()
{
    int8u xdata i;
    SetTempCalCnt(3);          //设置温度补偿点个数
    for(i=0;i<TEMP_CAL_NUM;i++)//21
    {
        Temp_Xcal.ptempval[i] = 500*(i-TEMP_CAL_NUM/2)+8000; //MCU的温度
        Temp_Xcal.adcarr[i] = 0;
        Temp_Ycal.adcarr[i] = 0;
    }
}
//***********************************************************************/
// *功    能：角度标定默认数据初始化
// *入    口：
// *出    口：
// *备    注：Flash 初始化的时候调用
// *函数性质：
//***********************************************************************/
void AngCalDataInit()
{
    int8u xdata i;
    SetAngCalCnt(3);          //角度校准点个数
    for(i=0;i<ANG_CAL_NUM;i++)//角度
    {
        Ang_Xcal.angarr[i] = 20000*(i-ANG_CAL_NUM/2);
        Ang_Xcal.adcarr[i] = 60000*(i-ANG_CAL_NUM/2)+8000000;//2011-11-07 修改校准角度为4位小数

        Ang_Ycal.angarr[i] = 20000*(i-ANG_CAL_NUM/2);
        Ang_Ycal.adcarr[i] = 60000*(i-ANG_CAL_NUM/2)+800000;
    }
    //----------------test-------------------
    Ang_Xcal.angarr[0]  =   -300000  ;	Ang_Xcal.adcarr[0]  =   1537     ;//2011-11-07 修改校准角度为三位小数
    Ang_Xcal.angarr[1]  =   0       ;	Ang_Xcal.adcarr[1]  =   129028   ;
    Ang_Xcal.angarr[2]  =   300000   ;	Ang_Xcal.adcarr[2]  =   258353  ;

    Ang_Ycal.angarr[0]  =   -300000  ;	Ang_Ycal.adcarr[0]  =   1537     ;
    Ang_Ycal.angarr[1]  =   0       ;	Ang_Ycal.adcarr[1]  =   128200    ;
    Ang_Ycal.angarr[2]  =   300000   ;	Ang_Ycal.adcarr[2]  =   258353   ;
    //-------------test-------------------	
}
//***********************************************************************/
// *功    能：校准模块初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：初始化函数指针
//***********************************************************************/
void    CalibInit(void)
{
    Temp_Xcal.ptempval=temp_buf;            	//温度补偿模块中温度AD值缓存数组
    Temp_Xcal.SetCalNum=SetTempXCalCnt;     	//设置校准个数	函数
    Temp_Xcal.RdTempVal=GetTempXCalTempVal;  	//读设置数据
    Temp_Xcal.RdAdcVal=GetTempXCalAdcVal;    	//读数据
    Temp_Xcal.WrTempVal=SetTempXCalTempVal;   	//写数据
    Temp_Xcal.WrAdcVal=SetTempXCalAdcVal;     	//写返回数据
//  Temp_Xcal.CalcPrm=TempXCalcPrm;           	//计算参数A,K

    Temp_Ycal.ptempval=temp_buf;            	//温度补偿模块中温度AD值缓存数组
    Temp_Ycal.SetCalNum=SetTempYCalCnt;
    Temp_Ycal.RdTempVal=GetTempYCalTempVal;
    Temp_Ycal.RdAdcVal=GetTempYCalAdcVal;
    Temp_Ycal.WrTempVal=SetTempYCalTempVal;
    Temp_Ycal.WrAdcVal=SetTempYCalAdcVal;
//  Temp_Ycal.CalcPrm=TempYCalcPrm;

    Ang_Xcal.SetCalNum=SetAngXCalCnt;
    Ang_Xcal.RdAngVal=GetAngXCalAngVal;
    Ang_Xcal.RdAdcVal=GetAngXCalAdcVal;
    Ang_Xcal.WrAngVal=SetAngXCalAngVal;
    Ang_Xcal.WrAdcVal=SetAngXCalAdcVal;
//  Ang_Xcal.CalcPrm=AngXCalcPrm;

    Ang_Ycal.SetCalNum=SetAngYCalCnt;
    Ang_Ycal.RdAngVal=GetAngYCalAngVal;
    Ang_Ycal.RdAdcVal=GetAngYCalAdcVal;
    Ang_Ycal.WrAngVal=SetAngYCalAngVal;
    Ang_Ycal.WrAdcVal=SetAngYCalAdcVal;
//  Ang_Ycal.CalcPrm=AngYCalcPrm;
}

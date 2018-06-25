#define __CALMCUT_C__
#include "_Include.h"


static struct mcut_calib xdata *PMCUTCAL= &McuTcal;
static int16u xdata mtemp_buf[MTEMP_CAL_NUM];

//***********************************************************************/
// *功    能：设置MCU温度补偿点个数
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void McuT_SetCalCnt(struct mcut_calib *p_cal, int8u num)
{
    p_cal->calcnt=num;
}

//***********************************************************************/
// *功    能：读温度校准模块中温度数组的某个元素
// *入    口：
// *出    口：温度值
// *备    注：
// *函数性质：
//***********************************************************************/

static int16s   McuT_RdTempVal(struct mcut_calib *p_cal, int8u posid)
{
    return (p_cal->mcu_temp[posid]);
}
//***********************************************************************/
// *功    能：读MCU温度校准模块中ADC数组的某个元素
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static int16u   McuT_RdAdcVal(struct mcut_calib *p_cal, int8u posid)
{
    return (p_cal->ptempadc[posid]);
}

//***********************************************************************/
// *功    能：设置温度校准模块中温度数组的某个元素
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void McuT_WrTempVal(struct mcut_calib *p_cal,int16s val, int8u posid)
{
    p_cal->mcu_temp[posid]=val;
}

//***********************************************************************/
// *功    能：设置温度校准模块中ADC数组的某个元素
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void McuT_WrAdcVal(struct mcut_calib *p_cal,int16u val, int8u posid)
{
    p_cal->ptempadc[posid]=val;
}

//***********************************************************************/
// *功    能：MCU温度A,K参数计算
// *入    口：p_cal,temp_calib结构体指针(X,Y);
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Mcut_CalcPrm(struct mcut_calib *p_cal)
{
    int8u xdata i;
//	float aa;
//	float bb,cc;
    for(i=0;i<(p_cal->calcnt-1);i++)    						//计算所有的A,K值
    {
//		aa=(float)(p_cal->mcu_temp[i+1]-p_cal->mcu_temp[i]);
//		bb=(float)((int32s)p_cal->ptempadc[i+1]-(int32s)p_cal->ptempadc[i]);
//		cc = (float)(aa/bb);
        p_cal->prm_k[i]=((float)(p_cal->mcu_temp[i+1]-p_cal->mcu_temp[i])/((int32s)p_cal->ptempadc[i+1]-(int32s)p_cal->ptempadc[i]));
        p_cal->prm_a[i] = (float)(p_cal->mcu_temp[i]-p_cal->prm_k[i]*(p_cal->ptempadc[i]) );
    }
}



void SetMcuTCalCnt(int8u cnt)
{
     McuT_SetCalCnt(&McuTcal,cnt);    //设置温度校准点个数
}

//***********************************************************************/
// *功    能：获取MCU温度校准点个数
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u GetMcuTCalCnt(void)
{
    return (McuTcal.calcnt);    //获取MCU温度校准点个数
}
//***********************************************************************/
// *功    能：读MCU温度校准中温度数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：温度值
// *备    注：
// *函数性质：
//***********************************************************************/
int16s GetMcuTCalTempVal(int8u pos)
{
    return (McuT_RdTempVal(&McuTcal,pos));
}
//***********************************************************************/
// *功    能：读温度校准模块中X轴ADC数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度0时的ADC值
// *备    注：保证位置在数组的范围内
//***********************************************************************/
int16u GetMcuTCalAdcVal(int8u pos)
{
    return (McuT_RdAdcVal(&McuTcal,pos));
}
//***********************************************************************/
// *功    能：设置温度校准模块中X轴温度数组的某个元素
// *入    口：val，温度AD值;pos,数组中的位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetMcuTCalTempVal(int16s val, int8u pos)
{
    McuT_WrTempVal(&McuTcal,val,pos);
}
//***********************************************************************/
// *功    能：设置温度校准模块中X轴ADC数组的某个元素
// *入    口：val,写入的角度0时的AD值;pos,位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetMcuTCalAdcVal(int16u val, int8u pos)
{
    McuT_WrAdcVal(&McuTcal,val,pos);
}
//***********************************************************************/
 
void  CalibMtInit(void)
{
	McuTcal.ptempadc=mtemp_buf;            //温度校准模块中温度AD值缓存数组
    McuTcal.SetCalNum=SetMcuTCalCnt;     //设置温度校准个数
    McuTcal.RdTempVal=GetMcuTCalTempVal; //读温度设置数据
    McuTcal.RdAdcVal=GetMcuTCalAdcVal;   //读温度AD数据
    McuTcal.WrTempVal=SetMcuTCalTempVal; //写温度设置数据
    McuTcal.WrAdcVal=SetMcuTCalAdcVal;   //写温度AD返回数据
//  McuTcal.CalcPrm=McuTCalcPrm;            //计算参数A,K
}
//***********************************************************************/
// *功    能：MCU温度标定默认数据初始化
// *入    口：
// *出    口：
// *备    注：Flash 初始化的时候调用
// *函数性质：
//***********************************************************************/
void MTCalDataInit()
{
    int8u xdata i;
    SetMcuTCalCnt(5);          								//设置温度校准点个数
    for(i=0;i<MTEMP_CAL_NUM;i++)							//10
    {
        McuTcal.ptempadc[i] = 500*(i-MTEMP_CAL_NUM/2)+8000; //MCU的温度AD
//		if(i==0)
//      {
//			McuTcal.mcu_temp[i] = 25;
//		}
		McuTcal.mcu_temp[i] = 5*(i-ANG_CAL_NUM/2); 
    }
	McuTcal.ptempadc[0]= 217;
	McuTcal.ptempadc[1]= 207;
	McuTcal.ptempadc[2]= 197;
	McuTcal.ptempadc[3]= 185;
	McuTcal.ptempadc[4]= 176;
	McuTcal.ptempadc[5]= 163;
	McuTcal.ptempadc[6]= 153;
	McuTcal.ptempadc[7]= 140;
	McuTcal.ptempadc[8]= 129;

	McuTcal.mcu_temp[0]= -15;
	McuTcal.mcu_temp[1]= -5;
	McuTcal.mcu_temp[2]= 5;
	McuTcal.mcu_temp[3]= 15;
	McuTcal.mcu_temp[4]= 25;
	McuTcal.mcu_temp[5]= 35;
	McuTcal.mcu_temp[6]= 45;
	McuTcal.mcu_temp[7]= 55;
	McuTcal.mcu_temp[8]= 65;
}
//***********************************************************************/
// *功    能：获取MCU温度值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
//float GetMt(int16u tadc)
//{
//    int8u xdata i;
//    float xdata offset;
//
//    for(i=0;i<(PMCUTCAL->calcnt-2);i++)								//找不到符合条件的直接通过最后一组标定值进行温度计算
//    {
//        if(tadc>(PMCUTCAL->ptempadc[i+1]))    						//查找温度在温度校准参数温度数组中的位置
//            break;
//    }
//    offset = (PMCUTCAL->prm_k[i]*tadc+PMCUTCAL->prm_a[i])*10000;    //获取相应的K,A值，并计算  
//
//    return offset;
//}



//***********************************************************************/
// *功    能：根据温度进行升序排序，并计算MCU温度系数K,A
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void McuTCalcPrm ()
{

	Sub_BufBubble16s(PMCUTCAL->mcu_temp,PMCUTCAL->ptempadc,PMCUTCAL->calcnt);//排序

	Mcut_CalcPrm(&McuTcal);											  			 //MCU T参数计算

}
//***********************************************************************/
// *功    能：写入温度校准参数
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u McuT_CalValWr (int8u id,int16s temp,int16u tadc)
{
	if(id>=MTEMP_CAL_NUM)return FALSE;
	PMCUTCAL->WrTempVal(temp,id);       //写标定的温度值
	PMCUTCAL->WrAdcVal(tadc,id);        //写当前标定的温度AD值

	return TRUE;
}
//***********************************************************************/
// *功    能：MCU温度标定开始
// *入    口：
// *出    口：
// *备    注：用于通信中
// *函数性质：
//***********************************************************************/
int8u McuT_CalSta (void)
{
//	if(MenuId!=MOD_MOD0)return FALSE;
//	MenuId=MOD_MT_CAL;							  //因为MCU温度标定和温补一起作业，此处需注释掉
	return TRUE;
}
//***********************************************************************/
// *功    能：MCU温度标定结束
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u McuT_CalStop (void)
{
//	if(MenuId!=MOD_MT_CAL)return FALSE;
//	MenuId=MOD_MOD0;						   //因为MCU温度标定和温补一起作业，此处需注释掉
	McuTCalcPrm();
	return TRUE;
}
//***********************************************************************/
int8u Mcut_CalSamp (int8u id,int16s temp,int16u tadc)
{
//	if(MenuId!=MOD_MT_CAL)
//		return FALSE;

	if(McuT_CalValWr(id,temp,tadc)==FALSE)
	return FALSE;
	Sub_SendMcuT(temp,tadc);
	return TRUE;
}
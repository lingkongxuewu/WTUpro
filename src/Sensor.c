/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Sensor.c
* �桡����  v1.0
* ��  ����  100T˫�� ���Ϻ�İ汾
* �����ˣ�  
* ��  �ڣ�  
--------------------------------------------------------------------------
*��ǰ�汾:	v1.0
*�� �� ��:
*�ա�  ��:
----------------------------------�޸�����--------------------------------
1��
2��
3��
4��
------------------------------------------------------------------------*/
#define __SENSOR_C__
#include "_Include.h"

//-------------define-----------------

static int8u    xdata  filtercnt; //�˲�ϵ��
//static int32u xdata  adfnum;
//***********************************************************************/
// *��    �ܣ����ݳ�ʼ��
// *��    �ڣ�p_sn,sensor�ṹ��ָ��(X,Y);p_tempcal,temp_calib�ṹ��ָ��(�²�);
//            p_angcal,ang_calib�ṹ��ָ��(�Ƕȱ궨)
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Sn_DataInit(struct sensor *p_sn, struct temp_calib *p_tempcal, struct ang_calib *p_angcal)
{
        p_sn->val=0;     //ԭʼֵ
        p_sn->ftval=0;   //�˲�֮���ֵ
        p_sn->ftval_f=0;
        p_sn->oftval=0;  //�²�֮���ֵ
        p_sn->ang=0;    //
        p_sn->tempcal=p_tempcal; //ָ��ṹ��temp_xcal /Temp_Ycal
        p_sn->angcal=p_angcal;    //ָ��ang_xcal Ang_Ycal
}

//***********************************************************************/
// *��    �ܣ�������ֵ�˲�
// *��    �ڣ�p_sn,sensor�ṹ��ָ��(X,Y);val,��ֵ;ftcnt��һ���˲�ϵ��
// *��    �ڣ�
// *��    ע��ADC�˲����ֵ
// *�������ʣ�
//***********************************************************************/
static void SnFilter(struct sensor *p_sn, int32u val, int8u ftcnt)
{
    if(ValInRange(p_sn->ftval,val,(int32u)FILTER_XY))    //�޷��˲�
    {
        p_sn->ftval_f=Filter_1st(p_sn->ftval_f,val,ftcnt);     //һ���ͺ��˲� 
        
        p_sn->ftval=(int32u)(p_sn->ftval_f+0.5);		//��������
        
    }
    else
        p_sn->ftval  =val;    //�仯���Ƚϴ�ֱ��д����ֵ	 
        p_sn->ftval_f=val;
}

//***********************************************************************/
// *��    �ܣ���ȡ�²��ĽǶ�ֵ
// *��    �ڣ�p_sn,sensor�ṹ��ָ��(X,Y);temp,�¶�ֵ
// *��    �ڣ��²�ֵ
// *��    ע��offset=k*temp+a;
// *�������ʣ�
//*********************************** ************************************/
static int32s GetOffset(struct sensor *p_sn, int16u temp)
{
    int8u xdata i;
    float xdata offset;

    for(i=0;i<(p_sn->tempcal->calcnt-2);i++)	  //�����¶�У׼�����  3
    {
        if(temp<(p_sn->tempcal->ptempval[i+1]))    //�����¶����²�ģ�����¶������е�λ��
            break;
    }
    offset =(int32s)( p_sn->tempcal->prm_k[i]*temp+p_sn->tempcal->prm_a[i]);    //��ȡ��Ӧ��K,Aֵ��������

    return offset;
}

//***********************************************************************/
// *��    �ܣ���ȡ�궨��ĽǶ�ֵ
// *��    �ڣ�p_sn,sensor�ṹ��ָ��(X,Y);val,�Ƕȵ�ADCֵ
// *��    �ڣ��Ƕ�����ֵ
// *��    ע��ang=k*ang_adc+a;																			
// *�������ʣ�
//***********************************************************************/
static float GetAngCalcVal(struct sensor *p_sn, int32u val)
{
    int8u xdata i;
    float xdata tmp;

	for(i=0;i<(p_sn->angcal->calcnt-2);i++)
    {
        if(val<(p_sn->angcal->adcarr[i+1]))    //���ҵ�ǰ�Ƕ�ADCֵ�ڽǶȱ궨ģ����ADCֵ�����е�λ��
            break;
    }
    tmp = p_sn->angcal->prm_k[i]*val + p_sn->angcal->prm_a[i];    //��ȡ��Ӧ��K,Aֵ��������
    return tmp;
}

//***********************************************************************/
// *��    �ܣ��Ƕȼ���
// *��    �ڣ�p_sn,sensor�ṹ��ָ��(X,Y);
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Ang_Calc(struct sensor *p_sn)
{
    float xdata ang;
    ang=GetAngCalcVal(p_sn,p_sn->oftval);       //�Ƕȱ궨��ĽǶ�����ֵ
	switch(AngRange)
	{
		case 30:
		{
			if(ang>=0.5)
		        p_sn->ang=300000;
		    else if(ang<=-0.5)		
		        p_sn->ang=-300000;
		    else
		    {
		        ang=asin(ang);
		        ang=ang/PI*1800000;
		        p_sn->ang=ang;
		    }
		}
		break;
		case 15:
		{
//		Comm_PutInt32s(ang*10000);
//		Comm_PutChar('\n');
//		Comm_SendSta();
			if(ang>=0.2588)
		        p_sn->ang=150000;
		    else if(ang<=-0.2588)
		        p_sn->ang=-150000;
		    else
		    {
		        ang=asin(ang);
		        ang=ang/PI*1800000;
		        p_sn->ang=ang;
		    }
		}
		break;
		case 5:
		{
			if(ang>=0.0871)
		        p_sn->ang=50000;
		    else if(ang<=-0.0871)
		        p_sn->ang=-50000;	
		    else
		    {
		        ang=asin(ang);
		        ang=ang/PI*1800000;
		        p_sn->ang=ang;
		    }	
		}
		break;
	}
}

//***********************************************************************/
// *��    �ܣ����ٶȴ�����X���ݳ�ʼ��
// *��    ע��temp_xcal���²�ģ�飻ang_xcal���Ƕȱ궨ģ��
//***********************************************************************/
void SnXDataInit(void)
{
    Sn_DataInit(&accel_x,&Temp_Xcal,&Ang_Xcal);
}
void SnYDataInit(void)
{
    Sn_DataInit(&accel_y,&Temp_Ycal,&Ang_Ycal);    //���ٶȴ�����Y���ݳ�ʼ��
}
void SnXFilter(int32u val, int8u ftcnt)
{
    SnFilter(&accel_x,val,ftcnt);    //X�˲�
}
void SnYFilter(int32u val, int8u ftcnt)
{
    SnFilter(&accel_y,val,ftcnt);    //y�˲�
}
int32s GetXOffset(int16u temp)
{
    return(GetOffset(&accel_x,temp));    //X�²�
}
int32s GetYOffset(int16u temp)
{
    return(GetOffset(&accel_y,temp));    //Y�²�
}
void AngXCalc(void)
{
    Ang_Calc(&accel_x);    //X�Ƕȼ���
}
void AngYCalc(void)
{
    Ang_Calc(&accel_y);    //Y�Ƕȼ���
}
//***********************************************************************/
// *��    �ܣ��������ṹ���ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void SnInit(void)
{
    CalibInit();            //У׼ģ���ʼ��
    SetFilterCnt(4);        //
    accel_x.DataInit=SnXDataInit;
    accel_x.Filter=SnXFilter;
    accel_x.GetTempOffset=GetXOffset;
    accel_x.AngCalc=AngXCalc;
    accel_x.DataInit();

    accel_y.DataInit=SnYDataInit;
    accel_y.Filter=SnYFilter;
    accel_y.GetTempOffset=GetYOffset;
    accel_y.AngCalc=AngYCalc;
    accel_y.DataInit();
}

//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȵ�ADֵ�������˲��󣩣�����һ���˲����¶Ȳ��� ������Ƕ�ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��2013-6-9 10:47AM�Ѿ��޸�
// *�������ʣ�
//***********************************************************************/
void SnProc(void)
{    
	if(AdcEndTag==0)	 return;
    AdcEndTag=0;
    
    switch(Adc_Flag)
	{	case 0:
				accel_x.val=Adc_GetXVal();                                             //��ȡX��ĽǶ�ADֵ �������˲���
				SnXFilter(accel_x.val,filtercnt);	//ֱ�ӵ��ú���
				accel_x.oftval=(int32s)((int32s)accel_x.ftval+
                                accel_x.GetTempOffset(Get_McuTemp()));  //�¶Ȳ���
				accel_x.AngCalc(); 					//����Ƕȣ��²���
				break;
	     case 1:
	     		accel_y.val=Adc_GetYVal();                                             //��ȡX��ĽǶ�ADֵ �������˲���
				SnYFilter(accel_y.val,filtercnt);	//ֱ�ӵ��ú���                             //һ���˲�
            	accel_y.oftval=(int32s)((int32s)accel_y.ftval+
                                    accel_y.GetTempOffset(Get_McuTemp()));          //�¶Ȳ���                                
				accel_y.AngCalc();
				break;
		default:
				break;               
	}
}

//***********************************************************************/
// *��    �ܣ���ȡһ���˲�ϵ��
// *��    �ڣ�
// *��    �ڣ�һ���˲�ϵ��
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u GetFilterCnt(void)
{
    return filtercnt;
}
//***********************************************************************/
// *��    �ܣ�����һ���˲�ϵ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void SetFilterCnt(int8u cnt)
{
    filtercnt=cnt;
}

//***********************************************************************/
// *��    �ܣ���ȡ��ǰ�¶�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int16u Get_McuTemp(void)
{
    return(Adc_GetTemp());      //MCU������¶�ֵ
}

//***********************************************************************/
// *��    �ܣ���ȡX��һ���˲����ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int32u GetXFtVal(void)
{
    return (accel_x.ftval);
}

//***********************************************************************/
// *��    �ܣ���ȡY��һ���˲����ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int32u GetYFtVal(void)
{
    return (accel_y.ftval);
}

//***********************************************************************/
// *��    �ܣ���ȡX���²����ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int32s GetXOftVal(void)
{
    return (accel_x.oftval);
}

//***********************************************************************/
// *��    �ܣ���ȡY���²����ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int32s GetYOftVal(void)
{
    return (accel_y.oftval);
}

//***********************************************************************/
// *��    �ܣ���ȡX��Ƕ�ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int32s GetXAng(void)
{
    return (accel_x.ang);
}

//***********************************************************************/
// *��    �ܣ���ȡY��Ƕ�ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int32s GetYAng(void)
{
    return (accel_y.ang);
}

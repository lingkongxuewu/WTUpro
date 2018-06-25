/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Calibration.c
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
#define __CALIBR_C__
#include "_Include.h"
//struct temp_calib xdata Temp_Xcal;    //X���¶Ȳ���
//struct temp_calib xdata Temp_Ycal;    //Y���¶Ȳ���

//struct ang_calib xdata Ang_Xcal;    //X��Ƕ�У׼
//struct ang_calib xdata Ang_Ycal;    //Y��Ƕ�У׼

static int16u xdata temp_buf[TEMP_CAL_NUM];    //21

static float    Ang_RdXAVal(struct ang_calib *p_cal, int8u posid);

//***********************************************************************/
// *��    �ܣ������¶Ȳ��������
// *��    �ڣ�p_cal,temp_calib�ṹ��ָ��(X,Y);num,����
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Temp_SetCalCnt(struct temp_calib *p_cal, int8u num)
{
    p_cal->calcnt=num;
}
//***********************************************************************/
// *��    �ܣ����¶�У׼ģ�����¶������ĳ��Ԫ��
// *��    �ڣ�p_cal,temp_calib�ṹ��ָ��(X,Y);posid,λ��ID��
// *��    �ڣ��¶�ֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static int16u   Temp_RdTempVal(struct temp_calib *p_cal, int8u posid)
{
    return (p_cal->ptempval[posid]);
}

//***********************************************************************/
// *��    �ܣ����¶�У׼ģ����ADC�����ĳ��Ԫ��
// *��    �ڣ�p_cal,temp_calib�ṹ��ָ��(X,Y);posid,λ��ID��
// *��    �ڣ��Ƕ�0ʱ��ADCֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static int32s   Temp_RdAdcVal(struct temp_calib *p_cal, int8u posid)
{
    return (p_cal->adcarr[posid]);
}

//***********************************************************************/
// *��    �ܣ������¶�У׼ģ�����¶������ĳ��Ԫ��
// *��    �ڣ�p_cal,temp_calib�ṹ��ָ��(X,Y);val,�¶�ADֵ;posid,λ��ID��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static void Temp_WrTempVal(struct temp_calib *p_cal,int16u val, int8u posid)
{
    p_cal->ptempval[posid]=val;
}

//***********************************************************************/
// *��    �ܣ������¶�У׼ģ����ADC�����ĳ��Ԫ��
// *��    �ڣ�p_cal,temp_calib�ṹ��ָ��(X,Y);val,д��ĽǶ�0ʱ��ADֵ;posid,λ��ID��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static void Temp_WrAdcVal(struct temp_calib *p_cal,int32s val, int8u posid)
{
    p_cal->adcarr[posid]=val;
}

//***********************************************************************/
// *��    �ܣ�0�Ƕ��¶�Ư��A,K��������//
// *��    �ڣ�p_cal,temp_calib�ṹ��ָ��(X,Y);
// *��    �ڣ�
// *��    ע��K=(ADC1-ADC0)/(TEMP1-TEMP0);A=ADC0-K*TEMP0;
// *�������ʣ��ڽǶ�Ϊ0����ĳһ���¶�Ϊ�ο����ǰ���£��Ƕ�ADֵ���¶ȱ仯�Ĺ�ϵ���������е�adֵ
//***********************************************************************/
static void Temp_CalcPrm(struct temp_calib *p_cal)
{
    int8u xdata i;
    for(i=0;i<(p_cal->calcnt-1);i++)    						//�������е�A,Kֵ
    {
        p_cal->prm_k[i]=((float)(p_cal->adcarr[i+1]-p_cal->adcarr[i])/(float)(p_cal->ptempval[i+1]-p_cal->ptempval[i]));
        p_cal->prm_a[i] = (float)(p_cal->adcarr[i])-p_cal->prm_k[i] * (p_cal->ptempval[i]);
    }
}

//***********************************************************************/
// *��    �ܣ����ýǶȱ궨�����
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);num,����
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Ang_SetCalCnt(struct ang_calib *p_cal, int8u num)
{
    p_cal->calcnt=num;
}
//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȱ궨ģ���еĽǶ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);posid,λ��ID��
// *��    �ڣ��Ƕ�ֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static int32s   Ang_RdAngVal(struct ang_calib *p_cal, int8u posid)
{
    return (p_cal->angarr[posid]);
}

//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȱ궨ģ���еĽǶ�ADֵ�����е�ĳ��Ԫ��
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);posid,λ��ID��
// *��    �ڣ��Ƕ�ADCֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static int32u   Ang_RdAdcVal(struct ang_calib *p_cal, int8u posid)
{
    return (p_cal->adcarr[posid]);
}
//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���еĽǶ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);val,�Ƕ�ֵ;posid,λ��ID��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static void Ang_WrAngVal(struct ang_calib *p_cal,int32s val, int8u posid)
{
    p_cal->angarr[posid]=val;
}

//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���еĽǶ�ADֵ�����е�ĳ��Ԫ��
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);val,�Ƕ�ADCֵ;posid,λ��ID��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static void Ang_WrAdcVal(struct ang_calib *p_cal,int32u val, int8u posid)
{
    p_cal->adcarr[posid]=val;
}

//***********************************************************************/
// *��    �ܣ��Ƕ�A,K��������
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);
// *��    �ڣ�
// *��    ע��K=(sin(ang1)-sin(ang0))/(adc1-adc0);A=sin(ang0)-K*adc0;
// *�������ʣ�
//***********************************************************************/
static void Ang_CalcPrm(struct ang_calib *p_cal)
{
    float xdata ang0;
    float xdata ang1;

    int8u xdata i;
    for(i=0;i<(p_cal->calcnt-1);i++)
    {
        ang0=(PI*(p_cal->angarr[i]))/1800000.0;    //�ǶȻ���ɻ���
        ang1=(PI*(p_cal->angarr[i+1]))/1800000.0;
        p_cal->prm_k[i] = (sin(ang1)-sin(ang0))/(int32s)(p_cal->adcarr[i+1]-p_cal->adcarr[i]);
        p_cal->prm_a[i] = sin(ang0)- p_cal->prm_k[i] * (p_cal->adcarr[i]);
    }
}


//***********************************************************************/
// *��    �ܣ���ȡX���¶Ȳ��������
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u GetTempXCalCnt(void)
{
    return (Temp_Xcal.calcnt);    //��ȡX���¶Ȳ��������
}

int8u GetAngXCalCnt(void)
{
    return (Ang_Xcal.calcnt);    //��ȡX��Ƕ�У׼�����
}
/*
int8u GetTempYCalCnt(void)
{
    return (Temp_Ycal.calcnt);    //��ȡY���¶Ȳ�������� (û���õ�ע��������
}
*/
int8u GetAngYCalCnt(void)
{
    return (Ang_Ycal.calcnt);    //��ȡY��Ƕ�У׼�����
}


//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void SetTempXCalCnt(int8u cnt)
{
     Temp_SetCalCnt(&Temp_Xcal,cnt);    //����X���¶Ȳ��������
}

void SetAngXCalCnt(int8u cnt)
{
     Ang_SetCalCnt(&Ang_Xcal,cnt);     //����X��Ƕ�У׼�����
}

void SetTempYCalCnt(int8u cnt)
{
     Temp_SetCalCnt(&Temp_Ycal,cnt);    //����Y���¶Ȳ��������
}

void SetAngYCalCnt(int8u cnt)
{
     Ang_SetCalCnt(&Ang_Ycal,cnt);    //����Y��Ƕ�У׼�����
}

void SetTempCalCnt(int8u cnt)         //�����¶Ȳ��������
{
     SetTempXCalCnt(cnt);
     SetTempYCalCnt(cnt);
}

void SetAngCalCnt(int8u cnt)         //���ýǶ�У׼�����
{
     SetAngXCalCnt(cnt);
     SetAngYCalCnt(cnt);
}



//***********************************************************************/
// *��    �ܣ����¶�У׼ģ����X���¶������ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��¶�ADֵ
// *��    ע����֤λ��������ķ�Χ��    X,Y�����¶�����ptempval[]
// *�������ʣ�
//***********************************************************************/
int16u GetTempXCalTempVal(int8u pos)
{
    return (Temp_RdTempVal(&Temp_Xcal,pos));
}
//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȱ궨ģ���е�X��Ƕ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�ֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
//***********************************************************************/
int32s GetAngXCalAngVal(int8u pos)
{
    return (Ang_RdAngVal(&Ang_Xcal,pos));
}
//***********************************************************************/
// *��    �ܣ����¶�У׼ģ����Y���¶������ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��¶�ADֵ
// *��    ע����֤λ��������ķ�Χ��
// *�������ʣ�
//***********************************************************************/
int16u GetTempYCalTempVal(int8u pos)
{
    return (Temp_RdTempVal(&Temp_Ycal,pos));
}
//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȱ궨ģ���е�Y��Ƕ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�ֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
//***********************************************************************/
int32s GetAngYCalAngVal(int8u pos)
{
    return (Ang_RdAngVal(&Ang_Ycal,pos));
}

//***********************************************************************/
// *��    �ܣ����¶�У׼ģ����X��ADC�����ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�0ʱ��ADCֵ
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
int32u GetTempXCalAdcVal(int8u pos)
{
    return (Temp_RdAdcVal(&Temp_Xcal,pos));
}
//***********************************************************************/
// *��    �ܣ����Ƕȱ궨ģ����X��ADC�����ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�ADCֵ
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
int32u GetAngXCalAdcVal(int8u pos)
{
    return (Ang_RdAdcVal(&Ang_Xcal,pos));
}
//***********************************************************************/
// *��    �ܣ����¶�У׼ģ����Y��ADC�����ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�0ʱ��ADCֵ
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
int32u GetTempYCalAdcVal(int8u pos)
{
    return (Temp_RdAdcVal(&Temp_Ycal,pos));
}
//***********************************************************************/
// *��    �ܣ����Ƕȱ궨ģ����Y��ADC�����ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�ADCֵ
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
int32u GetAngYCalAdcVal(int8u pos)
{
    return (Ang_RdAdcVal(&Ang_Ycal,pos));
}
//***********************************************************************/
// *��    �ܣ������¶�У׼ģ����X���¶������ĳ��Ԫ��
// *��    �ڣ�val���¶�ADֵ;pos,�����е�λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetTempXCalTempVal(int16u val, int8u pos)
{
    Temp_WrTempVal(&Temp_Xcal,val,pos);
}
//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���е�X��Ƕ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�val,�Ƕ�ֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
//***********************************************************************/
void SetAngXCalAngVal(int32s val, int8u pos)
{
    Ang_WrAngVal(&Ang_Xcal,val,pos);
}
//***********************************************************************/
// *��    �ܣ������¶�У׼ģ����Y���¶������ĳ��Ԫ��
// *��    �ڣ�val���¶�ADֵ;pos,�����е�λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetTempYCalTempVal(int32s val, int8u pos)
{
    Temp_WrTempVal(&Temp_Ycal,val,pos);
}
//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���е�Y��Ƕ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�val,�Ƕ�ֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
//***********************************************************************/
void SetAngYCalAngVal(int32s val, int8u pos)
{
    Ang_WrAngVal(&Ang_Ycal,val,pos);
}
//***********************************************************************/
// *��    �ܣ������¶�У׼ģ����X��ADC�����ĳ��Ԫ��
// *��    �ڣ�val,д��ĽǶ�0ʱ��ADֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetTempXCalAdcVal(int32s val, int8u pos)			//2013-6-9 03:21PM�����������޸�int32s��Ϊint32u
{
    Temp_WrAdcVal(&Temp_Xcal,val,pos);
}
//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���е�X��Ƕ�ADֵ�����е�ĳ��Ԫ��
// *��    �ڣ�val,�Ƕ�ADCֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetAngXCalAdcVal(int32u val, int8u pos)
{
    Ang_WrAdcVal(&Ang_Xcal,val,pos);
}
//***********************************************************************/
// *��    �ܣ������¶Ȳ���ģ����Y��ADC�����ĳ��Ԫ��
// *��    �ڣ�val,д��ĽǶ�0ʱ��ADֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetTempYCalAdcVal(int32u val, int8u pos)		//2013-6-9 03:21PM�޸�int32s��Ϊint32u
{
    Temp_WrAdcVal(&Temp_Ycal,val,pos);
}
//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���е�Y��Ƕ�ADֵ�����е�ĳ��Ԫ��
// *��    �ڣ�val,�Ƕ�ADCֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetAngYCalAdcVal (int32u val, int8u pos)
{
    Ang_WrAdcVal(&Ang_Ycal,val,pos);
}
//***********************************************************************/
// *��    �ܣ�X���¶Ȳ�����������
// *��    ע��
//***********************************************************************/
void TempXCalcPrm (void)
{
    Temp_CalcPrm(&Temp_Xcal);
}
//***********************************************************************/
// *��    �ܣ�Y���¶Ȳ�����������
//***********************************************************************/
void TempYCalcPrm (void)
{
    Temp_CalcPrm(&Temp_Ycal);
}
//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨��������
// *��    ע��
//***********************************************************************/
void AngXCalcPrm (void)
{
    Ang_CalcPrm(&Ang_Xcal);
}
//***********************************************************************/
// *��    �ܣ�Y��Ƕȱ궨��������
// *��    ע��
//***********************************************************************/
void AngYCalcPrm (void)
{
    Ang_CalcPrm(&Ang_Ycal);
}
//***********************************************************************/
// *��    �ܣ��²�Ĭ�����ݳ�ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��Flash ��ʼ����ʱ�����
// *�������ʣ�
//***********************************************************************/
void TempCalDataInit()
{
    int8u xdata i;
    SetTempCalCnt(3);          //�����¶Ȳ��������
    for(i=0;i<TEMP_CAL_NUM;i++)//21
    {
        Temp_Xcal.ptempval[i] = 500*(i-TEMP_CAL_NUM/2)+8000; //MCU���¶�
        Temp_Xcal.adcarr[i] = 0;
        Temp_Ycal.adcarr[i] = 0;
    }
}
//***********************************************************************/
// *��    �ܣ��Ƕȱ궨Ĭ�����ݳ�ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��Flash ��ʼ����ʱ�����
// *�������ʣ�
//***********************************************************************/
void AngCalDataInit()
{
    int8u xdata i;
    SetAngCalCnt(3);          //�Ƕ�У׼�����
    for(i=0;i<ANG_CAL_NUM;i++)//�Ƕ�
    {
        Ang_Xcal.angarr[i] = 20000*(i-ANG_CAL_NUM/2);
        Ang_Xcal.adcarr[i] = 60000*(i-ANG_CAL_NUM/2)+8000000;//2011-11-07 �޸�У׼�Ƕ�Ϊ4λС��

        Ang_Ycal.angarr[i] = 20000*(i-ANG_CAL_NUM/2);
        Ang_Ycal.adcarr[i] = 60000*(i-ANG_CAL_NUM/2)+800000;
    }
    //----------------test-------------------
    Ang_Xcal.angarr[0]  =   -300000  ;	Ang_Xcal.adcarr[0]  =   1537     ;//2011-11-07 �޸�У׼�Ƕ�Ϊ��λС��
    Ang_Xcal.angarr[1]  =   0       ;	Ang_Xcal.adcarr[1]  =   129028   ;
    Ang_Xcal.angarr[2]  =   300000   ;	Ang_Xcal.adcarr[2]  =   258353  ;

    Ang_Ycal.angarr[0]  =   -300000  ;	Ang_Ycal.adcarr[0]  =   1537     ;
    Ang_Ycal.angarr[1]  =   0       ;	Ang_Ycal.adcarr[1]  =   128200    ;
    Ang_Ycal.angarr[2]  =   300000   ;	Ang_Ycal.adcarr[2]  =   258353   ;
    //-------------test-------------------	
}
//***********************************************************************/
// *��    �ܣ�У׼ģ���ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ���ʼ������ָ��
//***********************************************************************/
void    CalibInit(void)
{
    Temp_Xcal.ptempval=temp_buf;            	//�¶Ȳ���ģ�����¶�ADֵ��������
    Temp_Xcal.SetCalNum=SetTempXCalCnt;     	//����У׼����	����
    Temp_Xcal.RdTempVal=GetTempXCalTempVal;  	//����������
    Temp_Xcal.RdAdcVal=GetTempXCalAdcVal;    	//������
    Temp_Xcal.WrTempVal=SetTempXCalTempVal;   	//д����
    Temp_Xcal.WrAdcVal=SetTempXCalAdcVal;     	//д��������
//  Temp_Xcal.CalcPrm=TempXCalcPrm;           	//�������A,K

    Temp_Ycal.ptempval=temp_buf;            	//�¶Ȳ���ģ�����¶�ADֵ��������
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
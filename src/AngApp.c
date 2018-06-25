/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  AngApp.c
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
#define   __ANGAPP_C__
#include "_Include.h"


static int32s xdata  angp;
static int32s xdata  angx_syszero;
static int32s xdata  angy_syszero;
static int32s xdata  angx_usrzero;
static int32s xdata  angy_usrzero;
static int32s xdata  angx_inst;     //��װ��
static int32s xdata  angy_inst;     //��װ��
static int32s xdata  angx_set;      //X�ᱨ���Ƕ�
static int32s xdata  angy_set;
static int32s xdata  angp_set;
static int32s xdata  BufZerox_usr;
static int32s xdata  BufZeroy_usr;
static int32s xdata  BufZerox_sys;
static int32s xdata  BufZeroy_sys;
//***********************************************************************/
// *��    �ܣ���ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ��ⲿ����
//***********************************************************************/
void AngInit(void)
{
    SnInit();                   //������ģ���ʼ��

    angp=0;                     //��Ƕ�
 
    SetAngXInst(0);
    SetAngYInst(0);             //���b��
    SetAngXSysZero(0);
    SetAngYSysZero(0);          //ϵ�y��0
    SetAngXUsrZero(0);          //�Ñ���0
    SetAngYUsrZero(0);
    SetAngXSet(50000);           //�󾯽Ƕ�
    SetAngYSet(50000);
    SetAngPSet(50000);           //��󾯽Ƕ�
                                //
    BufZerox_usr=0;             //�û���0����ԽǶ�ֵ
    BufZeroy_usr=0;
    BufZerox_sys=0;
    BufZeroy_sys=0;

}
//***********************************************************************/
// *��    �ܣ���Ƕȼ���
// *��    �ڣ�x= X�����ֵ y = y�����ֵ p = ��Ƕȴ洢��ַ
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static  int32s  PAngCalc (int32s angx,int32s angy)
{
    float xdata x,y,s;

    x  = angx;
    x  = x / 1800000 * PI;
    x  = sin(x);
    x *= x;

    y  = angy;
    y  = y / 1800000 * PI;
    y  = sin(y);
    y *= y;

    s  = sqrt(x+y);
    if( s >= 0.5 )//��Ƕȼ������?
    {
        return 300000;
    }
    s = 1800000 * asin(s) / PI ;
    return( (int32s)s );
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int32s GetAngP(void)
{

	angp=PAngCalc(GetXAngAllSetVal(),GetYAngAllSetVal());
    return angp;    //��ȡ��Ƕ�
}

int32s GetAngXSysZero(void)
{
    return angx_syszero;    //��ȡX��ϵͳ�������
}
int32s GetAngYSysZero(void)
{
    return angy_syszero;    //��ȡY��ϵͳ�������
}
int32s GetAngXUsrZero(void)
{
    return angx_usrzero;    //��ȡX���û��������
}
int32s GetAngYUsrZero(void)
{
    return angy_usrzero;    //��ȡY���û��������
}

int32s GetAngXInst(void)
{
    return angx_inst;    //��ȡX���û���װ�ǲ���
}
int32s GetAngYInst(void)
{
    return angy_inst;    //��ȡY���û���װ�ǲ���
}

int32s GetAngXSet(void)
{
    return angx_set;    //��ȡX���û������ǲ���
}
int32s GetAngYSet(void)
{
    return angy_set;    //��ȡY���û������ǲ���
}
int32s GetAngPSet(void)
{
    return angp_set;    //��ȡ��Ƕ��û������ǲ���
}
#if MODBUS_CHECK_ENB==1
int32s GetAngXZeroUsr(void)
{
    return  BufZerox_usr; //��ȡX����ԽǶ�ֵ�û�       
}
int32s GetAngYZeroUsr(void)
{
    return  BufZeroy_usr; //��ȡy����ԽǶ�ֵ�û�       
}
#endif
int32s GetAngXRelSys(void)
{
    return  BufZerox_sys; //��ȡX����ԽǶ�ֵ�ڲ�       
}
int32s GetAngYRelSys(void)
{
    return  BufZeroy_sys; //��ȡy����ԽǶ�ֵ�ڲ�       
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void SetAngXSysZero(int32s zore)
{
    angx_syszero=zore;    //����X��ϵͳ�������
}
void SetAngYSysZero(int32s zore)
{
    angy_syszero=zore;    //����Y��ϵͳ�������
}

void SetAngXUsrZero(int32s zore)
{
    angx_usrzero=zore;    //����X���û��������
}
void SetAngYUsrZero(int32s zore)
{
    angy_usrzero=zore;    //����Y���û��������
}

void SetAngXInst(int32s ang_inst)
{
    angx_inst=ang_inst;    //����X���û���װ�ǲ���
}
void SetAngYInst(int32s ang_inst)
{
    angy_inst=ang_inst;    //����Y���û���װ�ǲ���
}

void SetAngXSet(int32s ang_set)
{
    angx_set=ang_set;    //����X���û������ǲ���
}
void SetAngYSet(int32s ang_set)
{
    angy_set=ang_set;    //����Y���û������ǲ���
}
void SetAngPSet(int32s ang_set)
{
    angp_set=ang_set;    //������Ƕ��û������ǲ���
}


//***********************************************************************/
// *��    �ܣ����ýǶ�ֵ0λ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void SetAngZero (int32s *angval ,int32s setval)
{
    *angval=*angval-setval;
}

void SetAngZeroAdd (int32s *angval ,int32s setval)
{
    *angval=*angval+setval;
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int32s GetXAngAllSetVal(void)
{
    int32s xdata tmp;
    tmp=GetXAng();                    //��ȡX���ԭʼ�Ƕ�ֵ
    SetAngZero(&tmp,angx_syszero);    //ϵͳ��0
    BufZerox_sys=tmp;                 //ϵͳ��0����ԽǶ�ֵ
    SetAngZero(&tmp,angx_usrzero);    //�û���0
    BufZerox_usr=tmp;                 //�û���0����ԽǶ�ֵ
    SetAngZeroAdd(&tmp,angx_inst);    //��װ��
//	tmp = -14352;    
    return tmp;


}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int32s GetYAngAllSetVal(void)
{
    int32s xdata tmp;
    tmp=GetYAng();                    //��ȡX���ԭʼ�Ƕ�ֵ
    SetAngZero(&tmp,angy_syszero);    //ϵͳ��0
    BufZeroy_sys=tmp;                 //ϵͳ��0����ԽǶ�ֵ
    SetAngZero(&tmp,angy_usrzero);    //�û���0
    BufZeroy_usr=tmp;                 //�û���0����ԽǶ�ֵ
    SetAngZeroAdd(&tmp,angy_inst);    //��װ��
    return tmp;
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ� �Ƕȱ�������
//***********************************************************************/
#if	Alarm_X_Enb == 1
void  AlarmX( int8u vol )
{
    if( AlarmHighLow == 'H' )
    {
        if( vol == 'H' )
        {
            LED_X_ON() ;
        }
        else
        {
            LED_X_OFF() ;
        }
    }
    else
    {
        if( vol == 'H' )
        {
            LED_X_OFF() ;
        }
        else
        {
            LED_X_ON() ;
        }
    }
}
#endif
//***********************************************************************/
#if	Alarm_Y_Enb == 1
void  AlarmY( int8u vol )
{
    if( AlarmHighLow == 'H' )
    {
        if( vol == 'H' )
        {
            LED_Y_ON() ;
        }
        else
        {
            LED_Y_OFF() ;
        }
    }
    else
    {
        if( vol == 'H' )
        {
            LED_Y_OFF() ;
        }
        else
        {
            LED_Y_ON() ;
        }
    }
}
#endif
//***********************************************************************/
#if	Alarm_P_Enb == 1
void  AlarmS( int8u vol )
{
    if( AlarmHighLow == 'H' )
    {
        if( vol == 'H' )
        {
            LED_S_ON() ;
        }
        else
        {
            LED_S_OFF() ;
        }
    }
    else
    {
        if( vol == 'H' )
        {
            LED_S_OFF() ;
        }
        else
        {
            LED_S_ON() ;
        }
    }
}
#endif
//***********************************************************************/
#if 0
void  AngProc(void)
{
#if	Alarm_X_Enb == 1
    int32s  data_x;
#endif
#if	Alarm_Y_Enb == 1
	int32s  data_y;
#endif
#if	Alarm_X_Enb == 1
    data_x = GetXAngAllSetVal();
    if( ( data_x > ( angx_set+AngAlarmOffset ) ) || ( data_x < -( angx_set+AngAlarmOffset ) ) )
    {
        AlarmX('H') ;
    }
    else if( ( data_x < ( angx_set-AngAlarmOffset ) ) && ( data_x > -( angx_set-AngAlarmOffset ) ) )
    {
        AlarmX('L') ;
    }
#endif
#if	Alarm_Y_Enb == 1
    data_y = GetYAngAllSetVal();
    if( ( data_y > ( angy_set+AngAlarmOffset ) ) || ( data_y < -( angy_set+AngAlarmOffset ) ) )
    {
        AlarmY('H') ;
    }
    else if( ( data_y < ( angy_set-AngAlarmOffset ) ) && ( data_y > -( angy_set-AngAlarmOffset ) ) )
    {
        AlarmY('L') ;
    }
#endif
#if	Alarm_P_Enb == 1
    angp = PAngCalc(GetXAngAllSetVal(),GetYAngAllSetVal());
    if( angp > ( angp_set+AngAlarmOffset ) )
    {
        AlarmS('H') ;
    }
    else if( angp < ( angp_set-AngAlarmOffset ) )
    {
        AlarmS('L') ;
    }
#endif
}
#endif
//***********************************************************************/
//        E N D
//***********************************************************************/

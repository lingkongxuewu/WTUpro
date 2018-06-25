/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  AdcDrv.c
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
#define  __ADC_DRV_C__
#include "_Include.h"

/***********************************************/
//struct adc_entity xdata vdd_adc;
/*****************************************************/
//static int8u    code    PinIdTab[13] = {   2,   0,   1,   0,   1,   0,   1,   0,   1,   0,   1,   0,   1};
//static int8u    code    PinMuxTab[]  = {0x30,0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09};
                        //AD����ͨ��ѡ��0x0a=P1.2,0x09=P1.1,0x30=temp,0x0b=Vdd
/*****************************************************/

#if 0

static void Adc_SetInit (void);
static void Adc_VrefInit (void);
static void Adc_IsrInit (void);
//***********************************************************************/
// *��    �ܣ�ADC��ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Adc_Init (void)
{
    Adc_SetInit();
    Adc_VrefInit();
    Adc_IsrInit();
}
//***********************************************************************/
void Adc_Enable (void)
{
    ADC0CN |= 0x10;     // enable ADC0      AD0EN = 1;
}

//***********************************************************************/
// *��    �ܣ���ȡ�¶�ADCֵ
// *��    �ڣ�
// *��    �ڣ��¶�ADCֵ�������˲���
// *��    ע��
// *�������ʣ�
//***********************************************************************/
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
/*
int16u Adc_GetVdd(void)
{
    return  (vdd_adc.ftval);
}
*/
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/

static void Adc_SetInit (void)
{   int8u SfrPage_ = SFRPAGE;


    SFRPAGE = ACTIVE_PAGE;
    ADC0MX  = 0x0c;     //0x0c
    ADC0CF |= 0x01;     // Set GAINEN = 1
    ADC0H   = 0x04;     // Load the ADC0GNH address
    ADC0L   = 0x6C;     // Load the upper byte of 0x6CA to
    ADC0H   = 0x07;     // Load the ADC0GNL address
    ADC0L   = 0xA0;     // Load the lower nibble of 0x6CA to
    ADC0H   = 0x08;     // Load the ADC0GNA address
    ADC0L   = 0x01;     // Set the GAINADD bit
    ADC0CF &= ~0x01;    // Set GAINEN = 0
    ADC0CN  = 0x80;     // ADC0 disabled, normal tracking,
    REF0CN  = 0x00;     // Enable on-chip VREF and buffer
    ADC0MX  = 0x30;     // Set ADC input to Temp Sensor
    ADC0CF  = ((SYSCLK / 300000) - 1) << 3;    // Set SAR clock to 300KHz
    SFRPAGE = SfrPage_;
}
//***********************************************************************/
static void Adc_VrefInit (void)
{
    REF0CN    = 0x37;       // Enable on-chip VREF = 2.2v and buffer    ok
}
//***********************************************************************/
static void Adc_IsrInit (void)
{
    EIE1 |= 0x04;           // Enable ADC0 conversion complete int.
}

void AdcFilter_Temp(struct adc_entity *p_adc, int16u val) // reentrant
{
    if(ValInRange(p_adc->ftval,val,(int16u)FILTER_NUM))                     //�޷��˲�
    {
        p_adc->ftval=SlideWinFilter (&(p_adc->arrpt),&(p_adc->arrcnt),
                                     p_adc->valarr,val,(int16u)FILTER_NUM); //���?��??2��

    }
    else
    {
            p_adc->ftval=val;                                               //���ȱ仯��ֱ��ʹ�õ�ǰֵ
            p_adc->arrcnt=0;                                                //����д����
    }
}

//***********************************************************************/
// *��    �ܣ�ADCֵ�ı���Ͳɼ��˿�ѡ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Adc_SaveAndSwitch (int16u val)
{
    struct adc_entity *adc_ent;
    adc_ent=&temp_adc;                    //???��ADC?��11��?���������????��?����?

    AdcFilter_Temp(adc_ent,val);
    ADC0MX=0x30;
}
//***********************************************************************/
// *��    �ܣ�AD�жϴ���
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Adc_Isr (void) interrupt INTERRUPT_ADC0_EOC//9
{
    static xdata int32u sum = 0;
    static xdata int16u cnt = 0;

    AD0INT=0;                                    // clear ADC0 conv. complete flag]
    sum+=ADC0;

    if(++cnt==SUM_CNT)
    {
        Adc_SaveAndSwitch(sum/SUM_CNT1);
        sum=0;
        cnt=0;
    }
    else
        ADC0CN |= 0x10;
}
#endif

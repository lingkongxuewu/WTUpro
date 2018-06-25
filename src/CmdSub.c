/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  CmdSub.c
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
#define __CMDSUB_C__
#include "_Include.h"

static int32s xdata temp_xcalref;
static int32s xdata temp_ycalref;
static struct sensor xdata *px_axis=&accel_x;
static struct sensor xdata *py_axis=&accel_y;



//***********************************************************************/
// *��    �ܣ����ݽǶ�ֵ�滻�Ƕ�ADCֵ
// *��    �ڣ�ang,�Ƕ�ֵ��adc���Ƕ�ADCֵ;num,�����С;angarr���Ƕ�����ָ��;
//            adcarr,�Ƕ�ADCֵ����ָ��
// *��    �ڣ�TRUE���ɹ�;FALSE��ʧ��
// *��    ע��
// *�������ʣ�
//***********************************************************************/



int8u AngChkDiff(int32s ang,int32u adc,int8u num,int32s* angarr,int32u* adcarr)
{	
	int8u xdata i;
	for(i=0;i<num;i++)
	{	if(ang==angarr[i])       //���ҽǶ��������е�λ��
		{	
			adcarr[i] = adc;    //����Ӧ��λ�ò���ADCֵ
			return TRUE;
		}
	}
	return FALSE;               //���ڷ�Χ��
}

//***********************************************************************/
// *��    �ܣ�16λð����Ѷ
// *��    �ڣ�buf,������Ļ���;n,�����Ԫ�ظ���
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
/*
void Bubble16B (int16s* buf, int8u n)
{		
	int8u i,j;
	for(i=0;i<n;i++)
	{	for(j=n-1;j>i;j--)
		{	if(buf[i]>buf[j])
			{	
				buf[i]+=buf[j];
				buf[j]=buf[i]-buf[j];
				buf[i]=buf[i]-buf[j];
			}
		}
	}
}
 */
//***********************************************************************/
// *��    �ܣ������¶Ƚ����������򣬲������¶Ȳ���ϵ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void TempSortAndCalcPrm ()
{

	Sub_BufBubble2(px_axis->tempcal->ptempval,px_axis->tempcal->adcarr,py_axis->tempcal->adcarr,py_axis->tempcal->calcnt);//�²�ģ�����¶�����

	TempXCalcPrm();					//X���²���������
	TempYCalcPrm();					//Y���²��������� 
}

//***********************************************************************/
// *��    �ܣ�д���¶Ȳ�������
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Temp_CalValWr (int8u id,int16u temp,int32s x,int32s y)
{
	px_axis->tempcal->WrTempVal(temp,id);    //д�¶�ADֵ
	SetTempXCalAdcVal(x,id);
	SetTempYCalAdcVal(y,id);	   			 //д���¶Ȳ�������Ĳ�ֵad
}
//***********************************************************************/
// *��    �ܣ��²���ʼ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע������ͨ����
// *�������ʣ�
//***********************************************************************/
int8u Temp_CalSta (void)
{
	if(MenuId!=MOD_MOD0)return FALSE;
	MenuId=MOD_T_CAL;
	return TRUE;
}
//***********************************************************************/
// *��    �ܣ��²�����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u Temp_CalStop (void)
{
	if(MenuId!=MOD_T_CAL)return FALSE;
	MenuId=MOD_MOD0;
	TempSortAndCalcPrm();
	return TRUE;
}
//***********************************************************************/
int8u Temp_CalSamp (int8u id,int16u temp,int32s x,int32s y)
{
	if((MenuId!=MOD_T_CAL)||(id>TEMP_CAL_NUM))
	return FALSE;
		
	if(0==id)                                               //�Ե�һ���궨�¶�Ϊ�ο��¶�
	{
		temp_xcalref  = x;
		temp_ycalref  = y;
	};
	Sub_SendAdcTXY(temp,x,y);
	Temp_CalValWr(id,temp,(temp_xcalref-x),(temp_ycalref-y));
	return TRUE;
}
//***********************************************************************/
// *��    �ܣ������²�ģ������Բο���
// *��    �ڣ�id,�ο���λ��
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u Temp_CalTRefSet (int8u id)
{	int8u xdata i;
	int32s xdata x,y;
	
	
	if(id>(px_axis->tempcal->calcnt-1))return FALSE;
	x = px_axis->tempcal->RdAdcVal(id);
	y = py_axis->tempcal->RdAdcVal(id);
	
	for(i=0;i<(px_axis->tempcal->calcnt);i++)
	{
		px_axis->tempcal->adcarr[i] -= x;    //X�����ADֵ���Ƕ�0��
		py_axis->tempcal->adcarr[i] -= y;    //Y�����ADֵ���Ƕ�0��
	}
	MenuId=MOD_T_CAL;
	Temp_CalStop();
	return TRUE;
}



//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨��ʼ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u Adc_XCalSta (void)
{
	if(MenuId!=MOD_MOD0)return FALSE;
	MenuId=MOD_X_CAL;
	ACalXId=0;
	return TRUE;
}
//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨
// *��    �ڣ�ang,�Ƕ�ֵ;adc,�Ƕ�ADCֵ
// *��    �ڣ�TRUE,����ɹ�;FALSE��ʧ��
// *��    ע�������±궨ĳ���Ƕ�ֵ�ǣ�ԭ����ADֵ���ᱻ�滻
// *�������ʣ�
//***********************************************************************/
int8u Adc_XCalProc (int32s ang,int32u adc)
{
	if(MenuId!=MOD_X_CAL)return FALSE;
	if((ACalXId>0)&&(TRUE==AngChkDiff(ang,adc,ACalXId,px_axis->angcal->angarr,px_axis->angcal->adcarr)))
		return TRUE;    									//�滻�ɹ�
	if(ACalXId>=px_axis->angcal->calcnt) return FALSE;
	px_axis->angcal->angarr[ACalXId]=ang;    				//д����ֵ
	px_axis->angcal->adcarr[ACalXId]=adc;
	ACalXId++;
	return TRUE;
}
//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨����
// *��    �ڣ�
// *��    �ڣ��궨���� A,K
// *��    ע��
// *�������ʣ�
//***********************************************************************/

int8u Adc_XCalStop (void)
{	
	int8u n=px_axis->angcal->calcnt-1;
	
	if(ACalXId<px_axis->angcal->calcnt)  return FALSE;
	if(MenuId!=MOD_X_CAL)return FALSE;
	MenuId=MOD_MOD0;
	Sub_BufBubble(px_axis->angcal->angarr,px_axis->angcal->adcarr,px_axis->angcal->calcnt);//����Ƕ�ֵ�ͽǶ�ADֵ
//	px_axis->angcal->CalcPrm();    														   //�궨��������
	AngXCalcPrm();				   
	return TRUE;
}


//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨��ʼ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u Adc_YCalSta (void)
{
	if(MenuId!=MOD_MOD0)return FALSE;	
	MenuId=MOD_Y_CAL;
	ACalYId=0;
	return TRUE;
}
//***********************************************************************/
// *��    �ܣ�Y��Ƕȱ궨
// *��    �ڣ�ang,�Ƕ�ֵ;adc,�Ƕ�ADCֵ
// *��    �ڣ�TRUE,����ɹ�;FALSE��ʧ��
// *��    ע�������±궨ĳ���Ƕ�ֵʱ��ԭ����ADֵ���ᱻ�滻
//***********************************************************************/
int8u Adc_YCalProc (int32s ang,int32u adc)
{
	if(MenuId!=MOD_Y_CAL)return FALSE;
	if((ACalYId>0)&&(TRUE==AngChkDiff(ang,adc,ACalYId,py_axis->angcal->angarr,py_axis->angcal->adcarr)))
		return TRUE;    					//�滻�ɹ�
	if(ACalYId>=py_axis->angcal->calcnt) return FALSE;
	py_axis->angcal->angarr[ACalYId]=ang;    //д����ֵ
	py_axis->angcal->adcarr[ACalYId]=adc;
	ACalYId++;
	return TRUE;
}
//***********************************************************************/
int8u Adc_YCalStop (void)
{	
	int8u n=py_axis->angcal->calcnt-1;
	
	if(ACalYId<py_axis->angcal->calcnt)  return FALSE;
	if(MenuId!=MOD_Y_CAL)return FALSE;
	MenuId=MOD_MOD0;
	Sub_BufBubble(py_axis->angcal->angarr,py_axis->angcal->adcarr,py_axis->angcal->calcnt);//����Ƕ�ֵ�ͽǶ�ADֵ
//	py_axis->angcal->CalcPrm();    
	AngYCalcPrm();				   
	return TRUE;
}
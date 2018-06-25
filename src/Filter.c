/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Filter.c
* �桡����  v1.0
* ��  ����  �˲�����   100T˫�� ���Ϻ�İ汾
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
#include "Filter.h"

//***********************************************************************/
// *��    �ܣ��޷��˲�
// *��    �ڣ�old_val��ǰһ�ε�ֵ��new_val����ֵ��range����Χ
// *��    �ڣ�����ֵ�ڷ�����ʱ����1�����򷵻�0
// *��    ע������Ϊold_val-range<new_val<old_val+range
// *�������ʣ�
//***********************************************************************/
int8u ValInRange(int32u old_val,int32u new_val,int32u range) reentrant
{
	if((new_val>(old_val+range))||((new_val+range)<old_val))
		return 0;
	else
		return 1;
}

#if 0
int8u ValOutRange(int32u old_val,int32u new_val,int32u range) reentrant
{
	if((new_val>(old_val+range))||((new_val+range)<old_val))
		return 1;
	else
		return 0;
}
#endif
//***********************************************************************/
// *��    �ܣ����������˲�
// *��    �ڣ�pbufpos,����λ��ָ��;pbufcnt,������Ԫ�ظ���ָ��;pbuf,����ָ��;
//						val,��ֵ;ftcnt�����ڳ��ȣ�Ԫ�ظ�����
// *��    �ڣ���������������֮�󷵻�ƽ��ֵ�����򷵻ش���ֵ
// *��    ע����֤ftcntС�ڵ��ڻ���ָ����ָ�������Ĵ�С
// *�������ʣ�
//***********************************************************************/
int32u SlideWinFilter (int8u *pbufpos, int8u *pbufcnt, int32u *pbuf, int32u val, int16u ftcnt) reentrant
{
	int32u data_ram=0;
	int8u i;
	*(pbuf+*pbufpos)=val;    		//�滻��꣨λ�ã����ڵ�Ԫ��
	(*pbufpos)++;           		//������
	if(ftcnt==*(pbufpos))
		*pbufpos=0;			     	//��굽�ﴰ��ĩβ�����»ص���ʼλ��
	if((*pbufcnt)!=ftcnt)
	{	
		(*pbufcnt)++;				//��������δ��
		return val;
	}
	else 
	{
		for(i=0;i<ftcnt;i++)
		{	
			data_ram+=*(pbuf+i);
		}
		data_ram=data_ram/ftcnt;
		return ((int32u)data_ram);			
	}
}
//***********************************************************************/
// *��    �ܣ�һ���˲�
// *��    �ڣ�old_val��ǰһ���˲�ֵ��new_val����ֵ��filtercnt���˲�ϵ��
// *��    �ڣ�һ���˲�������ֵ
// *��    ע��0<filtercnt<256
// *�������ʣ�
//***********************************************************************/

float Filter_1st (float old_val,int32u new_val, int8u filtercnt)
{	
	return ((old_val*(filtercnt-1)+new_val)/filtercnt);
}

/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Filter.h
* �桡����  v1.0
* ��  ����  �˲����� 100T˫�� ���Ϻ�İ汾
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
//***********************************************************************/
#ifndef __FILTER_H__
#define __FILTER_H__
#include	"_Type.h"

	/*-----�궨��---------------------*/
//#define FQUEUE_LONG 9

	/*-----�ṹ���������Ͷ���---------*/
	/*
typedef struct
{
	int8u front;
	int8u rear;
	int32 qf_arr[FQUEUE_LONG];
}QF_SILWIN;
*/


	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	int8u ValInRange(int32u ref_val,int32u new_val,int32u range) reentrant;
	int8u ValOutRange(int32u old_val,int32u new_val,int32u range) reentrant;
	int32u SlideWinFilter (int8u *pbufpos, int8u *pbufcnt, int32u *pbuf, int32u val, int16u ftcnt) reentrant;
	float Filter_1st (float ref_val,int32u new_val, int8u filtercnt);
//	void Init_QF(void);		   
	
#endif
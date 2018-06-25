/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  AdcDrv.h
* �桡����  v1.0
* ��  ����  ADC�ײ�����ģ��	100T˫�� ���Ϻ�İ汾
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
	#include "_Type.h"
	#ifndef __ADC_DRV_H__
	#define __ADC_DRV_H__
	#include <compiler_defs.h>
	#include <c8051F500_defs.h>

//#include "_Include.h"
	
	

	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	void Adc_Init (void);
	void Adc_Enable (void);	
	int32u Adc_GetXVal(void);
	int32u Adc_GetYVal(void);
//	int16u Adc_GetTemp(void);
//	int16u Adc_GetVdd(void);


/***********************************************************************/
#ifdef   __ADC_DRV_C__
#define  ADC_DRV_EXT
#else
#define  ADC_DRV_EXT  extern
#endif
ADC_DRV_EXT int8u idata	AdcEndTag;
ADC_DRV_EXT int8u idata	AdcEndId;
#endif

	

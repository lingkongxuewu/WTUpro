/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Calibration.h
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
#include	"_Type.h"
#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

/***********************************************************************/
/*-----------------------------�궨��----------------------------------*/

#define	TEMP_CAL_NUM 21
#define	ANG_CAL_NUM  25     
/*-------------------------�ṹ���������Ͷ���--------------------------*/
	struct temp_calib{
		int8u calcnt;    				            //�¶Ȳ��������
		int16u *ptempval;    			            //temp����
		int32s  adcarr[TEMP_CAL_NUM];                //�Ƕ�ADC��������
		float prm_a[TEMP_CAL_NUM-1];                //����A��������
		float prm_k[TEMP_CAL_NUM-1];                //����K��������
		void (*SetCalNum)(int8u num);               //����У׼����
		int16u (*RdTempVal)( int8u id);             //����������
		int32u (*RdAdcVal)(int8u id);               //������
		void (*WrTempVal)(int16u val, int8u id);    //д����
		void (*WrAdcVal)(int32s val, int8u id);     //д��������
//		void (*CalcPrm)(void);                      //�������A,K
	};
	

	struct ang_calib{
		int8u calcnt;                               //�Ƕ�У׼�����
		int32s  angarr[ANG_CAL_NUM];                 //ang��������
		int32u  adcarr[ANG_CAL_NUM];                 //ADC��������
		float prm_a[ANG_CAL_NUM-1];                 //����A��������
		float prm_k[ANG_CAL_NUM-1];                 //����K��������
		void (*SetCalNum)(int8u num);               //����У׼����
		 int32s  (*RdAngVal)( int8u id);              //����������
		 int32u  (*RdAdcVal)(int8u id);               //������
		void (*WrAngVal)(int32s val, int8u id);     //д����
		void (*WrAdcVal)(int32u val, int8u id);     //д��������
//		void (*CalcPrm)(void);                      //�������A,K
	};

		

	/*-----���������ȣ�����---------*/
	
	/*-----��������-------------------*/
	int8u GetTempXCalCnt(void);                    //��ȡX���¶Ȳ�����ĸ���
	int8u GetAngXCalCnt(void);                     //��ȡX��Ƕ�У׼��ĸ���
	
//	int8u GetTempYCalCnt(void);                    //��ȡY���¶Ȳ�����ĸ���
	int8u GetAngYCalCnt(void);                     //��ȡY��Ƕ�У׼��ĸ���
	
	void SetTempXCalCnt(int8u cnt);                //����X���¶Ȳ�����ĸ���
	void SetAngXCalCnt(int8u cnt);                 //����X��Ƕ�У׼��ĸ���
	
	void SetTempYCalCnt(int8u cnt);                //����Y���¶Ȳ�����ĸ���
	void SetAngYCalCnt(int8u cnt);                 //����Y��Ƕ�У׼��ĸ���
	
	void SetTempCalCnt(int8u cnt);                 //�����¶Ȳ�����ĸ���
	void SetAngCalCnt(int8u cnt);                  //���ýǶ�У׼��ĸ���
	
	
	int16u GetTempXCalTempVal(int8u pos);          //��ȡX���¶Ȳ������¶������е�ĳ���¶�ֵ
	int32s GetAngXCalAngVal(int8u pos);            //��ȡX��Ƕ�У׼��У׼�����е�ĳ���Ƕ�ֵ
	
	int16u GetTempYCalTempVal(int8u pos);          //��ȡY���¶Ȳ������¶������е�ĳ���¶�ֵ
	int32s GetAngYCalAngVal(int8u pos);            //��ȡY��Ƕ�У׼��У׼�����е�ĳ���Ƕ�ֵ
	
	int32u GetTempXCalAdcVal(int8u pos);           //��ȡX���¶Ȳ�����ADC�����е�ĳ��ADֵ
	int32u GetAngXCalAdcVal(int8u pos);            //��ȡX��Ƕ�У׼��ADC�����е�ĳ��ADֵ
	
	int32u GetTempYCalAdcVal(int8u pos);
	int32u GetAngYCalAdcVal(int8u pos);
	
	
	void SetTempXCalTempVal(int16u val, int8u pos); //����X���¶Ȳ������¶������е�ĳ���¶�ֵ
	void SetAngXCalAngVal(int32s val, int8u pos);   //����X��Ƕ�У׼��У׼�����е�ĳ���Ƕ�ֵ
	
	void SetTempYCalTempVal(int32s val, int8u pos);
	void SetAngYCalAngVal(int32s val, int8u pos);
	
	void SetTempXCalAdcVal(int32s val, int8u pos);  //����X���¶Ȳ�����ADC�����е�ĳ��ADֵ
	void SetAngXCalAdcVal(int32u val, int8u pos);   //����X��Ƕ�У׼��ADC�����е�ĳ��ADֵ
	
	void SetTempYCalAdcVal(int32u val, int8u pos);
	void SetAngYCalAdcVal(int32u val, int8u pos);

	void TempCalDataInit();
	void AngCalDataInit();

	void TempXCalcPrm(void);
	void TempYCalcPrm(void);
	void AngXCalcPrm(void);
	void AngYCalcPrm(void);

	void	CalibInit(void);
//	float GetAngXaVal(int8u pos);
#endif

#ifdef   __CALIBR_C__
#define  CALIBR_EXT
#else
#define	 CALIBR_EXT  extern
#endif
CALIBR_EXT struct temp_calib xdata Temp_Xcal;
CALIBR_EXT struct temp_calib xdata Temp_Ycal;
CALIBR_EXT struct ang_calib  xdata Ang_Xcal;
CALIBR_EXT struct ang_calib  xdata Ang_Ycal;	
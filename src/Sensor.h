/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Sensor.h
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
#include "_Type.h"
#ifndef __SENSOR_H__
#define __SENSOR_H__


/***********************************************************************/

	/*-----�궨��---------------------*/

	/*-----�ṹ���������Ͷ���---------*/
	
	struct sensor{
		int32u val;									//ԭʼֵ�������˲���
		float  ftval_f;								//�˲�֮���ֵ(�м������������һ���˲���Filter_1st)
		int32u ftval;								//�˲�֮���ADֵ
		int32s oftval;   							//�²�֮���ADֵ
		int32s ang;   								//�Ƕ�(�����²���
		struct temp_calib *tempcal;				    //ָ��temp_calib�ṹ��ָ��
		struct ang_calib *angcal;					//ָ��ang_calib�ṹ��ָ��
		void (*DataInit)();						    //����ָ��
		void (*Filter)(int32u val, int8u ftcnt);	//ָ���˲�����
		int32s (*GetTempOffset)(int16u temp);		// ָ���ȡ�²��ĽǶ�ֵ����
		void (*AngCalc)();							//����ָ��ָ��Ƕȼ��㺯��		 
	};

	/*-----���������ȣ�����---------*/
	
	/*-----��������-------------------*/
	void SnInit(void);    //
	
	void SnProc(void);    //
	
	int8u GetFilterCnt(void);
	void SetFilterCnt(int8u cnt);
	
	int16u Get_McuTemp(void);
	
	int32u GetXFtVal(void);    //һ���˲����ֵ
	int32u GetYFtVal(void);    //
	
	int32s GetXOftVal(void);   //�²�֮���ֵ
	int32s GetYOftVal(void);
	
	int32s GetXAng(void);     //�Ƕ�ֵ
	int32s GetYAng(void);


#endif
/***********************************************************************/
#ifdef   __SENSOR_C__
#define  SENSOR_EXT
#else
#define	 SENSOR_EXT  extern
#endif
SENSOR_EXT struct sensor xdata accel_x;		  //���ٶȴ�����X
SENSOR_EXT struct sensor xdata accel_y;		  //���ٶȴ�����Y
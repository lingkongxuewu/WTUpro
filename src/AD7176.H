/********************************************************************************
 Author : Sijo (cic.asia@analog.com)

 Date : April 2014

 File name : AD7176.h

 Description : MACRO definition for AD7176

 Hardware plateform : ADuCM360 + AD7176
********************************************************************************/

#ifndef __AD7176_H__
#define __AD7176_H__

#include  "_Type.h"

#define CS	0x13	//P1.3 external Chip Select

#define PIN_HIGH 0x1
#define PIN_LOW  0x0

/*Operation Mode */

#define CONCONVMODE  0x00
#define CONREADMODE  0x01
#define SINGCONVMODE 0x02
#define STDBYMODE    0x03
#define CALMODE      0x04

/* COMMUNICATION REGISTER */

#define WRITE_EN     0x00
//#define WRITE        0x00
//#define READ         0x40

#define REG_STATUS   0x00
#define REG_ADCMODE  0x01
#define REG_IFMODE   0x02
#define REG_CHECK    0x03
#define REG_DATA     0x04
#define REG_GPIOCON  0x06
#define REG_ID       0x07

#define REG_CHMAP0  0x10
#define REG_CHMAP1  0x11
#define REG_CHMAP2  0x12
#define REG_CHMAP3  0x13

#define REG_SETUPCON0  0x20
#define REG_SETUPCON1  0x21
#define REG_SETUPCON2  0x22
#define REG_SETUPCON3  0x23

#define REG_FILTCON0  0x28
#define REG_FILTCON1  0x29
#define REG_FILTCON2  0x2A
#define REG_FILTCON3  0x2B

#define REG_OFFSET0  0x30
#define REG_OFFSET1  0x31
#define REG_OFFSET2  0x32
#define REG_OFFSET3  0x33

#define REG_GAIN0  0x38
#define REG_GAIN1  0x39
#define REG_GAIN2  0x3A
#define REG_GAIN3  0x3B

/* ADC MODE REGISTER */

#define REF_EN               0x8000
#define REF_DIS              0x0000

#define SING_CYC_EN          0x2000
#define SING_CYC_DIS         0x0000

#define DELAY_0US            0x0000
#define DELAY_4US            0x0100
#define DELAY_168US          0x0200
#define DELAY_40US           0x0300
#define DELAY_100US          0x0400
#define DELAY_200US          0x0500
#define DELAY_500US          0x0600
#define DELAY_1MS            0x0700

#define MODE_CONT_CONV       0x0000
#define MODE_SING_CONV       0x0010
#define MODE_STANDBY         0x0020
#define MODE_PD_DWN          0x0030
#define MODE_INT_OFFSET_CAL  0x0040
#define MODE_SYS_OFFSET_CAL  0x0060
#define MODE_SYS_GAIN_CAL    0x0070

#define CLOCKSEL_INTOS   0x0000
#define CLOCKSEL_INTOSO  0x0004
#define CLOCKSEL_EXTCLK  0x0008
#define CLOCKSEL_EXTOS   0x000C

/* INTERFACE MODE REGISTER */

#define ALTSYNC_DIS     0x0000
#define ALTSYNC_EN      0x1000

#define IOSTRENGTH_DIS  0x0000
#define IOSTRENGTH_EN   0x0800

#define DOUT_RESET_DIS  0x0000
#define DOUT_RESET_EN   0x0100

#define CONTREAD_DIS    0x0000
#define CONTREAD_EN     0x0080

#define DATA_STAT_DIS   0x0000
#define DATA_STAT_EN    0x0040

#define REG_CHECK_DIS   0x0000
#define REG_CHECK_EN    0x0020

#define CRC_DIS  	      0x0000
#define CRC_EN_XOR      0x0004
#define CRC_EN          0x0008

#define WL16            0x0001
#define WL24            0x0000


/* GPIO CONFIGURATION REGISTER */

#define MUX_IO_DIS 		0x0000
#define MUX_IO_EN  		0x1000

#define SYNC_DIS   		0x0000
#define SYNC_EN    		0x0800

#define ERR_DIS    		0x0000
#define ERR_IN     		0x0200
#define ERR_OD_EO  		0x0400
#define ERR_GPO    		0x0600

#define ERR_DAT    		0x0100
#define ERR_DAT_DIS		0x0000

#define IP_EN1     		0x0020
#define IP_EN1_DIS    0x0000

#define IP_EN0    		0x0010
#define IP_EN0_DIS 		0x0000

#define OP_EN1    		0x0008
#define OP_EN1_DIS		0x0000

#define OP_EN0    		0x0004
#define OP_EN0_DIS		0x0000

#define GP_DATA1_HIGH 0x0002
#define GP_DATA1_LOW  0x0000

#define GP_DATA0_HIGH 0x0001
#define GP_DATA0_LOW  0x0000

/* CHANNEL MAP REGISTER 0 */

#define CH_EN_DIS  0x0000
#define CH_EN      0x8000

#define SETUP0_SEL  0x0000
#define SETUP1_SEL  0x1000
#define SETUP2_SEL  0x2000
#define SETUP3_SEL  0x3000

#define AINPOS_AIN0      0x0000
#define AINPOS_AIN1      0x0020
#define AINPOS_AIN2      0x0040
#define AINPOS_AIN3      0x0060
#define AINPOS_AIN4      0x0080
#define AINPOS_REF_P     0x02A0
#define AINPOS_REF_N     0x02C0

#define AINNEG_AIN0      0x0000
#define AINNEG_AIN1      0x0001
#define AINNEG_AIN2      0x0002
#define AINNEG_AIN3      0x0003
#define AINNEG_AIN4      0x0004
#define AINNEG_REF_P     0x0015
#define AINNEG_REF_N     0x0016

/* SETUP CONFIGURATION REGISTER 0 */

#define UNI_POLAR0  0x0000
#define BI_POLAR0   0x1000

#define REF_SEL0_EXT  0x0000
#define REF_SEL0_INT   0x0020
#define REF_SEL0_DIAG  0x0030

/* FILTER CONFIGURATION REGISTER 0 */

#define SINC3MAP0_DIS 0x0000
#define SINC3MAP0_EN  0x8000

#define ENHFILT0_DIS 0x0000
#define ENHFILT0_EN  0x0800

#define ENFILT0_NO         0x0000
#define ENFILT0_27SPS    0x0200
#define ENFILT0_25SPS       0x0300
#define ENFILT0_20SPS       0x0500
#define ENFILT0_16_67SPS    0x0600

#define ORDER0_SINC5_SINC1  0x0000
#define ORDER0_SINC3        0x0060

#define ODR0_250000SPS      0x0000
#define ODR0_125000SPS      0x0001
#define ODR0_62500SPS       0x0002
#define ODR0_50000SPS       0x0003
#define ODR0_31250PS        0x0004
#define ODR0_25000SPS       0x0005
#define ODR0_15625SPS       0x0006
#define ODR0_10000SPS       0x0007
#define ODR0_5000SPS       	0x0008
#define ODR0_2500SPS       	0x0009
#define ODR0_1000SPS       	0x000A
#define ODR0_500SPS       	0x000B
#define ODR0_397_5SPS       0x000C
#define ODR0_200SPS       	0x000D
#define ODR0_100SPS         0x000E
#define ODR0_59_94SPS       0x000F
#define ODR0_49_96SPS       0x0010
#define ODR0_20SPS         	0x0011
#define ODR0_16_667SPS      0x0012
#define ODR0_10SPS         	0x0013
#define ODR0_5SPS         	0x0014

/*-----宏定义---------------------*/
	#define	SUM_CNT			32	
	#define FILTER_NUM1     32
	#define FILTER_NUM 		35		 //窗口滤波
	#define FILTER_XY		50 //250
	#define PERIOD 5   //5*2.25us
	
	/*-----结构体数据类型定义---------*/
	struct adc_entity{
		int8u num;					  //ADC端口号
		int8u arrpt;				  //缓存数组指针
		int8u arrcnt;				  //存入数组元素个数
		int32u valarr[FILTER_NUM];	  //缓存数组
		int32u ftval;	  			  //滤波后的值
		struct adc_entity *next;	  //ADC链表的下一个元素
//		void (*Filter)(int16u adc_val);	//滤波函数指针
	};
	typedef struct
	{
		int32u sum;
		int16u cnt;
	}Sam_AD;


void AD7176_ADCModeConfig(int16u RefEn,int16u SingCycEn,int16u Delay, int16u OpMode,int16u ClockSel);
void AD7176_INTModeConfig(int16u AltSyncEn, int16u IostrengthEn,int16u DoutReset, 
											 int16u ContReadEn, int16u DataStat, int16u RegCheckEn,
											 int16u CrcEn,int16u WordLen);
void AD7176_ChannelConfig(int8u RegChanSel,int16u ChanEn,int16u SetupSelNumber, int16u AinPos, int16u AinNeg);
void AD7176_SetUpConfig(int8u RegSetupConfigNumber, int16u BiUniPolar, int16u RefSel);
void AD7176_FilterConfig(int8u RefFilConfigNumber,int16u Sinc3Map, int16u EnFiltEn, int16u EnFilSel,int16u Order, int16u outputdatarate);
void AD7176_OffsetConfig(int8u RegOffsetSel,int32u OffestVal);
void AD7176_GainConfig(int8u RegGainSel,int32u GainVal);
void AD7176_Init(void);
void AD7176_RdRegData(int8u RegAdd,int8u NumOfBytes);
//int8u CrcPolynomial(int32u Data, int8u Command, int8u NoOfBytes);
void AD7176_IntProc(void);
void AD7176_InMainProc(void);
void AdcEntityInit(void);
void AdcFilter (struct adc_entity *p_adc, int32u val);
void Adc_Save(int32u val);
int8u Adc_GetTemp(void);
int32u Get_XSample(void);
int32u Get_YSample(void);




#ifdef   __AD7176_C__
#define  AD7176_EXT
#else
#define  AD7176_EXT  extern
#endif 

AD7176_EXT	int8u 	xdata	Adc_Flag ;
AD7176_EXT 	int32u	xdata	conf_test;
AD7176_EXT 	int32u	xdata	mode_test;
AD7176_EXT  int8u   xdata   AD_Data[5];
AD7176_EXT struct adc_entity xdata x_axis_adc;
AD7176_EXT struct adc_entity xdata y_axis_adc;
AD7176_EXT struct adc_entity xdata temp_adc;

AD7176_EXT int8u xdata state;

#endif

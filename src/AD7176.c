
#define  __AD7176_C__
#include "_Include.h"

volatile int32u xdata sample;

int16u xdata WordLen;
int16u xdata WrRegVal;

Sam_AD xdata q1;
Sam_AD xdata q2;

void Init_Samad(void)
{	
	q1.sum = q1.cnt = 0;
	q2.sum = q2.cnt = 0;
}


//***********************************************************************/
// *功    能：获取X轴ADC值	  
// *入    口：
// *出    口：X轴ADC值（窗口滤波后）
// *备    注：
// *函数性质：
//***********************************************************************/
int32u Adc_GetXVal(void)
{
    return  (x_axis_adc.ftval);
}
//***********************************************************************/
// *功    能：获取Y轴ADC值
// *入    口：
// *出    口：Y轴ADC值（窗口滤波后）
// *备    注：
// *函数性质：
//***********************************************************************/
int32u Adc_GetYVal(void)
{
    return  (y_axis_adc.ftval);
}


//***********************************************************************/
// *功    能：复位AD7176-2
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void AD7176_Reset(void)
{
	int8u xdata Regarr[] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	NSSMD0 = 0;				//the value of NSS is controlled by SPI0CN.2
	SPI_Write(Regarr,8);
	NSSMD0 = 0;
}


//***********************************************************************/
// *功    能：初始化AD7176-2，写入寄存器缺省值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void AD7176_Init(void)
{
	
	AD7176_Reset();
	/* Allow at least 500 us before accessing any of the on-chip registers. */
    delay500us();
		
	AD7176_OffsetConfig (REG_OFFSET0, 0x7FFF86);	 //0x30
	
//	AD7176_INTModeConfig (ALTSYNC_DIS, IOSTRENGTH_DIS, DOUT_RESET_DIS, CONTREAD_EN, DATA_STAT_EN, REG_CHECK_DIS, CRC_DIS, WL24);	  //0x02--0x0040
	AD7176_ADCModeConfig(REF_EN, SING_CYC_DIS, DELAY_0US, MODE_CONT_CONV, CLOCKSEL_INTOS);   //0x01--0x8000
	AD7176_GainConfig (REG_GAIN0, 0x5556A0);		//0x38


	AD7176_ChannelConfig (REG_CHMAP0, CH_EN, SETUP0_SEL, AINPOS_AIN0, AINNEG_AIN1);	   //0x10--0x8001
//	AD7176_ChannelConfig (REG_CHMAP1, CH_EN, SETUP0_SEL, AINPOS_AIN2, AINNEG_AIN3);	   //0x11--0x8043
	AD7176_ChannelConfig (REG_CHMAP1, CH_EN, SETUP0_SEL, AINPOS_AIN3, AINNEG_AIN2);
	AD7176_SetUpConfig (REG_SETUPCON0, BI_POLAR0, REF_SEL0_EXT);			   	//0x20--0x1000
	AD7176_FilterConfig (REG_FILTCON0, SINC3MAP0_DIS, ENHFILT0_EN, ENFILT0_27SPS, ORDER0_SINC5_SINC1, ODR0_62500SPS);   //0x28--0x0A00
	AD7176_INTModeConfig (ALTSYNC_DIS, IOSTRENGTH_DIS, DOUT_RESET_DIS, CONTREAD_EN, DATA_STAT_EN, REG_CHECK_DIS, CRC_DIS, WL24);	  //0x02--0x0040


	AdcEntityInit();

	Init_Samad();
	
}

void AD7176_WrRegVal(int8u RegAdd, int32u RegVal, int8u NumOfBytes)
{
	int8u  mid_arr[5]={0};										   
	int8u  i;
	int32u temp;

	mid_arr[0] = RegAdd;
	
	NSSMD0 = 0;				//Activate Slave Select
	SPI_Write(mid_arr,1);	//Write at communication Register

	
	temp=RegVal;					
	for(i=1;i<=NumOfBytes;i++)
	{
		mid_arr[i]=(int8u)(temp>>(8*(NumOfBytes-i)));
	}
	SPI_Write(&mid_arr[1],NumOfBytes);
	

    NSSMD0 = 0;
//	NSSMD0 = 1;   			//Deactivate Slave Select
}

void AD7176_ADCModeConfig(int16u RefEn,int16u SingCycEn,int16u Delay, int16u OpMode,int16u ClockSel)
{
	WrRegVal = RefEn + SingCycEn + Delay + OpMode + ClockSel;
	AD7176_WrRegVal(REG_ADCMODE,WrRegVal,2);
}

void AD7176_INTModeConfig(int16u AltSyncEn, int16u IostrengthEn,int16u DoutReset, 
											 int16u ContReadEn, int16u DataStat, int16u RegCheckEn,
											 int16u CrcEn,int16u WordLen)
{
	WrRegVal = AltSyncEn + IostrengthEn + DoutReset + ContReadEn + DataStat + RegCheckEn + CrcEn + WordLen;
	
	WordLen=WL16;	//
	AD7176_WrRegVal(REG_IFMODE,WrRegVal,2);
}

void AD7176_ChannelConfig(int8u RegChanSel,int16u ChanEn,int16u SetupSelNumber, int16u AinPos, int16u AinNeg)
{
	WrRegVal = ChanEn|SetupSelNumber|AinPos|AinNeg;
	AD7176_WrRegVal (RegChanSel, WrRegVal, 2);
		
}

void AD7176_SetUpConfig(int8u RegSetupConfigNumber, int16u BiUniPolar, int16u RefSel)
{
	WrRegVal = BiUniPolar + RefSel;
	AD7176_WrRegVal(RegSetupConfigNumber, WrRegVal, 2);
}

void AD7176_FilterConfig(int8u RefFilConfigNumber,int16u Sinc3Map, int16u EnFiltEn, int16u EnFilSel,int16u Order, int16u outputdatarate)
{
	WrRegVal = Sinc3Map + EnFiltEn + EnFilSel + Order + outputdatarate;
	AD7176_WrRegVal(RefFilConfigNumber, WrRegVal, 2);
}

void AD7176_OffsetConfig(int8u RegOffsetSel,int32u OffestVal)
{
	AD7176_WrRegVal(RegOffsetSel, OffestVal, 3);
}

void AD7176_GainConfig(int8u RegGainSel,int32u GainVal)
{
	AD7176_WrRegVal(RegGainSel, GainVal, 3);
}


//Used to read Data from any registers. Requires Register Address and Expected number of Bytes

//void AD7176_RdRegData(int8u RegAdd,int8u NumOfBytes)
//{
//	
//	NSSMD0 = 0;			 				//Activate slave select
//	AD_Data[0] = RegAdd|0x40;
//	SPI_Write_INT(AD_Data,1);  				//Write at communication register.
//
//	SPI_Read(&AD_Data[1],NumOfBytes);
//	#if AD_DEBUG
//	Debug_Displayad(RegAdd,NumOfBytes);
//	#endif
//
//	sample = AD_Data[1] << 1;
//	sample = (sample << 9)|(AD_Data[2]<<2)|(AD_Data[3]>>6);			  //18位	
//
//	state = 1;
//	NSSMD0 = 0;	
//}

//#if 0
//int32u Get_XSample()
//{
//	if((AD_Data[4]&0x03)==0)
//	return 	sample;
//	else
//	return 0;
//}
//
//int32u Get_YSample()
//{
//	if((AD_Data[4]&0x03)==1)
//	return 	sample;
//	else
//	return 0;
//}
//#endif

//***********************************************************************/
// *功    能：ADC值滤波
// *入    口：p_adc,ADC结构体指针（X,Y,Temp);val,新值
// *出    口：
// *备    注：为了提高响应速度，进行限幅滤波   ADC值底层滤波供X/Y/T的函数调用
// *函数性质：
//***********************************************************************/
void AdcFilter (struct adc_entity *p_adc, int32u val)
{
    if(ValInRange(p_adc->ftval,val,(int32u)FILTER_NUM1))                     //限幅滤波
    {
        p_adc->ftval=SlideWinFilter (&(p_adc->arrpt),&(p_adc->arrcnt),
                                     p_adc->valarr,val,(int16u)FILTER_NUM); //窗口滤波
    }
    else //if(ValOutRange(p_adc->ftval,val,(int32u)FILTER_OUT))
    {
        p_adc->ftval=val;                                               //幅度变化大，直接使用当前值
        p_adc->arrcnt=0;                                                //重新写数组
    }
																		    
}

int8u Adc_GetTemp(void)
{
    return temp_adc.ftval;		//sensor temperature
}

void AdcEntityInit(void)
{
    //---------X轴
    x_axis_adc.num=0;                 //ADC虚拟端口号
    x_axis_adc.arrpt=0;               //ADC缓存数组位置指针
    x_axis_adc.arrcnt=0;              //元素个数为0
    x_axis_adc.ftval=0;               //ADC滤波之后的值
    x_axis_adc.next=&y_axis_adc;      //ADC结构体链表初始化
    //---------Y轴
    y_axis_adc.num=1;
    y_axis_adc.arrpt=0;
    y_axis_adc.arrcnt=0;              //元素个数为0
    y_axis_adc.ftval=0;
    y_axis_adc.next=&temp_adc;
    //------------温度
    temp_adc.num=2;
    temp_adc.arrpt=0;
    temp_adc.arrcnt=0;                //元素个数为0
    temp_adc.ftval=0;
    temp_adc.next=(struct adc_entity *)0;  //链表结束
}

//***********************************************************************/
void AD7176_InMainProc(void)
{
  	struct adc_entity *adc_ent;
  	adc_ent=&x_axis_adc; 
                 
	if(state != 1) 
	return;

	Adc_Flag = AD_Data[4]&0x03;
	if(Adc_Flag==1)				
	{
		q1.sum+=sample;
		while(++q1.cnt==SUM_CNT)
		{
			AdcFilter(adc_ent,q1.sum/SUM_CNT);
			q1.sum=0;
			q1.cnt=0;
		}
	}
	else if(Adc_Flag==0)
	{
		q2.sum+=sample;
		while(++q2.cnt==SUM_CNT)
		{
			AdcFilter(adc_ent->next,q2.sum/SUM_CNT);
			q2.sum=0;
			q2.cnt=0;
		}
	}
	else
		return;
	
	state = 0;
	AdcEndTag = 1;
}



void AD7176_IntProc(void)
{

	SPI_Read(&AD_Data[1],4);

	sample = AD_Data[1] << 1;
	sample = (sample << 9)|(AD_Data[2]<<2)|(AD_Data[3]>>6);
	state = 1;
}


//#if 0
//int8u CrcPolynomial(int32u Data, int8u Command, int8u NoOfBytes)
//{
//	int32u remainder;   
//    int32u polynomial=0x107;
//	int32u polynomialShift;
//	int32u AD7176Data;
//	int32u dummyData = 0x80000000;
//	
//	int8u bit_num=31;
//	int8u checksum;
//	
//	AD7176Data = Command;
//	AD7176Data <<= 8*NoOfBytes;
//	AD7176Data |=Data;
//	
//  remainder  = (AD7176Data & 0XFFFFFF00);
//	
//	while (remainder > 0xFF)
//	{
//		while ((remainder & dummyData) == 0)
//		{
//			bit_num = bit_num - 1;
//			dummyData = dummyData >> 1;
//		}
//		polynomialShift = polynomial << (bit_num - 8);
//		remainder ^= polynomialShift;
//	}	
//		checksum = remainder;
//		
//		return checksum;
//}
//
//int8u CrcXor(int32u Data, int8u Command, int8u NoOfBytes)
//{
//	int8u Bytes = 0x00;
//	int8u XorResult = 0X00;
//	int8u SampleData = 0x00;
//		XorResult = Command;
//	for(Bytes = (NoOfBytes); Bytes > 1; Bytes--)
//	{
//		SampleData = (int8u)(Data>>(8*(Bytes-1)));
//		XorResult = XorResult^SampleData;
//	}
//
//	return XorResult;
//}
//#endif








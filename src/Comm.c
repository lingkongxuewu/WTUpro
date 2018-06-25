/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Comm.c
* 版　本：  v1.0
* 描  述：  100T双轴 整合后的版本
* 创建人：   
* 日  期：   
--------------------------------------------------------------------------
*当前版本:	v1.0
*修 改 人:
*日　  期:
----------------------------------修改履历--------------------------------
1、
2、
3、
4、
------------------------------------------------------------------------*/
#define   __COMM_C__

#include "_Include.h"

//***********************************************************************/
/*-----变量定义-------------------*/
static int16u  xdata	CommTimer;

int32u xdata sample3;

//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Comm_Isr (void) interrupt INTERRUPT_UART0
{	
	int8u temp;
	int8u err;
		
	if(TI0==1)								// Check if transmit flag is set
	{	
		TI0=0;								// Clear interrupt flag
		temp=Comm_GetTxChar(&err);
		if(COMM_TX_EPT!=err)
		{	
 			SBUF0 = temp;					// Transmit to Hyperterminal
			CommTxFlag = 1;
		}
		else
		{	
			Comm_SendStop();
			CommTxFlag = 0;
		}

	}

	if(RI0==1)
	{
		RI0=0;								//Clear interrupt flag
		temp=SBUF0;							// Read a character from UART	
		#ifdef	COMM_485
		if(CommTxFlag == 0)					
		#endif
		{
			Comm_PutRxChar(temp);
			Comm_TimerInit();
//			CommRxFlag = 1;
		}
	}
	
	if(OVR0 == 1)
	{	OVR0 = 0;
	}
}
//***********************************************************************/
void Comm_HardwareInit (void)
{	int8u SfrPage_ = SFRPAGE;
	
	
	SFRPAGE = CONFIG_PAGE;
	SCON0 = 0x10;							  
	// Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

	#if	((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 1)
			SBRL0 = -(SYSCLK / BAUDRATE / 2);
			SBCON0 |= 0x03;						 // Set prescaler to 1*/
	#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 4)
			SBRL0 = -(SYSCLK / BAUDRATE / 2 / 4);
			SBCON0 &= ~0x03;
			SBCON0 |= 0x01;						 // Set prescaler to 4
	#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 12)
			SBRL0 = -(SYSCLK / BAUDRATE / 2 / 12);
			SBCON0 &= ~0x03;					  // Set prescaler to 12
	#else
			SBRL0 = -(SYSCLK / BAUDRATE / 2 / 48);
			SBCON0 &= ~0x03;
			SBCON0 |= 0x02;						 // Set prescaler to 48
	#endif
	
	IP |= 0x10;								    // Make UART high priority
	ES0 = 0;								    // Enable UART0 interrupts
		
	SBCON0 |= 0x40;							    // Enable baud rate generator
	TI0 = 1;									// Indicate TX0 ready
//	PS0 = 1;
	SFRPAGE = SfrPage_;
}
//***********************************************************************/
void Comm_SoftwareInit (void)
{	
    COMM_RING_BUF *p_buf=&CommBuf;
		
	p_buf->RingBufRxCtr= 0;
	p_buf->RingBufRxInPtr  = &p_buf->RingBufRx[0];
	p_buf->RingBufRxOutPtr = &p_buf->RingBufRx[0];
	p_buf->RingBufTxCtr= 0;
	p_buf->RingBufTxInPtr  = &p_buf->RingBufTx[0];
	p_buf->RingBufTxOutPtr = &p_buf->RingBufTx[0];	
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
void Comm_SendSta (void)
{
    UART_T();	
	TI0=1;
}
//***********************************************************************/
void Comm_SendStop (void)
{
	UART_R();
}
//***********************************************************************/
//***********************************************************************/
//***********************************************************************/
//***********************************************************************/
// *功    能： 获取com数据区将要TX的数据
// *入    口： RingBufTxCtr 存储的数据量，RingBufTxOutPtr 输出数据的指针
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u Comm_GetTxChar (int8u *err)
{	COMM_RING_BUF *p_buf=&CommBuf;
	int8u c;
	
	ISR_DIS();
	if (p_buf->RingBufTxCtr > 0)
	{	p_buf->RingBufTxCtr--;
		c = *p_buf->RingBufTxOutPtr++;
		if (p_buf->RingBufTxOutPtr == &p_buf->RingBufTx[COMM_TX_BUF_SIZE])
		{	p_buf->RingBufTxOutPtr = &p_buf->RingBufTx[0];
		}
		*err = COMM_NO_ERR;
		ISR_EN();
		return (c);
	}
	else
	{	*err = COMM_TX_EPT;
		ISR_EN();
		return (NUL);
	}
}
//***********************************************************************/
//***********************************************************************/
// *功    能： 将要TX的数据填入com数据区
// *入    口： RingBufTxCtr 存储的数据量，RingBufTxInPtr 输入数据的指针
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u Comm_PutChar (int8u c)
{	COMM_RING_BUF *p_buf=&CommBuf;
	
	ISR_DIS();
	if (p_buf->RingBufTxCtr < COMM_TX_BUF_SIZE)
	{	p_buf->RingBufTxCtr++;
		*p_buf->RingBufTxInPtr++ = c;
		if (p_buf->RingBufTxInPtr == &p_buf->RingBufTx[COMM_TX_BUF_SIZE])
		{	p_buf->RingBufTxInPtr = &p_buf->RingBufTx[0];
		}
		ISR_EN();
		return (COMM_NO_ERR);
	}
	else
	{	ISR_EN();
		return (COMM_TX_FULL);
	}
}



//***********************************************************************/
//***********************************************************************/
// *功    能： 将接收的数据填入com数据区
// *入    口： RingBufRxCtr 存储的数据量，RingBufRxInPtr 填入数据的指针
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/

void Comm_PutRxChar (int8u c)
{	COMM_RING_BUF *p_buf=&CommBuf;
	
	ISR_DIS();
	if (p_buf->RingBufRxCtr < COMM_RX_BUF_SIZE)
	{	p_buf->RingBufRxCtr++;
		*p_buf->RingBufRxInPtr++ = c;
		if (p_buf->RingBufRxInPtr == &p_buf->RingBufRx[COMM_RX_BUF_SIZE])
		{	p_buf->RingBufRxInPtr = &p_buf->RingBufRx[0];
		}
	}
	ISR_EN();
}
//***********************************************************************/
// *功    能： 将接收的数据取出com数据区
// *入    口： RingBufRxCtr 存储的数据量，RingBufRxInPtr 填入数据的指针
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u Comm_GetChar (int8u *err)
{	COMM_RING_BUF *p_buf=&CommBuf;
	int8u c;
	
	ISR_DIS();
	if (p_buf->RingBufRxCtr > 0)
	{	p_buf->RingBufRxCtr--;
		c = *p_buf->RingBufRxOutPtr++;
		if (p_buf->RingBufRxOutPtr == &p_buf->RingBufRx[COMM_RX_BUF_SIZE])
		{	p_buf->RingBufRxOutPtr = &p_buf->RingBufRx[0];
		}
		*err = COMM_NO_ERR;
		ISR_EN();
		return (c);
	}
	else
	{	*err = COMM_RX_EPT;
		ISR_EN();
		return (NUL);
	}
}

//***********************************************************************/
void Comm_PutStr (int8u* ptr,int8u n)
{	int8u i;
	
	for(i=0;i<n;i++)
	{	Comm_PutChar(*ptr++);
	}
}


#if COM_DEBUG
struct XtoStr Comm_XtoStr(int8u temp)
{
 	struct XtoStr chang;
	chang.a=temp/0x10;
	chang.b=temp%0x10;

	if(chang.a>=0x0&&chang.a<=0x9)
	{
		chang.a+=0x30;	
	}
	else
	{
		chang.a+=0x37;
	}
	if(chang.b>=0x0&&chang.b<=0x9)
	{
		chang.b+=0x30;	
	}
	else
	{
		chang.b+=0x37;
	}
	return chang;
}


void Display_Char(int32u temp)
{
	int8u xdata arr[5]={0};
	int8u xdata i;

	for(i=0;i<4;i++)
	{
		arr[i] = temp>>8*(3-i);
	}

	Comm_PutChar(Comm_XtoStr(arr[0]).a);
	Comm_PutChar(Comm_XtoStr(arr[0]).b);
	Comm_PutChar(Comm_XtoStr(arr[1]).a);
	Comm_PutChar(Comm_XtoStr(arr[1]).b);
	Comm_PutChar(Comm_XtoStr(arr[2]).a);
	Comm_PutChar(Comm_XtoStr(arr[2]).b);
	Comm_PutChar(Comm_XtoStr(arr[3]).a);
	Comm_PutChar(Comm_XtoStr(arr[3]).b);
	Comm_PutChar('\n');

}

/********************************************
　　函数名称：Float2Byte
　　功 能：浮点数转换成char数组
　　参 数：入口参数floatNum，欲转换的浮点数
　　返 回 值：byteArry，转换后的字节数组
/*　　********************************************/

void Float2Byte(float floatNum,int8u* byteArry)
{
	int8u xdata i;
    int8u* pchar;

	pchar=(int8u*)&floatNum;
	for(i=0;i<sizeof(float);i++)
	{
		*byteArry=*pchar;
		pchar++;
		byteArry++;
	}
}

#endif
    #if AD_DEBUG  

void Debug_Displayad(int8u RegAdd,int8u NumOfBytes)
{
	switch(RegAdd)
	{
		case 0x07:
			 Comm_PutStr("Register ID = ",14);
			 break;
		case 0x30:
			 Comm_PutStr("Register OFFSET0 = ",19);
			 break;
		case 0x20:
			 Comm_PutStr("Register SETUPCON0 = ",21);
			 break;
		case 0x04:
			 Comm_PutStr("Register DATA = ",16);
			 break;
		case 0x02:
			 Comm_PutStr("Register IFMODE = ",18);
			 break;
		case 0x01:
			 Comm_PutStr("Register ADCMODE = ",19);
			 break;
		case 0x38:
			 Comm_PutStr("Register Gain = ",16);
			 break;
		case 0x10:
			 Comm_PutStr("Register Channel 1 = ",21);
			 break;
		case 0x11:
			 Comm_PutStr("Register Channel 2 = ",21);
			 break;
		case 0x28:
			 Comm_PutStr("Register Filtcon0 = ",20);
       		 break;
		case 0x00:
			 Comm_PutStr("Register Status = ",18);
			 break;
		default:
			 break;		
	}

	Comm_SendSta();
	delay_ms(100);

//	sample3 = AD_Data[1] << 1;
//	sample3 = (sample3 << 9)|(AD_Data[2]<<2)|(AD_Data[3]>>6);
//	if(AD_Data[4]&0x01 == 1)
//	Display_Char(sample3);

	if(NumOfBytes==1)
	{
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).b);
		delay_ms(50);
	}
	else if(NumOfBytes==2)
	{
		Comm_PutStr("0x",2);
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[2]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[2]).b);
		Comm_PutStr("\n",1);
		delay_ms(100);
		Comm_SendSta();
	}
	else if(NumOfBytes==4)
	{
		Comm_PutStr("0x",2);
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[2]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[2]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[3]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[3]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[4]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[4]).b);
		Comm_PutStr("\n",1);
		delay_ms(50);
		Comm_SendSta();	
	}
	else if(NumOfBytes==3)
	{
		Comm_PutStr("0x",2);
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[2]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[2]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[3]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[3]).b);
		Comm_PutStr("\n",1);
		delay_ms(50);
		Comm_SendSta();	
	}
	else if(NumOfBytes==5)
	{
		Comm_PutStr("0x",2);
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[1]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[2]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[2]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[3]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[3]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[4]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[4]).b);
		Comm_PutChar(Comm_XtoStr(AD_Data[5]).a);
		Comm_PutChar(Comm_XtoStr(AD_Data[5]).b);
		Comm_PutStr("\n",1);
		delay_ms(50);		    
		Comm_SendSta();
  }
  else
  {

	 Comm_PutStr("\n",1);
	 delay_ms(50);
	 Comm_SendSta();
  }
  delay_ms(40);
}
    #endif


//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Comm_TimerInit (void)
{
	CommTimer=10;			   //中断接收数据完毕一帧数据后，调用
}
//***********************************************************************/
void Comm_TimerProc (void)
{
	if( CommTimer>0 ) 
	{   
	if(--CommTimer==0) 
	CommRxFlag=1;         //liuliu     2013-02-22
	}
}
//***********************************************************************/

//***********************************************************************/
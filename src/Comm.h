/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Comm.h
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
#include	"_Type.h"
/***********************************************************************/
#ifndef __COMM_H__
#define __COMM_H__

//#include	"_Include.h"
	/*-----宏定义---------------------*/	
	#define  COMM_RX_BUF_SIZE     200
	#define  COMM_TX_BUF_SIZE     200
	#define  UART_T();			  P1|=0x20;  	//P1.5
	#define  UART_R();			  P1&=0xDF;  	//

	#define BAUDRATE        	9600           	// Baud rate of UART in bps
	#define COMM_NO_ERR			0
	#define COMM_RX_EPT			1
	#define COMM_TX_FULL		2
	#define COMM_TX_EPT			3
	#define	COMM_STOP			1
	#define COMM_TRAN			0
	#define	ISR_EN()		    EA=1
	#define	ISR_DIS()		    EA=0
		
//	#define	COMM_485  							//485通信宏定义
	/*-----结构体数据类型定义---------*/
typedef struct {
				    int16u  RingBufRxCtr;
				    int8u  *RingBufRxInPtr;
				    int8u  *RingBufRxOutPtr;
				    int8u   RingBufRx[COMM_RX_BUF_SIZE];
				    int16u  RingBufTxCtr;	   //TX数组中存放的数据个数
				    int8u  *RingBufTxInPtr;	   //存放数据的临时指针变量
				    int8u  *RingBufTxOutPtr;
				    int8u   RingBufTx[COMM_TX_BUF_SIZE];
				} COMM_RING_BUF;

typedef struct XtoStr{
	int8u a;
	int8u b;
};

	
	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	void  Comm_HardwareInit (void);
	void  Comm_SoftwareInit (void);
	void  Comm_SendSta (void);
	void  Comm_SendStop (void);
	int8u Comm_GetChar (int8u *err);
	int8u Comm_PutChar (int8u c);
	int8u Comm_GetTxChar (int8u *err);
	void  Comm_PutRxChar (int8u c);
	void  Comm_TimerInit (void);
	void  Comm_TimerProc (void);
	int8u Comm_StateGet (void);
	void  Comm_PutStr (int8u* ptr,int8u n);
	void  Sub_SendVer(void);
	void Comm_ClrRxBuf (void);
   struct XtoStr Comm_XtoStr(int8u temp);
   void Display_Char(int32u temp);
   void Float2Byte(float floatNum,int8u* byteArry);
   void Debug_Displayad(int8u RegAdd,int8u NumOfBytes);

/*-----------------------------------*/
#ifdef   __COMM_C__
#define  COMM_EXT
#else
#define	 COMM_EXT  extern
#endif
/*-------------变量声明--------------*/
COMM_EXT COMM_RING_BUF	xdata	CommBuf;
COMM_EXT	bdata	bit		CommRxFlag;		//通信接收标志位
COMM_EXT	bdata	bit		CommTxFlag;		//通信发送标志位
#endif


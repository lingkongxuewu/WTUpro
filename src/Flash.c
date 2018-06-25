/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Flash.c
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
#define   __FLASH_C__
#include  "_Include.h"
/***********************************************************************/
static void Ram_CalDataInit (void);
static void Ram_CalData2Buf (int8u);
static void Ram_Buf2CalData (int8u);
static void FlashArea_User (void) ;
static int8u Flash_CheckSum (int8u *addr,int16u n);
static int8u FlashChk (void);
static  void  Ram_CalDataInit_User(void);
static  void Flash_PageErase (int16u addr);
static  void Flash_BufWr (int16u addr,char* buf,int16u n);
static  void Flash_BufRd (int16u addr,char* buf,int16u n);
static  void FlashUpdate (int16u addr,int8u* buf,int16u n);
static  void Flash_CalDataUpdate_User (int8u page,int16u addr,int8u* buf,int16u n);


//***********************************************************************/
// *功    能：Flash calibrate Init 校准数据初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Flash_CalDataInit (void)
{
    int8u SfrPage_ = SFRPAGE;
    Flash_BufRd(FLASH_SYS,DispBuf,FLASH_BUF_NUM);               //Reading Form FLASH_SYS 读系统数据
    if(OK != FlashChk())                                        //判断校验字 0x85
    {
        Ram_CalDataInit();                                      //内存数据初始化
        Flash_CalDataUpdate();                                  //将内存的数据Update到Flash
    }
    else
    {
        Ram_Buf2CalData(1);                                     //将读出的系统数据保存到相关内存
        Flash_BufRd(FLASH_TEMP,DispBuf,FLASH_BUF_NUM);          //Reading Form FLASH_USER 读临时数据
        if((OK != FlashChk())||(DispBuf[1] != Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2))))
        {
           FlashArea_User();                                    //判断USER区域
        }
        else
        {
            Flash_BufRd(FLASH_USER,DispBuf,FLASH_BUF_NUM);      //取出用户区域的数据
            if((OK != FlashChk())||(DispBuf[1] != Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2))))
            {
               Flash_BufRd(FLASH_TEMP,DispBuf,FLASH_BUF_NUM);
               Ram_Buf2CalData(2);                              //读出临时区域的数据保存到相关内存
               Flash_CalDataUpdate_User (2,FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);      
            }													 //USER的数据异常则 将TEMP 的数据 UPDATE 到USER 区域		
            else
            {
                Flash_BufRd(FLASH_TEMP,DispBuf,FLASH_BUF_NUM);
                Ram_Buf2CalData(2);                             //读出临时区域的数据保存到相关内存
            }
         }

      }

    ACalXId=GetAngXCalCnt();                                    //获取x轴校准点
    ACalYId=GetAngYCalCnt();                                    //获取y轴校准点  

    MenuId=MOD_X_CAL;
    Adc_XCalStop();                                              //通过读Flash的数据调用校准结束函数 算出A,K

    MenuId=MOD_Y_CAL;
    Adc_YCalStop();

    MenuId=MOD_T_CAL;
    Temp_CalStop();			//温度补偿数据初始化
    
    McuT_CalStop();			//温度校准数据初始化


	MainTime4Cnt = SendSpeed[SendSpeedNum]; 					 

    SFRPAGE = CONFIG_PAGE;
    SBRL0   = -(SYSCLK / Braud[Braud_Num] / 2);             	//设置波特率   SFRPAGE=0x0F     SBRL0 = -(SYSCLK / BAUDRATE / 2);

    SFRPAGE = SfrPage_;
	//Braud[Braud_Num]
}
//***********************************************************************/
// *功    能：Flash Update
// *入    口：
// *出    口：
// *备    注： 系统用户命令 对应此操作
// *函数性质：
//***********************************************************************/
void Flash_CalDataUpdate (void)
{
    Ram_CalData2Buf(1);
    FlashUpdate(FLASH_SYS,&DispBuf[0],FLASH_BUF_NUM);          //存系统参数到Flash
    Ram_CalData2Buf(2);                                        //存用户参数到Flash
    FlashUpdate(FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);
    FlashUpdate(FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);
}
void Flash_UserDataUpdate(void)
{

    Flash_CalDataUpdate_User (2,FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);  
    Flash_CalDataUpdate_User (2,FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);  

}
//***********************************************************************/
void FlashUpdate (int16u addr,int8u* buf,int16u n)
{
    bit  ea_temp = EA;
    EA    = 0;

    Flash_PageErase(addr);
    delay_ms(250);
    Flash_BufWr(addr,buf,n);
	Flash_BufWr(0xFBFF,(int8u*)0x00,1);

    EA = ea_temp;
}

//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注： Update Flash User/Temp
// *函数性质：
//***********************************************************************/
void  Flash_CalDataUpdate_User (int8u page,int16u addr,int8u* buf,int16u n)
{

    int8u xdata sum;
    char code * data pread;                                 // FLASH read pointer
    pread = (char code *)(addr+1);

    sum = *pread;                                           // 取出FLASH 中的CheckSum
    Ram_CalData2Buf(page);                                  // 将参数保存到缓存区

    if(sum == DispBuf[1])return;                            // 判断前后的CheckSum 是否相等
    FlashUpdate(addr,buf,n);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Ram_CalDataInit (void)
{
    AngCalDataInit();          								//角度校准数据初始化
    TempCalDataInit();         								//温度补偿数据初始化
    MTCalDataInit();										//温度校准数据初始化
}
//************************将校准数据存到缓存区***********************************************/
static void Ram_CalData2Buf (int8u page)
{   int8u i;
	 int16u j;
    int16u k;
    if(page==1)                                               
    {
        for(i=0;i<ANG_CAL_NUM;i++)
        {
            *((int32s*)&DispBuf[i*16+2]) = GetAngXCalAngVal(i);	
            *((int32s*)&DispBuf[i*16+6]) = GetAngYCalAngVal(i);
            *((int32u*)&DispBuf[i*16+10]) = GetAngXCalAdcVal(i);				//2013-6-24 06:08PM孙娜修改
            *((int32u*)&DispBuf[i*16+14]) = GetAngYCalAdcVal(i); 
        }

        for(i=0;i<SystemVerNum;i++)
        {
          *((int8u*)&DispBuf[ANG_CAL_NUM*16+2+i]) = SystemVer[i];
        }

        j = ANG_CAL_NUM*16+2+SystemVerNum;

        *((int8u*)&DispBuf[j+0])  = GetAngXCalCnt();   //角度校准点数可设   
        *((int8u*)&DispBuf[j+1])  = GetTempXCalCnt();  //温度补尝点数可设
        *((int32u*)&DispBuf[j+2])  = AngRange;			//角度测量范围

		j = ANG_CAL_NUM*16+2+SystemVerNum + 6;
		
        *((int32s*)&DispBuf[j+0])  = GetAngXSysZero();   //X相对测量角度(内部清零）
        *((int32s*)&DispBuf[j+4])  = GetAngYSysZero();   //y相对测量角度(内部清零）
		*((int8u*)&DispBuf[j+8])  = KeyEnb;
		*((int8u*)&DispBuf[j+9])  = GetMcuTCalCnt();    //MCU温度校准点数可设 2013-8-30 02:26PM新增加变量
																														   
        k=ANG_CAL_NUM*16+2+SystemVerNum+16;
        for(;k<FLASH_BUF_NUM;k++)
        {
            DispBuf[k]=0;
        }                                                                       //剩余的 都置0

        DispBuf[0] = FLASH_DATA_CHK; 
        DispBuf[1] = Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2));   //计算CheckSum 
    }
    else if(2==page)                                                            
    {
        *((int8u*)&DispBuf[2]) = SendActPosi;                                   //主动发送OR应答发送
        *((int8u*)&DispBuf[3]) = SendSpeedNum;                                  //主动发送速度
        *((int8u*)&DispBuf[4])  = Braud_Num;         							//波特率
        *((int8u*)&DispBuf[5])  = GetFilterCnt();    							//滤波系数
		*((int16u*)&DispBuf[6]) = AlarmHighLow;
        *((int32s*)&DispBuf[8]) = AngAlarmOffset;                               //报警容差  HXL 新追加12/09/10

        j = 12;

        *((int32s*)&DispBuf[j+0])  = GetAngXUsrZero() ;                         //X相对测量角度(用户清零）
        *((int32s*)&DispBuf[j+4])  = GetAngYUsrZero() ;                          //Y相对测量角度(用户清零）   

        *((int32s*)&DispBuf[j+8]) = GetAngXInst() ;                             //X安装角度
        *((int32s*)&DispBuf[j+12]) = GetAngYInst();                             //Y安装角度

        *((int32s*)&DispBuf[j+16]) = GetAngXSet();
        *((int32s*)&DispBuf[j+20]) = GetAngYSet();                              //报警角度  
        *((int32s*)&DispBuf[j+24]) = GetAngPSet();                              //面报警角度  
		*((int32s*)&DispBuf[j+28]) = Addr;
   		j = 12+32;
    	for(i=0;i<TEMP_CAL_NUM;i++)
        {

            *((int16u*)&DispBuf[j+i*10+0]) = GetTempXCalTempVal(i);   //MCU的温度      
            *((int32s*)&DispBuf[j+i*10+2]) = GetTempXCalAdcVal(i);
            *((int32s*)&DispBuf[j+i*10+6]) = GetTempYCalAdcVal(i);
        }
        j=j+10*TEMP_CAL_NUM;
		for(i=0;i<MTEMP_CAL_NUM;i++)
		{
            *((int16s*)&DispBuf[j+i*4+0]) = GetMcuTCalTempVal(i);   //MCU的温度      
            *((int16u*)&DispBuf[j+i*4+2]) = GetMcuTCalAdcVal(i);
		}        
        
        
		k=j+4*MTEMP_CAL_NUM+32;					 							//	预留32个空间追加新的变量
        for(;k<FLASH_BUF_NUM;k++)
        {
            DispBuf[k]=0;
        }        
        

        DispBuf[0] = FLASH_DATA_CHK;
        DispBuf[1] = Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2));       
    }
}


//**************************************************************************/
//**************************将缓存区的数据存到内存DispBuf to Ram ************/
//**************************************************************************/
static void Ram_Buf2CalData (int8u page)
{   int8u i;
	int16u  j;
    if(1==page)
    {
        for(i=0;i<ANG_CAL_NUM;i++)                                              
        {
		/*
            Ang_Xcal.angarr[i] = *((int32s*)&DispBuf[i*12+2]);                  //将Flash读出的数据存到相关内存
            Ang_Ycal.angarr[i] = *((int32s*)&DispBuf[i*12+6]);
            Ang_Xcal.adcarr[i] = *((int16u*)&DispBuf[i*12+10]);
            Ang_Ycal.adcarr[i] = *((int16u*)&DispBuf[i*12+12]);                 //
       	*/
            SetAngXCalAngVal(*((int32s*)&DispBuf[i*16+2]),i);
            SetAngYCalAngVal(*((int32s*)&DispBuf[i*16+6]),i);
            SetAngXCalAdcVal(*((int32u*)&DispBuf[i*16+10]),i);
            SetAngYCalAdcVal(*((int32u*)&DispBuf[i*16+14]),i);                  //

        }

        for(i=0;i<SystemVerNum;i++)
        {
          SystemVer[i] = *((int8u*)&DispBuf[ANG_CAL_NUM*16+2+i]);
        }

        j=ANG_CAL_NUM*16+SystemVerNum+2; 
                                                           

        SetAngCalCnt(*((int8u*)&DispBuf[j+0]));    //角度校准点数可设   
        SetTempCalCnt(*((int8u*)&DispBuf[j+1]));   //温度补尝点数可设
        AngRange = *((int32u*)&DispBuf[j+2]);		//测量范围
        
		j = ANG_CAL_NUM*16+2+SystemVerNum+ 6;
		
		SetAngXSysZero(*((int32s*)&DispBuf[j+0]));
        SetAngYSysZero(*((int32s*)&DispBuf[j+4]));
		KeyEnb = *((int8u*)&DispBuf[j+8]);
		SetMcuTCalCnt(*((int8u*)&DispBuf[j+9]));    //MCU温度校准点数可设
    }
    if(2==page)
    {
        SendActPosi         = *((int8u*)&DispBuf[2]);
        SendSpeedNum        = *((int8u*)&DispBuf[3]);
        Braud_Num 			= *((int8u*)&DispBuf[4]);      									//波特率
        SetFilterCnt(*((int8u*)&DispBuf[5]));    											//滤波补尝点数可设
		AlarmHighLow		= *((int16u*)&DispBuf[6]);
        AngAlarmOffset      = *((int32s*)&DispBuf[8]);                                      //报警容差 

        j = 12;

        SetAngXUsrZero(*((int32s*)&DispBuf[j+0]));
        SetAngYUsrZero(*((int32s*)&DispBuf[j+4]));

        SetAngXInst(*((int32s*)&DispBuf[j+8]));
        SetAngYInst(*((int32s*)&DispBuf[j+12]));                                             //

        SetAngXSet(*((int32s*)&DispBuf[j+16]));
        SetAngYSet(*((int32s*)&DispBuf[j+20]));
        SetAngPSet(*((int32s*)&DispBuf[j+24]));
		Addr = *((int32s*)&DispBuf[j+28]);
		
		j = 12+32;
        for(i=0;i<TEMP_CAL_NUM;i++)                                            
        {
            SetTempXCalTempVal(*((int16u*)&DispBuf[j+i*10+0]),i);
            SetTempXCalAdcVal ( *((int32s*)&DispBuf[j+i*10+2]),i);
            SetTempYCalAdcVal ( *((int32s*)&DispBuf[j+i*10+6]),i);                     

        }
        j = j+10*TEMP_CAL_NUM;
		for(i=0;i<MTEMP_CAL_NUM;i++)
		{
           SetMcuTCalTempVal(*((int16s*)&DispBuf[j+i*4+0]),i);   //MCU的温度      
           SetMcuTCalAdcVal(*((int16u*)&DispBuf[j+i*4+2]),i);
		}        
    }
}


//************************* CHECKSUM *************************************/
static int8u Flash_CheckSum (int8u *addr,int16u n)
{   int8u * data pread;
    int16u i;
    int8u  sum=0;
    pread = addr;
    for(i=0;i<n;i++)
    {
      sum += *(pread++);
    }
    return(sum);
}

//***********************************************************************/
// *功    能：页擦除
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Flash_PageErase (int16u addr)
{   char xdata * data pwrite;               // FLASH write pointer
    bit  ea_temp = EA;                      // Preserve EA


    pwrite = (char xdata *) addr;
    EA = 0;                                 // Disable interrupts
    VDM0CN = 0xA0;                          // Enable VDD monitor and high threshold
    RSTSRC = 0x02;                          // Enable VDD monitor as a reset source
    FLKEY  = 0xA5;                          // Key Sequence 1
    FLKEY  = 0xF1;                          // Key Sequence 2
    PSCTL |= 0x03;                          // PSWE = 1; PSEE = 1
    VDM0CN = 0xA0;                          // Enable VDD monitor and high threshold
    RSTSRC = 0x02;                          // Enable VDD monitor as a reset source
    *pwrite = 0;                            // Initiate page erase
    PSCTL  &= ~0x03;                        // PSWE = 0; PSEE = 0
    EA = ea_temp;                           // Restore interrupts
}
//***********************************************************************/
// *功    能：Flash Write
// *入    口：addr - valid range is from 0x0000 to 0x7DFE for 32K devices
//            valid range is from 0x0000 to 0x3FFE for 16K devices
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Flash_BufWr (int16u addr,char* buf,int16u n)
{   char xdata * data pwrite;               // FLASH write pointer
    bit  ea_temp = EA;                      // Preserve EA
    int16u i;


    pwrite = (char xdata *) addr;
    EA = 0;                                 // Disable interrupts
    for(i=0;i<n;i++)
    {   VDM0CN = 0xA0;                      // Enable VDD monitor and high threshold
        RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
        FLKEY  = 0xA5;                      // Key Sequence 1
        FLKEY  = 0xF1;                      // Key Sequence 2
        PSCTL |= 0x01;                      // PSWE = 1 which enables writes
        VDM0CN = 0xA0;                      // Enable VDD monitor and high threshold
        RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
        *pwrite++=*buf++;
        PSCTL &= ~0x01;                     // PSWE = 0 which disable writes
    }
    EA = ea_temp;                           // Restore interrupts
}
//***********************************************************************/
// *功    能：Flash Read
// *入    口：addr - valid range is from 0x0000 to 0x7DFE for 32K devices
//            valid range is from 0x0000 to 0x3FFE for 16K devices
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Flash_BufRd (int16u addr,char* buf,int16u n)
{   char code * data pread;                 // FLASH read pointer
    bit  ea_temp = EA;                      // Preserve EA
    int16u i;

    pread = (char code *) addr;
    EA    = 0;                              // Disable interrupts
    for(i=0;i<n;i++)
    {
      *buf++ = *pread++;
    }
    EA = ea_temp;                           // Restore interrupts
}

//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static int8u FlashChk (void)
{   int8u temp;
    temp = DispBuf[0];
    if( temp == FLASH_DATA_CHK)return OK;
    else
    return ERR;
}

//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void FlashArea_User (void)
{
    Flash_BufRd(FLASH_USER,DispBuf,FLASH_BUF_NUM);                
    if((OK != FlashChk())||(DispBuf[1] != Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2))))
    {
      Ram_CalDataInit_User();                           //USER的CHK和CheckSum 有问题 则初始化 TEMP OR USER 局域的数据
      Flash_CalDataUpdate_User (2,FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);
      Flash_CalDataUpdate_User (2,FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);
    }
    else
    {
       Ram_Buf2CalData(2);                              //将USER区域的数据存到内存
       Flash_CalDataUpdate_User (2,FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);  
       													//USER的CHK和CheckSum 都没有问题 将USER 的数据UPDATE 到TEMP 区域
    }
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/

static  void  Ram_CalDataInit_User(void)
{

    SetAngXUsrZero(0);
    SetAngYUsrZero(0);	
    SetAngXSet(5000);			  					//X轴用户报警角参数
    SetAngYSet(5000);			 					//Y轴用户报警角参数
    SetAngPSet(5000);            	
    SetAngXInst(0);
    SetAngYInst(0);
    Init_SystemPara();
}




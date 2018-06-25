/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Send.c
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
#define   __SEND_C__
#include  "_Include.h"

static void Send_RDeal ( char* buf,int8u n);
static void Send_CDeal ( char* buf,int8u n);
static void Send_SDeal ( char* buf,int8u n);
static void Send_WDeal ( char* buf,int8u n);
static void Send_TDeal ( char* buf,int8u n);
static void Send_UDeal ( char* buf,int8u n);


static struct sensor xdata *px_ax=&accel_x;
static struct sensor xdata *py_ax=&accel_y;     


//***********************************************************************/
// *功    能：
// *入    口：CommTimer时间到 ，CommRxFlag有接收完成标志
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Send_TimeProc (void)
{   static xdata int8u rx_buf[COMM_RX_BUF_SIZE];
    static int8u* ptr;
    static int8u xdata i;
    static int8u xdata err;
    static int8u xdata cmd;

//    if(COMM_TRAN==Comm_StateGet()) return;		 //COMM_TRAN==0，CommTimer减到0返回1进入COM处理，CommTimer减没到0返回0
    if(CommRxFlag==0)   return;					 //接收标志

    CommRxFlag = 0;
    ptr=rx_buf;									 
    for(i=0;i<COMM_RX_BUF_SIZE;i++)
    {   
		*ptr++=Comm_GetChar(&err);				  //将接收缓冲区的数据 取出存入	rx_buf
        if(err==COMM_RX_EPT)					 //COMM_RX_EPT ==1	取完标志
        break;
    }
    if(i<3)
    {
    #ifdef  COMM_485
        if(SendActPosi == 0)
        {  
			 MainTime4Cnt = 2000;
        }
    #endif
        return;
    }
#if MODBUS_CHECK_ENB
    if((rx_buf[0] == Addr) || (rx_buf[0] == 0))    //支持广播方式
    {

        ModRtu_RxProc(rx_buf,i);
        return;
    }
#endif
    if((0x0D!=rx_buf[i-2])||(0x0A!=rx_buf[i-1]))	 //判断后两位数据是否是回车换行
    {   
    #ifdef  COMM_485
        if(SendActPosi == 0)
        {   
			MainTime4Cnt = 2000;
        }
    #endif
        return;
    }
	cmd=rx_buf[0];
    switch(cmd)
    { case 'r': if(1!=SendActPosi)return;   //SendActPosi=0:主动发送模式，=1：应答模式
                Send_RDeal(rx_buf,i);
                break;
      case 'c': if(1!=SendActPosi)return;
                Send_CDeal(rx_buf,i);
                break;
      case 's': Send_SDeal(rx_buf,i);
                break;
      case 'u': Send_UDeal(rx_buf,i);
                break;
      case 'w': if(1!=SendActPosi)return;
                Send_WDeal(rx_buf,i);
                break;
      case 't': if(1!=SendActPosi)return;
                Send_TDeal(rx_buf,i);
                break;
    }
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_RDeal ( char* buf,int8u n)
{   int8u xdata data_8u;

    if(0==strncmp(buf,"rd m all",8))
    {   if(n!=10)return;
        Sub_SendPrmtAll();
    }
    else if(0==strncmp(buf,"rd ang s",8))
    {   if(n!=10)return;
        Sub_SendAngXYP();
    }
    else if(0==strncmp(buf,"rd ang",6))
    {   if(n!=8)return;
        Sub_SendAngXY();
    }
    else if(0==strncmp(buf,"rd ver",6))
    {   if(n!=8)return;
        Sub_SendSoftVer();
    }
    else if(0==strncmp(buf,"rd sn",5))
    {   if(n!=7)return;
        Sub_SendSysNum();
    }
    else if(0==strncmp(buf,"rd id",5))
    {   if(n!=7)return;
        Sub_SendID();
    }
    else if(0==strncmp(buf,"rd t cal,",9))
    {   if(n!=13)return;
        data_8u=Sub_Str2Int8u(&buf[9]);
        if(data_8u>=TEMP_CAL_NUM){Sub_SendErr();return;}
        Sub_SendTempCal(data_8u);
    }    
    else if(0==strncmp(buf,"rd x offset",11))
    {   if(n!=13)return;
        Comm_PutStr("x offset,",9);
        Sub_SendOffset(GetAngXInst());                 // 获取安装角
    }
    else if(0==strncmp(buf,"rd y offset",11))
    {   if(n!=13)return;
        Comm_PutStr("y offset,",9);
        Sub_SendOffset(GetAngYInst());				  // 获取安装角
    }
    else if(0==strncmp(buf,"rd x cal,",9))
    {   if(n!=13)return;
        data_8u=Sub_Str2Int8u(&buf[9]);
        if(data_8u>=ANG_CAL_NUM){Sub_SendErr();return;}
        Sub_SendAngXCal(data_8u);
    }
    else if(0==strncmp(buf,"rd y cal,",9))
    {   if(n!=13)return;
        data_8u=Sub_Str2Int8u(&buf[9]);
        if(data_8u>=ANG_CAL_NUM){Sub_SendErr();return;}
        Sub_SendAngYCal(data_8u);
    }   
    else Sub_SendErr();
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_CDeal ( char* buf,int8u n)
{
    if(0==strncmp(buf,"clr zero internal",17))	//温度补尝后再进做一次0点校准(清零)
    {   if(n!=19)Sub_SendErr();
        SetAngXSysZero(0);
        SetAngYSysZero(0);                      
        Comm_PutStr(buf,17);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"clr zero",8))		//用户清零
    {   if(n!=10)return;
        Comm_PutStr(buf,8);
        Sub_SendOk();
        SetAngXUsrZero(0);
        SetAngYUsrZero(0);
    }

    else if(0==strncmp(buf,"cal x sta",9))  //x开始校准
    {   if(n!=11)return;
        if(TRUE==Adc_XCalSta())
        {   Comm_PutStr(buf,9);
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal x stop",10))    //x结束校准
    {   if(n!=12)return;
        if(TRUE==Adc_XCalStop())
        {   Comm_PutStr(buf,10);
            Sub_SendOk();
            SetAngXSysZero(0);//X结束校准完，X轴相对测量值归0(内部清零）
            SetAngXUsrZero(0); //X结束校准完，X轴相对测量值归0(用户清零）

        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal y sta",9))  //y开始校准
    {   if(n!=11)return;
        if(TRUE==Adc_YCalSta())
        {   Comm_PutStr(buf,9);
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal y stop",10))    //y结束校准
    {   if(n!=12)return;
        if(TRUE==Adc_YCalStop())
        {   Comm_PutStr(buf,10);
            Sub_SendOk();
            SetAngYSysZero(0);//Y结束校准完，X轴相对测量值归0(内部清零）
            SetAngYUsrZero(0);// Y结束校准完，Y轴相对测量值归0(用户清零）
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal t sta",9))      //温度补偿开始
    {   if(n!=11)return;
        if(TRUE==Temp_CalSta())
        {   Comm_PutStr(buf,9);
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal t stop",10))    //温度补偿结束
    {   if(n!=12)return;
        if(TRUE==Temp_CalStop())
        {   Comm_PutStr(buf,10);
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal mt sta",10))      //温度开始校准
    {   if(n!=12)return;
        if(TRUE==McuT_CalSta())
        {   Comm_PutStr(buf,10);
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal mt stop",11))    //温度结束校准
    {   if(n!=13)return;
        if(TRUE==McuT_CalStop())
        {   Comm_PutStr(buf,11);
            Sub_SendOk();
        }
        else Sub_SendErr();
    }    
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_SDeal ( char* buf,int8u n)
{   int8u xdata data_ram,data_8u;
    int32s xdata data_32s;
    int8u xdata i;
    int8u SfrPage_ = SFRPAGE;

    if(0==strncmp(buf,"set mode p",10)  )       //set mode p
    {   if(n!=12)return;
	    SendActPosi=1;  						//SendActPosi=0:主动发送模式，=1：应答模式
        Comm_PutStr("set mode p",10);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set mode a",10))    //set mode a
    {   if(n!=12)return;
	    SendActPosi=0;  						//SendActPosi=0:主动发送模式，=1：应答模式
        Comm_PutStr("set mode a",10);
        Sub_SendOk();
        if((Braud_Num < 3)&&(SendSpeedNum > 4))
        {
            SendSpeedNum = 3;
        }
    }
    else if((0==strncmp(buf,"set bps ",8))&&(0==SendActPosi))   //set bps n
    {   if(n!=11)return;
        if(((buf[8]-'0')>=0)&&((buf[8]-'0')<10))
        {
            i = buf[8]-'0';
            if((i > 3)&&(Braud_Num > 2)||(i < 4))		///2011-11-10 14:24修改波特率低于9600时发送频率不能高于4次每秒

            {
                SendSpeedNum= i;
                Comm_PutStr(buf,9);
                Sub_SendOk();
            }
            else
            {
                Sub_SendErr();
            }
            MainTime4Cnt=SendSpeed[SendSpeedNum];
        }
        else if((buf[8]=='t')&&(SendSpeedNum==0))
        {   Sub_SendAngXY();
        }
        else Sub_SendErr();
    }
    else if(1!=SendActPosi)return;  				//以下指令全部在SendActPosi==1 时才有效

    else if(0==strncmp(buf,"set filter1 num,",16))		//滤波系数可设
    {   if(n!=20)return;
        data_8u=Sub_Str2Int8u(&buf[16]);
        if((data_8u>99)||(data_8u<1)){Sub_SendErr();return;}
        SetFilterCnt(data_8u);                  
        Comm_PutStr(buf,18);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set zero internal",17))		//内部置0
    {   if(n!=19)return;
        SetAngXSysZero(GetXAng());
        SetAngYSysZero(GetYAng());            
        Comm_PutStr(buf,17);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set zero",8))       
    {   if(n!=10)return;
	    SetAngXUsrZero(GetAngXRelSys());
        SetAngYUsrZero(GetAngYRelSys());      		//用户置0（获取系统置0）
		Comm_PutStr(buf,8);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set x offset,",13))     //设置安装角
    {   if(n!=23)return;
        if(Sub_Str2Int32sAng(&data_32s,&buf[13])==FALSE){Sub_SendErr();return;}
        Comm_PutStr(buf,21);
        Sub_SendOk();
        SetAngXInst(data_32s);                      
    }
    else if(0==strncmp(buf,"set y offset,",13))     //
    {   if(n!=23)return;
        if(Sub_Str2Int32sAng(&data_32s,&buf[13])==FALSE){Sub_SendErr();return;}
        Comm_PutStr(buf,21);
        Sub_SendOk();
        SetAngYInst(data_32s);                  
    }  
    else if(0==strncmp(buf,"set ang num,",12))		//角度校准点数可设
    {   if(n!=16)return;
        data_ram = Sub_Str2Int8u(&buf[12]);
        if((data_ram>ANG_CAL_NUM)||(data_ram<2))return;
        SetAngCalCnt(data_ram); 
        Comm_PutStr(buf,14);
        Sub_SendOk();
    }    
    else if(0==strncmp(buf,"set t num,",10))    //温度补偿点数可设
    {   if(n!=14)return;
        data_ram =Sub_Str2Int8u(&buf[10]);
        if((data_ram>TEMP_CAL_NUM)||(data_ram<2))return;
        SetTempCalCnt(data_ram);                 //设置温度补偿个数
        Comm_PutStr(buf,12);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set mt num,",11))    //MCU温度校准点数可设
    {   if(n!=15)return;
        data_ram =Sub_Str2Int8u(&buf[11]);
        if((data_ram>MTEMP_CAL_NUM)||(data_ram<2))return;
        SetMcuTCalCnt(data_ram);                 
        Comm_PutStr(buf,13);
        Sub_SendOk();
    }        
    else if(0==strncmp(buf,"samp x,",7))            //X轴校准
    {   if(n!=17)return;
        if(Sub_Str2Int32sAng(&data_32s,&buf[7])==FALSE){Sub_SendErr();return;}
        if(TRUE==Adc_XCalProc(data_32s,GetXOftVal()))   
        {   Comm_PutStr(&buf[5],10);
            Comm_PutChar(',');
            Comm_PutInt32u(GetXOftVal());
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"single samp x,",14))    //X轴单点校准     Ang_Xcal.angarr[i]
    {   if(n!=24)return;
        if(Sub_Str2Int32sAng(&data_32s,&buf[14])==FALSE){Sub_SendErr();return;}
        if((GetAngXCalCnt()<1)||(FALSE == AngChkDiff(data_32s,GetXOftVal(),GetAngXCalCnt(),px_ax->angcal->angarr,px_ax->angcal->adcarr))) //

        {   Sub_SendErr();
            return;
        }
        Comm_PutStr(&buf[0],22);
        Comm_PutChar(',');
        Comm_PutInt32u(GetXOftVal());
        Sub_SendOk();
        ACalXId=GetAngXCalCnt();    			 //获取X轴校准个数
        MenuId=MOD_X_CAL;
        Adc_XCalStop();
    }
    else if(0==strncmp(buf,"samp y,",7))        //Y轴校准
    {   if(n!=17)return;
        if(Sub_Str2Int32sAng(&data_32s,&buf[7])==FALSE){Sub_SendErr();return;}
        if(TRUE==Adc_YCalProc(data_32s,GetYOftVal()))    
        {   Comm_PutStr(&buf[5],10);
            Comm_PutChar(',');
            Comm_PutInt32u(GetYOftVal());
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"single samp y,",14))    //Y轴单点校准
    {   if(n!=24)return;
        if(Sub_Str2Int32sAng(&data_32s,&buf[14])==FALSE){Sub_SendErr();return;}
        if((GetAngXCalCnt()<1)||(FALSE == AngChkDiff(data_32s,GetYOftVal(),GetAngXCalCnt(),py_ax->angcal->angarr,py_ax->angcal->adcarr)))

        {   Sub_SendErr();
            return;
        }
        Comm_PutStr(&buf[0],22);
        Comm_PutChar(',');
        Comm_PutInt32u(GetYOftVal());

        Sub_SendOk();
        ACalYId=GetAngXCalCnt();    
        MenuId=MOD_Y_CAL;
        Adc_YCalStop();
    }    
    else if(0==strncmp(buf,"samp mt,",8))    //MCU温度校准	 samp mt,00,+05
    {   if(n!=16)return;
        if(FALSE==Mcut_CalSamp(Sub_Str2Int8u(&buf[8]),Sub_Str2Int8s(&buf[11]),Get_McuTemp()))				 //HXL 20130701
        Sub_SendErr();
    }    
    else if(0==strncmp(buf,"samp t,",7))    //温度补偿
    {   if(n!=11)return;
        if(FALSE==Temp_CalSamp(Sub_Str2Int8u(&buf[7]),Get_McuTemp(),(int32s)GetXFtVal(),(int32s)GetYFtVal()))
		 Sub_SendErr();
    }
    else if(0==strncmp(buf,"set t ref,",10))   //温补参考点
    {   if(n!=14)return;
        if(Temp_CalTRefSet(Sub_Str2Int8u(&buf[10]))==FALSE){Sub_SendErr();return;}    
        Comm_PutStr(buf,12);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set x alarm,",12))
    {   if(n!=22)return;
        if(Sub_Str2Int32sAng(&data_32s,&buf[12])==FALSE){Sub_SendErr();return;}
        SetAngXSet(data_32s);      
        Comm_PutStr(buf,20);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set y alarm,",12))
    {   if(n!=22)return;
        if(Sub_Str2Int32sAng(&data_32s,&buf[12])==FALSE){Sub_SendErr();return;}
        SetAngYSet(data_32s);      
        Comm_PutStr(buf,20);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set p alarm,",12))
    {   if(n!=22)return;
        if(Sub_Str2Int32sAng(&data_32s,&buf[12])==FALSE){Sub_SendErr();return;}
        if(data_32s<0)data_32s = -data_32s;
        SetAngPSet(data_32s);    
        Comm_PutStr(buf,20);
        Sub_SendOk();
    }
	else if(0==strncmp(buf,"set alarm,",10))
    {   if(n!=13)return;
        if(Sub_StralarmCheck(&data_32s,&buf[10])==FALSE){Sub_SendErr();return;}
        AlarmHighLow=data_32s;      
        Comm_PutStr(buf,11);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"system reset",12))
    {   if(n!=14)return;
        Comm_PutStr(buf,12);
        Sub_SendOk();
        delay_ms(1000);
        Sub_SysRst();
    }
    else if(0==strncmp(buf,"set baud ",9))//设置UART通信参数
    {
        if(n!=12)return;
        data_8u = buf[9]-'0';
        if(data_8u>9)return;
        Braud_Num=data_8u;
        Comm_PutStr(buf,10);
        Sub_SendOk();
        delay_ms(500);
        SFRPAGE = 0x0F;
        SBRL0   = -(SYSCLK / Braud[Braud_Num] / 2);//设置波特率   SFRPAGE=0x0F     SBRL0 = -(SYSCLK / BAUDRATE / 2);
        SFRPAGE = SfrPage_;
    } 
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_WDeal ( char* buf,int8u n)
{   
	int32s xdata data_x,data_y;
    int16u xdata data_t;
	int8u a;

    if(0==strncmp(buf,"wr t,",5))//写温度补尝参数
    {   if(n!=35)return;
        if(Sub_Str2Int32s(&data_x,&buf[14])==FALSE)   {Sub_SendErr();return;}
        if(Sub_Str2Int32s(&data_y,&buf[24])==FALSE)   {Sub_SendErr();return;}
        if(Sub_Str2Int16u(&data_t,&buf[8])==FALSE)	  {Sub_SendErr();return;}
		a = Sub_Str2Int8u(&buf[5]);
		if(a<TEMP_CAL_NUM)
		{
			Temp_CalValWr(a,data_t,data_x,data_y);
			Comm_PutStr(buf,33);
			Sub_SendOk();
			return;
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"wr sn,",6))//写产品编号
    {   if(n!=(SystemVerNum+8))
        {   Sub_SendErr();
            return;
        }
        strncpy(SystemVer,&buf[6],SystemVerNum);
        Comm_PutStr(buf,(SystemVerNum+6));
        Sub_SendOk();
    }
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_UDeal( char* buf,int8u n)
{
    if(0==strncmp(buf,"update flash sys",16))
    {

        if(n!=18)return;
        Flash_CalDataUpdate();//保存一次数据
        Comm_PutStr(buf,16);
        Sub_SendOk();

    }
    else if(0==strncmp(buf,"update flash ",12))
    {
        if(n!=14)return;
        Flash_UserDataUpdate();
        Comm_PutStr(buf,12);
        Sub_SendOk();
    }

    else Sub_SendErr();
}



//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_TDeal( char* buf,int8u n)
{
    if(0==strncmp(buf,"t cal test",10))
    {   if(n!=12)return;
        Sub_SendTCalTest();
    }
}

//***********************************************************************/

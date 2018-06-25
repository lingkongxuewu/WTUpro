/*--------------------------------文*件*信*息-----------------------------
* 文件名：  ModBusRtu.c
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
#define   __MODBUSRTU_C__
#include  "_Include.h"
//***********************************************************************/
#if MODBUS_CHECK_ENB==1
static void ModRtu_RlyError (int8u* buf,int8u excode);
static void ModRtu_WrReg (int8u* buf,int8u n);
static void ModRtu_RdReg (int8u* buf);
static int8u xdata udflashtag=0;

//***********************************************************************/
static  int16u  FloatToInt( float ang, int8u highlow )			//2013-9-22 02:37PM孙娜修改参数类型为int8u
{
    int32u  temp ;
    int16u  vol ;

    ang /= 10000.0 ;
    temp = *( int32u* )&ang ;

    if( highlow == 'L' )
    {
        vol  = ( ( temp << 24 ) >> 24 ) << 8 ;
        vol += ( ( temp << 16 ) >> 24 ) << 0 ;
    }
    else
    {
        vol  = ( ( temp <<  8 ) >> 24 ) << 8 ;
        vol += ( ( temp <<  0 ) >> 24 ) << 0 ;
    }
    return( vol ) ;
}
//***********************************************************************/
// *功    能：计算校验码
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u ModRtu_ChkCrc (int8u *buf,int8u len)
{
    if(len<3)
    {   
		return(FALSE);
    }
    if( ModbusCrcCnt(buf, len) == 0 )		//数据和CRC验证码重新CRC计算后为0 
    {   
		return(TRUE);
    }
    else
    {   
		return(FALSE);
    }
}
//***********************************************************************/
// *功    能：存储校验码
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void ModRtu_WrCrc (int8u* buf,int16u crc)
{
    buf[0] = crc>>8;				//HIGH 8BIT 
    buf[1] = crc;					//LOW 8BIT
}
//***********************************************************************/
// *功    能：数据转换
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int16u  ModRtu_8to16 (int8u* buf)
{
    int16u temp;

    temp = buf[0];
    temp <<= 8;
    temp |= buf[1];
    return(temp);			 //用户接口部分是大端模式
}
//***********************************************************************/
// *功    能：零点设置
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void ModRtu_SetZero (int8u* buf)
{   int16u temp;
    temp = ModRtu_8to16(buf);

    if(1 == temp)
    {
        SetAngXUsrZero(GetAngXRelSys());   //  设置X轴用户置零参数(获取X轴相对角度值内部)
        SetAngYUsrZero(GetAngYRelSys());    
    }
    else if(0 == temp)
    {
        SetAngXUsrZero(0);
        SetAngYUsrZero(0);                      
    }
}
//***********************************************************************/
// *功    能：修改寄存器的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：大端模式下 指数据的高位，保存在内存的低地址
//***********************************************************************/
int32s ModRtu_Wr16u (int8u* buf)//int32s* dat)
{
    int16s temp;
    int32s dat;                

    temp = buf[0];
    temp <<= 8;
    temp |= buf[1];
    dat = (int32s)temp*10;
    return dat;               
}
//***********************************************************************/
// *功    能：修改本机地址
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u ModRtu_WrAddr (int8u* buf)
{
    int16u temp;

    temp = buf[0];
    temp <<= 8;
    temp |= buf[1];
    if((temp < 1) || (temp > 247)) return (FALSE);
    Addr = temp;
    return(TRUE);
}
//***********************************************************************/
// *功    能：修改波特率
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u ModRtu_WrBaud (int8u* buf)
{
    int16u temp;

    temp = buf[0];
    temp <<= 8;
    temp |= buf[1];
    if((temp < 0) || (temp > 8)) return (FALSE);//
    Braud_Num = temp;
    return(TRUE);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
//int8u ModRtu_WrVal (int16u addr, int8u* buf)
int8u ModRtu_WrVal (int16u addr, int8u* buf,int8u* excode)
{
    static  xdata int32u  vol32u ;
    static  xdata int8u  vol8u ;
    float   xdata volf ;
    int16u  xdata pc_data ;
    int16s  xdata vol16s ;

    pc_data = buf[1] * 256 + buf[0] ;  
    switch(addr)
    {
        case ANGX_OFFSET_ADDR:      SetAngXInst(ModRtu_Wr16u(buf));             //写入X轴安装角度
								    udflashtag = 1;
                                    break;
        case ANGY_OFFSET_ADDR:      SetAngYInst(ModRtu_Wr16u(buf));             //写入Y轴安装角度
									udflashtag = 1;
                                    break;        //
        case ANGX_ALARM_ADDR:       SetAngXSet(ModRtu_Wr16u(buf));              //写入X轴开关量输出角度
									udflashtag = 1;
                                    break;
        case ANGY_ALARM_ADDR:       SetAngYSet(ModRtu_Wr16u(buf));              //写入Y轴开关量输出角度
									udflashtag = 1;
                                    break;
        case ANGP_ALARM_ADDR:       SetAngPSet(ModRtu_Wr16u(buf));               //写入P开关量输出角度
									udflashtag = 1;
                                    break;
        case ZERO_ADDR:             ModRtu_SetZero(buf);                        //零点设置值
									udflashtag = 1;
                                    break;
//      case RST_ADDR:              Sub_SysRst();                               //复位系统
//                                  break;

        case SELF_ADDR:             
									udflashtag = 1;
									return (ModRtu_WrAddr(buf));				//修改终端机地址
        case BAUD_ADDR:             
									udflashtag = 1;
									return (ModRtu_WrBaud(buf));				//修改波特率
 //***********************************************
{
    {
    	 case 0x0111:
        {
//           vol32u = pc_data ;
        }break ;
    
        case 0x0112:
        {
//           vol32u += pc_data * 65536 ;
//           volf = *(float*)&vol32u ;
//           Set_AngLow(volf * 1000);             		////设置DA输出对应的最小角度
        }break ;
        case 0x0113:
        {
//           vol32u = pc_data ;
        }break ;
    
        case 0x0114:
        {
//            vol32u += pc_data * 65536 ;
//            volf = *(float*)&vol32u ;
//            Set_AngHigh(volf * 1000);             		////设置DA输出对应的最大角度
        }break ;
        case 0x0115:
        {
            AlarmHighLow = buf[1] ;
        }break ;
        case 0x0116:
        {
            vol32u = pc_data ;
        }break ;
        case 0x0117:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngXSet(volf * 10000);             
        }break ;
        case 0x0118:
        {
            vol32u = pc_data ;
        }break ;
        case 0x0119:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngYSet(volf * 10000);             
        }break ;
        case 0x011A:
        {
            vol32u = pc_data ;
        }break ;
        case 0x011B:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngPSet(volf * 10000);              
        }break ;
        case 0x011C:
        {
            vol32u = pc_data ;
        }break ;
        case 0x011D:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            AngAlarmOffset = volf * 10000 ;               //报警容差
        }break ;
        case 0x011E:
        {
            vol32u = pc_data ;
        }break ;
        case 0x011F:
        {
            vol32u += pc_data * 65536 ;
            if( vol32u == 0xAC35AC35 )
            {
                SetAngXUsrZero(GetAngXRelSys());   
            }
            else if( vol32u == 0x03050305 )
            {
                SetAngXUsrZero(0);
            }
        }break ;
        case 0x0120:
        {
            vol32u = pc_data ;
        }break ;
        case 0x0121:
        {
            vol32u += pc_data * 65536 ;
            if( vol32u == 0xAC35AC35 )
            {
                SetAngYUsrZero(GetAngYRelSys());   
            }
            else if( vol32u == 0x03050305 )
            {
                SetAngYUsrZero(0);
            }
        }break ;
        case 0x0122:
        {
            vol32u = pc_data ;
        }break ;
        case 0x0123:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngXInst(volf * 10000);
        }break ;
        case 0x0124:
        {
            vol32u = pc_data ;
        }break ;
        case 0x0125:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngYInst(volf * 10000);

        }break ;
        case 0x0126:break ;
        case 0x0127:break ;
        case 0x0128:SetFilterCnt(buf[1]); break ;
        case 0x0129:Addr = buf[1] ; break ;
        case 0x012A:break ;
		case 0x012B:
        {	
            if( buf[1] == 0xAC )
            {
                Flash_UserDataUpdate();
            }
        }break ;
        
        
        //***********************************************************
        case 0xA800:
        case 0xA801:
        case 0xA802:
        case 0xA803:
        case 0xA804:
        case 0xA805:
        case 0xA806:
        case 0xA807:
        {
            SystemVer[(addr-0xA800)*2+0] = buf[0] ;
            SystemVer[(addr-0xA800)*2+1] = buf[1] ;
        }break ;
        case 0xA808:
        {
            vol8u = buf[0] ; // 'X' 'Y'
            pc_data = buf[1] ;
            AngRange = pc_data ;                         //测量范围
            switch( pc_data )
            {
                case   5: SetAngCalCnt(11) ;  break ;//间隔1°
                case  15: SetAngCalCnt(11) ;  break ;//间隔3°
                case  30: SetAngCalCnt(13) ;  break ;//间隔5°
				default : *excode=3;  return(FALSE);
            }
        }break ;
        case 0xA809:
        {
            vol32u = pc_data ;
        }break ;
        //***********************************************
        case 0xA80A:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            if( volf > 0 )
            {
                vol16s = (int16s)volf + 2 ;
            }
            else
            {
                vol16s = (int16s)volf - 2 ;
            }
            if( AngRange < 30 )
            {
                vol16s = ( int16s )( vol16s / 5 ) ;
                pc_data = vol16s + (GetAngXCalCnt() / 2) ;   //獲取角度校準點個數  
            }
            else if( AngRange == 30 )
            {
                vol16s = ( int16s )( vol16s / 5 ) * 5 ;
                switch( vol16s )
                {
                    case -30 : pc_data = 0 ; break ;
                    case -15 : pc_data = 1 ; break ;	 
                    case  -5 : pc_data = 2 ; break ;
                    case   0 : pc_data = 3 ; break ;
                    case   5 : pc_data = 4 ; break ;
                    case  15 : pc_data = 5 ; break ;
                    case  30 : pc_data = 6 ; break ;
					default	 : *excode=3;  return(FALSE);
                }
            }
            else 
            {
                vol16s = ( int16s )( vol16s / 15 ) ;
                pc_data = vol16s + (GetAngXCalCnt() / 2) ;   // 獲取角度校準點個數  

            }
            if( vol8u == 'X' )
            {
              SetAngXCalAngVal(volf * 10000,pc_data);
              SetAngXCalAdcVal(GetXOftVal(),pc_data);              
            }
            else
            {
                SetAngYCalAngVal(volf * 10000,pc_data );           
                SetAngYCalAdcVal(GetYOftVal(),pc_data);            

            }
        }break ;
        case 0xA80B:
        {
            pc_data = buf[0] * 256 + buf[1] ;
            if( pc_data == 0x0305 )
            {
                Flash_CalDataUpdate() ;
            }
        }break ;
        case 0xA80E:
        {
            pc_data = buf[0] * 256 + buf[1] ;
            switch( pc_data )
            {
                case 0xA501:SetAngXSysZero(GetXAng());
                            SetAngYSysZero(GetYAng());
                            break ;                       
                case 0xA502:SetAngXSysZero(0);
                            SetAngYSysZero(0);
                            break ;                       
                case 0xA503:SetAngXUsrZero(GetAngXRelSys());
                            SetAngYUsrZero(GetAngYRelSys());
                            break ;
                case 0xA504:SetAngXUsrZero(0) ;
                            SetAngYUsrZero(0) ;
                            break ;
            }
        }break ;
        case 0xA80F:
        {
            vol32u = pc_data ;
        }break ;
        case 0xA810:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngXInst(volf * 10000);
        }break ;
        case 0xA811:
        {
            vol32u = pc_data ;
        }break ;
        case 0xA812:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngYInst(volf * 10000);         
        }break ;
        case 0xA813:
        {
            vol32u = pc_data ;
        }break ;
        case 0xA814:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngXSet(volf * 10000);             
        }break ;
        case 0xA815:
        {
            vol32u = pc_data ;
        }break ;
        case 0xA816:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngYSet(volf * 10000);             
        }break ;
        case 0xA817:
        {
            vol32u = pc_data ;
        }break ;
        case 0xA818:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngPSet(volf * 10000);              
        }break ;
        case 0xA819:
        {
            vol32u = pc_data ;
        }break ;
        case 0xA81A:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            AngAlarmOffset = volf * 10000 ;
        }break ;
        case 0xA81B:
        {
            pc_data = buf[0] * 256 + buf[1] ;
            SendSpeedNum = pc_data ;
        }break ;
        case 0xA81C:
        {
            pc_data = buf[0] * 256 + buf[1] ;
            SetFilterCnt(pc_data);
        }break ;

       case 0xA81D:
       {
//           vol32u = pc_data ;
       }break ;
    
       case 0xA81E:
       {
//            vol32u += pc_data * 65536 ;
//            volf = *(float*)&vol32u ;
//            Set_AngLow(volf * 1000);             		//设置DA输出对应的最小角度
       }break ;
       case 0xA81F:
       {
//           vol32u = pc_data ;
       }break ;
    
       case 0xA820:
        {
//            vol32u += pc_data * 65536 ;
//            volf = *(float*)&vol32u ;
//            Set_AngHigh(volf * 1000);               //设置DA输出对应的最大角度
        }break ;

   	  case 0xA821:
	  {
//	      Da_CalStaX( buf[1]) ;					      //X轴DA 校准0开始			
	  }break ; 

	  case 0xA822:
	  {
//	      vol8u =  buf[1] ;					          //X轴DA 校准数据数组位置			
	  }break ; 
	  case 0xA823:
	  {
//	      Da_CalPorcX(vol8u,pc_data*1000) ;			  //X轴DA 校准数据保存			
	  }break ; 


	  case 0xA824:
	  {
//	      Da_CalStaX( buf[1]) ;					      //X轴DA 校准1开始			
	  }break ; 

	  case 0xA825:
	  {
//	      vol8u =  buf[1] ;					          //X轴DA校准1数据数组位置			
	  }break ; 
	  case 0xA826:
	  {
//	      Da_CalPorcX(vol8u,pc_data*1000) ;			  //X轴DA 校准1数据保存			
	  }break ;  
      
	  case 0xA827:
	  {
//	      Da_CalStaY( buf[1]) ;					      //Y轴DA 校准0开始			
	  }break ; 

	  case 0xA828:
	  {
//	      vol8u =  buf[1] ;					          //Y轴DA 校准数据数组位置			
	  }break ; 
	  case 0xA829:
	  {
//	      Da_CalPorcY(vol8u,pc_data*1000) ;			  //Y轴DA 校准数据保存			
	  }break ; 


	  case 0xA82A:
	  {
//	      Da_CalStaY( buf[1]) ;					      //Y轴DA 校准1开始			
	  }break ; 

	  case 0xA82B:
	  {
//	      vol8u =  buf[1] ;					          //Y轴DA校准1数据数组位置			
	  }break ; 
	  case 0xA82C:
	  {
//	      Da_CalPorcY(vol8u,pc_data*1000) ;			  //Y轴DA 校准1数据保存			
	  }break ;        
        
        
    }
}
 //       default:return(FALSE);
        default:	*excode=02;
					return(FALSE);
    }
    return(TRUE);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void ModRtu_Rd16u (int8u* buf,int16s dat)
{
    buf[0] = MR_READ_REGH(dat);
    buf[1] = MR_READ_REGL(dat);
}
//***********************************************************************/
// *功    能：读寄存器
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u ModRtu_RdVal (int16u addr,int8u* buf,int8u* excode)
{

    int16u  xdata pc_data ;
    int16s  xdata vol16s ;
    switch(addr)
    {

        case ANGX_ADDR:         vol16s = (GetXAngAllSetVal()/10) ; pc_data = *(int16u*)&vol16s ; break ;    //获取最终的角度
        case ANGY_ADDR:         vol16s = (GetYAngAllSetVal()/10) ; pc_data = *(int16u*)&vol16s ; break ;    //HXL 12/8/27 整合更新

        case ANGP_ADDR:         vol16s = (GetAngP()/10)           ; pc_data = *(int16u*)&vol16s ; break ;    //HXL 12/8/27 整合更新

        case ANGX_OFFSET_ADDR:  vol16s = (GetAngXInst()/10)       ; pc_data = *(int16u*)&vol16s ; break ;
        case ANGY_OFFSET_ADDR:  vol16s = (GetAngYInst()/10)       ; pc_data = *(int16u*)&vol16s ; break ;  //HXL 12/8/26 整合更新

        case ANGX_ALARM_ADDR:   vol16s = (GetAngXSet()/10)        ; pc_data = *(int16u*)&vol16s ; break ;
        case ANGY_ALARM_ADDR:   vol16s = (GetAngYSet()/10)        ; pc_data = *(int16u*)&vol16s ; break ;     //HXL 12/8/26

        case ANGP_ALARM_ADDR:   vol16s = (GetAngPSet()/10)        ; pc_data = *(int16u*)&vol16s ; break ;          //HXL 12/8/26 整合更新
        case SELF_ADDR:         vol16s = Addr                     ; pc_data = *(int16u*)&vol16s ; break ;
        case BAUD_ADDR:         vol16s = Braud_Num                ; pc_data = *(int16u*)&vol16s ; break ;
 //**************************************
{
    {
        case 0x0100 : pc_data = SystemVer[ 0] *256 + SystemVer[ 1]       ; break ;
        case 0x0101 : pc_data = SystemVer[ 2] *256 + SystemVer[ 3]       ; break ;
        case 0x0102 : pc_data = SystemVer[ 4] *256 + SystemVer[ 5]       ; break ;
        case 0x0103 : pc_data = SystemVer[ 6] *256 + SystemVer[ 7]       ; break ;
        case 0x0104 : pc_data = SystemVer[ 8] *256 + SystemVer[ 9]       ; break ;
        case 0x0105 : pc_data = SystemVer[10] *256 + SystemVer[11]       ; break ;
        case 0x0106 : pc_data = SystemVer[12] *256 + SystemVer[13]       ; break ;
        case 0x0107 : pc_data = SystemVer[14] *256 + SystemVer[15]       ; break ;
        case 0x0108 : pc_data = FloatToInt( GetXAngAllSetVal() ,'L' )    ; break ;	  //X,Y角度值
        case 0x0109 : pc_data = FloatToInt( GetXAngAllSetVal() ,'H' )    ; break ;
        case 0x010A : pc_data = FloatToInt( GetYAngAllSetVal() ,'L' )    ; break ;
        case 0x010B : pc_data = FloatToInt( GetYAngAllSetVal() ,'H' )    ; break ;
        case 0x010C : pc_data = FloatToInt( GetAngP(), 'L' )             ; break ;
        case 0x010D : pc_data = FloatToInt( GetAngP(), 'H' )             ; break ;
        case 0x010E : pc_data = 0                                        ; break ;
        case 0x010F : pc_data = 0                                        ; break ;
        case 0x0110 : pc_data = 0                                        ; break ;
 		case 0x0111 : pc_data =0 ; break ;//FloatToInt( Get_AngLow()    , 'L' )      ; break ; //获取DA输出对应的最小角度
        case 0x0112 : pc_data =0 ; break ;//FloatToInt( Get_AngLow()    , 'H' )      ; break ; //获取DA输出对应的最小角度
        case 0x0113 : pc_data =0 ; break ;//FloatToInt( Get_AngHigh()    , 'L' )     ; break ; //获取DA输出对应的最大角度
        case 0x0114 : pc_data =0 ; break ;//FloatToInt( Get_AngHigh()    , 'H' )     ; break ; //获取DA输出对应的最大角度
        case 0x0115 : pc_data = AlarmHighLow                             ; break ;
        case 0x0116 : pc_data = FloatToInt( GetAngXSet(), 'L' )          ; break ;
        case 0x0117 : pc_data = FloatToInt( GetAngXSet(), 'H' )          ; break ;
        case 0x0118 : pc_data = FloatToInt( GetAngYSet(), 'L' )          ; break ;
        case 0x0119 : pc_data = FloatToInt( GetAngYSet(), 'H' )          ; break ;
        case 0x011A : pc_data = FloatToInt( GetAngPSet(), 'L' )          ; break ;
        case 0x011B : pc_data = FloatToInt( GetAngPSet(), 'H' )          ; break ;
        case 0x011C : pc_data = FloatToInt( AngAlarmOffset, 'L' )        ; break ;
        case 0x011D : pc_data = FloatToInt( AngAlarmOffset, 'H' )        ; break ;
        case 0x011E : pc_data = FloatToInt( GetAngXUsrZero()    , 'L' )  ; break ;
        case 0x011F : pc_data = FloatToInt( GetAngXUsrZero()    , 'H' )  ; break ;
        case 0x0120 : pc_data = FloatToInt( GetAngYUsrZero()    , 'L' )  ; break ;
        case 0x0121 : pc_data = FloatToInt( GetAngYUsrZero()    , 'H' )  ; break ;		 
        case 0x0122 : pc_data = FloatToInt( GetAngXInst(), 'L' )         ; break ;
        case 0x0123 : pc_data = FloatToInt( GetAngXInst(), 'H' )         ; break ;
        case 0x0124 : pc_data = FloatToInt( GetAngYInst(), 'L' )         ; break ;
        case 0x0125 : pc_data = FloatToInt( GetAngYInst(), 'H' )         ; break ;
        case 0x0126 : pc_data = 0                                        ; break ;
        case 0x0127 : pc_data = 0                                        ; break ;
        case 0x0128 : pc_data = (GetFilterCnt())                         ; break ;
        case 0x0129 : pc_data = Addr                                     ; break ;
        case 0x012A : pc_data = 0                                        ; break ;
//        case 0x012B : pc_data = 0                                        ; break ;

        //*************************************************************
        case 0xA000 : pc_data = 0xAC00                                   ; break ;
        case 0xA001 : pc_data = Get_McuTemp()                            ; break ;   //获取Sensor温度  18/03/20	yyc
        case 0xA002 : //pc_data = TempA                                    ; break ;
        case 0xA003 : //pc_data = TempA                                    ; break ;
        case 0xA004 : //pc_data = TCalTemp[ 0]                             ; break ;
        case 0xA005 : //pc_data = TCalTemp[ 1]                             ; break ;
        case 0xA006 : //pc_data = TCalTemp[ 2]                             ; break ;
        case 0xA007 : //pc_data = TCalTemp[ 3]                             ; break ;
        case 0xA008 : //pc_data = TCalTemp[ 4]                             ; break ;
        case 0xA009 : //pc_data = TCalTemp[ 5]                             ; break ;
        case 0xA00A : //pc_data = TCalTemp[ 6]                             ; break ;
        case 0xA00B : //pc_data = TCalTemp[ 7]                             ; break ;
        case 0xA00C : //pc_data = TCalTemp[ 8]                             ; break ;
        case 0xA00D : //pc_data = TCalTemp[ 9]                             ; break ;
        case 0xA00E : //pc_data = TCalTemp[10]                             ; break ;
        case 0xA00F : //pc_data = TCalTemp[11]                             ; break ;
        case 0xA010 : //pc_data = TCalTemp[12]                             ; break ;
        case 0xA011 : //pc_data = TCalTemp[13]                             ; break ;
        case 0xA012 : //pc_data = TCalTemp[14]                             ; break ;
        case 0xA013 : //pc_data = TCalTemp[15]                             ; break ;
        case 0xA014 : //pc_data = TCalTemp[16]                             ; break ;
        case 0xA015 : //pc_data = TCalTemp[17]                             ; break ;
        case 0xA016 : //pc_data = TCalTemp[18]                             ; break ;
        case 0xA017 : //pc_data = TCalTemp[19]                             ; break ;
        case 0xA018 : pc_data = GetTempXCalTempVal(addr-0xA004)            ; break ;   
        case 0xA019 : //pc_data = *( int16u* )&TCalXOffset[ 0]             ; break ;
        case 0xA01A : //pc_data = *( int16u* )&TCalXOffset[ 1]             ; break ;
        case 0xA01B : //pc_data = *( int16u* )&TCalXOffset[ 2]             ; break ;
        case 0xA01C : //pc_data = *( int16u* )&TCalXOffset[ 3]             ; break ;
        case 0xA01D : //pc_data = *( int16u* )&TCalXOffset[ 4]             ; break ;
        case 0xA01E : //pc_data = *( int16u* )&TCalXOffset[ 5]             ; break ;
        case 0xA01F : //pc_data = *( int16u* )&TCalXOffset[ 6]             ; break ;
        case 0xA020 : //pc_data = *( int16u* )&TCalXOffset[ 7]             ; break ;
        case 0xA021 : //pc_data = *( int16u* )&TCalXOffset[ 8]             ; break ;
        case 0xA022 : //pc_data = *( int16u* )&TCalXOffset[ 9]             ; break ;
        case 0xA023 : //pc_data = *( int16u* )&TCalXOffset[10]             ; break ;
        case 0xA024 : //pc_data = *( int16u* )&TCalXOffset[11]             ; break ;
        case 0xA025 : //pc_data = *( int16u* )&TCalXOffset[12]             ; break ;
        case 0xA026 : //pc_data = *( int16u* )&TCalXOffset[13]             ; break ;
        case 0xA027 : //pc_data = *( int16u* )&TCalXOffset[14]             ; break ;
        case 0xA028 : //pc_data = *( int16u* )&TCalXOffset[15]             ; break ;
        case 0xA029 : //pc_data = *( int16u* )&TCalXOffset[16]             ; break ;
        case 0xA02A : //pc_data = *( int16u* )&TCalXOffset[17]             ; break ;
        case 0xA02B : //pc_data = *( int16u* )&TCalXOffset[18]             ; break ;
        case 0xA02C : //pc_data = *( int16u* )&TCalXOffset[19]             ; break ;
//        case 0xA02D : pc_data = GetTempXCalAdcVal(addr-0xA019)             ; break ;
        case 0xA02D : pc_data = (int16u)(GetTempXCalAdcVal(addr-0xA019)>>8)             ; break ;        
        case 0xA02E : //pc_data = *( int16u* )&TCalYOffset[ 0]             ; break ;
        case 0xA02F : //pc_data = *( int16u* )&TCalYOffset[ 1]             ; break ;
        case 0xA030 : //pc_data = *( int16u* )&TCalYOffset[ 2]             ; break ;
        case 0xA031 : //pc_data = *( int16u* )&TCalYOffset[ 3]             ; break ;
        case 0xA032 : //pc_data = *( int16u* )&TCalYOffset[ 4]             ; break ;
        case 0xA033 : //pc_data = *( int16u* )&TCalYOffset[ 5]             ; break ;
        case 0xA034 : //pc_data = *( int16u* )&TCalYOffset[ 6]             ; break ;
        case 0xA035 : //pc_data = *( int16u* )&TCalYOffset[ 7]             ; break ;
        case 0xA036 : //pc_data = *( int16u* )&TCalYOffset[ 8]             ; break ;
        case 0xA037 : //pc_data = *( int16u* )&TCalYOffset[ 9]             ; break ;
        case 0xA038 : //pc_data = *( int16u* )&TCalYOffset[10]             ; break ;
        case 0xA039 : //pc_data = *( int16u* )&TCalYOffset[11]             ; break ;
        case 0xA03A : //pc_data = *( int16u* )&TCalYOffset[12]             ; break ;
        case 0xA03B : //pc_data = *( int16u* )&TCalYOffset[13]             ; break ;
        case 0xA03C : //pc_data = *( int16u* )&TCalYOffset[14]             ; break ;
        case 0xA03D : //pc_data = *( int16u* )&TCalYOffset[15]             ; break ;
        case 0xA03E : //pc_data = *( int16u* )&TCalYOffset[16]             ; break ;
        case 0xA03F : //pc_data = *( int16u* )&TCalYOffset[17]             ; break ;
        case 0xA040 : //pc_data = *( int16u* )&TCalYOffset[18]             ; break ;
        case 0xA041 : //pc_data = *( int16u* )&TCalYOffset[19]             ; break ;
//        case 0xA042 : pc_data = GetTempYCalAdcVal(addr-0xA02E)           ; break ;   //HXL 12/8/27
        case 0xA042 : pc_data = (int16u)(GetTempYCalAdcVal(addr-0xA02E)>>8)           ; break ;   //HXL 12/8/27        
        case 0xA043 : pc_data = Get_McuTemp() ;                          ; break ;
        case 0xA044 : pc_data = 0 ;                                      ; break ;
        case 0xA045 : pc_data = 0 ;                                      ; break ;
        case 0xA046 : pc_data = 0 ;                                      ; break ;
        case 0xA047 : pc_data = 0 ;                                      ; break ;
        case 0xA048 : pc_data = 0 ;                                      ; break ;
        case 0xA049 : pc_data = 0 ;                                      ; break ;
        case 0xA04A : pc_data = 0 ;                                      ; break ;
        case 0xA04B : pc_data = 0 ;                                      ; break ;
        case 0xA04C : pc_data = 0 ;                                      ; break ;
        case 0xA04D : pc_data = 0 ;                                      ; break ;
        case 0xA04E : pc_data = 0 ;                                      ; break ;
        //******************************************************
        case 0xA100 : pc_data = 0xAC01 ;                                 ; break ;
        case 0xA101 : pc_data = (int16u)(GetXOftVal()>>8)                             ; break ;    //HXL 12/8/27
        case 0xA102 : pc_data = FloatToInt(GetXAngAllSetVal(),'L')       ; break ;
        case 0xA103 : pc_data = FloatToInt(GetXAngAllSetVal(),'H')       ; break ;
        case 0xA104 : //pc_data = ACalXAdc[ 0]                             ; break ;
        case 0xA105 : //pc_data = ACalXAdc[ 1]                             ; break ;
        case 0xA106 : //pc_data = ACalXAdc[ 2]                             ; break ;
        case 0xA107 : //pc_data = ACalXAdc[ 3]                             ; break ;
        case 0xA108 : //pc_data = ACalXAdc[ 4]                             ; break ;
        case 0xA109 : //pc_data = ACalXAdc[ 5]                             ; break ;
        case 0xA10A : //pc_data = ACalXAdc[ 6]                             ; break ;
        case 0xA10B : //pc_data = ACalXAdc[ 7]                             ; break ;
        case 0xA10C : //pc_data = ACalXAdc[ 8]                             ; break ;
        case 0xA10D : //pc_data = ACalXAdc[ 9]                             ; break ;
        case 0xA10E : //pc_data = ACalXAdc[10]                             ; break ;
        case 0xA10F : //pc_data = ACalXAdc[11]                             ; break ;
        case 0xA110 : //pc_data = ACalXAdc[12]                             ; break ;
        case 0xA111 : //pc_data = ACalXAdc[13]                             ; break ;
        case 0xA112 : //pc_data = ACalXAdc[14]                             ; break ;
        case 0xA113 : //pc_data = ACalXAdc[15]                             ; break ;
        case 0xA114 : //pc_data = ACalXAdc[16]                             ; break ;
        case 0xA115 : //pc_data = ACalXAdc[17]                             ; break ;
        case 0xA116 : //pc_data = ACalXAdc[18]                             ; break ;
        case 0xA117 : //pc_data = ACalXAdc[19]                             ; break ;
        case 0xA118 : //pc_data = ACalXAdc[20]                             ; break ;
        case 0xA119 : //pc_data = ACalXAdc[21]                             ; break ;
        case 0xA11A : //pc_data = ACalXAdc[22]                             ; break ;
        case 0xA11B : //pc_data = ACalXAdc[23]                             ; break ;
//        case 0xA11C : pc_data = GetAngXCalAdcVal(addr-0xA104)              ; break ;
        case 0xA11C : pc_data = (int16u)(GetAngXCalAdcVal(addr-0xA104)>>8)              ; break ;
        case 0xA11D : //pc_data = FloatToInt( ACalXAng[ 0], 'L' )          ; break ;
        case 0xA11F : //pc_data = FloatToInt( ACalXAng[ 1], 'L' )          ; break ;
        case 0xA121 : //pc_data = FloatToInt( ACalXAng[ 2], 'L' )          ; break ;
        case 0xA123 : //pc_data = FloatToInt( ACalXAng[ 3], 'L' )          ; break ;
        case 0xA125 : //pc_data = FloatToInt( ACalXAng[ 4], 'L' )          ; break ;
        case 0xA127 : //pc_data = FloatToInt( ACalXAng[ 5], 'L' )          ; break ;
        case 0xA129 : //pc_data = FloatToInt( ACalXAng[ 6], 'L' )          ; break ;
        case 0xA12B : //pc_data = FloatToInt( ACalXAng[ 7], 'L' )          ; break ;
        case 0xA12D : //pc_data = FloatToInt( ACalXAng[ 8], 'L' )          ; break ;
        case 0xA12F : //pc_data = FloatToInt( ACalXAng[ 9], 'L' )          ; break ;
        case 0xA131 : //pc_data = FloatToInt( ACalXAng[10], 'L' )          ; break ;
        case 0xA133 : //pc_data = FloatToInt( ACalXAng[11], 'L' )          ; break ;
        case 0xA135 : //pc_data = FloatToInt( ACalXAng[12], 'L' )          ; break ;
        case 0xA137 : //pc_data = FloatToInt( ACalXAng[13], 'L' )          ; break ;
        case 0xA139 : //pc_data = FloatToInt( ACalXAng[14], 'L' )          ; break ;
        case 0xA13B : //pc_data = FloatToInt( ACalXAng[15], 'L' )          ; break ;
        case 0xA13D : //pc_data = FloatToInt( ACalXAng[16], 'L' )          ; break ;
        case 0xA13F : //pc_data = FloatToInt( ACalXAng[17], 'L' )          ; break ;
        case 0xA141 : //pc_data = FloatToInt( ACalXAng[18], 'L' )          ; break ;
        case 0xA143 : //pc_data = FloatToInt( ACalXAng[19], 'L' )          ; break ;
        case 0xA145 : //pc_data = FloatToInt( ACalXAng[20], 'L' )          ; break ;
        case 0xA147 : //pc_data = FloatToInt( ACalXAng[21], 'L' )          ; break ;
        case 0xA149 : //pc_data = FloatToInt( ACalXAng[22], 'L' )          ; break ;
        case 0xA14B : //pc_data = FloatToInt( ACalXAng[23], 'L' )          ; break ;
        case 0xA14D : pc_data = FloatToInt(GetAngXCalAngVal((addr-0xA11D)/2),'L'); break ; //HXL 12/8/27整合更新
        case 0xA11E : //pc_data = FloatToInt( ACalXAng[ 0], 'H' )          ; break ;
        case 0xA120 : //pc_data = FloatToInt( ACalXAng[ 1], 'H' )          ; break ;
        case 0xA122 : //pc_data = FloatToInt( ACalXAng[ 2], 'H' )          ; break ;
        case 0xA124 : //pc_data = FloatToInt( ACalXAng[ 3], 'H' )          ; break ;
        case 0xA126 : //pc_data = FloatToInt( ACalXAng[ 4], 'H' )          ; break ;
        case 0xA128 : //pc_data = FloatToInt( ACalXAng[ 5], 'H' )          ; break ;
        case 0xA12A : //pc_data = FloatToInt( ACalXAng[ 6], 'H' )          ; break ;
        case 0xA12C : //pc_data = FloatToInt( ACalXAng[ 7], 'H' )          ; break ;
        case 0xA12E : //pc_data = FloatToInt( ACalXAng[ 8], 'H' )          ; break ;
        case 0xA130 : //pc_data = FloatToInt( ACalXAng[ 9], 'H' )          ; break ;
        case 0xA132 : //pc_data = FloatToInt( ACalXAng[10], 'H' )          ; break ;
        case 0xA134 : //pc_data = FloatToInt( ACalXAng[11], 'H' )          ; break ;
        case 0xA136 : //pc_data = FloatToInt( ACalXAng[12], 'H' )          ; break ;
        case 0xA138 : //pc_data = FloatToInt( ACalXAng[13], 'H' )          ; break ;
        case 0xA13A : //pc_data = FloatToInt( ACalXAng[14], 'H' )          ; break ;
        case 0xA13C : //pc_data = FloatToInt( ACalXAng[15], 'H' )          ; break ;
        case 0xA13E : //pc_data = FloatToInt( ACalXAng[16], 'H' )          ; break ;
        case 0xA140 : //pc_data = FloatToInt( ACalXAng[17], 'H' )          ; break ;
        case 0xA142 : //pc_data = FloatToInt( ACalXAng[18], 'H' )          ; break ;
        case 0xA144 : //pc_data = FloatToInt( ACalXAng[19], 'H' )          ; break ;
        case 0xA146 : //pc_data = FloatToInt( ACalXAng[20], 'H' )          ; break ;
        case 0xA148 : //pc_data = FloatToInt( ACalXAng[21], 'H' )          ; break ;
        case 0xA14A : //pc_data = FloatToInt( ACalXAng[22], 'H' )          ; break ;
        case 0xA14C : //pc_data = FloatToInt( ACalXAng[23], 'H' )          ; break ;
        case 0xA14E : pc_data = FloatToInt(GetAngXCalAngVal((addr-0xA11E)/2),'H'); break ; 
        //******************************************************
        case 0xA200 : pc_data = 0xAC02 ;                                 ; break ;
//        case 0xA201 : pc_data = GetYOftVal()                             ; break ;
        case 0xA201 : pc_data = (int16u)(GetYOftVal()>>8)                             ; break ;                     
        case 0xA202 : pc_data = FloatToInt(GetYAngAllSetVal(),'L')       ; break ;
        case 0xA203 : pc_data = FloatToInt(GetYAngAllSetVal(),'H')       ; break ;           
        case 0xA204 : //pc_data = ACalYAdc[ 0]                             ; break ;
        case 0xA205 : //pc_data = ACalYAdc[ 1]                             ; break ;
        case 0xA206 : //pc_data = ACalYAdc[ 2]                             ; break ;
        case 0xA207 : //pc_data = ACalYAdc[ 3]                             ; break ;
        case 0xA208 : //pc_data = ACalYAdc[ 4]                             ; break ;
        case 0xA209 : //pc_data = ACalYAdc[ 5]                             ; break ;
        case 0xA20A : //pc_data = ACalYAdc[ 6]                             ; break ;
        case 0xA20B : //pc_data = ACalYAdc[ 7]                             ; break ;
        case 0xA20C : //pc_data = ACalYAdc[ 8]                             ; break ;
        case 0xA20D : //pc_data = ACalYAdc[ 9]                             ; break ;
        case 0xA20E : //pc_data = ACalYAdc[10]                             ; break ;
        case 0xA20F : //pc_data = ACalYAdc[11]                             ; break ;
        case 0xA210 : //pc_data = ACalYAdc[12]                             ; break ;
        case 0xA211 : //pc_data = ACalYAdc[13]                             ; break ;
        case 0xA212 : //pc_data = ACalYAdc[14]                             ; break ;
        case 0xA213 : //pc_data = ACalYAdc[15]                             ; break ;
        case 0xA214 : //pc_data = ACalYAdc[16]                             ; break ;
        case 0xA215 : //pc_data = ACalYAdc[17]                             ; break ;
        case 0xA216 : //pc_data = ACalYAdc[18]                             ; break ;
        case 0xA217 : //pc_data = ACalYAdc[19]                             ; break ;
        case 0xA218 : //pc_data = ACalYAdc[20]                             ; break ;
        case 0xA219 : //pc_data = ACalYAdc[21]                             ; break ;
        case 0xA21A : //pc_data = ACalYAdc[22]                             ; break ;
        case 0xA21B : //pc_data = ACalYAdc[23]                             ; break ;
//        case 0xA21C : pc_data = GetAngYCalAdcVal(addr-0xA204)              ; break ;  
        case 0xA21C : pc_data = (int16u)(GetAngYCalAdcVal(addr-0xA204)>>8)              ; break ; 
        case 0xA21D : //pc_data = FloatToInt( ACalYAng[ 0], 'L' )          ; break ;
        case 0xA21F : //pc_data = FloatToInt( ACalYAng[ 1], 'L' )          ; break ;
        case 0xA221 : //pc_data = FloatToInt( ACalYAng[ 2], 'L' )          ; break ;
        case 0xA223 : //pc_data = FloatToInt( ACalYAng[ 3], 'L' )          ; break ;
        case 0xA225 : //pc_data = FloatToInt( ACalYAng[ 4], 'L' )          ; break ;
        case 0xA227 : //pc_data = FloatToInt( ACalYAng[ 5], 'L' )          ; break ;
        case 0xA229 : //pc_data = FloatToInt( ACalYAng[ 6], 'L' )          ; break ;
        case 0xA22B : //pc_data = FloatToInt( ACalYAng[ 7], 'L' )          ; break ;
        case 0xA22D : //pc_data = FloatToInt( ACalYAng[ 8], 'L' )          ; break ;
        case 0xA22F : //pc_data = FloatToInt( ACalYAng[ 9], 'L' )          ; break ;
        case 0xA231 : //pc_data = FloatToInt( ACalYAng[10], 'L' )          ; break ;
        case 0xA233 : //pc_data = FloatToInt( ACalYAng[11], 'L' )          ; break ;
        case 0xA235 : //pc_data = FloatToInt( ACalYAng[12], 'L' )          ; break ;
        case 0xA237 : //pc_data = FloatToInt( ACalYAng[13], 'L' )          ; break ;
        case 0xA239 : //pc_data = FloatToInt( ACalYAng[14], 'L' )          ; break ;
        case 0xA23B : //pc_data = FloatToInt( ACalYAng[15], 'L' )          ; break ;
        case 0xA23D : //pc_data = FloatToInt( ACalYAng[16], 'L' )          ; break ;
        case 0xA23F : //pc_data = FloatToInt( ACalYAng[17], 'L' )          ; break ;
        case 0xA241 : //pc_data = FloatToInt( ACalYAng[18], 'L' )          ; break ;
        case 0xA243 : //pc_data = FloatToInt( ACalYAng[19], 'L' )          ; break ;
        case 0xA245 : //pc_data = FloatToInt( ACalYAng[20], 'L' )          ; break ;
        case 0xA247 : //pc_data = FloatToInt( ACalYAng[21], 'L' )          ; break ;
        case 0xA249 : //pc_data = FloatToInt( ACalYAng[22], 'L' )          ; break ;
        case 0xA24B : //pc_data = FloatToInt( ACalYAng[23], 'L' )          ; break ;
        case 0xA24D : pc_data = FloatToInt(GetAngYCalAngVal((addr-0xA21D)/2),'L'); break ;   
        case 0xA21E : //pc_data = FloatToInt( ACalYAng[ 0], 'H' )          ; break ;
        case 0xA220 : //pc_data = FloatToInt( ACalYAng[ 1], 'H' )          ; break ;
        case 0xA222 : //pc_data = FloatToInt( ACalYAng[ 2], 'H' )          ; break ;
        case 0xA224 : //pc_data = FloatToInt( ACalYAng[ 3], 'H' )          ; break ;
        case 0xA226 : //pc_data = FloatToInt( ACalYAng[ 4], 'H' )          ; break ;
        case 0xA228 : //pc_data = FloatToInt( ACalYAng[ 5], 'H' )          ; break ;
        case 0xA22A : //pc_data = FloatToInt( ACalYAng[ 6], 'H' )          ; break ;
        case 0xA22C : //pc_data = FloatToInt( ACalYAng[ 7], 'H' )          ; break ;
        case 0xA22E : //pc_data = FloatToInt( ACalYAng[ 8], 'H' )          ; break ;
        case 0xA230 : //pc_data = FloatToInt( ACalYAng[ 9], 'H' )          ; break ;
        case 0xA232 : //pc_data = FloatToInt( ACalYAng[10], 'H' )          ; break ;
        case 0xA234 : //pc_data = FloatToInt( ACalYAng[11], 'H' )          ; break ;
        case 0xA236 : //pc_data = FloatToInt( ACalYAng[12], 'H' )          ; break ;
        case 0xA238 : //pc_data = FloatToInt( ACalYAng[13], 'H' )          ; break ;
        case 0xA23A : //pc_data = FloatToInt( ACalYAng[14], 'H' )          ; break ;
        case 0xA23C : //pc_data = FloatToInt( ACalYAng[15], 'H' )          ; break ;
        case 0xA23E : //pc_data = FloatToInt( ACalYAng[16], 'H' )          ; break ;
        case 0xA240 : //pc_data = FloatToInt( ACalYAng[17], 'H' )          ; break ;
        case 0xA242 : //pc_data = FloatToInt( ACalYAng[18], 'H' )          ; break ;
        case 0xA244 : //pc_data = FloatToInt( ACalYAng[19], 'H' )          ; break ;
        case 0xA246 : //pc_data = FloatToInt( ACalYAng[20], 'H' )          ; break ;
        case 0xA248 : //pc_data = FloatToInt( ACalYAng[21], 'H' )          ; break ;
        case 0xA24A : //pc_data = FloatToInt( ACalYAng[22], 'H' )          ; break ;
        case 0xA24C : //pc_data = FloatToInt( ACalYAng[23], 'H' )          ; break ;
        case 0xA24E : pc_data = FloatToInt(GetAngYCalAngVal((addr-0xA21E)/2),'H'); break ;     
        //*************************************************************
        case 0xA300 : pc_data = 0xAC03                                      ; break ;
        case 0xA301 : pc_data = SystemVer[ 0] *256 + SystemVer[ 1]          ; break ;
        case 0xA302 : pc_data = SystemVer[ 2] *256 + SystemVer[ 3]          ; break ;
        case 0xA303 : pc_data = SystemVer[ 4] *256 + SystemVer[ 5]          ; break ;
        case 0xA304 : pc_data = SystemVer[ 6] *256 + SystemVer[ 7]          ; break ;
        case 0xA305 : pc_data = SystemVer[ 8] *256 + SystemVer[ 9]          ; break ;
        case 0xA306 : pc_data = SystemVer[10] *256 + SystemVer[11]          ; break ;
        case 0xA307 : pc_data = SystemVer[12] *256 + SystemVer[13]          ; break ;
        case 0xA308 : pc_data = SystemVer[14] *256 + SystemVer[15]          ; break ;
        case 0xA309 : pc_data = (int16u)(GetXFtVal()>>8)                                 ; break ;
        case 0xA30A : pc_data = (int16u)(GetYFtVal()>>8)                                 ; break ;
        case 0xA30B : pc_data = (int16u)(GetXOftVal()>>8)                                ; break ;
        case 0xA30C : pc_data = (int16u)(GetYOftVal()>>8)                                ; break ;
        case 0xA30D : pc_data = FloatToInt( GetXAng()         ,'L' )        ; break ;
        case 0xA30E : pc_data = FloatToInt( GetXAng()         ,'H' )        ; break ;
        case 0xA30F : pc_data = FloatToInt( GetYAng()         ,'L' )        ; break ;
        case 0xA310 : pc_data = FloatToInt( GetYAng()         ,'H' )        ; break ;
        case 0xA311 : pc_data = FloatToInt( GetAngXRelSys()  ,'L' )        ; break ;
        case 0xA312 : pc_data = FloatToInt( GetAngXRelSys()  ,'H' )        ; break ;
        case 0xA313 : pc_data = FloatToInt( GetAngYRelSys()  ,'L' )        ; break ;
        case 0xA314 : pc_data = FloatToInt( GetAngYRelSys()  ,'H' )        ; break ;
        case 0xA315 : pc_data = FloatToInt( GetAngXZeroUsr()     ,'L' )     ; break ;
        case 0xA316 : pc_data = FloatToInt( GetAngXZeroUsr()     ,'H' )     ; break ;
        case 0xA317 : pc_data = FloatToInt( GetAngYZeroUsr()     ,'L' )     ; break ;
        case 0xA318 : pc_data = FloatToInt( GetAngYZeroUsr()     ,'H' )     ; break ;
        case 0xA319 : pc_data = FloatToInt( GetXAngAllSetVal() ,'L' )       ; break ;
        case 0xA31A : pc_data = FloatToInt( GetXAngAllSetVal() ,'H' )       ; break ;
        case 0xA31B : pc_data = FloatToInt( GetYAngAllSetVal() ,'L' )       ; break ;
        case 0xA31C : pc_data = FloatToInt( GetYAngAllSetVal() ,'H' )       ; break ;
        case 0xA31D : pc_data = FloatToInt( GetAngP(), 'L' )                ; break ;
        case 0xA31E : pc_data = FloatToInt( GetAngP(), 'H' )                ; break ;
        case 0xA31F : pc_data = FloatToInt( GetAngXSet(), 'L' )             ; break ;
        case 0xA320 : pc_data = FloatToInt( GetAngXSet(), 'H' )             ; break ;
        case 0xA321 : pc_data = FloatToInt( GetAngYSet(), 'L' )             ; break ;
        case 0xA322 : pc_data = FloatToInt( GetAngYSet(), 'H' )             ; break ;
        case 0xA323 : pc_data = FloatToInt( GetAngPSet(), 'L' )             ; break ;
        case 0xA324 : pc_data = FloatToInt( GetAngPSet(), 'H' )             ; break ;
        case 0xA325 : pc_data = FloatToInt( AngAlarmOffset, 'L' )           ; break ;
        case 0xA326 : pc_data = FloatToInt( AngAlarmOffset, 'H' )           ; break ;
        case 0xA327 : pc_data = FloatToInt( GetAngXSysZero()   , 'L' )      ; break ;
        case 0xA328 : pc_data = FloatToInt( GetAngXSysZero()   , 'H' )      ; break ;
        case 0xA329 : pc_data = FloatToInt( GetAngYSysZero()   , 'L' )      ; break ;
        case 0xA32A : pc_data = FloatToInt( GetAngYSysZero()   , 'H' )      ; break ;
        case 0xA32B : pc_data = FloatToInt( GetAngXUsrZero()        , 'L' ) ; break ;
        case 0xA32C : pc_data = FloatToInt( GetAngXUsrZero()        , 'H' ) ; break ;
        case 0xA32D : pc_data = FloatToInt( GetAngYUsrZero()        , 'L' ) ; break ;
        case 0xA32E : pc_data = FloatToInt( GetAngYUsrZero()        , 'H' ) ; break ;
        case 0xA32F : pc_data = FloatToInt( GetAngXInst()    , 'L' )        ; break ;
        case 0xA330 : pc_data = FloatToInt( GetAngXInst()    , 'H' )        ; break ;
        case 0xA331 : pc_data = FloatToInt( GetAngYInst()    , 'L' )        ; break ;
        case 0xA332 : pc_data = FloatToInt( GetAngYInst()    , 'H' )        ; break ;

        case 0xA333 :// pc_data = 0                                           ; break ;
        case 0xA334 :// pc_data = 0                                           ; break ;
        case 0xA335 :// pc_data = CalXDaOutV[addr-0xA326]              			; break ;   //X轴DA电压电流数据
        case 0xA336 :// pc_data = 0                                           ; break ;
        case 0xA337 :// pc_data = 0                                           ; break ;
        case 0xA338 :// pc_data = CalXDaData[addr-0xA329]              			; break ;   //X轴DA数据        
        
        case 0xA339 :// pc_data = 0                                           ; break ;
        case 0xA33A :// pc_data = 0                                           ; break ;
        case 0xA33B :// pc_data = CalYDaOutV[addr-0xA326]              			; break ;   //Y轴DA电压电流数据
        case 0xA33C :// pc_data = 0                                           ; break ;
        case 0xA33D :// pc_data = 0                                           ; break ;
        case 0xA33E :// pc_data = CalYDaData[addr-0xA329]              			; break ;   //Y轴DA数据          
 
        case 0xA33F :// pc_data = FloatToInt( Get_AngLow()    , 'L' )      ; break ; //获取DA输出对应的最小角度
        case 0xA340 :// pc_data = FloatToInt( Get_AngLow()    , 'H' )      ; break ; //获取DA输出对应的最小角度
        case 0xA341 :// pc_data = FloatToInt( Get_AngHigh()    , 'L' )     ; break ; //获取DA输出对应的最大角度
        case 0xA342 :pc_data = 0 ; break ;// pc_data = FloatToInt( Get_AngHigh()    , 'H' )     ; break ; //获取DA输出对应的最大角度

        case 0xA343 : pc_data = '0'*256+'1'                                 ; break ;
        case 0xA344 : pc_data = '1'*256+'4'                                 ; break ;
        case 0xA345 : pc_data = 2013                              		   ; break ;
        case 0xA346 : pc_data = 1024                              		   ; break ;
        case 0xA347 : pc_data = 0                                           ; break ;
        case 0xA348 : pc_data = (GetAngXCalCnt() *256) + GetTempXCalCnt()   ; break ;
        case 0xA349 : pc_data = AngRange                                    ; break ;
        case 0xA34A : pc_data = SendActPosi                                 ; break ;
        case 0xA34B : pc_data = SendSpeedNum                                ; break ;
        case 0xA34C : pc_data = GetFilterCnt()                              ; break ;
        case 0xA34D : pc_data = 0                                           ; break ;
        case 0xA34E : pc_data = 224                                         ; break ;
    }
}
        default:    *excode=2;
					return(FALSE);
 //       default:     return(FALSE);
    }
    ModRtu_Rd16u( buf, pc_data ) ;
    return(TRUE);
}
//***********************************************************************/
// *功    能：读寄存器的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：	
//主机发送命令：[设备地址]+[命令号]+[起始寄存器地址高8位][低8位]
//  			 +[读取的寄存器数高8位][低8位]+[CRC校验的低8位][CRC校验的高8位]
//设备响应：         
// 	[设备地址] [命令号] [返回的字节个数][数据1][数据2]...[数据n][CRC校验的低8位] [CRC校验的高8位]

//***********************************************************************/
void ModRtu_RdReg (int8u* buf)
{   int16u xdata addr;
    int16u xdata i,j;
    int16u xdata crc=0;
    int8u xdata err_code = 0,excode;

    if(buf[0] == 0)return;              //不支持广播方式
    addr = ModRtu_8to16(&buf[2]);		//读要读取寄存器的起始地址 h=buf[2],l=buf[3]
    j = ModRtu_8to16(&buf[4]);			//读取的寄存器数 h=buf[4],l=buf[5]
    for(i = 0; i < j; i++)
    {
        if(FALSE == ModRtu_RdVal(addr,&buf[3+i*2],&excode))
        {
            err_code = 1;
            break;
        }
        addr++;
    }
    if(err_code == 1)
    {
        ModRtu_RlyError(buf,excode);
        return;
    }
    buf[2] = j*2;       			//数据长度
    i = j*2+3;						//CRC的寄存器个数=设备号+命令号+数据总数+数据数
    crc = ModbusCrcCnt(buf,i);		//计算CRC
    ModRtu_WrCrc(&buf[i],crc);      //写入CRC校验码
    Comm_PutStr(buf,i+2);			//i+2 加2是指加CRC的结果
    Comm_SendSta();
}
//***********************************************************************/
// *功    能：写寄存器的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void ModRtu_WrReg (int8u* buf,int8u n)
{   int16u xdata addr;
    int16u xdata i,j;
    int16u xdata crc;
    int8u xdata err_code = 0,excode;

    j = ModRtu_8to16(&buf[4]);
    if((n != (j*2+9))||(buf[6]!= (j*2)))    return;	//数据长度判断
    addr = ModRtu_8to16(&buf[2]);					//地址
    for(i = 0; i < j; i++)
    {
        if(FALSE == ModRtu_WrVal(addr,&buf[7+i*2],&excode))
        {
            err_code = 1;
            break;
        }
        addr++;
    }
    if(buf[0] == 0) return;         //广播方式无返回
    if(err_code == 1)
    {
        ModRtu_RlyError(buf,excode);
        return;
    }
    crc = ModbusCrcCnt(buf,6);
    ModRtu_WrCrc(&buf[6],crc);
//    Comm_PutStr(buf,8);
//    Comm_SendSta();
    if(  1 == udflashtag )
    {
        udflashtag=0;
		Flash_UserDataUpdate();
    }
}
//***********************************************************************/
// *功    能：通讯异常处理
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
/*void ModRtu_RlyError (int8u* buf)
{   int16u crc;

    buf[1] |= 0x80;                 //功能码最高位设置为1
    buf[2] = 0xff;
    crc = ModbusCrcCnt(buf,6);
    buf[3] = MR_READ_REGL(crc);
    buf[4] = MR_READ_REGH(crc);
    Comm_PutStr(buf,5);
    Comm_SendSta();
}*/
void ModRtu_RlyError (int8u* buf,int8u excode)
{   int16u xdata crc;

    buf[1] |= 0x80;                 //功能码最高位设置为1
    buf[2] = excode;
    crc = ModbusCrcCnt(buf,3);      //liuliu 更改长度,原为6,error
    buf[3] = MR_READ_REGH(crc);     //liuliu 2013-04-01
    buf[4] = MR_READ_REGL(crc);		//liuliu 2013-04-01
    Comm_PutStr(buf,5);
    Comm_SendSta();
}
//***********************************************************************/
// *功    能：接收数据帧处理
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void ModRtu_RxProc (int8u* buf, int8u n)
{
    if(FALSE == ModRtu_ChkCrc(buf,n)) return;                   //计算校验结果

    switch(buf[1])
    {
        case READ:          ModRtu_RdReg(buf);  				// 读取寄存器
                            break;
        case WRITE:         ModRtu_WrReg(buf,n);				// 写寄存器
                            break;
        default:            ModRtu_RlyError(buf,1);
                            break;
    }
}

#endif
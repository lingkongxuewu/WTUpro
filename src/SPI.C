/*--------------------------------文*件*信*息-----------------------------
* 项目名: 
* 文件名：  SPI.c
* 版　本：  v1.0
* 描  述：  
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
#define   __SPI_C__
#include "_Include.h"
//***********************************************************************/



void SPI_init(void)
{
	int8u SfrPage = SFRPAGE;
	SFRPAGE = ACTIVE_PAGE;

	
	SPI0CFG |= 0x70; //配置spi的相位和极性，设置主模式
	SPI0CN |= 0x09;  //4-wire and enable SPI0
	SPI0CKR = 0x0B;// f=1MHz 

	SFRPAGE = SfrPage;
}

void SPI_Read(int8u *dat,int8u num)
{
	int8u i;
	int8u temp[1] = {0x00};
	
	for(i = 0;i < num;i++)
	{
		SPI_Write_INT(temp,1);
		delayxus();
		*dat = SPI0DAT;
		dat++;
	}
}

void SPI_Write(int8u* dat,int8u num)
{
	int8u  i;
//	int8u temp=0x00;
	
	for(i = 0;i < num;i++)
	{
		while(!TXBMT);		//write the transmit buffer when the buffer is empty
		SPI0DAT = *dat;
		dat++;
//	    while(!SPIF);
	}

}

void SPI_Write_INT(int8u* dat,int8u num)
{
	int8u  i;
	int8u  temp=0x00;
	
	for(i = 0;i < num;i++)
	{
		while(!TXBMT);		//write the transmit buffer when the buffer is empty
		SPI0DAT = *dat;
		dat++;
//	    while(!SPIF);
	}

}

int8u SensorSPI_Write (int8u dat)
{	int8u i;
	int8u temp;
	int8u temp1;
	
	SPI_SCLK=0;		delay_cyc(PERIOD);
	SPI_CS=1;		delay_cyc(PERIOD);
	SPI_CS=0;		delay_cyc(PERIOD);

	temp = dat;
	for(i = 0;i < 16;i++)
	{
		if(i<8)
		{
			if((temp&0x80)==0)SPI_MOSI=0;
			else              SPI_MOSI=1;
			delay_cyc(PERIOD);
			SPI_SCLK=1;		delay_cyc(PERIOD);								
			SPI_SCLK=0;	   
			if(i==7)   delay_cyc(PERIOD);
			temp<<=1;
		}
		else
		{
			SPI_SCLK = 1; 
			temp1<<=1;
			if(SPI_MISO==1)	temp1 |=1;
							delay_cyc(PERIOD);
			SPI_SCLK=0;		delay_cyc(PERIOD);
		}
	}
	SPI_CS=1;
	return temp1;	
}


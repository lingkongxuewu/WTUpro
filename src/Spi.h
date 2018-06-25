#ifndef  __SPI_H__
#define  __SPI_H__


#include  "_Type.h"
#include <compiler_defs.h>
#include <c8051F500_defs.h>

	sbit SPI_MISO = P2^1;//P2^2;
	sbit SPI_MOSI = P2^2;//P2^1;
	sbit SPI_SCLK = P2^0;
	sbit SPI_CS   = P2^3;

	void SPI_init(void);
	void SPI_Read(int8u *dat,int8u num);
	void SPI_Write(int8u* dat,int8u num);
	void SPI_Read_INT(int8u *dat,int8u num);
	void SPI_Write_INT(int8u* dat,int8u num);
	int8u SensorSPI_Write (int8u dat);     
	
#endif

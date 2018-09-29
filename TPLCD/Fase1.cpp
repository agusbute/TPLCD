#include "stdafx.h"
#include "Fase1.h"

#define MY_LCD_DESCRIPTION "EDA LCD 2 B"
using namespace std;

Fase1::
Fase1()
{
	disp_status = FT_OK;
	if ((disp_status = FT_OpenEx(1, &disp_handler)) == FT_OK)
	{
		if (FT_SetBitMode(disp_handler, 0xFF, 1) != FT_OK)		//0xFF es la mascara, 1 es el modo
		{			
			cout << "Couldn't configure LCD" << endl;
		}
	}
	else
	{
		 cout<< "Couldn't open LCD" << endl;
	}
}

Fase1::
~Fase1()
{

};

bool Fase1::
lcdInit()
{
	
};

bool Fase1::
lcdWriteIR(unsigned char byte)
{

};

bool Fase1::
lcdWriteDR(unsigned char byte)
{
	unsigned char temp = byte & 0xF0;
	temp = temp | 0x20;
	lcdwriteNibble(&disp_handler, temp);
	temp = ((byte & 0x0F) << 4) & 0xF0;
	temp = temp | 0x02;
	lcdWriteNibble(&disp_handler, temp);
	return;
};

bool Fase1::
lcdWriteByte(unsigned char, unsigned char)
{

};

bool Fase1::
lcdWriteNibble(FT_HANDLE * disp, unsigned char nibble)
{
	unsigned char temp = nibble & 0xFE; // 11111110
	FT_Write(disp, &temp, 1, bytesSent);
	Sleep(1);
	temp = nibble | 0x01; // 00000001
	FT_Write(disp, &temp, 1, bytesSent);
	Sleep(3);
	temp = nibble & 0xFE; // 11111110
	FT_Write(disp, &temp, 1, bytesSent);
	Sleep(1);
	return true;
};
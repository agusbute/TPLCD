#include "stdafx.h"
#include "Fase1.h"

#define MY_LCD_DESCRIPTION "EDA LCD 2 B"
using namespace std;

Fase1::
Fase1()
{
	lcdInit();		//llama a inicializador de lcd y ftdi handler
}

Fase1::
~Fase1()
{
	FT_Close(disp_handler);
};

bool Fase1::	
lcdInit()	// esto genera un handler del display y setea el bitmode del ftdi
{
	disp_status = FT_OK;
	if ((disp_status = FT_Open(1, &disp_handler)) == FT_OK)
	{
		if (FT_SetBitMode(disp_handler, 0xFF, 1) != FT_OK)		//0xFF es la mascara, 1 es el modo
		{
			cout << "Couldn't configure LCD" << endl;
			return false;
		}
	}
	else
	{
		cout << "Couldn't open LCD" << endl;
		return false;
	}
	return true;
};

bool Fase1::
lcdWriteIR(BYTE byte)
{
	unsigned char temp = byte & 0xF0; //11110000 temp tiene el MSN
	temp = temp | LCD_RS_OFF; //0000 0010	
	if (lcdWriteNibble(&disp_handler, temp))
	{
		temp = ((byte & 0x0F) << 4) & 0xF0;
		temp = temp | LCD_RS_OFF; //
		if (lcdWriteNibble(&disp_handler, temp))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
};

bool Fase1::
lcdWriteDR(BYTE byte)		
{
	unsigned char temp = byte & 0xF0; //11110000 temp tiene el MSN
	temp = temp | LCD_RS_ON; //0000 0010	
	if (lcdWriteNibble(&disp_handler, temp))
	{
		temp = ((byte & 0x0F) << 4) & 0xF0;
		temp = temp | LCD_RS_ON; //
		if (lcdWriteNibble(&disp_handler, temp))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
};

bool Fase1::
lcdWriteByte(FT_HANDLE * deviceHandler, BYTE value, BYTE rs)
{
	bool res = false;
	if (rs == LCD_RS_ON)
	{
		if (lcdWriteDR(value))
		{
			res = true;
		}
	}
	else
	{
		if (lcdWriteIR(value))
		{
			res = true;
		}
	}
	return res;
};

bool Fase1::
lcdWriteNibble(FT_HANDLE * disp, BYTE nibble)
{
	bool res = false;
	unsigned char temp = nibble & 0xFE; // 11111110
	if (FT_Write(disp, &temp, 1, bytesSent) == FT_OK)		//guarda esto puede tirar error en todos lados
	{
		cout << "error en write nibble" << endl;
		res = true;
	}
	Sleep(1);
	temp = nibble | 0x01; // 00000001
	FT_Write(disp, &temp, 1, bytesSent);
	Sleep(3);
	temp = nibble & 0xFE; // 11111110
	if (FT_Write(disp, &temp, 1, bytesSent) == FT_OK)
	{
		cout << "error en write nibble" << endl;
		res = true;
	}
	Sleep(1);
	return res;
}
#include "stdafx.h"
#include "Fase1.h"

using namespace std;

Fase1::
Fase1()
{
	lcdInit();		//llama a inicializador de lcd y ftdi handler
	changeFourBitMode();	//inicializa el display en 4 bits
}

Fase1::
~Fase1()
{
	FT_Close(disp_handler);
};

FT_STATUS Fase1::getStatus()
{
	return disp_status;
}

bool Fase1::	
lcdInit()	// esto genera un handler del display y setea el bitmode del ftdi
{
	disp_status = FT_OK;
	if ((disp_status = FT_Open(1, &disp_handler)) == FT_OK)
	{
		if (FT_SetBitMode(disp_handler, 0xFF, 1) != FT_OK)		//0xFF es la mascara, 1 es el modo de 4 bits
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
	unsigned char temp = MSN(byte); //temp tiene el MSN
	temp = temp | LCD_RS_OFF; //0000 0010	
	if (!lcdWriteNibble(temp))
	{
		temp = LSN(byte);
		temp = temp | LCD_RS_OFF; //
		if (!lcdWriteNibble(temp))
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
	BYTE temp = MSN(byte);	//temp tiene el MSN
	temp = temp | LCD_RS_ON; //0000 0010	
	if (!lcdWriteNibble(temp))
	{
		temp = LSN(byte);
		temp = temp | LCD_RS_ON; //
		if (!lcdWriteNibble(temp))
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
lcdWriteByte(BYTE value, BYTE rs)
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
lcdWriteNibble(BYTE nibble)
{
	bytesSent = 0;
	bool res = false;
	unsigned char temp = nibble & 0xFE; // 11111110
	if (FT_Write(disp_handler, &temp, 1, &bytesSent) != FT_OK)
	{
		cout << "error en write nibble" << endl;
		res = true;
	}
	Sleep(1);
	temp = nibble | LCD_E; // 00000001
	FT_Write(disp_handler, &temp, 1, &bytesSent);
	Sleep(3);
	temp = nibble & 0xFE; // 11111110
	if (FT_Write(disp_handler, &temp, 1, &bytesSent) != FT_OK)
	{
		cout << "error en write nibble" << endl;
		res = true;
	}
	Sleep(1);
	return res;
}

FT_STATUS Fase1::
changeFourBitMode()
/*{
	FT_STATUS res = FT_IO_ERROR;
	BYTE function = ((FUNCTION_SET) | (MODE_8BIT));
	if (lcdWriteIR(function))
	{
		Sleep(5);
		if (lcdWriteIR(function))
		{
			Sleep(1);
			if (lcdWriteIR(function))
			{
				Sleep(1);
				function = (FUNCTION_SET) | (MODE_4BIT);
				if (lcdWriteIR(function))
				{
					Sleep(1);
					function = ((FUNCTION_SET) | (MODE_4BIT) | (DSP_LINES_TWO) | (FONT_5X8));
					if (lcdWriteIR(function))
					{
						Sleep(1);
						function = DISPLAY_CONTROL;
						if (lcdWriteIR(function))
						{
							Sleep(1);
							function = CLEAR_SCREEN;
							if (lcdWriteIR(function))
							{
								Sleep(10);
								function = ENTRY_MODE_SET;
								if (lcdWriteIR(function))
								{
									res = FT_OK;
								}
							}

						}

					}

				}

			}

		}

	}
	return res;
}*/
{
	bool progr_status = true; //FT_Ok es ==0 entonces si se suma deberia quedarse igual

	progr_status &= lcdWriteNibble(MSN(FUNCTION_SET) | MODE_8BIT); //Envia el nibble alto de ?function set? con el modo en 8 bits. 
	Sleep(4); //Sleep de 4ms 
	progr_status &= lcdWriteNibble(MSN(FUNCTION_SET) | MODE_8BIT); //Envia el nibble alto de ?function set? con el modo en 8 bits. 
	Sleep(1);//Sleep de 1ms (deberia ser 0,1ms pero no genera diferencia)
	progr_status &= lcdWriteNibble(MSN(FUNCTION_SET) | MODE_8BIT); //Envia el nibble alto de ?function set? con el modo en 8 bits. 
	progr_status &= lcdWriteNibble(MSN(FUNCTION_SET) | MODE_4BIT); //Envia el nibble alto de ?function set? con el modo en 8 bits. 
	progr_status &= lcdWriteIR(FUNCTION_SET | MODE_4BIT | DSP_LINES_TWO | FONT_5X8); //Envia la instruccion ?function set? con el modo en 4 bits.
	progr_status &= lcdWriteIR(DISPLAY_ON_OFF_CONTROL); //Envia la instrucci?n  ?display on/off control? con el modo en 4 bits.
	progr_status &= lcdWriteIR(CLEAR_SCREEN); //Envia la instrucci?n  ?clear screen? con el modo en 4 bits.
	progr_status &= lcdWriteIR(ENTRY_MODE_SET); //Envia la instrucci?n  ?mode set entry? con el modo en 4 bits.

	return progr_status;
}
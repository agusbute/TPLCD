#include "stdafx.h"
#include "Fase2.h"
#include "BasicLCD.h"


Fase2::Fase2()
{
	fase1 = new Fase1;
	cadd = 1;
	error = false;
}

Fase2::~Fase2()
{
	delete fase1;
}

bool Fase2::
lcdInitOk()
{
	bool res = false;
	if (fase1->getStatus() == FT_OK)
	{
			res = true;
	}
	return res;
}

FT_STATUS Fase2::
lcdGetError()
{
	return fase1->getStatus();
}


bool Fase2::
lcdClear()
{
	bool res = false;
	res = !(fase1->lcdWriteIR(CLEAR_SCREEN)); 
	Sleep(10);
	cadd = 1;
	return res;
}

bool Fase2::
lcdClearToEOL()
{
	cursorPosition pos = lcdGetCursorPosition();	//tomo la posición actual del cursor
	int buff = cadd;	
	bool res = true, res1=true;

	if (pos.row == 0)	//si está en la prime
	{
		while(cadd <= 16)
		{
			if (fase1->lcdWriteDR(' '))
			{
				res = true;
			}
			res1 = lcdMoveCursorRight();
		}
	}
	else
	{
		while(cadd <= 16)
		{
			if (!(fase1->lcdWriteDR(' ')))
			{
				res = false;	
			}
			res1 = lcdMoveCursorRight();
		}
	}
	cadd = buff;
	return (res1 || res);
}

Fase2& Fase2:: operator<<(const unsigned char c)
{
	fase1->lcdWriteDR(c);	//escribo el caracter						
	lcdMoveCursorRight();	//muevo el cursor a la siguiente posición
	return *this;
}

Fase2& Fase2:: 
operator<<(const unsigned char * c)
{
	int len = strlen((const char*)c);	//guardo el largo del string
	for(int i = 0; i < len; i++)	//voy escribiendo de a caracter
	{
		fase1->lcdWriteDR(c[i]);
		lcdMoveCursorRight();	//muevo el cursor a la siguiente posición
	}
	return *this;
}

bool Fase2::
lcdMoveCursorUp()
{
	if (cadd > 16)	//si cadd es < 16 no hace nada porque ya está en la primera linea
	{
		cadd -= 16;
		lcdUpdateCursor();
	}
	return !getError();
}

bool Fase2::
lcdMoveCursorDown()
{
	if (cadd <= 16)	//si es cadd > 16 no hace nada porque ya está en la segunda linea
	{
		cadd += 16;
		lcdUpdateCursor();
	}
	return !getError();
}

bool Fase2::
lcdMoveCursorRight()
{
	if (cadd < 32)	//si cadd > 32 no hace nada porque no puede ir más a la derecha
	{
		cadd++;
		lcdUpdateCursor();
	}
	return !getError();
}

bool Fase2::
lcdMoveCursorLeft()
{
	if (cadd > 1)	//si cadd = 1 no hace nada porque no puede ir más a la izquierda
	{
		cadd--;
		lcdUpdateCursor();
	}
	return !getError();
}

bool Fase2::
lcdSetCursorPosition(const cursorPosition pos)
{
	bool ans;
	if(pos.row <= 1 && pos.row >=0 && pos.column >=0 && pos.column <=16)	//verifico que row y column tomen parámetros válidos
	{
		cadd = pos.row * 16 + pos.column;
		ans = true;
	}
	else
	{
		ans = false;
	}
	return ans;
}

cursorPosition Fase2::
lcdGetCursorPosition()
{
	cursorPosition pos;
		if (cadd / 2 > 16)	//veo si el cursor está en la segunda linea
		{
			pos.row = 1;	//segunda linea
			pos.column = cadd - 17;	//setteo la posición
		}
		else
		{
			pos.row = 0;	//primera linea
			pos.column = cadd - 1;	//setteo la posición
		}
		return pos;
}

void Fase2::
lcdUpdateCursor()
{
	cursorPosition pos;
	if (cadd / 2 > 16)	//veo si el cursor está en la segunda linea
	{
		pos.row = 1;	//segunda linea
		pos.column = cadd - 17;	//setteo posición
	}
	else
	{
		pos.row = 0;	//primera linea 
		pos.column = cadd - 1;	//setteo posición
	}
	if (!(fase1->lcdWriteIR(SET_ADDRESS(pos.column, pos.row)))) //ponemos el cursor en la posicion del DDRAM que viene dada por el cadd
	{
		error = true;
	}
}

bool Fase2::
getError()
{
	return error;
}
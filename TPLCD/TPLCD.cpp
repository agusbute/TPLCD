#include "stdafx.h"
#include "Fase1.h"
#include "Fase2.h"

using namespace std;

int main()
{
	Fase2 pepito;
	pepito.lcdClear();
	const unsigned char x[] = "1337";
	pepito << x ;
	Sleep(5000);
	pepito.lcdClear();
	return 0;
}


#include "stdafx.h"
#include "Fase1.h"

using namespace std;
int main()
{
	Fase1 fase1;
	fase1.lcdWriteDR('W');
	Sleep(5000);
	fase1.lcdWriteIR(CLEAR_SCREEN);
	Sleep(5000);
	return 0;
}




#include "keyboard.h"
#include "stm32f1xx_hal.h"


const int16_t row_pins[COUNT_ROW_PINS]={(1<<6), (1<<7), (1<<8)};	//Входы с подтяжками
const int16_t col_pins[COUNT_COL_PINS]={(1<<10), (1<<11)};		//Выходы

const Ekey key_kode[COUNT_ROW_PINS][COUNT_COL_PINS]=
	{
	{KEY_Up, KEY_Down},
	{KEY_ENT, KEY_Left},
	{KEY_Right, KEY_ESC}
	};



Ekey CKeyBoard::ScanKey(void)
{
Ekey key=KEY_None;

for (uint8_t col=0; col<COUNT_COL_PINS; col++)
	{
	GPIOB->BRR = col_pins[col];
	for (uint8_t row=0; row<COUNT_ROW_PINS; row++)
		{
		if ((GPIOB->IDR&row_pins[row])==0) key=key_kode[row][col];
		}
	GPIOB->BSRR= col_pins[col];
	}
return key;
}



Ekey CKeyBoard::GetKey(void)
{
Ekey key=ScanKey();
uint8_t k=0;
if (key!=KEY_None)
	{
	for (uint8_t n=0; n<10; n++) if(ScanKey()==key) k++;
	if (k>=9) return key;
	}
return KEY_None;
}

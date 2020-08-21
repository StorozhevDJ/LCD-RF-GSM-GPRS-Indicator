

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define COUNT_ROW_PINS	3
#define COUNT_COL_PINS	2



#include <stdint.h>


typedef enum
	{
	KEY_None=-1,
	KEY_0=0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_Dot,
	KEY_ENT,
	KEY_ESC,
	KEY_Up,
	KEY_Down,
	KEY_Left,
	KEY_Right
	}Ekey;


class CKeyBoard
	{
	private:
	Ekey ScanKey(void);
	
	public:
	CKeyBoard(void){};
	Ekey GetKey(void);
	};



#endif
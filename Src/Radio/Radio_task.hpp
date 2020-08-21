

#define COUNTER_NUM		16


#include "wMBus/WMBus.h"





//��������� � ������� �� ���������
typedef struct
	{
	uint32_t	utime;	//����� ��������� ������ �� ��������
	bool mycounter;
	struct 		//������ �� wMBus ��������� ��������
		{
		uint16_t manid;	//M-field - Manufacturer: Unique User/Manufacturer ID of the meter
						//The "CEN" is coded according to EN 13757-3 as "C"=(43h-40h) = 3, "E"=5, "N"=14. Thus CEN= 32*32*3+32*5+14 = 3246d  = 0CAEh.
						//Most significant bit is null since it is a "hard" (i.e. manufacturer unique) address.
		uint32_t addr;		//A-field - Address - Manufacturer number (4 bytes - Identification-number; 1 byte - Version; 1 byte - Device type), specified in 5.4, 5.6 & 5.7 of EN 13757-3:2004
		uint8_t  ver;		//A-field - Version
		TETYPE   type;		//A-field - Device type
		}wmbus;
	uint8_t data_len;
	uint8_t data[40];
	} TSCounter;






void Radio_task(void const * argument);
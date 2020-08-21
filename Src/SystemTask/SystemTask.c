

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include "Drivers/Board/Board.h"
#include "Drivers/Keyboard/keyboard.h"

#include "SystemTask.h"




uint16_t bat_voltage;


extern CBoard Board;


CKeyBoard keyboard;

/* StartTask function */
void SystemTask(void const * argument)
{
uint8_t poweroff_timeout=0;
while(1)
{
bat_voltage = Board.GetBatVoltage();
if ((bat_voltage<3400)&&(!Board.IsCharge())) Board.PowerOn(false);

if (keyboard.GetKey()==KEY_ESC)
	{
	if ((poweroff_timeout++>2)&&(!Board.IsExtPower())) Board.PowerOn(false);
	}
else poweroff_timeout=0;

osDelay(1000);
}
}
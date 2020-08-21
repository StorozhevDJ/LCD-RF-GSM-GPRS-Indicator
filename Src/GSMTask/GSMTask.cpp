

#include "GSMSHIELD\GSM.h"


#include "GSMTask.hpp"

xQueueHandle queue_gsm_buf;


void GSM_task(void const * argument)
{
GSM gsm;

queue_gsm_buf = xQueueCreate( 64, sizeof( unsigned char ) );

gsm.begin(115200);



while(1) osDelay(1000);

}
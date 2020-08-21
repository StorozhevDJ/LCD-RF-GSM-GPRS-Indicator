
#include <string.h>

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include "Drivers/Board/Board.h"

#include "PrintScan/Parser.h"

#include "CC110L/cc110L.h"

#include "Radio_task.hpp"




TSCounter counters[16];

uint8_t radio_rx_buf[CC110l_DATA_LEN];
uint8_t radio_rx_len;
uint8_t radio_rssi;
//uint8_t radio_rssi_current[5];
xQueueHandle queue_radio_rssi_current;

/* StartDefaultTask function */
void Radio_task(void const * argument)
{
CC110l_Init();
CC110l_CmdStrobe(CC110l_SIDLE);
CC110l_CmdStrobe(CC110l_SFRX);
CC110l_CmdStrobe(CC110l_SFTX);
CC110l_CmdStrobe(CC110l_SRX);

queue_radio_rssi_current = xQueueCreate( 8, sizeof( unsigned char ) );

while(1)
{
osDelay(10);
uint8_t tmp=CC110l_GetRSSI();
xQueueSend( queue_radio_rssi_current, ( void * ) &tmp, 10);

TECC110l_MARCSTATE state=(TECC110l_MARCSTATE)CC110l_ReadReg(CC110l_MARCSTATE);
if (state==MARCSTATE_RXFIFO_OWERFLOW)
	{
	CC110l_CmdStrobe(CC110l_SIDLE);
	CC110l_CmdStrobe(CC110l_SFRX);
	CC110l_CmdStrobe(CC110l_SRX);
	}

if (radio_rx_len)
	{
	radio_rx_len=0;
	uint8_t tmp_buf[64];
	memcpy(tmp_buf, radio_rx_buf, 64);
	if (wmbCRCCalc(tmp_buf, sizeof(TSblock0)-2, CRC_CHECK)==0)
		{
		wmbCRCFree(&tmp_buf);
		TSblock0 *tmp;
		tmp=(TSblock0 *)tmp_buf;
		if ((tmp->TYPEF!=type_system_radio_converter_modem)&&
		    (tmp->TYPEF!=type_system_muc)&&
		    (tmp->TYPEF!=type_system_display))
			{
			counters[0].utime=RTC->CNTL;
			counters[0].wmbus.addr=tmp->ADDRF;
			counters[0].wmbus.manid=tmp->MANIDF;
			counters[0].wmbus.type=tmp->TYPEF;
			counters[0].wmbus.ver=tmp->VERF;
			counters[0].data_len=wmbCalcDataLen(tmp_buf);
			uint8_t start=wmbCalcDataStart(tmp_buf);
			for (uint8_t i=0; i<counters[0].data_len; i++) counters[0].data[i]=tmp_buf[start+i];
			}
		}
	}
}
}
/**************************************************/

/**
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panLoader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panLoader; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 03/03/2011
 */


#include <string.h>

#include "stm32f1xx_hal.h"

#include "cc110l.h"



CCPACKET_Type CCPACKET;

uint8_t CC110l_carrierFreq;    //Carrier frequency
uint8_t CC110l_channel;        //Frequency channel
uint8_t CC110l_syncWord[2];    //Synchronization word
uint8_t CC110l_devAddress;     //Device address





 /**
  * PATABLE
  */
//const uint8_t paTable[8] = {0xC0, 0xCE, 0x60, 0x68, 0x34, 0x17, 0x12, 0x30};
				// dBm  -37   -36   -35   -34   -33   -32   -31   -30   -29   -28   -27   -26   -25   -24   -23   -22   -21   -20   -19   -18
const uint8_t paTable[] = {0x30, 0x20, 0x10, 0x01, 0x11, 0x02, 0x03, 0x12, 0x04, 0x05, 0x13, 0x06, 0x07, 0x08, 0x09, 0x0b, 0x0c, 0x17, 0x22, 0x19,
				// dBm  -17   -16   -15   -14   -13   -12   -11   -10    -9    -8    -7    -6    -5    -4    -3    -2    -1     0     1     2
					  0x1a, 0x1b, 0x1d, 0x1e, 0x24, 0x33, 0x25, 0x34, 0x6e, 0x28, 0x29, 0x2a, 0x68, 0x57, 0x2f, 0x3c, 0x52, 0x60, 0x8d, 0xcf,
				// dBm    3     4     5     6     7     8     9    10                                 
					  0x8a, 0x87, 0xce, 0xcb, 0xc8, 0xc5, 0xc2, 0xc0};


SPI_HandleTypeDef hspi2;


/*******************************************************************************
 * init
 * Initialize CC110l
*******************************************************************************/
void CC110l_Init(void)
{  
__GPIOB_CLK_ENABLE();

GPIO_InitTypeDef GPIO_InitStruct;

/*Configure GPIO pin : CC110l_CS_Pin */
GPIO_InitStruct.Pin = CC110l_CS_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
HAL_GPIO_Init(CC110l_CS_GPIO_Port, &GPIO_InitStruct);
HAL_GPIO_WritePin(CC110l_CS_GPIO_Port, CC110l_CS_Pin, GPIO_PIN_SET);

//Configure GPIO pins : CC110L_SCK_Pin CC110L_MOSI_Pin 
GPIO_InitStruct.Pin = CC110L_SCK_Pin|CC110L_MOSI_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
HAL_GPIO_Init(CC110L_MOSI_GPIO_Port, &GPIO_InitStruct);

//Configure GPIO pins : CC110L_MISO_Pin
GPIO_InitStruct.Pin = CC110L_MISO_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(CC110L_MISO_GPIO_Port, &GPIO_InitStruct);

//Configure GPIO pins : CC110L_GDO0_Pin, CC110L_GDO2_Pin 
GPIO_InitStruct.Pin = CC110L_GDO0_Pin|CC110L_GDO2_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

hspi2.Instance = SPI2;
hspi2.Init.Mode = SPI_MODE_MASTER;
hspi2.Init.Direction = SPI_DIRECTION_2LINES;
hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
hspi2.Init.NSS = SPI_NSS_SOFT;
hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
hspi2.Init.TIMode = SPI_TIMODE_DISABLED;
hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
hspi2.Init.CRCPolynomial = 10;
HAL_SPI_Init(&hspi2);

CC110L_DESELECT;
CC110l_Reset();			// Reset CC110l
// Configure PATABLE
//CC110l_WriteBurstReg(CC110l_PATABLE, (uint8_t*)paTable[40], 2);
CC110l_WriteReg(CC110l_PATABLE, plus10dbm);

 /* Sets the priority grouping field */
HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}



uint8_t CC110L_SPISendByte(uint8_t data)
{
uint8_t ret;
HAL_SPI_TransmitReceive(&hspi2, &data, &ret, 1, 1000);
return ret;
}



uint8_t CC110L_SPISendBuf(uint8_t *data, uint8_t len)
{
return HAL_SPI_Transmit(&hspi2, data, len, 1000);
}



uint8_t CC110L_SPIGetBuf(uint8_t *data, uint8_t len)
{
return HAL_SPI_Receive(&hspi2, data, len, 1000);
}



/*******************************************************************************
 * Reset
 * Reset CC110l
*******************************************************************************/
void CC110l_Reset(void) 
{
CC110L_DESELECT;		// Deselect CC110l
delay_us(5);
CC110L_SELECT;			// Select CC110l
delay_us(10);
CC110L_DESELECT;		// Deselect CC110l
delay_us(41);
CC110L_SELECT;			// Select CC110l

while (MISO_HI);		// Wait until MISO goes low
CC110L_SPISendByte(CC110l_SRES);	// Send reset command strobe
while (MISO_HI);		// Wait until MISO goes low
CC110L_DESELECT;		// Deselect CC110l

CC110l_SetDefaultRegs();	// Reconfigure CC110l
//setRegsFromEeprom();	// Take user settings from EEPROM
}



/*******************************************************************************
 * wakeUp
 * Wake up CC110l from Power Down state
*******************************************************************************/
void CC110l_WakeUp(void)
{
CC110L_SELECT;			// Select CC110l
while (MISO_HI);		// Wait until MISO goes low
CC110L_DESELECT;		// Deselect CC110l
}



/*******************************************************************************
 * cmdStrobe
 * Send command strobe to the CC110l IC via SPI
 * 'cmd'	Command strobe
*******************************************************************************/
void CC110l_CmdStrobe(TECC110l_Strob cmd) 
{
CC110L_SELECT;				// Select CC110l
while (MISO_HI);			// Wait until MISO goes low
CC110L_SPISendByte(cmd);		// Send strobe command
CC110L_DESELECT;			// Deselect CC110l 
}



/*******************************************************************************
 * writeReg
 * Write single register into the CC110l IC via SPI
 * 'regAddr'	Register address
 * 'value'	Value to be writen
*******************************************************************************/
void CC110l_WriteReg(TECC110l_Reg regAddr, uint8_t value) 
{
CC110L_SELECT;			// Select CC110l
while (MISO_HI);		// Wait until MISO goes low
CC110L_SPISendByte(regAddr);// Send register address
CC110L_SPISendByte(value);// Send value
CC110L_DESELECT;		// Deselect CC110l
}



/*******************************************************************************
 * writeBurstReg
 * Write multiple registers into the CC110l IC via SPI
 * 'regAddr'	Register address
 * 'buffer'	Data to be writen
 * 'len'	Data length
*******************************************************************************/
void CC110l_WriteBurstReg(TECC110l_Reg regAddr, uint8_t* buffer, uint8_t len)
{
uint8_t addr;

CC110L_SELECT;				// Select CC110l
addr = regAddr | WRITE_BURST;	// Enable burst transfer
while (MISO_HI);			// Wait until MISO goes low
CC110L_SPISendByte(addr);	// Send register address

CC110L_SPISendBuf(buffer, len);

CC110L_DESELECT;			// Deselect CC110l
}



/*******************************************************************************
 * writeBurstReg
 * Write multiple registers into the CC110l IC via SPI
 * 'regAddr'	Register address
 * 'buffer'	Data to be writen
 * 'len'	Data length
*******************************************************************************/
/*void CC110l_WriteBurstPAT(uint8_t* patable, uint8_t len)
{
uint8_t addr;

CC110L_SELECT;				// Select CC110l
addr = regAddr | WRITE_BURST;	// Enable burst transfer
while (MISO_HI);			// Wait until MISO goes low
CC110L_SPISendByte(addr);	// Send register address

CC110L_SPISendBuf(buffer, len);

CC110L_DESELECT;			// Deselect CC110l
}*/



/*******************************************************************************
 * readReg
 * Read CC110l register via SPI
 * 'regAddr'	Register address
 * 'regType'	Type of register: CC110l_CONFIG_REGISTER or CC110l_STATUS_REGISTER
 * Return:
 * 	Data byte returned by the CC110l IC
*******************************************************************************/
uint8_t CC110l_ReadReg(TECC110l_Reg regAddr) 
{
uint8_t val;
if (CC110L_IS_SELECT) {CC110L_DESELECT; for (uint16_t t=0; t<10000; t++);}
CC110L_SELECT;				// Select CC110l
uint32_t timeout=HAL_GetTick()+10;
while (MISO_HI) if (HAL_GetTick()>timeout) {val=0; break;};	// Wait until MISO goes low
// Check for valid configuration register address, 0x3E refers to PATABLE
if ((regAddr <= 0x2E) || (regAddr == CC110l_PATABLE))
	// Send address + Instruction + 1 dummy byte (auto-read)
	val = (regAddr | CC110l_CONFIG_REGISTER);
else
	// Send address + Instruction + 1 dummy byte (auto-read)
	val = (regAddr | CC110l_STATUS_REGISTER);
CC110L_SPISendByte(val);	// Send register address
val = CC110L_SPISendByte(0x00);// Read result
CC110L_DESELECT;			// Deselect CC110l
return val;
}



/*******************************************************************************
 * readBurstReg
 * Read burst data from CC110l via SPI
 * 'buffer'	Buffer where to copy the result to
 * 'regAddr'	Register address
 * 'len'	Data length
*******************************************************************************/
void CC110l_ReadBurstReg(uint8_t * buffer, TECC110l_Reg regAddr, uint8_t len) 
{
uint8_t addr;

addr = regAddr | READ_BURST;
CC110L_SELECT;				// Select CC110l
while (MISO_HI);			// Wait until MISO goes low
CC110L_SPISendByte(addr);	// Send register address
CC110L_SPIGetBuf(buffer, len);
CC110L_DESELECT;			// Deselect CC110l
}



/*******************************************************************************
 * CC110l_setDefaultRegs
 * Configure CC110l registers
*******************************************************************************/
void CC110l_SetDefaultRegs(void) 
{
uint8_t defSyncWrd[] = {CC110l_DEFVAL_SYNC1, CC110l_DEFVAL_SYNC0};

CC110l_WriteReg(CC110l_IOCFG2,  CC110l_DEFVAL_IOCFG2);
CC110l_WriteReg(CC110l_IOCFG1,  CC110l_DEFVAL_IOCFG1);
CC110l_WriteReg(CC110l_IOCFG0,  CC110l_DEFVAL_IOCFG0);
CC110l_WriteReg(CC110l_FIFOTHR,  CC110l_DEFVAL_FIFOTHR);
CC110l_WriteReg(CC110l_PKTLEN,  CC110l_DEFVAL_PKTLEN);
CC110l_WriteReg(CC110l_PKTCTRL1,  CC110l_DEFVAL_PKTCTRL1);
CC110l_WriteReg(CC110l_PKTCTRL0,  CC110l_DEFVAL_PKTCTRL0);

// Set default synchronization word
CC110l_SetSyncWord(defSyncWrd, false);

// Set default device address
CC110l_SetDevAddress(CC110l_DEFVAL_ADDR, false);
// Set default frequency channel
CC110l_SetChannel(CC110l_DEFVAL_CHANNR, false);

CC110l_WriteReg(CC110l_FSCTRL1,  CC110l_DEFVAL_FSCTRL1);
CC110l_WriteReg(CC110l_FSCTRL0,  CC110l_DEFVAL_FSCTRL0);

// Set default carrier frequency = 433 MHz
CC110l_WriteReg(CC110l_FREQ2,  CC110l_DEFVAL_FREQ2_433);
CC110l_WriteReg(CC110l_FREQ1,  CC110l_DEFVAL_FREQ1_433);
CC110l_WriteReg(CC110l_FREQ0,  CC110l_DEFVAL_FREQ0_433);

CC110l_WriteReg(CC110l_MDMCFG4,  CC110l_DEFVAL_MDMCFG4);
CC110l_WriteReg(CC110l_MDMCFG3,  CC110l_DEFVAL_MDMCFG3);
CC110l_WriteReg(CC110l_MDMCFG2,  CC110l_DEFVAL_MDMCFG2);
CC110l_WriteReg(CC110l_MDMCFG1,  CC110l_DEFVAL_MDMCFG1);
CC110l_WriteReg(CC110l_MDMCFG0,  CC110l_DEFVAL_MDMCFG0);
CC110l_WriteReg(CC110l_DEVIATN,  CC110l_DEFVAL_DEVIATN);
CC110l_WriteReg(CC110l_MCSM2,  CC110l_DEFVAL_MCSM2);
CC110l_WriteReg(CC110l_MCSM1,  CC110l_DEFVAL_MCSM1);
CC110l_WriteReg(CC110l_MCSM0,  CC110l_DEFVAL_MCSM0);
CC110l_WriteReg(CC110l_FOCCFG,  CC110l_DEFVAL_FOCCFG);
CC110l_WriteReg(CC110l_BSCFG,  CC110l_DEFVAL_BSCFG);
CC110l_WriteReg(CC110l_AGCCTRL2,  CC110l_DEFVAL_AGCCTRL2);
CC110l_WriteReg(CC110l_AGCCTRL1,  CC110l_DEFVAL_AGCCTRL1);
CC110l_WriteReg(CC110l_AGCCTRL0,  CC110l_DEFVAL_AGCCTRL0);
CC110l_WriteReg(CC110l_WOREVT1,  CC110l_DEFVAL_WOREVT1);
CC110l_WriteReg(CC110l_WOREVT0,  CC110l_DEFVAL_WOREVT0);
CC110l_WriteReg(CC110l_WORCTRL,  CC110l_DEFVAL_WORCTRL);
CC110l_WriteReg(CC110l_FREND1,  CC110l_DEFVAL_FREND1);
CC110l_WriteReg(CC110l_FREND0,  CC110l_DEFVAL_FREND0);
CC110l_WriteReg(CC110l_FSCAL3,  CC110l_DEFVAL_FSCAL3);
CC110l_WriteReg(CC110l_FSCAL2,  CC110l_DEFVAL_FSCAL2);
CC110l_WriteReg(CC110l_FSCAL1,  CC110l_DEFVAL_FSCAL1);
CC110l_WriteReg(CC110l_FSCAL0,  CC110l_DEFVAL_FSCAL0);
CC110l_WriteReg(CC110l_RCCTRL1,  CC110l_DEFVAL_RCCTRL1);
CC110l_WriteReg(CC110l_RCCTRL0,  CC110l_DEFVAL_RCCTRL0);
CC110l_WriteReg(CC110l_FSTEST,  CC110l_DEFVAL_FSTEST);
CC110l_WriteReg(CC110l_PTEST,  CC110l_DEFVAL_PTEST);
CC110l_WriteReg(CC110l_AGCTEST,  CC110l_DEFVAL_AGCTEST);
CC110l_WriteReg(CC110l_TEST2,  CC110l_DEFVAL_TEST2);
CC110l_WriteReg(CC110l_TEST1,  CC110l_DEFVAL_TEST1);
CC110l_WriteReg(CC110l_TEST0,  CC110l_DEFVAL_TEST0);
}



void CC110l_ReceiveOn()
{
/*EIMSK |= (1<<INT1);  //Enable INT0
EICRA |= (1<<ISC11); //Trigger on falling edge of INT0*/
//setRxState();
CC110l_CmdStrobe(CC110l_SRX);
}



void CC110l_ReceiveOff()
{
//EIMSK &=~ (1<<INT1);  //Enable INT0
//setIdleState();
CC110l_CmdStrobe(CC110l_SIDLE);
}



/*******************************************************************************
 * setSyncWord
 * Set synchronization word
 * 'sync'	Synchronization word
 * 'save' If TRUE, save parameter in EEPROM
*******************************************************************************/
void CC110l_SetSyncWord(uint8_t *sync, uint8_t save) 
{
if ((CC110l_syncWord[0] != sync[0]) || (CC110l_syncWord[1] != sync[1]))
	{
	CC110l_WriteReg(CC110l_SYNC1, sync[0]);
	CC110l_WriteReg(CC110l_SYNC0, sync[1]);
	memcpy(CC110l_syncWord, sync, sizeof(CC110l_syncWord));
	// Save in EEPROM
	/*if (save)
		{
		 EEPROM.write(EEPROM_SYNC_WORD, sync[0]);
		 EEPROM.write(EEPROM_SYNC_WORD + 1, sync[1]);
		}*/
	}
}



/*******************************************************************************
 * setDevAddress
 * Set device address
 * 'addr'	Device address
 * 'save' If TRUE, save parameter in EEPROM
*******************************************************************************/
void CC110l_SetDevAddress(uint8_t addr, uint8_t save) 
{
if (CC110l_devAddress != addr)
	{
	CC110l_WriteReg(CC110l_ADDR, addr);
	CC110l_devAddress = addr;
	// Save in EEPROM
	//if (save)EEPROM.write(EEPROM_DEVICE_ADDR, addr);  
	}
}



/*******************************************************************************
 * CC110l_GetRSSI
 * Get RSSI
*******************************************************************************/
int8_t CC110l_GetRSSI(void)
{
TECC110l_MARCSTATE state=(TECC110l_MARCSTATE)CC110l_ReadReg(CC110l_MARCSTATE);
if ((state==MARCSTATE_RX)||(state==MARCSTATE_RX_END))
	{
	uint8_t rssi_dec; 
	int16_t rssi_dBm; 
	rssi_dec = CC110l_ReadReg(CC110l_RSSI); 
	if(rssi_dec >= 128) rssi_dBm = (int16_t)((int16_t)( rssi_dec - 256) / 2) - 72; 
	else rssi_dBm = (rssi_dec / 2) - 72;
	return rssi_dBm;
	}
else return 0;
}



/*******************************************************************************
 * setChannel
 * Set frequency channel
 * 'chnl'	Frequency channel
 * 'save' If TRUE, save parameter in EEPROM
*******************************************************************************/
void CC110l_SetChannel(uint8_t chnl, uint8_t save) 
{
if (CC110l_channel != chnl)
	{
	CC110l_WriteReg(CC110l_CHANNR,  chnl);
	CC110l_channel = chnl;
	// Save in EEPROM
	//if (save)EEPROM.write(EEPROM_FREQ_CHANNEL, chnl);
	}
}



/*******************************************************************************
 * setCarrierFreq
 * Set carrier frequency
 * 'freq'	New carrier frequency
*******************************************************************************/
void CC110l_SetCarrierFreq(uint8_t freq)
{
switch(freq)
	{
	case CFREQ_433:
		CC110l_WriteReg(CC110l_FREQ2,  CC110l_DEFVAL_FREQ2_433);
		CC110l_WriteReg(CC110l_FREQ1,  CC110l_DEFVAL_FREQ1_433);
		CC110l_WriteReg(CC110l_FREQ0,  CC110l_DEFVAL_FREQ0_433);
	break;

	case CFREQ_868:
		CC110l_WriteReg(CC110l_FREQ2,  CC110l_DEFVAL_FREQ2_868);
		CC110l_WriteReg(CC110l_FREQ1,  CC110l_DEFVAL_FREQ1_868);
		CC110l_WriteReg(CC110l_FREQ0,  CC110l_DEFVAL_FREQ0_868);
	break;
	default:
		CC110l_WriteReg(CC110l_FREQ2,  CC110l_DEFVAL_FREQ2_433);
		CC110l_WriteReg(CC110l_FREQ1,  CC110l_DEFVAL_FREQ1_433);
		CC110l_WriteReg(CC110l_FREQ0,  CC110l_DEFVAL_FREQ0_433);
	break;
	}
}



/*******************************************************************************
 * setPowerDownState
 * 
 * Put CC110l into power-down state
*******************************************************************************/
void CC110l_SetPowerDownState() 
{
// Comming from RX state, we need to enter the IDLE state first
CC110l_CmdStrobe(CC110l_SIDLE);
// Enter Power-down state
CC110l_CmdStrobe(CC110l_SPWD);
}



/*******************************************************************************
 * sendData
 * Send data packet via RF
 * 'packet'	Packet to be transmitted
 *  Return:
 *    True if the transmission succeeds
 *    False otherwise
*******************************************************************************/
uint8_t CC110l_SendData(CCPACKET_Type *packet)
{
//  uint8_t marcState;
CC110l_ReceiveOff();
//CC110l_WriteReg(CC110l_TXFIFO, packet->length);
CC110l_WriteBurstReg(CC110l_TXFIFO, packet->data, packet->length);//write data to send
CC110l_CmdStrobe(CC110l_STX);						//start send
uint32_t timeout=HAL_GetTick()+100;
while (!GDO0_HI) if (HAL_GetTick()>timeout) break;	// Wait for GDO0 to be set -> sync transmitted  
while (GDO0_HI);								// Wait for GDO0 to be cleared -> end of packet
CC110l_CmdStrobe(CC110l_SFTX);					//flush TXfifo

/*  // Enter RX state
  setRxState();
USART_SendString("\r1=");
//USART_SendChar(readStatusReg(CC110l_MARCSTATE) & 0x1F);
  // Check that the RX state has been entered
  while ((readStatusReg(CC110l_MARCSTATE) & 0x1F) != 0x0D){USART_SendChar(readStatusReg(CC110l_MARCSTATE) & 0x1F);};
  //delay(ms,1);  
  delay_us(1500);
  //delay(us,500);
USART_SendString("1\r");
  // Set data length at the first position of the TX FIFO
  CC110l_WriteReg(CC110l_TXFIFO,  CCPACKET.length);
  // Write data into the TX FIFO
  CC110l_writeBurstReg(CC110l_TXFIFO, CCPACKET.data, CCPACKET.length);
USART_SendString("2\r");
  // CCA enabled: will enter TX state only if the channel is clear
  //cmdStrobe(CC110l_STX);
  setTxState();

  // Check that TX state is being entered (state = RXTX_SETTLING)
  marcState = readStatusReg(CC110l_MARCSTATE) & 0x1F;
  if((marcState != 0x13) && (marcState != 0x14) && (marcState != 0x15))
  {
    setIdleState();       // Enter IDLE state
    flushTxFifo();        // Flush Tx FIFO
    setRxState();         // Back to RX state
    return false;
  }
USART_SendString("3\r");
  // Wait for the sync word to be transmitted
  while(!GDO0_HI);
USART_SendString("4\r");
  // Wait until the end of the packet transmission
  while(GDO0_HI);
USART_SendString("5\r");
  // Enter back into RX state
  setRxState();

  // Check that the TX FIFO is empty
  if((readStatusReg(CC110l_TXBYTES) & 0x7F) == 0) return true;
USART_SendString("6\r");*/
CC110l_ReceiveOn();
  return false;
}



/*******************************************************************************
 * sendData
 * Send data packet via RF
 * 'packet'	Packet to be transmitted
 *  Return:
 *    True if the transmission succeeds
 *    False otherwise
*******************************************************************************/
uint8_t CC110l_SendData(void *data, uint8_t len)
{
uint8_t ret=true;
//CC110l_ReceiveOff();
//setRxState();//test
CC110l_CmdStrobe(CC110l_SFTX);					//flush TXfifo
CC110l_WriteReg(CC110l_PKTLEN,  len);
CC110l_WriteBurstReg(CC110l_TXFIFO, (uint8_t *) data, len);//write data to send
CC110l_CmdStrobe(CC110l_STX);						//start send	
uint32_t timeout=HAL_GetTick()+3000;
while (!GDO2_HI) if (HAL_GetTick()>timeout) {ret=false; break;};	// Wait for GDO2 to be set -> sync transmitted  
while ( GDO2_HI) if (HAL_GetTick()>timeout) {ret=false; break;};	// Wait for GDO2 to be cleared -> end of packet

uint8_t tmp;
tmp=CC110l_ReadReg(CC110l_MARCSTATE);
if ((tmp==MARCSTATE_TXFIFO_UNDERFLOW)||(tmp==MARCSTATE_TX)) ret=false;

CC110l_WriteReg(CC110l_PKTLEN,  CC110l_DEFVAL_PKTLEN);
CC110l_CmdStrobe(CC110l_SFTX);					//flush TXfifo

/*  // Enter RX state
  setRxState();
USART_SendString("\r1=");
//USART_SendChar(readStatusReg(CC110l_MARCSTATE) & 0x1F);
  // Check that the RX state has been entered
  while ((readStatusReg(CC110l_MARCSTATE) & 0x1F) != 0x0D){USART_SendChar(readStatusReg(CC110l_MARCSTATE) & 0x1F);};
  //delay(ms,1);  
  delay_us(1500);
  //delay(us,500);
USART_SendString("1\r");
  // Set data length at the first position of the TX FIFO
  CC110l_WriteReg(CC110l_TXFIFO,  CCPACKET.length);
  // Write data into the TX FIFO
  CC110l_writeBurstReg(CC110l_TXFIFO, CCPACKET.data, CCPACKET.length);
USART_SendString("2\r");
  // CCA enabled: will enter TX state only if the channel is clear
  //cmdStrobe(CC110l_STX);
  setTxState();

  // Check that TX state is being entered (state = RXTX_SETTLING)
  marcState = readStatusReg(CC110l_MARCSTATE) & 0x1F;
  if((marcState != 0x13) && (marcState != 0x14) && (marcState != 0x15))
  {
    setIdleState();       // Enter IDLE state
    flushTxFifo();        // Flush Tx FIFO
    setRxState();         // Back to RX state
    return false;
  }
USART_SendString("3\r");
  // Wait for the sync word to be transmitted
  while(!GDO0_HI);
USART_SendString("4\r");
  // Wait until the end of the packet transmission
  while(GDO0_HI);
USART_SendString("5\r");
  // Enter back into RX state
  setRxState();

  // Check that the TX FIFO is empty
  if((readStatusReg(CC110l_TXBYTES) & 0x7F) == 0) return true;
USART_SendString("6\r");*/
//CC110l_ReceiveOn();
return ret;
}



/*******************************************************************************
 * receiveData
 * Read data packet from RX FIFO
 * Return:
 * 	Amount fo bytes received
 * 'packet'	Container for the packet received
*******************************************************************************/
char state;
uint8_t CC110l_ReceiveData(CCPACKET_Type *packet)
{
  uint8_t val;

  // Rx FIFO overflow?
  /*if ((readStatusReg(CC110l_MARCSTATE) & 0x1F) == 0x11)
  {
    setIdleState();       // Enter IDLE state
    flushRxFifo();        // Flush Rx FIFO
    packet->length = 0;
  }
  // Any byte waiting to be read?
  else*/ /*if (readStatusReg(CC110l_RXBYTES) & 0x7F)
  {USART_SendString("\rbyte waiting to be read=");
    // Read data length
    packet->length = readConfigReg(CC110l_RXFIFO);USART_SendChar(packet->length);
    // If packet is too long
    if (packet->length > CC110l_DATA_LEN)
      packet->length = 0;   // Discard packet
    else
    {USART_SendString("\rRead data packet");
      // Read data packet
      CC110l_readBurstReg(packet->data, CC110l_RXFIFO, packet->length);
      // Read RSSI
      packet->rssi = readConfigReg(CC110l_RXFIFO);
      // Read LQI and CRC_OK
      val = readConfigReg(CC110l_RXFIFO);
      packet->lqi = val & 0x7F;
      packet->crc_ok = val&0x80;  //read 7 bit
    }
  }
  else packet->length = 0;

  // Back to RX state
  setRxState();

  return packet->length;*/



	/*char status[2];
	char size;

	if(readStatusReg(CC110l_RXBYTES) & 0x7F)
	{USART_SendString("\rbyte waiting to be read=");
		size=readConfigReg(CC110l_RXFIFO);USART_SendChar(size);
		CC110l_readBurstReg(packet->data, CC110l_RXFIFO, size);
		CC110l_readBurstReg(status, CC110l_RXFIFO, 2);
		CC110l_cmdStrobe(CC110l_SFRX);
		return size;
	}
	else
	{setRxState();
		CC110l_cmdStrobe(CC110l_SFRX);
		return 0;
	}*/
	
	
	
  
// Rx FIFO overflow?
/*if ((readStatusReg(CC110l_MARCSTATE) & 0x1F) == 0x11)
	{
	USART_SendString("\rCC110l_MARCSTATE=0x11");
	setIdleState();       // Enter IDLE state
	flushRxFifo();        // Flush Rx FIFO
	packet->length = 0;
	return 0;
	}
else if ((readStatusReg(CC110l_MARCSTATE) & 0x1F) ==0x0D) return 0;*/

// char val;



char rxBytes = CC110l_ReadReg(CC110l_RXBYTES);


// Any byte waiting to be read and no overflow?
if (rxBytes & 0x7F && !(rxBytes & 0x80))
	{//USART_SendString("\rbyte waiting to be read=");
	// Read data length
	packet->length = rxBytes & 0x7F;
	// If packet is too long
	if (packet->length > CC110l_DATA_LEN) packet->length = 0;   // Discard packet
	else
		{
		// Read data packet
		CC110l_ReadBurstReg(packet->data, CC110l_RXFIFO, packet->length);
//		CC110l_readBurstReg(packet->data, CC110l_RXFIFO, CC110l_DATA_LEN);
		// Read RSSI
		packet->rssi = CC110l_ReadReg(CC110l_RXFIFO);
		// Read LQI and CRC_OK
		val = CC110l_ReadReg(CC110l_RXFIFO);
		packet->lqi = val & 0x7F;
		packet->crc_ok = val&0x80;
		}
	}
else packet->length = 0;

setIdleState();       // Enter IDLE state
flushRxFifo();        // Flush Rx FIFO
//cmdStrobe(CC110l_SCAL);

// Back to RX state
setRxState();

return packet->length;
}


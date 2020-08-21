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

#ifndef _CC110l_H
#define _CC110l_H



/**
 * HW
**/
#define CC110L_SELECT	CC110l_CS_GPIO_Port->BRR =CC110l_CS_Pin
#define CC110L_DESELECT	CC110l_CS_GPIO_Port->BSRR=CC110l_CS_Pin
#define CC110L_IS_SELECT	(!(CC110l_CS_GPIO_Port->ODR&CC110l_CS_Pin))
#define MISO_HI		(CC110L_MISO_GPIO_Port->IDR&CC110L_MISO_Pin)
#define GDO0_HI		(CC110L_GDO0_GPIO_Port->IDR&CC110L_GDO0_Pin)
#define GDO1_HI		(CC110L_MISO_GPIO_Port->IDR&CC110L_MISO_Pin)
#define GDO2_HI		(CC110L_GDO2_GPIO_Port->IDR&CC110L_GDO2_Pin)
//#define SPI_TX_RX(x)	CC110L_SPISend(x)

#define delay_us(x)		for (uint32_t t=0; t<x; t++) __no_operation()



/**
 * Buffer and data lengths
 */
#define CC110l_BUFFER_LEN        67
#define CC110l_DATA_LEN          CC110l_BUFFER_LEN - 3

/**
 * struct: CCPACKET
 * 
 * Description:
 * CC110l data struct
 */
typedef struct struct_CCPACKET
{		
        uint8_t length;                //Data length
        uint8_t data[CC110l_DATA_LEN]; //Data buffer
        uint8_t crc_ok;                //CRC OK flag
        uint8_t rssi;                  //Received Strength Signal Indication
        uint8_t lqi;                   //Link Quality Index
}CCPACKET_Type;

extern CCPACKET_Type CCPACKET;

/**
 * Carrier frequencies
 */
//#define  CFREQ = 433



/**
 * Carrier frequencies
 */
#define CFREQ_433	1
#define CFREQ_868	2
#define CFREQ_915	3    

/**
 * Frequency channels
 */
#define NUMBER_OF_FCHANNELS		1



typedef enum
	{
	plus10dbm=0xc0,
	plus5dbm=0x84,
	plus0dbm=0x51,
	minus10dbm=0x34,
	minus63dbm=0
	} TErf_power;


/**
 * Type of register
 */
#define CC110l_CONFIG_REGISTER	READ_SINGLE
#define CC110l_STATUS_REGISTER	READ_BURST

/**
 * Type of transfers
 */
//#define WRITE_BURST				0x3E
#define WRITE_BURST				0x40
#define READ_SINGLE				0x80
#define READ_BURST				0xC0


/**
 * CC110l configuration registers
 */
typedef enum
	{
	CC110l_IOCFG2=0x00,		// GDO2 Output Pin Configuration
	CC110l_IOCFG1=0x01,		// GDO1 Output Pin Configuration
	CC110l_IOCFG0=0x02,		// GDO0 Output Pin Configuration
	CC110l_FIFOTHR=0x03,	// RX FIFO and TX FIFO Thresholds
	CC110l_SYNC1=0x04,		// Sync Word, High Byte
	CC110l_SYNC0=0x05,		// Sync Word, Low Byte
	CC110l_PKTLEN=0x06,		// Packet Length
	CC110l_PKTCTRL1=0x07,	// Packet Automation Control
	CC110l_PKTCTRL0=0x08,	// Packet Automation Control
	CC110l_ADDR=0x09,		// Device Address
	CC110l_CHANNR=0x0A,		// Channel Number
	CC110l_FSCTRL1=0x0B,	// Frequency Synthesizer Control
	CC110l_FSCTRL0=0x0C,	// Frequency Synthesizer Control
	CC110l_FREQ2=0x0D,		// Frequency Control Word, High Byte
	CC110l_FREQ1=0x0E,		// Frequency Control Word, Middle Byte
	CC110l_FREQ0=0x0F,		// Frequency Control Word, Low Byte
	CC110l_MDMCFG4=0x10,	// Modem Configuration
	CC110l_MDMCFG3=0x11,	// Modem Configuration
	CC110l_MDMCFG2=0x12,	// Modem Configuration
	CC110l_MDMCFG1=0x13,	// Modem Configuration
	CC110l_MDMCFG0=0x14,	// Modem Configuration
	CC110l_DEVIATN=0x15,	// Modem Deviation Setting
	CC110l_MCSM2=0x16,		// Main Radio Control State Machine Configuration
	CC110l_MCSM1=0x17,		// Main Radio Control State Machine Configuration
	CC110l_MCSM0=0x18,		// Main Radio Control State Machine Configuration
	CC110l_FOCCFG=0x19,		// Frequency Offset Compensation Configuration
	CC110l_BSCFG=0x1A,		// Bit Synchronization Configuration
	CC110l_AGCCTRL2=0x1B,	// AGC Control
	CC110l_AGCCTRL1=0x1C,	// AGC Control
	CC110l_AGCCTRL0=0x1D,	// AGC Control
	CC110l_WOREVT1=0x1E,	// High Byte Event0 Timeout
	CC110l_WOREVT0=0x1F,	// Low Byte Event0 Timeout
	CC110l_WORCTRL=0x20,	// Wake On Radio Control
	CC110l_FREND1=0x21,		// Front End RX Configuration
	CC110l_FREND0=0x22,		// Front End TX Configuration
	CC110l_FSCAL3=0x23,		// Frequency Synthesizer Calibration
	CC110l_FSCAL2=0x24,		// Frequency Synthesizer Calibration
	CC110l_FSCAL1=0x25,		// Frequency Synthesizer Calibration
	CC110l_FSCAL0=0x26,		// Frequency Synthesizer Calibration
	CC110l_RCCTRL1=0x27,	// RC Oscillator Configuration
	CC110l_RCCTRL0=0x28,	// RC Oscillator Configuration
	CC110l_FSTEST=0x29,		// Frequency Synthesizer Calibration Control
	CC110l_PTEST=0x2A,		// Production Test
	CC110l_AGCTEST=0x2B,	// AGC Test
	CC110l_TEST2=0x2C,		// Various Test Settings
	CC110l_TEST1=0x2D,		// Various Test Settings
	CC110l_TEST0=0x2E,		// Various Test Settings
/**
 * Status registers
 */
	CC110l_PARTNUM=0x30,	// Chip ID
	CC110l_VERSION=0x31,	// Chip ID
	CC110l_FREQEST=0x32,	// Frequency Offset Estimate from Demodulator
	CC110l_LQI=0x33,		// Demodulator Estimate for Link Quality
	CC110l_RSSI=0x34,		// Received Signal Strength Indication
	CC110l_MARCSTATE=0x35,	// Main Radio Control State Machine State
	CC110l_WORTIME1=0x36,	// High Byte of WOR Time
	CC110l_WORTIME0=0x37,	// Low Byte of WOR Time
	CC110l_PKTSTATUS=0x38,	// Current GDOx Status and Packet Status
	CC110l_VCO_VC_DAC=0x39,	// Current Setting from PLL Calibration Module
	CC110l_TXBYTES=0x3A,	// Underflow and Number of Bytes
	CC110l_RXBYTES=0x3B,	// Overflow and Number of Bytes
	CC110l_RCCTRL1_STATUS=0x3C,// Last RC Oscillator Calibration Result
	CC110l_RCCTRL0_STATUS=0x3D,// Last RC Oscillator Calibration Result
	//} TECC110l_Reg;
	CC110l_PATABLE=0x3E,		// PATABLE address
	CC110l_TXFIFO=0x3F,		// TX FIFO address
	CC110l_RXFIFO=0x3F		// RX FIFO address
	} TECC110l_Reg;

/**
 * PATABLE & FIFO's
 */
/*#define CC110l_PATABLE			0x3E		// PATABLE address
#define CC110l_TXFIFO			0x3F		// TX FIFO address
#define CC110l_RXFIFO			0x3F		// RX FIFO address*/

/**
 * Command strobes
 */
typedef enum
	{
	CC110l_SRES=0x30,		// Reset CC110l chip
	CC110l_SFSTXON=0x31,	// Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1). If in RX (with CCA):
						// Go to a wait state where only the synthesizer is running (for quick RX / TX turnaround).
	CC110l_SXOFF=0x32,		// Turn off crystal oscillator
	CC110l_SCAL=0x33,		// Calibrate frequency synthesizer and turn it off. SCAL can be strobed from IDLE mode without
						// setting manual calibration mode (MCSM0.FS_AUTOCAL=0)
	CC110l_SRX=0x34,		// Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1
	CC110l_STX=0x35,		// In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1.
						// If in RX state and CCA is enabled: Only go to TX if channel is clear
	CC110l_SIDLE=0x36,		// Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
	CC110l_SWOR=0x38,		// Start automatic RX polling sequence (Wake-on-Radio) as described in Section 19.5 if
						// WORCTRL.RC_PD=0
	CC110l_SPWD=0x39,		// Enter power down mode when CSn goes high
	CC110l_SFRX=0x3A,		// Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
	CC110l_SFTX=0x3B,		// Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
	CC110l_SWORRST=0x3C,	// Reset real time clock to Event1 value
	CC110l_SNOP=0x3D		// No operation. May be used to get access to the chip status byte
	}TECC110l_Strob;



/**
 * MARCSTATE - Main Radio Control State Machine State
 */
typedef enum
	{
	MARCSTATE_SLEEP=0,		//0  (0x00)
	MARCSTATE_IDLE,		//1  (0x01)
	MARCSTATE_XOFF,		//2  (0x02)
	MARCSTATE_VCOON_MC,		//3  (0x03)
	MARCSTATE_REGON_MC,		//4  (0x04)
	MARCSTATE_MANCAL,		//5  (0x05)
	MARCSTATE_VCOON,		//6  (0x06)
	MARCSTATE_REGON,		//7  (0x07)
	MARCSTATE_STARTCAL,		//8  (0x08)
	MARCSTATE_BWBOOST,		//9  (0x09)
	MARCSTATE_FS_LOCK,		//10 (0x0A)
	MARCSTATE_IFADCON,		//11 (0x0B)
	MARCSTATE_ENDCAL,		//12 (0x0C)
	MARCSTATE_RX,			//13 (0x0D)
	MARCSTATE_RX_END,		//14 (0x0E)
	MARCSTATE_RX_RST,		//15 (0x0F)
	MARCSTATE_TXRX_SWITCH,	//16 (0x10)
	MARCSTATE_RXFIFO_OWERFLOW,//17 (0x11)
	MARCSTATE_FSTXON,		//18 (0x12)
	MARCSTATE_TX,			//19 (0x13)
	MARCSTATE_TX_END,		//20 (0x14)
	MARCSTATE_RXTX_SWITCH,	//21 (0x15)
	MARCSTATE_TXFIFO_UNDERFLOW//22 (0x16)
	}TECC110l_MARCSTATE;



/**
 * CC110l configuration registers - Default values extracted from SmartRF Studio
 *
 * Configuration:
 *
 * Deviation = 20.629883 
 * Base frequency = 867.999939 
 * Carrier frequency = 867.999939 
 * Channel number = 0 
 * Carrier frequency = 867.999939 
 * Modulated = true 
 * Modulation format = GFSK 
 * Manchester enable = false
 * Data whitening = off
 * Sync word qualifier mode = 30/32 sync word bits detected 
 * Preamble count = 4 
 * Channel spacing = 199.951172 
 * Carrier frequency = 867.999939 
 * Data rate = 38.3835 Kbps
 * RX filter BW = 101.562500 
 * Data format = Normal mode 
 * Length config = Variable packet length mode. Packet length configured by the first byte after sync word 
 * CRC enable = true 
 * Packet length = 255 
 * Device address = 1 
 * Address config = Enable address check
 * Append status = Append two status bytes to the payload of the packet. The status bytes contain RSSI and
 * LQI values, as well as CRC OK
 * CRC autoflush = false 
 * PA ramping = false 
 * TX power = 12
 * GDO0 mode = Asserts when sync word has been sent / received, and de-asserts at the end of the packet.
 * In RX, the pin will also de-assert when a packet is discarded due to address or maximum length filtering
 * or when the radio enters RXFIFO_OVERFLOW state. In TX the pin will de-assert if the TX FIFO underflows
 * Settings optimized for low current consumption
 */
#define CC110l_DEFVAL_IOCFG2		0x0E		// GDO2 Output Pin Configuration
#define CC110l_DEFVAL_IOCFG1		0x06		// GDO1 Output Pin Configuration
#define CC110l_DEFVAL_IOCFG0		0x06		// GDO0 Output Pin Configuration
#define CC110l_DEFVAL_FIFOTHR		0x47		// RX FIFO and TX FIFO Thresholds
#define CC110l_DEFVAL_SYNC1		0xF6		// Synchronization word, high byte
#define CC110l_DEFVAL_SYNC0		0x8D		// Synchronization word, low byte
#define CC110l_DEFVAL_PKTLEN		0x40		// Packet Length
#define CC110l_DEFVAL_PKTCTRL1	0x40		// Packet Automation Control
#define CC110l_DEFVAL_PKTCTRL0	0x00		// Packet Automation Control
#define CC110l_DEFVAL_ADDR		0x00		// Device Address
#define CC110l_DEFVAL_CHANNR		0x00		// Channel Number
#define CC110l_DEFVAL_FSCTRL1		0x06		// Frequency Synthesizer Control
#define CC110l_DEFVAL_FSCTRL0		0x00		// Frequency Synthesizer Control
   
// Carrier frequency = 434 MHz
#define CC110l_DEFVAL_FREQ2_433	0x10		// Frequency Control Word, High Byte
#define CC110l_DEFVAL_FREQ1_433	0xAF		// Frequency Control Word, Middle Byte
#define CC110l_DEFVAL_FREQ0_433	0xB6		// Frequency Control Word, Low Byte   
   
// Carrier frequency = 868 MHz
#define CC110l_DEFVAL_FREQ2_868	0x21		// Frequency Control Word, High Byte
#define CC110l_DEFVAL_FREQ1_868	0x62		// Frequency Control Word, Middle Byte
#define CC110l_DEFVAL_FREQ0_868	0x76		// Frequency Control Word, Low Byte

#define CC110l_DEFVAL_MDMCFG4		0xF6		// Modem Configuration
#define CC110l_DEFVAL_MDMCFG3		0x83		// Modem Configuration
#define CC110l_DEFVAL_MDMCFG2		0x02		// Modem Configuration
#define CC110l_DEFVAL_MDMCFG1		0x51		// Modem Configuration
#define CC110l_DEFVAL_MDMCFG0		0x2E		// Modem Configuration
#define CC110l_DEFVAL_DEVIATN		0x16		// Modem Deviation Setting
#define CC110l_DEFVAL_MCSM2		0x07		// Main Radio Control State Machine Configuration
//#define CC110l_DEFVAL_MCSM1		0x30		// Main Radio Control State Machine Configuration
#define CC110l_DEFVAL_MCSM1		0x2C		// Main Radio Control State Machine Configuration
#define CC110l_DEFVAL_MCSM0		0x10		// Main Radio Control State Machine Configuration
#define CC110l_DEFVAL_FOCCFG		0x16		// Frequency Offset Compensation Configuration
#define CC110l_DEFVAL_BSCFG		0x6C		// Bit Synchronization Configuration
#define CC110l_DEFVAL_AGCCTRL2	0x43		// AGC Control
#define CC110l_DEFVAL_AGCCTRL1	0x40		// AGC Control
#define CC110l_DEFVAL_AGCCTRL0	0x91		// AGC Control
#define CC110l_DEFVAL_WOREVT1		0x87		// High Byte Event0 Timeout
#define CC110l_DEFVAL_WOREVT0		0x6B		// Low Byte Event0 Timeout
#define CC110l_DEFVAL_WORCTRL		0xFB		// Wake On Radio Control
#define CC110l_DEFVAL_FREND1		0x56		// Front End RX Configuration
#define CC110l_DEFVAL_FREND0		0x10		// Front End TX Configuration
#define CC110l_DEFVAL_FSCAL3		0xE9		// Frequency Synthesizer Calibration
#define CC110l_DEFVAL_FSCAL2		0x2A		// Frequency Synthesizer Calibration
#define CC110l_DEFVAL_FSCAL1		0x00		// Frequency Synthesizer Calibration
#define CC110l_DEFVAL_FSCAL0		0x1F		// Frequency Synthesizer Calibration
//#define CC110l_DEFVAL_RCCTRL1		0x41		// RC Oscillator Configuration
//#define CC110l_DEFVAL_RCCTRL0		0x00		// RC Oscillator Configuration
#define CC110l_DEFVAL_RCCTRL1		0x00		// RC Oscillator Configuration
#define CC110l_DEFVAL_RCCTRL0		0x00		// RC Oscillator Configuration
#define CC110l_DEFVAL_FSTEST		0x59		// Frequency Synthesizer Calibration Control
#define CC110l_DEFVAL_PTEST		0x7F		// Production Test
#define CC110l_DEFVAL_AGCTEST		0x3F		// AGC Test
#define CC110l_DEFVAL_TEST2		0x81		// Various Test Settings
#define CC110l_DEFVAL_TEST1		0x35		// Various Test Settings
#define CC110l_DEFVAL_TEST0		0x09		// Various Test Settings



/**
 * Macros
 */
// Enter Rx state
#define setRxState()			CC110l_CmdStrobe(CC110l_SRX)
// Enter Tx state
#define setTxState()			CC110l_CmdStrobe(CC110l_STX)
// Enter IDLE state
#define setIdleState()			CC110l_CmdStrobe(CC110l_SIDLE)
// Flush Rx FIFO
#define flushRxFifo()			CC110l_CmdStrobe(CC110l_SFRX)
// Flush Tx FIFO
#define flushTxFifo()			CC110l_CmdStrobe(CC110l_SFTX)
// Disable address check
#define disableAddressCheck()		CC110l_WriteReg(CC110l_PKTCTRL1, 0x04)
// Enable address check
#define enableAddressCheck()		CC110l_WriteReg(CC110l_PKTCTRL1, 0x06)
// Disable CCA
#define disableCCA()			CC110l_WriteReg(CC110l_MCSM1, 0)
// Enable CCA
#define enableCCA()				CC110l_WriteReg(CC110l_MCSM1, CC110l_DEFVAL_MCSM1)

// Read CC110l Config register
#define readConfigReg(regAddr)	CC110l_ReadReg(regAddr, CC110l_CONFIG_REGISTER)
// Read CC110l Status register
#define readStatusReg(regAddr)	CC110l_ReadReg(regAddr, CC110l_STATUS_REGISTER)


    
/**
* init
* Initialise CC110l
*/
void CC110l_Init(void);

/**
* writeBurstReg
* Write multiple registers into the CC110l IC via SPI
* 'regAddr'	Register address
* 'buffer'	Data to be writen
* 'len'	Data length
*/
void CC110l_WriteBurstReg(TECC110l_Reg regAddr, uint8_t* buffer, uint8_t len);

/**
* readReg
* Read CC110l register via SPI
* 'regAddr'	Register address
* 'regType'	Type of register: CC110l_CONFIG_REGISTER or CC110l_STATUS_REGISTER
* Return:
* 	Data byte returned by the CC110l IC
*/
uint8_t CC110l_ReadReg(TECC110l_Reg regAddr);

/**
* readBurstReg
* Read burst data from CC110l via SPI
* 'buffer'	Buffer where to copy the result to
* 'regAddr'	Register address
* 'len'	Data length
*/
void CC110l_ReadBurstReg(uint8_t * buffer, TECC110l_Reg regAddr, uint8_t len);

/**
* setDefaultRegs
* 
* Configure CC110l registers
*/
void CC110l_SetDefaultRegs(void);

/**
* setRegsFromEeprom
* Set registers from EEPROM
*/
//void CC110l_setRegsFromEeprom(void);    

/**
* wakeUp
* Wake up CC110l from Power Down state
*/
void CC110l_WakeUp(void);

/**
* writeReg
* 
* Write single register into the CC110l IC via SPI
* 
* 'regAddr'	Register address
* 'value'	Value to be writen
*/
void CC110l_WriteReg(TECC110l_Reg regAddr, uint8_t value);

/**
* cmdStrobe
* Send command strobe to the CC110l IC via SPI
* 'cmd'	Command strobe
*/
void CC110l_CmdStrobe(TECC110l_Strob cmd);

/**
* reset
* Reset CC110l
*/
void CC110l_Reset(void);

/**
* setSyncWord
* Set synchronization word
* 'sync'	Synchronization word
* 'save' If TRUE, save parameter in EEPROM
*/
void CC110l_SetSyncWord(uint8_t *sync, uint8_t save);

/**
* setDevAddress
* Set device address
* 'addr'	Device address
* 'save' If TRUE, save parameter in EEPROM
*/
void CC110l_SetDevAddress(uint8_t addr, uint8_t save);

/**
 * CC110l_GetRSSI
 * Get RSSI
*/
int8_t CC110l_GetRSSI(void);

/**
* setCarrierFreq
* 
* Set carrier frequency
* 
* 'freq'	New carrier frequency
*/
void CC110l_SetCarrierFreq(uint8_t freq);

/**
* setChannel
* Set frequency channel
* 'chnl'	Frequency channel
* 'save' If TRUE, save parameter in EEPROM
*/
void CC110l_SetChannel(uint8_t chnl, uint8_t save);

/**
* setPowerDownState
* 
* Put CC110l into power-down state
*/
void CC110l_SetPowerDownState();

/**
* sendData
* Send data packet via RF
* 'packet'	Packet to be transmitted
*  Return:
*    True if the transmission succeeds
*    False otherwise
*/
uint8_t CC110l_SendData(CCPACKET_Type *packet);

/**
* sendData
* Send data packet via RF
* void *data	Pointer to data to be transmitted
* uint8_t len	Lenght of dato to transmit
*  Return:
*    True if the transmission succeeds
*    False otherwise
*/
uint8_t CC110l_SendData(void *data, uint8_t len);

/**
* receiveData
* Read data packet from RX FIFO
* Return:
* 	Amount of bytes received
* 'packet'	Container for the packet received
*/
uint8_t CC110l_ReceiveData(CCPACKET_Type *packet);


void CC110l_ReceiveOn();

#endif

    
    
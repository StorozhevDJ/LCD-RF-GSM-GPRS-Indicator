#ifndef WMBUS_H
#define WMBUS_H


#define MAN_ID(X,Y,Z) ((X-64)*32*32+(Y-64)*32+(Z-64))
#define MAN_ID_TO_STR(S)	(((S/(32*32))&0x1f)+64), (((S/32)&0x1f)+64), ((S&0x1f)+64)

#define wmbPacketLength(LField)    ((LField+3)+2*((LField+6)/16))
#define wmbLFields(PacketLength)  (PacketLength - (3+2*((PacketLength+5)/18)) )
#define L_BLOCK1    18
//#define L_BLOCK0    12


// PACKET type
 //#define SND_NR      0x44    // SEND/NO REPLY, Send actual data & alarm flags
//#define SND_UD      0x53    // SEND USER DATA
//#define SND_UD73    0x73    // SEND USER DATA
 //#define SND_IR      0x46    // Send installation request from RF converter to MUC (RF converter only, not for meter with RF or actuator)
 //#define SND_IR_ACK  0x06    // ACK for INIT;
//#define SND_AD      0x48    // Accsess demand
 //#define SND_NKE     0x40    // Acknowledgement of data reception & transfer delay value next data transmission
 //#define SND_ACK     0x00    // ACK
//#define REQ_UD1     0x5A    // REQUEST USER DATA1
 //#define REQ_UD2     0x5B    // REQUEST USER DATA2

//#define RSP_UD		0x08    // Response USER DATA*/

//#define SND_NR		0x44	//7A (Response from meter)	4 bytes	Meter -> MUC	Send actual data & alarm flags
//#define SND_NKE	0x40	//5B (CMD to meter)	12 bytes, RF converter, meter or actuator address	MUC -> Meter	Acknowledgement of data reception & transfer delay value next data transmission						
//#define SND_IR		0x46	//8A (Transport layer from meter)	4 bytes	Meter -> MUC	Send installation request from RF converter to MUC (RF converter only, not for meter with RF or actuator)						
//#define CNF_IR		0x06	//5B (CMD to meter)	12 bytes, RF converter address	MUC -> Meter	Confirmation installation request & send installation data (parameters of physical meter)						
//#define SND_ACK	0x00	//8A (Transport layer from meter)	4 bytes	Meter -> MUC	Acknowledgement of installation data reception						
//#define REQ_UD2	0x5B	//80 (Transport layer to meter)	12 bytes, RF converter, meter or actuator address	MUC -> Meter	Request user data from meter						
//#define REQ_UD2	0x5B	//C0 (Request rergistration parameters 1)	12 bytes, RF converter, meter or actuator address	MUC -> Meter	Request rergistration parameters of RF converter, meter with RF, actuator  or MUC						
//#define REQ_UD2	0x5B	//C1 (Request rergistration parameters 2)	12 bytes, RF converter address	MUC -> Meter	Request registration parameters of meter with pulses output (RF converter only, not for meter with RF or actuator)						
//#define RSP_UD		0x08	//7A (Response from meter)	4 bytes	Meter -> MUC	Response user data to MUC						
typedef enum
	{
	PktType_snd_ud=0x53,	// SEND USER DATA
	PktType_snd_ud73=0x73,	// SEND USER DATA
	PktType_snd_ad=0x48,	// Accsess demand
	PktType_rsp_ud=0x08,	// Response USER DATA
	PktType_snd_nr=0x44,	//7A (Response from meter)	4 bytes	Meter -> MUC	Send actual data & alarm flags
	PktType_snd_nke=0x40,	//5B (CMD to meter)	12 bytes, RF converter, meter or actuator address	MUC -> Meter	Acknowledgement of data reception & transfer delay value next data transmission						
	PktType_snd_ir=0x46,	//8A (Transport layer from meter)	4 bytes	Meter -> MUC	Send installation request from RF converter to MUC (RF converter only, not for meter with RF or actuator)						
	PktType_cnf_ir=0x06,	//5B (CMD to meter)	12 bytes, RF converter address	MUC -> Meter	Confirmation installation request & send installation data (parameters of physical meter)						
	PktType_snd_ack=0x00,	//8A (Transport layer from meter)	4 bytes	Meter -> MUC	Acknowledgement of installation data reception						
	PktType_req_ud1=0x5A,	// REQUEST USER DATA1
	PktType_req_ud2=0x5B	//80 (Transport layer to meter)	12 bytes, RF converter, meter or actuator address	MUC -> Meter	Request user data from meter						
	}TEPacketType;




// CI Field
/*#define wmbNoHdr				0x78
#define WMB_SHORT_HDR			0x7A
#define WMB_SHORT_HDR_TRANSPORT	0x8A
#define WMB_LONG_HDR_TRANSPORT	0x8B
#define wmbLongHdr				0x5B*/
typedef enum
	{
	cif_wmbNoHdr=0x78,			//
	cif_shortHdr=0x7A,			//
	cif_shortHdrTransport=0x8A,	//
	cif_longHdrTransport=0x8B,	//
	cif_LongHdr=0x5B			//
	}TECIF;


// DIF
// 0 -no 1- 1byte 2-2bytes 4-4(binary)
// 5-4(float) 6-6bytes 7-8bytes
// 0x80 - BCD coding
// VIF
#define VIF_TIMEs       0x20
#define VIF_DATETIME    0x6D

// DIF
// VIF
// DATA
//

typedef enum
	{
	type_none=0x00,
	type_meter_electricity=0x02,
	type_meter_gas=0x03,
	type_meter_heat=0x04,
	type_meter_warm_water=0x06,
	type_meter_water=0x07,
	type_meter_cold_water=0x16,
	type_ad_converter=0x19,
	type_actuator_electricity=0x20,
	type_actuator_gas=0x21,
	type_actuator_water=0x22,
	type_system_display=0x25,
	type_system_muc=0x31,
	type_system_radio_converter_modem=0x36,
	type_meter_radio_converter_pulses=0x37
	} TETYPE;

typedef enum
	{
	div_no_data=0x00,
	div_8Bit_bin=0x01,
	div_16Bit_bin=0x02,
	div_24Bit_bin=0x03,
	div_32Bit_bin=0x04,
	div_32Bit_real=0x05,
	div_48Bit_bin=0x06,
	div_64Bit_bin=0x07,
	div_selection_for_readout=0x08,
	div_2BCD=0x09,
	div_4BCD=0x0A,
	div_6BCD=0x0B,
	div_8BCD=0x0C,
	div_variable_length=0x0D,
	div_12BCD=0x0E,
	div_special_functions=0x0F
	}TEDIV;

typedef enum
	{
	vif_none=0xff,
	//Energy
	vif_0_001Wh=0x00,
	vif_0_010Wh=0x01,
	vif_0_100Wh=0x02,
	vif_1Wh=0x03,
	vif_10Wh=0x04,
	vif_100Wh=0x05,
	vif_1kWh=0x06,
	vif_10kWh=0x07,
	//Energy
	vif_0_001kJ=0x08,
	vif_0_010kJ=0x09,
	vif_0_100kJ=0x0A,
	vif_1kJ=0x0B,
	vif_10kJ=0x0B,
	vif_100kJ=0x0D,
	vif_1MJ=0x0E,
	vif_10MJ=0x0F,
	//Volume
	vif_0_001l=0x10,
	vif_0_010l=0x11,
	vif_0_100l=0x12,
	vif_1l=0x13,
	vif_10l=0x14,
	vif_100l=0x15,
	vif_1m3=0x16,
	vif_10m3=0x16,
	//Mass
	//Time
	//Power
	vif_0_010W=0x29,
	vif_0_100W=0x2A,
	vif_1W=0x2B,
	vif_10W=0x2C,
	//Energy
	vif_100Jh=0x32,
	//Volume flow
	vif_0_010lh=0x39,
	vif_0_100lh=0x3A,
	vif_1lh=0x3B,
	vif_10lh=0x3C,
	//Volume flow
	//Volume flow
	
	vif_0_010C_flow=0x59,
	vif_0_100C_flow=0x5A,
	vif_1C_flow=0x5B,
	vif_0_010C_ret=0x5D,
	vif_0_100C_ret=0x5E,
	vif_1C_ret=0x5F,
	vif_0_010C_diff=0x61,
	vif_serial_number=0x79
	}TEVIF;

typedef enum
	{
	vife_device_type=0x09FD,		//by OMS
	vife_manufacturer=0x0AFD,	//by EN 13757
	vife_model_version=0x0CFD,	//
	vife_hver=0x0DFD,			//binary
	vife_metrology_ver=0x0EFD,	//binary
	vife_other_soft_ver=0x0FFD,	//binary
	vife_Customer=0x11FD,		//BCD
	vife_error_flags=0x17FD,		//by device
	vife_remote_ctrl=0x1FFD,		//by device
	vife_last_calibr_data=0x00FF,	//type G
	vife_calibr_interval=0x01FF,	//years
	vife_delay_transmit=0x02FF,	//seconds
	vife_carrier_frequency=0x04FF,//binary
	vife_freq_offset=0x05FF,		//binary
	vife_cmd=0x10FF			//binary
	}TEVIFE;




#pragma pack(push, 1)
/*typedef struct
	{
	TEDIV    DIF1;		// 0x04
	TEVIF    VIF1;		// 0x14 - 10Liter, 0x20 - time
	uint32_t cnt;
	TEDIV    DIF2;		// 0x01 1 byte
	TEVIFE   VIF2;		// 0x17FD
	uint8_t  alarm;	// alarmState
	} TSblock2;*/

/*
SND-NR 	C - 44, Meter link address (pulses meter)	CI - 7A (Response from meter), header - 4 bytes	Actual measurement value (04/14) and alarm flags (02/FD 17) (4 channel)	Meter (P) -> MUC (S)
SND-NKE	C - 40, MUC address	CI - 80 (Transport layer to meter), header - 12 bytes, Meter appl. address (pulses meter)	Delay of next packet (03/FF 02) (seconds)1	MUC (P) -> Meter (S)
*/
//LF| C|MANID|   ADDR    |CI|
//32 08 B4 4C 02 00 00 00 01 07 AC 17 78 04 14 00 00 00 00 01 FD 17 00 04 14 00 00 00 F6 66 00 01 FD 17 00 04 14 00 00 00 00 01 FD 17 00 04 68 94 14 00 00 00 00 01 FD 17 00 3A 1E 18 76 E5 34 53 
typedef struct
	{
	uint8_t  LF;		//L-field - Length - The field specifies the number of subsequent user data bytes including the control and address bytes and excluding the CRC bytes
	uint8_t  CF;		//C-field - Control - It specifies the frame type, see pages "Mode S", "Mode T" & "Mode R2"
	uint16_t MANIDF;	//M-field - Manufacturer: Unique User/Manufacturer ID of the meter
					//The "CEN" is coded according to EN 13757-3 as "C"=(43h-40h) = 3, "E"=5, "N"=14. Thus CEN= 32*32*3+32*5+14 = 3246d  = 0CAEh.
					//Most significant bit is null since it is a "hard" (i.e. manufacturer unique) address.
	uint32_t ADDRF;	//A-field - Address - Manufacturer number (4 bytes - Identification-number; 1 byte - Version; 1 byte - Device type), specified in 5.4, 5.6 & 5.7 of EN 13757-3:2004
	uint8_t  VERF;		//A-field - Version
	TETYPE   TYPEF;	//A-field - Device type
	uint8_t  CRC0[2];	//
	} TSblock0;


typedef struct
	{
	uint8_t  access_no;	//Incremental
	uint8_t  status;	//0
	uint16_t signature;	//0
	} TSheader_short;

typedef struct
	{
	uint32_t ADDRF;	//A-field - Address - Manufacturer number (4 bytes - Identification-number; 1 byte - Version; 1 byte - Device type), specified in 5.4, 5.6 & 5.7 of EN 13757-3:2004
	uint16_t MANIDF;	//M-field - Manufacturer: Unique User/Manufacturer ID of the meter
					//	The "CEN" is coded according to EN 13757-3 as "C"=(43h-40h) = 3, "E"=5, "N"=14. Thus CEN= 32*32*3+32*5+14 = 3246d  = 0CAEh.
					//	Most significant bit is null since it is a "hard" (i.e. manufacturer unique) address.
	uint8_t  VERF;		//A-field - Version
	TETYPE   TYPEF;	//A-field - Device type
	uint8_t  access_no;	//Incremental
	uint8_t  status;	//0
	uint16_t signature;	//0
	} TSheader_long;


typedef struct
	{
	uint8_t sec:6;		//Second  UI6 [1 to 6]  <0 to 59> ; 63 : every second
	uint8_t summer:1;	//1= yes (summer time) ; 0 = no
	uint8_t leap_year:1;//1= leap year ; 0 = standard year
	
	uint8_t min:6;		//Minute  UI6 [9 to 14]  <0 to 59> ; 63 : every minute	
	uint8_t reserv:2;	//Reserved (all 0)
	
	uint8_t hour:5;	//Hour UI5 [17 to 21]  <0 to 23> >; 31 : every hour	
	uint8_t day_of_week:3;//Day of the week UI3 [22 to 24]  1 to 7> 1= Monday  7= Sunday 0= not specified
	
	uint8_t day:5;		//Day UI5  [25 to 29]  <1 to 31>  <0> (0= not specified)
	uint8_t year1:3;	//Year UI7 [30 to 32+37 to 40]  <0 to 99>   <127>   127= not specified
	
	uint8_t month:4;	//Month UI4 [33 to 36]  <1 to 12>  <0>  0= not specified
	uint8_t year2:4;	//Year UI7 [30 to 32+37 to 40]  <0 to 99>   <127>   127= not specified
	
	uint8_t week_num:6;	//Week UI6  [41 to 46]  <1 to 53> 0= not specified
	uint8_t daylight_saving:2;	//daylight saving deviation (hour) UI2  [47 to 48] <0 to 3>    0 = no daylight saving
	}TSdatetime_i;


typedef struct 
	{
	// Block0
	TSblock0 block0;
	// Block1
	uint8_t  CIF;		//0x7A - Response from meter: Variable data format response with short header
	struct
		{
		uint8_t  access_no;	//Incremental
		uint8_t  status;	//0
		uint16_t signature;	//0
		}header;
	//TSblock2 block2;
	uint8_t data[64];
	}TSwmbPacket_data;


typedef struct 
	{
	// Block0
	TSblock0 block0;
	// Block1
	uint8_t  CIF;		//8B (transport), header - 12 bytes
	TSheader_long header;
	//TSblock2 block2[3];
	uint8_t data[8];
	}TSwmbPacket_data_tx;

typedef struct
	{
	TEDIV div;
	uint32_t vif;
	uint32_t data;
	uint8_t data_len;
	}TSwmbData;


typedef struct 
	{
	// Block0
	TSblock0 block0;
	// Block1
	uint8_t  CIF;		//5B (CMD to meter), header - 12 bytes, pulses meter address
	TSheader_long header;
	struct 		//Delay to next packet (03/FF 02) (seconds)
		{
		uint8_t  dif1;		// 0x03
		uint16_t vif1;		// 0xFF02
		uint8_t power_time[3];//Delay to next packet
		uint8_t dif2;		// 0xX6
		uint8_t vif2;		// 0x6D
		TSdatetime_i datetime;
		}block2;
	}TSwmbPacket_SND_NKE;

typedef struct 
	{
	uint8_t  LF;		//L-field - Length - The field specifies the number of subsequent user data bytes including the control and address bytes and excluding the CRC bytes
	uint8_t  CF;		//C - 46, Meter link address (pulses meter)
	uint16_t MANIDF;	//M-field - Manufacturer: Unique User/Manufacturer ID of the meter
					//The "CEN" is coded according to EN 13757-3 as "C"=(43h-40h) = 3, "E"=5, "N"=14. Thus CEN= 32*32*3+32*5+14 = 3246d  = 0CAEh.
					//Most significant bit is null since it is a "hard" (i.e. manufacturer unique) address.
	uint32_t ADDRF;	//A-field - Address - Manufacturer number (4 bytes - Identification-number; 1 byte - Version; 1 byte - Device type), specified in 5.4, 5.6 & 5.7 of EN 13757-3:2004
	uint8_t  VERF;		//A-field - Version
	TETYPE   TYPEF;	//A-field - Device type
	uint8_t  CRC0[2];	//
	uint8_t  CIF;		//8A (Transport layer from meter), header - 4 bytes
	TSheader_short header;
	}TSwmbPacket_SND_IR;

typedef struct 
	{
	// Block0
	TSblock0 block0;
	// Block1
	uint8_t  CIF;		//5B (CMD to meter), header - 12 bytes, pulses meter address
	TSheader_long header;
	struct 		//Delay to next packet (03/FF 02) (seconds)
		{
		uint16_t dif_manufacturer;	// 0x82 0xX0
		uint16_t vif_manufacturer;	// 0xFD 0x0A
		uint16_t manufacturer;
		uint16_t dif_type;			// 0x81 0xX0
		uint16_t vif_type;			// 0xFD 0x09
		uint8_t  type;
		uint16_t dif_serial;		// 0x8C 0xX0
		uint8_t  vif_serial;		// 0x79
		uint32_t serial;
		uint16_t dif_initial_value;	// 0x84 0xX0
		TEVIF   vif_initial_value;	// 0x14
		uint32_t initial_value;
		uint16_t dif_discretes;		// 0x81 0xX0
		TEVIF    vif_discretes;		// 0x14
		uint8_t  discretes;
		}block2;
	}TSwmbPacket_CNF_IR;

typedef struct 
	{
	// Block0
	TSblock0 block0;
	// Block1
	uint8_t  CIF;		//5B (CMD to meter), header - 12 bytes, pulses meter address
	TSheader_long header;
	struct 			//Delay to next packet (03/FF 02) (seconds)
		{
		uint8_t  dif1;		// div_16Bit_bin=0x02
		uint16_t vif1;		// vife_cmd=0x10FF (0xFF10)
		uint16_t cmd;		// Cmd
		}block2;
	}TSwmbPacket_SND_UD;

#pragma pack(pop)



void InitWMBus(void);
uint8_t wmbCreatePacket(uint8_t *txBuf,uint8_t PacketTYPE);
bool    wmbCheckAddr(void *rxBuf);
uint8_t wmbCheckPacket(uint8_t *rxBuf,uint8_t cnt);
uint8_t wmbParse(uint8_t *rxBuf,uint8_t *txBuf);
#define CRC_CALC    0
#define CRC_APPEND  1
#define CRC_CHECK   2
uint16_t wmbCRCCalc(uint8_t *buf, uint8_t cnt, uint8_t mode);
uint8_t  wmbCRCFree(void *buf);
uint8_t  wmbCalcDataLen (void * wmb);
uint8_t  wmbCalcDataStart (void * wmb);
uint8_t  wmbGetDataStruct(void * buf, TSwmbData *wmb_data, int8_t buf_len);

void int2bcd(uint8_t bcdSize, uint32_t *intSource, uint8_t *bcdDest);


#endif
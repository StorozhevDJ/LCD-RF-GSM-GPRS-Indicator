
#ifndef Serial_H
#define Serial_H

//------------------------------------------------------------------------------
//User predefined parameters
#define RX_TIMEOUT_VAL	2		//timeout for setting flag ending receive
#define RX_END_STR_CHAR	'\r'		//symbol ending string in ASCII mode

#define RX_BUF_SIZE		65		//Size of received buffer
//#define TX_BUF_SIZE		64		//Size of transmitted buffer

//Insert user function in current code, called for description event
#define Serial_LedOn			//The code is executed if a byte has been accepted
#define Serial_LedOff		//Receiv buffer empty or receiv complette (receive is not)
#define Serial_BuffFull	//P3OUT|= (BIT4)	//Receiv buffer is full
#define Serial_BuffEmpty	//P3OUT&=~(BIT4)	//Receiv buffer empty

#define CIRCULAR_BUFFER		//If uncomment, then receive buffer is circular, else linear

//------------------------------------------------------------------------------



#if RX_BUF_SIZE < 0x100
	#define RX_BUFF_SIZE_TYPE	uint8_t
#elif RX_BUF_SIZE < 0x10000
	#define RX_BUFF_SIZE_TYPE	uint16_t
#else
	#error "Size of RX buffer is not correct"
#endif



typedef enum
	{
	BIN=0,
	ASCII
	}Emode_data;

typedef struct
	{
	struct
		{
		RX_BUFF_SIZE_TYPE cnt;	//Counter bytes of buffer
#ifdef CIRCULAR_BUFFER
		RX_BUFF_SIZE_TYPE ptr_tail;//Pointer to tail receive byte
		RX_BUFF_SIZE_TYPE ptr_head;//Pointer to head receive byte
#else
		RX_BUFF_SIZE_TYPE ptr;	//Pointer to receive byte
#endif
		bool		    complete:1;//Receive complete
		bool 	    overflow:1;//buffer overflow
		unsigned char timeout;	//Timeout receive
		char buf[RX_BUF_SIZE];	//Buffer for received bytes
		}rx;
#ifdef TX_BUF_SIZE
	struct
		{
		char buf[TX_BUF_SIZE];	//Buffer for transmited bytes
		unsigned char cnt;		//Counter bytes for transmit
		unsigned char complete:1;//Transmit complete
		unsigned char ptr:6;	//Pointer to transmit byte (available value from 0 to 63)
		}tx;
#endif
	Emode_data mode;
	}volatile TSserial;




//public function
/*******************************************************************************
* rx_funct_ptr - Pointer to function Complete receive
* RxByte_funct_ptr - Pointer to function read one byte
* TxByte_funct_ptr - Pointer to function send one byte
*******************************************************************************/
void Serial_Init(void (*rx_funct_ptr)(uint8_t *buf, char cnt),
			  char (*RxByte_funct_ptr)(void),
			  void (*TxByte_funct_ptr)(char byte));
//void Serial_SetBaud(uint32_t baud);
void Serial_SendChar(char data);
char Serial_SendString(void * str);
void Serial_SendBuf(void * buf, unsigned char len);
uint8_t Serial_Read(uint8_t *buf, uint8_t len);
uint8_t Serial_Peek(void);
void Serial_SetMode(Emode_data m);
RX_BUFF_SIZE_TYPE Serial_IsReceiv(void);
void Serial_Free(void);
void Serial_Timer10ms(void);
void Serial_RxBufEnqueue(char byte);

//private function
char Serial_RxBufDequeue(void);
char Serial_GetChar(void);
#endif //Serial_H
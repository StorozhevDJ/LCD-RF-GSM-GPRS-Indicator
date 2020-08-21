
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "Serial.h"




volatile TSserial serial={0, 0, 0, false, false};

void Serial_RecieveNULL(uint8_t *buf, char len);
void Serial_TxByteNULL(char byte);
char Serial_RxByteNULL(void);


//Pointer to receiv function
void (*Serial_Rx_complete)(uint8_t *buf, char cnt)=Serial_RecieveNULL;
char (*Serial_RxByte)(void)=Serial_RxByteNULL;
void (*Serial_TxByte)(char byte)=Serial_TxByteNULL;



/*******************************************************************************
* rx_funct_ptr - Pointer to function Comlete recieve
* RxByte_funct_ptr - Pointer to function read one byte
* TxByte_funct_ptr - Pointer to function send one byte
*******************************************************************************/
void Serial_Init(void (*rx_funct_ptr)(uint8_t *buf, char cnt),
			  char (*RxByte_funct_ptr)(void),
			  void (*TxByte_funct_ptr)(char byte))
{
if (rx_funct_ptr!=0) Serial_Rx_complete=rx_funct_ptr;
if (RxByte_funct_ptr!=0) Serial_RxByte=RxByte_funct_ptr;
if (TxByte_funct_ptr!=0) Serial_TxByte=TxByte_funct_ptr;
Serial_Free();
}



/*******************************************************************************
* Serial_RecieveNULL - NULL function
*******************************************************************************/
void Serial_RecieveNULL(uint8_t *buf, char len){}
void Serial_TxByteNULL(char byte){}
char Serial_RxByteNULL(void){return 0;}



/*******************************************************************************
* Send Char to Serial interface
* data - char to send
* Return - none
*******************************************************************************/
void Serial_SendChar(char data)
{
Serial_TxByte(data);
/*while (!(UCA0IFG & UCTXIFG)); //Ожидание опустошения буфера приема (окончания прошлой передачи)
UCA0TXBUF = data; //Начало передачи данных*/
}



/*******************************************************************************
* Send String to Serial interface.
* str - pointer to transmitted string
* Return - transmitted lenght
*******************************************************************************/
char Serial_SendString(void * str)
{
unsigned char *ptr=(unsigned char *)str;
unsigned char len=0;
while (*ptr)
	{
	Serial_SendChar(*ptr++);
	len++;
	}
return len;
}



/*******************************************************************************
* Send Buffer to Serial interface.
* buf - pointer to transmitted buffer
* len - lenght data for transmit
*******************************************************************************/
void Serial_SendBuf(void * buf, unsigned char len)
{
char *tmp;
tmp=(char *)buf;
for (; len; len--) Serial_SendChar(*tmp++);
}



/*******************************************************************************
*
*******************************************************************************/
inline char Serial_GetChar(void)
{
#ifdef CIRCULAR_BUFFER
    return Serial_RxBufDequeue();
#else
    return Serial_RxByte();
    /*while (!(UCA0IFG & UCRXIFG));    // RX a byte?
    return UCA0RXBUF;*/
#endif
}



/*******************************************************************************
*
*******************************************************************************/
int putchar(int c)
{
// Ждём опустошения буфера передачи
/*while (!(UCA0IFG & UCTXIFG));
// Записываем в буфер следующий байт
UCA0TXBUF = (unsigned char) c;*/
Serial_SendChar((char)c);
return c; // Прототип функции требует вернуть записанный байт
}



/*******************************************************************************
*
*******************************************************************************/
int getchar(void)
{
return Serial_GetChar();
}



/*******************************************************************************
* Determination of end receive data
*******************************************************************************/
inline void Serial_Timer10ms(void)
{
if (serial.rx.timeout) serial.rx.timeout--;	//Dicrement received timer
else	//Timeout receive
	{
#ifdef CIRCULAR_BUFFER
	if (serial.rx.ptr_tail != serial.rx.ptr_head)
		{
		if(serial.mode==BIN)
			{
			serial.rx.complete=true;
			Serial_Rx_complete((uint8_t *)serial.rx.buf, serial.rx.cnt);	//вызовим функцию обработки
			}
		else if ((serial.rx.buf[serial.rx.ptr_tail]==0)||
			    (serial.rx.buf[serial.rx.ptr_tail]=='\r')||
			    (serial.rx.buf[serial.rx.ptr_tail]=='\n'))
			{
			serial.rx.complete=true;
			Serial_Rx_complete((uint8_t *)serial.rx.buf, serial.rx.cnt);	//вызовим функцию обработки
			}
		}
#else
	if (serial.rx.overflow==true)
		{
		serial.rx.overflow=false;
		serial.rx.ptr=0;
		serial.rx.cnt=0;
		}
	else if ((serial.rx.cnt)&&(serial.mode==BIN))
		{
		serial.rx.complete=true;	//Receive completed
		serial.rx.ptr=0;
		Serial_Rx_complete((uint8_t *)serial.rx.buf, serial.rx.cnt);	//вызовим функцию обработки
		serial.rx.cnt=serial.rx.ptr;
		}
#endif
	}
}



/*******************************************************************************
* Addition of bytes in the buffer queue
*******************************************************************************/
void Serial_RxBufEnqueue(char byte)
{
Serial_LedOn;
#ifdef CIRCULAR_BUFFER
// if buffer full, set the overflow flag and return
//if ((serial.rx.ptr_tail + 1) % RX_BUF_SIZE != serial.rx.ptr_head)
if (Serial_IsReceiv()<RX_BUF_SIZE)
	{
	//читаем содержимое регистра данных Serial
	// save new data in buffer: tail points to where byte goes
	serial.rx.buf[serial.rx.ptr_tail] = byte; // save new byte
	//serial.rx.ptr_tail = (serial.rx.ptr_tail + 1) ;//% RX_BUF_SIZE;
	if (++serial.rx.ptr_tail>=RX_BUF_SIZE) serial.rx.ptr_tail=0;
	//serial.rx.cnt++;
	if (Serial_IsReceiv()>=(RX_BUF_SIZE-3)) Serial_BuffFull;
	}
else serial.rx.overflow = true;

serial.rx.timeout=RX_TIMEOUT_VAL;
#else
if (serial.rx.ptr<RX_BUF_SIZE) serial.rx.buf[serial.rx.ptr++]=byte;
if (serial.rx.ptr==0)
	{
	serial.rx.overflow=true;
	Serial_BuffFull;
	serial.rx.ptr--;
	//Serial_Rx_complete((char *)serial.rx.buf, serial.rx.cnt);
	}
serial.rx.cnt++;
serial.rx.timeout=RX_TIMEOUT_VAL;

//Если режим передачи ASCII символов
if (serial.mode==ASCII)
	{
	 //если первый байт CR или LF, пропустим его
	if ((serial.rx.ptr==1)&&((byte=='\r')||(byte=='\n')))
		{
		serial.rx.ptr=0;
		serial.rx.cnt=0;
		return;
		}
	//и приняли символ конца строки или символ 0
	if ((byte==RX_END_STR_CHAR)||(byte==0))
		{
		serial.rx.complete=true;	//выставим флаг "прием завершен"
		serial.rx.ptr=0;
		if (serial.rx.cnt<RX_BUF_SIZE)
			{
			serial.rx.buf[serial.rx.cnt]=0;//Add end string
			//serial.rx.cnt++;
			}
		Serial_Rx_complete((uint8_t *)serial.rx.buf, serial.rx.cnt);	//вызовим функцию обработки
		serial.rx.cnt=serial.rx.ptr;
		Serial_LedOff;
		}
	}
#endif
}



/*******************************************************************************
* Extraction of byte from the buffer queue
*******************************************************************************/
char Serial_RxBufDequeue(void)
{
#ifdef CIRCULAR_BUFFER
// Empty buffer?
if (serial.rx.ptr_tail == serial.rx.ptr_head)
	{
	Serial_BuffEmpty;
	return 0;
	}

// Read from "head"
char d = serial.rx.buf[serial.rx.ptr_head]; // grab next byte
serial.rx.ptr_head = (serial.rx.ptr_head + 1) % RX_BUF_SIZE;
//if (++serial.rx.ptr_head>=RX_BUF_SIZE) serial.rx.ptr_head=0;
//serial.rx.cnt--;

if (serial.rx.ptr_tail == serial.rx.ptr_head) Serial_BuffEmpty;

return d;
#else
return 0;
#endif
}



/*******************************************************************************
* Read data array to buf
*******************************************************************************/
uint8_t Serial_Read(uint8_t *buf, uint8_t len)
{
uint8_t size=Serial_IsReceiv();
if (len<size) len=size;
for (uint8_t i=0; i<len; i++) buf[i]=Serial_RxBufDequeue();
return len;
}



/*******************************************************************************
* Read last received byte
*******************************************************************************/
uint8_t Serial_Peek(void)
{
#ifdef CIRCULAR_BUFFER
return serial.rx.buf[serial.rx.ptr_tail];
#else
return 0;
#endif
}



/*******************************************************************************
* Change receive mode to ASCII or Binary
*******************************************************************************/
inline void Serial_SetMode(Emode_data m)
{
serial.mode=m;
}



RX_BUFF_SIZE_TYPE Serial_IsReceiv(void)
{
#ifdef CIRCULAR_BUFFER
//if (serial.rx.ptr_tail==serial.rx.ptr_head) Serial_BuffEmpty;
//return (serial.rx.ptr_tail + RX_BUF_SIZE - serial.rx.ptr_head) % RX_BUF_SIZE;
if (serial.rx.ptr_tail>=serial.rx.ptr_head) return (serial.rx.ptr_tail - serial.rx.ptr_head);
else return (serial.rx.ptr_tail + RX_BUF_SIZE - serial.rx.ptr_head);
#else
return serial.rx.ptr==0;
#endif
}



void Serial_Free(void)
{
serial.rx.complete=false;
Serial_BuffEmpty;
serial.rx.cnt=0;
#ifdef CIRCULAR_BUFFER
serial.rx.ptr_tail = serial.rx.ptr_head = 0;
//for (uint8_t i=0; i<RX_BUF_SIZE; i++) serial.rx.buf[i];
//memset((void *)serial.rx.buf, 0, RX_BUF_SIZE);
#else
serial.rx.ptr=0;
#endif
}

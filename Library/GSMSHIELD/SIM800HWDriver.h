
#ifndef _HWSERIAL_H_
#define _HWSERIAL_H_

#include <inttypes.h>
#include <ysizet.h>
#include <string.h>

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"



#define delay(x)	osDelay(x)
#define millis		HAL_GetTick

#define boolean	bool
#define byte		uint8_t


#define DEC	10

void SIM800_IRQHandler(char byte);

class SIM800HWDriver{

private:
     int write_error;
     size_t printNumber(unsigned long, uint8_t);
     size_t printFloat(double, uint8_t);
	
	static void MX_USART2_UART_Init(void);

public:
     SIM800HWDriver();
	~SIM800HWDriver();
	
	void SetBaud(uint32_t baud);
	virtual void begin(long baud_rate);
	void PowerOn(bool state);
	
     //bool listen();
     void end();
     int peek();
     

     virtual size_t write(uint8_t byte);
     size_t write(const char *str);
     virtual size_t write(const uint8_t *buffer, size_t size);

     int read();
     virtual int available();
     virtual void flush();
	
	bool IsDataMode();
	void SetDataMode();
	void SetCmdMode();

//     size_t print(const __FlashStringHelper *);
     //size_t print(const String &);
     size_t print(const char[]);
     size_t print(char);
     size_t print(unsigned char, int = DEC);
     size_t print(int, int = DEC);
     size_t print(unsigned int, int = DEC);
     size_t print(long, int = DEC);
     size_t print(unsigned long, int = DEC);
     size_t print(double, int = 2);


//     size_t println(const __FlashStringHelper *);
     //size_t println(const String &s);
     size_t println(const char[]);
     size_t println(char);
     size_t println(unsigned char, int = DEC);
     size_t println(int, int = DEC);
     size_t println(unsigned int, int = DEC);
     size_t println(long, int = DEC);
     size_t println(unsigned long, int = DEC);
     size_t println(double, int = 2);

     size_t println(void);

     int getString( char *pre_string, char *post_string, char *buffer, int length);
     boolean find(char *target);
     boolean  findUntil(char *target, char *terminate);
};
#endif
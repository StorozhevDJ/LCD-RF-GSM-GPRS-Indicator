
#include "SIM800HWDriver.h"

#include "Serial/Serial.h"

UART_HandleTypeDef huart2;




/*******************************************************************************
* Constructor
*******************************************************************************/
SIM800HWDriver::SIM800HWDriver()
{
__GPIOA_CLK_ENABLE();
__GPIOD_CLK_ENABLE();

GPIO_InitTypeDef GPIO_InitStruct;

/*Configure GPIO pin : WAN_DTR_Pin */
GPIO_InitStruct.Pin  = WAN_DTR_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(WAN_DTR_GPIO_Port, &GPIO_InitStruct);

/*Configure GPIO pin : WAN_PWKEY_Pin */
GPIO_InitStruct.Pin   = WAN_PWKEY_Pin;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
GPIO_InitStruct.Pull  =GPIO_NOPULL;
HAL_GPIO_Init(WAN_PWKEY_GPIO_Port, &GPIO_InitStruct);

MX_USART2_UART_Init();  
}



/*******************************************************************************
* Distructor
*******************************************************************************/
SIM800HWDriver::~SIM800HWDriver()
{
// Enable interrupt fromUSART2(NVIC level)
USART2->CR1  &=~ USART_CR1_RXNEIE;    // UART Receive Interrupt Disable.
NVIC_DisableIRQ(USART2_IRQn);
}



/*******************************************************************************
* USART2 init function
*******************************************************************************/
void SIM800HWDriver::MX_USART2_UART_Init(void)
{
huart2.Instance = USART2;
huart2.Init.BaudRate = 115200;
huart2.Init.WordLength = UART_WORDLENGTH_8B;
huart2.Init.StopBits = UART_STOPBITS_1;
huart2.Init.Parity = UART_PARITY_NONE;
huart2.Init.Mode = UART_MODE_TX_RX;
huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
huart2.Init.OverSampling = UART_OVERSAMPLING_16;
HAL_UART_Init(&huart2);

// Enable interrupt fromUSART1(NVIC level)
USART2->CR1  |= USART_CR1_RXNEIE;    // UART Receive Interrupt Enable.
NVIC_EnableIRQ(USART2_IRQn);
}



/*******************************************************************************
* Init UART2 and set baudrate
*******************************************************************************/
void SIM800HWDriver::SetBaud(uint32_t baud)
{
huart2.Instance = USART2;
huart2.Init.BaudRate = baud;
huart2.Init.WordLength = UART_WORDLENGTH_8B;
huart2.Init.StopBits = UART_STOPBITS_1;
huart2.Init.Parity = UART_PARITY_NONE;
huart2.Init.Mode = UART_MODE_TX_RX;
huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
huart2.Init.OverSampling = UART_OVERSAMPLING_16;
HAL_UART_Init(&huart2);
}



/*******************************************************************************
* Control to PWRKEY pin
* true - Key "Presed" (Low level)
* false - Key "Unpresed" (Three state)
*******************************************************************************/
void SIM800HWDriver::PowerOn(bool state)
{
if (state) WAN_PWKEY_GPIO_Port->BRR=WAN_PWKEY_Pin;
else WAN_PWKEY_GPIO_Port->BSRR=WAN_PWKEY_Pin;
}



void SIM800HWDriver::begin(long baud_rate)
{
Serial_Init(0, 0, 0);
SetBaud(baud_rate);
}



/*******************************************************************************
* Serial interface reciev interrupt handler
* usart_rx_buf - buffer to recieved data
* usart_rx_cnt - pointer to usart_rx_buf
*******************************************************************************/
void SIM800_IRQHandler(char byte)
{
//Программа обработки принятого байта
Serial_RxBufEnqueue(byte);
}



/*******************************************************************************
* Send Char to Serial interface
* data - char to send
* Return - none
*******************************************************************************/
inline void GSMSendChar(char data)
{
uint8_t tmp[1]={data};
HAL_UART_Transmit(&huart2, tmp, 1, 1000);
}



/*
bool SIM800HWDriver::listen(){
	return Serial1.listen();
}
*/
void SIM800HWDriver::end()
{
//Serial1.end();
}
/*
bool SIM800HWDriver::isListening(){
	return Serial1.isListening();
}

bool SIM800HWDriver::overflow(){
	return Serial1.overflow();
}
*/



int SIM800HWDriver::peek()
{
return 0;//Serial1.peek();
}



size_t SIM800HWDriver::write(uint8_t byte)
{
GSMSendChar(byte);
return 1;
}



size_t SIM800HWDriver::write(const char *str)
{
return write((const uint8_t *)str, strlen(str));
}



int SIM800HWDriver::read()
{
return Serial_RxBufDequeue();
}



int SIM800HWDriver::available()
{
return Serial_IsReceiv();
}



void SIM800HWDriver::flush()
{
Serial_Free();
}



bool SIM800HWDriver::IsDataMode()
{
return (WAN_DCD_GPIO_Port->IDR&WAN_DCD_Pin)?false:true;
}



void SIM800HWDriver::SetDataMode()
{
WAN_DTR_GPIO_Port->BRR=WAN_DTR_Pin;
}



void SIM800HWDriver::SetCmdMode()
{
WAN_DTR_GPIO_Port->BSRR=WAN_DTR_Pin;
//write("+++");
}



/**************************************/



/*
size_t SIM800HWDriver::print(const String &s)
{
     size_t n = 0;
     for (uint16_t i = 0; i < s.length(); i++) {
          n += write(s[i]);
     }
     return n;
}*/


size_t SIM800HWDriver::print(const char str[])
{
return write(str);
}

size_t SIM800HWDriver::print(char c)
{
     return write(c);
}

size_t SIM800HWDriver::print(unsigned char b, int base)
{
     return print((unsigned long) b, base);
}

size_t SIM800HWDriver::print(int n, int base)
{
     return print((long) n, base);
}

size_t SIM800HWDriver::print(unsigned int n, int base)
{
     return print((unsigned long) n, base);
}

size_t SIM800HWDriver::print(long n, int base)
{
     if (base == 0) {
          return write(n);
     } else if (base == 10) {
          if (n < 0) {
               int t = print('-');
               n = -n;
               return printNumber(n, 10) + t;
          }
          return printNumber(n, 10);
     } else {
          return printNumber(n, base);
     }
}

size_t SIM800HWDriver::print(unsigned long n, int base)
{
     if (base == 0) return write(n);
     else return printNumber(n, base);
}

size_t SIM800HWDriver::print(double n, int digits)
{
     return printFloat(n, digits);
}

/*size_t SIM800HWDriver::println(const __FlashStringHelper *ifsh)
{
     size_t n = print(ifsh);
     n += println();
     return n;
}*/


size_t SIM800HWDriver::println(void)
{
     size_t n = print('\r');
     n += print('\n');
     return n;
}

/*size_t SIM800HWDriver::println(const String &s)
{
     size_t n = print(s);
     n += println();
     return n;
}*/


size_t SIM800HWDriver::println(const char c[])
{
size_t n = print(c);
n += println();
return n;
}


size_t SIM800HWDriver::println(char c)
{
     size_t n = print(c);
     n += println();
     return n;
}

size_t SIM800HWDriver::println(unsigned char b, int base)
{
     size_t n = print(b, base);
     n += println();
     return n;
}

size_t SIM800HWDriver::println(int num, int base)
{
     size_t n = print(num, base);
     n += println();
     return n;
}

size_t SIM800HWDriver::println(unsigned int num, int base)
{
     size_t n = print(num, base);
     n += println();
     return n;
}

size_t SIM800HWDriver::println(long num, int base)
{
     size_t n = print(num, base);
     n += println();
     return n;
}

size_t SIM800HWDriver::println(unsigned long num, int base)
{
     size_t n = print(num, base);
     n += println();
     return n;
}

size_t SIM800HWDriver::println(double num, int digits)
{
     size_t n = print(num, digits);
     n += println();
     return n;
}



/******************************/
size_t SIM800HWDriver::write(const uint8_t *buffer, size_t size)
{
HAL_UART_Transmit(&huart2, (uint8_t *) buffer, size, 1000);
return size;
}



size_t SIM800HWDriver::printNumber(unsigned long n, uint8_t base)
{
     char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
     char *str = &buf[sizeof(buf) - 1];

     *str = '\0';

     // prevent crash if called with base == 1
     if (base < 2) base = 10;

     do {
          unsigned long m = n;
          n /= base;
          char c = m - base * n;
          *--str = c < 10 ? c + '0' : c + 'A' - 10;
     } while(n);

     return write(str);
}

size_t SIM800HWDriver::printFloat(double number, uint8_t digits)
{
     size_t n = 0;

     // Handle negative numbers
     if (number < 0.0) {
          n += print('-');
          number = -number;
     }

     // Round correctly so that print(1.999, 2) prints as "2.00"
     double rounding = 0.5;
     for (uint8_t i=0; i<digits; ++i)
          rounding /= 10.0;

     number += rounding;

     // Extract the integer part of the number and print it
     unsigned long int_part = (unsigned long)number;
     double remainder = number - (double)int_part;
     n += print(int_part);

     // Print the decimal point, but only if there are digits beyond
     if (digits > 0) {
          n += print(".");
     }

     // Extract digits from the remainder one at a time
     while (digits-- > 0) {
          remainder *= 10.0;
          int toPrint = int(remainder);
          n += print(toPrint);
          remainder -= toPrint;
     }

     return n;
}



boolean  SIM800HWDriver::find(char *target)
{
return findUntil(target, NULL);
}



boolean  SIM800HWDriver::findUntil(char *target, char *terminate)
{
     byte targetLen = strlen(target);
     byte index = 0;  // maximum target string length is 255 bytes
     byte termIndex = 0;
     byte termLen = (terminate == NULL ? 0 : strlen(terminate));
     char c;

     if( *target == 0)
          return true;   // return true if target is a null string
     while( (c = read()) != 0) {
          if( c == target[index]) {
               if(++index >= targetLen) { // return true if all chars in the target match
                    return true;
               }
          } else {
               index = 0;  // reset index if any char does not match
          }
          if(termLen > 0 && c == terminate[termIndex]) {
               if(++termIndex >= termLen)
                    return false;       // return false if terminate string found before target string
          } else
               termIndex = 0;
     }
     return false;
}



int SIM800HWDriver::getString( char *pre_string, char *post_string, char *buffer, int length)
{
if( find(pre_string) )
	{
	int index = 0;
	*buffer = 0;
	while(index < length-1 )
		{
		char c = read();
		if( c == 0 ) return 0;   // timeout returns 0 !
		else if( c == *post_string )
			{
			while (index < length)
				{
				buffer[index] = '\0'; // terminate the string !!!!!!!!!!!IT DOESN'T WORK!!!!!!!!!!!!!!!!!
				index++;
				}

			//buffer[index]=0;
			return index;               // data got successfully
			}
		else	buffer[index++] = c;
		}
	buffer[index] = 0;
	return index; // Note: buffer full before the closing post_string encountered
	}
return 0;    //failed to find the prestring
}




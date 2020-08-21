
#include <stdint.h>

#define sbit(byte,bit)     (byte|= (1<<bit))
#define cbit(byte,bit)     (byte&=~(1<<bit))
#define tbit(byte,bit)     (byte&  (1<<bit))
#define fbit(byte,bit)     (byte^= (1<<bit))
//
#define FromBCD(s)          ((s&0x0F)+10*(s>>4))
#define ToBCD(s)            ((s%10)|((s/10)<<4))
//
#define MIN(A,B) if(A<B)?A:B
#define MAX(A,B) if(A>B)?A:B

#define SWAB32(x) ((uint32_t)(\
        (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |\
        (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) |\
        (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) |\
        (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

uint8_t ToUpper(uint8_t s);
uint8_t AsciiToDigit(uint8_t s);
uint8_t AsciiToHex(uint8_t s);
uint32_t StringToHex(char * s, uint8_t col);
void BCDprint(uint8_t *buf, uint32_t W, uint8_t digit);
void Binprint(uint8_t *buf, uint32_t W, uint8_t digit);
uint8_t ConvertUart(uint8_t s);

void int2bcd(uint8_t bcdSize, uint32_t *intSource, uint8_t *bcdDest);
uint8_t Bin2bcd(uint8_t dec);

void reverse(char s[]);
void itoa(int n, char s[]);
void ltoa(long int n,char *str);
void ltoa_null(long n, char s[], int l);
#ifdef __cplusplus
void itoa(int n, char s[], int l);
#endif
//

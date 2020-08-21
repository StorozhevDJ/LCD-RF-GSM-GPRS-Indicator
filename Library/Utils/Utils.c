
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"



void int2bcd(uint8_t bcdSize, uint32_t *intSource, uint8_t *bcdDest)
{
uint64_t tmp = *intSource;
while(bcdSize != 0)
	{
	*bcdDest = (tmp % 10);
	tmp -= (tmp % 10);
	tmp = tmp / 10;
	bcdSize--;
	if (bcdSize == 0) break;
	*bcdDest += ((tmp % 10) << 4);
	tmp -= (tmp % 10);
	tmp = tmp / 10;
	bcdDest++;
	bcdSize--;
	}
}



uint8_t Bin2bcd(uint8_t dec)
{
return ((dec/10)<<4)+(dec % 10);
}



uint8_t AsciiToDigit(uint8_t s)
{
if((s-'0') < 10) return (s-'0');
else return -1;
}



uint8_t ToUpper(uint8_t s)
{
if('a'<=s && s<='z') s += ('A'-'a');
return s;
}



uint8_t AsciiToHex(uint8_t s)
{
if((s-'0') < 10) return (s-'0');
else if((s-'A') <  6) return (s-('A'-10));
else if((s-'a') <  6) return (s-('a'-10));
else return 0;
}



uint32_t StringToHex(char * s, uint8_t col)
{
uint32_t ret=0;
for (uint8_t i=0; i<col; i++)
	{
	if (((s[i]>='0')&&(s[i]<='9'))||
	    ((s[i]>='a')&&(s[i]<='z'))||
	    ((s[i]>='A')&&(s[i]<='Z')))
		{
		ret<<=4;
		ret+=AsciiToHex(s[i]);
		}
	else return ret;
	}
return ret;
}



uint8_t ConvertUart(uint8_t s)
{
//to upper
if((s == 0x0A) || (s == 0x0D)) return 0x0A;
if((s == 0x09) || (s == ',') || (s == ';')) return ' ';
if((s >= 'a') && (s <= 'z')) return s-('a'-'A');
if((s >= 'A') && (s <= 'Z')) return s;
if((s >= '0') && (s <= '9')) return s;
return s;
}



/* reverse:  переворачиваем строку s на месте */
void reverse(char s[])
{
int i, j;
char c;

for (i = 0, j = strlen(s)-1; i<j; i++, j--)
	{
	c = s[i];
	s[i] = s[j];
	s[j] = c;
	}
}



/* itoa:  конвертируем n в символы в s */
void itoa(int n, char s[])
{
int i, sign;

// записываем знак
if ((sign = n) < 0) n = -n;	// делаем n положительным числом
i = 0;

// генерируем цифры в обратном порядке
do	
	{
	s[i++] = n % 10 + '0';	// берем следующую цифру
	} while ((n /= 10) > 0);	// удаляем
if (sign < 0) s[i++] = '-';
s[i] = '\0';
reverse(s);
}


#ifdef __cplusplus
/* itoa:  конвертируем n в символы в s */
void itoa(int n, char s[], int l)
{
int i=l, sign;

// записываем знак
if ((sign = n) < 0) n = -n;	// делаем n положительным числом
i = 0;

// генерируем цифры в обратном порядке
do
	{
	s[i++] = n % 10 + '0';	// берем следующую цифру
	if (i==l) s[i++]='.';
	(n /= 10) > 0;	// удаляем
	} while (n||(l>=i-1));
if (sign < 0) s[i++] = '-';
s[i] = '\0';
reverse(s);
}
#endif



void ltoa(long int n,char *str)
{
unsigned long i;
unsigned char j,p;
i=1000000000L;
p=0;
if (n<0)
	{
	n=-n;
	*str++='-';
	};
do
	{
	j=(unsigned char) (n/i);
	if (j || p || (i==1))
		{
		*str++=j+'0';
		p=1;
		}
	n%=i;
	i/=10L;
	}
while (i!=0);
*str=0;
}



void ltoa_null(long n, char s[], int l)
{
long sign;
// записываем знак
if ((sign = n) < 0) n = -n;	// делаем n положительным числом

for (uint8_t i=0; i<l; i++)
	{
	s[i] = n%10+'0';
	n/=10;
	};
if (sign<0)
	{
	s[l] = '-';
	s[l+1] = '\0';
	}
else s[l] = '\0';
reverse(s);
}
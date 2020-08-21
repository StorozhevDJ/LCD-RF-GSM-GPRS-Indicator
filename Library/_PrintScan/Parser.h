

#include "Print.hpp"

class Parser : public Print
	{
	public:
	int FindPos (const char *str, char ch, char num);
	int GetParam(const char *str, char ch, char num, char &val);
	int GetParam(const char *str, char ch, char num, int  &val);
	int GetParam(const char *str, char ch, char num, long &val);
	int GetParam(const char *str, char ch, char num, short &val);
	int GetParam(const char *str, char ch, char num, char *val);
	
	
	void printNumber(unsigned long n, uint8_t base);
	void printFloat(double number, uint8_t digits) ;
	void BufPrintf(char *str, const char *format ...);
	
	private:
	void write(uint8_t b);
	/* reverse:  переворачиваем строку s на месте */
	void reverse(char s[]);
	/* itoa:  конвертируем n в символы в s */
	void itoa(int n, char s[]);
	void ltoa(long int n,char *str);
	/* itoa:  конвертируем n в символы в s */
	void itoa_dot(int n, char s[], int l);
	void ltoa_null(long n, char s[], int l);
	//size_t write(uint8_t b);
	};

//extern Parser parser;
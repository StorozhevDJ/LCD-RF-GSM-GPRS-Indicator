#ifndef _INETGSM_H_ 
#define _INETGSM_H_

#define BUFFERSIZE 1

#include "SIM800.h"



extern const char gprs_apn[][24];
extern const char gprs_user[][8];
extern const char gprs_pass[][8];


class InetGSM {
private:
     char _buffer[BUFFERSIZE];

public:
     int httpGET (const char* server, int port, const char* path, char* result, int resultlength);
     int httpPOST(const char* server, int port, const char* path, const char* parameters, char* result, int resultlength);
	bool httpPOSTConnect(const char* server,	//Server address to connect
					 int port,			//Port number
					 const char* path,		//Path to script
					 int body_len);		//Size of POST parameters
	bool httpPOSTConnect(const char * url,		//Server address and path to script
					 int port,			//Port number
					 int body_len);		//Size of POST parameters
	int httpPOSTAdd(uint8_t (*get_data)(uint8_t *buf, uint8_t cnt, int i),//Callback function to get data for POST parameters
				 int data_len,				//Size of POST parameters
				 const char* name,			//Name of parameters
				 bool http_binary_data_type,	//ASCII tipe = false, Binary type = true
				 bool send_data);			//Send data or only calculate lenght data
	int httpPOSTAdd(void * data,				//Data to send
				 int data_len,				//Size of POST parameters
				 const char* name,			//Name of parameters
				 bool http_binary_data_type,	//ASCII tipe = false, Binary type = true
				 bool send_data);			//Send data or only calculate lenght data
	int httpPOSTAdd(const char * name,			//Name of parameters
				 const char * string,		//String to send
				 bool send_data);			//Send data or only calculate lenght data
	int httpPOSTAdd(const char *name,			//Name of parameters
				 long val,				//Int val to send
				 bool send_data);			//Send data or only calculate lenght data
	bool httpPOSTEnd(void);

     // Fast and dirty solution. Should make a "mail" object. And by the moment it does not run.
     int openmail(char* server, char* loginbase64, char* passbase64, char* from, char* to, char* subj);
     int closemail();
	int attachGPRS(TEopsos opsos);
     int attachGPRS(const char* apn, const char* user, const char* pass);
     int dettachGPRS();
     int connectTCP(const char* server, int port);
     int disconnectTCP();
     int connectTCPServer(int port);
     boolean connectedClient();

     // This runs, yes
     //int tweet(const char* token, const char* msg);

};

#endif




#include "inetGSM.h"


#define _GSM_CONNECTION_TOUT_ 5
#define _TCP_CONNECTION_TOUT_ 20
#define _GSM_DATA_TOUT_ 10


const char gprs_apn[][24]=
	{
	"",
	//Russia
	"internet.mts.ru",		//MTS
	"internet",			//Megafon
	"internet",			//НСС, Aline-gsm, «Ростелеком»
	"internet",			//ЕТК, «Ростелеком»
	"internet.smarts.ru",	//СМАРТС
	"inet.bwc.ru",			//Байкалвестком, «Ростелеком»
	"internet.ntc",		//НТК
	"internet.usi.ru",		//Utel, «Ростелеком»
	"internet.tele2.ru",	//Tele2
	"inet.ycc.ru",			//motiv
	"internet.tmb.ru",		//Тамбов GSM, «Ростелеком»
	"internet.ogsm.ru",		//Оренбург-GSM, «Ростелеком»
	"internet.beeline.ru",	//Beeline
	//Belarus
	"m2m30.velcom.by",		//25701 Велком / Velcom
	"mts",				//25702 МТС / MTS
	"internet.life.com.by"	//25704 Лайф / Life
	};

const char gprs_user[][8]=
	{
	"",
	"mts",		//MTS
	"gdata",		//Megafon
	"",			//НСС, Aline-gsm, «Ростелеком»
	"",			//ЕТК, «Ростелеком»
	"",			//СМАРТС
	"",			//Байкалвестком, «Ростелеком»
	"",			//НТК
	"",			//Utel, «Ростелеком»
	"",			//Tele2
	"motiv",		//motiv
	"",			//Тамбов GSM, «Ростелеком»
	"",			//Оренбург-GSM, «Ростелеком»
	"beeline",	//Beeline
	//Belarus
	"",			//25701 Велком / Velcom
	"mts",		//25702 МТС / MTS
	""			//25704 Лайф / Life
	};

const char gprs_pass[][8]=
	{
	"",
	"mts",		//MTS
	"gdata",		//Megafon
	"",			//НСС, Aline-gsm, «Ростелеком»
	"",			//ЕТК, «Ростелеком»
	"",			//СМАРТС
	"",			//Байкалвестком, «Ростелеком»
	"",			//НТК
	"",			//Utel, «Ростелеком»
	"",			//Tele2
	"motiv",		//motiv
	"",			//Тамбов GSM, «Ростелеком»
	"",			//Оренбург-GSM, «Ростелеком»
	"beeline",	//Beeline
	//Belarus
	"",			//25701 Велком / Velcom
	"mts",		//25702 МТС / MTS
	""			//25704 Лайф / Life
	};





int InetGSM::httpGET(const char* server, int port, const char* path, char* result, int resultlength)
{
boolean connected=false;
int n_of_at=0;
//int length_write;
char end_c[2];
end_c[0]=0x1a;
end_c[1]='\0';

/*
Status = ATTACHED.
if(gsm.getStatus()!=GSM::ATTACHED)
  return 0;
*/
while(n_of_at<3)
	{
	if(!connectTCP(server, port))
		{
#ifdef DEBUG_ON
               printf("DB:NOT CONN");
#endif
		n_of_at++;
		}
	else
		{
		connected=true;
		n_of_at=3;
		}
	}

if(!connected) return 0;

gsm.SimpleWrite("GET "); gsm.SimpleWrite(path); gsm.SimpleWrite(" HTTP/1.0\r\n");
gsm.SimpleWrite("Host: "); gsm.SimpleWrite(server); gsm.SimpleWrite("\r\n");
gsm.SimpleWrite("User-Agent: TritonGSM\r\n");
gsm.SimpleWrite("\r\n");
gsm.SimpleWrite(end_c);

switch(gsm.WaitResp(15000, 10, "SEND OK"))
	{
	case RX_TMOUT_ERR:
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV:
		return 0;
	break;
	}


delay(50);
#ifdef DEBUG_ON
     printf("DB:SENT");
#endif
int res = gsm.read(result, resultlength);

//gsm.disconnectTCP();
return res;
}



int InetGSM::httpPOST(const char* server, int port, const char* path, const char* parameters, char* result, int resultlength)
{
     boolean connected=false;
     int n_of_at=0;
     char itoaBuffer[8];
     //int num_char;
     char end_c[2];
     end_c[0]=0x1a;
     end_c[1]='\0';

     while(n_of_at<3) {
          if(!connectTCP(server, port)) {
#ifdef DEBUG_ON
               printf("DB:NOT CONN");
#endif
               n_of_at++;
          } else {
               connected=true;
               n_of_at=3;
          }
     }

     if(!connected) return 0;

	gsm.SimpleWrite("POST "); gsm.SimpleWrite(path); gsm.SimpleWrite(" HTTP/1.1\r\n");
	gsm.SimpleWrite("Host: "); gsm.SimpleWrite(server); gsm.SimpleWrite("\r\n");
     gsm.SimpleWrite("User-Agent: TritonGSM\r\n");
     gsm.SimpleWrite("Content-Type: application/x-www-form-urlencoded\r\n");
     gsm.SimpleWrite("Content-Length: "); 
     itoa(strlen(parameters),itoaBuffer,10);
     gsm.SimpleWrite(itoaBuffer);
     gsm.SimpleWrite("\r\n\r\n");
     gsm.SimpleWrite(parameters);
     gsm.SimpleWrite("\r\n\r\n");
     gsm.SimpleWrite(end_c);

     switch(gsm.WaitResp(15000, 10, "SEND OK")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }

     delay(50);
#ifdef DEBUG_ON
     printf("DB:SENT");
#endif

     int res= gsm.read(result, resultlength);
     //gsm.disconnectTCP();
     return res;
}



bool InetGSM::httpPOSTConnect(const char * url,	//Server address and path to script
					    int port,		//Port number
					    int body_len)	//Size of POST parameters
{
char *script=(char *)strchr(url, '/');
char server[32]={};
strncpy(server, url, script-url);
//server[script-url]=0;
return httpPOSTConnect(server, port, script, body_len);
}



/* Stream binary POST inquery
POST http://www.site.ru/script.php HTTP/1.0\r\n 
Host: www.site.ru\r\n 
Content-Type: multipart/form-data; boundary=1BEF0A57BE110FD467A\r\n
Content-Length: 491\r\n
\r\n
--1BEF0A57BE110FD467A\r\n
Content-Disposition: form-data; name="news_file"; filename="measure.bin"\r\n
Content-Type: application/octet-stream\r\n
Content-Transfer-Encoding: binary\r\n
\r\n
Данные для передачи в файле.bin\r\n
--1BEF0A57BE110FD467A--\r\n
*/
bool InetGSM::httpPOSTConnect(const char* server,	//Server address to connect
					    int port,			//Port number
					    const char* path,		//Path to script
					    int body_len)		//Size of POST parameters
{
bool connected=false;
int n_of_at=0;
char itoaBuffer[8];

body_len+= 2 + 8 + 2;	//"--" + boundary + "--"

if (!gsm.IsDataMode())
	{
	while(n_of_at<3)
		{
		if(!connectTCP(server, port)) n_of_at++;
		else
			{
			connected=true;
			n_of_at=3;
			}
		}
	if(!connected) return false;
	if (!gsm.IsDataMode()) return false;
	}

gsm.SimpleWrite("POST "); gsm.SimpleWrite(path); gsm.SimpleWrite(" HTTP/1.1\r\n\
Host: "); gsm.SimpleWrite(server); gsm.SimpleWrite("\r\n\
User-Agent: TritonGSM\r\n\
Content-Type: multipart/form-data; boundary=PEFuyHYu\r\n\
Content-Length: "); itoa(body_len, itoaBuffer); gsm.SimpleWrite(itoaBuffer); gsm.SimpleWrite("\r\n\
\r\n");

/*gsm.SimpleWrite(end_c);

switch(gsm.WaitResp(15000, 10, "SEND OK"))
	{
	case RX_TMOUT_ERR:
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV:
		return 0;
	break;
	}*/

delay(50);
#ifdef DEBUG_ON
	printf("DB:SENT");
#endif

//int res= gsm.read(result, resultlength);

//return res;
return true;
}



int InetGSM::httpPOSTAdd(uint8_t (*get_data)(uint8_t *buf, uint8_t cnt, int i),//Callback function to get data for POST parameters
					int data_len,			//num of POST parameters
					const char* name,		//Name of parameters
					bool http_binary_data_type,//ASCII tipe = false, Binary type = true
					bool send_data)		//Send data or only calculate lenght data
{
int len=0;

if (send_data)
	{
	if (!gsm.IsDataMode()) return false;
	gsm.SimpleWrite("\
--PEFuyHYu\r\n\
Content-Disposition: form-data; name=\""); gsm.SimpleWrite(name); gsm.SimpleWrite("\"\r\n");
	if (http_binary_data_type==true) gsm.SimpleWrite("\
Content-Type: application/octet-stream\r\n\r\n");
	else gsm.SimpleWrite					   ("\
Content-Type: text/plain\r\n\r\n");
	}
for (int i=0; i<data_len; i++)
	{
	uint8_t param[64];
	uint8_t data_len_ret=get_data(param, 64, i);
	if (data_len_ret==0) break;
	len+=data_len_ret;
	if (send_data) gsm.SimpleWriteArray(&param, data_len_ret);
	}
if (send_data)
	{
	gsm.SimpleWrite("\r\n");
	if (!gsm.IsDataMode()) return false;
	}

return 12+(38+strlen(name)+3)+(http_binary_data_type?42:28)+len+2;
}



int InetGSM::httpPOSTAdd(void * data,			//Data to send
					int data_len,			//Size of POST parameters
					const char* name,		//Name of parameters
					bool http_binary_data_type,//ASCII tipe = false, Binary type = true
					bool send_data)		//Send data or only calculate lenght data
{
if (send_data)
	{
	if (!gsm.IsDataMode()) return false;
	gsm.SimpleWrite("\
--PEFuyHYu\r\n\
Content-Disposition: form-data; name=\""); gsm.SimpleWrite(name); gsm.SimpleWrite("\"\r\n");
	if (http_binary_data_type==true) gsm.SimpleWrite("\
Content-Type: application/octet-stream\r\n");
	else gsm.SimpleWrite					   ("\
Content-Type: text/plain\r\n");

	uint8_t * data_tmp;
	data_tmp =(uint8_t *)data;
	for (int i=0; i<data_len; )
		{
		char tmp=*data_tmp++;
		gsm.SimpleWrite(tmp);
		/*uint8_t param[100];
		int len=get_data(param, 100, i);
		i+=len;
		gsm.SimpleWriteArray(&param, len);*/
		}
	gsm.SimpleWrite("\r\n");
	if (!gsm.IsDataMode()) return false;
	}
return 12+(38+strlen(name)+3)+(http_binary_data_type?40:26)+data_len+2;
}


										    
int InetGSM::httpPOSTAdd(const char *name,		//Name of parameters
					long val,				//Int val to send
					bool send_data)		//Send data or only calculate lenght data
{
char str[12];
itoa(val, str);
return httpPOSTAdd(name, str, send_data);
}



int InetGSM::httpPOSTAdd(const char *name,		//Name of parameters
					const char *string,		//String to send
					bool send_data)		//Send data or only calculate lenght data
{
if (send_data)
	{
	if (!gsm.IsDataMode()) return false;
	gsm.SimpleWrite("\
--PEFuyHYu\r\n\
Content-Disposition: form-data; name=\""); gsm.SimpleWrite(name); gsm.SimpleWrite("\"\r\n\
Content-Type: text/plain\r\n\r\n");
	gsm.SimpleWrite(string);
	gsm.SimpleWrite("\r\n");
	if (!gsm.IsDataMode()) return false;
	}
return 12+(38+strlen(name)+3)+(24+4)+strlen(string)+2;
}



bool InetGSM::httpPOSTEnd(void)
{
if (!gsm.IsDataMode()) return false;
gsm.SimpleWrite("--PEFuyHYu--");
if (!gsm.IsDataMode()) return false;
/*char end_c[2];
end_c[0]=0x1a;
end_c[1]='\0';
gsm.SimpleWrite(end_c);
switch(gsm.WaitResp(15000, 10, "SEND OK"))
	{
	case RX_TMOUT_ERR:
		return false;
	break;
	case RX_FINISHED_STR_NOT_RECV:
		return false;
	break;
	}*/
return true;
}



/*bool InetGSM::httpPOSTReadHead(void)
{
switch(gsm.WaitResp(10000, 10, "HTTP/1.1 "))
	{
	case RX_TMOUT_ERR:
		return false;
	break;
	case RX_FINISHED_STR_NOT_RECV:
		return false;
	break;
	}
return true;
}*/



int InetGSM::openmail(char* server, char* loginbase64, char* passbase64, char* from, char* to, char* subj)
{
     boolean connected=false;
     int n_of_at=0;
     char end_c[2];
     end_c[0]=0x1a;
     end_c[1]='\0';

     while(n_of_at<3) {
          if(!connectTCP(server, 25)) {
#ifdef DEBUG_ON
               printf("DB:NOT CONN");
#endif
               n_of_at++;
          } else {
               connected=true;
               n_of_at=3;
          }
     }

     if(!connected) return 0;

     delay(100);
     gsm.SimpleWrite("HELO ");
     gsm.SimpleWrite(server);
     gsm.SimpleWrite("\n");
     gsm.SimpleWrite(end_c);
     gsm.WaitResp(5000, 100, "OK");
     if(!gsm.IsStringReceived("SEND OK"))
          return 0;
     delay(500);
     gsm.WaitResp(5000, 100);

     delay(100);
     gsm.SimpleWriteln("AT+CIPSEND");
     switch(gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     gsm.SimpleWrite("AUTH LOGIN\n");
     gsm.SimpleWrite(end_c);
     gsm.WaitResp(5000, 100, "OK");
     if(!gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     gsm.WaitResp(5000, 100);

     delay(100);
     gsm.SimpleWriteln("AT+CIPSEND");
     switch(gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     gsm.SimpleWrite(loginbase64);
     gsm.SimpleWrite("\n");
     gsm.SimpleWrite(end_c);
     gsm.WaitResp(5000, 100, "OK");
     if(!gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     gsm.WaitResp(5000, 100);

     delay(100);
     gsm.SimpleWriteln("AT+CIPSEND");
     switch(gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     gsm.SimpleWrite(passbase64);
     gsm.SimpleWrite("\n");
     gsm.SimpleWrite(end_c);
     gsm.WaitResp(5000, 100, "OK");
     if(!gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     gsm.WaitResp(5000, 100);


     delay(100);
     gsm.SimpleWriteln("AT+CIPSEND");
     switch(gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     gsm.SimpleWrite("MAIL From: <");
     gsm.SimpleWrite(from);
     gsm.SimpleWrite(">\n");
     gsm.SimpleWrite(end_c);
     gsm.WaitResp(5000, 100, "OK");
     if(!gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     gsm.WaitResp(5000, 100, "");

     delay(100);
     gsm.SimpleWriteln("AT+CIPSEND");
     switch(gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     gsm.SimpleWrite("RCPT TO: <");
     gsm.SimpleWrite(to);
     gsm.SimpleWrite(">\n");
     gsm.SimpleWrite(end_c);
     gsm.WaitResp(5000, 100, "OK");
     if(!gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     gsm.WaitResp(5000, 100, "");

     delay(100);
     gsm.SimpleWriteln("AT+CIPSEND");
     switch(gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     gsm.SimpleWrite("Data\n");
     gsm.SimpleWrite(end_c);
     gsm.WaitResp(5000, 100, "OK");
     if(!gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     gsm.WaitResp(5000, 100, "");

     delay(100);
     gsm.SimpleWriteln("AT+CIPSEND");
     switch(gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     gsm.SimpleWrite("Subject: ");
     gsm.SimpleWrite(subj);
     gsm.SimpleWrite("\n\n");

     return 1;
}



int InetGSM::closemail()
{
     char end_c[2];
     end_c[0]=0x1a;
     end_c[1]='\0';

     gsm.SimpleWrite("\n.\n");
     gsm.SimpleWrite(end_c);
     disconnectTCP();
     return 1;
}


																  
int InetGSM::attachGPRS(TEopsos opsos)
{
return attachGPRS(&gprs_apn[opsos][0], &gprs_user[opsos][0], &gprs_pass[opsos][0]);
}



int InetGSM::attachGPRS(const char* apn, const char* user, const char* pass)
{
//int i=0;
delay(5000);

//gsm._tf.setTimeout(_GSM_DATA_TOUT_);	//Timeout for expecting modem responses.
gsm.WaitResp(50, 50);
gsm.SimpleWriteln("AT+CIFSR");//Get Local IP Address
if(gsm.WaitResp(10000, 50, "ERROR")!=RX_FINISHED_STR_RECV)
	{
#ifdef DEBUG_ON
	printf("DB:ALREADY HAVE AN IP");
#endif
	gsm.SimpleWriteln("AT+CIPCLOSE");
	gsm.WaitResp(10000, 50, "ERROR");
	delay(2000);
	gsm.SimpleWriteln("AT+CIPSERVER=0");
	gsm.WaitResp(10000, 50, "ERROR");
	return 1;
	}
else
	{
#ifdef DEBUG_ON
	printf("DB:STARTING NEW CONNECTION");
#endif
	//Deactivate GPRS PDP context
	gsm.SimpleWriteln("AT+CIPSHUT");
	switch(gsm.WaitResp(500, 50, "SHUT OK"))
		{
		case RX_TMOUT_ERR: 
			return 0;
		break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
		break;
		}
#ifdef DEBUG_ON
	printf("DB:SHUTTED OK");
#endif
	delay(1000);
	//Start Task and Set APN, USER NAME, PASSWORD
	gsm.SimpleWrite("AT+CSTT=\"");
	gsm.SimpleWrite(apn);
	gsm.SimpleWrite("\",\"");
	gsm.SimpleWrite(user);
	gsm.SimpleWrite("\",\"");
	gsm.SimpleWrite(pass);
	gsm.SimpleWrite("\"\r");
	switch(gsm.WaitResp(500, 50, "OK"))
		{
		case RX_TMOUT_ERR:
			return 0;
		break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
		break;
		}
#ifdef DEBUG_ON
	printf("DB:APN OK");
#endif
	delay(5000);
	
	//Bring Up Wireless Connection with GPRS or CSD
	gsm.SimpleWriteln("AT+CIICR");
	switch(gsm.WaitResp(10000, 50, "OK"))
		{
		case RX_TMOUT_ERR:
			return 0;
		break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
		break;
		}
#ifdef DEBUG_ON
	printf("DB:CONNECTION OK");
#endif

	delay(1000);

	//Get Local IP Address
	gsm.SimpleWriteln("AT+CIFSR");
	if(gsm.WaitResp(5000, 50, "ERROR")!=RX_FINISHED_STR_RECV)
		{
#ifdef DEBUG_ON
		printf("DB:ASSIGNED AN IP");
#endif
		gsm.setStatus(gsm.ATTACHED);
		return 1;
		}
#ifdef DEBUG_ON
	printf("DB:NO IP AFTER CONNECTION");
#endif
	return 0;
	}
}



int InetGSM::dettachGPRS()
{
     if (gsm.getStatus()==gsm.IDLE) return 0;

     //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);

     //_cell.flush();

     //GPRS dettachment.
     gsm.SimpleWriteln("AT+CGATT=0");
     if(gsm.WaitResp(5000, 50, "OK")!=RX_FINISHED_STR_NOT_RECV) {
          gsm.setStatus(gsm.ERROR);
          return 0;
     }
     delay(500);

     // Commented in initial trial code!!
     //Stop IP stack.
     //_cell << "AT+WIPCFG=0" <<  _DEC(cr) << endl;
     //	if(!gsm._tf.find("OK")) return 0;
     //Close GPRS bearer.
     //_cell << "AT+WIPBR=0,6" <<  _DEC(cr) << endl;

     gsm.setStatus(gsm.READY);
     return 1;
}



int InetGSM::connectTCP(const char* server, int port)
{
//gsm._tf.setTimeout(_TCP_CONNECTION_TOUT_);

     //Status = ATTACHED.
     //if (getStatus()!=ATTACHED)
     //return 0;

     //_cell.flush();

     //Visit the remote TCP server.
gsm.SimpleWrite("AT+CIPSTART=\"TCP\",\"");
gsm.SimpleWrite(server);
gsm.SimpleWrite("\",");
gsm.SimpleWriteln(port);

switch(gsm.WaitResp(1000, 200, "OK"))
	{
	case RX_TMOUT_ERR:
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV:
		return 0;
	break;
	}
#ifdef DEBUG_ON
printf("DB:RECVD CMD");
#endif
if (/*(!gsm.IsStringReceived("CONNECT OK"))&&*/(!gsm.IsStringReceived("CONNECT")))
	{
	switch(gsm.WaitResp(15000, 200, "CONNECT"))
		{
		case RX_TMOUT_ERR:
			return 0;
		break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
		break;
		}
	}

#ifdef DEBUG_ON
printf("DB:OK TCP");
#endif

delay(3000);
/*gsm.SimpleWriteln("AT+CIPSEND");
switch(gsm.WaitResp(5000, 200, ">"))
	{
	case RX_TMOUT_ERR:
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV:
		return 0;
	break;
	}*/

#ifdef DEBUG_ON
printf("DB:>");
#endif
delay(2000);
return 1;
}



int InetGSM::disconnectTCP()
{
     //Status = TCPCONNECTEDCLIENT or TCPCONNECTEDSERVER.
     /*
     if ((getStatus()!=TCPCONNECTEDCLIENT)&&(getStatus()!=TCPCONNECTEDSERVER))
        return 0;
     */
     //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);


     //_cell.flush();

     //Switch to AT mode.
     //_cell << "+++" << endl;

     //delay(200);

     //Close TCP client and deact.
     gsm.SimpleWriteln("AT+CIPCLOSE");

     //If remote server close connection AT+QICLOSE generate ERROR
     /*if(gsm._tf.find("OK"))
     {
       if(getStatus()==TCPCONNECTEDCLIENT)
         gsm.setStatus(ATTACHED);
       else
         gsm.setStatus(TCPSERVERWAIT);
       return 1;
     }
     gsm.setStatus(ERROR);

     return 0;    */
     if(gsm.getStatus()==gsm.TCPCONNECTEDCLIENT)
          gsm.setStatus(gsm.ATTACHED);
     else
          gsm.setStatus(gsm.TCPSERVERWAIT);
     return 1;
}

int InetGSM::connectTCPServer(int port)
{
     /*
       if (getStatus()!=ATTACHED)
          return 0;
     */
     //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);

     //_cell.flush();

     // Set port

     gsm.SimpleWrite("AT+CIPSERVER=1,");
     gsm.SimpleWriteln(port);
     /*
       switch(gsm.WaitResp(5000, 50, "OK")){
     	case RX_TMOUT_ERR:
     		return 0;
     	break;
     	case RX_FINISHED_STR_NOT_RECV:
     		return 0;
     	break;
       }

       switch(gsm.WaitResp(5000, 50, "SERVER")){ //Try SERVER OK
     	case RX_TMOUT_ERR:
     		return 0;
     	break;
     	case RX_FINISHED_STR_NOT_RECV:
     		return 0;
     	break;
       }
     */
     //delay(200);

     return 1;

}

boolean InetGSM::connectedClient()
{
     /*
     if (getStatus()!=TCPSERVERWAIT)
        return 0;
     */

     gsm.SimpleWriteln("AT+CIPSTATUS");
     // Alternative: AT+QISTAT, although it may be necessary to call an AT
     // command every second,which is not wise
     /*
     switch(gsm.WaitResp(1000, 200, "OK")){
     case RX_TMOUT_ERR:
     	return 0;
     break;
     case RX_FINISHED_STR_NOT_RECV:
     	return 0;
     break;
     }*/
     //gsm._tf.setTimeout(1);
     if(gsm.WaitResp(5000, 50, "CONNECT OK")!=RX_FINISHED_STR_RECV) {
          gsm.setStatus(gsm.TCPCONNECTEDSERVER);
          return true;
     } else
          return false;
}




/*int InetGSM::httpGETstream(const char* server, int port, char* result, int resultlength)
{
boolean connected=false;
int n_of_at=0;
//int length_write;
char end_c[2];
end_c[0]=0x1a;
end_c[1]='\0';*/

/*
Status = ATTACHED.
if(gsm.getStatus()!=GSM::ATTACHED)
  return 0;
*/
/*while(n_of_at<3)
	{
	if(!connectTCP(server, port))
		{
#ifdef DEBUG_ON
               printf("DB:NOT CONN");
#endif
		n_of_at++;
		}
	else
		{
		connected=true;
		n_of_at=3;
		}
	}

if(!connected) return 0;

gsm.SimpleWrite("GET ");
gsm.SimpleWrite(path);
gsm.SimpleWrite(" HTTP/1.0\r\nHost: ");
gsm.SimpleWrite(server);
gsm.SimpleWrite("\r\n");
gsm.SimpleWrite("User-Agent: TritonGSM");
gsm.SimpleWrite("\r\n\r\n");
gsm.SimpleWrite(end_c);

switch(gsm.WaitResp(10000, 10, "SEND OK"))
	{
	case RX_TMOUT_ERR:
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV:
		return 0;
	break;
	}


delay(50);
#ifdef DEBUG_ON
     printf("DB:SENT");
#endif
int res = gsm.read(result, resultlength);

//gsm.disconnectTCP();

return res;
}*/
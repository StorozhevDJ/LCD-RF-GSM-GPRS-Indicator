#ifndef SIMCOM800_H
#define SIMCOM800_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Utils/Utils.h"

#include "SIM800HWDriver.h"
#include "GSM.h"


//OpSoS code (see http://cellidfinder.com/mcc-mnc)
typedef enum
	{
	MCCMNC_none=0,
	//Russia
	MCCMNC_mts=25001,		//Mobile TeleSystems 	MTS (МТС)
	MCCMNC_megafon=25002,	//MegaFon OJSC 	MegaFon (Мегафон)
	MCCMNC_aline_gsm=25003,	//НСС, Aline-gsm, «Ростелеком», Nizhegorodskaya Cellular Communications 	NCC
	MCCMNC_etk=25005,		//ЕТК, «Ростелеком», Yeniseytelecom 	ETK
	MCCMNC_skylink1=25006,	//CJSC Saratov System of Cellular Communications 	Skylink (Скайлинк)
	MCCMNC_smarts=25007,	//Zao SMARTS 	SMARTS
	MCCMNC_skylink2=25009,	//Khabarovsky Cellular Phone 	Skylink (Скайлинк)
	MCCMNC_yata=25011,		//Yota
	MCCMNC_bwc=25012,		//Байкалвестком, «Ростелеком»
	MCCMNC_smarts_ufa=25015,	//SMARTS Ufa, SMARTS Uljanovsk 	SMARTS
	MCCMNC_ntc=25016,		//НТК
	MCCMNC_Utel=25017,		//Utel, «Ростелеком»
	MCCMNC_tele2=25020,		//Tele2
	MCCMNC_k_tel=25032,		//K-Telecom	Win Mobile
	MCCMNC_motiv=25035,		//motiv
	MCCMNC_kodotel=25037,	//ZAO Kodotel	ZAO Kodotel
	MCCMNC_tmb=25038,		//Тамбов GSM, «Ростелеком»
	MCCMNC_ogsm=25039,		//Оренбург-GSM, «Ростелеком»
	MCCMNC_mts2=25050,		//Mobile TeleSystems	MTS (МТС)
	MCCMNC_beeline=25099,	//Beeline
	//Belarus
	MCCMNC_velcom_by=25701,	//MDC Velcom 	Velcom
	MCCMNC_mts_by=25702,	//Mobile TeleSystems	MTS
	MCCMNC_belcel=25703,	//BelCel	DIALLOG
	MCCMNC_life_by=25704,	//Belarussian Telecommunications Network	life :)
	//Kazakhstan
	MCCMNC_kar_tel=40101,	//Kar-Tel llc 	Beeline	Operational
	MCCMNC_kcell_kz=40102,	//GSM Kazakhstan Ltd	Kcell	Operational
	MCCMNC_dalacom=40107,	//Dalacom (CDMA)	Dalacom	Operational
	MCCMNC_kaztel=40108,	//Kazakhtelecom	Kazakhtelecom	Operational
	MCCMNC_mts_kz=40177		//Mobile Telecom Service LLP	Mobile Telecom Service	Operational
	} TEGSM_OPSOS;







//OpSoS list in base
typedef enum
	{
	opsos_none=0,
	mts_rus,			//MTS
	megafon_rus,		//Megafon
	aline_gsm_rus,		//НСС, Aline-gsm, «Ростелеком»
	etk_rus,			//ЕТК, «Ростелеком»
	smarts_rus,		//СМАРТС
	bwc_rus,			//Байкалвестком, «Ростелеком»
	ntc_rus,			//НТК
	Utel_rus,			//Utel, «Ростелеком»
	tele2_rus,		//Tele2
	motiv_rus,		//motiv
	tmb_rus,			//Тамбов GSM, «Ростелеком»
	ogsm_rus,			//Оренбург-GSM, «Ростелеком»
	beeline_rus,		//Beeline
	//Belarus
	velcom_by,		//Велком / Velcom
	mts_by,			//МТС / MTS
	life_by			//Лайф / Life
	} TEopsos;



class SIMCOM800 : public virtual GSM {

private:
     int configandwait(char* pin);
     int setPIN(char *pin);
     int changeNSIPmode(char);
	
	//uint8_t

public:
     SIMCOM800();
     ~SIMCOM800();
     int getCCI(char* cci);
     int getIMEI(char* imei);
	signed char GetSignalQuality();
	unsigned char GetBattery();
	unsigned char GetTemperature();
	void PowerDown();
     int sendSMS(const char* to, const char* msg);
     boolean readSMS(char* msg, int msglength, char* number, int nlength);
     boolean readCall(char* number, int nlength);
     boolean call(char* number, unsigned int milliseconds);
     char forceON();
     virtual int read(char* result, int resultlength);
     virtual uint8_t read();
	virtual int read(char* result, int resultlength, int timeout);
     virtual int available();
	virtual void flush();
	virtual bool IsDataMode();
	virtual void SetDataMode();
	virtual void SetCmdMode();
     bool readCellData(int &mcc, char &mnc, int &lac, int &cellid, char &rxl);
	char readCellData(int *mcc, char *mnc, int *lac, int *cellid, char *rxl);
	TEopsos GetOpsos(short mcc, char mnc);
     void SimpleRead();
     void WhileSimpleRead();
     size_t SimpleWrite(char *comm);
     size_t SimpleWrite(char const *comm);
     void SimpleWrite(int comm);
     //void SimpleWrite(const __FlashStringHelper *pgmstr);
     size_t SimpleWriteln(char *comm);
     size_t SimpleWriteln(char const *comm);
     //void SimpleWriteln(const __FlashStringHelper *pgmstr);
     void SimpleWriteln(int comm);
	void SimpleWriteArray(void * array, int len);
};

extern SIMCOM800 gsm;

#endif


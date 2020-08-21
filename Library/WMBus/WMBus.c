
#include <stdint.h>




#include "WMBus.h"










/*******************************************************************************
* 
*******************************************************************************/
void InitWMBus(void)
{
//Готовим передающий буфер (заполняем нужными данными)
/*TSset *set;
set=(TSset *)eeSET;
for (uint8_t i=0; i<MAX_CNT_LIST; i++)
	{
	data_counter[i].block0.MANIDF=set->wmbus.manidf;
	data_counter[i].block0.ADDRF =set->wmbus.addrf;
	data_counter[i].block0.VERF  =set->wmbus.verf;
	data_counter[i].block0.TYPEF =set->wmbus.typef;
	data_counter[i].block0.LF=sizeof(TSwmbPacket_data)-1-2;//-Size -CRC1
	data_counter[i].block0.CF=SND_NR;	//0x44
	
	data_counter[i].CIF=WMB_LONG_HDR_TRANSPORT;	//CI - 8B
	
	data_counter[i].header.access_no=0;	//Номер доступа
	data_counter[i].header.status=0;		//Status error
	data_counter[i].header.signature=0;	//Signature for encription
	
	for (uint8_t j=0; j<3; j++)
		{
		data_counter[i].block2[j].DIF1=div_32Bit_bin;
		data_counter[i].block2[j].VIF1=vif_0_100l;
		data_counter[i].block2[j].DIF2=div_8Bit_bin;
		data_counter[i].block2[j].VIF2=vife_error_flags;
		}
	}

TSwmbPacket_data_tx *data;
data=(TSwmbPacket_data_tx *)eeData;
for (uint8_t i=0; i<2; i++)
	{
	if (data->CIF==0x8B) data_counter[i]=*data++;
	}*/
}




// *****************************************************************************
// @fn		wmbCreate
// @brief		Создание пакетов для отправки
// @param		uint8_t *txBuf	Pointer to transmit buffer
// @param		uint8_t PacketTYPE	Packet type to create
// @return	uint8_t Return packet lenght
// *****************************************************************************
uint8_t wmbCreatePacket(uint8_t *txBuf, TEPacketType PacketTYPE)
{
TSwmbPacket_data_tx *WP;
WP=(TSwmbPacket_data_tx *)txBuf;

//WP->block0.CF=PacketTYPE;
switch(PacketTYPE)
	{
	case PktType_snd_ad://SND_AD:
		InitWMBus();
		//        WP->MANIDF=devInfo->MANIDF;
		//        WP->ADDRF=(uint32_t *)UnixTime();
		//        WP->VERF=devInfo->VERF;//RTCSEC;  //AlarmState;
		//        WP->TYPEF=devInfo->TYPEF;
	break;
	
	case PktType_snd_ack://SND_ACK:
		{
		/*TSset *set;
		set=(TSset *)eeSET;
		
		WP->block0.MANIDF=set->wmbus.manidf;
		WP->block0.CF	  =PacketTYPE;
		WP->block0.ADDRF =set->wmbus.addrf;
		WP->block0.VERF  =set->wmbus.verf;
		WP->block0.TYPEF =set->wmbus.typef;
		WP->block0.LF	  =0x0E;
		WP->CIF		  =WMB_SHORT_HDR_TRANSPORT;	//CI - 7A (Response from meter), header - 4 bytes
		WP->header.access_no=0;	//Номер доступа
		WP->header.status=0;	//Status error
		WP->header.signature=0;	//Signature for encription*/
		}
	break;
	
	case PktType_snd_nke://SND_NKE:
		return 0;
	break;
	
	case PktType_snd_ud://SND_UD:
		return 0;
	break;
	
	//Send actual data & alarm flags
	case PktType_snd_nr://SND_NR:
	break;

	//ReSPonse User Data (Data from counter)
	case PktType_rsp_ud://RSP_UD:
		return 0;
	break;
	
	//Installation request
	case PktType_snd_ir://SND_IR:
		{
		/*TSset *set;
		set=(TSset *)eeSET;
		
		WP->block0.LF	  =sizeof(TSwmbPacket_SND_IR)-3;//-Size -CRC1
		WP->block0.MANIDF=set->wmbus.manidf;
		WP->block0.CF	  =PacketTYPE;
		WP->block0.ADDRF =set->wmbus.addrf;
		WP->block0.VERF  =set->wmbus.verf;
		WP->block0.TYPEF =set->wmbus.typef;
		WP->CIF		  =WMB_SHORT_HDR_TRANSPORT;	//CI - 8A (Response from meter), header - 4 bytes
		WP->header.access_no=0;	//Номер доступа
		WP->header.status=0;	//Status error
		WP->header.signature=0;	//Signature for encription*/
		}
	break;

	//User Data1 packet (Alarm packet)
	case PktType_req_ud1://REQ_UD1:
		return 0;
	break;
	
	//User Data2 packet
	case PktType_req_ud2://REQ_UD2:
		return 0;
	break;
	
	default:
		return 0;
	break;
	};


// Check CRC
if(WP->block0.LF > 9)
	{
	char part=(WP->block0.LF-/*L_BLOCK0*/sizeof(TSblock0)+15)/16;//Количество частей для подсчета CRC
	char j=WP->block0.LF+1+2;
	for (char i=part; i>0; i--)
		{
		char start=j;
		for ( ; j>/*L_BLOCK0*/sizeof(TSblock0)+16*(i-1); j--)
			{
			txBuf[j+2*(i-1)-1]=txBuf[j-1];
			}
		char stop=j;
		wmbCRCCalc(txBuf+/*L_BLOCK0*/sizeof(TSblock0)+(i-1)*18, start-stop, CRC_APPEND); // CRC16
		}
	}
wmbCRCCalc(txBuf, /*L_BLOCK0*/sizeof(TSblock0)-2 ,CRC_APPEND);
//return wmbPacketLength(WP->block0.LF);
return WP->block0.LF+1+2+2*((WP->block0.LF-/*L_BLOCK0*/sizeof(TSblock0)+15)/16);//Return packet lenght
}



/*******************************************************************************
*					Разбор принятых пакетов
* rxBuf - указатель на приемный буфер
* txBuf - указатель на буфер для передачи
* return - длинна данных в передающем буфере
*******************************************************************************/
uint8_t wmbParse(uint8_t *rxBuf, uint8_t *txBuf)
{
TSblock0 *wP;
wP=(TSblock0 *)rxBuf;

switch(wP->CF)
	{
	case PktType_snd_ack://SND_ACK:

	//0x40	END of LINK
	case PktType_snd_nke://SND_NKE:
		{
		TSwmbPacket_SND_NKE *wp_snd_nke;
		wp_snd_nke=(TSwmbPacket_SND_NKE *)rxBuf;
		if(wp_snd_nke->CIF == cif_LongHdr)
			{
			if ((wp_snd_nke->block2.vif1==0x02FF)&&
			    (wp_snd_nke->block2.dif1==0x03))
				{
				union
					{
					uint8_t arr[3];
					uint32_t sec;
					}tmp_time;
				tmp_time.arr[0]=wp_snd_nke->block2.power_time[0];
				tmp_time.arr[1]=wp_snd_nke->block2.power_time[1];
				tmp_time.arr[2]=wp_snd_nke->block2.power_time[2];
				//PowerDog=tmp_time.sec&0xFFFFFF;
				};
			if ((wp_snd_nke->block0.LF == (sizeof(TSwmbPacket_SND_NKE)-3))&&
			    (wp_snd_nke->block2.vif2 == VIF_DATETIME)&&
			    ((wp_snd_nke->block2.dif2&0x0F)== 0x06))
				{
				uint8_t iT[8];
				// Setup RTC Timer
				iT[0]=wp_snd_nke->block2.datetime.sec;		//sec
				iT[1]=wp_snd_nke->block2.datetime.min;		//min
				iT[2]=wp_snd_nke->block2.datetime.hour;		//Hour
				//iT[3]=wp_snd_nke->block2.datetime.day_of_week;//Day of week
				iT[4]=wp_snd_nke->block2.datetime.day;		//Day
				iT[5]=wp_snd_nke->block2.datetime.month;	//Mon
				iT[6]=wp_snd_nke->block2.datetime.year1+(wp_snd_nke->block2.datetime.year2<<3);	//Year
				//SetupRTCBin(iT);
				};
			}
		}
	break;
	
	//0x44	SEND/NO REPLY, meter initiative
	case PktType_snd_nr://SND_NR:
	
	break;
	
	//0x53	SEND USER DATA
	case PktType_snd_ud://SND_UD:
		{
		TSwmbPacket_SND_UD *data;
		data=(TSwmbPacket_SND_UD *)rxBuf;
		//Получили команду
		if ((data->block2.dif1==div_16Bit_bin)&&(data->block2.vif1==vife_cmd))
			{
			//Команда очистки списка данных устройств
			if (data->block2.cmd==0x0110)
				{
				//CounterListClear();
				return wmbCreatePacket(txBuf, /*SND_ACK*/PktType_snd_ack);
				}
			//Команда запроса текущих данных
			if (data->block2.cmd==0x0210)
				{
				//DataRequest();
				return wmbCreatePacket(txBuf, /*SND_NR*/PktType_snd_nr);
				}
			}
		}
#ifdef KRAN
		//Открыть кран
		faucet=16*3;
		//P1OUT&=~BIT1;
		P1OUT&=~BIT4;
		P3OUT |= BIT5;//Красный
		for (char i=1; i<30; i++) {__delay_cycles(100000); RunWdt();}
		//P1OUT|= BIT1;
		P1OUT|= BIT4;
		P3OUT&=~BIT5;//Красный
		return wmbCreate(txBuf, SND_ACK);
#else //END KRAN
		#ifdef PULSE_METER
		return wmbCreate(txBuf, SND_ACK);
		#endif
#endif
	break;
	
	//0x73	SEND USER DATA
	case PktType_snd_ud73://SND_UD73:
#ifdef KRAN
		//Закрыть кран
		faucet=16*3;
		//P1OUT&=~BIT2;
		P3OUT&=~BIT7;
		P3OUT|= BIT5;//Красный
		for (char i=1; i<30; i++) {__delay_cycles(100000); RunWdt();}
		//P1OUT|= BIT2;
		P3OUT|= BIT7;
		P3OUT&=~BIT5;//Красный
		return wmbCreatePacket(txBuf, /*SND_ACK*/PktType_snd_ack);
#else //END KRAN
		return wmbCreatePacket(txBuf, /*RSP_UD*/PktType_rsp_ud);
#endif
	break;
	
	//0x06	Received manually initiated installation data;
	case PktType_cnf_ir://CNF_IR:
		
	break;

	//0x48	Access demand
	case PktType_snd_ad://SND_AD:
	
	break;
	//0x5A	REQUEST USER DATA1
	case PktType_req_ud1://REQ_UD1:
	
	break;
	//0x5B	REQUEST USER DATA2 (Data request (Request User Data Class2))
	case PktType_req_ud2://REQ_UD2:

	return wmbCreatePacket(txBuf, /*REQ_UD2*/PktType_req_ud2);
	
	//Undefined C-field
	default:
		return 0;
	break;
	}
return 0;
}



// *****************************************************************************
// @fn          wmbCheckAddr
// @brief       Check addresat
// @param       uint8_t *rxBuf	Array WM-Bus data
// @return      false - Not correct address (packet not for my)
// @return      true - My packet
// *****************************************************************************
bool wmbCheckAddr(void *rxBuf)
{
/*TSset *set;
TSheader_long * addr;
set=(TSset *)eeSET;
addr=(TSheader_long *) ((uint8_t *)rxBuf+12+1);
// check adrr
if (addr->ADDRF!=set->wmbus.addrf) return false;
if (addr->VERF !=set->wmbus.verf)  return false;
if (addr->TYPEF!=set->wmbus.typef) return false;*/
return true;
}



// *****************************************************************************
// @fn          wmbCheck
// @brief       Check correct WM-Bus packet (CRC and Lenght for block 1 and 2)
// @param       uint8_t *rxBuf	Array WM-Bus data
// @param       uint8_t cnt		Number of received bytes
// @return      0 - Find errors in WM-Bus packet
// @return      not null - lenght WM-Bus packet
// *****************************************************************************
uint8_t wmbCheckPacket(uint8_t *rxBuf, uint8_t cnt)
{
uint8_t packetL,total,p;
if(cnt < 12) return  0;   // RADIO
//if(cnt > 64) return  0;   // RADIO
// Block 0
if(wmbCRCCalc(rxBuf, /*L_BLOCK0*/sizeof(TSblock0)-2, CRC_CHECK)) return  0;   // RADIO
packetL=wmbPacketLength(rxBuf[0]);
if(packetL > cnt) return  0;   // RADIO
if(packetL < 9)   return  0;   // BUG
if(packetL > 64)  return  0;   // BUG

total = /*L_BLOCK0*/sizeof(TSblock0);
//  Block 1
while(total < packetL)
	{
	p = packetL - total;
	if(p > L_BLOCK1) p=L_BLOCK1;
	if(wmbCRCCalc(rxBuf+total,p-2,CRC_CHECK))  return 0; // RADIO
	else total += p;
	};
return total;
}



/*******************************************************************************
// @fn		wmbCRC
// @brief		Calculate and control CRC
			CRC: Cyclic redundancy check
			The CRC polynomial is: x16 + x13 + x12 + x11 + x10 + x8 + x6 + x5 + x2 + 1
			The initial value is: 0
// @param		uint8_t *rxBuf	Array WM-Bus data
// @param		uint8_t cnt		Counter bytes
// @param		uint8_t mode		0 normal return CRC16
							1 return CRC16 and place to end of buffer
							2 return CRC16 ^ CRC16fromBUFFER (0 if TRUE)
// @return	uint16_t			CRC is complemented
*******************************************************************************/
uint16_t wmbCRCCalc(uint8_t *buf, uint8_t cnt, uint8_t mode)
{
#define wmbPoly 0x3D65
uint8_t i;
union
	{
	uint16_t crc16;
	uint8_t  crc8[2];
	} CRC= {0};

while(cnt--)
	{
	CRC.crc8[1] ^= *buf++;
	for (i = 0; i < 8; i++)
	CRC.crc16 = CRC.crc16 & 0x8000 ? (CRC.crc16 << 1) ^ wmbPoly : CRC.crc16 << 1;
	};
//  Complement output
CRC.crc16 ^= 0xFFFF;
if (mode==CRC_CALC){ }
else if(mode==CRC_APPEND)
	{
	*buf++=CRC.crc8[1];
	*buf++=CRC.crc8[0];
	}
else if(mode==CRC_CHECK)
	{
	CRC.crc8[1]  ^= *buf++;
	CRC.crc8[0]  ^= *buf++;
	};
return CRC.crc16;
}



/*******************************************************************************
// @fn		wmbCRCFree
// @brief		Free CRC data from buffer
// @param		uint8_t  *rxBuf	Array WM-Bus data
// @return	uint8_t  new size of buffer
*******************************************************************************/
uint8_t wmbCRCFree(void *buff)
{
uint8_t *buf;
buf=(uint8_t *)buff;

uint8_t new_len=sizeof(TSblock0);
uint8_t ptr=buf[0]+3;		//packet lenght + size + CRC block0
for (uint8_t i=sizeof(TSblock0); i<ptr; i++)
	{
	buf[new_len++]=buf[i];	//Copy data without CRC bytes
	//if this bytes is CRC, then skiping this bytes
	if (((new_len-sizeof(TSblock0))&0x0F)==0x00)
		{
		i+=2;	//skiping this bytes
		ptr+=2;	//add in lenght two bytes from CRC
		}
	};
return new_len;
}



uint8_t wmbCalcDataLen (void * wmb)
{
TSblock0 *tmp;
tmp=(TSblock0 *)wmb;

return tmp->LF-wmbCalcDataStart(wmb)+1+2;
}



uint8_t wmbCalcDataStart (void * wmb)
{
uint8_t *buf;
buf=(uint8_t *)wmb;

uint8_t header_len;
if (buf[sizeof(TSblock0)]==cif_wmbNoHdr) header_len=0;
else if (buf[sizeof(TSblock0)]==cif_shortHdr) header_len=sizeof(TSheader_short);
//else if (radio_rx_buf[sizeof(TSblock0)]==cif_shortHdrTransport) header_len=sizeof(TSheader_short);
//else if (radio_rx_buf[sizeof(TSblock0)]==cif_longHdrTransport) header_len=sizeof(TSheader_short);
else if (buf[sizeof(TSblock0)]==cif_LongHdr) header_len=sizeof(TSheader_long);
return sizeof(TSblock0)+1+header_len;
}



uint8_t wmbGetDataStruct(void * buf, TSwmbData *wmb_data, int8_t buf_len)
{
uint8_t len=0;
uint8_t *wmb;
wmb=(uint8_t *)buf;

wmb_data->data_len=0;
wmb_data->vif=0;
wmb_data->data=0;

while (buf_len>0)
{
//Разбираем div
if (*wmb==(uint8_t)div_no_data) wmb_data->data_len=0;
else if ((*wmb==(uint8_t)div_8Bit_bin )||(*wmb==(uint8_t)div_2BCD)) wmb_data->data_len=1;
else if ((*wmb==(uint8_t)div_16Bit_bin)||(*wmb==(uint8_t)div_4BCD)) wmb_data->data_len=2;
else if ((*wmb==(uint8_t)div_24Bit_bin)||(*wmb==(uint8_t)div_6BCD)) wmb_data->data_len=3;
else if ((*wmb==(uint8_t)div_32Bit_bin)||(*wmb==(uint8_t)div_8BCD)) wmb_data->data_len=4;
else wmb_data->data_len=0;

wmb_data->div=(TEDIV)*wmb++;
buf_len--;

//Разбираем vif
wmb_data->vif=0;
for (uint8_t i=0; i<4; i++)
	{
	wmb_data->vif|=(*wmb)<<(8*i);
	buf_len--;
	if ((*wmb++&0x80)==0) break;
	}

//Копируем данные
wmb_data->data=0;
if (wmb_data->data_len) for (uint8_t i=0; i<wmb_data->data_len; i++)
	{
	wmb_data->data|=(*wmb++)<<(8*i);
	buf_len--;
	}
wmb_data++;

len++;
}
return len;
}
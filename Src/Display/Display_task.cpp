
//https://code.google.com/archive/p/u8glib/wikis/userreference.wiki
//https://code.google.com/archive/p/m2tklib/wikis/elref.wiki

#include <string.h>

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include "Drivers/LCD/u8g_arm.h"
#include "Drivers/Board/Board.h"
#include "Drivers/Keyboard/keyboard.h"

#include "PrintScan/Parser.hpp"

#include "Display_task.hpp"

#include "CC110L/cc110L.h"
#include "Radio/Radio_task.hpp"
#include "SystemTask/SystemTask.h"

#include "M2tklib/M2tk.h"
#include "M2tklib/utility/m2ghu8g.h"



extern U8GLIB u8glib;
extern CBoard Board;
Parser parser;


TETYPE type[]={type_none, type_meter_electricity, type_meter_gas, type_meter_heat,
	type_meter_warm_water, type_meter_water, type_meter_cold_water, type_ad_converter, 
	type_actuator_electricity, type_actuator_gas, type_actuator_water, type_system_display, 
	type_system_muc, type_system_radio_converter_modem, type_meter_radio_converter_pulses};
const char *name_str[]= {"Неизвестное устройство",
					"Электричество",
					"Газ",
					"Тепло",
					"Горячая вода",
					"Вода",
					"Холодная вода",
					"Аналоговый преобразователь",
					"Выключатель",
					"Газовый кран",
					"Водяной кран",
					"Индикатор",
					"Концентратор",
					"Модем",
					"Преобразователь интерфейсов"};
const char *units_name[]={"кВт/час", "Гкал", "м.куб.", "кг", " ", "Вт", "Мкал/час",
					 "м.куб/час", "л/мин", "л/мин", "кг/час", "°С", "°K", "Бар"};

const int8_t units_exponent_default[]= {6,	//Energy, 10(nnn-3) Wh (kWt)
								9,	//Energy, 10(nnn-3) kJ (GKall)
								6,	//Volume, 10(nnn-3) l (m3)
								3,	//Mass, 10(nnn-3) kg (kg)
								0,	//Time
								3,	//Power, 10(nnn-3) W (W)
								6,	//Energy, 10(nnn-3) kJ/h (MKall/h)
								6,	//Volume Flow, 10(nnn-3) l/h (m3/h)
								0,	//Volume Flow ext., 10(nnn-4) l/min
								0,	//Volume Flow ext., 10(nnn-6) l/s
								0,	//Mass Flow, 10(nnn-3) kg/h
								3,	//Temperature, 10(nn-3) °C
								0,	//Temperature, 10(nn-3) °C (K)
								3	//Pressure, 10(nn-3) bar
								};


uint8_t select_color = 0;

/*******************************************************************************
* accept selection
*******************************************************************************/
void fn_ok(m2_el_fnarg_p fnarg)
{
if (select_color==1)
	{
	/*M2_LABEL(hello_world_label, NULL, "Hello World");
	M2tk m22(&hello_world_label, NULL, NULL, m2_gh_u8g_bfs);
	u8glib.firstPage();  
	do{
		m22.draw();
		} while( u8glib.nextPage() );
	osDelay(3000);*/
	}
if (select_color==0)
	{
	u8glib.firstPage();
		do
		{
		u8glib.setFont(u8g_font_5x8);
		u8glib.drawStr(0, 8, "Error! Color not selected");
		} while ( u8glib.nextPage() );
	osDelay(3000);
	}
if (select_color==0)
	{
	
	}
}


void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
}






extern uint8_t radio_rx_buf[CC110l_DATA_LEN];
extern uint8_t radio_rssi;

extern xQueueHandle queue_radio_rssi_current;

extern TSCounter counters[16];


/* StartDefaultTask function */
void Display_task(void const * argument)
{
Board.WATCHDOG_Init();
//Board.WATCHDOG_Start();

// Connect u8glib with m2tklib
m2_SetU8g(u8glib.getU8g(), m2_u8g_box_icon);


// init code
u8glib.sleepOff();
u8glib.setDefaultForegroundColor();

// USER CODE BEGIN 5
uint8_t buf_ptr=0;
 int8_t buf[128]={};
//char str2[25]={};

uint8_t tmp;

// Infinite loop
/*for(;;)
{
{
//Radio button
M2_LABEL(el_label0, NULL, "Black and white");
M2_RADIO(el_radio0, "v3", &select_color);

M2_LABEL(el_label1, NULL, "Red");
M2_RADIO(el_radio1, "v4", &select_color);

M2_LABEL(el_label2, NULL, "Green");
M2_RADIO(el_radio2, "v1", &select_color);

M2_LABEL(el_label3, NULL, "Blue");
M2_RADIO(el_radio3, "v2", &select_color);

M2_BUTTON(el_cancel, NULL, "Cancel", fn_cancel);
M2_BUTTON(el_ok, NULL, " Ok ", fn_ok);

M2_LIST(list) = {
	&el_label0, &el_radio0,
	&el_label1, &el_radio1,
	&el_label2, &el_radio2,  
	&el_label3, &el_radio3, 
	&el_cancel, &el_ok 
};
M2_GRIDLIST(list_element, "c2", list);


M2tk m2(&list_element, m2_es_arm_u8g, m2_eh_6bs, m2_gh_u8g_ffs);

// Assign u8g font to index 0
m2.setFont(0, u8g_font_6x13);

// Setup keys
m2.setPin(M2_KEY_SELECT, KEY_ENT);
m2.setPin(M2_KEY_EXIT, KEY_ESC);
m2.setPin(M2_KEY_DATA_UP, KEY_Up);
m2.setPin(M2_KEY_DATA_DOWN, KEY_Down);
m2.setPin(M2_KEY_NEXT, KEY_Right);
m2.setPin(M2_KEY_PREV, KEY_Left);

while (select_color==0)
	{
	m2.checkKey();
	if ( m2.handleKey() )
		{
		u8glib.firstPage();  
		do{
			m2.draw();
			} while( u8glib.nextPage() );
		}
	}
}

switch (select_color)
	{
	case 1:
		{
		#define TEXT_LEN 12
		char text[] = "abcdefghijkl";

		M2_LABEL(el_label, "", "Enter Text:");
		M2_TEXT(el_text, NULL, text, TEXT_LEN);
		M2_BUTTON(el_ok, NULL, " Ok ", fn_ok);
		M2_LIST(list2) = { &el_label, &el_text, &el_ok };
		M2_VLIST(list_element2, NULL, list2);

		M2_LABEL(hello_world_label, NULL, "Green color!");
		M2tk m22(&hello_world_label, NULL, NULL, m2_gh_u8g_bfs);
		u8glib.firstPage();  
		do{
			m22.draw();
			} while( u8glib.nextPage() );
		osDelay(3000);
		}
	}
select_color=0;
}*/

while(1)
{
//Board.WATCHDOG_Refresh();
xQueueReceive( queue_radio_rssi_current, &( tmp ), 10 );
buf[(buf_ptr<128?buf_ptr++:buf_ptr=0)]=tmp;//CC110l_GetRSSI();


char str[20]={"%%%ii"};
uint8_t units_num;
{
TSwmbData wmb_data[8];

wmbGetDataStruct(counters[0].data, wmb_data, counters[0].data_len);

units_num=wmb_data[0].vif>>3;

char str_tmp[4];
parser.BufPrintf(str_tmp, str, units_exponent_default[units_num]-wmb_data[0].vif&0x07);
parser.BufPrintf(str, str_tmp, wmb_data[0].data);
}

/*str2[0]=0;
for (uint8_t i=0; i<8; i++)
	{
	char tmp[4]={};
	parser.BufPrintf(tmp, " %1x", radio_rx_buf[i]);
	strcat (str2, tmp);
	}*/

char str_adr[32];
parser.BufPrintf(str_adr, "%c%c%c  # %8h  ver.%2h",
			  MAN_ID_TO_STR(counters[0].wmbus.manid),
			  counters[0].wmbus.addr,
			  counters[0].wmbus.ver);

uint8_t counter_type_num=0;
for (uint8_t i=0; i<sizeof(type); i++)
	{
	if (type[i]==counters[0].wmbus.type)
		{
		counter_type_num=i;
		break;
		}
	}


u8glib.firstPage();
DisplayMainTop();
do
	{
	u8glib.setFont(my_small_font_Cyr6);
	u8glib.drawStr(0, 13, name_str[counter_type_num]);
	u8glib.setFont(u8g_font_5x7);
	u8glib.drawStr(0, 21, str_adr);
	
	u8glib.setFont(a_LCDNova_digit21);
	u8glib.drawStr(0, 40, str);
	uint8_t strl=u8glib.getStrWidth(str);
	u8glib.setFont(my_small_font_Cyr6);
	u8glib.drawStr(strl+3, 35, units_name[units_num]);
	for (uint8_t i=0; i<128; i++) u8glib.drawPixel(i, (int8_t)(64-(int8_t)(buf[i]+120)));
	

	
	} while ( u8glib.nextPage() );

}
/* USER CODE END 5 */ 
}


// 11111110
// 10000011
// 10000001
// 10000011
// 11111110
#define BAT_X	120
#define BAT_Y	0
const uint8_t bat_bitmap[] U8G_PROGMEM = { 0xFE, 0x83, 0x81, 0x83, 0xFe};
void DrawBattery (uint16_t val)
{
u8glib.drawBitmapP( BAT_X, BAT_Y, 1, 5, bat_bitmap);
if (val>3600) val=(val-3600)/100; else val=0;
for (uint8_t i=0; i<val; i++) u8glib.drawVLine(BAT_X+1+i, BAT_Y+1, 3);
}



// 01100111
// 01010100
// 01110110
// 01100100
// 01010100
#define RFANT_X	101
#define RFANT_Y	0
const uint8_t r_bitmap[] U8G_PROGMEM = { 0x67, 0x54, 0x76, 0x64, 0x54};
void DrawRFAnt (int8_t val)
{
u8glib.drawBitmapP( RFANT_X, RFANT_Y, 1, 5, r_bitmap);
if (val>-55) val=-55;
if (val<-95) val=-95;
val = 5 + ((val+55)/8);
for (uint8_t i=0; i<val; i++) u8glib.drawVLine(RFANT_X+8+i*2, RFANT_Y-i+4, i+1);
}



// 01000100
// 01010100
// 01010100
// 01010100
// 00101000
#define WIFIANT_X	83
#define WIFIANT_Y	0
const uint8_t w_bitmap[] U8G_PROGMEM = { 0x44, 0x54, 0x54, 0x54, 0x28};
void DrawWiFiAnt (uint8_t val)
{
u8glib.drawBitmapP( WIFIANT_X, WIFIANT_Y, 1, 5, w_bitmap);
for (uint8_t i=0; i<val; i++) u8glib.drawVLine(WIFIANT_X+8+i*2, WIFIANT_Y-i+4, i+1);
}



// 01000100
// 01010100
// 01010100
// 01010100
// 00101000
#define GSMANT_X	83
#define GSMANT_Y	0
const uint8_t g_bitmap[] U8G_PROGMEM = { 0x44, 0x54, 0x54, 0x54, 0x28};
void DrawGSMAnt (uint8_t val)
{
u8glib.drawBitmapP( GSMANT_X, GSMANT_Y, 1, 5, g_bitmap);
for (uint8_t i=0; i<val; i++) u8glib.drawVLine(GSMANT_X+8+i*2, GSMANT_Y-i+4, i+1);
}



void DisplayMainTop (void)
{
char timestr[6]={};
RTC_TimeTypeDef sTime;
HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN);
parser.BufPrintf(timestr, "%i:%2L", sTime.Hours, sTime.Minutes);

//do
	{
	/*u8glib.setColorIndex(1);
	u8glib.drawBox(0, 0, 128, 6);
	
	u8glib.setColorIndex(0);*/
	
	const uint8_t bat_bitmap[] U8G_PROGMEM = { 0x7E, 0x00, 0x7E, 0x00, 0x7E};//Menu icon
	u8glib.drawBitmapP( 0, 0, 1, 5, bat_bitmap);
	
	u8glib.setFont(u8g_font_04b_03bn);
	u8glib.drawStr(54, 5, timestr);
	
	DrawBattery(bat_voltage);
	DrawRFAnt(radio_rssi);
	DrawWiFiAnt(5);
	u8glib.drawHLine(0, 6, 128);
	
	//u8glib.setColorIndex(1);
	} //while ( u8glib.nextPage() );
}


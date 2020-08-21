
#ifndef BOARD_H
#define BOARD_H

extern RTC_HandleTypeDef hrtc;
//extern ADC_HandleTypeDef hadc1;

class CBoard
	{
	private:
	static void SystemClock_Config(void);
	static void MX_GPIO_Init(void);
	static void MX_ADC1_Init(void);
	static void MX_RTC_Init(void);
	static void MX_SPI1_Init(void);
	static void MX_SPI2_Init(void);
	static void MX_TIM3_Init(void);
	//static void MX_USART2_UART_Init(void);
	
	
	public:
	CBoard(void);
	void PowerOn(bool state);
	int  GetADCValue(uint32_t Channel);
	bool IsCharge(void);
	bool IsExtPower(void);
	uint16_t GetBatVoltage(void);
	void WATCHDOG_Init (void);
	void WATCHDOG_Start(void);
	void WATCHDOG_Refresh(void);
	};

#endif
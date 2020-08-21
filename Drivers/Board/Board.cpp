

#include "stm32f1xx_hal.h"

#include "Board.h"


ADC_HandleTypeDef hadc1;
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
//SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim3;
//UART_HandleTypeDef huart2;
WWDG_HandleTypeDef Watchdog;



CBoard::CBoard(void)
{
HAL_Init();
SystemClock_Config();
MX_GPIO_Init();
MX_ADC1_Init();
MX_RTC_Init();
MX_SPI1_Init();
//MX_SPI2_Init();//CC110L
//MX_TIM3_Init();
//MX_USART2_UART_Init();
}




/** System Clock Configuration
*/
void CBoard::SystemClock_Config(void)
{
RCC_OscInitTypeDef RCC_OscInitStruct;
RCC_ClkInitTypeDef RCC_ClkInitStruct;
RCC_PeriphCLKInitTypeDef PeriphClkInit;

RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_HSE;
RCC_OscInitStruct.HSEState = RCC_HSE_ON;
RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
RCC_OscInitStruct.LSEState = RCC_LSE_ON;
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
HAL_RCC_OscConfig(&RCC_OscInitStruct);

RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC
					|RCC_PERIPHCLK_USB;
PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
PeriphClkInit.UsbClockSelection = RCC_USBPLLCLK_DIV1_5;
HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

/* SysTick_IRQn interrupt configuration */
HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



void CBoard::MX_GPIO_Init(void)
{
GPIO_InitTypeDef GPIO_InitStruct;

/* GPIO Ports Clock Enable */
__GPIOC_CLK_ENABLE();
__GPIOD_CLK_ENABLE();
__GPIOA_CLK_ENABLE();
__GPIOB_CLK_ENABLE();

/*Configure GPIO pins : WAN_IRQ_Pin BOOTLOADER_RX_Pin */
GPIO_InitStruct.Pin = WAN_IRQ_Pin|BOOTLOADER_RX_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
HAL_GPIO_WritePin(GPIOA, USB_ON_Pin, GPIO_PIN_RESET);

/*Configure GPIO pins : EXT_POWER_Pin _CHARGE_STATUS_Pin KEY_ROW1_Pin
				 KEY_ROW2_Pin KEY_ROW3_Pin WAN_DCD_Pin */
GPIO_InitStruct.Pin = EXT_POWER_Pin|_CHARGE_STATUS_Pin| KEY_ROW1_Pin|
			   KEY_ROW2_Pin|KEY_ROW3_Pin|WAN_DCD_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/*Configure GPIO pin : _POWERDOWN_BAT_Pin */
GPIO_InitStruct.Pin = _POWERDOWN_BAT_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
HAL_GPIO_Init(_POWERDOWN_BAT_GPIO_Port, &GPIO_InitStruct);
HAL_GPIO_WritePin(_POWERDOWN_BAT_GPIO_Port, _POWERDOWN_BAT_Pin, GPIO_PIN_SET);

/*Configure GPIO pins : KEY_COL1_Pin KEY_COL2_Pin */
GPIO_InitStruct.Pin = KEY_COL1_Pin|KEY_COL2_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/*Configure GPIO pin : EEPROM_CS_Pin */
GPIO_InitStruct.Pin = EPROM_CS_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
HAL_GPIO_Init(EPROM_CS_GPIO_Port, &GPIO_InitStruct);

/*Configure GPIO pin : USB_ON_Pin */
GPIO_InitStruct.Pin = USB_ON_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
HAL_GPIO_Init(USB_ON_GPIO_Port, &GPIO_InitStruct);

/*Configure GPIO pin : WAN_PWKEY_Pin */
GPIO_InitStruct.Pin = WAN_PWKEY_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
HAL_GPIO_Init(WAN_PWKEY_GPIO_Port, &GPIO_InitStruct);

/*Configure GPIO pin : WAN_DTR_Pin */
GPIO_InitStruct.Pin = WAN_DTR_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(WAN_DTR_GPIO_Port, &GPIO_InitStruct);
}



/* ADC1 init function */
void CBoard::MX_ADC1_Init(void)
{
ADC_ChannelConfTypeDef sConfig;

/**Common config 
*/
hadc1.Instance = ADC1;
hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
hadc1.Init.ContinuousConvMode = DISABLE;
hadc1.Init.DiscontinuousConvMode = DISABLE;
hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
hadc1.Init.NbrOfConversion = 1;
HAL_ADC_Init(&hadc1);

/**Configure Regular Channel 
*/
sConfig.Channel = ADC_CHANNEL_8;
sConfig.Rank = 1;
sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}



/* RTC init function */
void CBoard::MX_RTC_Init(void)
{
/**Initialize RTC and set the Time and Date 
*/
hrtc.Instance = RTC;
hrtc.Init.AsynchPrediv = 32767-10;
hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
HAL_RTC_Init(&hrtc);


/*RTC_TimeTypeDef sTime;
RTC_DateTypeDef DateToUpdate;

sTime.Hours = 10;
sTime.Minutes = 33;
sTime.Seconds = 0;

HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);

DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
DateToUpdate.Month = RTC_MONTH_MARCH;
DateToUpdate.Date = 4;
DateToUpdate.Year = 16;

HAL_RTC_SetDate(&hrtc, &DateToUpdate, FORMAT_BIN);*/
}



/* SPI1 init function */
void CBoard::MX_SPI1_Init(void)
{
hspi1.Instance = SPI1;
hspi1.Init.Mode = SPI_MODE_MASTER;
hspi1.Init.Direction = SPI_DIRECTION_2LINES;
hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
hspi1.Init.NSS = SPI_NSS_SOFT;
hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
hspi1.Init.CRCPolynomial = 10;
HAL_SPI_Init(&hspi1);
}



/* SPI2 init function */
void CBoard::MX_SPI2_Init(void)
{
/*hspi2.Instance = SPI2;
hspi2.Init.Mode = SPI_MODE_MASTER;
hspi2.Init.Direction = SPI_DIRECTION_2LINES;
hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
hspi2.Init.NSS = SPI_NSS_SOFT;
hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
hspi2.Init.TIMode = SPI_TIMODE_DISABLED;
hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
hspi2.Init.CRCPolynomial = 10;
HAL_SPI_Init(&hspi2);*/
}



/* TIM3 init function */
void CBoard::MX_TIM3_Init(void)
{
TIM_ClockConfigTypeDef sClockSourceConfig;
TIM_MasterConfigTypeDef sMasterConfig;
TIM_OC_InitTypeDef sConfigOC;

htim3.Instance = TIM3;
htim3.Init.Prescaler = 0;
htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
htim3.Init.Period = 0;
htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
HAL_TIM_Base_Init(&htim3);

sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

HAL_TIM_PWM_Init(&htim3);

sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

sConfigOC.OCMode = TIM_OCMODE_PWM1;
sConfigOC.Pulse = 0;
sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
}



/* USART2 init function */
/*void CBoard::MX_USART2_UART_Init(void)
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
}*/



/* PowerDown_Bat */
void CBoard::PowerOn(bool state)
{
HAL_GPIO_WritePin(_POWERDOWN_BAT_GPIO_Port, _POWERDOWN_BAT_Pin, state?GPIO_PIN_SET:GPIO_PIN_RESET);
}



/* Battery charge status */
bool CBoard::IsCharge(void)
{
return HAL_GPIO_ReadPin(_CHARGE_STATUS_GPIO_Port, _CHARGE_STATUS_Pin)?false:true;
}



int CBoard::GetADCValue(uint32_t Channel)
{
ADC_ChannelConfTypeDef   sConfig;
sConfig.Channel=Channel;
sConfig.Rank=1;
sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
HAL_ADC_ConfigChannel(&hadc1,&sConfig);

HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1,1);

return HAL_ADC_GetValue(&hadc1);
}



/*******************************************************************************
* External power status
*******************************************************************************/
bool CBoard::IsExtPower(void)
{
GetADCValue(ADC_CHANNEL_9);
uint16_t adcResult = (uint32_t)((uint32_t)(HAL_ADC_GetValue(&hadc1)*2740)/1000);
HAL_ADC_Stop(&hadc1);
return (adcResult<3000)?false:true;
}



/*******************************************************************************
* Battery charge status 
*******************************************************************************/
uint16_t CBoard::GetBatVoltage(void)
{
GetADCValue(ADC_CHANNEL_8);
uint16_t adcResult = (uint32_t)((uint32_t)(HAL_ADC_GetValue(&hadc1)*2740)/1000);
HAL_ADC_Stop(&hadc1);
return adcResult;
}



void CBoard::WATCHDOG_Init (void)
{
//HAL_WWDG_DeInit(&Watchdog);
__WWDG_CLK_ENABLE();			//Enable WAtch dog timer clock
__HAL_FREEZE_WWDG_DBGMCU();		//Stop WWDG during Debugging

Watchdog.Instance=WWDG;

//WWDG clock counter = (PCLK1 (36MHz)/4096)/8) = 1098 Hz (~910 us) PCLK1 = AHB clock/2=72Mhz/2=36MHz

Watchdog.Init.Counter = 127;
Watchdog.Init.Prescaler = WWDG_PRESCALER_8;
Watchdog.Init.Window = 80;

HAL_WWDG_Init(&Watchdog);		//Intialize the Watchdog with configuration
}



void CBoard::WATCHDOG_Start(void)	//Starts the Watchdog timer with initial counter value to downcount
{
HAL_WWDG_Start(&Watchdog);
}



void CBoard::WATCHDOG_Refresh(void)//Refreshes the Watchdog timer counter for every to prevent the counter being underflowing
{
HAL_WWDG_Refresh(&Watchdog, 127);	//reload the watchdog timer with initial count value
}
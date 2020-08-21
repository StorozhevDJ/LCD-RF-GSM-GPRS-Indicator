

#include "stm32f1xx_hal.h"

#include "SIM800driver.h"

UART_HandleTypeDef huart1;

void SIM800Driver_HWInit()
{
__GPIOA_CLK_ENABLE();
__GPIOD_CLK_ENABLE();

GPIO_InitTypeDef GPIO_InitStruct;

/*Configure GPIO pin : WAN_DTR_Pin */
GPIO_InitStruct.Pin = WAN_DTR_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(WAN_DTR_GPIO_Port, &GPIO_InitStruct);

/*Configure GPIO pin : WAN_PWKEY_Pin */
GPIO_InitStruct.Pin = WAN_PWKEY_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
HAL_GPIO_Init(WAN_PWKEY_GPIO_Port, &GPIO_InitStruct);


};
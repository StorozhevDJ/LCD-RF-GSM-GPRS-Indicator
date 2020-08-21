/*
  
  u8g_arm.c
  

  u8g utility procedures for LPC11xx

  Universal 8bit Graphics Library
  
  Copyright (c) 2013, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

 
  The following delay procedures must be implemented for u8glib. This is done in this file:

  void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
  void u8g_MicroDelay(void)		Delay be one microsecond
  void u8g_10MicroDelay(void)	Delay by 10 microseconds
  
  Additional requirements:
  
      SysTick must be enabled, but SysTick IRQ is not required. Any LOAD values are fine,
      it is prefered to have at least 1ms
      Example:
        SysTick->LOAD = (SystemCoreClock/1000UL*(unsigned long)SYS_TICK_PERIOD_IN_MS) - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = 7;   // enable, generate interrupt (SysTick_Handler), do not divide by 2
*/

#include "u8g_arm.h"
#include "stm32f1xx_hal.h"

#include "Drivers/Keyboard/keyboard.h"

CKeyBoard KeyBoard;


/*========================================================================*/
/* system clock setup for LPC11xx*/

//uint32_t SystemCoreClock = 72000000;

/* activate PLL for the int. RC osc. Assumes the IRC is already running */
/* this procedure is not required for u8glib, but can be called from the init code */
void init_system_clock(void)
{

}

/*========================================================================*/
/* Generic ARM delay procedure, based on the system timer (SysTick) */

/*
  Delay by the provided number of system ticks.
  The delay must be smaller than the RELOAD value.
  This delay has an imprecision of about +/- 20 system ticks.   
*/
static void _delay_system_ticks_sub(uint32_t sys_ticks)
{
  uint32_t start_val, end_val, curr_val;
  uint32_t load;
  
  start_val = SysTick->VAL;
  start_val &= 0x0ffffffUL;
  end_val = start_val;
  
  if ( end_val < sys_ticks )
  {
    // check, if the operation after this if clause would lead to a negative result
    // if this would be the case, then add the reload value first
    load = SysTick->LOAD;
    load &= 0x0ffffffUL;
    end_val += load;
  }
  // counter goes towards zero, so end_val is below start value
  end_val -= sys_ticks;		
  
  
  // wait until interval is left
  if ( start_val >= end_val )
  {
    for(;;)
    {
      curr_val = SysTick->VAL;
      curr_val &= 0x0ffffffUL;
      if ( curr_val <= end_val )
	break;
      if ( curr_val > start_val )
	break;
    }
  }
  else
  {
    for(;;)
    {
      curr_val = SysTick->VAL;
      curr_val &= 0x0ffffffUL;
      if ( curr_val <= end_val && curr_val > start_val )
	break;
    }
  }
}

/*
  Delay by the provided number of system ticks.
  Any values between 0 and 0x0ffffffff are allowed.
*/
void delay_system_ticks(uint32_t sys_ticks)
{
  uint32_t load4;
  load4 = HAL_GetTick();//SysTick->LOAD;
  load4 &= 0x0ffffffUL;
  load4 >>= 2;
  
  while ( sys_ticks > load4 )
  {
    sys_ticks -= load4;
    _delay_system_ticks_sub(load4);
  }
  _delay_system_ticks_sub(sys_ticks);
}

/*
  Delay by the provided number of micro seconds.
  Limitation: "us" * System-Freq in MHz must now overflow in 32 bit.
  Values between 0 and 1.000.000 (1 second) are ok.
*/
void delay_micro_seconds(uint32_t us)
{
/*  uint32_t sys_ticks;

  sys_ticks = SystemCoreClock;
  sys_ticks /=1000000UL;
  sys_ticks *= us;
  delay_system_ticks(sys_ticks);*/
//for (uint32_t t=0; t<us; t++) __no_operation();
}

/*========================================================================*/
/* generic gpio procedures (not required for u8glib) */

struct _lpc_pin_info_struct
{
  uint16_t offset;
  uint16_t iocon_gpio_value_no_pullup;
  uint16_t iocon_gpio_value_pullup;		/* identical to iocon_gpio_value_no_pullup if there is no pullup */
};
typedef struct _lpc_pin_info_struct lpc_pin_info_struct;



//LPC_GPIO_TypeDef   *lpc11xx_gpio_base[4] = {LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3};


void set_gpio_mode(uint16_t pin, uint8_t is_output, uint8_t is_pullup)
{
  /*uint32_t value;
  LPC_GPIO_TypeDef   *gpio;

 LPC_SYSCON->SYSAHBCLKCTRL |= 1<<16;	// enable IOCON clock

   
  if ( is_pullup == 0 )
    value = lpc11xx_pin_info[pin].iocon_gpio_value_no_pullup;
  else
    value = lpc11xx_pin_info[pin].iocon_gpio_value_pullup;
  *(__IO uint32_t *)(((char *)LPC_IOCON)+(size_t)(lpc11xx_pin_info[pin].offset)) = value;
  
  gpio = lpc11xx_gpio_base[pin >> 4];
  
  if ( is_output == 0 )
    gpio->DIR &= ~( 1UL << (pin & 0x0f));
  else
    gpio->DIR |= ( 1UL << (pin & 0x0f));  */
}

void set_gpio_level(uint16_t pin, uint8_t level)
{
  /*LPC_GPIO_TypeDef  *gpio = lpc11xx_gpio_base[pin >> 4];
  pin &= 0x0f;
  if ( level == 0 )
  {
    gpio->MASKED_ACCESS[1<<(pin)] = 0;
    //gpio->DATA &= ~( 1UL << (pin));
  }
  else
  {
    gpio->MASKED_ACCESS[1<<(pin)] = ~0UL;
    //gpio->DATA |= ( 1UL << (pin));  
  }*/
}

uint8_t get_gpio_level(uint16_t pin)
{
  /*LPC_GPIO_TypeDef  *gpio = lpc11xx_gpio_base[pin >> 4];
  pin &= 0x0f;
  
  if ( (gpio->DATA & ( 1UL << (pin))) == 0 )
    return 0;*/
if (KeyBoard.GetKey()==pin) return 0;
else return 1;  
}



/*

*/
void LCD_HW_Init(void)
{
GPIO_InitTypeDef GPIO_InitStruct;

// GPIO Ports Clock Enable
__GPIOC_CLK_ENABLE();

//Configure GPIO pin : LCD__RES_Pin LCD_LED_Pin
GPIO_InitStruct.Pin = LCD__RES_Pin | LCD_LED_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/*Configure GPIO pins : LCD_D0_Pin LCD_D1_Pin LCD_D2_Pin LCD_D3_Pin 
				  LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin LCD_D7_Pin 
				  LCD__CS1_Pin LCD_A0_Pin LCD__WR_Pin LCD__RD_Pin */
GPIO_InitStruct.Pin = LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin 
				 |LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin 
				 |LCD__CS1_Pin|LCD_A0_Pin|LCD__WR_Pin|LCD__RD_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_SET);
LCD_WRITE_DATA;
}



void LCD_Out(uint8_t data)
{
LCD_STROB_L;
GPIOC->BRR =(data^0xFF);
GPIOC->BSRR= data;
__no_operation();
__no_operation();
LCD_STROB_H;
}



/*========================================================================*/
/*
  The following delay procedures must be implemented for u8glib

  void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
  void u8g_MicroDelay(void)		Delay be one microsecond
  void u8g_10MicroDelay(void)	Delay by 10 microseconds

*/
void u8g_Delay(uint16_t val)
{
HAL_Delay(val);
}

void u8g_MicroDelay(void)
{
  delay_micro_seconds(1);
}

void u8g_10MicroDelay(void)
{
  delay_micro_seconds(10);
}




/*========================================================================*/
/* u8glib com procedure */

/* eval board */
uint16_t u8g_pin_a0 = PIN(1,1);
uint16_t u8g_pin_cs = PIN(1,2);
uint16_t u8g_pin_rst = PIN(1,0);



uint8_t u8g_com_8bit_C86_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
switch (msg)
	{
	case U8G_COM_MSG_STOP:
	break;
    
	case U8G_COM_MSG_INIT:
		{
		LCD_HW_Init();
		u8g_MicroDelay();
		}
	break;
    
	case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
		u8g_10MicroDelay();
		//set_gpio_level(u8g_pin_a0, arg_val);
		arg_val?LCD_SET_DATA_MODE:LCD_SET_CMD_MODE;
		u8g_10MicroDelay();
	break;

	case U8G_COM_MSG_CHIP_SELECT:
		if ( arg_val == 0 )	// disable
			{
			// this delay is required to avoid that the display is switched off too early --> DOGS102 with LPC1114
			for(uint8_t i = 0; i < 5; i++ ) u8g_10MicroDelay();
			LCD_CS_OFF;
			}
		else				// enable
			{
			LCD_CS_ON;
			}
		u8g_MicroDelay();
	break;

	case U8G_COM_MSG_RESET:
		//set_gpio_level(u8g_pin_rst, arg_val);
		arg_val?LCD_RESET_OFF:LCD_RESET_ON;
		u8g_10MicroDelay();
	break;

	case U8G_COM_MSG_WRITE_BYTE:
		LCD_Out(arg_val);
		u8g_MicroDelay();
	break;

	//case U8G_COM_MSG_WRITE_SEQ:
	case U8G_COM_MSG_WRITE_SEQ:
		{
		register uint8_t *ptr = (uint8_t *)arg_ptr;
		while( arg_val > 0 )
			{
			LCD_Out(*ptr++);
			arg_val--;
			}
		}
	break;
	
	default: break;
	}
return 1;
}


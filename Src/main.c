/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include "Drivers/Board/Board.h"
#include "Drivers/USB/usb_device.h"
#include "Drivers/LCD/u8g_arm.h"
#include "CC110L/cc110L.h"
#include "PrintScan/Parser.h"

#include "Display\Display_task.hpp"
#include "Radio\Radio_task.hpp"
#include "GSMTask\GSMTask.hpp"
#include "SystemTask\SystemTask.h"




/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
CBoard Board;

osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/


U8GLIB u8glib;





/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
//void SystemClock_Config(void);


void Start_USBTask(void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */



void main(void)
{
  



  /*uint16_t x,y;
  x=1; y=1;
  uint8_t r=250;
  while(1)
	  {
	  //u8glib.setContrast(r);
  // picture loop
    u8glib.firstPage();
	do
	{
	u8glib.drawLine(0, 16, 128, 64);
	u8glib.drawEllipse(u8glib.getWidth()/2, u8glib.getHeight()/2, x, y, U8G_DRAW_ALL);
	u8glib.setFont(u8g_font_unifont);
  	u8glib.drawStr(0, 12, "Hello World!");
	} while ( u8glib.nextPage() );
  
	u8g_Delay(300);
	Ekey k=KEY_None;
	while (k==KEY_None) k=KeyBoard.GetKey();
	switch (k)
		{
		case KEY_Up: if (y<64) y++; break;
		case KEY_Down: if (y>1) y--; break;
		case KEY_Right: if (x<128) x++; break;
		case KEY_Left: if (x>1) x--; break;
		case KEY_ENT: if (r>1) r--; break;
		
		}
	if(k== KEY_ESC) break;
	  }*/
	/*for (uint8_t i=0; i<255; i++)
		{
		u8glib.setContrast(i);
		u8g_Delay(10);
		}*/
	//u8glib.sleepOn();
	//u8g_Delay(3000);
	//u8glib.setContrast(185);
	

	
	
	/*uint8_t x, y;
	 int8_t x_dir, y_dir;
	while (1)
		{
		if (x<=10) x_dir=1;
		if (x>=118) x_dir=-1;
		if (y<=10) y_dir=1;
		if (y>=54) y_dir=-1;
		x+=x_dir;
		y+=y_dir;
		u8glib.firstPage();
		do
			{
			u8glib.drawDisc(x, y, 10, U8G_DRAW_ALL);
			u8g_Delay(10);
			} while ( u8glib.nextPage() );
		}*/
	
	
  
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(DisplayTask, Display_task, osPriorityNormal, 0, 128*4);
  osThreadDef(RadioTask, Radio_task, osPriorityNormal, 0, 128*2);
  //osThreadDef(USBTask, Start_USBTask, osPriorityNormal, 0, 128);
  osThreadDef(GSMTask, GSM_task, osPriorityNormal, 0, 128*2);
  osThreadDef(SystemTask, SystemTask, osPriorityNormal, 0, 128);
  osThreadCreate(osThread(DisplayTask), NULL);
  osThreadCreate(osThread(RadioTask), NULL);
  //osThreadCreate(osThread(USBTask), NULL);
  osThreadCreate(osThread(GSMTask), NULL);
  osThreadCreate(osThread(SystemTask), NULL);
  defaultTaskHandle = osThreadCreate(osThread(SystemTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
while (1)
{
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
}
  /* USER CODE END 3 */

}






/* USER CODE BEGIN 4 */

/* USER CODE END 4 */
#include "Drivers/USB/usbd_cdc_if.h"
extern uint8_t radio_rx_buf[CC110l_DATA_LEN];
/* StartDefaultTask function */
void Start_USBTask(void const * argument)
{
/* init code for USB_DEVICE */
MX_USB_DEVICE_Init();

/* USER CODE BEGIN 5 */
/* Infinite loop */
for(;;)
	{
	osDelay(100);
	CDC_Transmit_FS((unsigned char *)"Test\r", 5);
	//uint8_t tmp[64];
	//xQueueReceive( queue_gsm_buf, &( tmp ), 100000 );
	//CDC_Transmit_FS(tmp, 64);
	}
/* USER CODE END 5 */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

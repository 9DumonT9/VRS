/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 6.0.0   2016-11-16

The MIT License (MIT)
Copyright (c) 2009-2016 Atollic AB

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************
*/

/* Includes */
#include "main.h"
#include <stddef.h>
#include <stdlib.h>
#include "stm32l1xx.h"

#include "gate/gate.h"
#include "display/display.h"
#include "display/ssd1306.h"
#include "ultrasonic/ultrasonic.h"

/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */
void delayApproxMs(int ms);

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

int main(void)
{
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//	GPIO_InitTypeDef gpioInitStruct;
//	gpioInitStruct.GPIO_OType = GPIO_OType_PP;
//	gpioInitStruct.GPIO_Speed = GPIO_Speed_400KHz;
//
//	gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
//	gpioInitStruct.GPIO_Pin = GPIO_Pin_10;
//	GPIO_Init(GPIOA, &gpioInitStruct);
//
//	GPIO_SetBits(GPIOA, GPIO_Pin_10);
//	while (1) {
//		GPIO_ResetBits(GPIOA, GPIO_Pin_10);
//		for (int i = 0; i < 10; i++);
//		GPIO_SetBits(GPIOA, GPIO_Pin_10);
//		for (int i = 0; i < 10; i++);
//	}

	RCC_ClocksTypeDef rccClocks;
	RCC_GetClocksFreq(&rccClocks);
	SysTick_Config(rccClocks.HCLK_Frequency/1000);

	timer = 0;
	initializeDisplay();
	initializeGate();
	initializeUltrasonicSensors();

	int placeIsFree[kUltrasonicSensorsCount] = {1, 1, 1, 1};

	while (1)
	{
		for (int i = 0; i < kUltrasonicSensorsCount; i++) {
			int isFree = !measureProximity(i);
			if (isFree != placeIsFree[i]) {
				placeIsFree[i] = isFree;
				displayPlaceFree(i, isFree);
			}
		}

		handleGate();
//		displayPlacesCount(freePlacesCount);

		//Display and servo demo
//		openGate();
//		delayApproxMs(1000);
//		closeGate();
//
//		int changedPlace = rand()%4;
//		freePlacesCount += placeIsFree[changedPlace] ? -1 : 1;
//		placeIsFree[changedPlace] = 1 - placeIsFree[changedPlace];
//		displayPlaceFree(changedPlace, placeIsFree[changedPlace]);
//		displayPlacesCount(freePlacesCount);
//
//		delayApproxMs(3000);
	}
	return 0;
}

void delayApproxMs(int ms) {
	for (int i = 0; i < ms*1000; i++);
}

void incTimer() {
	timer++;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}

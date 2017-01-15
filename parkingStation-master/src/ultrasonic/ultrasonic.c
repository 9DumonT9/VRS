/*
 * ultrasonic.c
 *
 *  Created on: 14. 1. 2017
 *      Author: Matej Vargovcik
 */

#include "ultrasonic.h"
#include "main.h"
#include <stddef.h>
#include "stm32l1xx.h"
#include "../display/display.h"

typedef struct UltrasonicSensor {
	GPIO_TypeDef *gpioOut;
	GPIO_TypeDef *gpioIn;
	GPIO_TypeDef *gpioLed;
	uint16_t pinOut;
	uint16_t pinIn;
	uint16_t pinLed;
	int placeOccupied;
	int senseCount;
	int lastProximity;
	int placeFreeingStart;
} UltrasonicSensor;

UltrasonicSensor ultrasonicSensor(GPIO_TypeDef *gpioOut, uint16_t pinOut, GPIO_TypeDef *gpioIn,
		uint16_t pinIn, GPIO_TypeDef *gpioLed, uint16_t pinLed) {
	UltrasonicSensor result;
	result.gpioOut = gpioOut;
	result.gpioIn = gpioIn;
	result.gpioLed = gpioLed;
	result.pinOut = pinOut;
	result.pinIn = pinIn;
	result.pinLed = pinLed;
	result.placeOccupied = 0;
	result.senseCount = 0;
	result.lastProximity = 0;
	return result;
}

UltrasonicSensor ultrasonicSensors[kUltrasonicSensorsCount];

void initializeUltrasonicSensors() {
	ultrasonicSensors[0] = ultrasonicSensor(GPIOA, GPIO_Pin_10, GPIOB, GPIO_Pin_3, GPIOB, GPIO_Pin_5);
	ultrasonicSensors[1] = ultrasonicSensor(GPIOB, GPIO_Pin_13, GPIOB, GPIO_Pin_14, GPIOB, GPIO_Pin_15);
	ultrasonicSensors[2] = ultrasonicSensor(GPIOB, GPIO_Pin_1, GPIOB, GPIO_Pin_2, GPIOB, GPIO_Pin_11);
	ultrasonicSensors[3] = ultrasonicSensor(GPIOA, GPIO_Pin_4, GPIOA, GPIO_Pin_1, GPIOA, GPIO_Pin_0);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	for (int i = 0; i < kUltrasonicSensorsCount; i++) {
		//trigger_sensor
		GPIO_InitTypeDef gpioInitStruct;
		gpioInitStruct.GPIO_OType = GPIO_OType_PP;
		gpioInitStruct.GPIO_Speed = GPIO_Speed_400KHz;

		gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
		gpioInitStruct.GPIO_Pin = ultrasonicSensors[i].pinOut;
		GPIO_Init(ultrasonicSensors[i].gpioOut, &gpioInitStruct);

		gpioInitStruct.GPIO_Mode = GPIO_Mode_IN;
		gpioInitStruct.GPIO_Pin = ultrasonicSensors[i].pinIn;
		GPIO_Init(ultrasonicSensors[i].gpioIn, &gpioInitStruct);

		gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
		gpioInitStruct.GPIO_Pin = ultrasonicSensors[i].pinLed;
		GPIO_Init(ultrasonicSensors[i].gpioLed, &gpioInitStruct);//
	}
}

int measureProximity(int sensor) {
//	  ultrasonicSensors[sensor].senseCount++;
//	  displayPlacesCount(5);

	  GPIO_TypeDef *gpioOut = ultrasonicSensors[sensor].gpioOut;
	  uint16_t pinOut = ultrasonicSensors[sensor].pinOut;
	  GPIO_TypeDef *gpioIn = ultrasonicSensors[sensor].gpioIn;
	  uint16_t pinIn = ultrasonicSensors[sensor].pinIn;

	  int time0 = 0;
	  int time1 = 0;
	  GPIO_SetBits(gpioOut, pinOut);
	  for(int i=0;i<10;i++);
	  GPIO_ResetBits(gpioOut, pinOut);
	  //wait for pulse on echo pin
	  while(GPIO_ReadInputDataBit(gpioIn, pinIn)==0 && time0 < 500)
		  time0++;
	  //measure pulse width
	  while(GPIO_ReadInputDataBit(gpioIn, pinIn)==1) time1++;
	  if (time0 == 500)
		  return ultrasonicSensors[sensor].placeOccupied;

	  float distance=time1/20.0;

//	  if (sensor == 0) {
//	  	  displayPlacesCount(distance);
//	  }

	  if (distance < 7.0) {
		  ultrasonicSensors[sensor].placeOccupied = 1;
		  GPIO_SetBits(ultrasonicSensors[sensor].gpioLed, ultrasonicSensors[sensor].pinLed);
	  }
	  else if (ultrasonicSensors[sensor].placeOccupied) {
		  if (ultrasonicSensors[sensor].lastProximity == 1)
			  ultrasonicSensors[sensor].placeFreeingStart = timer;

		  if (timer - ultrasonicSensors[sensor].placeFreeingStart > 500) {
			  ultrasonicSensors[sensor].placeOccupied = 0;
			  GPIO_ResetBits(ultrasonicSensors[sensor].gpioLed, ultrasonicSensors[sensor].pinLed);
		  }
	  }
	  ultrasonicSensors[sensor].lastProximity = distance < 7.0;

	  return ultrasonicSensors[sensor].placeOccupied;
}

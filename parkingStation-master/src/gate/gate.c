/*
 * gate.c
 *
 *  Created on: 13. 12. 2016
 *      Author: Matej Vargovcik
 */

#include "gate.h"
#include "servo.h"
#include "main.h"
#include "../display/display.h"
#include "./ultrasonic/ultrasonic.h"
#include "stm32l1xx.h"

int gateButtonLastPressed = 0;
int gateButtonLastClickTime = -1;
int gateOpenTime = -1;
int freePlacesCount = 4;

void setGateAngle(int degrees);
void initializeGateButton();

void initializeGate() {
	initializeServo();
	initializeGateButton();
	closeGate();
}

void openGate() {
	setGateAngle(70);
}

void closeGate() {
	setGateAngle(0);
}

void setGateAngle(int degrees) {
	setServoSignalLength(degrees*1000/180 + 400);
}

void initializeGateButton() {
	GPIO_InitTypeDef gpioInitStruct;
	gpioInitStruct.GPIO_OType = GPIO_OType_PP;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_400KHz;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN;
	gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &gpioInitStruct);
}

void handleGate() {
	if (gateOpenTime == -1) {
		int pressed = (GPIOC->IDR & (uint16_t)(0b01<<6)) != 0;
		int clicked = pressed == 0 && gateButtonLastPressed == 1;
		gateButtonLastPressed = pressed;
		if (clicked) {
			if (gateButtonLastClickTime == -1) {
				gateButtonLastClickTime = timer;
				if (gateButtonLastClickTime == -1)
					gateButtonLastClickTime = -2;
			}
			else {
				if (freePlacesCount < kUltrasonicSensorsCount) {
					openGate();
					gateOpenTime = timer;
					if (gateOpenTime == -1)
						gateOpenTime = -2;
					freePlacesCount++;
					displayPlacesCount(freePlacesCount);
				}
				gateButtonLastClickTime = -1;
			}
		}
		else if (gateButtonLastClickTime != -1 && timer - gateButtonLastClickTime > 500) {
			if (freePlacesCount > 0) {
				openGate();
				gateOpenTime = timer;
				if (gateOpenTime == -1)
					gateOpenTime = -2;
				freePlacesCount--;
				displayPlacesCount(freePlacesCount);
			}
			gateButtonLastClickTime = -1;
		}
	}
	else {
		if (gateOpenTime != -1 && timer - gateOpenTime > 1500) {
			closeGate();
			gateOpenTime = -1;
		}
	}
}

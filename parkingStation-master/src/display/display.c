/*
 * display.c
 *
 *  Created on: 13. 12. 2016
 *      Author: Matej Vargovcik
 */
#include "display.h"
#include <stdlib.h>
#include "spi.h"
#include "ssd1306.h"
#include "ili9163.h"
#include "../ultrasonic/ultrasonic.h"

const int placesCount = kUltrasonicSensorsCount;
const int placeWidth = 16, placeHeight = 25;
const int placesX[] = {7, 37, 67, 97};
const int placesY[] = {25, 25, 25, 25};
const int placesOrientation[] = {0, 0, 0, 0};

const uint8_t car[] =
	   {0x1F, 0xF8,
		0x33, 0xCC,
		0x27, 0xE4,
		0x3F, 0xFC,
		0x3F, 0xFC,
		0x3F, 0xFC,
		0x3F, 0xFC,
		0x38, 0x1C,
		0x30, 0x0C,
		0x38, 0x1C,
		0x28, 0x14,
		0x2C, 0x34,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x3F, 0xFC,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x3F, 0xFC,
		0x30, 0x0C,
		0x28, 0x14,
		0x37, 0xEC,
		0x1F, 0xF8
	    };
const uint8_t carUpsideDown[] =
	   {0x1F, 0xF8,
		0x37, 0xEC,
		0x28, 0x14,
		0x30, 0x0C,
		0x3F, 0xFC,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x3F, 0xFC,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x2F, 0xF4,
		0x2C, 0x34,
		0x28, 0x14,
		0x38, 0x1C,
		0x30, 0x0C,
		0x38, 0x1C,
		0x3F, 0xFC,
		0x3F, 0xFC,
		0x3F, 0xFC,
		0x3F, 0xFC,
		0x27, 0xE4,
		0x33, 0xCC,
		0x1F, 0xF8,
	    };

void initializeDisplay() {
	initSPI2();
	initCD_Pin();
	initCS_Pin();
	initRES_Pin();

	lcdInitialise(LCD_ORIENTATION2);
	lcdClearDisplay(decodeRgbValue(0, 0, 0));

	lcdPutS("Free places count: ", lcdTextX(0), lcdTextY(0),
		  decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));

	lcdFilledRectangle(10, 70, 110, 80, decodeRgbValue(16, 16, 16));
	lcdFilledRectangle(10, 55, 20, 70, decodeRgbValue(16, 16, 16));
	lcdFilledRectangle(40, 55, 50, 70, decodeRgbValue(16, 16, 16));
	lcdFilledRectangle(70, 55, 80, 70, decodeRgbValue(16, 16, 16));
	lcdFilledRectangle(100, 55, 110, 70, decodeRgbValue(16, 16, 16));
	lcdFilledRectangle(10, 80, 20, 90, decodeRgbValue(16, 16, 16));

	for (int i = 0; i < placesCount; i++) {
		displayPlaceFree(i, 1);
	}
	displayPlacesCount(4);
}

void displayPlaceFree(int place, int free) {
	if (free) {
		lcdFilledRectangle(placesX[place], placesY[place],
				placesX[place]+placeWidth, placesY[place]+placeHeight, decodeRgbValue(0, 31, 0));
	}
	else {
		lcdDrawPictogram(placesOrientation[place] ? car : carUpsideDown,
				placesX[place], placesY[place], placeWidth, placeHeight,
				decodeRgbValue(31, 0, 0), decodeRgbValue(0, 0, 0));
	}
}

void displayPlacesCount(int count) {
	char s[5];
	lcdPutS(itoa(count, s, 10), lcdTextX(19), lcdTextY(0),
			  decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
}

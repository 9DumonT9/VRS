/*
 * ultrasonic.h
 *
 *  Created on: 14. 1. 2017
 *      Author: Matej Vargovcik
 */

#ifndef ULTRASONIC_ULTRASONIC_H_
#define ULTRASONIC_ULTRASONIC_H_

#define kUltrasonicSensorsCount 4
void initializeUltrasonicSensors();
int measureProximity(int sensor);

#endif /* ULTRASONIC_ULTRASONIC_H_ */

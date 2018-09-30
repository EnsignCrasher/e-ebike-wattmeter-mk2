/*
 Name:		e_bike_wattmeter.ino
 Created:	20.08.2018 20:59:11
 Author:	Administrator
*/

#define _simulation




#include "lib/TaskScheduler.h"
#include "variables.h"
#include "helperFunctions.h"
#include "debug.h"
#include "inputs.h"


void t1Callback();
void hallSensorTriggered();
void readVoltage();
void readCurrent();

Task t1(2000, 10, &t1Callback);

#define wheelCycleTime 267 //ca. 28kmh

Task simulateHallSensor(wheelCycleTime, TASK_FOREVER, hallSensorTriggered);
Task t_readVoltage(1000, TASK_FOREVER, readVoltage);
Task t_readCurrent(1000, TASK_FOREVER, readCurrent);

Scheduler runner;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	Serial.println("Scheduler TEST");

	initVariables();

	runner.init();
	Serial.println("Initialized scheduler");

	runner.addTask(t1);
	runner.addTask(t_readCurrent);
	runner.addTask(t_readVoltage);
#ifdef _simulation
	runner.addTask(simulateHallSensor);
	simulateHallSensor.enable();
#endif
	t1.enable();
	t_readCurrent.enable();
	t_readVoltage.enable();

	Serial.println("added tasks");

	delay(500);
}

// the loop function runs over and over again until power down or reset
void loop() {
	runner.execute();
	
	//reset speed if we don't move 
	if (millis() > lastDistanceMS.getTimestamp() + calcSpeedTimeout()) {
		actSpeed = 0;
	}

	delay(10); //should be fast enough
}

void t1Callback() {
	//do sth
}





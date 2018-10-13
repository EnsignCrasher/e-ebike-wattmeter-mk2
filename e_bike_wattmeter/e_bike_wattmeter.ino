/*
 Name:		e_bike_wattmeter.ino
 Created:	20.08.2018 20:59:11
 Author:	Administrator
*/
#pragma once
#define _simulation



#include "Arduino.h"
#include "debug.h"
#include "lib/TaskScheduler.h"
#include "variables.h"
#include "helperFunctions.h"
#include "inputs.h"
#include "20x4_lcd_display.h"






void t1Callback();
void hallSensorTriggered();
void readVoltage();
void readCurrent();
void initPages();
void initLcdValues();

Task t1(2000, 10, &t1Callback);

#define wheelCycleTime 267 //ca. 28kmh

Task simulateHallSensor(wheelCycleTime, TASK_FOREVER, hallSensorTriggered);
Task t_readVoltage(1000, TASK_FOREVER, readVoltage);
Task t_readCurrent(1000, TASK_FOREVER, readCurrent);
Task t_updateLCD(5000, TASK_FOREVER, display.refreshCurrentPage);

Scheduler runner;
lcdDisplay display;
page *pages[1];
lcdValue *values[1];

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
	runner.addTask(t_updateLCD);
#ifdef _simulation
	runner.addTask(simulateHallSensor);
	simulateHallSensor.enable();
#endif
	t1.enable();
	t_readCurrent.enable();
	t_readVoltage.enable();

	Serial.println("added tasks");

	initPages();
	initLcdValues();

	display.insertPage(pages[0]);
	display.showPage(pages[0]);
	t_updateLCD.enable();

	delay(500);
}


void initLcdValues() {
	values[0] = new lcdValue();
	values[0]->getValue = lastCurrent.getValue;
	values[0]->param = "kmh";
}

void initPages() {
	pages[0] = new page();
	pages[0]->lcdval[0] = values[0];
	pages[0]->lcdval[1] = NULL;
	pages[0]->lcdval[2] = NULL;
	pages[0]->lcdval[3] = values[0];
	pages[0]->lcdval[4] = values[0];
	pages[0]->lcdval[5] = NULL;
	pages[0]->lcdval[6] = NULL;
	pages[0]->lcdval[7] = values[0];

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





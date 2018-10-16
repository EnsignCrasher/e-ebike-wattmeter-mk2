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

Scheduler runner;
page *pages[1];
lcdValue *values[1];
lcdDisplay display;

void taskfunc_updateDisplay() {
	display.refreshCurrentPage();
}
Task t1(2000, 10, &t1Callback);

#define wheelCycleTime 267 //ca. 28kmh

Task simulateHallSensor(wheelCycleTime, TASK_FOREVER, hallSensorTriggered);
Task t_readVoltage(1000, TASK_FOREVER, readVoltage);
Task t_readCurrent(1000, TASK_FOREVER, readCurrent);
Task t_updateLCD(5000, TASK_FOREVER, taskfunc_updateDisplay);


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

	initLcdValues();
	initPages();
	display.init();
	
	

	display.insertPage(pages[0]);
	display.showPage(pages[0]);
	t_updateLCD.enable();

	delay(500);
	
}

float(*fpt)();

void initLcdValues() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	values[0] = new lcdValue();
	values[0]->mp = &lastCurrent;
	values[0]->param = (char *)malloc(4); /* make sure you check for != NULL in real code */
	values[0]->param = "kmh";
	dprintf(lvl_verbose, "set lcdvalue 0, param: " + String(values[0]->param) + " \n")

	values[1] = new lcdValue();
	values[1]->mp = &lastVoltage;
	values[1]->param = (char *)malloc(4); /* make sure you check for != NULL in real code */
	values[1]->param = "km ";
	dprintf(lvl_verbose, "set lcdvalue 1, param: " + String(values[1]->param) + " \n")
	dprintf(lvl_trace, String(__func__) + " leave\n");
}	

void initPages() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	pages[0] = new page();
	lcdValue *tmpVal = values[0];
	dprintf(lvl_verbose, "Assign value " + String(tmpVal->param) + "\n");
	pages[0]->lcdval[0] = values[1];
	pages[0]->lcdval[1] = values[0];
	pages[0]->lcdval[2] = values[0];
	pages[0]->lcdval[3] = values[1];
	pages[0]->lcdval[4] = values[1];
	pages[0]->lcdval[5] = values[0];
	pages[0]->lcdval[6] = values[0];
	pages[0]->lcdval[7] = values[1];

	for (int i = 0; i < 8; i++)
		dprintf(lvl_verbose, pages[0]->lcdval[i] ? String(pages[0]->lcdval[i]->param) : "no value\n");

	dprintf(lvl_trace, String(__func__) + " leave\n");
}

// the loop function runs over and over again until power down or reset
void loop() {
	runner.execute();
	
	//reset speed if we don't move 
	if (millis() > lastDistanceMS.getTimestamp() + calcSpeedTimeout()) {
		actSpeed = 0;
	}

	delay(100); //should be fast enough
}

void t1Callback() {
	//do sth
}





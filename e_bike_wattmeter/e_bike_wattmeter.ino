/*
 Name:		e_bike_wattmeter.ino
 Created:	20.08.2018 20:59:11
 Author:	Administrator
*/

#include "lib/TaskScheduler.h"

void t1Callback();

Task t1(2000, 10, &t1Callback);

Scheduler runner;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	Serial.println("Scheduler TEST");

	runner.init();
	Serial.println("Initialized scheduler");

	runner.addTask(t1);
	Serial.println("added t1");

	delay(500);

	t1.enable();

}

// the loop function runs over and over again until power down or reset
void loop() {
	runner.execute();
}

void t1Callback() {
	//do sth
}


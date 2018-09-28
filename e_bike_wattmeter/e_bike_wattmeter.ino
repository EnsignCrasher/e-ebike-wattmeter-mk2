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
#ifdef _simulation
	#include "inputSimulation.h"
#endif

void t1Callback();
void hallSensorTriggered();

Task t1(2000, 10, &t1Callback);

#define wheelCycleTime 267 //ca. 28kmh

Task simulateHallSensor(wheelCycleTime, TASK_FOREVER, hallSensorTriggered);

Scheduler runner;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	Serial.println("Scheduler TEST");

	initVariables();

	runner.init();
	Serial.println("Initialized scheduler");

	runner.addTask(t1);
#ifdef _simulation
	runner.addTask(simulateHallSensor);
	simulateHallSensor.enable();
#endif
	Serial.println("added tasks");

	delay(500);

	t1.enable();


	

}

// the loop function runs over and over again until power down or reset
void loop() {
	runner.execute();
	
	//reset speed if we don't move 
	if (millis() > lastDistanceMS.getTimestamp() + speedTimeout()) {
		actSpeed = 0;
	}

	delay(10); //should be fast enough
}

void t1Callback() {
	//do sth
}

void hallSensorTriggered() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	measurement *lastMS = lastDistanceMS.makeCopy();
	lastDistanceMS.setValue(lastDistanceMS.getValue() + wheelDiameter);
	unsigned long deltaT = lastDistanceMS.getTimestamp() - lastMS->getTimestamp();
	float deltaKM = lastDistanceMS.getValue() - lastMS->getValue();

	float speed = deltaKM * 1000 * 3.6 / deltaT;

	/*
		we only want to count speed values over threshold as for too small values our calculation for wh/km would go through the top
		we don't need to null speed here, loop() does that for us when it's time
	*/
	if (lastMS) {
		dprintf(lvl_info, "t1 " + String(lastMS->getTimestamp()));
		dprintf(lvl_info, " t2 " + String(lastDistanceMS.getTimestamp()));
		dprintf(lvl_info, " km1 " + String(lastMS->getValue()));
		dprintf(lvl_info, " km2 " + String(lastDistanceMS.getValue()));
		dprintf(lvl_info, " deltaT " + String(deltaT));
		dprintf(lvl_info, " deltaKM " + String(deltaKM));
		dprintf(lvl_info, "calculated speed " + String(speed) + "\n");
	}
	if (speed > speedThreshold) {
		actSpeed = speed;
	}

	delete lastMS;
	dprintf(lvl_trace, String(__func__) + " leave\n");
}


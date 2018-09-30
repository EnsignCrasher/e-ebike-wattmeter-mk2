#pragma once

#define CURRENT_UPPER_RANGE 50 //in ampere
#define VOLTAGE_UPPER_RANGE 60 //in volts

/*
	output voltage in analog range 0-1024 (VOLTAGE_LOWER_RANGE-VOLTAGE_UPPER_RANGE)
*/
unsigned short analogReadVoltage() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	unsigned short ret = 0;
#ifdef _simulation
	ret = 819; //ca. 48V
#else
	dprintf(lvl_err, "real analogRead not implemented yet!")
#endif

	dprintf(lvl_trace, String(__func__) + " leave\n");
	return ret;
}

/*
	output in analog range 0-1024 (CURRENT_LOWER_RANGE-CURRENT_UPPER_RANGE) 
*/
unsigned short analogReadCurrent() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	unsigned short ret = 0;
#ifdef _simulation
	ret = 72; //ca. 3,5A
#else
	dprintf(lvl_err, "real analogRead not implemented yet!");
	ret = 0;
#endif

	dprintf(lvl_trace, String(__func__) + " leave\n");
	return ret;
}

void readVoltage() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	unsigned short analog_volt = analogReadVoltage();

	// remap value 
	float actVolt = (float)analog_volt * VOLTAGE_UPPER_RANGE / 1024;

	lastVoltage.setValue(actVolt);
	dprintf(lvl_info, "Measured voltage: " + String(lastVoltage.getValue()) + "V\n");

	dprintf(lvl_trace, String(__func__) + " leave\n");
}


void readCurrent() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	unsigned short analog_cur = analogReadCurrent();

	// remap value 
	float actCur = (float)analog_cur * CURRENT_UPPER_RANGE / 1024;

	lastCurrent.setValue(actCur);
	dprintf(lvl_info, "Measured current: " + String(lastCurrent.getValue()) + "V\n");

	dprintf(lvl_trace, String(__func__) + " leave\n");
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
	dbg_vpf = false; //don't output these values
	dprintf(lvl_verbose, "t1 " + String(lastMS->getTimestamp()));
	dprintf(lvl_verbose, " t2 " + String(lastDistanceMS.getTimestamp()));
	dprintf(lvl_verbose, " km1 " + String(lastMS->getValue()));
	dprintf(lvl_verbose, " km2 " + String(lastDistanceMS.getValue()));
	dprintf(lvl_verbose, " deltaT " + String(deltaT));
	dprintf(lvl_verbose, " deltaKM " + String(deltaKM));
	dbg_vpf = true;

	dprintf(lvl_verbose, "calculated speed " + String(speed) + "\n");

	if (speed > speedThreshold) {
		actSpeed = speed;
	}

	delete lastMS;
	dprintf(lvl_trace, String(__func__) + " leave\n");
}

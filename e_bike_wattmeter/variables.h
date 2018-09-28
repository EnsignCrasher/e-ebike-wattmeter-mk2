#pragma once
#include "debug.h"

float wheelDiameter = 2.073656; //26"
short speedThreshold = 3;//kmh


class measurement {
public:
	void setValue(float val) {
		value = val;
		timestamp = millis();
	};
	float getValue() {
		return value;
	}
	unsigned long getTimestamp() {
		return timestamp;
	}

	measurement* makeCopy() {
		measurement *tmp = new measurement();
		tmp->value = this->value;
		tmp->timestamp = this->timestamp;
		return tmp;
	}
private:
	unsigned long timestamp;
	float value;
};

measurement lastDistanceMS;
float actSpeed;

void initVariables() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	lastDistanceMS.setValue(0);

	actSpeed = 0;
	dprintf(lvl_trace, String(__func__) + " leave\n");
}
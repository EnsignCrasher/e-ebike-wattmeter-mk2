#pragma once

unsigned short calcSpeedTimeout() {
	return (speedThreshold * 1000) / 3.6;
}


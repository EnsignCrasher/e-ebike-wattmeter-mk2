#pragma once

#define _debug

#ifdef _debug
	#define dprintf(...) debug_print(__VA_ARGS__, 3);
#else
	#define	dprintf(x)
#endif


#ifdef _debug
enum debuglevel {
		lvl_trace,
		lvl_info,
		lvl_warn,
		lvl_err
	};
	
	void debug_print(debuglevel lvl, String str, int shift) {
		switch (lvl) {
		case lvl_trace:
			break;
		case lvl_info:
			for (int i = 0; i < shift; i++) Serial.print(" ");
			break;
		case lvl_warn:
			Serial.print("!");
			for (int i = 0; i < shift*2; i++) Serial.print(" ");
			break;
		case lvl_err:
			Serial.print("!!!");
			for (int i = 0; i < shift*3; i++) Serial.print(" ");
			break;
		default:
			break;
		}
		Serial.print(str);
	}
#endif

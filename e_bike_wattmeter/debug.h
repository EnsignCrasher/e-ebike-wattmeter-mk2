#pragma once

#define _debug
#define _enable_trace 1
#define _enable_info 1 
#define _enable_warn 1 
#define _enable_err 1 
#define _enable_verbose 1

#ifdef _debug
	#define dprintf(level, ...) debug_print(level, __VA_ARGS__, 3);
#else
	#define	dprintf(x)
#endif

#ifdef _debug
boolean dbg_vpf = true; //verbose per function, always print verbose if not explicitly disabled 


enum debuglevel {
		lvl_none,
		lvl_trace,
		lvl_info,
		lvl_warn,
		lvl_err,
		lvl_verbose
	};
	
	void debug_print(debuglevel lvl, String str, int shift) {
		switch (lvl) {
		case lvl_none:
			break;
		case lvl_trace:
			if (_enable_trace == 0)
				return;
			break;
		case lvl_info:
			if (_enable_info == 0)
				return;
			for (int i = 0; i < shift; i++) Serial.print(" ");
			break;
		case lvl_warn:
			if (_enable_warn == 0)
				return;
			Serial.print("!");
			for (int i = 0; i < shift*2; i++) Serial.print(" ");
			break;
		case lvl_err:
			if (_enable_err == 0)
				return;
			Serial.print("!!!");
			for (int i = 0; i < shift*3; i++) Serial.print(" ");
			break;
		case lvl_verbose:
			if (_enable_verbose == 0)
				return;
			break;
		default:
			break;
		}
		if((lvl == lvl_verbose && dbg_vpf) || (lvl != lvl_verbose && lvl != lvl_none))
			Serial.print(str);
	}
#endif

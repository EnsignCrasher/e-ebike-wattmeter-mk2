#pragma once

#define _debug

#ifdef _debug
	#define dprintf(...) debug_print(__VA_ARGS__, 3);
#else
	#define	dprintf(x)
#endif

boolean dbg_vpf = true; //verbose per function, always print verbose if not explicitly disabled 

#ifdef _debug
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
		case lvl_verbose:
		default:
			break;
		}
		if((lvl == lvl_verbose && dbg_vpf) || !lvl == (lvl_verbose && !lvl == lvl_none))
			Serial.print(str);
	}
#endif

/* radare - LGPL - Copyright 2010-2015 pancake */

#include <r_debug.h>

R_API ut64 r_debug_arg_get (RDebug *dbg, int cctype, int num) {
	ut32 n32;
	ut64 n64, sp;
	char reg[32];
	switch (cctype) {
	case R_ANAL_CC_TYPE_NONE:
	case R_ANAL_CC_TYPE_SYSV:
	case R_ANAL_CC_TYPE_FASTCALL:
		snprintf (reg, 30, "SP%d", num);
		return r_debug_reg_get (dbg, reg);
	case R_ANAL_CC_TYPE_STDCALL:
	case R_ANAL_CC_TYPE_PASCAL:
		sp = r_debug_reg_get (dbg, "SP");
		if (dbg->bits == 64) {
			sp += 8; // skip return address, asume we are inside the call
			sp += 8 * num;
			dbg->iob.read_at (dbg->iob.io, sp, (ut8*)&n64, sizeof(ut64));
			// TODO: honor endianness of platform
			return (ut64)n64;
		} else {
			sp += 4; // skip return address, asume we are inside the call
			sp += 4 * num;
			dbg->iob.read_at (dbg->iob.io, sp, (ut8*)&n32, sizeof(ut32));
			// TODO: honor endianness of platform
			return (ut64)n32;
		}
		break;
	}
	snprintf (reg, 30, "A%d", num);
	return r_debug_reg_get (dbg, reg);
}

R_API bool r_debug_arg_set (RDebug *dbg, int cctype, int num, ut64 val) {
	// TODO
	char reg[32];
	switch (cctype) {
	case R_ANAL_CC_TYPE_NONE:
	case R_ANAL_CC_TYPE_SYSV:
	case R_ANAL_CC_TYPE_FASTCALL:
		snprintf (reg, 30, "A%d", num);
		return r_debug_reg_set (dbg, reg, val);
	case R_ANAL_CC_TYPE_STDCALL:
	case R_ANAL_CC_TYPE_PASCAL:
		/* TODO: get from stack */
		break;
	}
	return false;
}

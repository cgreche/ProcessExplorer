
//Time
// File: time.h
// Last Edit: 27/10/2017 02:40 (UTC-3)
// Author: CGR

#ifndef __TIME_H__
#define __TIME_H__

#include <uilib/types.h>

namespace uilib {

	u64 os_ticks();
	u64 os_ticksPerSecond();
	void os_sleep(u64 ticksToSleep);

	class OSTime {
	public:
		static u64 ticks() { return os_ticks(); }
		static u64 ticksPerSecond() { return os_ticksPerSecond(); }
		static u64 sleep(u64 ticksToSleep) { os_sleep(ticksToSleep); }
	};

}

#endif
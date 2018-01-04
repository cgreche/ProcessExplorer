
//OSDTime
// File: time.cpp
// Last Edit: 27/10/2017 02:36 (UTC-3)
// Author: CGR

//WINDOWS Timing functions

#include <windows.h>
#include <uilib/types.h>
#include "..\\time.h"

namespace uilib {

	static u64 g_tps = 0;

	u64 os_ticks()
	{
		LARGE_INTEGER freq;
		::QueryPerformanceCounter(&freq);
		return (s64)freq.QuadPart;
	}

	u64 os_ticksPerSecond() {
		LARGE_INTEGER freq;
		if (g_tps)
			return g_tps;
		::QueryPerformanceFrequency(&freq);
		if (freq.QuadPart == 0)
			return 0;
		g_tps = freq.QuadPart;
		return g_tps;
	}

	//Copied from MAME
	void os_sleep(u64 ticksToSleep)
	{
		DWORD msec;

		// make sure we've computed ticks_per_second
		if (g_tps == 0)
			os_ticksPerSecond();

		// convert to milliseconds, rounding down
		msec = (DWORD)(ticksToSleep * 1000 / g_tps);

		// only sleep if at least 2 full milliseconds
		if (msec >= 2)
		{
			HANDLE current_thread = GetCurrentThread();
			int old_priority = GetThreadPriority(current_thread);

			// take a couple of msecs off the top for good measure
			msec -= 2;

			// bump our thread priority super high so that we get
			// priority when we need it
			SetThreadPriority(current_thread, THREAD_PRIORITY_TIME_CRITICAL);
			Sleep(msec);
			SetThreadPriority(current_thread, old_priority);
		}
	}

}
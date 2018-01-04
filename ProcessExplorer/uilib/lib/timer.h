
//Timer
// File: timer.h
// Last Edit: 12/01/2015 20:42 (UTC-3)
// Author: CGR

#ifndef __TIMER_H__
#define __TIMER_H__

#include "..\\types.h"
#include "time.h"

namespace uilib {

	class Timer;
	class TimerSystem;

	typedef void(*TimerFunc)(void *param, int id);

	class Timer
	{
		friend class TimerSystem;

		TimerSystem &m_handler;

		bool m_enabled;
		bool m_periodic;
		u64 m_startTime;
		u64 m_expireTime;
		u64 m_duration; //for periodic times

		TimerFunc m_timerFunc;
		void *m_param;
		int m_id;

		Timer *m_prev;
		Timer *m_next;

		Timer(TimerSystem &handler);
		~Timer();

		void trigger();

	public:

		void start(u64 duration, bool periodic = true);
		void reset();
		void set(u64 duration, bool periodic, bool enabled, TimerFunc callback, void *param, int id = 0);
		void setEnabled(bool enabled);
		void setDuration(u64 duration, bool periodic, bool update = false);
		inline void setPeriodic(bool periodic) { m_periodic = periodic; }
		inline void setParam(void *param) { m_param = param; }

		bool enabled() const { return m_enabled; }
		bool periodic() const { return m_periodic; }
		u64 startTime() const { return m_startTime; }
		u64 expireTime() const { return m_expireTime; }
		u64 duration() const { return m_duration; }

		u64 elapsed() const;
		u64 remaining() const;

		static inline u64 seconds(int s);
		static inline u64 miliseconds(int ms);
	};

	inline u64 Timer::seconds(int s) { return s*os_ticksPerSecond(); }
	inline u64 Timer::miliseconds(int ms) { return ms*os_ticksPerSecond() / 1000; }

	class TimerSystem
	{
		friend class Timer;

		u64 m_currentTime;

		Timer *m_firstTimer;

		void insertTimerIntoList(Timer &timer);
		void removeTimerFromList(Timer &timer);
		inline void updateTimer(Timer &timer);

	public:
		TimerSystem();

		Timer *createTimer(TimerFunc callback, void *param, int id = 0);
		Timer *createTimer(int id = 0) { return createTimer(NULL, NULL, id); }
		Timer *createOneShotTimer(u64 duration, TimerFunc callback, void *param, int id = 0);
		Timer *createPeriodicTimer(u64 period, TimerFunc callback, void *param, int id = 0);

		void execute();

		u64 currentTime() const;
	};

}

#endif
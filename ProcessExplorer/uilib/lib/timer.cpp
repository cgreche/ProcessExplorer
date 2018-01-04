
//Timer
// File: timer.cpp
// Last Edit: 27/10/2017 03:23 (UTC-3)
// Author: CGR

#include "timer.h"

namespace uilib {

	Timer::Timer(TimerSystem &handler)
		: m_handler(handler),
		m_enabled(false),
		m_periodic(false),
		m_duration(-1),
		m_timerFunc(NULL),
		m_param(NULL),
		m_id(0),
		m_startTime(0),
		m_expireTime(-1),

		m_prev(NULL),
		m_next(NULL)
	{
	}

	void Timer::trigger()
	{
		if (!m_periodic)
			m_enabled = false;

		//save information about the current startTime
		u64 oldStart = m_startTime;

		if (m_timerFunc)
			(*m_timerFunc)(m_param, m_id);

		//we only need to reschedule if the timer wasn't changed inside the callback
		if (m_startTime == oldStart) {
			if (m_periodic) {
				//re-schedule
				m_startTime = m_expireTime;
				m_expireTime += m_duration;
			}
		}
	}

	void Timer::setEnabled(bool enabled)
	{
		if (m_enabled == enabled)
			return;

		m_enabled = enabled;
		m_handler.updateTimer(*this);
	}

	void Timer::start(u64 duration, bool periodic)
	{
		m_enabled = true;
		m_duration = duration;
		m_periodic = periodic;

		u64 time = m_handler.currentTime();
		m_startTime = time;
		m_expireTime = time + duration;
		m_handler.updateTimer(*this);
	}

	void Timer::reset()
	{
		start(m_duration);
	}

	void Timer::set(u64 duration, bool periodic, bool enabled, TimerFunc callback, void *param, int id)
	{
		u64 time = m_handler.currentTime();
		m_periodic = true;
		m_duration = duration;
		m_timerFunc = callback;
		m_param = param;
		m_id = id;

		setEnabled(enabled);
		m_startTime = time;
		m_expireTime = time + duration;
		m_handler.updateTimer(*this);
	}

	void Timer::setDuration(u64 duration, bool periodic, bool update)
	{
		m_duration = duration;
		m_periodic = periodic;

		if (update) {
			m_expireTime = m_startTime + duration;
			m_handler.updateTimer(*this);
		}
	}

	u64 Timer::elapsed() const
	{
		return m_handler.currentTime() - m_startTime;
	}

	u64 Timer::remaining() const
	{
		u64 current = m_handler.currentTime();
		if (current >= m_expireTime)
			return 0;
		return m_expireTime - current;
	}



	TimerSystem::TimerSystem()
		: m_firstTimer(NULL)
	{
		m_currentTime = os_ticks();
	}

	void TimerSystem::insertTimerIntoList(Timer &timer)
	{
		Timer *t = m_firstTimer;

		if (!t) {
			m_firstTimer = &timer;
		}
		else {
			u64 expire = timer.expireTime();
			bool enabled = timer.enabled();

			for (;;) {
				if (enabled && t->expireTime() > expire) {

					timer.m_prev = t->m_prev;
					timer.m_next = t;
					if (t->m_prev)
						t->m_prev->m_next = &timer;
					t->m_prev = &timer;

					if (m_firstTimer == t)
						m_firstTimer = &timer;
					return;
				}

				if (!t->m_next)
					break;

				t = t->m_next;
			}

			//we will be the last timer
			t->m_next = &timer;
			timer.m_prev = t;
			timer.m_next = NULL;
		}
	}

	void TimerSystem::removeTimerFromList(Timer &timer)
	{
		if (m_firstTimer == &timer) {
			m_firstTimer = timer.m_next;
		}
		else {
			if (timer.m_prev)
				timer.m_prev->m_next = timer.m_next;
		}

		if (timer.m_next)
			timer.m_next->m_prev = timer.m_prev;
	}

	inline void TimerSystem::updateTimer(Timer &timer)
	{
		removeTimerFromList(timer);
		insertTimerIntoList(timer);
	}

	Timer *TimerSystem::createTimer(TimerFunc callback, void *param, int id)
	{
		Timer *timer = new Timer(*this);
		timer->m_timerFunc = callback;
		timer->m_param = param;
		timer->m_id = id;
		insertTimerIntoList(*timer);
		return timer;
	}

	Timer *TimerSystem::createOneShotTimer(u64 duration, TimerFunc callback, void *param, int id)
	{
		Timer *timer = new Timer(*this);
		timer->set(duration, false, true, callback, param, id);
		timer->reset();
		insertTimerIntoList(*timer);
		return timer;
	}

	Timer *TimerSystem::createPeriodicTimer(u64 period, TimerFunc callback, void *param, int id)
	{
		Timer *timer = new Timer(*this);
		timer->set(period, true, true, callback, param, id);
		timer->reset();
		insertTimerIntoList(*timer);
		return timer;
	}

	void TimerSystem::execute()
	{
		if (!m_firstTimer)
			return;

		u64 curTime = os_ticks();
		m_currentTime = curTime;

		while (curTime > m_firstTimer->m_expireTime) {
			Timer &t = *m_firstTimer;

			if (t.enabled()) {
				t.trigger();
			}

			updateTimer(t);
		}
	}

	u64 TimerSystem::currentTime() const
	{
		return m_currentTime;
	}

}
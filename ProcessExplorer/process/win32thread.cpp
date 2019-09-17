
//Process
//Filename: win32thread.cpp
//Last edit: 13/09/2019 22:46 (UTC-3)
//Author: CGR

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "win32thread.h"

Win32Thread::Win32Thread(HANDLE hThread)
{
	m_hThread = hThread;
	m_threadId = ::GetThreadId(hThread);
}

int Win32Thread::release()
{
	if (m_hThread != NULL)
	{
		::CloseHandle(m_hThread);
		m_hThread = 0;
	}

	delete this;
	return 0;
}

bool Win32Thread::suspend()
{
	return ::SuspendThread(m_hThread) >= 0;
}

bool Win32Thread::resume()
{
	return ::ResumeThread(m_hThread) >= 0;
}

int Win32Thread::sync(unsigned long time)
{
	::WaitForSingleObject(m_hThread, time);
	return 0;
}

int Win32Thread::exitCode() const
{
	DWORD exitcode;
	::GetExitCodeThread(m_hThread, &exitcode);
	return exitcode;
}

bool Win32Thread::active() const
{
	DWORD exitcode;
	return ::GetExitCodeThread(m_hThread, &exitcode) && exitcode == STILL_ACTIVE;
}

void Win32Thread::exit(int code)
{
	if (active())
		::TerminateThread(m_hThread, code);
}
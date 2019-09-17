
//Process
//Filename: win32thread.h
//Last edit: 13/09/2019 22:46 (UTC-3)
//Author: CGR

#ifndef __WIN32_THREAD_H__
#define __WIN32_THREAD_H__

#include <windows.h>

#include "thread.h"

class Win32Thread : public CThread
{
	DWORD m_threadId;
	HANDLE m_hThread;

	Win32Thread(HANDLE handle);

public:
	friend class Win32Process;

	virtual int release();

	virtual bool suspend();
	virtual bool resume();

	virtual int sync(unsigned long time = INFINITE);
	virtual void exit(int code = 0);

	virtual unsigned int id() const { return (unsigned int)m_threadId; }
	virtual void* internalHandle() const { return (void*)m_hThread; }
	virtual int exitCode() const;
	virtual bool active() const;
};

#endif
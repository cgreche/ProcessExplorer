
//Process
//Filename: win32process.h
//Last edit: 13/09/2019 21:58 (UTC-3)
//Author: CGR

#ifndef __WIN32_PROCESS_H__
#define __WIN32_PROCESS_H__

#include <windows.h>

#include <list>
#include <vector>

#include "process.h"

class Win32Thread : public CThread
{
	DWORD m_threadId;
	HANDLE m_hThread;

	Win32Thread(HANDLE handle);
	~Win32Thread();

	void close();

public:
	friend class Win32Process;
	enum ThreadState
	{
		NORMAL = 0,
		SUSPENDED
	};

	virtual bool suspend();
	virtual bool resume();

	virtual int sync(unsigned long time = INFINITE);

	virtual int exitCode() const;
	virtual bool active() const;
	virtual void exit(int code = 0);

	virtual unsigned int id() const { return (unsigned int)m_threadId; }
	virtual void* internalHandle() const { return (void*)m_hThread; }

};

class Win32Process : public CProcess
{
	DWORD m_processId;
	HANDLE m_hProcess;
	std::vector<int> m_threadList;

	Win32Process(HANDLE handle);
	~Win32Process();

	CThread *openThread(DWORD threadId) const;

public:
	friend class ProcessSpawner;

	enum CreationError
	{
		SUCCESS = 0,
		FILE_NOT_FOUND,
		BAD_SYSTEM_EXECUTABLE_FILE,
		OUT_OF_MEMORY,
	};

	virtual const void *alloc(size_t size);
	virtual void free(const void *address);
	virtual bool write(const void *address, const unsigned char *inputBuffer, size_t size);
	virtual bool read(const void *address, unsigned char *outputBuffer, size_t size);

	virtual void wait(unsigned long timeout = INFINITE);
	virtual void sync(unsigned long timeout = INFINITE);

	virtual int exitCode() const;
	virtual bool active() const;
	virtual void close();
	virtual void exit(int code = 0);

	virtual CThread *createThread(const void *address, void *param, CThread::ThreadState initialState);
	virtual std::vector<int> threadList() const;
	virtual CThread *thread(unsigned int threadId) const;

	virtual unsigned int id() const { return m_processId; }
	virtual const void *internalHandle() const { return m_hProcess; }

	HWND getMainWindow();
};

#endif
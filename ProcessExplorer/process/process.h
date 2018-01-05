
//Process
//Filename: process.h
//Last edit: 05/01/2018 00:22 (UTC-3)
//Author: CGR

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <windows.h>

#include <list>
#include <vector>


struct ProcessBasicInfo
{
	unsigned int id;
	char name[MAX_PATH];
};

class CThread
{
	DWORD m_threadId;
	HANDLE m_hThread;
	
	~CThread();
	
public:
	enum ThreadState
	{
		NORMAL = 0,
		SUSPENDED
	};

	CThread(HANDLE hThread);
	
	bool suspend();
	bool resume();

	void wait(DWORD time = INFINITE);

	int exitCode();
	bool isActive();
	void close();
	void terminate(int code = 0);

	DWORD id() { return m_threadId; }
	HANDLE internalHandle() { return m_hThread; }

};

class CProcess
{
	DWORD m_processId;
	HANDLE m_hProcess;
	std::vector<CThread*> m_threadList;

	void removeAllThreads();
	void removeThread();

	~CProcess();

public:

	enum CreationError
	{
		SUCCESS = 0,
		FILE_NOT_FOUND,
		BAD_SYSTEM_EXECUTABLE_FILE,
		OUT_OF_MEMORY,
	};

	CProcess(HANDLE hProcess);

	BYTE *alloc(size_t size);
	void free(BYTE *lpAddr);
	bool write(LPVOID lpAddr,  LPVOID lpBuffer, DWORD nSize);
	bool read(LPVOID lpAddr,  LPVOID lpBuffer, DWORD nSize);

	void wait(DWORD timeout = INFINITE);
	void sync(DWORD timeout = INFINITE);

	int exitCode();
	bool isActive();
	void close();
	void terminate(int code = 0);

	CThread *createThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParam, CThread::ThreadState initialState);
	CThread *openThread(DWORD id);

	bool is64BitProcess();

	HWND getMainWindow();

	std::vector<CThread*>& threadList();

	DWORD id() { return m_processId; }
	HANDLE internalHandle() { return m_hProcess; }
};

#endif
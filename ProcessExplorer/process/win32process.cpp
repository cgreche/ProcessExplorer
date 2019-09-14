
//Process
//Filename: win32process.cpp
//Last edit: 13/09/2019 21:58 (UTC-3)
//Author: CGR

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include "win32process.h"

Win32Process::Win32Process(HANDLE hProcess)
{
	m_hProcess = hProcess;
	m_processId = ::GetProcessId(m_hProcess);
}

Win32Process::~Win32Process()
{

}

const void *Win32Process::alloc(size_t size)
{
	return (BYTE *)VirtualAllocEx(m_hProcess, NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

void Win32Process::free(const void *address)
{
	::VirtualFreeEx(m_hProcess, (LPVOID)address, 0, MEM_DECOMMIT);
}

bool Win32Process::write(const void *address, const unsigned char *inputBuffer, size_t size)
{
	DWORD dwOldProtect;
	SIZE_T nWritten;

	HANDLE h = m_hProcess;
	if (!::VirtualProtectEx(h, (LPVOID)address, size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		return false;
	::WriteProcessMemory(h, (LPVOID)address, inputBuffer, size, &nWritten);
	::VirtualProtectEx(h, (LPVOID)address, size, dwOldProtect, &dwOldProtect);

	return true;
}


bool Win32Process::read(const void *address, unsigned char *outputBuffer, size_t size)
{
	DWORD dwOldProtect;
	SIZE_T nRead;

	HANDLE h = m_hProcess;
	if (!::VirtualProtectEx(h, (LPVOID)address, size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		return false;
	::ReadProcessMemory(h, address, outputBuffer, size, &nRead);
	::VirtualProtectEx(h, (LPVOID)address, size, dwOldProtect, &dwOldProtect);

	return true;
}

void Win32Process::wait(unsigned long timeout)
{
	::WaitForSingleObject(m_hProcess, timeout);
}

void Win32Process::sync(unsigned long timeout)
{
	::WaitForInputIdle(m_hProcess, timeout);
}

int Win32Process::exitCode() const
{
	DWORD exitcode = ::GetExitCodeProcess(m_hProcess, &exitcode);
	return exitcode;
}

bool Win32Process::active() const
{
	DWORD exitcode;
	return ::GetExitCodeProcess(m_hProcess, &exitcode) && exitcode == STILL_ACTIVE;
}



void Win32Process::close()
{
	if (m_hProcess != NULL)
	{
		::CloseHandle(m_hProcess);
		m_hProcess = 0;
	}

	delete this;
}


void Win32Process::exit(int code)
{
	if (active())
		::TerminateProcess(m_hProcess, code);
}



CThread *Win32Process::createThread(const void *address, void *param, CThread::ThreadState initialState)
{
	DWORD _flags;
	DWORD threadid;
	HANDLE hThread;
	CThread *thread;

	if (initialState == CThread::NORMAL)
		_flags = 0;
	else if (initialState == CThread::SUSPENDED)
		_flags = CREATE_SUSPENDED;

	hThread = ::CreateRemoteThread(m_hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)address, param, _flags, &threadid);
	if (!hThread)
		return NULL;

	thread = new Win32Thread(hThread);
	if (!thread) {
		::TerminateThread(hThread, 0);
		::CloseHandle(hThread);
		return NULL;
	}

	return thread;
}


CThread *Win32Process::openThread(DWORD threadId) const
{
	//TODO: search in a map and return, open otherwise

	/*
	CThread *thread;

	HANDLE hThread = ::OpenThread(THREAD_ALL_ACCESS,FALSE,threadId);
	if(hThread == NULL)
	return NULL;

	thread = new CThread(hThread);
	if(!thread) {
	::CloseHandle(hThread);
	return NULL;
	}
	else {
	m_threadList.push_back(thread);
	}

	return thread;
	*/
	return NULL;
}

std::vector<int> Win32Process::threadList() const
{
	return m_threadList;
}

CThread *Win32Process::thread(unsigned int threadId) const
{
	return openThread(threadId);
}



struct st_getMainWindow
{
	CProcess *process;
	HWND mainHwnd;
};


static BOOL CALLBACK wndEnumProc(HWND hwnd, LPARAM lParam)
{
	DWORD wndProcessId;
	DWORD pid;
	st_getMainWindow *gmw;

	gmw = (st_getMainWindow*)lParam;
	::GetWindowThreadProcessId(hwnd, &wndProcessId);
	pid = gmw->process->id();

	if (wndProcessId == pid) {
		gmw->mainHwnd = hwnd;
		return FALSE;
	}

	return TRUE;
}

HWND Win32Process::getMainWindow()
{
	st_getMainWindow gmw;
	gmw.process = this;
	gmw.mainHwnd = NULL;
	::EnumWindows(wndEnumProc, (LPARAM)&gmw);
	return gmw.mainHwnd;
}
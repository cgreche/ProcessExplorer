
//Process
//Filename: process.cpp
//Last edit: 05/01/2018 00:22 (UTC-3)
//Author: CGR

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include "process.h"

CThread::CThread(HANDLE hThread)
{
	m_hThread = hThread;
	m_threadId = ::GetThreadId(hThread);
}

CThread::~CThread()
{
}

bool CThread::suspend()
{
	return ::SuspendThread(m_hThread) >= 0;
}

bool CThread::resume()
{
	return ::ResumeThread(m_hThread) >= 0;
}

void CThread::wait(DWORD time)
{
	::WaitForSingleObject(m_hThread,time);
}

int CThread::exitCode()
{
	DWORD exitcode;
	::GetExitCodeThread(m_hThread,&exitcode);
	return exitcode;
}

bool CThread::isActive()
{
	DWORD exitcode;
	return ::GetExitCodeThread(m_hThread,&exitcode) && exitcode == STILL_ACTIVE;
}

void CThread::close()
{
	if(m_hThread != NULL)
	{
		::CloseHandle(m_hThread);
		m_hThread = 0;
	}
	delete this;
}

void CThread::terminate(int code)
{
	if(isActive())
		::TerminateThread(m_hThread,code);
}











CProcess::CProcess(HANDLE hProcess)
{
	m_hProcess = hProcess;
	m_processId = ::GetProcessId(m_hProcess);
}

CProcess::~CProcess() {
	for (CThread *thread : m_threadList) {
		thread->close();
	}
}

BYTE *CProcess::alloc(size_t size)
{
	return (BYTE *)VirtualAllocEx(m_hProcess,NULL,size,MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

void CProcess::free(BYTE *lpAddr)
{
	::VirtualFreeEx(m_hProcess,lpAddr,0,MEM_DECOMMIT);
}

bool CProcess::write(LPVOID lpAddr, LPVOID lpBuffer, DWORD nSize)
{
	DWORD dwOldProtect;
	SIZE_T nWritten;

	HANDLE h = m_hProcess;
	if(!::VirtualProtectEx(h,lpAddr,nSize,PAGE_EXECUTE_READWRITE,&dwOldProtect))
		return false;

	::WriteProcessMemory(h,lpAddr,lpBuffer,nSize,&nWritten);
	::VirtualProtectEx(h,lpAddr,nSize,dwOldProtect,&dwOldProtect);

	return true;
}


bool CProcess::read(LPVOID lpAddr, LPVOID lpBuffer, DWORD nSize)
{
	DWORD dwOldProtect;
	SIZE_T nRead;

	HANDLE h = m_hProcess;
	
	if(!::VirtualProtectEx(h,lpAddr,nSize,PAGE_EXECUTE_READWRITE,&dwOldProtect))
		return false;

	::ReadProcessMemory(h,lpAddr,lpBuffer,nSize,&nRead);
	::VirtualProtectEx(h,lpAddr,nSize,dwOldProtect,&dwOldProtect);

	return true;
}

void CProcess::wait(DWORD timeout)
{
	::WaitForSingleObject(m_hProcess, timeout);
}

void CProcess::sync(DWORD timeout)
{
	::WaitForInputIdle(m_hProcess, timeout);
}

int CProcess::exitCode()
{
	DWORD exitcode = ::GetExitCodeProcess(m_hProcess,&exitcode);
	return exitcode;
}

bool CProcess::isActive()
{
	DWORD exitcode;
	return ::GetExitCodeProcess(m_hProcess,&exitcode) && exitcode == STILL_ACTIVE;
}



void CProcess::close()
{
	if(m_hProcess != NULL)
	{
		::CloseHandle(m_hProcess);
		m_hProcess = 0;
	}
	
	delete this;
}


void CProcess::terminate(int code)
{
	if(isActive())
		::TerminateProcess(m_hProcess,code);
}



CThread *CProcess::createThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParam, CThread::ThreadState initialState)
{
	DWORD _flags;
	DWORD threadid;
	HANDLE hThread;
	CThread *thread;

	if(initialState == CThread::NORMAL)
		_flags = 0;
	else if(initialState == CThread::SUSPENDED)
		_flags = CREATE_SUSPENDED;

	hThread = ::CreateRemoteThread(m_hProcess,NULL,0,lpStartAddress,lpParam,_flags,&threadid);
	if(!hThread)
		return NULL;

	thread = new CThread(hThread);
	if(!thread) {
		::TerminateThread(hThread,0);
		::CloseHandle(hThread);
		return NULL;
	}

	return thread;
}


CThread *CProcess::openThread(DWORD threadId)
{
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
}

bool CProcess::is64BitProcess() {
	BOOL ret;
	return ::IsWow64Process(m_hProcess,&ret) && ret == FALSE;
}


std::vector<CThread*>& CProcess::threadList()
{
	return m_threadList;
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
	::GetWindowThreadProcessId(hwnd,&wndProcessId);
	pid = gmw->process->id();

	if(wndProcessId==pid) {
		gmw->mainHwnd = hwnd;
		return FALSE;
	}

	return TRUE;
}

HWND CProcess::getMainWindow()
{
	st_getMainWindow gmw;
	gmw.process = this;
	gmw.mainHwnd = NULL;
	::EnumWindows(wndEnumProc,(LPARAM)&gmw);
	return gmw.mainHwnd;
}
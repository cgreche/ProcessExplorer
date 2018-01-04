#include "process/process.h"
#include "ProcessSpawner.h"
#include <tlhelp32.h>

CProcess *ProcessSpawner::OpenByWindowName(const char *winName) {
	HWND hwnd = ::FindWindow(0, winName);
	if (!hwnd) return false;

	DWORD pid;
	::GetWindowThreadProcessId(hwnd, &pid);
	if (!pid) return false;

	return ProcessSpawner::Open(pid);
}

CProcess *ProcessSpawner::OpenByProcessName(const char *processName) {
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return NULL;

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!::Process32First(hProcessSnap, &pe32))
		goto error;

	do
	{
		if (_stricmp(processName, pe32.szExeFile) == 0)
		{
			::CloseHandle(hProcessSnap);
			return ProcessSpawner::Open(pe32.th32ProcessID);
		}

	} while (::Process32Next(hProcessSnap, &pe32));


error:
	::CloseHandle(hProcessSnap);
	return NULL;
}

CProcess *ProcessSpawner::Open(DWORD pid) {
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
		return NULL;

	CProcess *process = new CProcess(hProcess);
	if (!process) {
		::CloseHandle(hProcess);
		return NULL;
	}

	return process;
}

CProcess *ProcessSpawner::Create(const char *appName, CThread::ThreadState initalState, const char *initialWorkpath, CProcess::CreationError *pErr, bool openMainThread) {
	CProcess *process;
	DWORD _flags;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	if (initalState == CThread::NORMAL)
		_flags = 0;
	else if (initalState == CThread::SUSPENDED)
		_flags = CREATE_SUSPENDED;

	int res = ::CreateProcess(NULL, (char*)appName, NULL, NULL, FALSE, _flags, NULL, initialWorkpath, &si, &pi);
	if (res == 0) {
		if (pErr) {
			int err = GetLastError();
			if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND || err == ERROR_DIRECTORY) {
				*pErr = CProcess::FILE_NOT_FOUND;
			}
			else if (err == ERROR_BAD_EXE_FORMAT) {
				*pErr = CProcess::BAD_SYSTEM_EXECUTABLE_FILE;
			}
		}
		return NULL;
	}

	::CloseHandle(pi.hThread);
	process = new CProcess(pi.hProcess);
	if (!process) {
		if (pErr)
			*pErr = CProcess::OUT_OF_MEMORY;
		::CloseHandle(pi.hProcess);
	}

	if (openMainThread)
		process->openThread(pi.dwThreadId);

	if (pErr)
		*pErr = CProcess::SUCCESS;

	return process;
}

bool ProcessSpawner::GetProcessList(std::vector<ProcessBasicInfo> &processList) {
	bool error;
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return false;

	error = true;
	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!::Process32First(hProcessSnap, &pe32))
		goto _finish;

	do
	{
		ProcessBasicInfo info;
		info.id = pe32.th32ProcessID;
		::strcpy(info.name, pe32.szExeFile);
		processList.push_back(info);

	} while (::Process32Next(hProcessSnap, &pe32));

	error = false;

_finish:
	::CloseHandle(hProcessSnap);
	return false;
}


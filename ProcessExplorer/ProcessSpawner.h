#pragma once

#ifndef __PROCESS_SPAWNER_H__
#define __PROCESS_SPAWNER_H__

#include "process/process.h"

class ProcessSpawner {

public:
	static CProcess *OpenByWindowName(const char *win_name);
	static CProcess *OpenByProcessName(const char *proc_name);
	static CProcess *Open(DWORD pid);
	static CProcess *Create(const char *appname, CThread::ThreadState initalState = CThread::NORMAL, const char *initial_workpath = NULL, CProcess::CreationError *pErr = NULL, bool openMainThread = true);
	static bool GetProcessList(std::vector<ProcessBasicInfo> &processList);
};

#endif
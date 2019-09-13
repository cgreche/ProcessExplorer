#pragma once

#ifndef __PROCESS_SPAWNER_H__
#define __PROCESS_SPAWNER_H__

#include "process/process.h"

class ProcessSpawner {

public:
	static CProcess *GetByWindowName(const char *win_name);
	static CProcess *Get(const char *processName);
	static CProcess *Get(unsigned int processId);
	static CProcess *Create(const char *appFileName, CThread::ThreadState initalState = CThread::NORMAL, const char *initialWorkpath = NULL, CProcess::CreationError *pErr = NULL);
	static bool GetProcessList(std::vector<ProcessBasicInfo> &processList);
};

#endif
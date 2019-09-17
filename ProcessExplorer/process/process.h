
//Process
//Filename: process.h
//Last edit: 02/01/2018 21:57 (UTC-3)
//Author: CGR

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <list>
#include <vector>

#include "thread.h"

#define INFINITE            0xFFFFFFFF
#define MAX_PATH            260

struct ProcessBasicInfo
{
	unsigned int id;
	char name[MAX_PATH];
};

class CProcess
{
public:
	enum CreationError
	{
		SUCCESS = 0,
		FILE_NOT_FOUND,
		BAD_SYSTEM_EXECUTABLE_FILE,
		SYSTEM_ERROR,
	};

	virtual int release() = 0;

	virtual const void *alloc(size_t size) = 0;
	virtual void free(const void *address) = 0;
	virtual bool write(const void *address, const unsigned char *inputBuffer, size_t size) = 0;
	virtual bool read(const void *address, unsigned char *outputBuffer, size_t size) = 0;

	virtual void wait(unsigned long timeout = INFINITE) = 0;
	virtual void sync(unsigned long timeout = INFINITE) = 0;
	virtual void exit(int code = 0) = 0;

	virtual CThread *createThread(const void *startAddress, void *param, CThread::ThreadState initialState) = 0;

	virtual unsigned int id() const = 0;
	virtual const void *internalHandle() const = 0;
	virtual int exitCode() const = 0;
	virtual bool active() const = 0;
	virtual std::vector<int> threadList() const = 0;
	virtual CThread *thread(unsigned int threadId) const = 0;
};

#endif
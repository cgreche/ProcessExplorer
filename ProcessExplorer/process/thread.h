
//Thread
//Filename: thread.h
//Last edit: 13/09/2019 22:46 (UTC-3)
//Author: CGR

#ifndef __THREAD_H__
#define __THREAD_H__

#define INFINITE            0xFFFFFFFF
#define MAX_PATH            260

struct ThreadBasicInfo {
	unsigned int id;
	char name[MAX_PATH];
};

class CThread
{
public:
	enum ThreadState
	{
		NORMAL = 0,
		SUSPENDED
	};

	virtual bool suspend() = 0;
	virtual bool resume() = 0;
	virtual int sync(unsigned long time = INFINITE) = 0;

	virtual void exit(int code = 0) = 0;

	virtual unsigned int id() const = 0;
	virtual void *internalHandle() const = 0;
	virtual int exitCode() const = 0;
	virtual bool active() const = 0;

};

#endif
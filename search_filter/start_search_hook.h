#pragma once

#include <Windows.h>

#include <mutex>

extern bool gStartSearchPressed;

extern CRITICAL_SECTION gCrtSection;

class StartSearchHook
{
public:
	StartSearchHook();
	~StartSearchHook();

	void release();
	void init();
private:
};
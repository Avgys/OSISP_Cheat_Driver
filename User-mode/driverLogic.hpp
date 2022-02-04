#pragma once

#include "stdafx.hpp"
#include "Msg.hpp"

#define ADDRS_SIZE 10000UL

class cheatDriver {
public:
	BOOL AttachDriver();
	BOOL startDriver();
	BOOL stopDriver();
	BOOL FindValue(DWORD pid, void* value, size_t valueLength);
	BOOL FilterValue(DWORD pid, size_t* addrs, void* value, size_t valueLength);
	BOOL SetValue(DWORD pid, void* value, size_t valueLength, PVOID pV);
	Msg* getMsg();
	cheatDriver() {
		memset(addrs, 0, sizeof(addrs));
	}

private:
	Msg msg;
	HANDLE driverHandle;
	size_t addrs[ADDRS_SIZE] = { 0 };
};
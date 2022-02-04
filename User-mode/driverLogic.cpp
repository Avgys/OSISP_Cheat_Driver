#include <iostream>
#include "utils.hpp"
#include "stdafx.hpp"
#include "driverLogic.hpp"
#include "Console.h"


#pragma once
//#define DEVICE 0x800
#define START_DRIVER 134225923UL//CTL_CODE(DEVICE, 0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define STOP_DRIVER 134225927UL
#define FIND_VALUE 134225931UL
#define FILTER_VALUE 134225935UL
#define SET_VALUE 134225939UL

BOOL cheatDriver::AttachDriver() {
	//system("sc create cheatDriver type= kernel binPath= \\");	

	system("sc start cheatDriver");
	Sleep(100);
	//system("sc start cheatDriver");
	driverHandle = CreateFile(L"\\\\.\\cheatMoneyDevice", GENERIC_WRITE,
		FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
	if (driverHandle == INVALID_HANDLE_VALUE) {
		REPORTERROR;
		return NULL;
	}
	
	return startDriver();

}

Msg* cheatDriver::getMsg() {
	return &msg;
}

BOOL cheatDriver::FindValue(DWORD pid, void* value, size_t valueLength) {
	DWORD returned = 0;
	msg.pid = pid;
	msg.value = value;
	msg.valueLength = valueLength;
	memset(addrs, 0, sizeof(addrs));
	msg.addrs = addrs;
	BOOL success = DeviceIoControl(driverHandle, FIND_VALUE, &msg, sizeof(msg), &msg, sizeof(msg), &returned, nullptr);
	if (success) {
		return success;
	}
	else {
		REPORTERROR;
		return FALSE;
	}	
}

BOOL cheatDriver::FilterValue(DWORD pid, size_t* newAddrs, void* value, size_t valueLength) {
	DWORD returned = 0;
	msg.pid = pid;
	msg.value = value;
	memset(this->addrs, 0, sizeof(addrs));
	msg.addrs = this->addrs;
	BOOL success = DeviceIoControl(driverHandle, FILTER_VALUE, &msg, sizeof(msg), &msg, sizeof(msg), &returned, nullptr);
	if (success) {
		return success;
	}
	else {
		REPORTERROR;
		return FALSE;
	}
}

BOOL cheatDriver::SetValue(DWORD pid, void* value, size_t valueLength, PVOID pV) {
	DWORD returned = 0;
	msg.pid = pid;
	msg.value = value;
	msg.pFound = pV;
	msg.valueLength = valueLength;
	BOOL success = DeviceIoControl(driverHandle, SET_VALUE, &msg, sizeof(msg), nullptr, 0, &returned, nullptr);
	if (success) {
		return success;
	}
	else {
		REPORTERROR;
		return FALSE;
	}
}

BOOL cheatDriver::startDriver() {
	DWORD returned = 0;
	BOOL success = DeviceIoControl(driverHandle, START_DRIVER, NULL, 0, nullptr, 0, &returned, nullptr);
	if (success) {
		return success;
	}
	else {
		REPORTERROR;
		return FALSE;
	}
}

BOOL cheatDriver::stopDriver() {
	DWORD returned = 0;
	BOOL success = DeviceIoControl(driverHandle, STOP_DRIVER, NULL, 0, nullptr, 0, &returned, nullptr);
	if (success) {
	}
	else {
		REPORTERROR;
		return FALSE;
	}
	CloseHandle(driverHandle);
	system("sc stop cheatDriver");
	return success;
}
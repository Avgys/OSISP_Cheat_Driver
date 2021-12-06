#include "stdafx.hpp"
#include "utils.hpp"
void setTextAlignment(HWND hwnd, int intTextAlignment)
{
	LONG_PTR s;
	LONG_PTR textalignment = GetWindowLongPtr(hwnd, GWL_STYLE);
	if (textalignment != intTextAlignment)
	{
		//delete the last text alignment
		if (intTextAlignment == 0)
		{
			s = GetWindowLongPtr(hwnd, GWL_STYLE);
			s = s & ~(SS_LEFT);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)s);
		}
		else if (intTextAlignment == 1)
		{
			
			s = GetWindowLongPtr(hwnd, GWL_STYLE);
			s = s & ~(SS_CENTER);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)s);
		}
		else if (intTextAlignment == 2)
		{
			s = GetWindowLongPtr(hwnd, GWL_STYLE);
			s = s & ~(SS_RIGHT);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)s);
		}

		textalignment = intTextAlignment;

		//put the new text alignment
		if (textalignment == 0)
		{
			s = GetWindowLongPtr(hwnd, GWL_STYLE);
			s = s | (SS_LEFT);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)s);
		}
		else if (textalignment == 1)
		{
			s = GetWindowLongPtr(hwnd, GWL_STYLE);
			s = s | (SS_CENTER);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)s);
		}
		else if (textalignment == 2)
		{
			s = GetWindowLongPtr(hwnd, GWL_STYLE);
			s = s | (SS_RIGHT);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)s);
		}
		SetWindowPos(hwnd, 0, 0, 0, 0, 0,
			SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_DRAWFRAME);
	}
}

void ReportError(const char* CallingFunction)
{
	DWORD error = GetLastError();
	LPVOID lpMsgBuf;
	DWORD bufLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);
	char buf[10];
	_ultoa_s(error, buf, 10);
	MessageBoxA(NULL, (LPCSTR)&lpMsgBuf, buf, MB_OK);
	wprintf(L"%S: Error '%s'\n", CallingFunction, (wchar_t*)lpMsgBuf);
}

HANDLE GetProcessByName(PCWSTR name)
{
	DWORD pid = 0;

	// Create toolhelp snapshot.
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			if (std::wstring(process.szExeFile) == std::wstring(name))
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}

	CloseHandle(snapshot);

	if (pid != 0)
	{
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	}

	return NULL;
}
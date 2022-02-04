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
	std::string func = { CallingFunction };
	std::wstring wfunc = { func.begin(), func.end() };
	std::wstring buf = std::to_wstring(error);
	using namespace std::string_literals;
	
	std::wstring message{ wfunc };
	MessageBoxW(NULL, message.c_str(), buf.c_str(), MB_OK);
}

DWORD GetPidByName(PCWSTR name)
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
		return pid;
	}

	return NULL;
}
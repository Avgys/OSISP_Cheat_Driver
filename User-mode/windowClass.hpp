#pragma once
#define WINAPI __stdcall

class Window
{

	
public:
	static Window* GetIntance();
	void Init();
	void setHWnd(HWND hWnd);
	void setHEdit(HWND hWnd);
	HFONT getHFont();
	HWND getHBtnFind();
	HWND getHBtnFilter();
	HWND getHInputProgram();
	HWND getHInputValue();
	HWND getHListView();
	wchar_t ProgramName[320] = { 0 };
	wchar_t Value[320] = { 0 };

private:
	Window() {
		hFont = NULL;
		hWndInputProgram = NULL;
		hWndInputValue = NULL;
		hWndEdit = NULL;
		hWndBtnFind = NULL;
		hWndBtnFilter = NULL;
		hWndPrompt = NULL;
		hbrBkgnd = NULL;
		hWndListView = NULL;
	}
	HFONT hFont;
	HWND  hWnd;
	HWND  hWndInputProgram;
	HWND  hWndInputValue;
	HWND  hWndListView;
	HWND  hWndEdit;
	HWND  hWndBtnFind;
	HWND  hWndBtnFilter;
	HWND  hWndPrompt;
	HBRUSH hbrBkgnd;

	//public:
	//	LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

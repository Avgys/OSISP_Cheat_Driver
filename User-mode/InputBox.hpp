#pragma once

#define ASPECT_RATIO_X	2
#define ASPECT_RATIO_Y	2
#define TOP_EDGE		10 * ASPECT_RATIO_Y
#define INPUTBOX_WIDTH	500 * ASPECT_RATIO_X
#define INPUTBOX_HEIGHT 150 * ASPECT_RATIO_Y
#define TEXTEDIT_HEIGHT	30 * ASPECT_RATIO_Y
#define BUTTON_HEIGHT	25 * ASPECT_RATIO_Y
#define BUTTON_WIDTH	120 * ASPECT_RATIO_X
#define FONT_HEIGHT		20 * ASPECT_RATIO_Y

#define WINAPI __stdcall



//HFONT m_hFont = NULL;
//HWND  m_hWndInputBox = NULL;
//HWND  m_hWndParent = NULL;
//HWND  m_hWndEdit = NULL;
//HWND  m_hWndOK = NULL;
//HWND  m_hWndCancel = NULL;
//HWND  m_hWndPrompt = NULL;
//wchar_t m_String[320];
//static HBRUSH hbrBkgnd = NULL;

//InputBox CreateInputBox(HINSTANCE hInstance);

//class InputBox
//{
//	static HFONT hFont;
//	static HWND  hWndInputBox;
//	static HWND  hWndParent;
//	static HWND  hWndEdit;
//	static HWND  hWndOK;
//	static HWND  hWndCancel;
//	static HWND  hWndPrompt;
//	static wchar_t String[320];
//	static HBRUSH hbrBkgnd;
//
//	LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//public:
//	static LPWSTR GetString(LPCTSTR szCaption, LPCTSTR szPrompt, LPCTSTR szDefaultText = L"");
//
//};

//#include "InputBox.hpp"
//#include "utils.hpp"
//#include "stdafx.hpp"
//
////HFONT InputBox::hFont = NULL;
////HWND  InputBox::hWndInputBox = NULL;
////HWND  InputBox::hWndParent = NULL;
////HWND  InputBox::hWndEdit = NULL;
////HWND  InputBox::hWndOK = NULL;
////HWND  InputBox::hWndCancel = NULL;
////HWND  InputBox::hWndPrompt = NULL;
////wchar_t InputBox::String[320];
////HBRUSH InputBox::hbrBkgnd = NULL;
//
//
//
//#define SOFT_BLUE RGB(206,214,240)
//
//LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	LOGFONT lfont;
//	HINSTANCE m_hInst = NULL;
//	//switch (message)
//	//{
//	//case WM_CTLCOLORSTATIC:
//	//{
//	//	/*HDC hdcStatic = (HDC)wParam;
//	//	if (hbrBkgnd == NULL)
//	//	{
//	//		hbrBkgnd = CreateSolidBrush(SOFT_BLUE);
//	//	}
//	//	SetTextColor(hdcStatic, RGB(0, 0, 0));
//	//	SetBkColor(hdcStatic, SOFT_BLUE);*/
//
//	//	return (INT_PTR)hbrBkgnd;
//	//}
//	//break;
//	//case WM_CREATE:
//	//	// Define out font
//	//	memset(&lfont, 0, sizeof(lfont));
//	//	lstrcpy(lfont.lfFaceName, FONT_NAME);
//	//	lfont.lfHeight = FONT_HEIGHT;
//	//	lfont.lfWeight = FW_NORMAL;//FW_BOLD;
//	//	lfont.lfItalic = FALSE;
//	//	lfont.lfCharSet = DEFAULT_CHARSET;
//	//	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
//	//	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
//	//	lfont.lfQuality = DEFAULT_QUALITY;
//	//	lfont.lfPitchAndFamily = DEFAULT_PITCH;
//	//	m_hFont = CreateFontIndirect(&lfont);
//	//	// 
//	//	m_hInst = GetModuleHandle(NULL);
//
//	//	// The TextEdit Control - For the text to be input
//	//	m_hWndEdit = CreateWindowEx(WS_EX_STATICEDGE,
//	//		TEXTEDIT_CLASS, L"",
//	//		WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,
//	//		5, TOP_EDGE + BUTTON_HEIGHT * 2 + 30, INPUTBOX_WIDTH - 30, TEXTEDIT_HEIGHT,
//	//		hWnd,
//	//		NULL,
//	//		m_hInst,
//	//		NULL);
//
//	//	if (m_hWndEdit == NULL)
//	//	{
//	//		REPORTERROR;
//	//		return NULL;
//	//	}
//
//	//	SetFontToControl(m_hWndEdit);
//
//	//	// The Confirm button
//	//	m_hWndOK = CreateWindowEx(WS_EX_STATICEDGE,
//	//		PUSH_BUTTON, L"Confirm",
//	//		WS_VISIBLE | WS_CHILD | WS_TABSTOP,
//	//		INPUTBOX_WIDTH - BUTTON_WIDTH - 30, TOP_EDGE, BUTTON_WIDTH, BUTTON_HEIGHT,
//	//		hWnd,
//	//		NULL,
//	//		m_hInst,
//	//		NULL);
//	//	if (m_hWndOK == NULL)
//	//	{
//	//		REPORTERROR;
//	//		return NULL;
//	//	}
//	//	// setting font
//	//	SetFontToControl(m_hWndOK);
//
//	//	// The Cancel button
//	//	m_hWndCancel = CreateWindowEx(WS_EX_STATICEDGE,
//	//		PUSH_BUTTON, L"Cancel",
//	//		WS_VISIBLE | WS_CHILD | WS_TABSTOP,
//	//		INPUTBOX_WIDTH - BUTTON_WIDTH - 30, TOP_EDGE + BUTTON_HEIGHT + 15, BUTTON_WIDTH, BUTTON_HEIGHT,
//	//		hWnd,
//	//		NULL,
//	//		m_hInst,
//	//		NULL);
//
//	//	if (m_hWndCancel == NULL)
//	//	{
//	//		REPORTERROR;
//	//		return NULL;
//	//	}
//
//	//	// setting font
//	//	SetFontToControl(m_hWndCancel);
//
//	//	// The SG_InputBox Caption Static text
//	//	m_hWndPrompt = CreateWindowEx(WS_EX_STATICEDGE,
//	//		L"static", L"",
//	//		WS_VISIBLE | WS_CHILD,
//	//		5, TOP_EDGE, INPUTBOX_WIDTH - BUTTON_WIDTH - 50, BUTTON_HEIGHT * 2 + TOP_EDGE,
//	//		hWnd,
//	//		NULL,
//	//		m_hInst,
//	//		NULL);
//
//	//	if (m_hWndPrompt == NULL)
//	//	{
//	//		REPORTERROR;
//	//		return NULL;
//	//	}
//
//	//	// setting font
//	//	SetFontToControl(m_hWndPrompt);
//	//	SetFocus(m_hWndEdit);
//
//	//	break;
//	//case WM_DESTROY:
//
//	//	DeleteObject(m_hFont);
//
//
//	//	EnableWindow(m_hWndParent, TRUE);
//	//	SetForegroundWindow(m_hWndParent);
//	//	DestroyWindow(hWnd);
//	//	PostQuitMessage(0);
//
//	//	break;
//	//case WM_COMMAND:
//	//	switch (HIWORD(wParam))
//	//	{
//
//	//	case BN_CLICKED:
//	//		if ((HWND)lParam == m_hWndOK)
//	//			PostMessage(m_hWndInputBox, WM_KEYDOWN, VK_RETURN, 0);
//	//		if ((HWND)lParam == m_hWndCancel)
//	//			PostMessage(m_hWndInputBox, WM_KEYDOWN, VK_ESCAPE, 0);
//	//		break;
//	//	}
//	//	break;
//
//	//default:
//	//	return DefWindowProc(hWnd, message, wParam, lParam);
//	//}
//	return 0;
//}
//
////InputBox CreateInputBox(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
//InputBox CreateInputBox(HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex;
//	HWND hWnd;
//	MSG msg;
//
//	InputBox* res = new InputBox;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc = WndProc;
//	wcex.cbClsExtra = 0;
//	wcex.cbWndExtra = 0;
//	wcex.hInstance = hInstance;
//	wcex.hIcon = 0;
//	wcex.hCursor = LoadCursor(0, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName = NULL;
//	wcex.lpszClassName = CLASSNAME;
//	wcex.hIconSm = 0;
//
//	if (RegisterClassEx(&wcex) == 0)
//		REPORTERROR;
//
//	m_hWndParent = hWnd;
//
//	m_hWndInputBox = CreateWindowEx(/*WS_EX_TOOLWINDOW*/WS_EX_DLGMODALFRAME,
//		CLASSNAME,
//		szCaption,
//		WS_POPUPWINDOW | WS_CAPTION | WS_TABSTOP,
//		(r.right - INPUTBOX_WIDTH) / 2, (r.bottom - INPUTBOX_HEIGHT) / 2,
//		INPUTBOX_WIDTH, INPUTBOX_HEIGHT,
//		m_hWndParent,
//		NULL,
//		NULL,
//		NULL);
//	if (m_hWndInputBox == NULL)
//	{
//		REPORTERROR;
//		return NULL;
//	}
//
//
//	RegisterClassEx(&wcex);
//
//
//	hWnd = CreateWindowEx(0, CLASSNAME, NULL, (WS_OVERLAPPEDWINDOW | WS_VISIBLE),
//		0, 0, 800, 500, 0, 0, hInstance, NULL);
//
//
//	ShowWindow(hWnd, SW_NORMAL);
//	UpdateWindow(hWnd);
//	//GetClientRect(hWnd, &windowRect);
//
//	while (GetMessage(&msg, 0, 0, 0))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//
//	return ;
//}
//
//LPWSTR CreateInputBox(LPCTSTR szCaption, LPCTSTR szPrompt, LPCTSTR szDefaultText)
//{
//	//RECT r;
//	//HWND hWnd = GetDesktopWindow();
//	//GetWindowRect(hWnd, &r);
//
//	//HINSTANCE hInst = GetModuleHandle(NULL);
//
//	//WNDCLASSEX wcex;
//
//	//if (!GetClassInfoEx(hInst, CLASSNAME, &wcex))
//	//{
//	//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	//	wcex.lpfnWndProc = WndProc;
//	//	wcex.cbClsExtra = 0;
//	//	wcex.cbWndExtra = 0;
//	//	wcex.hInstance = hInst;
//	//	wcex.hIcon = NULL;//LoadIcon(hInst, (LPCTSTR)IDI_MYINPUTBOX);
//	//	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//	//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
//	//	wcex.lpszMenuName = NULL;
//	//	wcex.lpszClassName = CLASSNAME;
//	//	wcex.hIconSm = NULL;
//
//	//	if (RegisterClassEx(&wcex) == 0)
//	//		REPORTERROR;
//	//}
//
//	//m_hWndParent = hWnd;
//
//	//m_hWndInputBox = CreateWindowEx(/*WS_EX_TOOLWINDOW*/WS_EX_DLGMODALFRAME,
//	//	CLASSNAME,
//	//	szCaption,
//	//	WS_POPUPWINDOW | WS_CAPTION | WS_TABSTOP,
//	//	(r.right - INPUTBOX_WIDTH) / 2, (r.bottom - INPUTBOX_HEIGHT) / 2,
//	//	INPUTBOX_WIDTH, INPUTBOX_HEIGHT,
//	//	m_hWndParent,
//	//	NULL,
//	//	NULL,
//	//	NULL);
//	//if (m_hWndInputBox == NULL)
//	//{
//	//	REPORTERROR;
//	//	return NULL;
//	//}
//
//	//// Set text alingment
//	//setTextAlignment(m_hWndPrompt, SS_CENTER);
//	//SetWindowText(m_hWndPrompt, szPrompt);
//	//setTextAlignment(m_hWndEdit, SS_CENTER);
//	//SetForegroundWindow(m_hWndInputBox);
//
//	//// Set default button
//	//SendMessage((HWND)m_hWndOK, BM_SETSTYLE,
//	//	(WPARAM)LOWORD(BS_DEFPUSHBUTTON), MAKELPARAM(TRUE, 0));
//	//SendMessage((HWND)m_hWndCancel, BM_SETSTYLE,
//	//	(WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
//
//	//// Set default text
//	//SendMessage(m_hWndEdit, EM_SETSEL, 0, -1);
//	//SendMessage(m_hWndEdit, EM_REPLACESEL, 0, (LPARAM)szDefaultText);
//	//SendMessage(m_hWndEdit, EM_SETSEL, 0, -1);
//	//SetFocus(m_hWndEdit);
//
//
//
//	//EnableWindow(m_hWndParent, FALSE);
//
//	//ShowWindow(m_hWndInputBox, SW_SHOW);
//	//UpdateWindow(m_hWndInputBox);
//
//	//BOOL ret = 0;
//
//	//MSG msg;
//
//	//HWND hWndFocused;
//
//	//while (GetMessage(&msg, NULL, 0, 0))
//	//{
//	//	if (msg.message == WM_KEYDOWN)
//	//	{
//	//		if (msg.wParam == VK_ESCAPE)
//	//		{
//	//			SendMessage(m_hWndInputBox, WM_DESTROY, 0, 0);
//	//			ret = 0;
//	//		}
//	//		if (msg.wParam == VK_RETURN)
//	//		{
//	//			int nCount = GetWindowTextLength(m_hWndEdit);
//	//			nCount++;
//	//			GetWindowText(m_hWndEdit, m_String, nCount);
//	//			SendMessage(m_hWndInputBox, WM_DESTROY, 0, 0);
//	//			ret = 1;
//	//		}
//	//		if (msg.wParam == VK_TAB)
//	//		{
//	//			hWndFocused = GetFocus();
//	//			if (hWndFocused == m_hWndEdit) SetFocus(m_hWndOK);
//	//			if (hWndFocused == m_hWndOK) SetFocus(m_hWndCancel);
//	//			if (hWndFocused == m_hWndCancel) SetFocus(m_hWndEdit);
//	//		}
//
//	//	}
//	//	TranslateMessage(&msg);
//	//	DispatchMessage(&msg);
//	//}
//
//	//return m_String;
//}

#include "User-mode.hpp"
#include "driverLogic.hpp"
#include "stdafx.hpp"
#include "windowClass.hpp"
#include "utils.hpp"
#include <commctrl.h>
#include <Commctrl.h>
#include "Console.h"

#define IDC_STATIC1 105


HINSTANCE hInst;
Window* wnd = Window::GetIntance();
cheatDriver* chDriver;

HWND CreateListView(HWND hwndParent, UINT uId);
void HandleWM_NOTIFY(HWND hWnd, LPARAM lParam);
WCHAR** WINAPI GetListViewItems(HWND hWndLV, int iPos);
BOOL WINAPI AddListViewItem(HWND hWndLV, int colNum, int textMaxLen, WCHAR** item);



LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hWndLV = NULL;
	PAINTSTRUCT ps;
	HDC hDC;
	LOGFONT lfont;
	static int width = 0, height = 0;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		wnd->setHWnd(hWnd);
		wnd->Init();

		if (hWnd == NULL)
		{
			return 0;
		}
		break;
	}

	case WM_NOTIFY:
		HandleWM_NOTIFY(hWnd, lParam);
		break;

	case WM_DESTROY:
		chDriver->stopDriver();
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:

			int nCount = GetWindowTextLength(wnd->getHInputProgram());
			nCount++;
			GetWindowText(wnd->getHInputProgram(), wnd->ProgramName, nCount);

			nCount = GetWindowTextLength(wnd->getHInputValue());
			nCount++;
			GetWindowText(wnd->getHInputValue(), wnd->Value, nCount);
			//size_t value = std::stoull(wnd->Value);
			unsigned int value = std::stoi(wnd->Value);
			HWND hList = wnd->getHListView();
			ListView_DeleteAllItems(hList);
			DWORD pid = GetPidByName(wnd->ProgramName);
			if (!pid)
				ReportError("Pid not found");
			else {
				if ((HWND)lParam == wnd->getHBtnFind())
				{
					if (chDriver->FindValue(pid, &value, sizeof(value))) {
						Msg msg = *chDriver->getMsg();
						WCHAR item1[COLTEXTLENGTH]{ 0 };
						WCHAR item2[COLTEXTLENGTH]{ 0 };
						WCHAR* item[COLNUM]{ item1, item2 };
						for (size_t i = 0; i < 1024; i++) {
							if (msg.addrs[i] != 0) {
								std::wstring it1 = std::to_wstring(msg.addrs[i]);
								std::wstring it0 = std::to_wstring(value);
								wcscpy_s(item[1], COLTEXTLENGTH, it1.c_str());
								wcscpy_s(item[0], COLTEXTLENGTH, it0.c_str());
								AddListViewItem(hList, COLNUM, COLTEXTLENGTH, item);
							}
						}
					}
				}

				if ((HWND)lParam == wnd->getHBtnFilter())
				{
					if (chDriver->FilterValue(pid, NULL, &value, sizeof(value))) {
						Msg msg = *chDriver->getMsg();
						WCHAR item1[COLTEXTLENGTH]{ 0 };
						WCHAR item2[COLTEXTLENGTH]{ 0 };
						WCHAR* item[COLNUM]{ item1, item2 };
						for (size_t i = 0; i < 1024; i++) {
							if (msg.addrs[i] != 0) {
								std::wstring it1 = std::to_wstring(msg.addrs[i]);
								std::wstring it0 = std::to_wstring(value);
								wcscpy_s(item[1], COLTEXTLENGTH, it1.c_str());
								wcscpy_s(item[0], COLTEXTLENGTH, it0.c_str());
								AddListViewItem(hList, COLNUM, COLTEXTLENGTH, item);
							}
						}
						//delete &console;
					}
				}
				break;
			}
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void HandleWM_NOTIFY(HWND hWnd, LPARAM lParam)
{
	NMLVDISPINFO* plvdi;

	LPNMHDR lpnmHdr;
	lpnmHdr = (LPNMHDR)lParam;

	if ((lpnmHdr->idFrom == IDC_LISTVIEW) && (lpnmHdr->code == NM_CLICK))
	{
		// Структура, содержащая полные данные сообщение от ListView
		LPNMLISTVIEW pnmLV = (LPNMLISTVIEW)lParam;
		WCHAR buf[10] = { 0 };

		// Копируем строку в буфер из ячейки ListView (pnmLV->iItem - номер строки;
		// pnmLV->iSubItem - номер столбца)
		ListView_EditLabel(lpnmHdr->hwndFrom, pnmLV->iItem);
	}

	if ((lpnmHdr->code == LVN_ENDLABELEDIT)) {
		LPNMLVDISPINFOW pdi = (LPNMLVDISPINFOW)lParam;
		LVITEM item = pdi->item;
		ListView_SetItem(lpnmHdr->hwndFrom, &item);
		wchar_t buf[COLTEXTLENGTH] = { 0 };
		ListView_GetItemText(lpnmHdr->hwndFrom, item.iItem, 1, buf, COLTEXTLENGTH);

		int nCount = GetWindowTextLength(wnd->getHInputProgram());
		nCount++;
		GetWindowText(wnd->getHInputProgram(), wnd->ProgramName, nCount);

		nCount = GetWindowTextLength(wnd->getHInputValue());
		nCount++;
		GetWindowText(wnd->getHInputValue(), wnd->Value, nCount);

		//size_t value = std::stoull(item.pszText);
		unsigned int value = std::stoi(item.pszText);
		size_t addr = std::stoull(buf);
		HWND hList = wnd->getHListView();
		DWORD pid = GetPidByName(wnd->ProgramName);
		if (pid)
			chDriver->SetValue(pid, &value, sizeof(value), (void*)addr);
		else
			ReportError("Pid not found");
	}
	return;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;
	hInst = hInstance;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WndClass;
	wcex.hIconSm = 0;
	RegisterClassEx(&wcex);

	hWnd = CreateWindowEx(0, WndClass, WndClass, (WS_OVERLAPPEDWINDOW | WS_VISIBLE),
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	chDriver = new cheatDriver();

	//INT_PTR success = (INT_PTR)ShellExecute(hWnd, L"helper.exe", L"sc start cheatDriver", NULL, NULL, 0);
	//if (success > 32) {
	//	if (chDriver->AttachDriver()) {
	//		//MessageBox(hWnd, L"text", L"test", MB_OK);			
	//	}
	//}
	//else {
	//	REPORTERROR;
	//}

	if (chDriver->AttachDriver()) {
		//MessageBox(hWnd, L"text", L"test", MB_OK);			
	}

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

BOOL InitListViewColumns(HWND hWndListView)
{
	WCHAR szText[256] = { 0 };     // Temporary buffer.

	WCHAR headers[][COLTEXTLENGTH] = { L"Value", L"Address" };
	LVCOLUMN lvc;
	int iCol;

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	const int colNum = 2;

	// Add the columns.
	for (iCol = 0; iCol < colNum; iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = headers[iCol];
		lvc.cx = (WINDOW_WIDTH - 100) / 2;               // Width of column in pixels.

		if (iCol < 2)
			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
		else
			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.		

		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
			return FALSE;

		lvc.cchTextMax = COLTEXTLENGTH;

	}

	return TRUE;
}

BOOL WINAPI AddListViewItem(HWND hWndLV, int colNum, int textMaxLen, WCHAR** item)
{
	int iLastIndex = ListView_GetItemCount(hWndLV);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = textMaxLen;
	lvi.iItem = iLastIndex;
	lvi.pszText = item[0];
	lvi.iSubItem = 0;
	if (ListView_InsertItem(hWndLV, &lvi) == -1)
		return FALSE;
	for (int i = 1; i < colNum; i++)
		ListView_SetItemText(hWndLV, iLastIndex, i, item[i]);

	return TRUE;
}

BOOL WINAPI GetListViewItem(HWND hWndLV, int iItem, WCHAR** buf)
{
	for (int subItem = 0; subItem < COLNUM; subItem++)
		ListView_GetItemText(hWndLV, iItem, subItem, buf[subItem], COLTEXTLENGTH);
	return TRUE;
}

BOOL WINAPI RemoveListViewItems(HWND hWndLV)
{
	for (int i = 0; i < ListView_GetItemCount(hWndLV); i++) {
		ListView_DeleteItem(hWndLV, i);
	}
	return FALSE;
}

HWND CreateListView(HWND hwndParent, UINT uId)
{
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	// Create the list-view window in report view with label editing enabled.
	HWND hWndListView = CreateWindowEx(0, WC_LISTVIEW,
		L"",
		WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
		50, WINDOW_HEIGHT / 2 - 110,
		WINDOW_WIDTH - 100,
		WINDOW_HEIGHT / 2,
		hwndParent,
		(HMENU)uId,
		hInst,
		NULL);

	if (!InitListViewColumns(hWndListView)) {
		REPORTERROR;
	};

	return (hWndListView);
}

void Window::Init() {
	//Define out font
	LOGFONT lfont;
	memset(&lfont, 0, sizeof(lfont));
	lstrcpy(lfont.lfFaceName, FONT_NAME);
	lfont.lfHeight = FONT_HEIGHT;
	lfont.lfWeight = FW_NORMAL;//FW_BOLD;
	lfont.lfItalic = FALSE;
	lfont.lfCharSet = DEFAULT_CHARSET;
	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfont.lfQuality = DEFAULT_QUALITY;
	lfont.lfPitchAndFamily = DEFAULT_PITCH;
	hFont = CreateFontIndirect(&lfont);

	if ((hWndListView = CreateListView(hWnd, IDC_LISTVIEW)) == NULL)
		MessageBox(NULL, L"Невозможно создать элемент ListView", L"Ошибка", MB_OK);
	ShowWindow(hWndListView, SW_SHOWDEFAULT);

	//Program name input edit
	hWndInputProgram = CreateWindowEx(WS_EX_STATICEDGE,
		TEXTEDIT_CLASS, L"",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,
		20, TOP_EDGE + BUTTON_HEIGHT * 2 + 30, TEXTEDIT_WIDTH, TEXTEDIT_HEIGHT,
		hWnd,
		NULL,
		hInst,
		NULL);

	if (hWndInputProgram == NULL)
	{
		REPORTERROR;
		return;
	}
	SetFontToControl(hWndInputProgram, wnd->getHFont());

	hWndInputValue = CreateWindowEx(WS_EX_STATICEDGE,
		TEXTEDIT_CLASS, L"",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,
		WINDOW_WIDTH / 2, TOP_EDGE + BUTTON_HEIGHT * 2 + 30, TEXTEDIT_WIDTH, TEXTEDIT_HEIGHT,
		hWnd,
		NULL,
		hInst,
		NULL);

	if (hWndInputValue == NULL)
	{
		REPORTERROR;
		return;
	}
	SetFontToControl(hWndInputValue, wnd->getHFont());


	// The Find button
	hWndBtnFind = CreateWindowEx(WS_EX_STATICEDGE,
		PUSH_BUTTON, L"Find",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP,
		WINDOW_WIDTH - BUTTON_WIDTH - 30, TOP_EDGE, BUTTON_WIDTH, BUTTON_HEIGHT,
		hWnd,
		NULL,
		hInst,
		NULL);

	if (hWndBtnFind == NULL)
	{
		REPORTERROR;
		return;
	}

	// setting font
	SetFontToControl(hWndBtnFind, wnd->getHFont());

	// The Filter button
	hWndBtnFilter = CreateWindowEx(WS_EX_STATICEDGE,
		PUSH_BUTTON, L"Filter",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP,
		WINDOW_WIDTH - BUTTON_WIDTH - 30, TOP_EDGE + BUTTON_HEIGHT + 15, BUTTON_WIDTH, BUTTON_HEIGHT,
		hWnd,
		NULL,
		hInst,
		NULL);

	if (hWndBtnFilter == NULL)
	{
		REPORTERROR;
		return;
	}

	// setting font
	SetFontToControl(hWndBtnFilter, wnd->getHFont());
}
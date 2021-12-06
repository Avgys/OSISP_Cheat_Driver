#include "User-mode.hpp"
#include "stdafx.hpp"
#include "windowClass.hpp"
#include "utils.hpp"
#include <commctrl.h>

#define COLTEXTLENGTH 14
#define COLNUM 2


HINSTANCE hInst;
Window* wnd = Window::GetIntance();

HWND CreateListView(HWND hwndParent, UINT uId);
void HandleWM_NOTIFY(HWND hWnd, LPARAM lParam);

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
		if ((hWndLV = CreateListView(hWnd, IDC_LISTVIEW)) == NULL)
			MessageBox(NULL, L"Невозможно создать элемент ListView", L"Ошибка", MB_OK);
		ShowWindow(hWndLV, SW_SHOWDEFAULT);

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
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{

		case BN_CLICKED:
			if ((HWND)lParam == wnd->getHBtnFind())
			{
				int nCount = GetWindowTextLength(wnd->getHInputProgram());
				nCount++;
				GetWindowText(wnd->getHInputProgram(), wnd->ProgramName, nCount);

				nCount = GetWindowTextLength(wnd->getHInputValue());
				nCount++;
				GetWindowText(wnd->getHInputValue(), wnd->Value, nCount);
			}

			if ((HWND)lParam == wnd->getHBtnFilter())
			{
				int nCount = GetWindowTextLength(wnd->getHInputProgram());
				nCount++;
				GetWindowText(wnd->getHInputProgram(), wnd->ProgramName, nCount);

				nCount = GetWindowTextLength(wnd->getHInputValue());
				nCount++;
				GetWindowText(wnd->getHInputValue(), wnd->Value, nCount);
			}
			break;
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
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

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void HandleWM_NOTIFY(HWND hWnd, LPARAM lParam)
{
	NMLVDISPINFO* plvdi;

	switch (((LPNMHDR)lParam)->code)
	{
	case LVN_GETDISPINFO:

		plvdi = (NMLVDISPINFO*)lParam;

		switch (plvdi->item.iSubItem)
		{
		case 0:
			//plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szKind;
			break;

		case 1:
			//plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szBreed;
			break;

		case 2:
			//plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szPrice;
			break;

		default:
			break;
		}

		break;
	}

	return;
}

BOOL InitListViewColumns(HWND hWndListView)
{
	WCHAR szText[256] = { 0 };     // Temporary buffer.

	WCHAR headers[][COLTEXTLENGTH] = { L"header1", L"header2"};
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
		lvc.cx = (WINDOW_WIDTH - 100)/2;               // Width of column in pixels.

		if (iCol < 2)
			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
		else
			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

		// Load the names of the column headings from the string resources.
		/*LoadString(hInst,
			iCol,
			szText,
			sizeof(szText) / sizeof(szText[0]));*/

		// Insert the columns into the list view.
		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
			return FALSE;

		lvc.cchTextMax = COLTEXTLENGTH;

		/*for (int i = 0; i < colNum; i++)
		{
			lvc.pszText = header[i];
			if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
				return FALSE;
		}*/
	}

	return TRUE;
}

BOOL WINAPI AddListViewItems(HWND hWndLV, int colNum, int textMaxLen, WCHAR** item)
{
	int iLastIndex = ListView_GetItemCount(hWndLV);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = textMaxLen;
	lvi.iItem = iLastIndex;

	//lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.pszText = item[0];
	lvi.iSubItem = 0;
	if (ListView_InsertItem(hWndLV, &lvi) == -1)
		return FALSE;
	for (int i = 1; i < colNum; i++)
		ListView_SetItemText(hWndLV, iLastIndex, i, item[i]);

	return TRUE;
}

WCHAR** WINAPI GetListViewItems(HWND hWndLV, int iPos)
{

	for (int i = 0)
		ListView_GetItemText(
		hwndLV,
		i,
		iSubItem_,
		pszText_,
		cchTextMax_
	);
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = textMaxLen;
	lvi.iItem = iLastIndex;

	//lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.pszText = item[0];
	lvi.iSubItem = 0;
	if (ListView_InsertItem(hWndLV, &lvi) == -1)
		return FALSE;
	for (int i = 1; i < colNum; i++)
		ListView_SetItemText(hWndLV, iLastIndex, i, item[i]);

	return TRUE;
}



//BOOL InsertListViewItems(HWND hWndListView, int cItems)
//{
//	LVITEM lvI;
//
//	// Initialize LVITEM members that are common to all items.
//	lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
//	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
//	lvI.stateMask = 0;
//	lvI.iSubItem = 0;
//	lvI.state = 0;
//
//	cItems = 1;
//
//	// Initialize LVITEM members that are different for each item.
//	for (int index = 0; index < cItems; index++)
//	{
//		lvI.iItem = index;
//		lvI.iImage = index;
//
//		// Insert items into the list.
//		if (ListView_InsertItem(hWndListView, &lvI) == -1)
//			return FALSE;
//	}
//
//	return TRUE;
//}

HWND CreateListView(HWND hwndParent, UINT uId)
{
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	//RECT rcClient;                       // The parent window's client area.

	//GetClientRect(hwndParent, &rcClient);

	// Create the list-view window in report view with label editing enabled.
	HWND hWndListView = CreateWindowEx(0, WC_LISTVIEW,
		L"",
		WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
		50, WINDOW_HEIGHT / 2 - 110,
		WINDOW_WIDTH - 100,
		WINDOW_HEIGHT / 2 - 50,
		hwndParent,
		(HMENU)uId,
		hInst,
		NULL);

	if (!InitListViewColumns(hWndListView)) {
		REPORTERROR;
	};

	//if (!InsertListViewItems(hWndListView, 5)) {
	//	REPORTERROR;
	//};

	WCHAR item1[]{ L"item1" };
	WCHAR item2[]{ L"item2" };
	WCHAR* items[COLNUM]{ item1, item2 };

	if (!AddListViewItems(hWndListView, COLNUM, COLTEXTLENGTH, items)) {
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
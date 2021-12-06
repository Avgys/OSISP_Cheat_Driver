#pragma once


#define ASPECT_RATIO_X	1
#define ASPECT_RATIO_Y	1
#define TOP_EDGE		10 * ASPECT_RATIO_Y
#define WINDOW_WIDTH	800 * ASPECT_RATIO_X
#define WINDOW_HEIGHT	500 * ASPECT_RATIO_Y
#define TEXTEDIT_HEIGHT	30 * ASPECT_RATIO_Y
#define TEXTEDIT_WIDTH	(WINDOW_WIDTH / 2 - 50)
#define BUTTON_HEIGHT	25 * ASPECT_RATIO_Y
#define BUTTON_WIDTH	120 * ASPECT_RATIO_X
#define FONT_HEIGHT		20 * ASPECT_RATIO_Y

//#define InputBoxClass				L"Inputbox"
#define WndClass					L"CheatMoney"
#define PUSH_BUTTON					L"Button"
#define FONT_NAME					L"Times"
#define TEXTEDIT_CLASS				L"edit"
#define SetFontToControl(n,__hFont)			SendMessage((n), WM_SETFONT, (WPARAM)__hFont, 0);

#define IDC_LISTVIEW                1111
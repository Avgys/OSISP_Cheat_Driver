#include "stdafx.hpp"
#include "windowClass.hpp"


Window* Window::GetIntance() {
	static Window window;
	return &window;
}

void Window::setHWnd(HWND _hWnd) {
	this->hWnd = _hWnd;
}

void Window::setHEdit(HWND _hEdit) {
	this->hWndEdit = _hEdit;
}

HFONT Window::getHFont(){
	return this->hFont;
}

HWND Window::getHBtnFind(){
	return this->hWndBtnFind;
}

HWND Window::getHListView(){
	return this->hWndListView;
}

HWND Window::getHBtnFilter(){
	return this->hWndBtnFilter;
}

HWND Window::getHInputProgram(){
	return this->hWndInputProgram;
}

HWND Window::getHInputValue(){
	return this->hWndInputValue;
}
#pragma once
#include <windows.h>
#include<WinUser.h>
#include<vector>

BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
std::vector<HWND> buttons;
HWND diablo;

oid ReloadMenuButtons() {
	diablo = FindWindow("SDlgDialog", NULL);
	buttons.clear();
	EnumChildWindows(diablo, EnumWindowsProc, NULL);
	cout << "VECTOR SIZE: " << buttons.size() << endl;
}

void PressEnter() {
	PostMessage(diablo, WM_KEYDOWN, VK_RETURN, NULL);
	ReloadMenuButtons();
}


void SelectMenuButton(int i) {
	if (i >= buttons.size()) {
		cout << "ERROR: selectmenubutton index " << i << " exceeds vector size " << buttons.size() << endl;
		return;
	}
	if (i == 0) {
		PostMessage(buttons[1], WM_KEYDOWN, VK_UP, NULL);
	}
	else {
		PostMessage(buttons[i - 1], WM_KEYDOWN, VK_DOWN, NULL);
	}
}

bool setDiablo() {
	diablo = FindWindow("SDlgDialog", NULL);
	return diablo;
}


#pragma once

#include "window.h"

class HIDWindow : public Window
{
public:
	HIDWindow();
	~HIDWindow();

protected:
	LRESULT window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
};


#pragma once

#include "win32_hid_input_device.h"

class Win32HidWindow
{
public:
	Win32HidWindow();
	~Win32HidWindow();

	std::vector<std::shared_ptr<Win32HidInputDevice> > devices;

private:
	Win32HidWindow(const Win32HidWindow &);
	Win32HidWindow &operator=(const Win32HidWindow &);

	LRESULT window_proc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK static_window_proc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam);

	HWND window_handle;
};

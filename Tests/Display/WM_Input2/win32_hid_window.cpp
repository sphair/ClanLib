
#include "precomp.h"
#include "win32_hid_window.h"

using namespace clan;

class DisplayMessageQueue
{
public:
	DisplayMessageQueue() {};
	~DisplayMessageQueue() {};

	static DisplayMessageQueue message_queue;

public:
	int wait(const std::vector<Event> &events, int timeout);
};
DisplayMessageQueue message_queue;

int DisplayMessageQueue::wait(const std::vector<Event> &events, int timeout)
{
	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	BOOL result = PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
	if (result)
	{
		DispatchMessage(&msg);
		return 0;
	}

	// Ignoring the other events!

	return -1;

}



Win32HidWindow::Win32HidWindow()
: window_handle(0)
{
	devices = Win32HidInputDevice::create_devices();

	WNDCLASSEX class_desc = { 0 };
	class_desc.cbSize = sizeof(WNDCLASSEX);
	class_desc.style = 0;
	class_desc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	class_desc.lpfnWndProc = &Win32HidWindow::static_window_proc;
	class_desc.lpszClassName = L"Win32HidWindow";
	ATOM class_atom = RegisterClassEx(&class_desc);

	DWORD ex_style = 0;
	DWORD style = WS_POPUPWINDOW;
	HWND window_handle = CreateWindowEx(ex_style, L"Win32HidWindow", L"Win32HidWindow", style, CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, 0, 0, (HINSTANCE) GetModuleHandle(0), this);
	if (window_handle == 0)
		throw Exception("CreateWindowEx failed");

	Win32HidInputDevice::register_raw_input_devices(window_handle);

	KeepAlive::func_event_wait().set(&message_queue, &DisplayMessageQueue::wait);

}

Win32HidWindow::~Win32HidWindow()
{
	DestroyWindow(window_handle);
}

LRESULT Win32HidWindow::window_proc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	if (message_id == WM_INPUT)
	{
		HRAWINPUT handle = (HRAWINPUT)lparam;

		DataBuffer rawinput_data = Win32HidInputDevice::get_rawinput_data(handle);
		RAWINPUT *rawinput = rawinput_data.get_data<RAWINPUT>();

		for (size_t i = 0; i < devices.size(); i++)
		{
			devices[i]->update(rawinput);
		}

		return DefWindowProc(window_handle, message_id, wparam, lparam);
	}
	else
	{
		return DefWindowProc(window_handle, message_id, wparam, lparam);
	}
}

LRESULT Win32HidWindow::static_window_proc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	Win32HidWindow *self = 0;
	if (message_id == WM_CREATE)
	{
		LPCREATESTRUCT create_struct = (LPCREATESTRUCT) lparam;
		self = (Win32HidWindow *)create_struct->lpCreateParams;
		SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR) self);
	}
	else
	{
		self = (Win32HidWindow *)GetWindowLongPtr(window_handle, GWLP_USERDATA);
	}
	return self->window_proc(window_handle, message_id, wparam, lparam);
}

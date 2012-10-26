
#include "precomp.h"
#include "window.h"

Window::Window()
: handle(0)
{
	create_window_class();
	handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, TEXT("WindowClass"), TEXT("My Window"), WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0, 1024, 768, 0, 0, GetModuleHandle(0), this);
	if (handle == 0)
		throw Exception("Failed to create window");
}

Window::~Window()
{
	if (handle)
		DestroyWindow(handle);
}

int Window::exec()
{
	int quit_msg = 0;
	while (true)
	{
		MSG msg;
		memset(&msg, 0, sizeof(MSG));
		BOOL result = GetMessage(&msg, 0, 0, 0);
		if (result == -1)
			break;
		if (result == 0) // WM_QUIT message
		{
			quit_msg = (int) msg.wParam;
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return quit_msg;
}

void Window::create_window_class()
{
	if (class_atom == 0)
	{
		WNDCLASSEX class_ex;
		memset(&class_ex, 0, sizeof(WNDCLASSEX));
		class_ex.cbSize = sizeof(WNDCLASSEX);
		class_ex.style = CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW;
		class_ex.lpfnWndProc = &Window::static_window_proc;
		class_ex.hInstance = GetModuleHandle(0);
		class_ex.lpszClassName = TEXT("WindowClass");
		class_ex.hIcon = 0;
		class_atom = RegisterClassEx(&class_ex);
		if (class_atom == 0)
			throw Exception("Failed to register window class");
	}
}

LRESULT Window::window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT Window::static_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window *self = 0;
	if (uMsg == WM_CREATE)
	{
		LPCREATESTRUCT create_struct = (LPCREATESTRUCT) lParam;
		self = (Window *) create_struct->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) self);
	}
	else
	{
		self = (Window *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	if (self)
	{
		LRESULT result = self->window_proc(hwnd, uMsg, wParam, lParam);
		if (uMsg == WM_DESTROY)
			self->handle = 0;
		return result;
	}
	else
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

ATOM Window::class_atom = 0;

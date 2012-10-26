
#pragma once

class Window
{
public:
	Window();
	~Window();

	HWND get_handle() const { return handle; }

	static int exec();

protected:
	virtual LRESULT window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	static void create_window_class();
	static LRESULT CALLBACK static_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND handle;
	static ATOM class_atom;
};


#pragma once

class NotificationAreaIcon
{
public:
	NotificationAreaIcon();
	~NotificationAreaIcon();

public:
	void show(int icon_resource_id);
	void hide();
	void update_message(const CL_StringRef &message);
	void start_flash();
	void end_flash();

	CL_Callback_v0 cb_lbuttonup;
	CL_Callback_v0 cb_lbuttondblclk;
	CL_Callback_v0 cb_rbuttonup;
	CL_Callback_v0 cb_rbuttondblclk;

private:
#ifdef WIN32
	LRESULT on_tray_message(WPARAM wParam, LPARAM lParam);
	void on_flash_timer();
	void update_flash_icon();

	HWND handle;
	bool visible;
	CL_String tip_string;
	CL_Timer flash_timer;
	bool flash_show_icon;
	HICON icon;

	static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static ATOM class_atom;
#endif
};

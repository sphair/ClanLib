
#include "precomp.h"
#include "notification_area_icon.h"

#ifdef WIN32
#include <tchar.h>

NotificationAreaIcon::NotificationAreaIcon()
: handle(0), visible(false), flash_show_icon(true), icon(0)
{
	flash_timer.func_expired().set(this, &NotificationAreaIcon::on_flash_timer);

	if (class_atom == 0)
	{
		WNDCLASSEX wndclassex;
		memset(&wndclassex, 0, sizeof(WNDCLASSEX));
		wndclassex.cbSize = sizeof(WNDCLASSEX);
		wndclassex.lpfnWndProc = &NotificationAreaIcon::wnd_proc;
		wndclassex.hInstance = GetModuleHandle(0);
		wndclassex.lpszClassName = TEXT("TrayWindow");
		class_atom = RegisterClassEx(&wndclassex);
	}

	handle = CreateWindowEx(0, (LPCTSTR) class_atom, TEXT("Notification Window"), WS_POPUP, 0, 0, 0, 0, 0, 0, GetModuleHandle(0), this);
}

NotificationAreaIcon::~NotificationAreaIcon()
{
	hide();
	DestroyWindow(handle);
}

void NotificationAreaIcon::show(int icon_resource_id)
{
	if (!visible)
	{
		icon = (HICON) LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(icon_resource_id), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

		NOTIFYICONDATA iconData;
		memset(&iconData, 0, sizeof(NOTIFYICONDATA));
		iconData.cbSize = sizeof(NOTIFYICONDATA);
		iconData.hWnd = handle;
		iconData.hIcon = icon;
		iconData.uID = 0;
		iconData.uCallbackMessage = WM_USER;
		iconData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		_tcscpy(
			iconData.szTip,
			tip_string.substr(0, 63).c_str());
		Shell_NotifyIcon(NIM_ADD, &iconData);
		visible = true;
	}
}

void NotificationAreaIcon::hide()
{
	if (visible)
	{
		NOTIFYICONDATA iconData;
		memset(&iconData, 0, sizeof(NOTIFYICONDATA));
		iconData.cbSize = sizeof(NOTIFYICONDATA);
		iconData.hWnd = handle;
		iconData.uID = 0;
		Shell_NotifyIcon(NIM_DELETE, &iconData);
		visible = false;

		DestroyIcon(icon);
		icon = 0;
	}
}

void NotificationAreaIcon::update_message(const CL_StringRef &message)
{
	if (message != tip_string)
	{
		tip_string = message;

		NOTIFYICONDATA iconData;
		memset(&iconData, 0, sizeof(NOTIFYICONDATA));
		iconData.cbSize = sizeof(NOTIFYICONDATA);
		iconData.hWnd = handle;
		iconData.uCallbackMessage = WM_USER;
		iconData.uFlags = NIF_TIP | NIF_MESSAGE;
		_tcscpy(
			iconData.szTip,
			message.substr(0, 63).c_str());
		Shell_NotifyIcon(NIM_MODIFY, &iconData);
	}
}

void NotificationAreaIcon::start_flash()
{
	if (visible)
	{
		flash_timer.start(750);
		flash_show_icon = true;
		update_flash_icon();
	}
}

void NotificationAreaIcon::end_flash()
{
	flash_timer.stop();
	flash_show_icon = true;
	update_flash_icon();
}

void NotificationAreaIcon::on_flash_timer()
{
	flash_show_icon = !flash_show_icon;
	update_flash_icon();
}

void NotificationAreaIcon::update_flash_icon()
{
	if (visible)
	{
		NOTIFYICONDATA iconData;
		memset(&iconData, 0, sizeof(NOTIFYICONDATA));
		iconData.cbSize = sizeof(NOTIFYICONDATA);
		iconData.hWnd = handle;
		iconData.uFlags = NIF_ICON;
		iconData.hIcon = flash_show_icon ? icon : 0;
		Shell_NotifyIcon(NIM_MODIFY, &iconData);
	}
}

LRESULT NotificationAreaIcon::on_tray_message(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_RBUTTONDOWN:
		return TRUE;

	case WM_RBUTTONUP:
/*		{
			POINT cursorPos;
			GetCursorPos(&cursorPos);
			on_context_menu(handle, cursorPos.x, cursorPos.y);
			// PostMessage(handle, WM_NULL, 0, 0); // KB135788
		}*/
		if (!cb_rbuttonup.is_null())
			cb_rbuttonup.invoke();
		return TRUE;
	case WM_LBUTTONUP:
		if (!cb_lbuttonup.is_null())
			cb_lbuttonup.invoke();
		return TRUE;
		
	case WM_LBUTTONDBLCLK:
		if (!cb_lbuttondblclk.is_null())
			cb_lbuttondblclk.invoke();
		return TRUE;

	case WM_RBUTTONDBLCLK:
		if (!cb_rbuttondblclk.is_null())
			cb_rbuttondblclk.invoke();
		return TRUE;
	}
	return FALSE;
}

LRESULT NotificationAreaIcon::wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	#ifdef _MSC_VER
		#pragma warning(disable: 4244) // 'argument' : conversion from 'LONG_PTR' to 'LONG', possible loss of data
		#pragma warning(disable: 4312) // 'type cast' : conversion from 'LONG' to 'Tray *' of greater size
	#endif
	NotificationAreaIcon *tray = 0;
	switch (uMsg)
	{
	case WM_CREATE:
		{
			CREATESTRUCT *createstruct = (CREATESTRUCT *) lParam;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) createstruct->lpCreateParams);
		}
		return 0;

	case WM_USER:
		tray = (NotificationAreaIcon *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
		return tray->on_tray_message(wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

ATOM NotificationAreaIcon::class_atom = 0;

#else

NotificationAreaIcon::NotificationAreaIcon()
{
}

NotificationAreaIcon::~NotificationAreaIcon()
{
}

void NotificationAreaIcon::show(int icon_resource_id)
{
}

void NotificationAreaIcon::hide()
{
}

void NotificationAreaIcon::update_message(const CL_StringRef &message)
{
}

void NotificationAreaIcon::start_flash()
{
}

void NotificationAreaIcon::end_flash()
{
}

#endif

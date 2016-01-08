/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/ErrorReporting/exception_dialog.h"
#include "API/Core/Text/string_help.h"
#include "exception_dialog_impl.h"
#include "API/Core/Math/cl_math.h"

#ifdef WIN32
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

namespace clan
{
	void ExceptionDialog::show(const std::exception_ptr &exception)
	{
		try
		{
			std::rethrow_exception(exception);
		}
		catch (Exception &e)
		{
			ExceptionDialog_Impl::show(e.get_message_and_stack_trace());
		}
		catch (std::exception &e)
		{
			ExceptionDialog_Impl::show(e.what());
		}
		catch (...)
		{
			ExceptionDialog_Impl::show("Unknown exception!");
		}
	}

	#ifdef WIN32

	#define WM_OK_BUTTON_CLICK (WM_USER + 78)

	void ExceptionDialog_Impl::show(const std::string &message_and_stack_trace)
	{
		INITCOMMONCONTROLSEX desc = { 0 };
		desc.dwSize = sizeof(INITCOMMONCONTROLSEX);
		desc.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES | ICC_COOL_CLASSES;
		InitCommonControlsEx(&desc);

		ExceptionDialog_Impl dlg(message_and_stack_trace, 0);
		while (true)
		{
			MSG msg;
			int result = GetMessage(&msg, 0, 0, 0);
			if (result <= 0)
			{
				break;
			}
			else if (msg.message == WM_QUIT)
			{
				PostQuitMessage(msg.wParam);
				break;
			}
			else if (msg.message == WM_OK_BUTTON_CLICK || (msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN))
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//MessageBox(0, StringHelp::utf8_to_ucs2(e.get_message_and_stack_trace()).c_str(), L"Unhandled Exception", MB_OK|MB_ICONERROR);
	}

	ExceptionDialog_Impl::ExceptionDialog_Impl(const std::string &message_and_stack_trace, HWND owner)
	: window_handle(0), frame(0), text_label(0), ok_button(0)
	{
		if (owner == 0)
			owner = GetDesktopWindow();

		HDC screen_dc = GetDC(0);
		int ppi = GetDeviceCaps(screen_dc, LOGPIXELSX);
		ReleaseDC(0, screen_dc);

		WNDCLASSEX class_desc = { 0 };
		class_desc.cbSize = sizeof(WNDCLASSEX);
		class_desc.style = 0;
		class_desc.hbrBackground = CreateSolidBrush(RGB(240,240,240));
		class_desc.lpfnWndProc = &ExceptionDialog_Impl::static_window_proc;
		class_desc.lpszClassName = L"ExceptionDialog";
		ATOM class_atom = RegisterClassEx(&class_desc);

		HINSTANCE instance = (HINSTANCE)GetModuleHandle(0);

		DWORD ex_style = WS_EX_DLGMODALFRAME;
		DWORD style = WS_POPUPWINDOW|WS_CAPTION;
		window_handle = CreateWindowEx(ex_style, L"ExceptionDialog", L"Unhandled Exception", style, CW_USEDEFAULT, CW_USEDEFAULT, 320 * ppi / 96, 240 * ppi / 96, 0, 0, instance, this);
		if (window_handle == 0)
			throw Exception("CreateWindowEx failed");

		std::wstring text = StringHelp::utf8_to_ucs2(message_and_stack_trace);

		frame = CreateWindowEx(0, L"STATIC", L"", WS_VISIBLE | WS_CHILD | SS_LEFT | SS_EDITCONTROL | SS_NOPREFIX, 0, 0, 100 * ppi / 96, 50 * ppi / 96, window_handle, 0, instance, 0);
		text_label = CreateWindowEx(0, L"STATIC", text.c_str(), WS_VISIBLE | WS_CHILD | SS_LEFT | SS_EDITCONTROL | SS_NOPREFIX | SS_NOTIFY, 0, 0, 100 * ppi / 96, 50 * ppi / 96, window_handle, 0, instance, 0);
		ok_button = CreateWindowEx(0, L"BUTTON", L"OK", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 50 * ppi / 96, 10 * ppi / 96, window_handle, 0, instance, 0);

		int point_size = 9;
		font = CreateFont(-(point_size * 96 + 36) / 72 * ppi / 96, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Segoe UI");
		SendMessage(frame, WM_SETFONT, (WPARAM)font, 0);
		SendMessage(text_label, WM_SETFONT, (WPARAM)font, 0);
		//SendMessage(ok_button, WM_SETFONT, (WPARAM)font, 0);

		RECT rect = { 0,0,0,0 };
		HDC dc = GetDC(text_label);
		int old_map_mode = SetMapMode(dc, MM_TEXT);
		HGDIOBJ old_font = SelectObject(dc, font);
		DrawText(dc, text.data(), text.length(), &rect, DT_CALCRECT|DT_LEFT|DT_TOP|DT_NOPREFIX);
		SelectObject(dc, old_font);
		SetMapMode(dc, old_map_mode);
		ReleaseDC(text_label, dc);

		RECT owner_box = { 0,0,0,0 };
		GetWindowRect(owner, &owner_box);

		int width = clan::min(rect.right, (long int)1350 * ppi / 96) + 50 * ppi / 96;
		int height = clan::min(rect.bottom, (long int)700 * ppi / 96) + 100 * ppi / 96;
		SetWindowPos(window_handle, 0, (owner_box.left + owner_box.right - width) / 2, (owner_box.top + owner_box.bottom - height) / 2, width, height, SWP_NOZORDER);

		RECT client_box = { 0,0,0,0 };
		GetClientRect(window_handle, &client_box);

		int button_width = 88 * ppi / 96;
		int button_height = 26 * ppi / 96;
		SetWindowPos(ok_button, 0, client_box.right - button_width - 11 * ppi / 96, client_box.bottom - 48 * ppi / 96 + button_height / 2, button_width, button_height, SWP_NOZORDER);

		int text_bottom = client_box.bottom - 48 * ppi / 96;
		SetWindowPos(frame, 0, 0, 0, client_box.right, text_bottom, SWP_NOZORDER);
		SetWindowPos(text_label, 0, 11 * ppi / 96, 11 * ppi / 96, client_box.right - 22 * ppi / 96, text_bottom - (11 + 7) * ppi / 96, SWP_NOZORDER);

		ShowWindow(window_handle, SW_SHOW);
		SetFocus(ok_button);
	}

	ExceptionDialog_Impl::~ExceptionDialog_Impl()
	{
		DestroyWindow(window_handle);
		DeleteObject(font);
	}

	LRESULT ExceptionDialog_Impl::window_proc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
	{
		if ((message_id == WM_COMMAND && HIWORD(wparam) == BN_CLICKED) || message_id == WM_CLOSE)
		{
			if (lparam != (LPARAM)text_label)	// Allow double clicks to copy text to clipboard
				PostMessage(window_handle, WM_OK_BUTTON_CLICK, 0, 0);
			return 0;
		}
		else if (message_id == WM_CTLCOLORSTATIC && lparam == (LPARAM)frame)
		{
			return (LRESULT)GetStockObject(WHITE_BRUSH);
		}
		else if (message_id == WM_CTLCOLORSTATIC && lparam == (LPARAM)text_label)
		{
			return (LRESULT)GetStockObject(WHITE_BRUSH);
		}
		else
		{
			return DefWindowProc(window_handle, message_id, wparam, lparam);
		}
	}

	LRESULT ExceptionDialog_Impl::static_window_proc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
	{
		ExceptionDialog_Impl *self = 0;
		if (message_id == WM_CREATE)
		{
			LPCREATESTRUCT create_struct = (LPCREATESTRUCT) lparam;
			self = (ExceptionDialog_Impl *)create_struct->lpCreateParams;
			SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR) self);
		}
		else
		{
			self = (ExceptionDialog_Impl *)GetWindowLongPtr(window_handle, GWLP_USERDATA);
		}
		return self->window_proc(window_handle, message_id, wparam, lparam);
	}

	#else

	void ExceptionDialog_Impl::show(const std::string &message_and_stack_trace)
	{
	}

	#endif
}

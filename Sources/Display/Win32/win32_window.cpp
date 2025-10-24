
/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/Core/Text/logger.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/display.h"
#include "API/Display/2D/color.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/keys.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/ImageProviders/png_provider.h"
#include "API/Display/screen_info.h"
#include "win32_window.h"
#include "input_device_provider_win32keyboard.h"
#include "input_device_provider_win32mouse.h"
#include "input_device_provider_win32tablet.h"
#include "input_device_provider_directinput.h"
#include "display_message_queue_win32.h"
#include "cursor_provider_win32.h"
#include "../Window/input_context_impl.h"

// #define fun_and_games_with_vista

#include <emmintrin.h>

#ifdef fun_and_games_with_vista
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

#ifdef __MINGW32__
#include <wingdi.h>
#include <excpt.h>
#endif

CL_Win32Window::CL_Win32Window()
: hwnd(0), destroy_hwnd(true), current_cursor(0), large_icon(0), small_icon(0), cursor_set(false), cursor_hidden(false), site(0),
  directinput(0), direct8_module(0),
  minimum_size(0,0), maximum_size(0xffff, 0xffff), layered(false), allow_dropshadow(false), minimized(false), maximized(false),
  update_window_worker_thread_started(false)
{
	memset(&paintstruct, 0, sizeof(PAINTSTRUCT));
	keyboard = CL_InputDevice(new CL_InputDeviceProvider_Win32Keyboard(this));
	mouse = CL_InputDevice(new CL_InputDeviceProvider_Win32Mouse(this));

	create_direct_input();
	CL_DisplayMessageQueue_Win32::message_queue.add_client(this);

	register_clipboard_formats();
}

CL_Win32Window::~CL_Win32Window()
{
	if (update_window_worker_thread_started)
	{
		update_window_event_stop.set();
		update_window_worker_thread.join();
	}


	CL_DisplayMessageQueue_Win32::message_queue.remove_client(this);
	if (ic.impl)
		ic.impl->dispose();
	get_keyboard()->dispose();
	get_mouse()->dispose();

	for (size_t i = 0; i < joysticks.size(); i++)
		joysticks[i].get_provider()->dispose();

	if (directinput)
		directinput->Release();

	if (destroy_hwnd && hwnd)
		DestroyWindow(hwnd);

	// We must delete direct_input module AFTER the window is destroyed. Although input has been released, it seems a bug somewhere in DirectInput causes random crashes unloading
	if (direct8_module)
	{
		FreeLibrary(direct8_module);
		ptr_DirectInput8Create = 0;
	}

	if (large_icon)
		DestroyIcon(large_icon);
	if (small_icon)
		DestroyIcon(small_icon);
}

//////////////////////////////////////////////////////////////////////////
// CL_Win32Window Attributes:

CL_Rect CL_Win32Window::get_geometry() const
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	return CL_Rect(rect.left, rect.top, rect.right, rect.bottom);
}

CL_Rect CL_Win32Window::get_viewport() const
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	return CL_Rect(rect.left, rect.top, rect.right, rect.bottom);
}

bool CL_Win32Window::has_focus() const
{
	return (GetFocus() == hwnd);
}

bool CL_Win32Window::is_minimized() const
{
	return IsIconic(hwnd) != 0;
}

bool CL_Win32Window::is_maximized() const
{
	return IsZoomed(hwnd) != 0;
}

bool CL_Win32Window::is_visible() const
{
	return IsWindowVisible(hwnd) != 0;
}

CL_Size CL_Win32Window::get_minimum_size(bool client_area) const
{
	if (!client_area)
		return minimum_size;
	else
		throw CL_Exception("CL_Win32Window::get_minimum_size not implemented for client_area");
}

CL_Size CL_Win32Window::get_maximum_size(bool client_area) const
{
	if (!client_area)
		return maximum_size;
	else
		throw CL_Exception("CL_Win32Window::get_maximum_size not implemented for client_area");
}

CL_String CL_Win32Window::get_title() const
{
	TCHAR str[1024];
	int len = GetWindowText(hwnd, str, 1024);
	return CL_String(str, len);
}

//////////////////////////////////////////////////////////////////////////
// CL_Win32Window Operations:

void CL_Win32Window::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &description)
{
	site = new_site;
	if (hwnd)
		modify_window(description);
	else
		create_new_window(description);
}

CL_Point CL_Win32Window::client_to_screen(const CL_Point &client)
{
	POINT point = { client.x, client.y };
	ClientToScreen(hwnd, &point);
	return CL_Point(point.x, point.y);
}

CL_Point CL_Win32Window::screen_to_client(const CL_Point &screen)
{
	POINT point = { screen.x, screen.y };
	ScreenToClient(hwnd, &point);
	return CL_Point(point.x, point.y);
}

void CL_Win32Window::show_system_cursor()
{
	ShowCursor(TRUE);
	cursor_hidden = false;
}

void CL_Win32Window::set_cursor(CL_CursorProvider_Win32 *cursor)
{
	SetCursor(cursor->handle);
	current_cursor = cursor->handle;
	cursor_set = true;
}

void CL_Win32Window::set_cursor(CL_StandardCursor type)
{
	HCURSOR cursor_handle = 0;
	switch (type)
	{
	case cl_cursor_arrow: cursor_handle = LoadCursor(0, IDC_ARROW); break;
	case cl_cursor_appstarting: cursor_handle = LoadCursor(0, IDC_APPSTARTING); break;
	case cl_cursor_cross: cursor_handle = LoadCursor(0, IDC_CROSS); break;
	case cl_cursor_hand: cursor_handle = LoadCursor(0, IDC_HAND); break;
	case cl_cursor_ibeam: cursor_handle = LoadCursor(0, IDC_IBEAM); break;
	case cl_cursor_no: cursor_handle = LoadCursor(0, IDC_NO); break;
	case cl_cursor_size_all: cursor_handle = LoadCursor(0, IDC_SIZEALL); break;
	case cl_cursor_size_nesw: cursor_handle = LoadCursor(0, IDC_SIZENESW); break;
	case cl_cursor_size_ns: cursor_handle = LoadCursor(0, IDC_SIZENS); break;
	case cl_cursor_size_nwse: cursor_handle = LoadCursor(0, IDC_SIZENWSE); break;
	case cl_cursor_size_we: cursor_handle = LoadCursor(0, IDC_SIZEWE); break;
	case cl_cursor_uparrow: cursor_handle = LoadCursor(0, IDC_UPARROW); break;
	case cl_cursor_wait: cursor_handle = LoadCursor(0, IDC_WAIT); break;
	default: throw CL_Exception("Unknown standard cursor type");
	}
	SetCursor(cursor_handle);
	current_cursor = cursor_handle;
	cursor_set = true;
}

void CL_Win32Window::hide_system_cursor()
{
	ShowCursor(FALSE);
	cursor_hidden = true;
}

void CL_Win32Window::set_title(const CL_StringRef &new_title)
{
	SetWindowText(hwnd, CL_StringHelp::utf8_to_ucs2(new_title).c_str());
}

void CL_Win32Window::set_position(const CL_Rect &pos, bool client_area)
{
	if (client_area)
	{
		RECT rect = { pos.left, pos.top, pos.right, pos.bottom };
		AdjustWindowRectEx(
			&rect,
			GetWindowLongPtr(hwnd, GWL_STYLE),
			FALSE,
			GetWindowLongPtr(hwnd, GWL_EXSTYLE));

		SetWindowPos(hwnd, 0, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, SWP_NOACTIVATE|SWP_NOREPOSITION|SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(hwnd, 0, pos.left, pos.top, pos.get_width(), pos.get_height(), SWP_NOACTIVATE|SWP_NOREPOSITION|SWP_NOZORDER);
	}
}

void CL_Win32Window::set_size(int width, int height, bool client_area)
{
	if (client_area)
	{
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = width;
		rect.bottom = height;

		AdjustWindowRectEx(
			&rect,
			GetWindowLongPtr(hwnd, GWL_STYLE),
			FALSE,
			GetWindowLongPtr(hwnd, GWL_EXSTYLE));

		SetWindowPos(hwnd, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOREPOSITION|SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(hwnd, 0, 0, 0, width, height, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOREPOSITION|SWP_NOZORDER);
	}
}

void CL_Win32Window::set_enabled(bool enable)
{
	EnableWindow(hwnd, enable ? TRUE : FALSE);
}

void CL_Win32Window::minimize()
{
	ShowWindow(hwnd, SW_MINIMIZE);
}

void CL_Win32Window::restore()
{
	ShowWindow(hwnd, SW_RESTORE);
}

void CL_Win32Window::maximize()
{
	ShowWindow(hwnd, SW_MAXIMIZE);
}

void CL_Win32Window::show(bool activate)
{
	ShowWindow(hwnd, activate ? SW_SHOW : SW_SHOWNA);
}

void CL_Win32Window::hide()
{
	ShowWindow(hwnd, SW_HIDE);
}

void CL_Win32Window::bring_to_front()
{
	BringWindowToTop(hwnd);
}

void CL_Win32Window::capture_mouse(bool capture)
{
	if (capture)
		SetCapture(hwnd);
	else
		ReleaseCapture();
}

void CL_Win32Window::create_direct_input()
{
	direct8_module = LoadLibrary(TEXT("dinput8.dll"));
	ptr_DirectInput8Create = (FuncDirectInput8Create*) GetProcAddress(direct8_module, "DirectInput8Create");
	if (ptr_DirectInput8Create)
	{
		HRESULT result = ptr_DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *) &directinput, 0);
		if (FAILED(result))
		{
			cl_log_event("debug", "Unable to initialize direct input");
		}
	}
}

class CL_ExceptionCatcher
{
public:
	CL_ExceptionCatcher()
	: enabled(true)
	{
	}

	~CL_ExceptionCatcher()
	{
		if (enabled)
		{
			// It is considered an application error if an exception reaches the message handler, since
			// it will now need to unroll the stack beyond code under C++'s control.  We therefore
			// force an application crash here.
			//unexpected();
			RaiseException(EXCEPTION_NONCONTINUABLE_EXCEPTION, EXCEPTION_NONCONTINUABLE, 0, 0);
		}
	}
	void disable() { enabled = false; }

private:
	bool enabled;
};
#ifdef _MSC_VER
LONG cl_grr(PEXCEPTION_POINTERS info)
{
	// The point of this function is to prevent something within Windows from eating access violations
	// and other fatal exceptions.
#ifdef NOT_USED
	switch (info->ExceptionRecord->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
	case EXCEPTION_DATATYPE_MISALIGNMENT:
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
	/*case EXCEPTION_FLT_DENORMAL_OPERAND:
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
	case EXCEPTION_FLT_INEXACT_RESULT:
	case EXCEPTION_FLT_INVALID_OPERATION:
	case EXCEPTION_FLT_OVERFLOW:
	case EXCEPTION_FLT_STACK_CHECK:
	case EXCEPTION_FLT_UNDERFLOW:
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
	case EXCEPTION_INT_OVERFLOW:*/
	case EXCEPTION_PRIV_INSTRUCTION:
	case EXCEPTION_IN_PAGE_ERROR:
	case EXCEPTION_ILLEGAL_INSTRUCTION:
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
	case EXCEPTION_STACK_OVERFLOW:
	case EXCEPTION_INVALID_DISPOSITION:
	/*case EXCEPTION_GUARD_PAGE:
	case EXCEPTION_INVALID_HANDLE:
	case EXCEPTION_POSSIBLE_DEADLOCK:*/
		return UnhandledExceptionFilter(info);
	default:
		return EXCEPTION_CONTINUE_SEARCH;
	}
#endif
	DebugBreak();
	FatalAppExit(0, TEXT("GRR!"));
	SetUnhandledExceptionFilter(0);
	UnhandledExceptionFilter(info);
	return EXCEPTION_CONTINUE_SEARCH;
}
#else
EXCEPTION_DISPOSITION cl_grr;
#endif
LRESULT CL_Win32Window::static_window_try_proc(
	HWND wnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
#ifdef _MSC_VER
{
	__try
	{
		return static_window_proc(wnd, msg, wparam, lparam);
	}
	__except(cl_grr(exception_info()))
	{
		// A special thanks to whoever at Microsoft chose to eat access violations during WM_PAINT calls.
		return DefWindowProc(wnd, msg, wparam, lparam);
	}
}
#else
{
	__try1(cl_grr)
	{
		return static_window_proc(wnd, msg, wparam, lparam);
	}
	__except1
	{
		// A special thanks to whoever at Microsoft chose to eat access violations during WM_PAINT calls.
		return DefWindowProc(wnd, msg, wparam, lparam);
	}
}
#endif

LRESULT CL_Win32Window::static_window_proc(
	HWND wnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
{
	CL_ExceptionCatcher exception_catcher;
	CL_Win32Window *self = 0;
	if (msg == WM_CREATE)
	{
		LPCREATESTRUCT create_struct = (LPCREATESTRUCT) lparam;
		self = (CL_Win32Window *) create_struct->lpCreateParams;
		SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR) self);
	}
	else
	{
		self = (CL_Win32Window *) GetWindowLongPtr(wnd, GWLP_USERDATA);
	}

	LRESULT lresult = 0;
	BOOL call_window_proc = true;

#ifdef fun_and_games_with_vista
	/*
		Misc notes about DWM:

		1. The blur effect is drawn first by the DWM and only covers the region
		   set by DwmEnableBlurBehindWindow.
		2. WM_NCPAINT draws the window frame graphics, including the shadow effect.
		   This means that if we do not forward WM_NCPAINT, we get the blur but not
		   the default window frame. However, we have to draw the shadow ourselves
		   then.
		3. DwmDefWindowProc handles mouse events in the window frame. This is
		   important if we intend to allow GUI objects in the window frame (such as
		   toolbars or a larger app icon like Office 2007). In such a situation we
		   should only pass on the mouse events to DwmDefWindowProc if nothing in the
		   GUI handled it.
		4. If the user disables transparency to dodge the terrible blur effect, the
		   entire DWM is disabled and we have to fall back to Windows XP non-client
		   area handling. This unfortunately means we lose the ability to do
		   transparency in the window generally.
		5. When doing Windows XP non-client area rendering, we have no transparency
		   and have to setup a complex clipping region if we start drawing our own
		   frames (unless we restrict those to the default window frame region).
		6. To avoid Vista enabling backward compatibility stuff, we have to explicitly
		   set the DWMWA_NCRENDERING_POLICY using DwmSetWindowAttribute.
	*/

	BOOL dwm_enabled = FALSE;
	HRESULT result = DwmIsCompositionEnabled(&dwm_enabled);
	if (FAILED(result))
		dwm_enabled = FALSE;

	if (dwm_enabled)
		call_window_proc = !DwmDefWindowProc(wnd, msg, wparam, lparam, &lresult);
	else
		MessageBox(0, TEXT("DWM is not enabled!"), TEXT("Oh noes!"), MB_OK|MB_ICONEXCLAMATION);

	if (msg == WM_CREATE)
	{
		if (dwm_enabled)
		{
			// Following code needed according to MS documentation - go figure.
			RECT window_rect = { 0,0,0,0 };
			GetWindowRect(wnd, &window_rect);
			SetWindowPos(wnd, 0, window_rect.left, window_rect.top, window_rect.right-window_rect.left, window_rect.bottom-window_rect.top, SWP_FRAMECHANGED|SWP_NOZORDER);

			DWMNCRENDERINGPOLICY policy = DWMNCRP_ENABLED;
			DwmSetWindowAttribute(wnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(policy));
			call_window_proc = true;
		}
	}
	else if (msg == WM_ACTIVATE)
	{
		BOOL dwm_enabled = FALSE;
		HRESULT result = DwmIsCompositionEnabled(&dwm_enabled);
		if (SUCCEEDED(result) && dwm_enabled)
		{
			MARGINS margins;
			margins.cxLeftWidth = 8;
			margins.cxRightWidth = 8;
			margins.cyTopHeight = 20;
			margins.cyBottomHeight = 27;
			DwmExtendFrameIntoClientArea(wnd, &margins);

			DWM_BLURBEHIND blur_behind;
			memset(&blur_behind, 0, sizeof(DWM_BLURBEHIND));
			blur_behind.dwFlags = DWM_BB_ENABLE|DWM_BB_BLURREGION;
			blur_behind.fEnable = TRUE;
			blur_behind.hRgnBlur = 0; // apply the blur behind the entire client area
			DwmEnableBlurBehindWindow(wnd, &blur_behind);
		}
		call_window_proc = true;
	}
	else if (call_window_proc && msg == WM_NCCALCSIZE)
	{
		if (wparam == TRUE)
		{
			BOOL dwm_enabled = FALSE;
			HRESULT result = DwmIsCompositionEnabled(&dwm_enabled);
			if (FAILED(result))
				dwm_enabled = FALSE;

			if (dwm_enabled)
			{
				NCCALCSIZE_PARAMS *calcsize_params = reinterpret_cast<NCCALCSIZE_PARAMS *>(lparam);
				calcsize_params->rgrc[0].left += 0;
				calcsize_params->rgrc[0].top += 0;
				calcsize_params->rgrc[0].right -= 0;
				calcsize_params->rgrc[0].bottom -= 0;
				return 0;
			}
		}
	}
#endif

	if (call_window_proc)
	{
		if (self)
		{
			lresult = self->window_proc(wnd, msg, wparam, lparam);
		}
		else
		{
			lresult = DefWindowProc(wnd, msg, wparam, lparam);
		}
	}
	exception_catcher.disable();
	return lresult;
}

LRESULT CL_Win32Window::window_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_ERASEBKGND:
		// We process this function because the DWM needs a background color on the class.
		// The DWM uses this color for clearing the window itself if our WM_PAINT doesn't
		// complete in time.  This is important when the window is being resized, since the
		// color chosen can have a significant impact on visual presentation when our
		// WM_PAINT is too slow and the DWM is forced to present the window prematurely
		// with just the fill completed.
		return 0;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		received_keyboard_input(msg, wparam, lparam);
		return 0;

	case WM_INPUT:
		received_joystick_input(msg, wparam, lparam);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
		received_mouse_input(msg, wparam, lparam);
		return 0;

	case WM_MOUSEMOVE:
		received_mouse_move(msg, wparam, lparam);
		return 0;

	case WM_NCMOUSEMOVE:
		if (get_tablet() && get_tablet()->device_present() && !get_tablet()->is_context_on_top())
			get_tablet()->set_context_on_top(true);
		return 0;

	case WM_MOVE:
		if (get_tablet() && get_tablet()->device_present())
			get_tablet()->check_monitor_changed();
		if (site)
		{
			site->sig_window_moved->invoke();
		}
		break;

	case WM_SIZING:
		if (site)
		{
			WINDOWINFO wi;
			wi.cbSize = sizeof(WINDOWINFO);
			GetWindowInfo(wnd, &wi);

			TITLEBARINFO ti;
			ti.cbSize = sizeof(TITLEBARINFO);
			GetTitleBarInfo(wnd, &ti);

			RECT *win_rect = (RECT*)lparam;

			// enforce minimum size
			if (win_rect->right - win_rect->left < minimum_size.width)
				win_rect->right = win_rect->left + minimum_size.width;
			if (win_rect->bottom - win_rect->top < minimum_size.height)
				win_rect->bottom = win_rect->top + minimum_size.height;

			// enforce max size
			if (win_rect->right - win_rect->left > maximum_size.width)
				win_rect->right = win_rect->left + maximum_size.width;
			if (win_rect->bottom - win_rect->top > maximum_size.height)
				win_rect->bottom = win_rect->top + maximum_size.height;

			CL_Rect client_rect(
				win_rect->left+wi.cxWindowBorders,
				win_rect->top+wi.cyWindowBorders+(ti.rcTitleBar.bottom-ti.rcTitleBar.top),
				win_rect->right-wi.cxWindowBorders,
				win_rect->bottom-wi.cyWindowBorders);

			if (!site->func_window_resize->is_null())
				site->func_window_resize->invoke(client_rect);
			win_rect->left = client_rect.left - wi.cxWindowBorders;
			win_rect->right = client_rect.right + wi.cxWindowBorders;
			win_rect->top = client_rect.top - wi.cyWindowBorders - (ti.rcTitleBar.bottom-ti.rcTitleBar.top);
			win_rect->bottom = client_rect.bottom + wi.cyWindowBorders;
			return TRUE;
		}
		break;

	case WM_SIZE:
		if (!callback_on_resized.is_null())
			callback_on_resized.invoke();

		switch(wparam)
		{
		// Message is sent to all pop-up windows when some other window is maximized.
		case SIZE_MAXHIDE:
			break;

		// Message is sent to all pop-up windows when some other window has been restored to its former size.
		case SIZE_MAXSHOW:
			break;

		// The window has been maximized.
		case SIZE_MAXIMIZED:
			if (site)
				site->sig_window_maximized->invoke();
			minimized = false;
			maximized = true;
			break;

		// The window has been minimized.
		case SIZE_MINIMIZED:
			if (site)
				site->sig_window_minimized->invoke();
			minimized = true;
			maximized = false;
			break;

		// The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
		case SIZE_RESTORED:
			if (minimized || maximized)
			{
				if (site)
					site->sig_window_restored->invoke();
				minimized = false;
				maximized = false;
			}
			break;
		}

		if (site)
			site->sig_resize->invoke(LOWORD(lparam), HIWORD(lparam));

		return 0;

	case WM_ACTIVATE:
		if (site)
		{
			if (LOWORD(wparam) == WA_INACTIVE)
			{
				site->sig_lost_focus->invoke();
				if (get_tablet() && get_tablet()->device_present())
					get_tablet()->set_context_on_top(false);
			}
			else
			{
				site->sig_got_focus->invoke();
				if (get_tablet() && get_tablet()->device_present())
					get_tablet()->set_context_on_top(true);
			}
		}
		return 0;

	case WM_CLOSE:
		if (site)
			site->sig_window_close->invoke();
		return 0;

	case WM_DESTROY:
		if (site)
			site->sig_window_destroy->invoke();
		return 0;

	case WM_PAINT:
		{
			RECT rect;
			if (GetUpdateRect(hwnd, &rect, FALSE))
			{
				memset(&paintstruct, 0, sizeof(PAINTSTRUCT));
				BeginPaint(hwnd, &paintstruct);

				CL_Rect cl_rect;
				cl_rect.left = rect.left;
				cl_rect.top = rect.top;
				cl_rect.right = rect.right;
				cl_rect.bottom = rect.bottom;

				// cl_log_event(cl_format("Dirty %1", has_drop_shadow ? " Pop" : ""), cl_format("Rect: l: %1  t: %2  r: %3  b: %4", cl_rect.left, cl_rect.top, cl_rect.right, cl_rect.bottom));

				if (site)
					site->sig_paint->invoke(cl_rect);

				EndPaint(hwnd, &paintstruct);
				memset(&paintstruct, 0, sizeof(PAINTSTRUCT));
			}
		}
		return 0;

	case WT_PACKET:
		if (get_tablet() && get_tablet()->device_present())
			return get_tablet()->process_packet(lparam, wparam);
		return FALSE;

	case WT_PROXIMITY:
		if (get_tablet() && get_tablet()->device_present())
			return get_tablet()->process_proximity(lparam, wparam);
		return FALSE;

	case WM_SYSCOMMAND:
		switch (wparam)
		{
		case SC_MINIMIZE:
			if (site && !site->func_minimize_clicked->is_null())
			{
				if (site->func_minimize_clicked->invoke())
					return 0;
			}
			break;

		//case SC_SCREENSAVE: // To do: add CL_DisplayWindow::set_allow_screensaver(bool enable)
		//	return 0; 

		default:
			break;
		}
		break;

	case WM_GETICON:
		if (wparam == ICON_BIG && large_icon)
			return reinterpret_cast<LRESULT>(large_icon);
		else if ((wparam == ICON_SMALL || wparam == ICON_SMALL2) && small_icon)
			return reinterpret_cast<LRESULT>(small_icon);
		break;
	}

	// Do default window processing if our message handler didn't handle it:
	return DefWindowProc(wnd, msg, wparam, lparam);
}

void CL_Win32Window::create_new_window(const CL_DisplayWindowDescription &desc)
{
	if (desc.get_handle())
	{
		hwnd = desc.get_handle();
		destroy_hwnd = false;
//		old_wndproc = (WNDPROC) GetWindowLongPtr(hwnd, GWLP_WNDPROC);
//		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR) CL_OpenGLWindowProvider_WGL::message_handler);
	}
	else
	{
		register_window_class();

		DWORD style = 0, ex_style = 0;
		get_styles_from_description(desc, style, ex_style);

		RECT window_rect = get_window_geometry_from_description(desc, style, ex_style);

		HWND parent = 0;
		if (!desc.get_owner().is_null())
			parent = desc.get_owner().get_provider()->get_hwnd();

		hwnd = CreateWindowEx(
			ex_style,
			desc.has_drop_shadow() ? TEXT("ClanApplicationDS") : TEXT("ClanApplication"),
			CL_StringHelp::utf8_to_ucs2(desc.get_title()).c_str(),
			style,
			window_rect.left,
			window_rect.top,
			window_rect.right - window_rect.left,
			window_rect.bottom - window_rect.top,
			parent,
			0,
			(HINSTANCE) GetModuleHandle(0),
			this);

		if (hwnd == 0)
			throw CL_Exception("Unable to create window");

		if (desc.is_fullscreen())
		{
			SetWindowPos(hwnd, HWND_TOP, window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, SWP_NOACTIVATE);
		}

		if (desc.is_topmost())
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);

		if (desc.is_visible())
			ShowWindow(hwnd, SW_SHOW);

	//	if (desc.is_layered()) // TODO: Make the RGB value optional
	//		SetLayeredWindowAttributes(hwnd, RGB(0,0,0), 0, LWA_COLORKEY);

		minimized = is_minimized();
		maximized = is_maximized();
	}

	connect_window_input(desc);
}

void CL_Win32Window::modify_window(const CL_DisplayWindowDescription &desc)
{
	DWORD style = 0, ex_style = 0;
	get_styles_from_description(desc, style, ex_style);
	RECT window_rect = get_window_geometry_from_description(desc, style, ex_style);

	SetWindowText(hwnd, CL_StringHelp::utf8_to_ucs2(desc.get_title()).c_str());

	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowLong(hwnd, GWL_EXSTYLE, ex_style);

	if (desc.is_fullscreen())
	{
		// Place the window above all topmost or non-topmost windows depending on the topmost setting
		SetWindowPos(hwnd, desc.is_topmost() ? HWND_TOPMOST : HWND_TOP, window_rect.left, window_rect.top,
			window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, SWP_NOACTIVATE|SWP_FRAMECHANGED);
	}
	else
	{
		// Setup always on top flag; size as requested by description struct.
		SetWindowPos(
			hwnd,
			desc.is_topmost() ? HWND_TOPMOST : HWND_NOTOPMOST,
			window_rect.left,
			window_rect.top,
			window_rect.right-window_rect.left,
			window_rect.bottom-window_rect.top,
			SWP_NOACTIVATE|SWP_FRAMECHANGED);
	}

	ShowWindow(hwnd, desc.is_visible() ? SW_SHOW : SW_HIDE);
	RedrawWindow(0, 0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE);

	minimized = is_minimized();
	maximized = is_maximized();
}

void CL_Win32Window::received_keyboard_input(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Is message a down or up event?
	bool keydown;
	switch (msg)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			keydown = true;
			break;
		
		case WM_KEYUP:
		case WM_SYSKEYUP:
			keydown = false;
			break;

		default:
			return;
	}

	// Check for Alt+F4 and translate it into a WM_CLOSE event.
	if (wparam == VK_F4 && (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP))
	{
		if ((lparam & (1 << 29)) && msg == WM_SYSKEYDOWN) // context code. Bit 29 is set if ALT key is down.
			SendMessage(get_hwnd(), WM_CLOSE, 0, 0);
		return;
	}

	int scancode = (lparam & 0xff0000) >> 16;
//	bool extended_key = (lparam & 0x1000000) != 0;

	// Update the ctrl/alt/shift hints:
	int key_id = wparam;

	// Add to repeat count
	if(keydown)
	{
		if( repeat_count.find(key_id) == repeat_count.end() )
			repeat_count[key_id] = 0;
		else
			repeat_count[key_id]++;
	}

	// Prepare event to be emitted:
	CL_InputEvent key;
	if (keydown)
		key.type = CL_InputEvent::pressed;
	else
		key.type = CL_InputEvent::released;
	key.mouse_pos = mouse_pos;
	key.id = key_id;
	key.repeat_count = repeat_count[key_id];

	if( !keydown )
	{
		repeat_count[key_id] = -1;
	}

	if (keydown)
	{
		unsigned char keys_down[256];
		GetKeyboardState(keys_down);

		// Figure out what character sequence this maps to:
		WCHAR buf[16];
		int result = ToUnicode(
			(UINT) key_id,
			scancode,
			keys_down,
			buf,
			16,
			0);
		if (result > 0)
			key.str = CL_StringHelp::ucs2_to_text(CL_String16(buf, result));
	}

	set_modifier_keys(key);

	// Emit message:
	get_keyboard()->sig_provider_event->invoke(key);
}

void CL_Win32Window::received_mouse_input(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Map windows events to something more interesting:
	int id;
	bool up = false;
	bool down = false;
	bool dblclk = false;

	switch (msg)
	{
	case WM_LBUTTONDOWN: id = CL_MOUSE_LEFT; down = true; break;
	case WM_LBUTTONUP: id = CL_MOUSE_LEFT; up = true; break;
	case WM_LBUTTONDBLCLK: id = CL_MOUSE_LEFT; dblclk = true; break;
	case WM_RBUTTONDOWN: id = CL_MOUSE_RIGHT; down = true; break;
	case WM_RBUTTONUP: id = CL_MOUSE_RIGHT; up = true; break;
	case WM_RBUTTONDBLCLK: id = CL_MOUSE_RIGHT; dblclk = true; break;
	case WM_MBUTTONDOWN: id = CL_MOUSE_MIDDLE; down = true; break;
	case WM_MBUTTONUP: id = CL_MOUSE_MIDDLE; up = true; break;
	case WM_MBUTTONDBLCLK: id = CL_MOUSE_MIDDLE; dblclk = true; break;
	case WM_MOUSEWHEEL: id = ((short)HIWORD(wparam) > 0) ? CL_MOUSE_WHEEL_UP : CL_MOUSE_WHEEL_DOWN; up = true; down = true; break;
	case WM_XBUTTONDOWN: id = CL_MOUSE_XBUTTON1 + HIWORD(wparam) - 1; down = true; break;
	case WM_XBUTTONUP: id = CL_MOUSE_XBUTTON1 + HIWORD(wparam) - 1; up = true; break;
	case WM_XBUTTONDBLCLK: id = CL_MOUSE_XBUTTON1 + HIWORD(wparam) - 1; dblclk = true; break;
	default:
		return;
	}
	// Prepare event to be emitted:
	CL_InputEvent key;
	key.mouse_pos = mouse_pos;
	key.id = id;
	set_modifier_keys(key);

	if (dblclk)
	{
		key.type = CL_InputEvent::doubleclick;

		// Emit message:
		if (id >= 0 && id < 32)
			get_mouse()->key_states[id] = true;

		get_mouse()->sig_provider_event->invoke(key);
	}

	if (down)
	{
		key.type = CL_InputEvent::pressed;

		// Emit message:
		if (id >= 0 && id < 32)
			get_mouse()->key_states[id] = true;

		get_mouse()->sig_provider_event->invoke(key);
	}

	// It is possible for 2 events to be called when the wheelmouse is used
	if (up)
	{
		key.type = CL_InputEvent::released;

		// Emit message:
		if (id >= 0 && id < 32)
			get_mouse()->key_states[id] = false;

		get_mouse()->sig_provider_event->invoke(key);	
	}
}

void CL_Win32Window::received_mouse_move(UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (get_tablet() && get_tablet()->device_present() && !get_tablet()->is_context_on_top())
		get_tablet()->set_context_on_top(true);

	cursor_set = false;

	// Fetch coordinates
	short x = LOWORD(lparam);
	short y = HIWORD(lparam);

	if (mouse_pos.x != x || mouse_pos.y != y)
	{
		mouse_pos.x = x;
		mouse_pos.y = y;

		// Prepare event to be emitted:
		CL_InputEvent key;
		key.type = CL_InputEvent::pointer_moved;
		key.mouse_pos = mouse_pos;
		set_modifier_keys(key);

		// Fire off signal
		get_mouse()->sig_provider_event->invoke(key);
	}

	if (!cursor_set && !cursor_hidden)
	{
		if (current_cursor == 0)
			set_cursor(cl_cursor_arrow);
		else
			SetCursor(current_cursor);
	}
}

void CL_Win32Window::received_joystick_input(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Polling all the input context devices.
	// This is untidy, as mouse, keyboard will also be poll'ed
	// However, the mouse and keyboard poll() functions are empty.
	// So i think that's okay :)
	ic.poll(false);
/*
	if (msg == WM_INPUT)
	{
		HRAWINPUT handle = (HRAWINPUT)lparam;
		UINT size = 0;
		BOOL result = GetRawInputData(handle, RID_INPUT, 0, &size, sizeof(RAWINPUTHEADER));
		if (result)
		{
			CL_DataBuffer buffer(size);
			result = GetRawInputData(handle, RID_INPUT, buffer.get_data(), &size, sizeof(RAWINPUTHEADER));
			if (result)
			{
				RAWINPUT *rawinput = (RAWINPUT*)buffer.get_data();
				if (rawinput->header.dwType == RIM_TYPEMOUSE)
				{
					// To do: generate relative mouse movement events based on HID mouse information.
				}
			}
		}
	}
*/
}

void CL_Win32Window::setup_tablet()
{
	if (get_tablet())
		get_tablet()->dispose();

	tablet = CL_InputDevice(new CL_InputDeviceProvider_Win32Tablet(this));

	if (get_tablet()->device_present())
		ic.add_tablet(tablet);
}

BOOL CL_Win32Window::enum_devices_callback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	CL_Win32Window *self = (CL_Win32Window *) pvRef;
	try
	{
		CL_InputDevice device(new CL_InputDeviceProvider_DirectInput(self, lpddi));
		self->joysticks.push_back(device);
		self->ic.add_joystick(device);
	}
	catch (const CL_Exception& error)
	{
		cl_log_event("debug", "Could not setup game controller: %1", error.message);
	}
	return TRUE;
}

void CL_Win32Window::set_clipboard_text(const CL_StringRef &text)
{
	CL_String16 text16 = CL_StringHelp::utf8_to_ucs2(text);

	BOOL result = OpenClipboard(hwnd);
	if (result == FALSE)
		throw CL_Exception("Unable to open clipboard");

	result = EmptyClipboard();
	if (result == FALSE)
	{
		CloseClipboard();
		throw CL_Exception("Unable to empty clipboard");
	}

	unsigned int length = (text16.length()+1) * sizeof(CL_StringRef16::char_type);
	HANDLE handle = GlobalAlloc(GMEM_MOVEABLE, length);
	if (handle == 0)
	{
		CloseClipboard();
		throw CL_Exception("Unable to allocate clipboard memory");
	}

	void *data = GlobalLock(handle);
	if (data == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw CL_Exception("Unable to lock clipboard memory");
	}
	memcpy(data, text16.c_str(), length);
	GlobalUnlock(handle);

	HANDLE data_result = SetClipboardData(CF_UNICODETEXT, handle);

	if (data_result == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw CL_Exception("Unable to set clipboard data");
	}

	CloseClipboard();
}

CL_String CL_Win32Window::get_clipboard_text() const
{
	BOOL result = OpenClipboard(hwnd);
	if (result == FALSE)
		throw CL_Exception("Unable to open clipboard");

	HANDLE handle = GetClipboardData(CF_UNICODETEXT);
	if (handle == 0)
	{
		CloseClipboard();
		return CL_String();
	}

	CL_String16::char_type *data = (CL_String16::char_type *) GlobalLock(handle);
	if (data == 0)
	{
		CloseClipboard();
		return CL_String();
	}
	CL_String str = CL_StringHelp::ucs2_to_utf8(data);
	GlobalUnlock(handle);

	CloseClipboard();
	return str;
}

bool CL_Win32Window::is_clipboard_text_available() const
{
	return IsClipboardFormatAvailable(CF_UNICODETEXT) == TRUE;
}

bool CL_Win32Window::is_clipboard_image_available() const
{
	if (IsClipboardFormatAvailable(CF_DIBV5))
		return true;
	else if (IsClipboardFormatAvailable(CF_DIB))
		return true;
	else if (IsClipboardFormatAvailable(CF_BITMAP))
		return true;
	else if (IsClipboardFormatAvailable(png_clipboard_format))
		return true;

	return false;
}

void CL_Win32Window::set_clipboard_image(const CL_PixelBuffer &image)
{
	BOOL result = OpenClipboard(hwnd);
	if (result == FALSE)
		throw CL_Exception("Unable to open clipboard");

	result = EmptyClipboard();
	if (result == FALSE)
	{
		CloseClipboard();
		throw CL_Exception("Unable to empty clipboard");
	}

	add_dib_to_clipboard(image);
	add_png_to_clipboard(image);

	CloseClipboard();
}


void CL_Win32Window::add_png_to_clipboard(const CL_PixelBuffer &image)
{
	CL_DataBuffer png_data_buf(1024*8);
	CL_IODevice_Memory iodev_mem(png_data_buf);
	CL_PNGProvider::save(image, iodev_mem);
	CL_DataBuffer png_data = iodev_mem.get_data();

	unsigned int length = png_data.get_size();
	HANDLE handle = GlobalAlloc(GMEM_MOVEABLE, length);
	if (handle == 0)
	{
		CloseClipboard();
		throw CL_Exception("Unable to allocate clipboard memory");
	}

	char *data = (char *) GlobalLock(handle);
	if (data == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw CL_Exception("Unable to lock clipboard memory");
	}

	memcpy(data, png_data.get_data(), png_data.get_size());

	GlobalUnlock(handle);

	HANDLE data_result = SetClipboardData(png_clipboard_format, handle);

	if (data_result == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw CL_Exception("Unable to set clipboard data");
	}
}

void CL_Win32Window::add_dib_to_clipboard(const CL_PixelBuffer &image)
{
	CL_PixelBuffer bmp_image = create_bitmap_data(image, image.get_size());

	unsigned int length = sizeof(BITMAPV5HEADER) + bmp_image.get_pitch() * bmp_image.get_height();
	HANDLE handle = GlobalAlloc(GMEM_MOVEABLE, length);
	if (handle == 0)
	{
		CloseClipboard();
		throw CL_Exception("Unable to allocate clipboard memory");
	}

	char *data = (char *) GlobalLock(handle);
	if (data == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw CL_Exception("Unable to lock clipboard memory");
	}

	BITMAPV5HEADER *bmp_header = (BITMAPV5HEADER*) data;
	memset(bmp_header, 0, sizeof(BITMAPV5HEADER));
	bmp_header->bV5Size = sizeof(BITMAPV5HEADER);
	bmp_header->bV5Width = bmp_image.get_width();
	bmp_header->bV5Height = bmp_image.get_height();
	bmp_header->bV5Planes = 1;
	bmp_header->bV5BitCount = 32;
	bmp_header->bV5Compression = BI_RGB;
	bmp_header->bV5AlphaMask = 0xff000000;
	bmp_header->bV5CSType = LCS_WINDOWS_COLOR_SPACE;

	memcpy(data + sizeof(BITMAPV5HEADER), bmp_image.get_data(), bmp_image.get_pitch() * bmp_image.get_height());

	GlobalUnlock(handle);

	HANDLE data_result = SetClipboardData(CF_DIBV5, handle);

	if (data_result == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw CL_Exception("Unable to set clipboard data");
	}
}

CL_PixelBuffer CL_Win32Window::create_bitmap_data(const CL_PixelBuffer &image, const CL_Rect &rect)
{
	if (rect.left < 0 || rect.top < 0 || rect.right > image.get_width(), rect.bottom > image.get_height())
		throw CL_Exception("Rectangle passed to CL_Win32Window::create_bitmap_data() out of bounds");

	// Convert pixel buffer to DIB compatible format:
	int pitch = 4;
	CL_PixelBuffer bmp_image(rect.get_width(), rect.get_height(), cl_argb8);

	image.convert(bmp_image, CL_Rect(0, 0, bmp_image.get_width(), bmp_image.get_height()), rect);
	bmp_image.flip_vertical(); // flip_vertical() ensures the pixels are stored upside-down as expected by the BMP format
	
	// Note that the APIs use pre-multiplied alpha, which means that the red,
	// green and blue channel values in the bitmap must be pre-multiplied with
	// the alpha channel value. For example, if the alpha channel value is x,
	// the red, green and blue channels must be multiplied by x and divided by
	// 0xff prior to the call.
	int w = bmp_image.get_width();
	int h = bmp_image.get_height();
	unsigned int *p = (unsigned int *) bmp_image.get_data();
	for (int y = 0; y < h; y++)
	{
		int index = y * w;
		unsigned int *line = p + index;
		for (int x = 0; x < w; x++)
		{
			unsigned int a = ((line[x] >> 24) & 0xff);
			unsigned int r = ((line[x] >> 16) & 0xff);
			unsigned int g = ((line[x] >> 8) & 0xff);
			unsigned int b = (line[x] & 0xff);

			r = r * a / 255;
			g = g * a / 255;
			b = b * a / 255;

			line[x] = (a << 24) + (r << 16) + (g << 8) + b;
		}
	}

	return bmp_image;
}

HBITMAP CL_Win32Window::create_bitmap(HDC hdc, const CL_PixelBuffer &image)
{
	CL_PixelBuffer bmp_image = create_bitmap_data(image, image.get_size());

	BITMAPV5HEADER bmp_header;
	memset(&bmp_header, 0, sizeof(BITMAPV5HEADER));
	bmp_header.bV5Size = sizeof(BITMAPV5HEADER);
	bmp_header.bV5Width = bmp_image.get_width();
	bmp_header.bV5Height = bmp_image.get_height();
	bmp_header.bV5Planes = 1;
	bmp_header.bV5BitCount = 32;
	bmp_header.bV5Compression = BI_RGB;

	HBITMAP bitmap = CreateDIBitmap(hdc, (BITMAPINFOHEADER*) &bmp_header, CBM_INIT, bmp_image.get_data(), (BITMAPINFO *) &bmp_header, DIB_RGB_COLORS);
	return bitmap;
}

HICON CL_Win32Window::create_icon(const CL_PixelBuffer &image) const
{
	HDC hdc = GetDC(hwnd);
	HBITMAP bitmap = create_bitmap(hdc, image);
	if (bitmap == 0)
	{
		ReleaseDC(hwnd, hdc);
		return 0;
	}

	ICONINFO iconinfo;
	memset(&iconinfo, 0, sizeof(ICONINFO));
	iconinfo.fIcon = TRUE;
	iconinfo.hbmColor = bitmap;
	iconinfo.hbmMask = bitmap;
	HICON icon = CreateIconIndirect(&iconinfo);

	ReleaseDC(hwnd, hdc);
	return icon;
}

void CL_Win32Window::request_repaint( const CL_Rect &cl_rect )
{
	RECT rect;
	rect.left = cl_rect.left;
	rect.right = cl_rect.right;
	rect.top = cl_rect.top;
	rect.bottom = cl_rect.bottom;

	InvalidateRect(hwnd, &rect, false);
}

void CL_Win32Window::set_minimum_size( int width, int height, bool client_area)
{
	if (client_area)
		throw CL_Exception("Congratulations! You just got assigned the task of adding support for client area in CL_Win32Window::set_minimum_size(...).");

	this->minimum_size = CL_Size(width,height);
}

void CL_Win32Window::set_maximum_size( int width, int height, bool client_area)
{
	if (client_area)
		throw CL_Exception("Congratulations! You just got assigned the task of adding support for client area in CL_Win32Window::set_maximum_size(...).");

	this->maximum_size = CL_Size(width,height);
}

CL_PixelBuffer CL_Win32Window::get_clipboard_image() const
{
	BOOL result = OpenClipboard(hwnd);
	if (result == FALSE)
		throw CL_Exception("Unable to open clipboard");

	UINT format = EnumClipboardFormats(0);
	UINT png_format = 0;
	while (format)
	{
		TCHAR szFormatName[80];
		int retLen = GetClipboardFormatName(format, szFormatName, sizeof(szFormatName));
		
		if (CL_StringRef16(L"image/png") == szFormatName || 
			CL_StringRef16(L"PNG") == szFormatName)
		{
			png_format = format;
			break;
		}

		format = EnumClipboardFormats(format);
	}

	if (png_format != 0)
	{
		HGLOBAL handle = reinterpret_cast<HGLOBAL>(GetClipboardData(png_format));
		if (handle)
		{
			cl_uchar *data = reinterpret_cast<cl_uchar *>(GlobalLock(handle));
			size_t size = GlobalSize(handle);

			CL_PixelBuffer image = get_argb8888_from_png(data, size);

			GlobalUnlock(handle);
			CloseClipboard();
			return image;
		}
	}
	else
	{
		HGLOBAL handle = reinterpret_cast<HGLOBAL>(GetClipboardData(CF_DIBV5));
		if (handle)
		{
			BITMAPV5HEADER *data = reinterpret_cast<BITMAPV5HEADER*>(GlobalLock(handle));
			size_t size = GlobalSize(handle);

			CL_PixelBuffer image;
			if (data->bV5Compression == BI_RGB)
				image = get_argb8888_from_rgb_dib(data, size);
			else if (data->bV5Compression == BI_BITFIELDS)
				image = get_argb8888_from_bitfields_dib(data, size);

			GlobalUnlock(handle);
			CloseClipboard();
			return image;
		}
	}

	CloseClipboard();
	return CL_PixelBuffer();
}

CL_PixelBuffer CL_Win32Window::get_argb8888_from_rgb_dib(BITMAPV5HEADER *bitmapInfo, size_t size) const
{
	size_t offsetBitmapBits = bitmapInfo->bV5Size + bitmapInfo->bV5ClrUsed * sizeof(RGBQUAD);
	char *bitmapBits = reinterpret_cast<char*>(bitmapInfo)+offsetBitmapBits;
	size_t bitmapBitsSize = bitmapInfo->bV5SizeImage;
	if (bitmapInfo->bV5Compression == BI_RGB)
	{
		size_t pitch = (bitmapInfo->bV5BitCount * bitmapInfo->bV5Width + 31) / 32;
		pitch *= 4;
		bitmapBitsSize = pitch * abs(bitmapInfo->bV5Height);
	}

	if (offsetBitmapBits + bitmapBitsSize > size)
		throw CL_Exception("Out of bounds in get_rgba8888_from_dib!");

	// Ask GDI to do the hard work and convert it to rgba8888:
	HDC hdc = GetDC(0);

	void *bits2 = 0;
	HBITMAP bitmap = CreateDIBSection(hdc, (BITMAPINFO*)bitmapInfo, DIB_RGB_COLORS, &bits2, 0, 0);
	memcpy(bits2, bitmapBits, bitmapBitsSize);

	BITMAPV5HEADER rgbBitmapInfo;
	memset(&rgbBitmapInfo, 0, sizeof(BITMAPV5HEADER));
	rgbBitmapInfo.bV5Size = sizeof(BITMAPV5HEADER);
	rgbBitmapInfo.bV5Width = bitmapInfo->bV5Width;
	rgbBitmapInfo.bV5Height = bitmapInfo->bV5Height;
	rgbBitmapInfo.bV5Planes = 1;
	rgbBitmapInfo.bV5BitCount = 32;
	rgbBitmapInfo.bV5Compression = BI_RGB;
	rgbBitmapInfo.bV5AlphaMask = 0xff000000;
	size_t pitch = rgbBitmapInfo.bV5Width*4;
	rgbBitmapInfo.bV5SizeImage = abs(rgbBitmapInfo.bV5Height) * pitch;
	CL_DataBuffer bitmap_data(pitch * abs(rgbBitmapInfo.bV5Height));
	int scanlines = GetDIBits(hdc, bitmap, 0, abs(bitmapInfo->bV5Height), bitmap_data.get_data(), (LPBITMAPINFO) &rgbBitmapInfo, DIB_RGB_COLORS);
	if (scanlines != abs(bitmapInfo->bV5Height))
		throw CL_Exception("GetDIBits failed");

	// GetDIBits above sets the alpha channel to 0 - need to convert it to 255.
	cl_uchar *data = (cl_uchar *)bitmap_data.get_data();
	for (int y=0; y<abs(bitmapInfo->bV5Height); y++)
	{
		for (int x=0; x<abs(bitmapInfo->bV5Width); x++)
		{
			data[y*pitch + x*4 + 3] = 255;
		}
	}


	CL_PixelBuffer pixelbuffer(rgbBitmapInfo.bV5Width, abs(rgbBitmapInfo.bV5Height), cl_argb8, bitmap_data.get_data());

	ReleaseDC(0, hdc);

	if (bitmapInfo->bV5Height > 0)
		flip_pixelbuffer_vertical(pixelbuffer);

	return pixelbuffer;
}


CL_PixelBuffer CL_Win32Window::get_argb8888_from_bitfields_dib(BITMAPV5HEADER *bitmapInfo, size_t size) const
{
	size_t offsetBitmapBits = bitmapInfo->bV5Size + 3 * sizeof(DWORD);
	char *bitmapBits = reinterpret_cast<char*>(bitmapInfo)+offsetBitmapBits;
	size_t bitmapBitsSize = bitmapInfo->bV5SizeImage;
	if (bitmapInfo->bV5Compression == BI_BITFIELDS) // BI_RGB
	{
		size_t pitch = (bitmapInfo->bV5BitCount * bitmapInfo->bV5Width + 31) / 32;
		pitch *= 4;
		bitmapBitsSize = pitch * abs(bitmapInfo->bV5Height);
	}

	if (offsetBitmapBits + bitmapBitsSize > size)
		throw CL_Exception("Out of bounds in get_rgba8888_from_dib!");

	// Ask GDI to do the hard work and convert it to rgba8888:
	HDC hdc = GetDC(0);

	void *bits2 = 0;
	HBITMAP bitmap = CreateDIBSection(hdc, (BITMAPINFO*)bitmapInfo, DIB_RGB_COLORS, &bits2, 0, 0);
	memcpy(bits2, bitmapBits, bitmapBitsSize);

	BITMAPV5HEADER rgbBitmapInfo;
	memset(&rgbBitmapInfo, 0, sizeof(BITMAPV5HEADER));
	rgbBitmapInfo.bV5Size = sizeof(BITMAPV5HEADER);
	rgbBitmapInfo.bV5Width = bitmapInfo->bV5Width;
	rgbBitmapInfo.bV5Height = bitmapInfo->bV5Height;
	rgbBitmapInfo.bV5Planes = 1;
	rgbBitmapInfo.bV5BitCount = 32;
	rgbBitmapInfo.bV5Compression = BI_BITFIELDS;
	rgbBitmapInfo.bV5RedMask   = 0xff000000;
	rgbBitmapInfo.bV5GreenMask = 0x00ff0000;
	rgbBitmapInfo.bV5BlueMask  = 0x0000ff00;
	rgbBitmapInfo.bV5AlphaMask = 0x000000ff;
	size_t pitch = rgbBitmapInfo.bV5Width*4;
	rgbBitmapInfo.bV5SizeImage = abs(rgbBitmapInfo.bV5Height) * pitch;
	CL_DataBuffer bitmap_data(pitch * abs(rgbBitmapInfo.bV5Height));
	int scanlines = GetDIBits(hdc, bitmap, 0, abs(bitmapInfo->bV5Height), bitmap_data.get_data(), (LPBITMAPINFO) &rgbBitmapInfo, DIB_RGB_COLORS);
	if (scanlines != abs(bitmapInfo->bV5Height))
		throw CL_Exception("GetDIBits failed");

	// GetDIBits above sets the alpha channel to 0 - need to convert it to 255.
	cl_uchar *data = (cl_uchar *)bitmap_data.get_data();
	for (int y=0; y<abs(bitmapInfo->bV5Height); y++)
	{
		for (int x=0; x<abs(bitmapInfo->bV5Width); x++)
		{
			data[y*pitch + x*4 + 3] = 255;
		}
	}


	CL_PixelBuffer pixelbuffer(rgbBitmapInfo.bV5Width, abs(rgbBitmapInfo.bV5Height), cl_argb8, bitmap_data.get_data());

	ReleaseDC(0, hdc);

	if (bitmapInfo->bV5Height > 0)
		flip_pixelbuffer_vertical(pixelbuffer);

	return pixelbuffer;
}



void CL_Win32Window::flip_pixelbuffer_vertical(CL_PixelBuffer &pbuf) const
{
	cl_uchar *data = (cl_uchar*)pbuf.get_data();

	for (int y=0; y<(pbuf.get_height()/2); y++)
	{
		cl_uint *dy = (cl_uint*)(data + (y*pbuf.get_pitch()));
		cl_uint *dy2 = (cl_uint*)(data + (pbuf.get_height()-y-1)*pbuf.get_pitch());

		for (int x=0; x<pbuf.get_width(); x++)
		{
			cl_uint tmp = dy[x];
			dy[x] = dy2[x];
			dy2[x] = tmp;
		}
	}
}

CL_PixelBuffer CL_Win32Window::get_argb8888_from_png(cl_uchar *data, size_t size) const
{
	CL_DataBuffer data_buffer(data, size);
	CL_IODevice_Memory iodev(data_buffer);
	CL_PixelBuffer pbuf = CL_PNGProvider::load(iodev);
	return pbuf;
}

void CL_Win32Window::register_clipboard_formats()
{
//	TCHAR *png_format_str = L"image/png";
	TCHAR *png_format_str = L"PNG";
	png_clipboard_format = RegisterClipboardFormat(png_format_str);
}

void CL_Win32Window::set_large_icon(const CL_PixelBuffer &image)
{
	if (large_icon)
		DestroyIcon(large_icon);
	large_icon = 0;
	large_icon = create_icon(image);

	SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(large_icon));
}

void CL_Win32Window::set_small_icon(const CL_PixelBuffer &image)
{
	if (small_icon)
		DestroyIcon(small_icon);
	small_icon = 0;
	small_icon = create_icon(image);

	SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(small_icon));
}

void CL_Win32Window::set_modifier_keys(CL_InputEvent &key)
{
	key.alt = (GetKeyState(VK_MENU) & 0xfe) != 0;
	key.shift = (GetKeyState(VK_SHIFT) & 0xfe) != 0;
	key.ctrl = (GetKeyState(VK_CONTROL) & 0xfe) != 0;
}

CL_InputDeviceProvider_Win32Keyboard *CL_Win32Window::get_keyboard()
{
	return static_cast<CL_InputDeviceProvider_Win32Keyboard *>(keyboard.get_provider());
}

CL_InputDeviceProvider_Win32Mouse *CL_Win32Window::get_mouse()
{
	return static_cast<CL_InputDeviceProvider_Win32Mouse *>(mouse.get_provider());
}

CL_InputDeviceProvider_Win32Tablet *CL_Win32Window::get_tablet()
{
	return static_cast<CL_InputDeviceProvider_Win32Tablet *>(tablet.get_provider());
}

void CL_Win32Window::register_window_class()
{
	static bool first_call = true;
	if (first_call)
	{
		WNDCLASS wndclass;
		memset(&wndclass, 0, sizeof(WNDCLASS));
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // 0; 
		wndclass.lpfnWndProc = (WNDPROC) CL_Win32Window::static_window_try_proc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = (HINSTANCE) GetModuleHandle(0);
		wndclass.hIcon = 0;
		wndclass.hCursor = 0;
		wndclass.hbrBackground = CreateSolidBrush(RGB(255,255,255)); // Todo: add CL_DisplayWindow::set_dwm_clear_color(const CL_Colorf &color)
		wndclass.lpszMenuName = 0;
		wndclass.lpszClassName = TEXT("ClanApplication");
		RegisterClass(&wndclass);

		memset(&wndclass, 0, sizeof(WNDCLASS));

		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		if (allow_dropshadow)
		{
			wndclass.style |= CS_DROPSHADOW;
		}

		wndclass.lpfnWndProc = (WNDPROC) CL_Win32Window::static_window_try_proc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = (HINSTANCE) GetModuleHandle(0);
		wndclass.hIcon = 0;
		wndclass.hCursor = 0;
		wndclass.hbrBackground = CreateSolidBrush(RGB(255,255,255)); // Todo: add CL_DisplayWindow::set_dwm_clear_color(const CL_Colorf &color)
		wndclass.lpszMenuName = 0;
		wndclass.lpszClassName = TEXT("ClanApplicationDS");
		RegisterClass(&wndclass);

		first_call = false;
	}
}

void CL_Win32Window::connect_window_input(const CL_DisplayWindowDescription &desc)
{
	// Connect input context to new window:

	// See http://msdn.microsoft.com/en-us/library/ms789918.aspx

	#ifndef HID_USAGE_PAGE_GENERIC
	#define HID_USAGE_PAGE_GENERIC		((USHORT) 0x01)
	#endif

	#ifndef HID_USAGE_GENERIC_MOUSE
	#define HID_USAGE_GENERIC_MOUSE	((USHORT) 0x02)
	#endif

	#ifndef HID_USAGE_GENERIC_JOYSTICK
	#define HID_USAGE_GENERIC_JOYSTICK	((USHORT) 0x04)
	#endif

	#ifndef HID_USAGE_GENERIC_GAMEPAD
	#define HID_USAGE_GENERIC_GAMEPAD	((USHORT) 0x05)
	#endif

	#ifndef RIDEV_INPUTSINK
	#define RIDEV_INPUTSINK	(0x100)
	#endif

	// NOTE: During developing this, I noticed that changing any of these options require
	// you to go into control panel / Game controllers - Else the device is not detected!
	// Maybe it is because we never clear RegisterRawInputDevices() on exit?

	// Treat joystick and gamepad as the same thing

//	RAWINPUTDEVICE Rid[3];	(for mouse)
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
	Rid[0].usUsage = HID_USAGE_GENERIC_JOYSTICK; 
	Rid[0].dwFlags = 0;//RIDEV_INPUTSINK;   
	Rid[0].hwndTarget = hwnd;
	Rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC; 
	Rid[1].usUsage = HID_USAGE_GENERIC_GAMEPAD; 
	Rid[1].dwFlags = 0;//RIDEV_INPUTSINK;   
	Rid[1].hwndTarget = hwnd;
	BOOL result = RegisterRawInputDevices(Rid, 2, sizeof(RAWINPUTDEVICE));
/*
	-- Experimental mouse support disabled for now.  Note, (many?) mices are still PS2.
	Rid[2].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[2].usUsage = HID_USAGE_GENERIC_MOUSE; 
	Rid[2].dwFlags = 0;//RIDEV_INPUTSINK;   
	Rid[2].hwndTarget = hwnd;
	BOOL result = RegisterRawInputDevices(Rid, 3, sizeof(RAWINPUTDEVICE));
*/
	ic.clear();
	ic.add_keyboard(keyboard);
	ic.add_mouse(mouse);

	for (size_t i = 0; i < joysticks.size(); i++)
		joysticks[i].get_provider()->dispose();
	joysticks.clear();

	// Go looking for joysticks:
	if (directinput)
	{
		HRESULT result = directinput->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			&CL_Win32Window::enum_devices_callback,
			this,
			DIEDFL_ATTACHEDONLY);
		if (FAILED(result))
		{
			cl_log_event("debug", "Unable to enumerate direct input devices");
		}
	}

	if (desc.get_tablet_context())
		setup_tablet();
}

void CL_Win32Window::get_styles_from_description(const CL_DisplayWindowDescription &desc, DWORD &style, DWORD &ex_style)
{
	style = 0;
	ex_style = 0;

	if (desc.is_fullscreen() || desc.is_dialog() || !desc.get_decorations() || !desc.has_caption())
		style |= WS_POPUP;

	if (desc.get_allow_resize() && !desc.is_fullscreen() && !desc.is_dialog())
		style |= WS_SIZEBOX;

	if (desc.has_caption() && desc.get_decorations())
	{
		style |= WS_CAPTION;

		if (desc.has_sysmenu())
		{
			style |= WS_SYSMENU;
		}
		if (!desc.is_dialog())
		{
			if (desc.has_minimize_button())
				style |= WS_MINIMIZEBOX;
			if (desc.has_maximize_button() && desc.get_allow_resize())
				style |= WS_MAXIMIZEBOX;
		}
	}

	if (desc.is_layered())
	{
		layered = true;
		ex_style |= WS_EX_LAYERED;
	}
	else
	{
		layered = false;
	}

	if (desc.is_tool_window())
		ex_style |= WS_EX_TOOLWINDOW;

	if (desc.is_dialog())
	{
		ex_style |= WS_EX_DLGMODALFRAME;
		ex_style |= WS_EX_WINDOWEDGE;
	}
}

RECT CL_Win32Window::get_window_geometry_from_description(const CL_DisplayWindowDescription &desc, DWORD style, DWORD ex_style)
{
	int x = desc.get_position().left;
	int y = desc.get_position().top;
	int width = desc.get_size().width;
	int height = desc.get_size().height;

	bool clientSize = desc.get_position_client_area();	// false = Size includes the window frame. true = Size is the drawable size.

	if (desc.is_fullscreen())
	{
		int primary_screen = 0;
		CL_ScreenInfo screen_info;
		std::vector<CL_Rect> screen_rects = screen_info.get_screen_geometries(primary_screen);
		CL_Rect R = screen_rects[desc.get_fullscreen_monitor()];

		clientSize = false;
		x = R.left;
		y = R.top;
		width = R.get_width();
		height = R.get_height();

/*		x = 0;
		y = 0;
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);*/
	}
	else if (desc.get_position().left == -1 && desc.get_position().top == -1)
	{
		int scr_width = GetSystemMetrics(SM_CXSCREEN);
		int scr_height = GetSystemMetrics(SM_CYSCREEN);

		x = scr_width/2 - width/2;
		y = scr_height/2 - height/2;
	}

	// get size of window with decorations to pass to CreateWindow
	RECT window_rect = { x, y, x+width, y+height };
	if (clientSize)
		AdjustWindowRectEx( &window_rect, style, FALSE, ex_style );
	return window_rect;
}

void CL_Win32Window::update_layered(CL_PixelBuffer &image)
{
	if (!update_window_worker_thread_started)
	{
		update_window_worker_thread.start(this, &CL_Win32Window::update_layered_worker_thread);
		update_window_worker_thread_started = true;
	}
	else
	{
		update_window_event_completed.wait();
		update_window_event_completed.reset();
	}
	update_window_image = image;
	update_window_event_start.set();

}

void CL_Win32Window::update_layered_process_alpha(int y_start, int y_stop)
{
	int w = update_window_image.get_width();

#ifndef CL_DISABLE_SSE2
	int sse_size = (w/8)*8;
#endif

	cl_uint *p = (cl_uint *) update_window_image.get_data();
	for (int y = y_start; y < y_stop; y++)
	{
		int index = y * w;
		cl_uint *line = p + index;

		int size = w;
#ifndef CL_DISABLE_SSE2

		int not_aligned = (line - p) & 0x3;	// 4 lots of 32bit integers
		// Draw any that is not 128bit aligned
		for (int x = 0; x < not_aligned; x++)
		{
			// Reading RGBA
			unsigned int cval = *line;
			cl_uint r = (cval >> 24) & 0xff;
			cl_uint g = (cval >> 16) & 0xff;
			cl_uint b = (cval >> 8) & 0xff;
			cl_uint a = cval & 0xff;

			r = r * a / 255;
			g = g * a / 255;
			b = b * a / 255;

			// Writing ARGB
			*(line++) = (a << 24) + (r << 16) + (g << 8) + b;
		}
		size -= not_aligned;

		int sse_size = (size/4)*4;

		const __m128i alpha_mask = _mm_set1_epi32(0xFF);
		const __m128i lomask = _mm_set1_epi32(0x00FF00FF);
		const __m128i round = _mm_set1_epi16(128);

		for (int x = 0; x < sse_size; x+=4)
		{
			__m128i source = _mm_load_si128((__m128i*)(line+x));
		// now source = RRGGBBAA

			__m128i alpha = _mm_and_si128(alpha_mask, source);
			alpha = _mm_or_si128(alpha, _mm_slli_epi32(alpha, 16));
		// now alpha = 00AA00AA

			__m128i red_blue = _mm_and_si128(lomask, _mm_srli_epi32(source, 8));
		// now red_blue = 00RR00BB

			red_blue =_mm_mullo_epi16(red_blue, alpha);
			red_blue = _mm_add_epi16(red_blue, round);
			__m128i t = _mm_srli_epi16(red_blue, 8);
			t = _mm_add_epi16(t, red_blue);
			red_blue = _mm_srli_epi16(t, 8);
		// now red_blue = 00rr00bb  (rr = RR * AA/255) (bb = BB * AA/255)

			__m128i green_alpha = _mm_and_si128(lomask, source);
		// now green_alpha = 00GG00AA

			green_alpha =_mm_mullo_epi16(green_alpha, alpha);
			green_alpha = _mm_add_epi16(green_alpha, round);
			t = _mm_srli_epi16(green_alpha, 8);
			t = _mm_add_epi16(t, green_alpha);
			green_alpha = _mm_srli_epi16(t, 8);
		// now green_alpha = 00gg00aa  (gg = GG * AA/255) (aa = AA * AA/255)

			t = _mm_or_si128(red_blue, _mm_slli_epi32(green_alpha, 24));
		// now t = aarr00bb

			t = _mm_or_si128(t, _mm_srli_epi32(green_alpha, 8));
		// now t = aarrggbb

			_mm_store_si128((__m128i*)(line+x), t);
		}

		// Do remaining pixels
		size-=sse_size;
		line+=sse_size;

#endif
		for (int x = 0; x < size; x++)
		{
			// Reading RGBA
			unsigned int cval = *line;
			cl_uint r = (cval >> 24) & 0xff;
			cl_uint g = (cval >> 16) & 0xff;
			cl_uint b = (cval >> 8) & 0xff;
			cl_uint a = cval & 0xff;

			r = r * a / 255;
			g = g * a / 255;
			b = b * a / 255;

			// Writing ARGB
			*(line++) = (a << 24) + (r << 16) + (g << 8) + b;
		}
	}
}

void CL_Win32Window::update_layered_worker_thread()
{
	while (true)
	{
		int wakeup_reason = CL_Event::wait(update_window_event_start, update_window_event_stop);

		if (wakeup_reason != 0)
			break;

		update_window_event_start.reset();
		update_layered_worker_thread_process();
		update_window_event_completed.set();
	}
}

void CL_Win32Window::update_layered_worker_thread_process()
{
	// Note that the APIs use pre-multiplied alpha, which means that the red,
	// green and blue channel values in the bitmap must be pre-multiplied with
	// the alpha channel value. For example, if the alpha channel value is x,
	// the red, green and blue channels must be multiplied by x and divided by
	// 0xff prior to the call.

	update_layered_process_alpha(0, update_window_image.get_height());

	BITMAPV5HEADER bmp_header;
	memset(&bmp_header, 0, sizeof(BITMAPV5HEADER));
	bmp_header.bV5Size = sizeof(BITMAPV5HEADER);
	bmp_header.bV5Width = update_window_image.get_width();
	bmp_header.bV5Height = update_window_image.get_height();
	bmp_header.bV5Planes = 1;
	bmp_header.bV5BitCount = 32;
	bmp_header.bV5Compression = BI_RGB;

	HDC hdc = GetDC(hwnd);
	HDC bitmap_dc = CreateCompatibleDC(hdc);
	HBITMAP bitmap = CreateDIBitmap(hdc, (BITMAPINFOHEADER *) &bmp_header, CBM_INIT, update_window_image.get_data(), (BITMAPINFO *) &bmp_header, DIB_RGB_COLORS);
	HGDIOBJ old_bitmap = SelectObject(bitmap_dc, bitmap);

	SIZE size = { update_window_image.get_width(), update_window_image.get_height() };
	POINT point = { 0, 0 };
	COLORREF rgb_colorkey = RGB(0, 0, 0);
	BLENDFUNCTION blend;
	memset(&blend, 0, sizeof(BLENDFUNCTION));
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = (BYTE) 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	UpdateLayeredWindow(hwnd, 0, 0, &size, bitmap_dc, &point, rgb_colorkey, &blend, ULW_ALPHA);

	SelectObject(bitmap_dc, old_bitmap);
	DeleteObject(bitmap);
	DeleteDC(bitmap_dc);
	ReleaseDC(hwnd, hdc);

	update_window_image = CL_PixelBuffer();
}

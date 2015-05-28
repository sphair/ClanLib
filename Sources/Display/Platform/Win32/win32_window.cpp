/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/Core/Text/logger.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/memory_device.h"
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
#include "input_device_provider_win32hid.h"
#include "display_message_queue_win32.h"
#include "cursor_provider_win32.h"
#include "dwm_functions.h"
#include "../../Window/input_context_impl.h"
#include "../../setup_display.h"

#include <emmintrin.h>

#ifdef __MINGW32__
#include <wingdi.h>
#include <excpt.h>
#endif

namespace clan
{

Win32Window::Win32Window()
: hwnd(0), destroy_hwnd(true), current_cursor(0), large_icon(0), small_icon(0), cursor_set(false), cursor_hidden(false), site(0),
  minimum_size(0,0), maximum_size(0xffff, 0xffff), allow_dropshadow(false),
  update_window_worker_thread_started(false), update_window_region(0), update_window_max_region_rects(1024)
{
	HDC dc = GetDC(0);
	int ppi = GetDeviceCaps(dc, LOGPIXELSX);
	ReleaseDC(0, dc);
	set_pixel_ratio(ppi / 96.0f);

	memset(&paintstruct, 0, sizeof(PAINTSTRUCT));
	keyboard = InputDevice(new InputDeviceProvider_Win32Keyboard(this));
	mouse = InputDevice(new InputDeviceProvider_Win32Mouse(this));

	SetupDisplay::get_message_queue()->add_client(this);

	register_clipboard_formats();
}

Win32Window::~Win32Window()
{
	if (update_window_worker_thread_started)
	{
		std::unique_lock<std::mutex> lock(update_window_mutex);
		update_window_stop_flag = true;
		lock.unlock();
		update_window_worker_event.notify_all();
		update_window_worker_thread.join();
	}
	if (update_window_region)
		DeleteObject(update_window_region);

	SetupDisplay::get_message_queue()->remove_client(this);
	ic.dispose();
	get_keyboard()->dispose();
	get_mouse()->dispose();

	for (size_t i = 0; i < joysticks.size(); i++)
		joysticks[i].get_provider()->dispose();

	if (destroy_hwnd && hwnd)
		DestroyWindow(hwnd);

	if (large_icon)
		DestroyIcon(large_icon);
	if (small_icon)
		DestroyIcon(small_icon);
}

//////////////////////////////////////////////////////////////////////////
// Win32Window Attributes:

Rect Win32Window::get_geometry() const
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}

Rect Win32Window::get_viewport() const
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}

bool Win32Window::has_focus() const
{
	return (GetFocus() == hwnd);
}

bool Win32Window::is_minimized() const
{
	return IsIconic(hwnd) != 0;
}

bool Win32Window::is_maximized() const
{
	return IsZoomed(hwnd) != 0;
}

bool Win32Window::is_visible() const
{
	return IsWindowVisible(hwnd) != 0;
}

Size Win32Window::get_minimum_size(bool client_area) const
{
	if (!client_area)
		return minimum_size;
	else
		throw Exception("Win32Window::get_minimum_size not implemented for client_area");
}

Size Win32Window::get_maximum_size(bool client_area) const
{
	if (!client_area)
		return maximum_size;
	else
		throw Exception("Win32Window::get_maximum_size not implemented for client_area");
}

std::string Win32Window::get_title() const
{
	WCHAR str[1024];
	int len = GetWindowText(hwnd, str, 1024);
	return StringHelp::ucs2_to_utf8(std::wstring(str, len));
}

//////////////////////////////////////////////////////////////////////////
// Win32Window Operations:

void Win32Window::create(DisplayWindowSite *new_site, const DisplayWindowDescription &description)
{
	window_desc = description.clone();
	site = new_site;
	create_new_window();
}

Point Win32Window::client_to_screen(const Point &client)
{
	POINT point = { client.x, client.y };
	ClientToScreen(hwnd, &point);
	return Point(point.x, point.y);
}

Point Win32Window::screen_to_client(const Point &screen)
{
	POINT point = { screen.x, screen.y };
	ScreenToClient(hwnd, &point);
	return Point(point.x, point.y);
}

void Win32Window::show_system_cursor()
{
	ShowCursor(TRUE);
	cursor_hidden = false;
}

void Win32Window::set_cursor(CursorProvider_Win32 *cursor)
{
	SetCursor(cursor->handle);
	current_cursor = cursor->handle;
	cursor_set = true;
}

void Win32Window::set_cursor_handle(HCURSOR handle)
{
	SetCursor(handle);
	current_cursor = handle;
	cursor_set = true;
}

void Win32Window::set_cursor(StandardCursor type)
{
	HCURSOR cursor_handle = 0;
	switch (type)
	{
	case StandardCursor::arrow: cursor_handle = LoadCursor(0, IDC_ARROW); break;
	case StandardCursor::appstarting: cursor_handle = LoadCursor(0, IDC_APPSTARTING); break;
	case StandardCursor::cross: cursor_handle = LoadCursor(0, IDC_CROSS); break;
	case StandardCursor::hand: cursor_handle = LoadCursor(0, IDC_HAND); break;
	case StandardCursor::ibeam: cursor_handle = LoadCursor(0, IDC_IBEAM); break;
	case StandardCursor::no: cursor_handle = LoadCursor(0, IDC_NO); break;
	case StandardCursor::size_all: cursor_handle = LoadCursor(0, IDC_SIZEALL); break;
	case StandardCursor::size_nesw: cursor_handle = LoadCursor(0, IDC_SIZENESW); break;
	case StandardCursor::size_ns: cursor_handle = LoadCursor(0, IDC_SIZENS); break;
	case StandardCursor::size_nwse: cursor_handle = LoadCursor(0, IDC_SIZENWSE); break;
	case StandardCursor::size_we: cursor_handle = LoadCursor(0, IDC_SIZEWE); break;
	case StandardCursor::uparrow: cursor_handle = LoadCursor(0, IDC_UPARROW); break;
	case StandardCursor::wait: cursor_handle = LoadCursor(0, IDC_WAIT); break;
	default: throw Exception("Unknown standard cursor type");
	}
	SetCursor(cursor_handle);
	current_cursor = cursor_handle;
	cursor_set = true;
}

void Win32Window::hide_system_cursor()
{
	ShowCursor(FALSE);
	cursor_hidden = true;
}

void Win32Window::set_title(const std::string &new_title)
{
	SetWindowText(hwnd, StringHelp::utf8_to_ucs2(new_title).c_str());
}

void Win32Window::set_position(const Rect &pos, bool client_area)
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
		SetWindowPos(hwnd, 0, pos.left, pos.top, pos.get_width(), pos.get_height(), SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOZORDER);
	}
}

void Win32Window::set_size(int width, int height, bool client_area)
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

void Win32Window::set_enabled(bool enable)
{
	EnableWindow(hwnd, enable ? TRUE : FALSE);
}

void Win32Window::minimize()
{
	ShowWindow(hwnd, SW_MINIMIZE);
}

void Win32Window::restore()
{
	ShowWindow(hwnd, SW_RESTORE);
}

void Win32Window::maximize()
{
	ShowWindow(hwnd, SW_MAXIMIZE);
}

// ------------------------------------------------------------------------
// Correct fullscreen implementation according to Raymond Chen:
// http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx
// ------------------------------------------------------------------------
void Win32Window::toggle_fullscreen()
{
	DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	if (dwStyle & WS_OVERLAPPEDWINDOW) 
	{
		window_style_before_fullscreen = GetWindowLong(hwnd, GWL_STYLE);
		MONITORINFO mi = { sizeof(mi) };
		if (GetWindowPlacement(hwnd, &window_positon_before_fullscreen) &&
			GetMonitorInfo(MonitorFromWindow(hwnd,
			MONITOR_DEFAULTTOPRIMARY), &mi)) 
		{
			SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, 
				mi.rcMonitor.left, mi.rcMonitor.top, 
				mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLong(hwnd, GWL_STYLE, dwStyle | window_style_before_fullscreen);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		SetWindowPlacement(hwnd, &window_positon_before_fullscreen);
	}
}

void Win32Window::show(bool activate)
{
	ShowWindow(hwnd, activate ? SW_SHOW : SW_SHOWNA);
}

void Win32Window::hide()
{
	ShowWindow(hwnd, SW_HIDE);
}

void Win32Window::bring_to_front()
{
	BringWindowToTop(hwnd);
}

void Win32Window::capture_mouse(bool capture)
{
	if (capture)
		SetCapture(hwnd);
	else
		ReleaseCapture();
}

LRESULT Win32Window::static_window_proc(
	HWND wnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
{
	SEHCatchAllWorkaround vista_x64_workaround;

	Win32Window *self = 0;
	if (msg == WM_CREATE)
	{
		LPCREATESTRUCT create_struct = (LPCREATESTRUCT) lparam;
		self = (Win32Window *) create_struct->lpCreateParams;
		SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR) self);
		self->hwnd = wnd;
	}
	else
	{
		self = (Win32Window *) GetWindowLongPtr(wnd, GWLP_USERDATA);
	}

	LRESULT lresult = 0;
	BOOL call_window_proc = true;

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
	vista_x64_workaround.unpatch();
	return lresult;
}

LRESULT Win32Window::window_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (DwmFunctions::is_composition_enabled())
	{
		// Hit test handling for the Minimize + Maximize + Close buttons
		if (DwmFunctions::Dwm_DefWindowProc)
		{
			LRESULT result = 0;
			if (DwmFunctions::Dwm_DefWindowProc(wnd, msg, wparam, lparam, &result) == TRUE)
				return result;
		}
	}

	if ((site) && (site->func_window_message))
	{
		if ((site->func_window_message)(wnd, msg, wparam, lparam))
			return TRUE;
	}

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
	case WM_DWMCOMPOSITIONCHANGED: return wm_dwm_composition_changed(wparam, lparam);
	case WM_CREATE: return wm_create(wparam, lparam);

	case WM_NCHITTEST: return wm_nc_hittest(wparam, lparam);
	case WM_NCCALCSIZE: return wm_nc_calcsize(wparam, lparam);

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

	case WM_MOUSEACTIVATE:
		if (window_desc.has_no_activate())
			return MA_NOACTIVATE;
		break;

	case WM_NCMOUSEMOVE:
		if (get_tablet() && get_tablet()->device_present() && !get_tablet()->is_context_on_top())
			get_tablet()->set_context_on_top(true);
		return 0;

	case WM_MOVE:
		if (get_tablet() && get_tablet()->device_present())
			get_tablet()->check_monitor_changed();
		if (site)
		{
			(site->sig_window_moved)();
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

			Rect client_rect(
				win_rect->left+wi.cxWindowBorders,
				win_rect->top+wi.cyWindowBorders+(ti.rcTitleBar.bottom-ti.rcTitleBar.top),
				win_rect->right-wi.cxWindowBorders,
				win_rect->bottom-wi.cyWindowBorders);

			Rectf client_rectf;
			client_rectf.left = client_rect.left / pixel_ratio;
			client_rectf.top = client_rect.top / pixel_ratio;
			client_rectf.right = client_rect.right / pixel_ratio;
			client_rectf.bottom = client_rect.bottom / pixel_ratio;

			if (site->func_window_resize)
				(site->func_window_resize)(client_rectf);

			client_rect.left = (int)std::round(client_rectf.left * pixel_ratio);
			client_rect.top = (int)std::round(client_rectf.top * pixel_ratio);
			client_rect.right = (int)std::round(client_rectf.right * pixel_ratio);
			client_rect.bottom = (int)std::round(client_rectf.bottom * pixel_ratio);

			win_rect->left = client_rect.left - wi.cxWindowBorders;
			win_rect->right = client_rect.right + wi.cxWindowBorders;
			win_rect->top = client_rect.top - wi.cyWindowBorders - (ti.rcTitleBar.bottom-ti.rcTitleBar.top);
			win_rect->bottom = client_rect.bottom + wi.cyWindowBorders;
			return TRUE;
		}
		break;

	case WM_SIZE:
		if (callback_on_resized)
			callback_on_resized();

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
				(site->sig_window_maximized)();
			break;

		// The window has been minimized.
		case SIZE_MINIMIZED:
			if (site)
				(site->sig_window_minimized)();
			break;

		// The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
		case SIZE_RESTORED:
			if (site)
				(site->sig_window_restored)();
			break;
		}

		if (site)
			(site->sig_resize)(LOWORD(lparam), HIWORD(lparam));

		return 0;

	case WM_ACTIVATE:
		update_dwm_settings();
		if (site)
		{
			if (LOWORD(wparam) == WA_INACTIVE)
			{
				(site->sig_lost_focus)();
				if (get_tablet() && get_tablet()->device_present())
					get_tablet()->set_context_on_top(false);
			}
			else
			{
				(site->sig_got_focus)();
				if (get_tablet() && get_tablet()->device_present())
					get_tablet()->set_context_on_top(true);
			}
		}
		return 0;

	case WM_CLOSE:
		if (site)
			(site->sig_window_close)();
		return 0;

	case WM_DESTROY:
		if (site)
			(site->sig_window_destroy)();
		return 0;

	case WM_PAINT:
		{
			RECT rect;
			if (GetUpdateRect(hwnd, &rect, FALSE))
			{
				memset(&paintstruct, 0, sizeof(PAINTSTRUCT));
				BeginPaint(hwnd, &paintstruct);
				try
				{
					Rect cl_rect;
					cl_rect.left = rect.left;
					cl_rect.top = rect.top;
					cl_rect.right = rect.right;
					cl_rect.bottom = rect.bottom;

					// log_event(string_format("Dirty %1", has_drop_shadow ? " Pop" : ""), string_format("Rect: l: %1  t: %2  r: %3  b: %4", cl_rect.left, cl_rect.top, cl_rect.right, cl_rect.bottom));

					if (site)
						(site->sig_paint)(cl_rect);

					EndPaint(hwnd, &paintstruct);
					memset(&paintstruct, 0, sizeof(PAINTSTRUCT));
				}
				catch (...)
				{
					EndPaint(hwnd, &paintstruct);
					memset(&paintstruct, 0, sizeof(PAINTSTRUCT));
					throw;
				}
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
			if (site && (site->func_minimize_clicked))
			{
				if ((site->func_minimize_clicked)())
					return 0;
			}
			break;

		case SC_SCREENSAVE:
			if (!window_desc.get_allow_screensaver())
				return 0;
			break;

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

LRESULT Win32Window::wm_dwm_composition_changed(WPARAM wparam, LPARAM lparam)
{
	update_dwm_settings();
	return DefWindowProc(hwnd, WM_DWMCOMPOSITIONCHANGED, wparam, lparam);
}

void Win32Window::create_new_window()
{
	DwmFunctions::open_dll();

	if (window_desc.get_handle().hwnd)
	{
		hwnd = window_desc.get_handle().hwnd;
		destroy_hwnd = false;
	}
	else
	{
		register_window_class();

		DWORD style, ex_style;
		get_styles_from_description(window_desc, style, ex_style);

		RECT window_rect = get_window_geometry_from_description(window_desc, style, ex_style);

		HWND parent = 0;
		if (!window_desc.get_owner().is_null())
			parent = window_desc.get_owner().get_provider()->get_handle().hwnd;

		hwnd = CreateWindowEx(
			ex_style,
			window_desc.has_drop_shadow() ? TEXT("ClanApplicationDS") : TEXT("ClanApplication"),
			StringHelp::utf8_to_ucs2(window_desc.get_title()).c_str(),
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
			throw Exception("Unable to create window");

		/// This should preceed HWND_TOP setting at fullscreen according to:
		/// http://blogs.msdn.com/b/oldnewthing/archive/2005/11/21/495246.aspx
		if (window_desc.is_topmost())
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

		if (window_desc.is_fullscreen())
		{
			window_style_before_fullscreen = GetWindowLong(hwnd, GWL_STYLE);
			MONITORINFO mi = { sizeof(mi) };
			if (GetWindowPlacement(hwnd, &window_positon_before_fullscreen) &&
				GetMonitorInfo(MonitorFromWindow(hwnd,
				MONITOR_DEFAULTTOPRIMARY), &mi))
			{
				SetWindowPos(hwnd, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
			else
			{
				SetWindowPos(hwnd, HWND_TOP, 
					window_rect.left, window_rect.top, 
					window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, 
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
			SetWindowLong(hwnd, GWL_STYLE, window_style_before_fullscreen & ~WS_OVERLAPPEDWINDOW);
		}

		//update_dwm_settings(); <-- set in WM_CREATE

		if (window_desc.is_visible())
			ShowWindow(hwnd, SW_SHOW);
	}

	connect_window_input(window_desc);
}

void Win32Window::update_dwm_settings()
{
	if (DwmFunctions::is_composition_enabled())
	{
		extend_frame_into_client_area(
			(int)std::round(window_desc.get_extend_frame_left() * pixel_ratio),
			(int)std::round(window_desc.get_extend_frame_top() * pixel_ratio),
			(int)std::round(window_desc.get_extend_frame_right() * pixel_ratio),
			(int)std::round(window_desc.get_extend_frame_bottom() * pixel_ratio));

		if ((window_desc.get_type() != WindowType::normal) || (window_desc.is_layered()))
			set_alpha_channel();
	}
}

void Win32Window::received_keyboard_input(UINT msg, WPARAM wparam, LPARAM lparam)
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
	InputCode key_id = static_cast<InputCode>(wparam);

	// Add to repeat count
	if(keydown)
	{
		if( repeat_count.find(key_id) == repeat_count.end() )
			repeat_count[key_id] = 0;
		else
			repeat_count[key_id]++;
	}

	// Prepare event to be emitted:
	InputEvent key;
	if (keydown)
		key.type = InputEvent::pressed;
	else
		key.type = InputEvent::released;
	key.mouse_pos = Pointf(mouse_pos.x / pixel_ratio, mouse_pos.y / pixel_ratio);
	key.mouse_device_pos = mouse_pos;
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
			key.str = StringHelp::ucs2_to_text(std::wstring(buf, result));
	}

	set_modifier_keys(key);

	// Emit message:
	(*get_keyboard()->sig_provider_event)(key);
}

void Win32Window::received_mouse_input(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Map windows events to something more interesting:
	InputCode id;
	bool up = false;
	bool down = false;
	bool dblclk = false;

	switch (msg)
	{
	case WM_LBUTTONDOWN: id = mouse_left; down = true; break;
	case WM_LBUTTONUP: id = mouse_left; up = true; break;
	case WM_LBUTTONDBLCLK: id = mouse_left; dblclk = true; break;
	case WM_RBUTTONDOWN: id = mouse_right; down = true; break;
	case WM_RBUTTONUP: id = mouse_right; up = true; break;
	case WM_RBUTTONDBLCLK: id = mouse_right; dblclk = true; break;
	case WM_MBUTTONDOWN: id = mouse_middle; down = true; break;
	case WM_MBUTTONUP: id = mouse_middle; up = true; break;
	case WM_MBUTTONDBLCLK: id = mouse_middle; dblclk = true; break;
	case WM_MOUSEWHEEL: id = ((short)HIWORD(wparam) > 0) ? mouse_wheel_up : mouse_wheel_down; up = true; down = true; break;
	case WM_XBUTTONDOWN: id = static_cast<InputCode>(mouse_xbutton1 + HIWORD(wparam) - 1); down = true; break;
	case WM_XBUTTONUP: id = static_cast<InputCode>(mouse_xbutton1 + HIWORD(wparam) - 1); up = true; break;
	case WM_XBUTTONDBLCLK: id = static_cast<InputCode>(mouse_xbutton1 + HIWORD(wparam) - 1); dblclk = true; break;
	default:
		return;
	}
	// Prepare event to be emitted:
	InputEvent key;
	key.mouse_pos = Pointf(mouse_pos.x / pixel_ratio, mouse_pos.y / pixel_ratio);
	key.mouse_device_pos = mouse_pos;
	key.id = id;
	set_modifier_keys(key);

	if (dblclk)
	{
		key.type = InputEvent::doubleclick;

		// Emit message:
		if (id >= 0 && id < 32)
			get_mouse()->key_states[id] = true;

		(*get_mouse()->sig_provider_event)(key);
	}

	if (down)
	{
		key.type = InputEvent::pressed;

		// Emit message:
		if (id >= 0 && id < 32)
			get_mouse()->key_states[id] = true;

		(*get_mouse()->sig_provider_event)(key);
	}

	// It is possible for 2 events to be called when the wheelmouse is used
	if (up)
	{
		key.type = InputEvent::released;

		// Emit message:
		if (id >= 0 && id < 32)
			get_mouse()->key_states[id] = false;

		(*get_mouse()->sig_provider_event)(key);
	}
}

void Win32Window::received_mouse_move(UINT msg, WPARAM wparam, LPARAM lparam)
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
		InputEvent key;
		key.type = InputEvent::pointer_moved;
		key.mouse_pos = Pointf(mouse_pos.x / pixel_ratio, mouse_pos.y / pixel_ratio);
		key.mouse_device_pos = mouse_pos;
		set_modifier_keys(key);

		// Fire off signal
		(*get_mouse()->sig_provider_event)(key);
	}

	if (!cursor_set && !cursor_hidden)
	{
		if (current_cursor == 0)
			set_cursor(StandardCursor::arrow);
		else
			SetCursor(current_cursor);
	}
}

void Win32Window::received_joystick_input(UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_INPUT)
	{
		HRAWINPUT handle = (HRAWINPUT)lparam;
		UINT size = 0;
		GetRawInputData(handle, RID_INPUT, 0, &size, sizeof(RAWINPUTHEADER));
		if (size > 0)
		{
			DataBuffer buffer(size);
			BOOL result = GetRawInputData(handle, RID_INPUT, buffer.get_data(), &size, sizeof(RAWINPUTHEADER));
			if (result)
			{
				RAWINPUT *rawinput = (RAWINPUT*)buffer.get_data();
				if (rawinput->header.dwType == RIM_TYPEMOUSE)
				{
					// To do: generate relative mouse movement events based on HID mouse information.
				}
				else if (rawinput->header.dwType == RIM_TYPEHID)
				{
					for (size_t i = 0; i < joysticks.size(); i++)
					{
						InputDeviceProvider_Win32Hid *hid_provider = dynamic_cast<InputDeviceProvider_Win32Hid*>(joysticks[i].get_provider());
						if (hid_provider)
							hid_provider->update(rawinput);
					}
				}
			}
		}
	}
}

void Win32Window::setup_tablet()
{
	if (get_tablet())
		get_tablet()->dispose();

	tablet = InputDevice(new InputDeviceProvider_Win32Tablet(this));

	if (get_tablet()->device_present())
		ic.add_tablet(tablet);
}

void Win32Window::set_clipboard_text(const std::string &text)
{
	std::wstring text16 = StringHelp::utf8_to_ucs2(text);

	BOOL result = OpenClipboard(hwnd);
	if (result == FALSE)
		throw Exception("Unable to open clipboard");

	result = EmptyClipboard();
	if (result == FALSE)
	{
		CloseClipboard();
		throw Exception("Unable to empty clipboard");
	}

	unsigned int length = (text16.length()+1) * sizeof(std::wstring::value_type);
	HANDLE handle = GlobalAlloc(GMEM_MOVEABLE, length);
	if (handle == 0)
	{
		CloseClipboard();
		throw Exception("Unable to allocate clipboard memory");
	}

	void *data = GlobalLock(handle);
	if (data == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw Exception("Unable to lock clipboard memory");
	}
	memcpy(data, text16.c_str(), length);
	GlobalUnlock(handle);

	HANDLE data_result = SetClipboardData(CF_UNICODETEXT, handle);

	if (data_result == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw Exception("Unable to set clipboard data");
	}

	CloseClipboard();
}

std::string Win32Window::get_clipboard_text() const
{
	BOOL result = OpenClipboard(hwnd);
	if (result == FALSE)
		throw Exception("Unable to open clipboard");

	HANDLE handle = GetClipboardData(CF_UNICODETEXT);
	if (handle == 0)
	{
		CloseClipboard();
		return std::string();
	}

	std::wstring::value_type *data = (std::wstring::value_type *) GlobalLock(handle);
	if (data == 0)
	{
		CloseClipboard();
		return std::string();
	}
	std::string str = StringHelp::ucs2_to_utf8(data);
	GlobalUnlock(handle);

	CloseClipboard();
	return str;
}

bool Win32Window::is_clipboard_text_available() const
{
	return IsClipboardFormatAvailable(CF_UNICODETEXT) == TRUE;
}

bool Win32Window::is_clipboard_image_available() const
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

void Win32Window::set_clipboard_image(const PixelBuffer &image)
{
	BOOL result = OpenClipboard(hwnd);
	if (result == FALSE)
		throw Exception("Unable to open clipboard");

	result = EmptyClipboard();
	if (result == FALSE)
	{
		CloseClipboard();
		throw Exception("Unable to empty clipboard");
	}

	add_dib_to_clipboard(image);
	add_png_to_clipboard(image);

	CloseClipboard();
}


void Win32Window::add_png_to_clipboard(const PixelBuffer &image)
{
	DataBuffer png_data_buf(1024*8);
	MemoryDevice iodev_mem(png_data_buf);
	PNGProvider::save(image, iodev_mem);
	DataBuffer png_data = iodev_mem.get_data();

	unsigned int length = png_data.get_size();
	HANDLE handle = GlobalAlloc(GMEM_MOVEABLE, length);
	if (handle == 0)
	{
		CloseClipboard();
		throw Exception("Unable to allocate clipboard memory");
	}

	char *data = (char *) GlobalLock(handle);
	if (data == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw Exception("Unable to lock clipboard memory");
	}

	memcpy(data, png_data.get_data(), png_data.get_size());

	GlobalUnlock(handle);

	HANDLE data_result = SetClipboardData(png_clipboard_format, handle);

	if (data_result == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw Exception("Unable to set clipboard data");
	}
}

void Win32Window::add_dib_to_clipboard(const PixelBuffer &image)
{
	PixelBuffer bmp_image = create_bitmap_data(image, image.get_size());

	unsigned int length = sizeof(BITMAPV5HEADER) + bmp_image.get_pitch() * bmp_image.get_height();
	HANDLE handle = GlobalAlloc(GMEM_MOVEABLE, length);
	if (handle == 0)
	{
		CloseClipboard();
		throw Exception("Unable to allocate clipboard memory");
	}

	char *data = (char *) GlobalLock(handle);
	if (data == 0)
	{
		GlobalFree(handle);
		CloseClipboard();
		throw Exception("Unable to lock clipboard memory");
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
		throw Exception("Unable to set clipboard data");
	}
}

PixelBuffer Win32Window::create_bitmap_data(const PixelBuffer &image, const Rect &rect)
{
	if (rect.left < 0 || rect.top < 0 || rect.right > image.get_width(), rect.bottom > image.get_height())
		throw Exception("Rectangle passed to Win32Window::create_bitmap_data() out of bounds");

	// Convert pixel buffer to DIB compatible format:
	int pitch = 4;
	PixelBuffer bmp_image(rect.get_width(), rect.get_height(), tf_bgra8);

	bmp_image.set_subimage(image, Point(0, 0), rect);
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

HBITMAP Win32Window::create_bitmap(HDC hdc, const PixelBuffer &image)
{
	PixelBuffer bmp_image = create_bitmap_data(image, image.get_size());

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

HICON Win32Window::create_icon(const PixelBuffer &image) const
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

void Win32Window::request_repaint( const Rect &cl_rect )
{
	RECT rect;
	rect.left = cl_rect.left;
	rect.right = cl_rect.right;
	rect.top = cl_rect.top;
	rect.bottom = cl_rect.bottom;

	InvalidateRect(hwnd, &rect, false);
}

void Win32Window::set_minimum_size( int width, int height, bool client_area)
{
	if (client_area)
		throw Exception("Congratulations! You just got assigned the task of adding support for client area in Win32Window::set_minimum_size(...).");

	this->minimum_size = Size(width,height);
}

void Win32Window::set_maximum_size( int width, int height, bool client_area)
{
	if (client_area)
		throw Exception("Congratulations! You just got assigned the task of adding support for client area in Win32Window::set_maximum_size(...).");

	this->maximum_size = Size(width,height);
}

void Win32Window::set_pixel_ratio(float ratio)
{
	pixel_ratio = ratio;
}

PixelBuffer Win32Window::get_clipboard_image() const
{
	BOOL result = OpenClipboard(hwnd);
	if (result == FALSE)
		throw Exception("Unable to open clipboard");

	UINT format = EnumClipboardFormats(0);
	UINT png_format = 0;
	while (format)
	{
		WCHAR szFormatName[80];
		int retLen = GetClipboardFormatName(format, szFormatName, sizeof(szFormatName));

		if (std::wstring(L"image/png") == szFormatName ||
			std::wstring(L"PNG") == szFormatName)
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
			uint8_t *data = reinterpret_cast<uint8_t *>(GlobalLock(handle));
			size_t size = GlobalSize(handle);

			PixelBuffer image = get_argb8888_from_png(data, size);

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

			PixelBuffer image;
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
	return PixelBuffer();
}

PixelBuffer Win32Window::get_argb8888_from_rgb_dib(BITMAPV5HEADER *bitmapInfo, size_t size) const
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
		throw Exception("Out of bounds in get_rgba8888_from_dib!");

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
	DataBuffer bitmap_data(pitch * abs(rgbBitmapInfo.bV5Height));
	int scanlines = GetDIBits(hdc, bitmap, 0, abs(bitmapInfo->bV5Height), bitmap_data.get_data(), (LPBITMAPINFO) &rgbBitmapInfo, DIB_RGB_COLORS);
	if (scanlines != abs(bitmapInfo->bV5Height))
		throw Exception("GetDIBits failed");

	// GetDIBits above sets the alpha channel to 0 - need to convert it to 255.
	uint8_t *data = (uint8_t *)bitmap_data.get_data();
	for (int y=0; y<abs(bitmapInfo->bV5Height); y++)
	{
		for (int x=0; x<abs(bitmapInfo->bV5Width); x++)
		{
			data[y*pitch + x*4 + 3] = 255;
		}
	}


	PixelBuffer pixelbuffer(rgbBitmapInfo.bV5Width, abs(rgbBitmapInfo.bV5Height), tf_bgra8, bitmap_data.get_data());

	ReleaseDC(0, hdc);

	if (bitmapInfo->bV5Height > 0)
		flip_pixelbuffer_vertical(pixelbuffer);

	return pixelbuffer;
}


PixelBuffer Win32Window::get_argb8888_from_bitfields_dib(BITMAPV5HEADER *bitmapInfo, size_t size) const
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
		throw Exception("Out of bounds in get_rgba8888_from_dib!");

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
	DataBuffer bitmap_data(pitch * abs(rgbBitmapInfo.bV5Height));
	int scanlines = GetDIBits(hdc, bitmap, 0, abs(bitmapInfo->bV5Height), bitmap_data.get_data(), (LPBITMAPINFO) &rgbBitmapInfo, DIB_RGB_COLORS);
	if (scanlines != abs(bitmapInfo->bV5Height))
		throw Exception("GetDIBits failed");

	// GetDIBits above sets the alpha channel to 0 - need to convert it to 255.
	uint8_t *data = (uint8_t *)bitmap_data.get_data();
	for (int y=0; y<abs(bitmapInfo->bV5Height); y++)
	{
		for (int x=0; x<abs(bitmapInfo->bV5Width); x++)
		{
			data[y*pitch + x*4 + 3] = 255;
		}
	}


	PixelBuffer pixelbuffer(rgbBitmapInfo.bV5Width, abs(rgbBitmapInfo.bV5Height), tf_bgra8, bitmap_data.get_data());

	ReleaseDC(0, hdc);

	if (bitmapInfo->bV5Height > 0)
		flip_pixelbuffer_vertical(pixelbuffer);

	return pixelbuffer;
}



void Win32Window::flip_pixelbuffer_vertical(PixelBuffer &pbuf) const
{
	uint8_t *data = (uint8_t*)pbuf.get_data();

	for (int y=0; y<(pbuf.get_height()/2); y++)
	{
		uint32_t *dy = (uint32_t*)(data + (y*pbuf.get_pitch()));
		uint32_t *dy2 = (uint32_t*)(data + (pbuf.get_height()-y-1)*pbuf.get_pitch());

		for (int x=0; x<pbuf.get_width(); x++)
		{
			uint32_t tmp = dy[x];
			dy[x] = dy2[x];
			dy2[x] = tmp;
		}
	}
}

PixelBuffer Win32Window::get_argb8888_from_png(uint8_t *data, size_t size) const
{
	DataBuffer data_buffer(data, size);
	MemoryDevice iodev(data_buffer);
	PixelBuffer pbuf = PNGProvider::load(iodev);
	return pbuf;
}

void Win32Window::register_clipboard_formats()
{
//	WCHAR *png_format_str = L"image/png";
	WCHAR *png_format_str = L"PNG";
	png_clipboard_format = RegisterClipboardFormat(png_format_str);
}

void Win32Window::set_large_icon(const PixelBuffer &image)
{
	if (large_icon)
		DestroyIcon(large_icon);
	large_icon = 0;
	large_icon = create_icon(image);

	SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(large_icon));
}

void Win32Window::set_small_icon(const PixelBuffer &image)
{
	if (small_icon)
		DestroyIcon(small_icon);
	small_icon = 0;
	small_icon = create_icon(image);

	SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(small_icon));
}

void Win32Window::set_modifier_keys(InputEvent &key)
{
	key.alt = (GetKeyState(VK_MENU) & 0xfe) != 0;
	key.shift = (GetKeyState(VK_SHIFT) & 0xfe) != 0;
	key.ctrl = (GetKeyState(VK_CONTROL) & 0xfe) != 0;
}

InputDeviceProvider_Win32Keyboard *Win32Window::get_keyboard()
{
	return static_cast<InputDeviceProvider_Win32Keyboard *>(keyboard.get_provider());
}

InputDeviceProvider_Win32Mouse *Win32Window::get_mouse()
{
	return static_cast<InputDeviceProvider_Win32Mouse *>(mouse.get_provider());
}

InputDeviceProvider_Win32Tablet *Win32Window::get_tablet()
{
	return static_cast<InputDeviceProvider_Win32Tablet *>(tablet.get_provider());
}

void Win32Window::register_window_class()
{
	static bool first_call = true;
	if (first_call)
	{
		WNDCLASS wndclass;
		memset(&wndclass, 0, sizeof(WNDCLASS));
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // 0;
		wndclass.lpfnWndProc = (WNDPROC) Win32Window::static_window_proc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = (HINSTANCE) GetModuleHandle(0);
		wndclass.hIcon = 0;
		wndclass.hCursor = 0;
		wndclass.hbrBackground = CreateSolidBrush(RGB(255,255,255)); // Todo: add DisplayWindow::set_dwm_clear_color(const Colorf &color)
		wndclass.lpszMenuName = 0;
		wndclass.lpszClassName = TEXT("ClanApplication");
		RegisterClass(&wndclass);

		memset(&wndclass, 0, sizeof(WNDCLASS));

		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		if (allow_dropshadow)
		{
			wndclass.style |= CS_DROPSHADOW;
		}

		wndclass.lpfnWndProc = (WNDPROC) Win32Window::static_window_proc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = (HINSTANCE) GetModuleHandle(0);
		wndclass.hIcon = 0;
		wndclass.hCursor = 0;
		wndclass.hbrBackground = CreateSolidBrush(RGB(255,255,255)); // Todo: add DisplayWindow::set_dwm_clear_color(const Colorf &color)
		wndclass.lpszMenuName = 0;
		wndclass.lpszClassName = TEXT("ClanApplicationDS");
		RegisterClass(&wndclass);

		first_call = false;
	}
}

void Win32Window::connect_window_input(const DisplayWindowDescription &desc)
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

	for (size_t i = 0; i < joysticks.size(); i++)
		joysticks[i].get_provider()->dispose();
	joysticks.clear();

	ic.add_keyboard(keyboard);
	ic.add_mouse(mouse);

	create_hid_devices();

	if (desc.get_tablet_context())
		setup_tablet();
}

void Win32Window::create_hid_devices()
{
	UINT num_devices = 0;
	UINT result = GetRawInputDeviceList(0, &num_devices, sizeof(RAWINPUTDEVICELIST));
	if (result != (UINT)-1 && num_devices > 0)
	{
		std::vector<RAWINPUTDEVICELIST> device_list(num_devices);
		result = GetRawInputDeviceList(&device_list[0], &num_devices, sizeof(RAWINPUTDEVICELIST));
		if (result == (UINT)-1)
			throw Exception("GetRawInputDeviceList failed");

		for (size_t i = 0; i < device_list.size(); i++)
		{
			RID_DEVICE_INFO device_info;
			UINT device_info_size = sizeof(RID_DEVICE_INFO);
			device_info.cbSize = device_info_size;
			result = GetRawInputDeviceInfo(device_list[i].hDevice, RIDI_DEVICEINFO, &device_info, &device_info_size);
			if (result == (UINT)-1)
				throw Exception("GetRawInputDeviceInfo failed");

			if (device_info.dwType == RIM_TYPEHID)
			{
				if (device_info.hid.usUsagePage == HID_USAGE_PAGE_GENERIC && (device_info.hid.usUsage == HID_USAGE_GENERIC_JOYSTICK || device_info.hid.usUsage == HID_USAGE_GENERIC_GAMEPAD))
				{
					try
					{
						InputDevice device(new InputDeviceProvider_Win32Hid(device_list[i].hDevice));
						joysticks.push_back(device);
						ic.add_joystick(device);
					}
					catch (const Exception& error)
					{
						log_event("debug", "Could not setup game controller: %1", error.message);
					}
				}
			}
		}
	}
}

void Win32Window::get_styles_from_description(const DisplayWindowDescription &desc, DWORD &out_style, DWORD &out_ex_style)
{

	out_style = 0;
	out_ex_style = 0;

	WindowType type = desc.get_type();

	if (desc.is_fullscreen() || type == WindowType::popup || !desc.has_caption())
		out_style |= WS_POPUP;

	if (desc.get_allow_resize() && !desc.is_fullscreen() && type != WindowType::popup)
		out_style |= WS_SIZEBOX;

	if (desc.has_caption())
	{
		out_style |= WS_CAPTION;

		if (desc.has_sysmenu())
		{
			out_style |= WS_SYSMENU;
		}
		if (type != WindowType::popup)
		{
			if (desc.has_minimize_button())
				out_style |= WS_MINIMIZEBOX;
			if (desc.has_maximize_button() && desc.get_allow_resize())
				out_style |= WS_MAXIMIZEBOX;
		}
	}

	if (desc.is_layered())
	{
		if (!DwmFunctions::is_composition_enabled())
		{
			out_ex_style |= WS_EX_LAYERED;
		}
	}

	if (desc.has_no_activate())
		out_ex_style |= WS_EX_NOACTIVATE;

	if (type == WindowType::tool)
		out_ex_style |= WS_EX_TOOLWINDOW;
}

RECT Win32Window::get_window_geometry_from_description(const DisplayWindowDescription &desc, DWORD style, DWORD ex_style)
{
	int x = (int)std::round(desc.get_position().left * pixel_ratio);
	int y = (int)std::round(desc.get_position().top * pixel_ratio);
	int width = (int)std::round(desc.get_size().width * pixel_ratio);
	int height = (int)std::round(desc.get_size().height * pixel_ratio);

	bool clientSize = desc.get_position_client_area();	// false = Size includes the window frame. true = Size is the drawable size.

	if (desc.is_fullscreen())
	{
		int primary_screen = 0;
		ScreenInfo screen_info;
		std::vector<Rectf> screen_rects = screen_info.get_screen_geometries(primary_screen);
		Rectf R = screen_rects[desc.get_fullscreen_monitor()];

		clientSize = false;
		x = (int)std::round(R.left * pixel_ratio);
		y = (int)std::round(R.top * pixel_ratio);
		width = (int)std::round(R.get_width() * pixel_ratio);
		height = (int)std::round(R.get_height() * pixel_ratio);
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

void Win32Window::update_layered(PixelBuffer &image)
{
	if (!update_window_worker_thread_started)
	{
		update_window_worker_thread = std::thread(&Win32Window::update_layered_worker_thread, this);
		update_window_worker_thread_started = true;
	}
	else
	{
		std::unique_lock<std::mutex> lock(update_window_mutex);
		update_window_main_event.wait(lock, [&]() { return update_window_completed_flag; });
		update_window_completed_flag = false;
		lock.unlock();

		// SetWindowRgn can only be called from the main thread, not a worker thread.
		// The region is one frame behind, but nobody will notice!
		if (update_window_region)
			if (SetWindowRgn(hwnd, update_window_region, FALSE) == 0)
				DeleteObject(update_window_region);

		update_window_region = 0;
	}
	update_window_image = image;

	std::unique_lock<std::mutex> lock(update_window_mutex);
	update_window_start_flag = true;
	lock.unlock();
	update_window_worker_event.notify_one();
}

void Win32Window::update_layered_worker_thread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(update_window_mutex);
		update_window_worker_event.wait(lock, [&]() { return update_window_start_flag || update_window_stop_flag; });
		if (update_window_stop_flag)
			break;
		update_window_start_flag = false;
		lock.unlock();

		if (DwmFunctions::is_composition_enabled())
		{
			update_layered_worker_thread_process_dwm();
		}
		else
		{
			update_layered_worker_thread_process();
		}

		lock.lock();
		update_window_image = PixelBuffer();
		update_window_completed_flag = true;
		lock.unlock();
		update_window_main_event.notify_one();
	}
}

void Win32Window::update_layered_worker_thread_process_dwm()
{
	int width = update_window_image.get_width();
	int height = update_window_image.get_height();
	const unsigned char *pixels = update_window_image.get_data_uint8();

	RECT bbox;
	bbox.left =   0x7ffffff;
	bbox.right =  0x8000000;
	bbox.top =    0x7ffffff;
	bbox.bottom = 0x8000000;

	std::vector<RECT> rects;
	rects.reserve(update_window_max_region_rects);
	for (int y = 0; y < height; y++)
	{
		int start = 0;
		bool transparent = true;
		for (int x = 0; x < width; x++)
		{
			if (transparent)
			{
				if (pixels[x + y * width] != 0)
				{
					start = x;
					transparent = false;
				}
			}
			else
			{
				if (pixels[x + y * width] == 0)
				{
					RECT r;
					r.left = start;
					r.right = x;
					r.bottom = height - y;
					r.top = r.bottom - 1;

					bbox.left = min(bbox.left, r.left);
					bbox.top = min(bbox.top, r.top);
					bbox.right = max(bbox.right, r.right);
					bbox.bottom = max(bbox.bottom, r.bottom);

					rects.push_back(r);
					transparent = true;
				}
			}
		}
		if (!transparent)
		{
			RECT r;
			r.left = start;
			r.right = width;
			r.bottom = height - y;
			r.top = r.bottom - 1;

			bbox.left = min(bbox.left, r.left);
			bbox.top = min(bbox.top, r.top);
			bbox.right = max(bbox.right, r.right);
			bbox.bottom = max(bbox.bottom, r.bottom);

			rects.push_back(r);
		}
	}


	if (rects.empty())
	{
		bbox.left =   0;
		bbox.right =  0;
		bbox.top =    0;
		bbox.bottom = 0;
		rects.push_back(bbox);
	}

	// Calculate estimated memory to reserve for next iteration
	if (update_window_max_region_rects < rects.size())
		update_window_max_region_rects = rects.size() + 1024;

	int size = sizeof(RGNDATAHEADER) + sizeof(RECT) * rects.size();
	DataBuffer region_data_buffer(size);
	RGNDATA *region_data = (RGNDATA*)region_data_buffer.get_data();
	region_data->rdh.dwSize = sizeof(RGNDATAHEADER);
	region_data->rdh.iType = RDH_RECTANGLES;
	region_data->rdh.nCount = rects.size();
	region_data->rdh.nRgnSize = sizeof(RECT) * rects.size();
	region_data->rdh.rcBound = bbox;
	RECT *region_data_rects = (RECT*)region_data->Buffer;
	for (size_t i = 0; i < rects.size(); i++)
		region_data_rects[i] = rects[i];

	update_window_region = ExtCreateRegion(0, region_data_buffer.get_size(), region_data);
	// SetWindowRgn() is called in update_layered()

}

void Win32Window::update_layered_worker_thread_process()
{
	// Note that the APIs use pre-multiplied alpha, which means that the red,
	// green and blue channel values in the bitmap must be pre-multiplied with
	// the alpha channel value. For example, if the alpha channel value is x,
	// the red, green and blue channels must be multiplied by x and divided by
	// 0xff prior to the call.

	// ClanLib now uses pre-multiplied alpha

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
}

void Win32Window::enable_alpha_channel(const Rect &blur_rect)
{
	window_blur_rect = blur_rect;
	set_alpha_channel();
}

void Win32Window::set_alpha_channel()
{
	if (window_blur_rect.get_width() == 0)
	{
		DwmFunctions::enable_alpha_channel(hwnd, 0);
	}
	else
	{
		HRGN enable_alpha_region = ::CreateRectRgn(window_blur_rect.left, window_blur_rect.top, window_blur_rect.right, window_blur_rect.bottom);
		DwmFunctions::enable_alpha_channel(hwnd, enable_alpha_region);
		DeleteObject(enable_alpha_region);
	}

}

void Win32Window::extend_frame_into_client_area(int left, int top, int right, int bottom)
{
	DwmFunctions::extend_frame_into_client_area(hwnd, left, top, right, bottom);
}

LRESULT Win32Window::wm_nc_hittest(WPARAM wparam, LPARAM lparam)
{
	if (window_desc.get_type() != WindowType::custom)
		return DefWindowProc(hwnd, WM_NCHITTEST, wparam, lparam);

	// Hit test handling for the resize + caption draggable area
	POINT mousePos = { LOWORD(lparam), HIWORD(lparam) };

	RECT window_box;
	GetWindowRect(hwnd, &window_box);

	int xGrid[6] = { window_box.left, window_box.left + 8, window_box.left + 16, window_box.right - 16, window_box.right - 8, window_box.right };
	int yGrid[6] = { window_box.top, window_box.top + 4, window_box.top + 16, window_box.bottom - 16, window_box.bottom - 8, window_box.bottom };

	int xIndex = 2;
	for (int i = 0; i < 5; i++)
	{
		if (mousePos.x >= xGrid[i] && mousePos.x < xGrid[i + 1])
		{
			xIndex = i;
			break;
		}
	}

	int yIndex = 2;
	for (int i = 0; i < 5; i++)
	{
		if (mousePos.y >= yGrid[i] && mousePos.y < yGrid[i + 1])
		{
			yIndex = i;
			break;
		}
	}

	long hit = HTNOWHERE;
	switch (xIndex + yIndex * 5)
	{
	case 0: hit = HTTOPLEFT; break;
	case 1: hit = HTTOPLEFT; break;
	case 2: hit = HTTOP; break;
	case 3: hit = HTTOPRIGHT; break;
	case 4: hit = HTTOPRIGHT; break;
	case 5: hit = HTTOPLEFT; break;
	case 9: hit = HTTOPRIGHT; break;
	case 10: hit = HTLEFT; break;
	case 14: hit = HTRIGHT; break;
	case 15: hit = HTBOTTOMLEFT; break;
	case 19: hit = HTBOTTOMRIGHT; break;
	case 20: hit = HTBOTTOMLEFT; break;
	case 21: hit = HTBOTTOMLEFT; break;
	case 22: hit = HTBOTTOM; break;
	case 23: hit = HTBOTTOMRIGHT; break;
	case 24: hit = HTBOTTOMRIGHT; break;
	default: break;
	}

	if (hit == HTNOWHERE)
	{
		if (mousePos.y < window_box.top + TOPEXTENDWIDTH)
		{
			hit = HTCAPTION;
		}
		else
		{
			hit = HTCLIENT;
		}
	}

	return hit;
}

LRESULT Win32Window::wm_nc_calcsize(WPARAM wparam, LPARAM lparam)
{
	if (wparam == FALSE)
	{
		NCCALCSIZE_PARAMS *params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);

		RECT &window_box = params->rgrc[0]; // proposed new window coordinates
		const RECT &old_window_box = params->rgrc[1];
		const RECT &old_client_box = params->rgrc[2];

		// Result indicates which part of the client area contains valid information or have to be redrawn
		return WVR_HREDRAW | WVR_VREDRAW;
	}
	else
	{
		if (window_desc.get_type() == WindowType::normal)
			return DefWindowProc(hwnd, WM_NCCALCSIZE, wparam, lparam);

		// On entry, the structure contains the proposed window rectangle for the window.
		RECT window_box = *reinterpret_cast<RECT*>(lparam);

		RECT client_box = window_box;

		// On exit, the structure should contain the screen coordinates of the corresponding window client area.
		*reinterpret_cast<RECT*>(lparam) = client_box;

		return 0; // must always be 0
	}
}

LRESULT Win32Window::wm_create(WPARAM wparam, LPARAM lparam)
{
	update_dwm_settings();
	resend_nccalcsize();
	return DefWindowProc(hwnd, WM_CREATE, wparam, lparam);
}
void Win32Window::resend_nccalcsize()
{
	RECT box = { 0 };
	GetWindowRect(hwnd, &box);
	SetWindowPos(hwnd, 0, box.left, box.top, box.right - box.left, box.bottom - box.top, SWP_FRAMECHANGED|SWP_NOACTIVATE);
}

}

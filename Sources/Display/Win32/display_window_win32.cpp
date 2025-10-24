/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "dinput.h"
#include "Display/display_precomp.h"
#include "display_window_win32.h"
#include "input_device_win32keyboard.h"
#include "input_device_win32mouse.h"
#include "input_device_directinput.h"
#include "Core/System/Win32/init_win32.h"
#include "API/Display/input_event.h"
#include "API/Display/keys.h"
#include "API/Display/display_window_description.h"
#include "API/Core/Math/rect.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/log.h"

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL    0x020A
#endif

#ifndef WM_XBUTTONDOWN
#define WM_XBUTTONDOWN   0x020B
#define WM_XBUTTONUP     0x020C
#define WM_XBUTTONDBLCLK 0x020D
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif

// My dinput.lib seem to be broken (the one with msvc7).
// It gives me a linker error about DirectInput8Create. Easier to create with cocreateinstance than
// download hundred megabytes. :) -- mbn 5 oct 2003
#define BROKEN_DINPUT

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow_Win32 construction:

CL_DisplayWindow_Win32::CL_DisplayWindow_Win32()
: hwnd(NULL), title("nonamed"), fullscreen(false),
	allow_resize(false), layered(false), bpp(0), saved_position(0, 0, 0, 0),
	left_ctrl_down(false), left_alt_down(false), left_shift_down(false),
	right_ctrl_down(false), right_alt_down(false), right_shift_down(false),
	mouse_pos(-1,-1), directinput(0), repeat_count(0), previous_key_down(0)
{
	keyboard = CL_InputDevice(new CL_InputDevice_Win32Keyboard(this));
	mouse = CL_InputDevice(new CL_InputDevice_Win32Mouse(this));

	memset(keys_down, 0, 256);

#ifndef BROKEN_DINPUT
	HRESULT result = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *) &directinput, 0);
	if (FAILED(result))
	{
		CL_Log::log("debug", "Unable to initialize direct input");
	}
#else
	// My directinput doesnt know this directinput8create function (hmm weird)
	// So creating it via CoCreateInstance instead..

	HRESULT result = CoInitialize(0);
	if (FAILED(result))
	{
		throw CL_Error("CL_DisplayWindow_Win32: Damn murphy must hate you. CoInitialize failed!");
	}

	result = CoCreateInstance(CLSID_DirectInput8, 0, CLSCTX_INPROC_SERVER, IID_IDirectInput8A, (LPVOID *) &directinput);
	if (FAILED(result))
	{
		CL_Log::log("debug", "Unable to create direct input");
	}
	else
	{
		result = directinput->Initialize(GetModuleHandle(0), DIRECTINPUT_VERSION);
		if (FAILED(result))
		{
			directinput->Release();
			directinput = 0;

			CL_Log::log("debug", "Unable to initialize direct input");
		}
	}
#endif
}

CL_DisplayWindow_Win32::~CL_DisplayWindow_Win32()
{
	if (hwnd) destroy_window();
#ifndef BROKEN_DINPUT
	if (directinput) directinput->Release();
#else
	if (directinput) directinput->Release();
	CoUninitialize();
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow_Win32 attributes:

std::map<HWND, CL_DisplayWindow_Win32 *> CL_DisplayWindow_Win32::window_map;

int CL_DisplayWindow_Win32::get_width() const
{
	if (layered) return layered_position.get_width();

	RECT rect;
	GetClientRect(hwnd, &rect);
	return rect.right - rect.left;
}

int CL_DisplayWindow_Win32::get_height() const
{
	if (layered) return layered_position.get_height();

	RECT rect;
	GetClientRect(hwnd, &rect);
	return rect.bottom - rect.top;
}

bool CL_DisplayWindow_Win32::is_fullscreen() const
{
	return fullscreen;
}

bool CL_DisplayWindow_Win32::has_focus() const
{
	return (GetFocus() == hwnd);
}

int CL_DisplayWindow_Win32::get_buffer_count() const
{
	return 1;
}

bool CL_DisplayWindow_Win32::get_keycode(int keycode) const
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow_Win32 operations:

void CL_DisplayWindow_Win32::set_fullscreen(int width, int height, int bpp, int refresh_rate)
{
	CL_DisplayWindowDescription desc;
	desc.set_title(title);
	desc.set_size(CL_Size(width, height));
	desc.set_bpp(bpp);
	desc.set_refresh_rate(refresh_rate);
	desc.set_allow_resize(allow_resize);
	desc.set_fullscreen(true);
	desc.set_layered(layered);
	create_window(desc);
}

void CL_DisplayWindow_Win32::set_windowed()
{
	if (!fullscreen) return;

	CL_DisplayWindowDescription desc;
	desc.set_title(title);
    desc.set_size(CL_Size(get_width(), get_height()));
	desc.set_allow_resize(allow_resize);
	desc.set_layered(layered);
	create_window(desc);
}

void CL_DisplayWindow_Win32::set_title(const std::string &_title)
{
	title = _title;
	SetWindowText(hwnd, title.c_str());
}

void CL_DisplayWindow_Win32::set_position(const CL_Rect &pos)
{
	RECT rect, clientRect;
	GetWindowRect(hwnd, &rect);
	GetClientRect(hwnd, &clientRect);
	int windowWidth = pos.get_width()+(rect.right-rect.left)-(clientRect.right-clientRect.left);
	int windowHeight = pos.get_height()+(rect.bottom-rect.top)-(clientRect.bottom-clientRect.top);

	layered_position = pos;
	SetWindowPos(hwnd, 0, pos.left, pos.top, windowWidth, windowHeight, SWP_NOREPOSITION|SWP_NOZORDER);
}

void CL_DisplayWindow_Win32::set_position(int x, int y)
{
	CL_Size size = layered_position.get_size();
	layered_position = CL_Rect(x, y, x+size.width, y+size.height);

	SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOREPOSITION|SWP_NOZORDER|SWP_NOSIZE);
}

void CL_DisplayWindow_Win32::set_size(int width, int height)
{
	layered_position.set_size(CL_Size(width, height));

	RECT rect, clientRect;
	GetWindowRect(hwnd, &rect);
	GetClientRect(hwnd, &clientRect);
	int windowWidth = width+(rect.right-rect.left)-(clientRect.right-clientRect.left);
	int windowHeight = height+(rect.bottom-rect.top)-(clientRect.bottom-clientRect.top);

	SetWindowPos(hwnd, 0, 0, 0, windowWidth, windowHeight, SWP_NOREPOSITION|SWP_NOZORDER|SWP_NOMOVE);
}

void CL_DisplayWindow_Win32::set_buffer_count(int flipping_buffers)
{
}

void CL_DisplayWindow_Win32::update(const CL_Rect &rect)
{
}

void CL_DisplayWindow_Win32::flip(int interval)
{
}

void CL_DisplayWindow_Win32::create_window(const CL_DisplayWindowDescription &desc)
{
	if (hwnd)
	{
		modify_window(desc);
	}
	else
	{
		create_new_window(desc);
	}
}

void CL_DisplayWindow_Win32::modify_window(const CL_DisplayWindowDescription &desc)
{
	bool clientSize = true;

	if (!fullscreen)
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		saved_position.left = rect.left;
		saved_position.top = rect.top;
		saved_position.right = rect.right;
		saved_position.bottom = rect.bottom;
	}

	title = desc.get_title();
	fullscreen = desc.is_fullscreen();
	layered = desc.is_layered();
	layered_position = desc.get_position();
	allow_resize = desc.get_allow_resize();

	int x = 0;
	int y = 0;
	int width = desc.get_size().width;
	int height = desc.get_size().height;

	if (desc.is_fullscreen())
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		int scr_width = GetSystemMetrics(SM_CXSCREEN);
		int scr_height = GetSystemMetrics(SM_CYSCREEN);

		x = scr_width/2 - width/2;
		y = scr_height/2 - height/2;
	}

	if (width == 0 && height == 0)
	{
		x = saved_position.left;
		y = saved_position.top;
		width = saved_position.get_width();
		height = saved_position.get_height();
		clientSize = false;
	}

	int style;
	if (desc.is_fullscreen())
	{
		style = WS_POPUP;
	}
	else if (allow_resize)
	{
		style = WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
	}
	else
	{
		style = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
	}

	int ex_style = 0;
	if (desc.is_layered())
	{
		ex_style = WS_EX_LAYERED;
	}

	// get size of window with decorations to pass to CreateWindow
	RECT window_rect = { x, y, x+width, y+height };
	if (clientSize)
		AdjustWindowRectEx( &window_rect, style, FALSE, ex_style );

	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowLong(hwnd, GWL_EXSTYLE, ex_style);

	if (desc.is_fullscreen())
	{
		// Make always on top
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, width, height, SWP_FRAMECHANGED);
	}
	else
	{
		// Clear always on top flag; size as requested by description struct.
		SetWindowPos(
			hwnd,
			HWND_NOTOPMOST,
			window_rect.left,
			window_rect.top,
			window_rect.right-window_rect.left,
			window_rect.bottom-window_rect.top,
			SWP_FRAMECHANGED);
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	RedrawWindow(0, 0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_UPDATENOW);
}

void CL_DisplayWindow_Win32::create_new_window(const CL_DisplayWindowDescription &desc)
{
	bool clientSize = true;

	title = desc.get_title();
	fullscreen = desc.is_fullscreen();
	layered = desc.is_layered();
	layered_position = desc.get_position();
	allow_resize = desc.get_allow_resize();

	WNDCLASS wndclass;

	wndclass.style = CS_DBLCLKS;
	wndclass.lpfnWndProc = (WNDPROC) CL_DisplayWindow_Win32::message_handler;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = CL_System_Win32::hInstance;
	wndclass.hIcon = LoadIcon(CL_System_Win32::hInstance, MAKEINTRESOURCE(1001));
	wndclass.hCursor = LoadCursor (NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndclass.lpszMenuName = "ClanApplication";
	wndclass.lpszClassName = "ClanApplication";

	RegisterClass(&wndclass);

	int x = 0;
	int y = 0;
	int width = desc.get_size().width;
	int height = desc.get_size().height;

	if (desc.is_fullscreen())
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		int scr_width = GetSystemMetrics(SM_CXSCREEN);
		int scr_height = GetSystemMetrics(SM_CYSCREEN);

		x = scr_width/2 - width/2;
		y = scr_height/2 - height/2;
	}

	if (width == 0 && height == 0)
	{
		x = saved_position.left;
		y = saved_position.top;
		width = saved_position.get_width();
		height = saved_position.get_height();
		clientSize = false;
	}

	int style;
	if (desc.is_fullscreen())
	{
		style = WS_POPUP;
	}
	else if (allow_resize)
	{
		style = WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
	}
	else
	{
		style = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
	}

	int ex_style = 0;
	if (desc.is_layered())
	{
		ex_style = WS_EX_LAYERED;
	}

	// get size of window with decorations to pass to CreateWindow
	RECT window_rect = { x, y, x+width, y+height };
	if (clientSize)
		AdjustWindowRectEx( &window_rect, style, FALSE, ex_style );

	hwnd = CreateWindowEx(
		ex_style,
		"ClanApplication",
		title.c_str(),
		style,
		window_rect.left,
		window_rect.top,
		window_rect.right - window_rect.left,
		window_rect.bottom - window_rect.top,
		NULL,
		NULL,
		CL_System_Win32::hInstance,
		NULL);

	window_map[hwnd] = this; // register window in "hwnd to window" hash table

	cl_assert(hwnd != NULL);

	if (desc.is_fullscreen())
	{
		// Make always on top
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, width, height, 0); 
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// Connect input context to new window:

	input_context.clear();
	input_context.add_keyboard(keyboard);
	input_context.add_mouse(mouse);

	// Go looking for joysticks:
	if (directinput)
	{
		HRESULT result = directinput->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			&CL_DisplayWindow_Win32::enum_devices_callback,
			this,
			DIEDFL_ATTACHEDONLY);
		if (FAILED(result))
		{
			CL_Log::log("debug", "Unable to enumerate direct input devices");
		}
	}
}

BOOL CL_DisplayWindow_Win32::enum_devices_callback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	CL_DisplayWindow_Win32 *self = (CL_DisplayWindow_Win32 *) pvRef;
	try
	{
		CL_InputDevice device(new CL_InputDevice_DirectInput(self, lpddi));
		self->input_context.add_joystick(device);
	}
	catch (CL_Error error)
	{
		CL_Log::log("debug", "Could not setup game controller: %1", error.message);
	}
	return TRUE;
}

void CL_DisplayWindow_Win32::destroy_window()
{
	if (hwnd == NULL) return;

	fullscreen = false;
	layered = false;

	input_context.clear();

	BOOL res = DestroyWindow(hwnd);
	cl_assert(res == TRUE);
	hwnd = NULL;
}

void CL_DisplayWindow_Win32::show_system_cursor()
{
	ShowCursor(TRUE);
}

void CL_DisplayWindow_Win32::hide_system_cursor()
{
	ShowCursor(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow_Win32 implementation:

LONG WINAPI CL_DisplayWindow_Win32::message_handler(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	// Lookup window and call its event handler:
 	std::map<HWND, CL_DisplayWindow_Win32 *>::iterator it = window_map.find(hWnd);
	if (it != window_map.end())
	{
		bool handled = it->second->received_event(uMsg, wParam, lParam);
		if (handled) return TRUE;
	}

	// Do default window processing if our event handler didnt handle it:
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool CL_DisplayWindow_Win32::received_event(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		received_keyboard_input(uMsg, wParam, lParam);
		return true;

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
		received_mouse_input(uMsg, wParam, lParam);
		return true;

	case WM_MOUSEMOVE:
		received_mouse_move(uMsg, wParam, lParam);
		return true;

	case WM_SIZE:
		sig_resize(LOWORD(lParam), HIWORD(lParam));
		return true;

	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
			sig_lost_focus();
		else
			sig_got_focus();
		return true;

	case WM_CLOSE:
		sig_window_close();
		return true;

	case WM_PAINT:
		{
//			HDC hdc = (HDC) wParam;
			RECT rect;
			if (GetUpdateRect(hwnd, &rect, FALSE))
			{
				PAINTSTRUCT paint;
				BeginPaint(hwnd, &paint);

				CL_Rect cl_rect;
				cl_rect.left = rect.left;
				cl_rect.top = rect.top;
				cl_rect.right = rect.right;
				cl_rect.bottom = rect.bottom;

				sig_paint(cl_rect);

				EndPaint(hwnd, &paint);
			}
			else
			{
				CL_Rect cl_rect(0, 0, get_width(), get_height());
				sig_paint(cl_rect);
			}
		}
		return true;

	case WM_DESTROY:
		window_map.erase(window_map.find(hwnd));
		return true;

	case WM_SYSCOMMAND:
		// Do not allow screensaver to start.
		if (wParam == SC_SCREENSAVE) return true;
		break;
	}

	return false;
}

void CL_DisplayWindow_Win32::set_videomode(int width, int height, int bpp, int refresh_rate)
{
	DEVMODE devmode;
	memset(&devmode, 0, sizeof(DEVMODE));

	// Default to 32 bit BPP:
	if (bpp == 0) bpp = 32;
	
	int index = 0;

	// User wants a specific refresh-rate:
	if (refresh_rate != 0)
	{
		while (true)
		{
			DEVMODE cur_devmode;
			memset(&cur_devmode, 0, sizeof(DEVMODE));
			cur_devmode.dmSize = sizeof(DEVMODE);
			if (EnumDisplaySettings(0, index++, &cur_devmode) == 0) break;

			if (
				cur_devmode.dmPelsWidth == width &&
				cur_devmode.dmPelsHeight == height &&
				cur_devmode.dmBitsPerPel == bpp &&
				cur_devmode.dmDisplayFrequency == refresh_rate)
			{
				devmode = cur_devmode;
			}
		}
	}
	
	// If specific mode not found, try to find the highest possible refresh-rate mode:
	if (devmode.dmFields == 0)
	{
		while (true)
		{
			DEVMODE cur_devmode;
			memset(&cur_devmode, 0, sizeof(DEVMODE));
			cur_devmode.dmSize = sizeof(DEVMODE);
			if (EnumDisplaySettings(0, index++, &cur_devmode) == 0) break;
			
			if (
				cur_devmode.dmPelsWidth == width &&
				cur_devmode.dmPelsHeight == height &&
				cur_devmode.dmBitsPerPel == bpp &&
				cur_devmode.dmDisplayFrequency > devmode.dmDisplayFrequency)
			{
				devmode = cur_devmode;
			}
		}
	}

	// If the mode wasnt found, try anyway but without refresh mode info:
	if (devmode.dmFields == 0)
	{
		devmode.dmSize = sizeof(DEVMODE);
		devmode.dmPelsWidth = width;
		devmode.dmPelsHeight = height;
		devmode.dmBitsPerPel = bpp;
		devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	}

	saved_window_positions.clear();
	//EnumWindows(&CL_DisplayWindow_Win32::enum_windows_callback_save, (LPARAM) this);

	LONG err = ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);

	if (err != DISP_CHANGE_SUCCESSFUL)
	{
		switch (err)
		{
		case DISP_CHANGE_RESTART:
			throw CL_Error("This OS sucks! It wants you to restart your computer to change to fullscreen!!!");

		case DISP_CHANGE_FAILED:
			throw CL_Error("The display driver failed the specified graphics mode.");

		case DISP_CHANGE_BADMODE:
			throw CL_Error("The graphics mode is not supported.");

		default:
			throw CL_Error(CL_String::format("Could not change videomode, error %1", (int)err));
		}
	}
}

void CL_DisplayWindow_Win32::restore_videomode()
{
	ChangeDisplaySettings(NULL, 0);
	//EnumWindows(&CL_DisplayWindow_Win32::enum_windows_callback_restore, (LPARAM) this);
	saved_window_positions.clear();
}

BOOL CL_DisplayWindow_Win32::enum_windows_callback_save(HWND hwnd, LPARAM lParam)
{
	if (IsWindowVisible(hwnd) == FALSE) return TRUE;

	CL_DisplayWindow_Win32 *self = (CL_DisplayWindow_Win32*) lParam;
	WINDOWPLACEMENT placement;
	memset(&placement, 0, sizeof(WINDOWPLACEMENT));
	placement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd, &placement);
	LONG wndprc = GetWindowLong(hwnd, GWL_WNDPROC);
	self->saved_window_positions[hwnd] = std::pair<LONG, WINDOWPLACEMENT>(wndprc, placement);
	return TRUE;
}

BOOL CL_DisplayWindow_Win32::enum_windows_callback_restore(HWND hwnd, LPARAM lParam)
{
	if (IsWindowVisible(hwnd) == FALSE) return TRUE;

	CL_DisplayWindow_Win32 *self = (CL_DisplayWindow_Win32*) lParam;
	if (self->saved_window_positions.find(hwnd) == self->saved_window_positions.end()) return TRUE;

	LONG wndprc = GetWindowLong(hwnd, GWL_WNDPROC);
	if (wndprc == self->saved_window_positions[hwnd].first)
	{
		SetWindowPlacement(hwnd, &self->saved_window_positions[hwnd].second);
	}
	return TRUE;
}

void CL_DisplayWindow_Win32::received_keyboard_input(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Keep get_keycode() up-to-date.
	GetKeyboardState(keys_down);

	// Is message a down or up event?
	bool keydown;
	switch (uMsg)
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

//	int scancode = (lParam & 0xff0000) >> 16;
//	bool extended_key = (lParam & 0x1000000) != 0;

	// Update the ctrl/alt/shift hints:
	int key_id = wParam;

	// Ah fuckit. This platform appearently cant properly tell left from right unless you're
	// polling. -- mbn 6. august 2002.
	switch (key_id)
	{
	case VK_CONTROL: left_ctrl_down = keydown; right_ctrl_down = keydown; break;
	case VK_MENU: left_alt_down = keydown; right_alt_down = keydown; break;
	case VK_SHIFT: left_shift_down = keydown; right_shift_down = keydown; break;
	}

	// Add to repeat count
	if(keydown)
	{
		if(key_id == previous_key_down)
			repeat_count++;
		previous_key_down = key_id;
	}
	else
	{
		repeat_count = 0;
		previous_key_down = 0;
	}

	// Prepare event to be emitted:
	CL_InputEvent key;
	if (keydown)
		key.type = CL_InputEvent::pressed;
	else
		key.type = CL_InputEvent::released;
	key.mouse_pos = mouse_pos;
	key.id = key_id;
	key.device = keyboard;
	key.repeat_count = repeat_count;

	// Figure out what character sequence this maps to:
	char buf[4];
	int result = ToAscii(
		(UINT) key_id,
		MapVirtualKey((UINT) key_id, 0),
		keys_down,
		(LPWORD) buf,
		0);
	if (result == 1 || result == 2) key.str = std::string(buf, result);

	// Emit message:
	CL_InputDevice keyboard = get_ic()->get_keyboard();
	if (keydown)
		keyboard.sig_key_down()(key);
	else
		keyboard.sig_key_up()(key);
}

void CL_DisplayWindow_Win32::received_mouse_input(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Map windows events to something more interesting:
	int id;
	bool up = false;
	bool down = false;
	bool dbl = false;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN: id = 0; down = true; break;
	case WM_LBUTTONUP: id = 0; up = true; break;
	case WM_LBUTTONDBLCLK: id = 0; down = true; dbl = true; break;
	case WM_RBUTTONDOWN: id = 1; down = true; break;
	case WM_RBUTTONUP: id = 1; up = true; break;
	case WM_RBUTTONDBLCLK: id = 1; down = true; break;
	case WM_MBUTTONDOWN: id = 2; down = true; break;
	case WM_MBUTTONUP: id = 2; up = true; break;
	case WM_MBUTTONDBLCLK: id = 2; down = true; dbl = true; break;
	case WM_MOUSEWHEEL: id = ((short)HIWORD(wParam) > 0) ? 3 : 4; up = true; down = true; break;
	case WM_XBUTTONDOWN: id = 4 + HIWORD(wParam); down = true; break;
	case WM_XBUTTONUP: id = 4 + HIWORD(wParam); up = true; break;
	case WM_XBUTTONDBLCLK: id = 4 + HIWORD(wParam); down = true; dbl = true; break;
	default:
		return;
	}

	// Prepare event to be emitted:
	CL_InputEvent key;
	key.mouse_pos = mouse_pos;
	key.id = id;
	key.device = mouse;

	CL_InputDevice mouse = get_ic()->get_mouse();

	// Emit message:
	if (down)
	{
		key.type = CL_InputEvent::pressed;
		if (dbl)
		{
			key.repeat_count = 2;
			mouse.sig_key_dblclk()(key);
		}
		else
		{
			mouse.sig_key_down()(key);
		}
	}

	if (up)
	{
		key.type = CL_InputEvent::released;
		mouse.sig_key_up()(key);
	}
}

void CL_DisplayWindow_Win32::received_mouse_move(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Fetch coordinates
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	if(mouse_pos.x != x || mouse_pos.y != y)
	{
		mouse_pos.x = x;
		mouse_pos.y = y;

		// Prepare event to be emitted:
		CL_InputEvent key;
		key.type = CL_InputEvent::moved;
		key.device = mouse;
		key.mouse_pos = mouse_pos;

		// Fire off signal
		CL_InputDevice mouse = get_ic()->get_mouse();
		mouse.sig_move()(key);
	}
}

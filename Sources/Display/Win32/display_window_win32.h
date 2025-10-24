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

#ifndef header_display_window_win32
#define header_display_window_win32

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "Display/display_window_generic.h"
#include "API/Core/Math/rect.h"
#include "API/Display/input_context.h"
#include "API/Display/input_device.h"
#include <map>

#include <dinput.h>

#if DIRECTINPUT_HEADER_VERSION < 0x0800
#error Found DirectX headers older than 8.0. Please download a newer directx, and make sure its FIRST in the include path and library path (Tools->Options->Directories in MSVC).
#endif

class CL_API_DISPLAY CL_DisplayWindow_Win32 : public CL_DisplayWindow_Generic
{
//! Construction:
public:
	CL_DisplayWindow_Win32();

	virtual ~CL_DisplayWindow_Win32();

//! Attributes:
public:
	virtual int get_width() const;

	virtual int get_height() const;

	virtual bool is_fullscreen() const;

	virtual bool has_focus() const;

	virtual int get_buffer_count() const;

	virtual bool get_keycode(int keycode) const;

	HWND get_hwnd() { return hwnd; }

	virtual CL_InputContext *get_ic() { return &input_context; }

	const CL_Rect &get_layer_position() const { return layered_position; }

	LPDIRECTINPUT8 get_directinput() const { return directinput; }

	CL_Point mouse_pos;

	bool left_ctrl_down, left_alt_down, left_shift_down;

	bool right_ctrl_down, right_alt_down, right_shift_down;

//! Operations:
public:
	virtual void set_fullscreen(int width, int height, int bpp, int refresh_rate);

	virtual void set_windowed();

	virtual void set_title(const std::string &title);

	virtual void set_position(const CL_Rect &pos);

	virtual void set_position(int x, int y);

	virtual void set_size(int width, int height);

	virtual void set_buffer_count(int flipping_buffers);

	virtual void update(const CL_Rect &rect);

	virtual void flip(int interval);

	virtual void create_window(const CL_DisplayWindowDescription &desc);

	virtual void destroy_window();

	virtual void show_system_cursor();

	virtual void hide_system_cursor();

	void set_videomode(int width, int height, int bpp, int refresh_rate);

	void restore_videomode();

	virtual bool received_event(UINT uMsg, WPARAM wParam, LPARAM lParam);

//! Implementation:
private:
	//: Creates the initial window based on the window description.
	void create_new_window(const CL_DisplayWindowDescription &desc);

	//: Updates the already created window to new window description.
	void modify_window(const CL_DisplayWindowDescription &desc);

	static BOOL CALLBACK enum_windows_callback_save(HWND hwnd, LPARAM lParam);

	static BOOL CALLBACK enum_windows_callback_restore(HWND hwnd, LPARAM lParam);

	static BOOL CALLBACK enum_devices_callback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	void received_keyboard_input(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void received_mouse_input(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void received_mouse_move(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static std::map<HWND, CL_DisplayWindow_Win32 *> window_map;

	static LONG WINAPI message_handler(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);

	CL_InputContext input_context;

	CL_InputDevice keyboard;

	CL_InputDevice mouse;

	HWND hwnd;

	std::string title;

	bool fullscreen;

	bool allow_resize;

	bool layered;

	CL_Rect layered_position;

	int bpp;

	unsigned char keys_down[256];

	int previous_key_down;
	int repeat_count;

	CL_Rect saved_position;

	LPDIRECTINPUT8 directinput;

	std::map< HWND, std::pair<LONG, WINDOWPLACEMENT> > saved_window_positions;
};

#endif

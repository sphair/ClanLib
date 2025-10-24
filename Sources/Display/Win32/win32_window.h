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

#pragma once

#include <list>
#include <map>
#include <dinput.h>
#include "API/Display/api_display.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "API/Core/System/sharedptr.h"
#include "API/Core/System/event.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "API/Core/System/cl_platform.h"
#include "API/Core/Signals/callback_v0.h"
#include "API/Core/System/thread.h"
#include "API/Display/Image/pixel_buffer.h"

#if DIRECTINPUT_HEADER_VERSION < 0x0800
#error Found DirectX headers older than 8.0. Please download a newer directx, and make sure its FIRST in the include path and library path (Tools->Options->Directories in MSVC).
#endif

class CL_InputDeviceProvider_Win32Keyboard;
class CL_InputDeviceProvider_Win32Mouse;
class CL_InputDeviceProvider_Win32Tablet;
class CL_DisplayMessageQueue_Win32;
class CL_DisplayWindowSite;
class CL_DisplayWindowDescription;
class CL_PixelBuffer;
class CL_DataBuffer;
class CL_CursorProvider_Win32;
class CL_InputEvent;
class CL_Colorf;

class CL_API_DISPLAY CL_Win32Window
{
public:
	CL_Win32Window();
	~CL_Win32Window();

public:
	HWND get_hwnd() const { return hwnd; }
	LPDIRECTINPUT8 get_directinput() const { return directinput; }
	CL_Rect get_geometry() const;
	CL_Rect get_viewport() const;
	bool has_focus() const;
	bool is_minimized() const;
	bool is_maximized() const;
	bool is_visible() const;
	CL_Size get_minimum_size(bool client_area) const;
	CL_Size get_maximum_size(bool client_area) const;
	CL_String get_title() const;
	CL_Callback_v0 &func_on_resized() { return callback_on_resized; }
	CL_InputContext &get_ic() { return ic; }
	const CL_InputContext &get_ic() const { return ic; }
	bool is_clipboard_text_available() const;
	bool is_clipboard_image_available() const;
	bool is_painting() const { return paintstruct.hdc != 0; }
	const PAINTSTRUCT &get_paint_data() const { return paintstruct; }
	bool is_layered() const { return layered; }

public:
	void create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);

	CL_Point client_to_screen(const CL_Point &client);
	CL_Point screen_to_client(const CL_Point &screen);

	void show_system_cursor();
	void set_cursor(CL_CursorProvider_Win32 *cursor);
	void set_cursor(enum CL_StandardCursor type);
	void hide_system_cursor();

	void set_large_icon(const CL_PixelBuffer &image);
	void set_small_icon(const CL_PixelBuffer &image);

	void set_title(const CL_StringRef &new_title);
	void set_enabled(bool enable);

	void set_position(const CL_Rect &pos, bool client_area);
	void set_size(int width, int height, bool client_area);
	void set_minimum_size(int width, int height, bool client_area);
	void set_maximum_size(int width, int height, bool client_area);
	void minimize();
	void restore();
	void maximize();

	void show(bool activate);
	void hide();

	void bring_to_front();

	void capture_mouse(bool capture);

	void set_clipboard_text(const CL_StringRef &text);
	CL_String get_clipboard_text() const;
	CL_PixelBuffer get_clipboard_image() const;
	void set_clipboard_image(const CL_PixelBuffer &image);
	static CL_PixelBuffer create_bitmap_data(const CL_PixelBuffer &image, const CL_Rect &rect);

	static HBITMAP create_bitmap(HDC hdc, const CL_PixelBuffer &image);
	HICON create_icon(const CL_PixelBuffer &image) const;

	void request_repaint(const CL_Rect &rect);

	void set_modifier_keys(CL_InputEvent &key);

	void update_layered(CL_PixelBuffer &image);

	void set_allow_drop_shadow(bool value) { allow_dropshadow = value; }

private:
	void update_layered_worker_thread();
	void update_layered_worker_thread_process();
	void update_layered_process_alpha(int y_start, int y_stop);

	void create_direct_input();

	static LRESULT WINAPI static_window_try_proc(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);
	static LRESULT WINAPI static_window_proc(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);
	LRESULT window_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// \brief Creates the initial window based on the window description.
	void create_new_window(const CL_DisplayWindowDescription &desc);

	/// \brief Updates the already created window to new window description.
	void modify_window(const CL_DisplayWindowDescription &desc);

	void get_styles_from_description( const CL_DisplayWindowDescription &desc, DWORD &style, DWORD &ex_style  );
	RECT get_window_geometry_from_description( const CL_DisplayWindowDescription &desc, DWORD style, DWORD ex_style );
	void connect_window_input( const CL_DisplayWindowDescription &desc );
	void register_window_class();

	void received_keyboard_input(UINT msg, WPARAM wparam, LPARAM lparam);
	void received_mouse_input(UINT msg, WPARAM wparam, LPARAM lparam);
	void received_mouse_move(UINT msg, WPARAM wparam, LPARAM lparam);
	void received_joystick_input(UINT msg, WPARAM wparam, LPARAM lparam);

	void setup_tablet();

	static BOOL CALLBACK enum_devices_callback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	CL_PixelBuffer get_argb8888_from_png(cl_ubyte8 *data, size_t size) const;
	CL_PixelBuffer get_argb8888_from_rgb_dib(BITMAPV5HEADER *bitmapInfo, size_t size) const;
	CL_PixelBuffer get_argb8888_from_bitfields_dib(BITMAPV5HEADER *bitmapInfo, size_t size) const;

	void flip_pixelbuffer_vertical(CL_PixelBuffer &pbuf) const;
	void add_png_to_clipboard(const CL_PixelBuffer &image);
	void add_dib_to_clipboard(const CL_PixelBuffer &image);
	void register_clipboard_formats();

	CL_InputDeviceProvider_Win32Keyboard *get_keyboard();
	CL_InputDeviceProvider_Win32Mouse *get_mouse();
	CL_InputDeviceProvider_Win32Tablet *get_tablet();

	HWND hwnd;
	bool destroy_hwnd;
	HCURSOR current_cursor;
	HICON large_icon;
	HICON small_icon;
	bool cursor_set, cursor_hidden;
	CL_DisplayWindowSite *site;
	HMODULE direct8_module;
	typedef HRESULT (WINAPI FuncDirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);
	FuncDirectInput8Create *ptr_DirectInput8Create;
	LPDIRECTINPUT8 directinput;
	CL_InputDevice keyboard, mouse, tablet;
	std::vector<CL_InputDevice> joysticks;
	CL_Point mouse_pos;
	std::map<int,int> repeat_count;
	CL_Callback_v0 callback_on_resized;
	CL_InputContext ic;
	PAINTSTRUCT paintstruct;
	CL_Size minimum_size;
	CL_Size maximum_size;
	bool layered;
	UINT png_clipboard_format;
	CL_String class_name;
	bool allow_dropshadow;
	bool minimized;
	bool maximized;

	CL_Thread update_window_worker_thread;
	bool update_window_worker_thread_started;
	CL_PixelBuffer update_window_image;
	CL_Event update_window_event_stop;
	CL_Event update_window_event_start;
	CL_Event update_window_event_completed;

	friend class CL_InputDeviceProvider_DirectInput;
};

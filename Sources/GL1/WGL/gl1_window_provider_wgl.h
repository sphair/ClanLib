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
**    Mark Page
*/

#pragma once


#include "../gl1_window_provider.h"
#include "Display/Win32/win32_window.h"
#include "API/Core/System/weakptr.h"
#include "API/Core/System/sharedptr.h"
#include "API/Display/TargetProviders/render_window_provider.h"

typedef BOOL (APIENTRY *ptr_wglSwapIntervalEXT)(int interval);

class CL_GL1WindowDescription;
class CL_PBuffer_GL1;
class CL_GL1WindowProvider_WGL;
class CL_GL1GraphicContextProvider;

class CL_RenderWindowProvider_WGL : public CL_RenderWindowProvider
{
public:
	CL_RenderWindowProvider_WGL(CL_GL1WindowProvider_WGL & window, HGLRC wgl_context, bool own_context);
	virtual ~CL_RenderWindowProvider_WGL();
	virtual int get_viewport_width() const;
	virtual int get_viewport_height() const;
	virtual void flip_buffers(int interval) const;
	virtual void make_current() const;

	CL_GL1WindowProvider_WGL &get_window();

private:
	CL_GL1WindowProvider_WGL & window;
	HGLRC wgl_context;
	bool own_context;

	friend class CL_GL1WindowProvider_WGL;
};


class CL_GL1WindowProvider_WGL : public CL_GL1WindowProvider
{
/// \name Construction
/// \{

public:
	CL_GL1WindowProvider_WGL();
	~CL_GL1WindowProvider_WGL();


/// \}
/// \name Attributes
/// \{

public:
	CL_Rect get_geometry() const;
	CL_Rect get_viewport() const;
	bool is_fullscreen() const;
	bool has_focus() const;
	bool is_minimized() const;
	bool is_maximized() const;
	bool is_visible() const;
	CL_String get_title() const;
	CL_Size get_minimum_size(bool client_area) const;
	CL_Size get_maximum_size(bool client_area) const;
	HWND get_hwnd() const { return hwnd; }
	HDC get_device_context() const { return device_context; }
	HGLRC get_opengl_context() const { return opengl_context; }
	CL_GraphicContext& get_gc() { return gc; }
	CL_InputContext& get_ic() { return win32_window.get_ic(); }
	bool is_clipboard_text_available() const;
	bool is_clipboard_image_available() const;
	CL_String get_clipboard_text() const;
	CL_PixelBuffer get_clipboard_image() const;

/// \}
/// \name Operations
/// \{

public:
	CL_Point client_to_screen(const CL_Point &client);
	CL_Point screen_to_client(const CL_Point &screen);
	void create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);

	HGLRC get_share_context();
	void show_system_cursor();
	CL_CursorProvider *create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot);
	void set_cursor(CL_CursorProvider *cursor);
	void set_cursor(CL_StandardCursor type);
	void hide_system_cursor();
	void set_title(const CL_StringRef &new_title);
	void set_position(const CL_Rect &pos, bool client_area);
	void set_size(int width, int height, bool client_area);
	void set_minimum_size(int width, int height, bool client_area);
	void set_maximum_size( int width, int height, bool client_area);
	void set_enabled(bool enable);
	void minimize();
	void restore();
	void maximize();
	void show(bool activate);
	void hide();
	void bring_to_front();

	/// \brief Flip OpenGL buffers.
	void flip(int interval);

	/// \brief Copy a region of the back buffer to the front buffer.
	void update(const CL_Rect &rect);

	/// \brief Capture/Release the mouse.
	void capture_mouse(bool capture);

	/// \brief Stores text in the clipboard.
	void set_clipboard_text(const CL_StringRef &text);

	void set_clipboard_image(const CL_PixelBuffer &buf);

	/// \brief Invalidates a region of a screen, causing a repaint.
	void request_repaint(const CL_Rect &rect);

	void set_large_icon(const CL_PixelBuffer &image);
	void set_small_icon(const CL_PixelBuffer &image);

	CL_PBuffer_GL1 create_pbuffer(CL_GL1GraphicContextProvider *gc_provider, CL_Size size);

/// \}
/// \name Implementation
/// \{

private:
	static BOOL CALLBACK enum_windows_callback_save(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK enum_windows_callback_restore(HWND hwnd, LPARAM lParam);
	void create_shadow_window(HWND wnd);
	void on_window_resized();

	CL_GraphicContext gc;
	CL_Win32Window win32_window;

	/// \brief OpenGL rendering context for this window.
	HGLRC opengl_context;

	/// \brief Device context for this window.
	HDC device_context;
	HWND hwnd;
	bool shadow_window;
	CL_DisplayWindowSite *site;
	bool fullscreen;

	ptr_wglSwapIntervalEXT wglSwapIntervalEXT;
	int swap_interval;
/// \}
};

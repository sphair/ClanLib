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
*/

#pragma once

#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"

#ifdef WIN32
#include "Display/Win32/win32_window.h"
#else
#include "Display/X11/x11_window.h"
#endif

namespace clan
{

class SWRenderDisplayWindowProvider : public DisplayWindowProvider
{
/// \name Construction
/// \{
public:
	SWRenderDisplayWindowProvider();
	~SWRenderDisplayWindowProvider();

/// \}
/// \name Attributes
/// \{
public:
	Rect get_geometry() const;
	Rect get_viewport() const;

	bool has_focus() const;
	bool is_minimized() const;
	bool is_maximized() const;
	bool is_visible() const;
	bool is_fullscreen() const { return false; } // FIXME: real implementation
	std::string get_title() const;
	Size get_minimum_size(bool client_area=false) const;
	Size get_maximum_size(bool client_area=false) const;

	GraphicContext& get_gc();
	InputContext get_ic();

#ifdef WIN32
	HWND get_hwnd() const;
#elif defined(__APPLE__)
	// nothing
#else
	/// \brief Returns the X11 display handle.
	::Display *get_display() const { return window.get_display(); }

	/// \brief Handle to X11 window handle.
	::Window get_window() const { return window.get_window(); }
#endif

	bool is_clipboard_text_available() const;
	bool is_clipboard_image_available() const;
	std::string get_clipboard_text() const;
	PixelBuffer get_clipboard_image() const;

/// \}
/// \name Operations
/// \{
public:
	Point client_to_screen(const Point &client);
	Point screen_to_client(const Point &screen);

	void capture_mouse(bool capture);

	void create(DisplayWindowSite *site, const DisplayWindowDescription &description);

	void show_system_cursor();
	CursorProvider *create_cursor(const CursorDescription &cursor_description, const Point &hotspot);
	void set_cursor(CursorProvider *cursor);
	void set_cursor(StandardCursor type);
#ifdef WIN32
	void set_cursor_handle(HCURSOR cursor);
#endif
	void hide_system_cursor();

	void set_title(const std::string &new_title);
	void set_position(const Rect &pos, bool client_area);
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

	void flip(int interval);

	void update(const Rect &rect);

	void set_clipboard_text(const std::string &text);
	void set_clipboard_image(const PixelBuffer &buf);

	void request_repaint(const Rect &rect);

	void set_large_icon(const PixelBuffer &image);
	void set_small_icon(const PixelBuffer &image);
	void enable_alpha_channel(const Rect &blur_rect);
	void extend_frame_into_client_area(int height);

/// \}
/// \name Implementation
/// \{
private:
	void on_window_resized();

#ifdef WIN32
	void draw_image(HDC hdc, const Rect &dest, const PixelBuffer &image);
	void draw_image(HDC hdc, const Rect &dest, const PixelBuffer &image, const Rect &src);
	Win32Window window;
#else
#if defined(__APPLE__)
	MacWindow window;
#else
	void draw_image(const Rect &dest, const PixelBuffer &image, const Rect &src);
	X11Window window;
#endif
#endif

	DisplayWindowSite *site;
	GraphicContext gc;

	bool flip_timer_set;
	unsigned int flip_last_time;
	int refresh_rate;
	int swap_interval;
/// \}
};

}

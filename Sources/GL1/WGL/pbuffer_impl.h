/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include "../pbuffer.h"
#include "API/Core/System/disposable_object.h"
#include "../opengl1_wrap.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "Display/Win32/win32_window.h"
#include <memory>

namespace clan
{

class GL1GraphicContextProvider;

class PBuffer_GL1_Impl : public DisposableObject, public DisplayWindowProvider
{
/// \name Construction
/// \{

public:
	PBuffer_GL1_Impl(GL1GraphicContextProvider *gc_provider);

	~PBuffer_GL1_Impl();


/// \}
/// \name Attributes
/// \{

public:

	// Stubs
	Rect get_geometry() const {return Rect();}
	Rect get_viewport() const {return Rect();}
	bool is_fullscreen() const {return false;}
	bool has_focus() const {return false;}
	bool is_minimized() const {return false;}
	bool is_maximized() const {return false;}
	bool is_visible() const {return false;}
	std::string get_title() const {return std::string();}
	Size get_minimum_size(bool client_area) const  {return Size();}
	Size get_maximum_size(bool client_area) const {return Size();}
	HWND get_hwnd() const { return 0; }
	HDC get_device_context() const { return 0; }
	HGLRC get_opengl_context() const { return 0; }
	GraphicContext& get_gc() { throw Exception("Not implemented"); }
	InputContext get_ic() { throw Exception("Not implemented"); }
	bool is_clipboard_text_available() const {return false;}
	bool is_clipboard_image_available() const {return false;}
	std::string get_clipboard_text() const {return std::string();}
	PixelBuffer get_clipboard_image() const {return PixelBuffer();}


/// \}
/// \name Operations
/// \{

public:
	void make_current() const;
	void create(GL1WindowProvider_WGL &window_provider, Size &size);
	void set_active() const;

	// Stubs
	Point client_to_screen(const Point &client) {return Point();}
	Point screen_to_client(const Point &screen) {return Point();}
	void create(DisplayWindowSite *site, const DisplayWindowDescription &description) {}
	HGLRC get_share_context() {return 0;}
	void show_system_cursor() {};
	CursorProvider *create_cursor(const SpriteDescription &sprite_description, const Point &hotspot) {return NULL;}
	void set_cursor(CursorProvider *cursor) {}
	void set_cursor(StandardCursor type) {}
#ifdef WIN32
	void set_cursor_handle(HCURSOR cursor) { }
#endif
	void hide_system_cursor() {}
	void set_title(const std::string &new_title) {}
	void set_position(const Rect &pos, bool client_area) {}
	void set_size(int width, int height, bool client_area) {}
	void set_minimum_size(int width, int height, bool client_area) {}
	void set_maximum_size( int width, int height, bool client_area) {}
	void set_enabled(bool enable) {}
	void minimize() {}
	void restore() {}
	void maximize() {}
	void show(bool activate) {}
	void hide() {}
	void bring_to_front() {}
	void flip(int interval) {}
	void update(const Rect &rect) {}
	void capture_mouse(bool capture) {}
	void set_clipboard_text(const std::string &text) {}
	void set_clipboard_image(const PixelBuffer &buf) {}
	void request_repaint(const Rect &rect) {}
	void set_large_icon(const PixelBuffer &image) {}
	void set_small_icon(const PixelBuffer &image) {}


/// \}
/// \name Implementation
/// \{

private:
	void on_dispose();
	void reset();

	GL1GraphicContextProvider *gc_provider;
	GL1Functions::HPBUFFERARB pbuffer;
	HGLRC pbuffer_context;
	HDC pbuffer_dc;

	GL1GraphicContextProvider *pbuffer_gc_provider;
	GraphicContext pbuffer_gc;

/// \}
};

}

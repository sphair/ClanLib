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
**    Mark Page
*/

#pragma once

#include "../opengl1_wrap.h"
#include "API/Display/TargetProviders/display_window_provider.h"

namespace clan
{

class GL1GraphicContextProvider;
class GL1WindowProvider_WGL;

class PBuffer_GL1_Impl : public DisplayWindowProvider
{

public:
	PBuffer_GL1_Impl(GL1GraphicContextProvider *gc_provider);

	~PBuffer_GL1_Impl();

public:
	//gc_provider->get_render_window().has_focus();
	Rect get_geometry() const {return pbuffer_size;}
	Rect get_viewport() const {return pbuffer_size;}
	bool has_focus() const {return false;}
	bool is_minimized() const {return false;}
	bool is_maximized() const {return false;}
	bool is_visible() const {return false;}
	bool is_fullscreen() const {return false;}
	Size get_minimum_size(bool client_area) const {return pbuffer_size;}
	Size get_maximum_size(bool client_area) const {return pbuffer_size;}
	std::string get_title() const {return "PBuffer";}
	GraphicContext& get_gc() {return pbuffer_gc;}
	InputContext get_ic();
	HWND get_hwnd() const;
	bool is_clipboard_text_available() const {return false;}
	bool is_clipboard_image_available() const {return false;}
	std::string get_clipboard_text() const {return "";}
	PixelBuffer get_clipboard_image() const;
	void make_current() const;
	Point client_to_screen(const Point &client) {return client;}
	Point screen_to_client(const Point &screen) {return screen;}
	void capture_mouse(bool capture) {}
	void request_repaint(const Rect &rect) {}
	void create(DisplayWindowSite *site, const DisplayWindowDescription &description) {}
	void show_system_cursor() {}
	CursorProvider *create_cursor(const CursorDescription &cursor_description, const Point &hotspot) {return 0;}
	void set_cursor(CursorProvider *cursor) {}
	void set_cursor(enum StandardCursor type) {}
	void set_cursor_handle(HCURSOR cursor) {}
	void hide_system_cursor() {}
	void set_title(const std::string &new_title) {}
	void set_position(const Rect &pos, bool client_area) {}
	void set_size(int width, int height, bool client_area) {}
	void set_minimum_size(int width, int height, bool client_area) {}
	void set_maximum_size(int width, int height, bool client_area) {}
	void set_enabled(bool enable) {}
	void minimize() {}
	void restore() {}
	void maximize() {}
	void show(bool activate) {}
	void hide() {}
	void bring_to_front() {}
	void flip(int interval) {}
	void update(const Rect &rect) {}
	void set_clipboard_text(const std::string &text) {}
	void set_clipboard_image(const PixelBuffer &buf) {}
	void set_large_icon(const PixelBuffer &image) {}
	void set_small_icon(const PixelBuffer &image) {}
	void enable_alpha_channel(const Rect &blur_rect) {}
	void extend_frame_into_client_area(int height) {}



public:
	void create(GL1WindowProvider_WGL &window_provider, const Size &size);
	void set_active() const;


private:
	void reset();

	GL1GraphicContextProvider *gc_provider;
	GL1Functions::HPBUFFERARB pbuffer;
	HGLRC pbuffer_context;
	HDC pbuffer_dc;

	GL1GraphicContextProvider *pbuffer_gc_provider;
	GraphicContext pbuffer_gc;
	Size pbuffer_size;


};

}

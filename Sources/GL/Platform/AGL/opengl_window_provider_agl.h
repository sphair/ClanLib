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

#pragma once

#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/GL/opengl_wrap.h"

namespace clan
{
class OpenGLWindowProvider;

void cl_agl_make_none_current();
DisplayWindowProvider *cl_alloc_display_window_agl();
void cl_set_default_frame_buffer(DisplayWindowProvider *provider);
}

#ifdef __OBJC__

#include "cocoa_window.h"

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

namespace clan
{

class OpenGLWindowProvider;

class OpenGLWindowProvider : public DisplayWindowProvider
{
/// \name Construction
/// \{
public:
	OpenGLWindowProvider();
	~OpenGLWindowProvider();
/// \}

/// \name Attributes
/// \{
public:
	bool is_double_buffered() const { return true; }
	Rect get_geometry() const {return cocoa_window.get_geometry();}
	Rect get_viewport() const {return cocoa_window.get_viewport();}
	bool is_fullscreen() const {return cocoa_window.is_fullscreen();}
	bool has_focus() const {return cocoa_window.has_focus();}
	bool is_minimized() const {return cocoa_window.is_minimized();}
	bool is_maximized() const {return cocoa_window.is_maximized();}
	bool is_visible() const {return cocoa_window.is_visible();}
	bool is_clipboard_text_available() const { return cocoa_window.is_clipboard_text_available(); }
	bool is_clipboard_image_available() const { return cocoa_window.is_clipboard_image_available(); }
	std::string get_title() const { return cocoa_window.get_title(); }
	Size get_minimum_size(bool client_area) const { return cocoa_window.get_minimum_size(client_area); }
	Size get_maximum_size(bool client_area) const { return cocoa_window.get_maximum_size(client_area); }
	std::string get_clipboard_text() const { return cocoa_window.get_clipboard_text(); }
	PixelBuffer get_clipboard_image() const { return cocoa_window.get_clipboard_image(); }
	EAGLContext *get_opengl_context() { return opengl_context; }
	GraphicContext& get_gc() { return gc; }
	InputContext get_ic() { return cocoa_window.get_ic(); }
	GraphicContext gc;

	ProcAddress *get_proc_address(const std::string& function_name) const;

	GraphicContext& get_gc() override;
	InputDevice &get_keyboard() override;
	InputDevice &get_mouse() override;

/// \}

/// \name Operations
/// \{
public:
	void make_current() const;
	void destroy() { delete this; }
	Point client_to_screen(const Point &client) { return cocoa_window.client_to_screen(client); }
	Point screen_to_client(const Point &screen) { return cocoa_window.screen_to_client(screen); }
	void create(DisplayWindowSite *site, const DisplayWindowDescription &description);
	void show_system_cursor() { cocoa_window.show_system_cursor(); }
	CursorProvider *create_cursor(const SpriteDescription &sprite_description, const Point &hotspot);
	void set_cursor(CursorProvider *cursor);
	void set_cursor(StandardCursor type) { cocoa_window.set_cursor(type); }
	void hide_system_cursor()  { cocoa_window.hide_system_cursor(); }
	void set_title(const std::string &new_title) { cocoa_window.set_title(new_title); }
	void set_position(const Rect &pos, bool client_area) { return cocoa_window.set_position(pos, client_area); };
	void set_size(int width, int height, bool client_area)  { return cocoa_window.set_size(width, height, client_area); }
	void set_minimum_size(int width, int height, bool client_area) { return cocoa_window.set_minimum_size(width, height, client_area); }
	void set_maximum_size( int width, int height, bool client_area) { return cocoa_window.set_maximum_size(width, height, client_area); }
	void set_enabled(bool enable) { return cocoa_window.set_enabled(enable); }
	void minimize() { cocoa_window.minimize(); }
	void restore() { cocoa_window.restore(); }
	void maximize() { cocoa_window.maximize(); }
	void toggle_fullscreen() { } // FIXME: real implementation
	void show(bool activate)  { cocoa_window.show(activate); }
	void hide() { cocoa_window.hide(); }
	void bring_to_front() { cocoa_window.bring_to_front(); }
	void flip(int interval);
	void capture_mouse(bool capture) { cocoa_window.capture_mouse(capture); }
	void process_messages();

	EAGLContext *create_context();
    void setup_default_framebuffer();
    void set_default_frame_buffer();

	void set_clipboard_text(const std::string &text) { cocoa_window.set_clipboard_text(text); }
	void set_clipboard_image(const PixelBuffer &buf) { cocoa_window.set_clipboard_image(buf); }
	void request_repaint() { cocoa_window.request_repaint(); }

	void set_large_icon(const PixelBuffer &image);
	void set_small_icon(const PixelBuffer &image);

	void enable_alpha_channel(const Rect &blur_rect);
	void extend_frame_into_client_area(int left, int top, int right, int bottom);

/// \}

/// \name Implementation
/// \{
private:
	void on_window_resized();

	CocoaWindow cocoa_window;
	EAGLContext *opengl_context;
    GLuint default_framebuffer_handle;
    GLuint default_colorbuffer_handle;
    GLuint default_depthbuffer_handle;
	DisplayWindowSite *site;
	int swap_interval;
/// \}
};

}

#endif

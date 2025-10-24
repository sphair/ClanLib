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

#include "../opengl_window_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/TargetProviders/render_window_provider.h"
#include "API/GL/opengl_wrap.h"

void cl_agl_make_none_current();
CL_OpenGLWindowProvider *cl_alloc_display_window_agl();
void cl_set_default_frame_buffer(CL_RenderWindowProvider *provider);

#ifdef __OBJC__

#include "cocoa_window.h"

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

class CL_OpenGLWindowProvider_AGL;

class CL_GL_RenderWindowProvider_AGL : public CL_RenderWindowProvider
{
public:
	CL_GL_RenderWindowProvider_AGL(CL_OpenGLWindowProvider_AGL & window, EAGLContext *agl_context, bool own_context);
	virtual ~CL_GL_RenderWindowProvider_AGL();
	virtual int get_viewport_width() const;
	virtual int get_viewport_height() const;
	virtual void flip_buffers(int interval) const;
	virtual void make_current() const;

	EAGLContext *get_context() const {return agl_context;}
    void set_default_frame_buffer();
    
private:
	CL_OpenGLWindowProvider_AGL & window;
	EAGLContext *agl_context;
	bool own_context;
};


class CL_OpenGLWindowProvider_AGL : public CL_OpenGLWindowProvider
{
/// \name Construction
/// \{
public:
	CL_OpenGLWindowProvider_AGL();
	~CL_OpenGLWindowProvider_AGL();
/// \}
    
/// \name Attributes
/// \{
public:
	CL_Rect get_geometry() const {return cocoa_window.get_geometry();}
	CL_Rect get_viewport() const {return cocoa_window.get_viewport();}
	bool is_fullscreen() const {return cocoa_window.is_fullscreen();}
	bool has_focus() const {return cocoa_window.has_focus();}
	bool is_minimized() const {return cocoa_window.is_minimized();}
	bool is_maximized() const {return cocoa_window.is_maximized();}
	bool is_visible() const {return cocoa_window.is_visible();}
	bool is_clipboard_text_available() const { return cocoa_window.is_clipboard_text_available(); }
	bool is_clipboard_image_available() const { return cocoa_window.is_clipboard_image_available(); }
	CL_String get_title() const { return cocoa_window.get_title(); }
	CL_Size get_minimum_size(bool client_area) const { return cocoa_window.get_minimum_size(client_area); }
	CL_Size get_maximum_size(bool client_area) const { return cocoa_window.get_maximum_size(client_area); }
	CL_String get_clipboard_text() const { return cocoa_window.get_clipboard_text(); }
	CL_PixelBuffer get_clipboard_image() const { return cocoa_window.get_clipboard_image(); }
	EAGLContext *get_opengl_context() { return opengl_context; }
	CL_GraphicContext& get_gc() { return gc; }
	CL_InputContext& get_ic() { return cocoa_window.get_ic(); }
	CL_GraphicContext gc;
/// \}

/// \name Operations
/// \{
public:
	CL_Point client_to_screen(const CL_Point &client) { return cocoa_window.client_to_screen(client); }
	CL_Point screen_to_client(const CL_Point &screen) { return cocoa_window.screen_to_client(screen); }
	void create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);
	void show_system_cursor() { cocoa_window.show_system_cursor(); }
	CL_CursorProvider *create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot);
	void set_cursor(CL_CursorProvider *cursor);
	void set_cursor(CL_StandardCursor type) { cocoa_window.set_cursor(type); }
	void hide_system_cursor()  { cocoa_window.hide_system_cursor(); }
	void set_title(const CL_StringRef &new_title) { cocoa_window.set_title(new_title); }
	void set_position(const CL_Rect &pos, bool client_area) { return cocoa_window.set_position(pos, client_area); };
	void set_size(int width, int height, bool client_area)  { return cocoa_window.set_size(width, height, client_area); }
	void set_minimum_size(int width, int height, bool client_area) { return cocoa_window.set_minimum_size(width, height, client_area); }
	void set_maximum_size( int width, int height, bool client_area) { return cocoa_window.set_maximum_size(width, height, client_area); }
	void set_enabled(bool enable) { return cocoa_window.set_enabled(enable); }
	void minimize() { cocoa_window.minimize(); }
	void restore() { cocoa_window.restore(); }
	void maximize() { cocoa_window.maximize(); }
	void show(bool activate)  { cocoa_window.show(activate); }
	void hide() { cocoa_window.hide(); }
	void bring_to_front() { cocoa_window.bring_to_front(); }
	void flip(int interval);
	void update(const CL_Rect &rect);
	void capture_mouse(bool capture) { cocoa_window.capture_mouse(capture); }
	void process_messages();

	EAGLContext *create_context();
    void setup_default_framebuffer();
    void set_default_frame_buffer();

	void set_clipboard_text(const CL_StringRef &text) { cocoa_window.set_clipboard_text(text); }
	void set_clipboard_image(const CL_PixelBuffer &buf) { cocoa_window.set_clipboard_image(buf); }
	void request_repaint(const CL_Rect &rect) { cocoa_window.request_repaint(rect); }

	void set_large_icon(const CL_PixelBuffer &image);
	void set_small_icon(const CL_PixelBuffer &image);
/// \}

/// \name Implementation
/// \{
private:
	void on_window_resized();

	CL_CocoaWindow cocoa_window;
	EAGLContext *opengl_context;
    GLuint default_framebuffer_handle;
    GLuint default_colorbuffer_handle;
    GLuint default_depthbuffer_handle;
	CL_DisplayWindowSite *site;
	int swap_interval;
/// \}
};

#endif

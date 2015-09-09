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
**  Trigve Siver
**  Mark Page
*/

#pragma once

#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "Display/Platform/X11/x11_window.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/GL/opengl_context_description.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

//#include <X11/extensions/XInput.h>

#include <GL/glx.h>

#include "API/GL/opengl_wrap.h"

namespace clan
{

typedef int (*ptr_glXSwapIntervalSGI)(int interval);
typedef int (*ptr_glXSwapIntervalMESA)(int interval);
typedef void (*ptr_glXSwapIntervalEXT)(::Display *dptr, GLXDrawable drawable, int interval);
typedef GLXContext (*ptr_glXCreateContextAttribs)(::Display *dpy, GLXFBConfig config, GLXContext share_list, Bool direct, const int *attrib_list);

class OpenGLWindowProvider;
class OpenGLContextDescription;
class DisplayWindowDescription;

#define GL_USE_DLOPEN		// Using dlopen for linux by default

class GL_GLXFunctions
{
public:
	typedef XVisualInfo* (GLFUNC *ptr_glXChooseVisual)(::Display *dpy, int screen, int *attrib_list);
	typedef void (GLFUNC *ptr_glXCopyContext)(::Display *dpy, GLXContext src, GLXContext dst, unsigned long mask);
	typedef GLXContext (GLFUNC *ptr_glXCreateContext)(::Display *dpy, XVisualInfo *vis, GLXContext share_list, Bool direct);
	typedef GLXPixmap (GLFUNC *ptr_glXCreateGLXPixmap)(::Display *dpy, XVisualInfo *vis, Pixmap pixmap);
	typedef void (GLFUNC *ptr_glXDestroyContext)(::Display *dpy, GLXContext ctx);
	typedef void (GLFUNC *ptr_glXDestroyGLXPixmap)(::Display *dpy, GLXPixmap pix);
	typedef int (GLFUNC *ptr_glXGetConfig)(::Display *dpy, XVisualInfo *vis, int attrib, int *value);
	typedef GLXContext (GLFUNC *ptr_glXGetCurrentContext)(void);
	typedef GLXDrawable (GLFUNC *ptr_glXGetCurrentDrawable)(void);
	typedef Bool (GLFUNC *ptr_glXIsDirect)(::Display *dpy, GLXContext ctx);
	typedef Bool (GLFUNC *ptr_glXMakeCurrent)(::Display *dpy, GLXDrawable drawable, GLXContext ctx);
	typedef Bool (GLFUNC *ptr_glXQueryExtension)(::Display *dpy, int *error_base, int *event_base);
	typedef Bool (GLFUNC *ptr_glXQueryVersion)(::Display *dpy, int *major, int *minor);
	typedef void (GLFUNC *ptr_glXSwapBuffers)(::Display *dpy, GLXDrawable drawable);
	typedef void (GLFUNC *ptr_glXUseXFont)(Font font, int first, int count, int list_base);
	typedef void (GLFUNC *ptr_glXWaitGL)(void);
	typedef void (GLFUNC *ptr_glXWaitX)(void);
	typedef const char *(GLFUNC *ptr_glXGetClientString)(::Display *dpy, int name);
	typedef const char *(GLFUNC *ptr_glXQueryServerString)(::Display *dpy, int screen, int name);
	typedef const char *(GLFUNC *ptr_glXQueryExtensionsString)(::Display *dpy, int screen);
	typedef ::Display *(GLFUNC *ptr_glXGetCurrentDisplay)(void);
	typedef GLXFBConfig *(GLFUNC *ptr_glXChooseFBConfig)(::Display *dpy, int screen, const int *attrib_list, int *nelements);
	typedef GLXContext (GLFUNC *ptr_glXCreateNewContext)(::Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
	typedef GLXPbuffer (GLFUNC *ptr_glXCreatePbuffer)(::Display *dpy, GLXFBConfig config, const int *attrib_list);
	typedef GLXPixmap (GLFUNC *ptr_glXCreatePixmap)(::Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list);
	typedef GLXWindow (GLFUNC *ptr_glXCreateWindow)(::Display *dpy, GLXFBConfig config, Window win, const int *attrib_list);
	typedef void (GLFUNC *ptr_glXDestroyPbuffer)(::Display *dpy, GLXPbuffer pbuf);
	typedef void (GLFUNC *ptr_glXDestroyPixmap)(::Display *dpy, GLXPixmap pixmap);
	typedef void (GLFUNC *ptr_glXDestroyWindow)(::Display *dpy, GLXWindow win);
	typedef GLXDrawable (GLFUNC *ptr_glXGetCurrentReadDrawable)(void);
	typedef int (GLFUNC *ptr_glXGetFBConfigAttrib)(::Display *dpy, GLXFBConfig config, int attribute, int *value);
	typedef GLXFBConfig *(GLFUNC *ptr_glXGetFBConfigs)(::Display *dpy, int screen, int *nelements);
	typedef void (GLFUNC *ptr_glXGetSelectedEvent)(::Display *dpy, GLXDrawable draw, unsigned long *event_mask);
	typedef XVisualInfo *(GLFUNC *ptr_glXGetVisualFromFBConfig)(::Display *dpy, GLXFBConfig config);
	typedef Bool (GLFUNC *ptr_glXMakeContextCurrent)(::Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
	typedef int (GLFUNC *ptr_glXQueryContext)(::Display *dpy, GLXContext ctx, int attribute, int *value);
	typedef void (GLFUNC *ptr_glXQueryDrawable)(::Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
	typedef void (GLFUNC *ptr_glXSelectEvent)(::Display *dpy, GLXDrawable draw, unsigned long event_mask);

	typedef __GLXextFuncPtr (GLFUNC *ptr_glXGetProcAddress) (const GLubyte *);
	typedef void (*(GLFUNC *ptr_glXGetProcAddressARB)(const GLubyte *procName))(void);

	typedef GLXPbuffer (*ptr_glXCreatePbufferSGIX)(
		::Display *dpy,
		GLXFBConfig config,
		unsigned int width,
		unsigned int height,
		int *attrib_list);

public:
	ptr_glXChooseVisual glXChooseVisual;
	ptr_glXCopyContext glXCopyContext;
	ptr_glXCreateContext glXCreateContext;
	ptr_glXCreateGLXPixmap glXCreateGLXPixmap;
	ptr_glXDestroyContext glXDestroyContext;
	ptr_glXDestroyGLXPixmap glXDestroyGLXPixmap;
	ptr_glXGetConfig glXGetConfig;
	ptr_glXGetCurrentContext glXGetCurrentContext;
	ptr_glXGetCurrentDrawable glXGetCurrentDrawable;
	ptr_glXIsDirect glXIsDirect;
	ptr_glXMakeCurrent glXMakeCurrent;
	ptr_glXQueryExtension glXQueryExtension;
	ptr_glXQueryVersion glXQueryVersion;
	ptr_glXSwapBuffers glXSwapBuffers;
	ptr_glXUseXFont glXUseXFont;
	ptr_glXWaitGL glXWaitGL;
	ptr_glXWaitX glXWaitX;
	ptr_glXGetClientString glXGetClientString;
	ptr_glXQueryServerString glXQueryServerString;
	ptr_glXQueryExtensionsString glXQueryExtensionsString;
	ptr_glXGetCurrentDisplay glXGetCurrentDisplay;
	ptr_glXChooseFBConfig glXChooseFBConfig;
	ptr_glXCreateNewContext glXCreateNewContext;
	ptr_glXCreatePbuffer glXCreatePbuffer;
	ptr_glXCreatePixmap glXCreatePixmap;
	ptr_glXCreateWindow glXCreateWindow;
	ptr_glXDestroyPbuffer glXDestroyPbuffer;
	ptr_glXDestroyPixmap glXDestroyPixmap;
	ptr_glXDestroyWindow glXDestroyWindow;
	ptr_glXGetCurrentReadDrawable glXGetCurrentReadDrawable;
	ptr_glXGetFBConfigAttrib glXGetFBConfigAttrib;
	ptr_glXGetFBConfigs glXGetFBConfigs;
	ptr_glXGetSelectedEvent glXGetSelectedEvent;
	ptr_glXGetVisualFromFBConfig glXGetVisualFromFBConfig;
	ptr_glXMakeContextCurrent glXMakeContextCurrent;
	ptr_glXQueryContext glXQueryContext;
	ptr_glXQueryDrawable glXQueryDrawable;
	ptr_glXSelectEvent glXSelectEvent;
	ptr_glXGetProcAddress glXGetProcAddress;
	ptr_glXGetProcAddressARB glXGetProcAddressARB;
	ptr_glXCreatePbufferSGIX glXCreatePbufferSGIX;
	ptr_glXDestroyPbuffer glXDestroyPbufferSGIX;
	ptr_glXChooseFBConfig glXChooseFBConfigSGIX;
	ptr_glXGetVisualFromFBConfig glXGetVisualFromFBConfigSGIX;

};

class OpenGLWindowProvider : public DisplayWindowProvider
{
/// \name Construction
/// \{

public:
	OpenGLWindowProvider(OpenGLContextDescription &opengl_desc);

	~OpenGLWindowProvider();

/// \}
/// \name Attributes
/// \{

public:
	bool is_double_buffered() const { return true; }

	Rect get_geometry() const override { return x11_window.get_geometry(); }
	Rect get_viewport() const override { return x11_window.get_viewport(); }

	float get_pixel_ratio() const override { return x11_window.get_pixel_ratio(); }

	bool has_focus() const override { return x11_window.has_focus(); }
	bool is_fullscreen() const override { return x11_window.is_fullscreen(); }
	bool is_minimized() const override { return x11_window.is_minimized(); }
	bool is_maximized() const override { return x11_window.is_maximized(); }
	bool is_visible() const override { return x11_window.is_visible(); }

	bool is_clipboard_text_available() const override { return x11_window.is_clipboard_text_available(); }
	bool is_clipboard_image_available() const override { return x11_window.is_clipboard_image_available(); }

	std::string get_title() const override { return x11_window.get_title(); }
	Size get_minimum_size(bool client_area) const override { return x11_window.get_minimum_size(client_area); }
	Size get_maximum_size(bool client_area) const override { return x11_window.get_maximum_size(client_area); }

	std::string get_clipboard_text() const override { return x11_window.get_clipboard_text(); }

	PixelBuffer get_clipboard_image() const override { return x11_window.get_clipboard_image(); }

	DisplayWindowHandle get_handle() const override { return x11_window.get_handle(); }

	/// \brief Returns the GLX rendering context for this window.
	GLXContext get_opengl_context() { return opengl_context; }

	GraphicContext& get_gc() override { return gc; }

	InputContext get_ic() override { return x11_window.get_ic(); }

	GraphicContext gc;

	GL_GLXFunctions glx;

	ProcAddress *get_proc_address(const std::string& function_name) const;

/// \}
/// \name Operations
/// \{

public:
	void make_current() const;
	void destroy() { delete this; }

	Point client_to_screen(const Point &client) override { return x11_window.client_to_screen(client); }
	Point screen_to_client(const Point &screen) override { return x11_window.screen_to_client(screen); }

	void create(DisplayWindowSite *site, const DisplayWindowDescription &description) override;

	void show_system_cursor() override { x11_window.show_system_cursor(); }
	CursorProvider *create_cursor(const CursorDescription &cursor_description) override;
	void set_cursor(CursorProvider *cursor) override;
	void set_cursor(StandardCursor type) override { x11_window.set_cursor(type); }
	void hide_system_cursor() override  { x11_window.hide_system_cursor(); }

	void set_title(const std::string &new_title) override { x11_window.set_title(new_title); }
	void set_position(const Rect &pos, bool client_area) override { return x11_window.set_position(pos, client_area); }

	void set_pixel_ratio(float ratio) override { return x11_window.set_pixel_ratio(ratio); }
	void set_size(int width, int height, bool client_area) override { return x11_window.set_size(width, height, client_area); }
	void set_minimum_size(int width, int height, bool client_area) override { return x11_window.set_minimum_size(width, height, client_area); }
	void set_maximum_size( int width, int height, bool client_area) override { return x11_window.set_maximum_size(width, height, client_area); }

	void set_enabled(bool enable) override { return x11_window.set_enabled(enable); }

	void minimize() override { x11_window.minimize(); }
	void restore() override { x11_window.restore(); }
	void maximize() override { x11_window.maximize(); }
	void toggle_fullscreen() override { } // FIXME: real implementation
	void show(bool activate) override  { x11_window.show(activate); }
	void hide() override { x11_window.hide(); }
	void bring_to_front() override { x11_window.bring_to_front(); }

	/// \brief Flip opengl buffers.
	void flip(int interval) override;

	/// \brief Capture/Release the mouse.
	void capture_mouse(bool capture) override { x11_window.capture_mouse(capture); }

	void process_messages();

	GLXContext create_context(const DisplayWindowDescription &desc);

	/// \brief Check for window messages
	/** \return true when there is a message*/
	//bool has_messages() { return x11_window.has_messages(); }

	void set_clipboard_text(const std::string &text) override { x11_window.set_clipboard_text(text); }

	void set_clipboard_image(const PixelBuffer &buf) override { x11_window.set_clipboard_image(buf); }

	void request_repaint() override { x11_window.request_repaint(); }

	void set_large_icon(const PixelBuffer &image) override;
	void set_small_icon(const PixelBuffer &image) override;

	void enable_alpha_channel(const Rect &blur_rect) override;
	void extend_frame_into_client_area(int left, int top, int right, int bottom) override;

/// \}
/// \name Implementation
/// \{

private:

	bool on_clicked(XButtonEvent &event);

	GLXContext create_context_glx_1_3(const DisplayWindowDescription &desc, GLXContext shared_context);
	GLXContext create_context_glx_1_2(const DisplayWindowDescription &desc, GLXContext shared_context);
	void create_glx_1_3(DisplayWindowSite *new_site, const DisplayWindowDescription &desc, ::Display *disp);
	void create_glx_1_2(DisplayWindowSite *new_site, const DisplayWindowDescription &desc, ::Display *disp);
	GLXContext create_context_glx_1_3_helper(GLXContext shared_context, int major_version, int minor_version, const DisplayWindowDescription &desc, ptr_glXCreateContextAttribs glXCreateContextAttribs);
	void get_opengl_version(int &version_major, int &version_minor);

	void on_window_resized();

	bool is_glx_extension_supported(const char *ext_name);

	void setup_extension_pointers();

	X11Window x11_window;

	/// \brief GLX rendering context handle.
	GLXContext opengl_context;

	DisplayWindowSite *site;

	XVisualInfo *opengl_visual_info;

	ptr_glXSwapIntervalSGI glXSwapIntervalSGI;
	ptr_glXSwapIntervalMESA glXSwapIntervalMESA;
	ptr_glXSwapIntervalEXT glXSwapIntervalEXT = nullptr;
	int swap_interval;

	GLXFBConfig fbconfig;

#ifdef GL_USE_DLOPEN
	void *opengl_lib_handle;
#endif
	bool glx_1_3;
	OpenGLContextDescription opengl_desc;
	bool using_gl3;

	friend class PBuffer_GL1_Impl;

/// \}
};

}


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
**  Trigve Siver
**  Mark Page
*/

#pragma once


#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "Display/X11/x11_window.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/GL1/opengl1_wrap.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

//#include <X11/extensions/XInput.h>

#include <GL/glx.h>

typedef int (*ptr_glXSwapIntervalSGI)(int interval);
typedef int (*ptr_glXSwapIntervalMESA)(int interval);

class PBuffer_GL1;
class CL_GL1WindowProvider_GLX;
class CL_GL1GraphicContextProvider;

class OpenGLWindowProvider_GLX;

#define GL_USE_DLOPEN		// Using dlopen for linux by default

class CL_GL1_GLXFunctions
{
public:
	typedef XVisualInfo* (CL_GL1FUNC *ptr_glXChooseVisual)(Display *dpy, int screen, int *attrib_list);
	typedef void (CL_GL1FUNC *ptr_glXCopyContext)(Display *dpy, GLXContext src, GLXContext dst, unsigned long mask);
	typedef GLXContext (CL_GL1FUNC *ptr_glXCreateContext)(Display *dpy, XVisualInfo *vis, GLXContext share_list, Bool direct);
	typedef GLXPixmap (CL_GL1FUNC *ptr_glXCreateGLXPixmap)(Display *dpy, XVisualInfo *vis, Pixmap pixmap);
	typedef void (CL_GL1FUNC *ptr_glXDestroyContext)(Display *dpy, GLXContext ctx);
	typedef void (CL_GL1FUNC *ptr_glXDestroyGLXPixmap)(Display *dpy, GLXPixmap pix);
	typedef int (CL_GL1FUNC *ptr_glXGetConfig)(Display *dpy, XVisualInfo *vis, int attrib, int *value);
	typedef GLXContext (CL_GL1FUNC *ptr_glXGetCurrentContext)(void);
	typedef GLXDrawable (CL_GL1FUNC *ptr_glXGetCurrentDrawable)(void);
	typedef Bool (CL_GL1FUNC *ptr_glXIsDirect)(Display *dpy, GLXContext ctx);
	typedef Bool (CL_GL1FUNC *ptr_glXMakeCurrent)(Display *dpy, GLXDrawable drawable, GLXContext ctx);
	typedef Bool (CL_GL1FUNC *ptr_glXQueryExtension)(Display *dpy, int *error_base, int *event_base);
	typedef Bool (CL_GL1FUNC *ptr_glXQueryVersion)(Display *dpy, int *major, int *minor);
	typedef void (CL_GL1FUNC *ptr_glXSwapBuffers)(Display *dpy, GLXDrawable drawable);
	typedef void (CL_GL1FUNC *ptr_glXUseXFont)(Font font, int first, int count, int list_base);
	typedef void (CL_GL1FUNC *ptr_glXWaitGL)(void);
	typedef void (CL_GL1FUNC *ptr_glXWaitX)(void);
	typedef const char *(CL_GL1FUNC *ptr_glXGetClientString)(Display *dpy, int name);
	typedef const char *(CL_GL1FUNC *ptr_glXQueryServerString)(Display *dpy, int screen, int name);
	typedef const char *(CL_GL1FUNC *ptr_glXQueryExtensionsString)(Display *dpy, int screen);
	typedef Display *(CL_GL1FUNC *ptr_glXGetCurrentDisplay)(void);
	typedef GLXFBConfig *(CL_GL1FUNC *ptr_glXChooseFBConfig)(Display *dpy, int screen, const int *attrib_list, int *nelements);
	typedef GLXContext (CL_GL1FUNC *ptr_glXCreateNewContext)(Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
	typedef GLXPbuffer (CL_GL1FUNC *ptr_glXCreatePbuffer)(Display *dpy, GLXFBConfig config, const int *attrib_list);
	typedef GLXPixmap (CL_GL1FUNC *ptr_glXCreatePixmap)(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list);
	typedef GLXWindow (CL_GL1FUNC *ptr_glXCreateWindow)(Display *dpy, GLXFBConfig config, Window win, const int *attrib_list);
	typedef void (CL_GL1FUNC *ptr_glXDestroyPbuffer)(Display *dpy, GLXPbuffer pbuf);
	typedef void (CL_GL1FUNC *ptr_glXDestroyPixmap)(Display *dpy, GLXPixmap pixmap);
	typedef void (CL_GL1FUNC *ptr_glXDestroyWindow)(Display *dpy, GLXWindow win);
	typedef GLXDrawable (CL_GL1FUNC *ptr_glXGetCurrentReadDrawable)(void);
	typedef int (CL_GL1FUNC *ptr_glXGetFBConfigAttrib)(Display *dpy, GLXFBConfig config, int attribute, int *value);
	typedef GLXFBConfig *(CL_GL1FUNC *ptr_glXGetFBConfigs)(Display *dpy, int screen, int *nelements);
	typedef void (CL_GL1FUNC *ptr_glXGetSelectedEvent)(Display *dpy, GLXDrawable draw, unsigned long *event_mask);
	typedef XVisualInfo *(CL_GL1FUNC *ptr_glXGetVisualFromFBConfig)(Display *dpy, GLXFBConfig config);
	typedef Bool (CL_GL1FUNC *ptr_glXMakeContextCurrent)(Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
	typedef int (CL_GL1FUNC *ptr_glXQueryContext)(Display *dpy, GLXContext ctx, int attribute, int *value);
	typedef void (CL_GL1FUNC *ptr_glXQueryDrawable)(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
	typedef void (CL_GL1FUNC *ptr_glXSelectEvent)(Display *dpy, GLXDrawable draw, unsigned long event_mask);

	typedef __GLXextFuncPtr (CL_GL1FUNC *ptr_glXGetProcAddress) (const GLubyte *);
	typedef void (*(CL_GL1FUNC *ptr_glXGetProcAddressARB)(const GLubyte *procName))(void);

	typedef GLXPbuffer (*ptr_glXCreatePbufferSGIX)(
		Display *dpy,
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

	bool glx_1_3;
	ptr_glXCreatePbufferSGIX glXCreatePbufferSGIX;
	ptr_glXDestroyPbuffer glXDestroyPbufferSGIX;
	ptr_glXChooseFBConfig glXChooseFBConfigSGIX; 
	ptr_glXGetVisualFromFBConfig glXGetVisualFromFBConfigSGIX;
};

class CL_GL1WindowProvider_GLX : public DisplayWindowProvider
{
/// \name Construction
/// \{

public:
	CL_GL1WindowProvider_GLX();

	~CL_GL1WindowProvider_GLX();


/// \}
/// \name Attributes
/// \{

public:
	Rect get_geometry() const {return x11_window.get_geometry();}

	Rect get_viewport() const {return x11_window.get_viewport();}

	bool is_fullscreen() const {return x11_window.is_fullscreen();}

	bool has_focus() const {return x11_window.has_focus();}

	bool is_minimized() const {return x11_window.is_minimized();}

	bool is_maximized() const {return x11_window.is_maximized();}

	bool is_visible() const {return x11_window.is_visible();}

	std::string get_title() const { return x11_window.get_title(); }
	Size get_minimum_size(bool client_area) const { return x11_window.get_minimum_size(client_area); }
	Size get_maximum_size(bool client_area) const { return x11_window.get_maximum_size(client_area); }

	bool is_clipboard_text_available() const { return x11_window.is_clipboard_text_available(); }

	bool is_clipboard_image_available() const { return x11_window.is_clipboard_image_available(); }

	std::string get_clipboard_text() const { return x11_window.get_clipboard_text(); }

	PixelBuffer get_clipboard_image() const { return x11_window.get_clipboard_image(); }

	/// \brief Returns the X11 display handle.
	Display *get_display() const { return x11_window.get_display(); }

	/// \brief Handle to X11 window handle.
	Window get_window() const { return x11_window.get_window(); }

	/// \brief Returns the GLX rendering context for this window.
	GLXContext get_opengl_context() { return opengl_context; }

	GraphicContext& get_gc() { return gc; }

	InputContext get_ic() { return x11_window.get_ic(); }

	GraphicContext gc;

	CL_GL1_GLXFunctions glx;

/// \}
/// \name Operations
/// \{

public:
	CL_GL1ProcAddress *get_proc_address(const std::string& function_name) const;

	void make_current() const;
	void destroy() { delete this; }
	Point client_to_screen(const Point &client) { return x11_window.client_to_screen(client); }

	Point screen_to_client(const Point &screen) { return x11_window.screen_to_client(screen); }

	void create(DisplayWindowSite *site, const DisplayWindowDescription &description);

	void show_system_cursor() { x11_window.show_system_cursor(); }

	CursorProvider *create_cursor(const SpriteDescription &sprite_description, const Point &hotspot);

	void set_cursor(CursorProvider *cursor);

	void set_cursor(StandardCursor type) { x11_window.set_cursor(type); }

	void hide_system_cursor()  { x11_window.hide_system_cursor(); }

	void set_title(const std::string &new_title) { x11_window.set_title(new_title); }

	void set_position(const Rect &pos, bool client_area) { return x11_window.set_position(pos, client_area); };

	void set_size(int width, int height, bool client_area)  { return x11_window.set_size(width, height, client_area); }

	void set_minimum_size(int width, int height, bool client_area) { return x11_window.set_minimum_size(width, height, client_area); }

	void set_maximum_size( int width, int height, bool client_area) { return x11_window.set_maximum_size(width, height, client_area); }

	void set_enabled(bool enable) { return x11_window.set_enabled(enable); }

	void minimize() { x11_window.minimize(); }

	void restore() { x11_window.restore(); }

	void maximize() { x11_window.maximize(); }

	void show(bool activate)  { x11_window.show(activate); }

	void hide() { x11_window.hide(); }

	void bring_to_front() { x11_window.bring_to_front(); }

	/// \brief Flip opengl buffers.
	void flip(int interval);

	/// \brief Copy a region of the backbuffer to the frontbuffer.
	void update(const Rect &rect);

	/// \brief Capture/Release the mouse.
	void capture_mouse(bool capture) { x11_window.capture_mouse(capture); }

	void process_messages();

	GLXContext create_context();

	/// \brief Check for window messages
	/** \return true when there is a message*/
	//bool has_messages() { return x11_window.has_messages(); }

	void set_clipboard_text(const std::string &text) { x11_window.set_clipboard_text(text); }

	void set_clipboard_image(const PixelBuffer &buf) { x11_window.set_clipboard_image(buf); }

	void request_repaint(const Rect &rect) { x11_window.request_repaint(rect); }

	PBuffer_GL1 create_pbuffer(CL_GL1GraphicContextProvider *gc_provider, Size size);

	GLXContext get_share_context();

	void set_large_icon(const PixelBuffer &image);
	void set_small_icon(const PixelBuffer &image);

/// \}
/// \name Implementation
/// \{

private:

	void on_window_resized();

	bool is_glx_extension_supported(const char *ext_name);

	void setup_extension_pointers();

	CL_X11Window x11_window;

	/// \brief GLX rendering context handle.
	GLXContext opengl_context;

	DisplayWindowSite *site;

	XVisualInfo *opengl_visual_info;

	ptr_glXSwapIntervalSGI glXSwapIntervalSGI;
	ptr_glXSwapIntervalMESA glXSwapIntervalMESA;

	int swap_interval;

#ifdef GL_USE_DLOPEN
	void *opengl_lib_handle;
#endif

/// \}
};




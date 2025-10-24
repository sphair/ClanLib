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

#include <cstdio>
#include <errno.h>

#include "display_window_opengl.h"
#include "input_device_linuxjoystick.h"
#include "input_device_linuxusbmouse.h"
#include "input_device_linuxevent.h"
#include "Display/display_precomp.h"
#include "Display/display_window_generic.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/system.h"
#include "graphic_context_opengl.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"
#include "GL/gl_state_checker.h"
#include "GL/GLX/input_device_x11keyboard.h"
#include "GL/GLX/input_device_x11mouse.h"
#include "GL/GLX/input_device_xinput.h"
#include "API/Core/System/clanstring.h"
#include "API/GL/opengl_window_description.h"
#include "API/GL/opengl_state.h"
#include "API/GL/opengl_wrap.h"
#include "../opengl_window_description_generic.h"
#include <unistd.h>
#include <X11/extensions/XInput.h>

GLXContext CL_DisplayWindow_OpenGL::share_context = 0;

Display *CL_DisplayWindow_OpenGL::disp = 0;

int CL_DisplayWindow_OpenGL::disp_ref_count = 0;

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_DisplayWindow_OpenGL::CL_DisplayWindow_OpenGL() :
	left_ctrl_down(false), left_alt_down(false), left_shift_down(false),
	right_ctrl_down(false), right_alt_down(false), right_shift_down(false),
	fullscreen(false), fullscreen_width(0), fullscreen_height(0),
	saved_position(0, 0, 0, 0),
	context(0),
	window(0)
{
	gc = CL_GraphicContext(new CL_GraphicContext_OpenGL(this));
}

CL_DisplayWindow_OpenGL::~CL_DisplayWindow_OpenGL()
{
	if(fullscreen)
		set_windowed();
	destroy_window();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_DisplayWindow_OpenGL::get_width() const
{
	XWindowAttributes attr;
	XGetWindowAttributes(disp, window, &attr);

	return attr.width;
}

int CL_DisplayWindow_OpenGL::get_height() const
{
	XWindowAttributes attr;
	XGetWindowAttributes(disp, window, &attr);

	return attr.height;
}

bool CL_DisplayWindow_OpenGL::is_fullscreen() const
{
	return fullscreen;
}

bool CL_DisplayWindow_OpenGL::has_focus() const
{
	return focus;
}

CL_PixelBuffer CL_DisplayWindow_OpenGL::get_buffer(int i) const
{
	if (i == 0) 
	{
		return gc.get_pixeldata(CL_Rect(0, 0, get_width(), get_height()), CL_FRONT);
	}
	else
	{
		return gc.get_pixeldata(CL_Rect(0, 0, get_width(), get_height()), CL_BACK);
	}
}

int CL_DisplayWindow_OpenGL::get_buffer_count() const
{
	return 2;
}

CL_GraphicContext *CL_DisplayWindow_OpenGL::get_gc()
{
	return &gc;
}

Display *CL_DisplayWindow_OpenGL::get_hwnd()
{
	return disp;
}

CL_Size CL_DisplayWindow_OpenGL::get_max_surface_size() const
{
	CLint max_texture_size;
	clGetIntegerv(CL_MAX_TEXTURE_SIZE, &max_texture_size);
	return CL_Size(max_texture_size, max_texture_size);
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_DisplayWindow_OpenGL::create_window(const CL_DisplayWindowDescription &desc)
{
	const CL_OpenGLWindowDescription_Generic *gl_desc = 0;
	gl_desc = dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(desc.impl.get());

	fullscreen_width  = desc.get_size().width;
	fullscreen_height = desc.get_size().height;

	XVisualInfo *vi;
	Colormap cmap;

	if (disp == 0)
	{
		disp = XOpenDisplay(0);
		if (disp == 0) throw CL_Error("Could not open X11 display!");
	}
	disp_ref_count++;

	int gl_attribs_single[] =
	{
		GLX_RGBA,
		GLX_RED_SIZE, 4,
		GLX_GREEN_SIZE, 4,
		GLX_BLUE_SIZE, 4,
		GLX_DEPTH_SIZE, 16,
		None
	};

	int gl_attribs[32];
	int i = 0;
	
	if( gl_desc )
	{
		if( gl_desc->rgba ) gl_attribs[i++] = GLX_RGBA;
		if( gl_desc->doublebuffer ) gl_attribs[i++] = GLX_DOUBLEBUFFER;
		if( gl_desc->stereo ) gl_attribs[i++] = GLX_STEREO;
		gl_attribs[i++] = GLX_BUFFER_SIZE;
		gl_attribs[i++] = gl_desc->buffer_size;
		gl_attribs[i++] = GLX_LEVEL;
		gl_attribs[i++] = gl_desc->level;
		gl_attribs[i++] = GLX_AUX_BUFFERS;
		gl_attribs[i++] = gl_desc->aux_buffers;
		gl_attribs[i++] = GLX_RED_SIZE; 
		gl_attribs[i++] = gl_desc->red_size;
		gl_attribs[i++] = GLX_GREEN_SIZE;
		gl_attribs[i++] = gl_desc->green_size;
		gl_attribs[i++] = GLX_BLUE_SIZE;
		gl_attribs[i++] = gl_desc->blue_size;
		gl_attribs[i++] = GLX_DEPTH_SIZE;
		gl_attribs[i++] = gl_desc->depth_size;
		gl_attribs[i++] = GLX_STENCIL_SIZE;
		gl_attribs[i++] = gl_desc->stencil_size;
		gl_attribs[i++] = GLX_ACCUM_RED_SIZE;
		gl_attribs[i++] = gl_desc->accum_red_size;
		gl_attribs[i++] = GLX_ACCUM_GREEN_SIZE;
		gl_attribs[i++] = gl_desc->accum_green_size;
		gl_attribs[i++] = GLX_ACCUM_BLUE_SIZE;
		gl_attribs[i++] = gl_desc->accum_blue_size;
		gl_attribs[i++] = GLX_ACCUM_ALPHA_SIZE;
		gl_attribs[i++] = gl_desc->accum_alpha_size;
		gl_attribs[i++] = GLX_ACCUM_RED_SIZE;
		gl_attribs[i++] = gl_desc->accum_red_size;
		gl_attribs[i++] = None;
	}
	else
	{
		gl_attribs[i++] = GLX_RGBA;
		gl_attribs[i++] = GLX_DOUBLEBUFFER;
		gl_attribs[i++] = GLX_RED_SIZE;
		gl_attribs[i++] = 4;
		gl_attribs[i++] = GLX_GREEN_SIZE;
		gl_attribs[i++] = 4;
		gl_attribs[i++] = GLX_BLUE_SIZE;
		gl_attribs[i++] = 4;
		gl_attribs[i++] = GLX_DEPTH_SIZE;
		gl_attribs[i++] = 16;
		gl_attribs[i++] = None;
	}

	// get an appropriate visual
	vi = glXChooseVisual(disp, DefaultScreen(disp), gl_attribs);
	
	if (vi == NULL)
	{
		vi = glXChooseVisual(disp, window, gl_attribs_single);
		printf("Requested visual not supported by your OpenGL implementation. Falling back on singlebuffered Visual!\n");
	}

	// create a GLX context
	context = glXCreateContext(disp, vi, share_context, True);

	if( share_context == NULL )
		share_context = context;

	glXGetConfig(disp, vi, GLX_BUFFER_SIZE, &glx_bpp);

	// create a color map
	cmap = XCreateColormap( disp, RootWindow(disp, vi->screen), vi->visual, AllocNone);

	attributes.colormap = cmap;
	attributes.border_pixel = 0;
	attributes.override_redirect = False;

	// create a window in window mode
	attributes.event_mask =
		ExposureMask |
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask |
		ButtonReleaseMask |
		StructureNotifyMask |
		PointerMotionMask |
		EnterWindowMask |
		LeaveWindowMask |
		FocusChangeMask;

	window = XCreateWindow(disp, RootWindow(disp, vi->screen),
		0, 0, desc.get_size().width, desc.get_size().height, 0, vi->depth, InputOutput, vi->visual,
		CWBorderPixel | CWColormap | CWOverrideRedirect | CWEventMask, &attributes);
	
	XSelectInput(disp, window,
		FocusChangeMask | KeyPressMask | KeyReleaseMask
		| PropertyChangeMask | StructureNotifyMask |
		KeymapStateMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

	// set title of window:
	set_title(desc.get_title());
	
	// setup size hints:
	XSizeHints size_hints;
	memset(&size_hints, 0, sizeof(XSizeHints));
	size_hints.width       = desc.get_size().width;
	size_hints.height      = desc.get_size().height;
	size_hints.base_width  = desc.get_size().width;
	size_hints.base_height = desc.get_size().height;
	size_hints.min_width   = size_hints.width;
	size_hints.min_height  = size_hints.height;
	size_hints.max_width   = size_hints.width;
	size_hints.max_height  = size_hints.height;
	size_hints.flags       = PSize|PBaseSize;
	if (!desc.get_allow_resize()) size_hints.flags |= PMinSize | PMaxSize;
	XSetWMNormalHints(disp, window, &size_hints);

	// handle wm_delete_events if in windowed mode:
	Atom wm_delete = XInternAtom(disp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(disp, window, &wm_delete, 1);

	// make window visible:
	XMapRaised(disp, window);

	if (!glXIsDirect(disp, context))
		printf("No hardware acceleration available. I hope you got a really fast machine..\n");

	// Create input devices for window:
	keyboard = CL_InputDevice(new CL_InputDevice_X11Keyboard(this));
	mouse    = CL_InputDevice(new CL_InputDevice_X11Mouse(this));

	get_ic()->clear();
	get_ic()->add_keyboard(keyboard);
	get_ic()->add_mouse(mouse);

	setup_joysticks();
// 	setup_usb_mice();
//	setup_xinput();
//	setup_event();

	XSync(disp, True);
	
	focus = true;
	
	system_cursor = XCreateFontCursor(disp, XC_left_ptr);
	char *data = (char*)malloc(64); // 8x8
	memset(data, 0, 64);

	XColor black_color;
	memset(&black_color, 0, sizeof(black_color));

	cursor_bitmap = XCreateBitmapFromData(disp, window, data, 8, 8);
	hidden_cursor = XCreatePixmapCursor(disp, cursor_bitmap, cursor_bitmap, &black_color, &black_color, 0,0 );
	
	if (desc.is_fullscreen())
		set_fullscreen(desc.get_size().width, desc.get_size().height, desc.get_bpp(), desc.get_refresh_rate());
}

void
CL_DisplayWindow_OpenGL::setup_event()
{
	char pathname[256];
	bool done = false;

	for(int i = 0; !done; ++i)
	{
		snprintf(pathname, sizeof(pathname), "/dev/input/event%d", i);
		if (access(pathname, R_OK) == 0)
		{
			try
			{
				get_ic()->add_mouse(CL_InputDevice(new CL_InputDevice_LinuxEvent(this, pathname)));
			}
			catch (CL_Error& err)
			{
				CL_Log::log("debug", "EvDev Error: %1", err.message);
				done = true;
			}
		}
		else
		{
			CL_Log::log("debug", "EvDev Error: %1: %2", pathname, strerror(errno));
			done = true;
		}
	}
}

void CL_DisplayWindow_OpenGL::setup_usb_mice()
{
	char pathname[256];
	bool done = false;

	for(int i = 0; !done; ++i)
	{
		snprintf(pathname, sizeof(pathname), "/dev/input/mouse%d", i);
		if (access(pathname, R_OK) == 0)
		{
			try
			{
				get_ic()->add_mouse(CL_InputDevice(new CL_InputDevice_LinuxUSBMouse(this, pathname)));
			}
			catch (CL_Error& err)
			{
				CL_Log::log("debug", "Mouse Error: %1", err.message);
				done = true;
			}
		}
		else
		{
			CL_Log::log("debug", "Mouse Error: %1: %2", pathname, strerror(errno));
			done = true;
		}
	}
}

void CL_DisplayWindow_OpenGL::setup_joysticks()
{
	// This could need some improvments, it doesn't look for
	// /dev/input/jsX for example
	char pathname[256];
	char joydev[256];
	bool done = false;

	CL_Log::log("debug", "Setting up joystick");

	if (access("/dev/input/", R_OK | X_OK) == 0)
	{
		strcpy(joydev, "/dev/input/js%d");
	} 
	else
	{
		strcpy(joydev, "/dev/js%d");
	}

	for(int i = 0; !done; ++i)
	{
		snprintf(pathname, sizeof(pathname), joydev, i);
		
		if (access(pathname, R_OK) == 0)
		{
			try
			{
				get_ic()->add_joystick(CL_InputDevice(new CL_InputDevice_LinuxJoystick(pathname)));
			} 
			catch (CL_Error& err)
			{
				CL_Log::log("debug", "Joystick Error: %1", err.message);
				done = true;
			}
		}
		else
		{
			CL_Log::log("debug", "Joystick Error: %1: %2", pathname, strerror(errno));
			done = true;
		}
	}
}


Bool
CL_DisplayWindow_OpenGL::xinput_is_present()
{
	XExtensionVersion	*version;
	Bool		present;
    
	version = XGetExtensionVersion(disp, INAME);

	if (version && (version != (XExtensionVersion*) NoSuchExtension)) {
		present = version->present;
		XFree(version);
		return present;
	} else {
		return False;
	}
}

void
CL_DisplayWindow_OpenGL::setup_xinput()
{
	if (!xinput_is_present())
	{
		CL_Log::log("debug", "XInput extentsion not found");
	}
	else
	{
		XDeviceInfo		*info;
		int			loop;

		int		num_devices;
    
		info = XListInputDevices(disp, &num_devices);
	
		for(loop=0; loop<num_devices; loop++) {
			// FIXME: Xinput isn't necesarrily a mouse, could be anything
			if (info[loop].use == IsXExtensionDevice)
			{
				get_ic()->add_mouse(CL_InputDevice(new CL_InputDevice_XInput(this, info + loop)));
			}
		}
	}
}

void CL_DisplayWindow_OpenGL::destroy_window()
{
	set_windowed();

	if (context)
	{
		if (!glXMakeCurrent(disp, None, NULL))
		{
			printf("Could not release drawing context.\n");
		}

		glXDestroyContext(disp, context);
		context = NULL;
	}

	keyboard = CL_InputDevice();
	mouse = CL_InputDevice();
	get_ic()->clear();

	if (window) XDestroyWindow(disp, window);
	window = 0;
	
	if (disp_ref_count > 0)
	{
		disp_ref_count--;
		if (disp_ref_count == 0)
		{
			XCloseDisplay(disp);
			disp = 0;
		}
	}
}

void CL_DisplayWindow_OpenGL::on_xevent(XEvent &event)
{
	//Disabled for now
}

void CL_DisplayWindow_OpenGL::flip(int interval)
{
	CL_GLStateChecker::from_gc(get_gc())->flush();
	
	if (interval != -1)
	{
		typedef int (*ptr_glXSwapIntervalSGI)(int interval);
		ptr_glXSwapIntervalSGI glXSwapIntervalSGI = (ptr_glXSwapIntervalSGI) CL_OpenGL::get_proc_address("glXSwapIntervalSGI");
		if (glXSwapIntervalSGI)
		{
			glXSwapIntervalSGI(interval);
		}
	}
	
	XSync(disp,False);
	glXSwapBuffers(disp, window);
}

void CL_DisplayWindow_OpenGL::set_fullscreen(int width, int height, int bpp, int refresh_rate)
{
	if(fullscreen)
		return;

	// Vid-mode Switching
	XF86VidModeModeLine cur_mode;

	XF86VidModeGetModeLine(disp, 0, &dotclock, &cur_mode);

	old_mode.dotclock   = dotclock;
	old_mode.hdisplay   = cur_mode.hdisplay;
	old_mode.hsyncstart = cur_mode.hsyncstart;
	old_mode.hsyncend   = cur_mode.hsyncend;
	old_mode.htotal     = cur_mode.htotal;
	old_mode.vdisplay   = cur_mode.vdisplay;
	old_mode.vsyncstart = cur_mode.vsyncstart;
	old_mode.vsyncend   = cur_mode.vsyncend;
	old_mode.vtotal     = cur_mode.vtotal;
	old_mode.flags      = cur_mode.flags;
	old_mode.privsize   = 0;

	int num_modes;
	XF86VidModeModeInfo **modes;
	XF86VidModeGetAllModeLines(disp, 0, &num_modes, &modes);

	std::list<XF86VidModeModeInfo *> usable_modes;
	for(int i = 0; i < num_modes; i++)
	{
		if(modes[i]->hdisplay == width && modes[i]->vdisplay == height)
		{
			CL_Log::log("debug", "Useable fullscreen mode found: %1x%2", width, height);
			usable_modes.push_back(modes[i]);
		}
	}
	
	if (usable_modes.empty())
	{
		CL_Log::log("debug", "No useable fullscreen modes available!");
	}
	else 
	{	
		if(!width)
			width = get_width();

		if(!height)
			height = get_height();
	
		if(!bpp)
			bpp = glx_bpp;

		//Hide Window
		if (0)
		{  // FIXME: allow_override doesn't play together with
			// GrabPointer, not sure what is wrong but it simply doesn't
			// work.
			//
			// The code outside the 'if(0)' as it is now, works mostly,
			// however it doesn't work when the window or a part of it is
			// outside of the screen, since the window isn't moved
			// fullscreen will only show half the window, shouldn't be a
			// problem for most of the time, but will be tricky if the
			// window has equal size as the desktop.

			// Move the window into the right position, this must happen
			// BEFORE we remove control from the window manager
			XMoveResizeWindow(disp, window, 0, 0, width, height);

			// Move the mouse and switch moves
			XWarpPointer(disp, None, None, 0, 0, 0, 0, width/2, height/2);

			XUnmapWindow(disp, window);
			{ // Wait for window to disapear
				XEvent event;
				do {
					XMaskEvent(disp, StructureNotifyMask, &event);
				} while ( (event.type != UnmapNotify) || (event.xunmap.event != window) );
			}
			// Turn off WM control
			attributes.override_redirect = True;
			XChangeWindowAttributes(disp, window, CWBorderPixel | CWColormap | CWOverrideRedirect, &attributes);

			// Re-appear window
			XMapRaised(disp, window);
		}

		// Get input focus
		//XSetInputFocus(disp,window, RevertToNone, CurrentTime);
		while (1) 
		{
			int result = XGrabPointer(disp, window, True, 0, 
											  GrabModeAsync, GrabModeAsync, window, None, CurrentTime);
			if ( result == GrabSuccess ) {
				break;
			}
			CL_System::sleep(100);
		}

		XF86VidModeGetViewPort(disp, DefaultScreen(disp), &old_x, &old_y);

		XF86VidModeSwitchToMode(disp, 0, *(usable_modes.begin()));
		Window child_window;
		int x, y;
		// Get the windows absolute position (aka relative to
		// the root window)
		XTranslateCoordinates(disp, window, DefaultRootWindow(disp), 
									 0, 0, &x, &y, &child_window);
	
		XF86VidModeSetViewPort(disp, DefaultScreen(disp), x, y);
		
		XSync(disp, True);

		fullscreen = true;
	}
}

void CL_DisplayWindow_OpenGL::set_windowed()
{
	if(!fullscreen)
		return;

	if (0)
	{ // FIXME: See FIXME in set_fullscreen
		// Hide window
		XUnmapWindow(disp, window);
		XSync(disp, True);

		// Turn on WM control 
		attributes.override_redirect = False;
		XChangeWindowAttributes(disp, window, CWBorderPixel | CWColormap | CWOverrideRedirect, &attributes);

		XMoveResizeWindow(disp, window, 0, 0, fullscreen_width, fullscreen_height);

		XMapRaised(disp, window);
		XSync(disp, True);
	}

	fullscreen = false;

	// Only restore mode if we ever switched mode.
	if (old_x != -1 || old_y != -1)
	{
		XF86VidModeSwitchToMode(disp, 0, &old_mode);
		XF86VidModeSetViewPort(disp, 0, old_x, old_y);
	}

	XUngrabPointer(disp, CurrentTime);

	XSync(disp, True);
}

void CL_DisplayWindow_OpenGL::set_title(const std::string &title)
{
	XSetStandardProperties(disp, window, title.c_str(), title.c_str(), None, NULL, 0, NULL);
}

void CL_DisplayWindow_OpenGL::set_position(const CL_Rect &pos)
{
	set_position(pos.left, pos.top);
	XResizeWindow(disp, window, pos.get_width(), pos.get_height());
	set_size(pos.get_width(), pos.get_height());
}

void CL_DisplayWindow_OpenGL::set_position(int x, int y)
{
	XMoveWindow(disp, window, x, y);
}

void CL_DisplayWindow_OpenGL::set_size(int width, int height)
{
	fullscreen_width = width;
	fullscreen_height = height;

	sig_resize(width, height);
}

void CL_DisplayWindow_OpenGL::set_buffer_count(int flipping_buffers)
{
// Not implemented.
// We can't change how many buffer we use in OpenGL, the drivers pick for us
	CL_Log::log("debug", "CL_DisplayWindow_OpenGL::set_buffer_count is not implemented");
}

void CL_DisplayWindow_OpenGL::update(const CL_Rect &rect)
{
	CL_OpenGLState state(get_gc());
	state.set_active();
	state.setup_2d();
	
	CLboolean is_doublebuffered = CL_FALSE;
	clGetBooleanv(CL_DOUBLEBUFFER, &is_doublebuffered);
	if (is_doublebuffered)
	{
		clReadBuffer(CL_BACK);
		
		float raster_pos[4];
		clGetFloatv(CL_CURRENT_RASTER_POSITION, raster_pos);

		clDrawBuffer(CL_FRONT);
		clRasterPos3f(rect.left, rect.bottom, raster_pos[2]);

		clCopyPixels(rect.top, rect.left, rect.get_width(), rect.get_height(), CL_COLOR);

		clDrawBuffer(CL_BACK);
	}
}

void CL_DisplayWindow_OpenGL::show_system_cursor()
{
	XDefineCursor(disp, window, system_cursor);
}

void CL_DisplayWindow_OpenGL::hide_system_cursor()
{
	XDefineCursor(disp, window, hidden_cursor);
}

void CL_DisplayWindow_OpenGL::cursor_changed()
{
// We have to extract the data from the cursor in order to display it

// At this point nothing more than hiding/unhiding the system cursor is supported
// I'm waiting for the release of XFree86 4.3, which has the Xcursor extension library.
// This will greatly simplify this, so I don't want to start implenting this.
// --MSR, Nov. 13, 2002
}

void CL_DisplayWindow_OpenGL::keep_alive()
{
	XEvent event, next_event;
	CL_Rect *rect;
	
	for (int i=XPending(disp); i>0; i--)
	{
		XNextEvent(disp, &event);
		switch(event.type)
		{
			//Resize or Move
			case ConfigureNotify:
				#ifdef DEBUG
					CL_Log::log("debug", "ConfigureNotify Event received");
				#endif
				set_size(event.xconfigure.width,event.xconfigure.height);
				break;
			case ClientMessage:
				#ifdef DEBUG
					CL_Log::log("debug", "Received ClientMessage, sending close signal");
				#endif
				sig_window_close();
				break;
			case Expose:
				// Repaint notification
				// Could be more efficient if we checked ahead for other
				// repaint notifications
				#ifdef DEBUG
					CL_Log::log("debug", "Expose Event received");
				#endif
				rect = new CL_Rect(event.xexpose.x, event.xexpose.y,
					event.xexpose.x + event.xexpose.width, event.xexpose.y + event.xexpose.height);
				sig_paint(*rect);
				delete rect;
				break;
			case FocusIn:
				#ifdef DEBUG
					CL_Log::log("debug", "Focus In");
				#endif
				focus = true;
				sig_got_focus();
				break;
			case FocusOut:
				#ifdef DEBUG
					CL_Log::log("debug", "Focus Out");
				#endif
				focus = false;
				sig_lost_focus();
				break;
			case KeyRelease:
				if( XEventsQueued( disp, QueuedAfterReading ) )
				{
					XPeekEvent( disp, &next_event );
					if( next_event.type == KeyPress &&
						next_event.xkey.window == event.xkey.window &&
						next_event.xkey.keycode == event.xkey.keycode &&
						next_event.xkey.time == event.xkey.time )
					{
						// Do not report anything for this event
						break;
					}
				}
			case KeyPress:
			case KeymapNotify:
			case ButtonPress:
			case ButtonRelease:
			case MotionNotify:
				sig_xevent(event);
				break;
			case PropertyNotify:
				#ifdef DEBUG
				// This looks to be mostly useless
				// I'm getting a lot of WM messages that aren't really useful.
				// --MSR, April 5, 2003
				/*
					if(event.xproperty.state == PropertyNewValue)
					{
						CL_Log::log("debug", "%1 has a new value", XGetAtomName(disp, event.xproperty.atom));
					}
				*/
				#endif
				break;
				
			#ifdef DEBUG
			case EnterNotify:
			case LeaveNotify:
				CL_Log::log("debug", "The mouse has left or entered the window");
				break;
//			default:
				//			CL_Log::log("debug", "Unhandled event type: %1", event.type);
			#endif
		default:
			sig_unknown_xevent(event);
			break;
		}
	}
}

// Local Variables: ***
// mode: clanlib ***
// End: ***

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
**    Hans de Goede
**    (if your name is missing here, please add it)
*/

#include <cstdio>
#include <errno.h>
#include <cstring>

#include "display_window_opengl.h"
#ifdef HAVE_LINUX_JOYSTICK_H
#include "input_device_linuxjoystick.h"
#endif
#include "input_device_linuxusbmouse.h"
#ifdef HAVE_LINUX_INPUT_H
#include "input_device_linuxevent.h"
#endif
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
	context(0),
	window(None), wm_window(None), fs_window(None), width(0), height(0),
	old_x(-1), old_y(-1)
{
	gc = CL_GraphicContext(new CL_GraphicContext_OpenGL(this));
}

CL_DisplayWindow_OpenGL::~CL_DisplayWindow_OpenGL()
{
	destroy_window();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_DisplayWindow_OpenGL::get_width() const
{
	return width;
}

int CL_DisplayWindow_OpenGL::get_height() const
{
	return height;
}

bool CL_DisplayWindow_OpenGL::is_fullscreen() const
{
	return fs_window != None;
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

	width  = desc.get_size().width;
	height = desc.get_size().height;
	allow_resize = desc.get_allow_resize();

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
		vi = glXChooseVisual(disp, DefaultScreen(disp), gl_attribs_single);
		printf("Requested visual not supported by your OpenGL implementation. Falling back on singlebuffered Visual!\n");
	}

	// create a GLX context
	context = glXCreateContext(disp, vi, share_context, True);

	if( share_context == NULL )
		share_context = context;

	// create a color map
	cmap = XCreateColormap( disp, RootWindow(disp, vi->screen), vi->visual, AllocNone);

	XSetWindowAttributes attributes;

	attributes.colormap = cmap;
	attributes.background_pixel = XBlackPixel(disp, vi->screen);
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
		PropertyChangeMask |
		KeymapStateMask |
		FocusChangeMask;

	wm_window = XCreateWindow(disp, RootWindow(disp, vi->screen),
		0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
		CWBackPixel | CWBorderPixel | CWColormap | CWOverrideRedirect | CWEventMask, &attributes);
	
	window = XCreateWindow(disp, wm_window,
		0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
		CWBackPixel | CWBorderPixel | CWColormap | CWOverrideRedirect | CWEventMask, &attributes);
	
	// Map the real / drawing window it won't appear untill the parent does
	XMapWindow(disp, window);

	// set title of window:
	set_title(desc.get_title());
	
	// setup size hints:
	XSizeHints size_hints;
	memset(&size_hints, 0, sizeof(XSizeHints));
	size_hints.width       = width;
	size_hints.height      = height;
	size_hints.base_width  = width;
	size_hints.base_height = height;
	size_hints.min_width   = width;
	size_hints.min_height  = height;
	size_hints.max_width   = width;
	size_hints.max_height  = height;
	size_hints.flags       = PSize|PBaseSize;
	if (!allow_resize) size_hints.flags |= PMinSize | PMaxSize;
	XSetWMNormalHints(disp, wm_window, &size_hints);

	// handle wm_delete_events if in windowed mode:
	Atom wm_delete = XInternAtom(disp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(disp, wm_window, &wm_delete, 1);

	// make the managed window visible (and thus the real one too):
	XMapRaised(disp, wm_window);

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
		set_fullscreen(width, height, desc.get_bpp(), desc.get_refresh_rate());
}

void
CL_DisplayWindow_OpenGL::setup_event()
{
#ifdef HAVE_LINUX_INPUT_H
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
#else
	CL_Log::log("debug", "linux input support not compiled-in");
#endif
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
#ifdef HAVE_LINUX_JOYSTICK_H
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
#else
	CL_Log::log("debug", "linux joystick support not compiled-in");
#endif
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
	if(fs_window != None)
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

	if (window != None)
	{
		XDestroyWindow(disp, window);
		window = None;
	}
	if (wm_window != None)
	{
		XDestroyWindow(disp, wm_window);
		wm_window = None;
	}
	
	if (disp_ref_count > 0)
	{
		disp_ref_count--;
		if (disp_ref_count == 0)
		{
			XCloseDisplay(disp);
			disp = NULL;
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

/* qsort comparison function for sorting the modes */
static int cmpmodes(const void *va, const void *vb)
{
	const XF86VidModeModeInfo *a = *(const XF86VidModeModeInfo **)va;
	const XF86VidModeModeInfo *b = *(const XF86VidModeModeInfo **)vb;
	if ( a->hdisplay == b->hdisplay )
		return b->vdisplay - a->vdisplay;  
	else
		return b->hdisplay - a->hdisplay;
}

void CL_DisplayWindow_OpenGL::set_resolution(void)
{
	int i, dotclock, mode_count, fs_width, fs_height;
	XF86VidModeModeInfo **modes;
	XF86VidModeModeLine current_mode;

	current_mode.privsize = 0;
	XF86VidModeGetModeLine(disp, vi->screen, &dotclock, &current_mode);
	if (!XF86VidModeGetAllModeLines(disp, 0, &mode_count, &modes))
		mode_count = 0;

	/* Search for an exact matching video mode.  */
	for (i = 0; i < mode_count; i++) {
		if ((modes[i]->hdisplay == width) && 
		    (modes[i]->vdisplay == height))
			break;
	}

	/* Search for a non exact match (smallest bigger res). */
	if (i == mode_count) {
		int best_width = 0, best_height = 0;
		qsort(modes, mode_count, sizeof(void *), cmpmodes);
		for (i = mode_count-1; i > 0; i--) {
			if ( ! best_width ) {
				if ( (modes[i]->hdisplay >= width) &&
				     (modes[i]->vdisplay >= height) ) {
					best_width = modes[i]->hdisplay;
					best_height = modes[i]->vdisplay;
				}
			} else {
				if ( (modes[i]->hdisplay != best_width) ||
				     (modes[i]->vdisplay != best_height) ) {
					i++;
					break;
				}
			}
		}
	}

	/* Switch video mode */
	if (i >= 0)
	{
		if ((modes[i]->hdisplay != current_mode.hdisplay) ||
		    (modes[i]->vdisplay != current_mode.vdisplay))
		{
			if (old_x == -1 && old_y == -1)
			{
				XF86VidModeModeLine *l =
					(XF86VidModeModeLine *)((char *)
						&old_mode +
						sizeof(old_mode.dotclock));
				*l = current_mode;
				old_mode.dotclock = dotclock;
				XF86VidModeGetViewPort(disp, vi->screen,
					&old_x, &old_y);
			}
			XF86VidModeSwitchToMode(disp, vi->screen,
				modes[i]);
		}
		fs_width  = modes[i]->hdisplay;
		fs_height = modes[i]->vdisplay;
	}
	else
	{
		fs_width  = DisplayWidth(disp, vi->screen);
		fs_height = DisplayHeight(disp, vi->screen);
	}

	/* center window */
	XMoveWindow(disp, window, (fs_width - width) / 2,
		(fs_height - height) / 2);

	/* Lock mode switching.  */
	XF86VidModeLockModeSwitch(disp, vi->screen, True);
	       
	/* Set viewport. */
	XF86VidModeSetViewPort(disp, vi->screen, 0, 0);

	/* clean up */
	if (current_mode.privsize)
	  XFree(current_mode.c_private);

	if (mode_count)
	{
	  for (i = 0; i < mode_count; i++)
	    if (modes[i]->privsize)
	      XFree(modes[i]->c_private);
	  XFree(modes);
	}
}

void CL_DisplayWindow_OpenGL::set_fullscreen(int width, int height, int bpp, int refresh_rate)
{
	if(!width)
		width = this->width;
	if(!height)
		height = this->height;
		
	set_size(width, height);
	
	if(fs_window != None)
	{
		set_resolution();
		return;
	}

	/* Create the fullscreen window */
	XSetWindowAttributes attributes;
	XEvent event;

	attributes.override_redirect = True;
	attributes.background_pixel = XBlackPixel(disp, vi->screen);
	attributes.border_pixel = XBlackPixel(disp, vi->screen);
	attributes.event_mask = StructureNotifyMask;
	attributes.colormap = cmap;

	fs_window = XCreateWindow(disp, RootWindow(disp, vi->screen),
	                             0, 0,
	                             DisplayWidth(disp, vi->screen),
	                             DisplayHeight(disp, vi->screen), 0,
	                             vi->depth, InputOutput,
	                             vi->visual, CWOverrideRedirect |
	                             CWBackPixel | CWColormap | CWBorderPixel |
	                             CWEventMask, &attributes);

	/* Map the fullscreen window */
	XMapRaised(disp, fs_window);
	/* wait until we are mapped. (shamelessly borrowed from SDL) */
	do {
	  XMaskEvent(disp, StructureNotifyMask, &event); 
	} while ( (event.type != MapNotify) || 
	          (event.xmap.event != fs_window) );
	/* Make sure we got to the top of the window stack */
	XRaiseWindow(disp, fs_window);
	
	/* Reparent the real window */
	XReparentWindow(disp, window, fs_window, 0, 0);

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

	set_resolution();
}

void CL_DisplayWindow_OpenGL::set_windowed()
{
	if(fs_window == None)
		return;

	// Only restore mode if we ever switched mode.
	if (old_x != -1 || old_y != -1)
	{
		XF86VidModeSwitchToMode(disp, vi->screen, &old_mode);
		XF86VidModeSetViewPort(disp, vi->screen, old_x, old_y);
		old_x = old_y = -1;
	}

	XUngrabPointer(disp, CurrentTime);

	/* Reparent the real window! */
	XReparentWindow(disp, window, wm_window, 0, 0);
	XUnmapWindow(disp, fs_window);
	XDestroyWindow(disp, fs_window);
	fs_window = None;
}

void CL_DisplayWindow_OpenGL::set_title(const std::string &title)
{
	XSetStandardProperties(disp, wm_window, title.c_str(), title.c_str(), None, NULL, 0, NULL);
}

void CL_DisplayWindow_OpenGL::set_position(const CL_Rect &pos)
{
	set_position(pos.left, pos.top);
	set_size(pos.get_width(), pos.get_height());
}

void CL_DisplayWindow_OpenGL::set_position(int x, int y)
{
	XMoveWindow(disp, wm_window, x, y);
}

void CL_DisplayWindow_OpenGL::set_size(int width, int height)
{
	if((width == this->width) && (height == this->height))
		return;

	// in order to resize a non resizable window we must change the hints
	// as the window manager won't allow the resize otherwise.
	if (!allow_resize)
	{
		XSizeHints size_hints;
		memset(&size_hints, 0, sizeof(XSizeHints));
		size_hints.width       = width;
		size_hints.height      = height;
		size_hints.base_width  = width;
		size_hints.base_height = height;
		size_hints.min_width   = width;
		size_hints.min_height  = height;
		size_hints.max_width   = width;
		size_hints.max_height  = height;
		size_hints.flags       = PSize|PBaseSize|PMinSize|PMaxSize;
		XSetWMNormalHints(disp, wm_window, &size_hints);
	}
	XResizeWindow(disp, wm_window, width, height);
	XResizeWindow(disp, window, width, height);
	this->width  = width;
	this->height = height;
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
				if (event.xconfigure.window == wm_window &&
				    (event.xconfigure.width  != width ||
				     event.xconfigure.height != height))
				{
					width  = event.xconfigure.width;
					height = event.xconfigure.height;
					XResizeWindow(disp, window, width,
						 height);
					sig_resize(event.xconfigure.width, event.xconfigure.height);
				}
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
						(next_event.xkey.time == event.xkey.time ||
						next_event.xkey.time-1 == event.xkey.time) )
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

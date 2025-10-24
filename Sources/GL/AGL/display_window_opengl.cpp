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
**     misrepresented as being theset_ original software.
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

#include <iostream>
#include <cstdio>
#include <errno.h>

#include "display_window_opengl.h"
#include "Display/display_precomp.h"
#include "Display/display_window_generic.h"
#include "API/Core/System/keep_alive.h"
#include "../graphic_context_opengl.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"
#include "GL/gl_state_checker.h"
#include "GL/AGL/input_device_mackeyboard.h"
#include "GL/AGL/input_device_macmouse.h"
#include "API/Core/System/clanstring.h"
#include "API/GL/opengl_window_description.h"
#include "API/GL/opengl_state.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/keys.h"
#include "API/Display/input_device.h"
#include "API/Display/input_event.h"
#include "../opengl_window_description_generic.h"
#include <unistd.h>

AGLContext CL_DisplayWindow_OpenGL::share_context = 0;

int CL_DisplayWindow_OpenGL::disp_ref_count = 0;
OSStatus AEHandler(EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon);

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_DisplayWindow_OpenGL::CL_DisplayWindow_OpenGL() :
	left_ctrl_down(false), left_alt_down(false), left_shift_down(false),
	right_ctrl_down(false), right_alt_down(false), right_shift_down(false),
	fullscreen(false), fullscreen_width(0), fullscreen_height(0),
	saved_position(0, 0, 0, 0),
	context(0), win_context(0),
	fs_context(0), window_ref(0)
{
	static bool first_call = true;
	if (first_call)
	{
		// Enable key-up:
		SetEventMask(everyEvent);
		target_ref = GetEventDispatcherTarget();
		first_call = false;
		
		//install special handler to process Apple Event messages - SAR
		
		const EventTypeSpec kEvents[] = {{kEventClassAppleEvent, kEventAppleEvent}};
		InstallApplicationEventHandler(NewEventHandlerUPP(AEHandler),
 	    GetEventTypeCount(kEvents), kEvents, 0, NULL);
	}

	gc = CL_GraphicContext(new CL_GraphicContext_OpenGL(this));
}

CL_DisplayWindow_OpenGL::~CL_DisplayWindow_OpenGL()
{
//	if(fullscreen)
//		set_windowed();
	destroy_window();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_DisplayWindow_OpenGL::get_width() const
{
	Rect rect;
	GetWindowPortBounds(window_ref, &rect);
	return rect.right-rect.left;
}

int CL_DisplayWindow_OpenGL::get_height() const
{
	Rect rect;
	GetWindowPortBounds(window_ref, &rect);
	return rect.bottom-rect.top;
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
	OSStatus result;

	const CL_OpenGLWindowDescription_Generic *gl_desc = 0;
	gl_desc = dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(desc.impl.get());

	fullscreen_width = desc.get_size().width;
	fullscreen_height = desc.get_size().height;

	disp_ref_count++;

	GLint gl_attribs_single[] =
	{
		AGL_RGBA,
		AGL_RED_SIZE, 4,
		AGL_GREEN_SIZE, 4,
		AGL_BLUE_SIZE, 4,
		AGL_DEPTH_SIZE, 16,
		AGL_NONE
	};

	GLint gl_attribs[32];
	int i = 0;
	
	if( gl_desc )
	{
		if( gl_desc->rgba ) gl_attribs[i++] = AGL_RGBA;
		if( gl_desc->doublebuffer ) gl_attribs[i++] = AGL_DOUBLEBUFFER;
		//if( gl_desc->stereo ) gl_attribs[i++] = AGL_STEREO;
		gl_attribs[i++] = AGL_BUFFER_SIZE;
		gl_attribs[i++] = gl_desc->buffer_size;
		gl_attribs[i++] = AGL_LEVEL;
		gl_attribs[i++] = gl_desc->level;
		gl_attribs[i++] = AGL_AUX_BUFFERS;
		gl_attribs[i++] = gl_desc->aux_buffers;
		gl_attribs[i++] = AGL_RED_SIZE; 
		gl_attribs[i++] = gl_desc->red_size;
		gl_attribs[i++] = AGL_GREEN_SIZE;
		gl_attribs[i++] = gl_desc->green_size;
		gl_attribs[i++] = AGL_BLUE_SIZE;
		gl_attribs[i++] = gl_desc->blue_size;
		gl_attribs[i++] = AGL_DEPTH_SIZE;
		gl_attribs[i++] = gl_desc->depth_size;
		gl_attribs[i++] = AGL_STENCIL_SIZE;
		gl_attribs[i++] = gl_desc->stencil_size;
		gl_attribs[i++] = AGL_ACCUM_RED_SIZE;
		gl_attribs[i++] = gl_desc->accum_red_size;
		gl_attribs[i++] = AGL_ACCUM_GREEN_SIZE;
		gl_attribs[i++] = gl_desc->accum_green_size;
		gl_attribs[i++] = AGL_ACCUM_BLUE_SIZE;
		gl_attribs[i++] = gl_desc->accum_blue_size;
		gl_attribs[i++] = AGL_ACCUM_ALPHA_SIZE;
		gl_attribs[i++] = gl_desc->accum_alpha_size;
		gl_attribs[i++] = AGL_ACCUM_RED_SIZE;
		gl_attribs[i++] = gl_desc->accum_red_size;
//		gl_attribs[i++] = AGL_FULLSCREEN;
	}
	else
	{
		gl_attribs[i++] = AGL_RGBA;
		gl_attribs[i++] = AGL_NO_RECOVERY;
		gl_attribs[i++] = AGL_DOUBLEBUFFER;
		gl_attribs[i++] = AGL_DEPTH_SIZE;
		gl_attribs[i++] = 16;
	} 
/*	
	else
	{
		gl_attribs[i++] = AGL_RGBA;
		gl_attribs[i++] = AGL_DOUBLEBUFFER;
		gl_attribs[i++] = AGL_RED_SIZE;
		gl_attribs[i++] = 4;
		gl_attribs[i++] = AGL_GREEN_SIZE;
		gl_attribs[i++] = 4;
		gl_attribs[i++] = AGL_BLUE_SIZE;
		gl_attribs[i++] = 4;
		gl_attribs[i++] = AGL_DEPTH_SIZE;
		gl_attribs[i++] = 16;
//		gl_attribs[i++] = AGL_FULLSCREEN;
	}
*/

	gl_attribs[i] = AGL_NONE;

	CGDirectDisplayID display = CGMainDisplayID();
	GDHandle gdhDisplay, *pgdhDisplay;
	int numDisplay;

	pgdhDisplay = &gdhDisplay;
	
	if (noErr == DMGetGDeviceByDisplayID ((DisplayIDType)display, pgdhDisplay, false)) 
		numDisplay = 1;	
	else 
	{
		pgdhDisplay = 0;
		numDisplay = 0;
	}
		
	AGLPixelFormat pixelformat;	
	pixelformat = aglChoosePixelFormat(pgdhDisplay, numDisplay, gl_attribs);
	win_context = aglCreateContext(pixelformat, share_context);
	if (!share_context) share_context = win_context;
	aglDestroyPixelFormat(pixelformat);
	gl_attribs[i++] = AGL_FULLSCREEN;
	gl_attribs[i] = AGL_NONE;
	pixelformat = aglChoosePixelFormat(pgdhDisplay, numDisplay, gl_attribs);
	fs_context = aglCreateContext(pixelformat, win_context);
	aglDestroyPixelFormat(pixelformat);

	if (!(win_context && fs_context)) 
	{
		printf("Requested visual not supported by your OpenGL implementation. Falling back on singlebuffered Visual!\n");
		pixelformat = aglChoosePixelFormat(0, 0, gl_attribs_single);
		win_context = aglCreateContext(pixelformat, share_context);
		aglDestroyPixelFormat(pixelformat);
		fs_context = 0;
	}

	WindowAttributes style = kWindowCloseBoxAttribute | kWindowStandardHandlerAttribute;
	if (desc.get_allow_resize()) style |= kWindowResizableAttribute | kWindowFullZoomAttribute;
	Rect window_rect;
	SetRect(&window_rect, 50, 50, 50+desc.get_size().width, 50+desc.get_size().height);
	result = CreateNewWindow(kDocumentWindowClass, style, &window_rect, &window_ref);
	if (result != noErr)
		printf("Could not create window, due to error %d\n", (int)result);

	// set title of window:
	set_title(desc.get_title());

	// Set standard arrow cursor:
	InitCursor(); // do we need to do this? -- iMBN, 13. may 2004
	
	// Create input devices for window:
	keyboard = CL_InputDevice(new CL_InputDevice_MacKeyboard(this));
	mouse	= CL_InputDevice(new CL_InputDevice_MacMouse(this));

	get_ic()->clear();
	get_ic()->add_keyboard(keyboard);
	get_ic()->add_mouse(mouse);

//	buffer_front = CL_PixelBuffer(new CL_PixelBuffer_OpenGL_Frame(CL_FRONT, gc));
//	buffer_back = CL_PixelBuffer(new CL_PixelBuffer_OpenGL_Frame(CL_BACK, gc));

	if (!aglSetDrawable(win_context, GetWindowPort(window_ref)))
	{
		printf("Unable to set drawable");
	}
	
	if (desc.is_fullscreen())
		set_fullscreen(desc.get_size().width, desc.get_size().height, desc.get_bpp(), desc.get_refresh_rate());
	else {
		fullscreen = true;  // not really, but need to fool set_windowed
		set_windowed();
	}

	ShowWindow(window_ref);
}

void CL_DisplayWindow_OpenGL::install_event_handler(EventTargetRef targ_ref, int remove_only) 
{
	static EventHandlerUPP window_handler_proc = 0;
	if (window_handler_proc == 0)
		window_handler_proc = NewEventHandlerUPP(&CL_DisplayWindow_OpenGL::on_window_event);
	
	static EventHandlerRef current_handler = 0;
	if (current_handler != 0)
		RemoveEventHandler(current_handler);
	
	if (remove_only) return;
	
	EventTypeSpec events[20];
	events[0].eventClass = kEventClassKeyboard;
	events[0].eventKind = kEventRawKeyDown;
	events[1].eventClass = kEventClassKeyboard;
	events[1].eventKind = kEventRawKeyRepeat;
	events[2].eventClass = kEventClassKeyboard;
	events[2].eventKind = kEventRawKeyUp;
	events[3].eventClass = kEventClassMouse;
	events[3].eventKind = kEventMouseDown;
	events[4].eventClass = kEventClassMouse;
	events[4].eventKind = kEventMouseUp;
	events[5].eventClass = kEventClassMouse;
	events[5].eventKind = kEventMouseMoved;
	events[6].eventClass = kEventClassMouse;
	events[6].eventKind = kEventMouseWheelMoved;
	events[7].eventClass = kEventClassWindow;
	events[7].eventKind = kEventWindowCollapsing;
	events[8].eventClass = kEventClassWindow;
	events[8].eventKind = kEventWindowDrawContent;
	events[9].eventClass = kEventClassWindow;
	events[9].eventKind = kEventWindowShown;
	events[10].eventClass = kEventClassWindow;
	events[10].eventKind = kEventWindowClose;
	events[11].eventClass = kEventClassWindow;
	events[11].eventKind = kEventWindowActivated;
	events[12].eventClass = kEventClassWindow;
	events[12].eventKind = kEventWindowDeactivated;
	events[13].eventClass = kEventClassWindow;
	events[13].eventKind = kEventWindowBoundsChanged;
	events[14].eventClass = kEventClassWindow;
	events[14].eventKind = kEventWindowZoomed;
	events[15].eventClass = kEventClassWindow;
	events[15].eventKind = kEventWindowFocusAcquired;
	events[16].eventClass = kEventClassWindow;
	events[16].eventKind = kEventWindowFocusRelinquish;
	events[17].eventClass = kEventClassMouse;
	events[17].eventKind = kEventMouseDragged;
	events[18].eventClass = kEventClassKeyboard;
	events[18].eventKind = kEventRawKeyModifiersChanged;

	InstallEventHandler(targ_ref, window_handler_proc, 19, events, this, &current_handler);
}

void CL_DisplayWindow_OpenGL::destroy_window()
{
	install_event_handler(0, 1);

	if (context)
		aglSetCurrentContext(0);

	if (fs_context)
	{
		aglDestroyContext(fs_context);
		fs_context = 0;
	}

	if (win_context)
	{
		aglDestroyContext(win_context);
		win_context = 0;
	}

	keyboard = CL_InputDevice();
	mouse = CL_InputDevice();
	get_ic()->clear();

	if (window_ref)
		ReleaseWindow(window_ref);
	window_ref = 0;
}

void CL_DisplayWindow_OpenGL::flip(int interval)
{
	CL_GLStateChecker::from_gc(get_gc())->flush();

	if (interval != -1)
	{
		GLint param = interval;
		aglSetInteger(context, AGL_SWAP_INTERVAL, &param);
	}

	aglSwapBuffers(context);
}

void CL_DisplayWindow_OpenGL::set_fullscreen(int width, int height, int bpp, int refresh_rate)
{
	if(fullscreen)
	{
		//we just want to update the size, this will do that for us..
		set_size(width, height);
		set_windowed();
	}

	CGCaptureAllDisplays();
	
	aglSetCurrentContext(fs_context);
	aglSetFullScreen(fs_context, width, height, refresh_rate, 0);

	context = fs_context;
	install_event_handler(GetApplicationEventTarget());
	
	fullscreen = true;
	focus = true;

	//After toggling to the fullscreen context, nothing is going draw unless
	//we reapply the GL states/properties -mrfun 6-2-2006
	CL_GLStateChecker::from_gc(get_gc())->reinitialize_asap();

}

void CL_DisplayWindow_OpenGL::set_windowed()
{
	//if(!fullscreen)
	//	return;

	if (context == fs_context)
	{
	    aglSetDrawable(fs_context, 0);
	}
	
	CGReleaseAllDisplays();
	aglSetCurrentContext(win_context);
	aglUpdateContext(win_context);
	install_event_handler(GetWindowEventTarget(window_ref));

	fullscreen = false;
	context = win_context;
	focus = IsWindowHilited(window_ref);
	
	//After toggling to the fullscreen context, nothing is going draw unless
	//we reapply the GL states/properties -mrfun 6-2-2006
	CL_GLStateChecker::from_gc(get_gc())->reinitialize_asap();
	
}

void CL_DisplayWindow_OpenGL::set_title(const std::string &title)
{
	CFStringRef s = CFStringCreateWithCString(0, title.c_str(), kCFStringEncodingISOLatin1);
	SetWindowTitleWithCFString(window_ref, s);
	CFRelease(s);
}

void CL_DisplayWindow_OpenGL::set_position(const CL_Rect &pos)
{
	set_position(pos.left, pos.top);
	set_size(pos.get_width(), pos.get_height());
}

void CL_DisplayWindow_OpenGL::set_position(int x, int y)
{
	MoveWindow(window_ref, x, y, false);
}

void CL_DisplayWindow_OpenGL::set_size(int width, int height)
{
	fullscreen_width = width;
	fullscreen_height = height;
	SizeWindow(window_ref, width, height, true);
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

	clReadBuffer(CL_BACK);

	if(clGetError() == CL_NO_ERROR)		// Check if we got more than 1 buffer.
	{
		float raster_pos[4];

		clGetFloatv(CL_CURRENT_RASTER_POSITION, raster_pos);

		clDrawBuffer(CL_FRONT);
		clRasterPos3f(rect.left, rect.bottom, raster_pos[2]);

		clCopyPixels(rect.top, rect.left, rect.get_width(), rect.get_height(), CL_COLOR);

		// Return to drawing to the back buffer
		clDrawBuffer(CL_BACK);
	}
}

void CL_DisplayWindow_OpenGL::show_system_cursor()
{
	ShowCursor();
}

void CL_DisplayWindow_OpenGL::hide_system_cursor()
{
	// Should be replaced by a function only hiding it over this window, if possible.
	HideCursor();
}

void CL_DisplayWindow_OpenGL::cursor_changed()
{
}

void CL_DisplayWindow_OpenGL::keep_alive()
{
	EventRef theEvent;

	while(ReceiveNextEvent(0, NULL,kEventDurationNoWait,true,&theEvent)== noErr)
	{
		SendEventToEventTarget (theEvent, target_ref);
		ReleaseEvent(theEvent);
	}
}

int CL_DisplayWindow_OpenGL::clkey_to_keycode(int clkey)
{
	switch (clkey)
	{
	case CL_KEY_A:				return 0;
	case CL_KEY_S:				return 1;
	case CL_KEY_D:				return 2;
	case CL_KEY_F:				return 3;
	case CL_KEY_H:				return 4;
	case CL_KEY_G:				return 5;
	case CL_KEY_Z:				return 6;
	case CL_KEY_X:				return 7;
	case CL_KEY_C:				return 8;
	case CL_KEY_V:				return 9;
	case CL_KEY_B:				return 11;
	case CL_KEY_Q:				return 12;
	case CL_KEY_W:				return 13;
	case CL_KEY_E:				return 14;
	case CL_KEY_R:				return 15;
	case CL_KEY_Y:				return 16;
	case CL_KEY_T:				return 17;
	case CL_KEY_1:				return 18;
	case CL_KEY_2:				return 19;
	case CL_KEY_3:				return 20;
	case CL_KEY_4:				return 21;
	case CL_KEY_6:				return 22;
	case CL_KEY_5:				return 23;
	case CL_KEY_EQUALS:			return 24;
	case CL_KEY_9:				return 25;
	case CL_KEY_7:				return 26;
	case CL_KEY_SUBTRACT:		return 27;
	case CL_KEY_8:				return 28;
	case CL_KEY_0:				return 29;
	case CL_KEY_RIGHT_BRACKET:	return 30;
	case CL_KEY_O:				return 31;
	case CL_KEY_U:				return 32;
	case CL_KEY_LEFT_BRACKET:	return 33;
	case CL_KEY_I:				return 34;
	case CL_KEY_P:				return 35;
	case CL_KEY_ENTER:			return 36;
	case CL_KEY_L:				return 37;
	case CL_KEY_J:				return 38;
	case CL_KEY_QUOTE:			return 39;
	case CL_KEY_K:				return 40;
	case CL_KEY_SEMICOLON:		return 41;
	case CL_KEY_COMMA:			return 43;
	case CL_KEY_DIVIDE:			return 44;
	case CL_KEY_N:				return 45;
	case CL_KEY_M:				return 46;
	case CL_KEY_PERIOD:			return 47;
	case CL_KEY_TAB:			return 48;
	case CL_KEY_SPACE:			return 49;
	case CL_KEY_TILDE:			return 50;
	case CL_KEY_BACKSPACE:		return 51;
	case CL_KEY_ESCAPE:			return 53;
	case CL_KEY_SHIFT:			return 56;
	case CL_KEY_MENU:			return 58;
	case CL_KEY_CONTROL:		return 59;
	case CL_KEY_NUMPAD_DECIMAL:	return 65;
	case CL_KEY_NUMPAD_MULTIPLY:return 67;
	case CL_KEY_NUMPAD_ADD:		return 69;
	case CL_KEY_NUMPAD_DIVIDE:	return 75;
	case CL_KEY_NUMPAD_ENTER:	return 76;
	case CL_KEY_NUMPAD_SUBTRACT:return 78;
	case CL_KEY_NUMPAD_EQUALS:	return 81;
	case CL_KEY_NUMPAD_0:		return 82;
	case CL_KEY_NUMPAD_1:		return 83;
	case CL_KEY_NUMPAD_2:		return 84;
	case CL_KEY_NUMPAD_3:		return 85;
	case CL_KEY_NUMPAD_4:		return 86;
	case CL_KEY_NUMPAD_5:		return 87;
	case CL_KEY_NUMPAD_6:		return 88;
	case CL_KEY_NUMPAD_7:		return 89;
	case CL_KEY_NUMPAD_8:		return 91;
	case CL_KEY_NUMPAD_9:		return 92;
	case CL_KEY_F5:				return 96;
	case CL_KEY_F6:				return 97;
	case CL_KEY_F7:				return 98;
	case CL_KEY_F3:				return 99;
	case CL_KEY_F8:				return 100;
	case CL_KEY_F9:				return 101;
	case CL_KEY_F13:			return 105;
	case CL_KEY_F14:			return 107;
	case CL_KEY_F10:			return 109;
	case CL_KEY_F12:			return 111;
	case CL_KEY_F15:			return 113;
	case CL_KEY_HELP:			return 114;
	case CL_KEY_HOME:			return 115;
	case CL_KEY_PAGE_UP:		return 116;
	case CL_KEY_DELETE:			return 117;
	case CL_KEY_F4:				return 118;
	case CL_KEY_END:			return 119;
	case CL_KEY_F2:				return 120;
	case CL_KEY_PAGE_DOWN:		return 121;
	case CL_KEY_F1:				return 122;
	case CL_KEY_LEFT:			return 123;
	case CL_KEY_RIGHT:			return 124;
	case CL_KEY_DOWN:			return 125;
	case CL_KEY_UP:				return 126;
	default:					return clkey - 0x10000;
	}
	return 0;
}

std::set<int> CL_DisplayWindow_OpenGL::modifiercode_to_clkeys(int m)
{
	std::set<int> r;
	if (m & shiftKey)		r.insert(CL_KEY_SHIFT);
	if (m & optionKey)		r.insert(CL_KEY_OPTION);
	if (m & controlKey)		r.insert(CL_KEY_CONTROL);
	if (m & rightShiftKey)	r.insert(CL_KEY_RSHIFT);
	if (m & rightControlKey)r.insert(CL_KEY_RCONTROL);
	if (m & cmdKey)			r.insert(CL_KEY_COMMAND);
	if (m & alphaLock)		r.insert(CL_KEY_CAPSLOCK);
	//if (m & rightOptionKey)	r.insert([right option key]);
	return r;
}

int CL_DisplayWindow_OpenGL::keycode_to_clkey(int keycode)
{
	switch (keycode)
	{
	case 0:			return CL_KEY_A;
	case 1:			return CL_KEY_S;
	case 2:			return CL_KEY_D;
	case 3:			return CL_KEY_F;
	case 4:			return CL_KEY_H;
	case 5:			return CL_KEY_G;
	case 6:			return CL_KEY_Z;
	case 7:			return CL_KEY_X;
	case 8:			return CL_KEY_C;
	case 9:			return CL_KEY_V;
	case 11:		return CL_KEY_B;
	case 12:		return CL_KEY_Q;
	case 13:		return CL_KEY_W;
	case 14:		return CL_KEY_E;
	case 15:		return CL_KEY_R;
	case 17:		return CL_KEY_T;
	case 16:		return CL_KEY_Y;
	case 18:		return CL_KEY_1;
	case 19:		return CL_KEY_2;
	case 20:		return CL_KEY_3;
	case 21:		return CL_KEY_4;
	case 22:		return CL_KEY_6;
	case 23:		return CL_KEY_5;
	case 24:		return CL_KEY_EQUALS;
	case 25:		return CL_KEY_9;
	case 26:		return CL_KEY_7;
	case 27:		return CL_KEY_SUBTRACT;
	case 28:		return CL_KEY_8;
	case 29:		return CL_KEY_0;
	case 30:		return CL_KEY_RIGHT_BRACKET;
	case 31:		return CL_KEY_O;
	case 32:		return CL_KEY_U;
	case 33:		return CL_KEY_LEFT_BRACKET;
	case 34:		return CL_KEY_I;
	case 35:		return CL_KEY_P;
	case 36:		return CL_KEY_ENTER;
	case 37:		return CL_KEY_L;
	case 38:		return CL_KEY_J;
	case 39:		return CL_KEY_QUOTE;
	case 40:		return CL_KEY_K;
	case 41:		return CL_KEY_SEMICOLON;
	case 43:		return CL_KEY_COMMA;
	case 44:		return CL_KEY_DIVIDE;
	case 45:		return CL_KEY_N;
	case 46:		return CL_KEY_M;
	case 47:		return CL_KEY_PERIOD;
	case 48:		return CL_KEY_TAB;
	case 49:		return CL_KEY_SPACE;
	case 50:		return CL_KEY_TILDE;
	case 51:		return CL_KEY_BACKSPACE;
	case 53:		return CL_KEY_ESCAPE;
	case 65:		return CL_KEY_NUMPAD_DECIMAL;
	case 67:		return CL_KEY_NUMPAD_MULTIPLY;
	case 69:		return CL_KEY_NUMPAD_ADD;
	case 75:		return CL_KEY_NUMPAD_DIVIDE;
	case 76:		return CL_KEY_NUMPAD_ENTER;
	case 78:		return CL_KEY_NUMPAD_SUBTRACT;
	case 82:		return CL_KEY_NUMPAD_0;
	case 83:		return CL_KEY_NUMPAD_1;
	case 84:		return CL_KEY_NUMPAD_2;
	case 85:		return CL_KEY_NUMPAD_3;
	case 86:		return CL_KEY_NUMPAD_4;
	case 87:		return CL_KEY_NUMPAD_5;
	case 88:		return CL_KEY_NUMPAD_6;
	case 89:		return CL_KEY_NUMPAD_7;
	case 91:		return CL_KEY_NUMPAD_8;
	case 92:		return CL_KEY_NUMPAD_9;
	case 96:		return CL_KEY_F5;
	case 97:		return CL_KEY_F6;
	case 98:		return CL_KEY_F7;
	case 99:		return CL_KEY_F3;
	case 100:		return CL_KEY_F8;
	case 101:		return CL_KEY_F9;
	case 105:		return CL_KEY_F13;
	case 107:		return CL_KEY_F14;
	case 109:		return CL_KEY_F10;
	case 111:		return CL_KEY_F12;
	case 113:		return CL_KEY_F15;
	case 114:		return CL_KEY_HELP;
	case 115:		return CL_KEY_HOME;
	case 116:		return CL_KEY_PAGE_UP;
	case 117: 		return CL_KEY_DELETE;
	case 118:		return CL_KEY_F4;
	case 119:		return CL_KEY_END;
	case 120:		return CL_KEY_F2;
	case 121:		return CL_KEY_PAGE_DOWN;
	case 122:		return CL_KEY_F1;
	case 123:		return CL_KEY_LEFT;
	case 124:		return CL_KEY_RIGHT;
	case 125:		return CL_KEY_DOWN;
	case 126:		return CL_KEY_UP;
	
	default:		return keycode + 0x10000;
	}
}

OSStatus CL_DisplayWindow_OpenGL::on_window_event(EventHandlerCallRef call_ref, EventRef event_ref, void *user_data)
{
	CL_DisplayWindow_OpenGL *self = (CL_DisplayWindow_OpenGL *) user_data;

	OSStatus result = CallNextEventHandler(call_ref, event_ref);

	UInt32 event_class = GetEventClass(event_ref);
	UInt32 event_kind = GetEventKind(event_ref);
	
	if (event_class == kEventClassKeyboard)
	{
		Point mouse_pos;
		UInt32 key_modifiers = 0;
		UInt32 click_count = 0;
		UInt32 key_code = 0;
                
		// Todo:
		// KeyboardLayoutRef layout;
		// KLGetCurrentKeyboardLayout(&layout);
		// KLGetKeyboardLayoutProperty(layout, kKLuchrData, &data);
		// call UCKeyTranslate to get unicode string for keys being pressed.

		GetMouse(&mouse_pos);
		GetEventParameter(event_ref, kEventParamKeyModifiers, typeUInt32, 0, sizeof(UInt32), 0, &key_modifiers);
		GetEventParameter(event_ref, kEventParamKeyCode, typeUInt32, 0, sizeof(UInt32), 0, &key_code);
		GetEventParameter(event_ref, kEventParamClickCount, typeUInt32, 0, sizeof(UInt32), 0, &click_count);

		if (!self->fullscreen) {
			Rect wbounds;
			GetWindowBounds(self->window_ref, kWindowContentRgn, &wbounds);
			mouse_pos.h -= wbounds.left;
			mouse_pos.v -= wbounds.top;
		}

		static unsigned long state = 0;
		static Ptr keymap = nil;
		Ptr new_keymap;

		// Get the current keyboard map resource
		new_keymap = (Ptr)GetScriptManagerVariable(smKCHRCache);
		if (new_keymap != keymap)
		{
			keymap = new_keymap;
			state = 0;
		}

		CL_InputEvent event;
		event.str = KeyTranslate(keymap, key_code|key_modifiers, &state) & 0xffff;
		event.device = self->keyboard;
		event.mouse_pos = CL_Point(mouse_pos.h, mouse_pos.v);
		event.repeat_count = click_count;

		self->mouse_pos = event.mouse_pos; // if kEventParamMouseLocation is not valid here, reverse this.
		
		switch (event_kind)
		{
		case kEventRawKeyModifiersChanged:
			{
				std::set<int> current_keys = modifiercode_to_clkeys(key_modifiers);
				for (std::set<int>::iterator i=current_keys.begin(); i!=current_keys.end(); i++)
				{
					if (!self->prev_modifier_keys.count(*i))
					{
						event.id = *i;
						event.type = CL_InputEvent::pressed;
						self->keyboard.sig_key_down().call(event);
					}
				}
				for (std::set<int>::iterator i=self->prev_modifier_keys.begin(); i!=self->prev_modifier_keys.end(); i++)
				{
					if (!current_keys.count(*i))
					{
						event.id = *i;
						event.type = CL_InputEvent::released;
						self->keyboard.sig_key_up().call(event);
					}
				}
				self->prev_modifier_keys = current_keys;
				return noErr;
			}
		
		case kEventRawKeyDown:
			// first tests for cmd-q(12) or cmd-w(13) to close the window if needed
			if ((key_modifiers & cmdKey) && (key_code==12 || key_code==13))
			{
				EventRef close_event;
				CreateEvent(NULL, kEventClassWindow, kEventWindowClose, 0, kEventAttributeNone, &close_event);
				EventTargetRef target = GetWindowEventTarget(self->window_ref);
				SendEventToEventTarget(close_event, target);
				return noErr;
			}
			else
			{
				event.id = keycode_to_clkey(key_code);
				event.type = CL_InputEvent::pressed;
				self->keyboard.sig_key_down().call(event);
				return noErr;
			}
			
		case kEventRawKeyRepeat:
			event.id = keycode_to_clkey(key_code);
			event.type = CL_InputEvent::pressed;
			self->keyboard.sig_key_down().call(event);
			return noErr;
			
		case kEventRawKeyUp:
			event.id = keycode_to_clkey(key_code);
			event.type = CL_InputEvent::released;
			self->keyboard.sig_key_up().call(event);
			return noErr;
		}
	}
	else if (event_class == kEventClassMouse)
	{
		Point mouse_pos;
		EventMouseButton mouse_button;
		UInt32 key_modifiers = 0;

		GetEventParameter(event_ref, kEventParamMouseLocation, typeQDPoint, 0, sizeof(Point), 0, &mouse_pos);
		GetEventParameter(event_ref, kEventParamMouseButton, typeMouseButton, 0, sizeof(EventMouseButton), 0, &mouse_button);
		GetEventParameter(event_ref, kEventParamKeyModifiers, typeUInt32, 0, sizeof(UInt32), 0, &key_modifiers);
				
		if (!self->fullscreen) {
			Rect wbounds;
			GetWindowBounds(self->window_ref, kWindowContentRgn, &wbounds);
			mouse_pos.h -= wbounds.left;
			mouse_pos.v -= wbounds.top;
		}

		CL_InputEvent event;
		event.id = mouse_button-1;
		event.device = self->mouse;
		event.mouse_pos = CL_Point(mouse_pos.h, mouse_pos.v);

		// Simulate second mouse button.
		if ((key_modifiers & controlKey) && (event.id == CL_MOUSE_LEFT))
			event.id = CL_MOUSE_RIGHT;

		self->mouse_pos = event.mouse_pos;

		switch (event_kind)
		{
		case kEventMouseDown:
			self->mouse_states[event.id] = true;
			
			int repeat_count;

			GetEventParameter(event_ref, kEventParamClickCount, typeUInt32, 0, sizeof(UInt32), 0, &repeat_count);
			event.type = CL_InputEvent::pressed;
			if (repeat_count == 2)
			{
				//we need to issue a normal mouse click too, for compatibility with older clanlibs we
				//ask it to pretend it's non-repeater, in case they are using the same callback for double clicks too
				self->mouse.sig_key_down().call(event);

				event.repeat_count = repeat_count;
				self->mouse.sig_key_dblclk().call(event);
			} else
			{
				event.repeat_count = repeat_count;
				self->mouse.sig_key_down().call(event);
			}
			return noErr;
	
		
		case kEventMouseUp:
			self->mouse_states[event.id] = false;
			event.type = CL_InputEvent::released;
			self->mouse.sig_key_up().call(event);
			return noErr;
			
		case kEventMouseMoved:
		case kEventMouseDragged:
			self->mouse.sig_pointer_move().call(event);
			return noErr;

		case kEventMouseWheelMoved:
			//Note, this is not properly handling large quick wheel movements, but the implementation on other
			//platforms don't seem to either so I will do it their way for consistancy - mrfun
			
			EventMouseWheelAxis axis;
			GetEventParameter(event_ref, kEventParamMouseWheelAxis, typeMouseWheelAxis, NULL, sizeof(axis), NULL, &axis);

			if (axis == kEventMouseWheelAxisY)
			{
			
			long wheel_delta = 0;
			GetEventParameter(event_ref, kEventParamMouseWheelDelta, typeLongInteger, 0, sizeof(long), 0, &wheel_delta);
			
			//Ok, let me explain this bIgnoreThisOne thing.  We are being passed two identical wheel events for every wheel movement -
			//This behavior is also in the apple example app GLCarbonAGLWindow and I can't figure out a way to get it to send only one
			//event or a difference in the events. (like a press and release tag differentiating them)

			//So the hack solution is to simply ignore every other mouse wheel event.  This way, programs behave the same on every
			//platform instead of wheel zooming being twice as fast on macs.

			//If anybody has a better solution or finds a problem here please feel free to dig in. -mrfun
			
			static bool bIgnoreThisOne = true;
			bIgnoreThisOne = !bIgnoreThisOne;
			
			if (bIgnoreThisOne)
				if (wheel_delta)
				{
					if (wheel_delta > 0)
					{
						event.id = CL_MOUSE_WHEEL_UP;
					} else
					{
						event.id = CL_MOUSE_WHEEL_DOWN;
					}
			
					//sending both the down and up event, like the linux version
					event.type = CL_InputEvent::pressed;
					self->mouse.sig_key_down().call(event);
					event.type = CL_InputEvent::released;
					self->mouse.sig_key_up().call(event);
				} else
				{
				//handle the X axis someday?
				}
			}
			
			return noErr;
		}
	}
	else if (event_class == kEventClassWindow)
	{
		switch (event_kind)
		{
		case kEventWindowCollapsing:
			break;

		case kEventWindowDrawContent:
			{
				if (!self->fullscreen) 
				{
					Rect rectPort;
					GetWindowPortBounds(self->window_ref, &rectPort);
					self->sig_paint(
						CL_Rect(
							rectPort.left,
							rectPort.top,
							rectPort.right,
							rectPort.bottom));
				}
			}
			break;
			
		case kEventWindowShown: // called on initial show (not on un-minimize)
			break;
			
		case kEventWindowClose:
			self->sig_window_close();
			return noErr;

		case kEventWindowActivated:
		case kEventWindowFocusAcquired:
			self->focus = true;
			self->sig_got_focus();
			return noErr;
			
		case kEventWindowDeactivated:
		case kEventWindowFocusRelinquish:
			self->focus = false;
			self->sig_lost_focus();
			return noErr;
			
		case kEventWindowBoundsChanged:
			{
				if (!self->fullscreen) {
					Rect rectPort;
					GetWindowPortBounds (self->window_ref, &rectPort);
					self->set_size(rectPort.right-rectPort.left, rectPort.bottom-rectPort.top);
				}
			}
			return noErr;
			
		case kEventWindowZoomed:
			// when maximized, but kEventWindowBoundsChanged is also called
			break;
		}
	} 	

	return result;
}

OSStatus AEHandler(EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon)
{
	
    Boolean     release = false;
    EventRecord eventRecord;
    OSErr       ignoreErrForThisSample;
	
    // Events of type kEventAppleEvent must be removed from the queue
    //  before being passed to AEProcessAppleEvent.
	
    if (IsEventInQueue(GetMainEventQueue(), inEvent))
    {
        // RemoveEventFromQueue will release the event, which will
        //  destroy it if we don't retain it first.
		
        RetainEvent(inEvent);
        release = true;
        RemoveEventFromQueue(GetMainEventQueue(), inEvent);
    }
	
    // Convert the event ref to the type AEProcessAppleEvent expects.
    ConvertEventRefToEventRecord(inEvent, &eventRecord);
    ignoreErrForThisSample = AEProcessAppleEvent(&eventRecord);
	
    if (release)
		
        ReleaseEvent(inEvent);
    // This Carbon event has been handled, even if no AppleEvent handlers
    //  were installed for the Apple event.
    return noErr;
	
}

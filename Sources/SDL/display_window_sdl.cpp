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

#include "display_window_sdl.h"
#include "Display/display_precomp.h"
#include "Display/display_window_generic.h"
#include "API/Core/System/keep_alive.h"
#include "graphic_context_sdl.h"
#include "pixel_buffer_sdl_frame.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"
#include "input_device_sdlkeyboard.h"
#include "input_device_sdlmouse.h"
#include "input_device_sdljoystick.h"
#include "API/Core/System/clanstring.h"
#include "API/Display/input_event.h"
#include <map>

#include <SDL/SDL_syswm.h>

static int refcount_init_joystick = 0;

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_DisplayWindow_SDL::CL_DisplayWindow_SDL()
{
	refcount_init_joystick++;
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
	{
		if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
		{
			CL_Log::log("debug", "Could not init joystick subsystem: %1", SDL_GetError());
		}
		SDL_JoystickEventState(SDL_ENABLE);
	}
}

CL_DisplayWindow_SDL::~CL_DisplayWindow_SDL()
{
	destroy_window();

	refcount_init_joystick--;
	if (refcount_init_joystick == 0 && SDL_WasInit(SDL_INIT_JOYSTICK) != 0)
	{
		SDL_JoystickEventState(SDL_DISABLE);
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK); 
	}
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_DisplayWindow_SDL::get_width() const
{
	return screen->w;
}

int CL_DisplayWindow_SDL::get_height() const
{
	return screen->h;
}

bool CL_DisplayWindow_SDL::is_fullscreen() const
{
	return fullscreen;
}

bool CL_DisplayWindow_SDL::has_focus() const
{
	Uint8 focus_state = SDL_GetAppState();
	if(focus_state&SDL_APPMOUSEFOCUS)
		return true;
	else if(focus_state&SDL_APPINPUTFOCUS)
		return true;
	return false;
}

CL_PixelBuffer CL_DisplayWindow_SDL::get_buffer(int i) const
{
	return gc.get_pixeldata();
}

int CL_DisplayWindow_SDL::get_buffer_count() const
{
	if(screen->flags&SDL_DOUBLEBUF)
		return 2;
	return 1;
}

CL_GraphicContext *CL_DisplayWindow_SDL::get_gc()
{
	return &gc;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_DisplayWindow_SDL::create_window(const CL_DisplayWindowDescription &desc)
{
	Uint32 mode = SDL_SWSURFACE;

	if(desc.get_flipping_buffers() >= 2)
		mode |= SDL_DOUBLEBUF;

	if(desc.is_fullscreen()){
		mode |= SDL_FULLSCREEN;
		fullscreen = true;
	}else{
		fullscreen = false;
	}

	if(desc.get_allow_resize())
		mode |= SDL_RESIZABLE;
	
	// Lawouach : I guess on error here we're supposed to throw a CL_Error ?
	if((screen = SDL_SetVideoMode(desc.get_size().width,desc.get_size().height,desc.get_bpp(),mode)) == NULL)
		throw CL_Error(SDL_GetError());

	if(!desc.get_title().empty())
		set_title(desc.get_title());

	gc = CL_GraphicContext(new CL_GraphicContext_SDL(this));

	// Create input devices for window:
	keyboard = CL_InputDevice(new CL_InputDevice_SDLKeyboard(this));
	mouse = CL_InputDevice(new CL_InputDevice_SDLMouse(this));
	
	get_ic()->clear();
	get_ic()->add_keyboard(keyboard);
	get_ic()->add_mouse(mouse);
	
	int num_joysticks = SDL_NumJoysticks();
	for (int index_joystick = 0; index_joystick < num_joysticks; index_joystick++)
	{
		try
		{
			CL_InputDevice joystick(new CL_InputDevice_SDLJoystick(this, index_joystick));
			get_ic()->add_joystick(joystick);
		}
		catch (CL_Error error)
		{
			CL_Log::log("debug", "Failed to setup joystick #%1: %2", index_joystick+1, error.message);
		}
	}
}

void CL_DisplayWindow_SDL::destroy_window()
{
	keyboard = CL_InputDevice();
	mouse = CL_InputDevice();
	//joystick = CL_InputDevice();
	get_ic()->clear();
	set_windowed();
	// Lawouach : SDL tells us not to free ourselves the screen surface as it is done by SDL_Quit()
	// SDL_FreeSurface(screen);
}

void CL_DisplayWindow_SDL::flip(int interval)
{
	// Lawouach : if we asked for software memory surface, this is not
	// the correct method to call, but that one will work anyway as it
	// will call itself SDL_UpdateRect which deals with software
	// surfaces. SDL_Flip deals with hardware memory ones. We use it
	// anyway as later we could add the hardware memory choice to the
	// SDL display window creation
	SDL_Flip(screen);
}

void CL_DisplayWindow_SDL::set_fullscreen(int width, int height, int bpp, int refresh_rate)
{
	if(fullscreen == false)
		if(SDL_WM_ToggleFullScreen(screen) == 1)
			fullscreen = true;
}

void CL_DisplayWindow_SDL::set_windowed()
{
	// Lawouach : if the display window is already in fullscreen mode it will get back to windowed mode
	if(fullscreen == true)
		if(SDL_WM_ToggleFullScreen(screen) == 1)
			fullscreen = false;
}

void CL_DisplayWindow_SDL::set_title(const std::string &title)
{
	SDL_WM_SetCaption(title.c_str(),0);
}

void CL_DisplayWindow_SDL::set_position(const CL_Rect &pos)
{
	CL_Log::log("debug", "set_position not supported by SDL!");
}

void CL_DisplayWindow_SDL::set_position(int x, int y)
{
	CL_Log::log("debug", "set_position not supported by SDL!");
}

void CL_DisplayWindow_SDL::set_size(int width, int height)
{
	// todo: set sdl window size
	sig_resize(width, height);
}

void CL_DisplayWindow_SDL::set_buffer_count(int flipping_buffers)
{
	CL_Log::log("debug", "set_buffer_count not supported by SDL!");
}

void CL_DisplayWindow_SDL::update(const CL_Rect &rect)
{
	SDL_UpdateRect(screen,rect.left,rect.top,rect.get_width(),rect.get_height());
}

void CL_DisplayWindow_SDL::show_system_cursor()
{
	SDL_ShowCursor(SDL_ENABLE);
}

void CL_DisplayWindow_SDL::hide_system_cursor()
{
	SDL_ShowCursor(SDL_DISABLE);
}

void CL_DisplayWindow_SDL::cursor_changed()
{
}

void CL_DisplayWindow_SDL::keep_alive()
{
	// Dispatch all available events from system:
	while (SDL_PollEvent(&event) > 0)
	{
		switch(event.type)
		{
		case SDL_VIDEORESIZE:
#ifdef DEBUG
			CL_Log::log("debug", "ConfigureNotify Event received");
#endif
			set_size(event.resize.w,event.resize.h);
			break;
		case SDL_QUIT:
#ifdef DEBUG
			CL_Log::log("debug", "Received ClientMessage, sending close signal");
#endif
			sig_window_close();
			break;
		case SDL_VIDEOEXPOSE:
			// SDL does support this message but doesn't actually get what area needs to be redrawn.
#ifdef DEBUG
			CL_Log::log("debug", "Expose Event received");
#endif
			break;
		case SDL_ACTIVEEVENT:
			if (event.active.state == SDL_APPINPUTFOCUS)
			{
#ifdef DEBUG
				CL_Log::log("debug", "Focus In");
#endif
				sig_got_focus();
			}
			else if (event.active.state == SDL_APPMOUSEFOCUS)
			{
				CL_Log::log("debug", "The mouse has left or entered the window");
			}
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			sig_sdl_keyboard_event(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
			sig_sdl_mouse_event(event);
			break;
		case SDL_JOYAXISMOTION:
		case SDL_JOYBALLMOTION:
		case SDL_JOYHATMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			sig_sdl_joystick_event(event);
			break;
		default:
			CL_Log::log("debug", "Unhandled event type: %1", event.type);
		}
	}
}

#ifdef WIN32
HWND CL_DisplayWindow_SDL::get_wnd()
{
	SDL_SysWMinfo info;
	SDL_GetWMInfo(&info);
	return info.window;
}
#else
#ifndef __APPLE__
Display *CL_DisplayWindow_SDL::get_wnd()
{
	SDL_SysWMinfo info;
	SDL_GetWMInfo(&info);
	return info.info.x11.display;
}
#endif
#endif

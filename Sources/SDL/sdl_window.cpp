/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
**    Mark Page
*/

#include "SDL/precomp.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/Core/Text/logger.h"
#include "API/Core/System/databuffer.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/display_window_message.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/keys.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Image/pixel_buffer.h"

#include "input_device_provider_sdlkeyboard.h"
#include "input_device_provider_sdlmouse.h"
#include "input_device_provider_sdljoystick.h"
#include "display_message_queue_sdl.h"

#include "sdl_window.h"

#include <SDL/SDL.h>

static int cl_sdl_init_refcount = 0;
static int cl_sdl_init_joystick_refcount = 0;

CL_SDLWindow::CL_SDLWindow(CL_DisplayMessageQueue_SDL *message_queue)
:site(0), screen(0), fullscreen(false),
  keyboard(0), mouse(0), joystick(0), message_queue(message_queue),minimum_size(0,0), maximum_size(0xffff, 0xffff), layered(false)
{
	// Create input devices for window:
	keyboard = new CL_InputDeviceProvider_SDLKeyboard(this);
	mouse = new CL_InputDeviceProvider_SDLMouse(this);

	message_queue->add_client(this);
}

CL_SDLWindow::~CL_SDLWindow()
{
	message_queue->remove_client(this);
	destroy();
	delete keyboard;
	delete mouse;
}

CL_Rect CL_SDLWindow::get_geometry() const
{
	if (!screen)
		return CL_Rect();

	return CL_Rect(0, 0, CL_Size(screen->w, screen->h));
}

CL_Rect CL_SDLWindow::get_viewport() const
{
	if (!screen)
		return CL_Rect();

	return CL_Rect(0, 0, CL_Size(screen->w, screen->h));
}

bool CL_SDLWindow::has_focus() const
{
	Uint8 focus_state = SDL_GetAppState();
	if(focus_state&SDL_APPMOUSEFOCUS)
		return true;
	else if(focus_state&SDL_APPINPUTFOCUS)
		return true;
	return false;
}

bool CL_SDLWindow::is_minimized() const
{
	return false;
}

bool CL_SDLWindow::is_maximized() const
{
	return false;
}

bool CL_SDLWindow::is_visible() const
{
	return true;
}

void CL_SDLWindow::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &description)
{
	destroy();
	site = new_site;

	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
	{
		if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
		{
			//CL_Log::log("debug", "Could not init joystick subsystem: %1", SDL_GetError());
		}
		SDL_JoystickEventState(SDL_ENABLE);
	}
	cl_sdl_init_joystick_refcount++;

	if (cl_sdl_init_refcount == 0)
	{
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
			throw CL_Exception(SDL_GetError());
	
		SDL_EnableUNICODE(1);

		// Enable key repeat
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
	}else
	{
		throw CL_Exception(cl_text("SDL does not handle multiple windows, 1.3 may do in the future"));
	}

	cl_sdl_init_refcount++;

	Uint32 mode = SDL_HWSURFACE;

	if(description.get_flipping_buffers() >= 2)
		mode |= SDL_DOUBLEBUF;

	if(description.is_fullscreen()){
		mode |= SDL_FULLSCREEN;
		fullscreen = true;
	}else{
		fullscreen = false;
	}

	if(description.get_allow_resize())
		mode |= SDL_RESIZABLE;
	
	// Lawouach : I guess on error here we're supposed to throw a CL_Error ?
	if((screen = SDL_SetVideoMode(description.get_size().width,description.get_size().height,description.get_bpp(),mode)) == NULL)
		throw CL_Exception(SDL_GetError());

	if(!description.get_title().empty())
		set_title(description.get_title());

	ic.clear();
	ic.add_keyboard(keyboard);
	ic.add_mouse(mouse);
	
	int num_joysticks = SDL_NumJoysticks();
	for (int index_joystick = 0; index_joystick < num_joysticks; index_joystick++)
	{
		try
		{
			joystick = new CL_InputDeviceProvider_SDLJoystick(this);
			ic.add_joystick(joystick);
			break;	//TODO: Support multiple joysticks
		}
		catch (CL_Exception error)
		{
			//CL_Log::log("debug", "Failed to setup joystick #%1: %2", index_joystick+1, error.message);
		}
	}
}

void CL_SDLWindow::destroy()
{
	if (screen)
	{
		set_windowed();

		ic.clear();
		//delete joystick;

		SDL_FreeSurface(screen);
		screen = NULL;

		cl_sdl_init_joystick_refcount--;
		if (cl_sdl_init_joystick_refcount == 0 && SDL_WasInit(SDL_INIT_JOYSTICK) != 0)
		{
			SDL_JoystickEventState(SDL_DISABLE);
			SDL_QuitSubSystem(SDL_INIT_JOYSTICK); 
		}

		cl_sdl_init_refcount--;
		if (cl_sdl_init_refcount == 0)	
		{
			SDL_Quit();
		}
	}
}

void CL_SDLWindow::set_windowed()
{
	if(fullscreen == true)
		if(SDL_WM_ToggleFullScreen(screen) == 1)
			fullscreen = false;
}

CL_Point CL_SDLWindow::client_to_screen(const CL_Point &client)
{
	return client;
}

CL_Point CL_SDLWindow::screen_to_client(const CL_Point &screen)
{
	return screen;
}

void CL_SDLWindow::show_system_cursor()
{
}

void CL_SDLWindow::set_cursor(CL_CursorProvider_SDL *cursor)
{
}

void CL_SDLWindow::set_cursor(CL_StandardCursor type)
{
}

void CL_SDLWindow::hide_system_cursor()
{
}

void CL_SDLWindow::set_title(const CL_StringRef &new_title)
{
	SDL_WM_SetCaption(CL_StringHelp::text_to_local8(new_title).c_str(), CL_StringHelp::text_to_local8(new_title).c_str());
}

void CL_SDLWindow::set_position(const CL_Rect &pos, bool client_area)
{
}

void CL_SDLWindow::set_size(int width, int height, bool client_area)
{
}

void CL_SDLWindow::set_enabled(bool enable)
{
}

void CL_SDLWindow::minimize()
{
	SDL_WM_IconifyWindow();
}

void CL_SDLWindow::restore()
{
}

void CL_SDLWindow::maximize()
{
}

void CL_SDLWindow::show(bool activate)
{
}

void CL_SDLWindow::hide()
{
}

void CL_SDLWindow::bring_to_front()
{
}

void CL_SDLWindow::capture_mouse(bool capture)
{
}

void CL_SDLWindow::set_clipboard_text(const CL_StringRef &text)
{
}

CL_String CL_SDLWindow::get_clipboard_text() const
{
	return CL_String();
}

bool CL_SDLWindow::is_clipboard_text_available() const
{
	return false;
}

void CL_SDLWindow::set_clipboard_image(const CL_PixelBuffer &image)
{
}

void CL_SDLWindow::request_repaint( const CL_Rect &cl_rect )
{
}

void CL_SDLWindow::set_minimum_size( int width, int height, bool client_area)
{
	if (client_area)
		throw CL_Exception(cl_text("Congratulations! You just got assigned the task of adding support for client area in CL_SDLWindow::set_minimum_size(...)."));

	this->minimum_size = CL_Size(width,height);
}

void CL_SDLWindow::set_maximum_size( int width, int height, bool client_area)
{
	if (client_area)
		throw CL_Exception(cl_text("Congratulations! You just got assigned the task of adding support for client area in CL_SDLWindow::set_maximum_size(...)."));

	this->maximum_size = CL_Size(width,height);
}

// This is called for each window by CL_DisplayMessageQueue_SDL to check for messages
bool CL_SDLWindow::has_messages()
{
	bool message_flag = false;

	if (SDL_PollEvent(NULL) > 0)
	{
		message_flag = true;
	}

	if (ic.poll(true))
	{
		message_flag = true;
	}

	return message_flag;
}

bool CL_SDLWindow::get_message(SDL_Event &clan_event)
{
	SDL_Event event;
	CL_Rect *rect;
	CL_DisplayWindowMessage message;
	bool clan_event_set = false;

	ic.poll(false);		// Check input devices

	// Dispatch all available events from system:
	if (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_VIDEORESIZE:
			if (!callback_on_resized.is_null())
				callback_on_resized.invoke();

			if (site)
			{
				site->sig_resize->invoke(event.resize.w, event.resize.h);
			}
			break;
		case SDL_QUIT:
			if (site) site->sig_window_close->invoke();
			break;
		case SDL_VIDEOEXPOSE:
			// SDL does support this message but doesn't actually get what area needs to be redrawn.
			break;
		case SDL_ACTIVEEVENT:
			if (event.active.state == SDL_APPINPUTFOCUS)
			{
				if (site)
					site->sig_got_focus->invoke();
			}
			else if (event.active.state == SDL_APPMOUSEFOCUS)
			{

			}
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyboard->handle_keyboard_event(event.key);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			mouse->handle_mouse_input_event(event.button);
			break;
		case SDL_MOUSEMOTION:
			mouse->handle_mouse_move_event(event.motion);
			break;
		case SDL_JOYAXISMOTION:
		case SDL_JOYBALLMOTION:
		case SDL_JOYHATMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			//sig_sdl_joystick_event(event);
			break;
		//default:

		}

		clan_event = event;
		clan_event_set = true;
	}

	return clan_event_set;
}

void CL_SDLWindow::flip()
{
	SDL_Flip(screen);
}


void CL_SDLWindow::set_large_icon(const CL_PixelBuffer &image)
{

}

void CL_SDLWindow::set_small_icon(const CL_PixelBuffer &image)
{

}



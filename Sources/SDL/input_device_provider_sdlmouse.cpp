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
**    Mark Page
*/

#include "SDL/precomp.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Core/Text/string_format.h"
#include "input_device_provider_sdlmouse.h"
#include "sdl_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_SDLMouse construction:

CL_InputDeviceProvider_SDLMouse::CL_InputDeviceProvider_SDLMouse(CL_SDLWindow *window)
: sig_provider_event(0), window(window)
{
}

CL_InputDeviceProvider_SDLMouse::~CL_InputDeviceProvider_SDLMouse()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_SDLMouse attributes:

int CL_InputDeviceProvider_SDLMouse::get_x() const
{
	int xpos;
	SDL_GetMouseState(&xpos, NULL);
	return xpos;
}

int CL_InputDeviceProvider_SDLMouse::get_y() const
{
	int ypos;
	SDL_GetMouseState(NULL, &ypos);
	return ypos;
}

bool CL_InputDeviceProvider_SDLMouse::get_keycode(int keycode) const
{
	unsigned int state = SDL_GetMouseState(NULL, NULL);

	if (state&SDL_BUTTON(keycode))
	{
		return true;
	}
	return false;
}

CL_String CL_InputDeviceProvider_SDLMouse::get_key_name(int id) const
{
	switch (id)
	{
	case 0: return cl_text("Mouse left");
	case 1: return cl_text("Mouse right");
	case 2: return cl_text("Mouse middle");
	case 3: return cl_text("Mouse wheel up");
	case 4: return cl_text("Mouse wheel down");
	}

	return cl_format(cl_text("Mouse button %1"), id);
}

float CL_InputDeviceProvider_SDLMouse::get_axis(int index) const
{
	return 0.0f;
}

CL_String CL_InputDeviceProvider_SDLMouse::get_name() const
{
	return cl_text("System Mouse");
}

CL_String CL_InputDeviceProvider_SDLMouse::get_device_name() const
{
	return cl_text("System Mouse");
}

int CL_InputDeviceProvider_SDLMouse::get_axis_count() const
{
	return 0;
}

int CL_InputDeviceProvider_SDLMouse::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_SDLMouse operations:

void CL_InputDeviceProvider_SDLMouse::set_position(int x, int y)
{
}

void CL_InputDeviceProvider_SDLMouse::handle_mouse_move_event(SDL_MouseMotionEvent &event)
{
	// Prepare event to be emitted:
	CL_InputEvent key;
	key.type = CL_InputEvent::pointer_moved;
	key.mouse_pos = CL_Point(event.x, event.y);

	SDLMod mod_state = SDL_GetModState();

	key.alt =  (mod_state & KMOD_ALT) != 0;
	key.shift =  (mod_state & KMOD_SHIFT) != 0;
	key.ctrl =  (mod_state & KMOD_CTRL) != 0;

	// Fire off signal
	sig_provider_event->invoke(key);

}

void CL_InputDeviceProvider_SDLMouse::handle_mouse_input_event(SDL_MouseButtonEvent &event)
{
	int id;

	switch(event.button)
	{
		case 1: id = CL_MOUSE_LEFT; break;
		case 3: id = CL_MOUSE_RIGHT; break;
		case 2: id = CL_MOUSE_MIDDLE; break;
		case 4: id = CL_MOUSE_WHEEL_UP; break;
		case 5: id = CL_MOUSE_WHEEL_DOWN; break;
		case 6: id = CL_MOUSE_XBUTTON1; break;
		case 7: id = CL_MOUSE_XBUTTON2; break;
		default: return;	// Unknown press
	}

	// Prepare event to be emitted:
	CL_InputEvent key;
	key.mouse_pos = CL_Point( event.x, event.y );
	key.id = id;
	if (event.state == SDL_PRESSED )
		key.type = CL_InputEvent::pressed;
	else	key.type = CL_InputEvent::released;

	// Emit message:
	sig_provider_event->invoke(key);

}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_SDLMouse implementation:

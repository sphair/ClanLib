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

#include "API/Display/input_device.h"
#include "API/Display/input_event.h"
#include "API/Core/System/clanstring.h"
#include "input_device_sdlmouse.h"

#include "display_window_sdl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLMouse construction:

CL_InputDevice_SDLMouse::CL_InputDevice_SDLMouse(CL_DisplayWindow_SDL *owner) :
	owner(owner)
{
	type = CL_InputDevice::mouse;
	key_states[0] = false;
	key_states[1] = false;
	key_states[2] = false;
	key_states[3] = false;
	key_states[4] = false;
	slot_sdlevent = owner->sig_sdl_mouse_event.connect(this, &CL_InputDevice_SDLMouse::handle_mouse_event);
}

CL_InputDevice_SDLMouse::~CL_InputDevice_SDLMouse()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLMouse attributes:

int CL_InputDevice_SDLMouse::get_x() const
{
	return mouse_pos.x;
}

int CL_InputDevice_SDLMouse::get_y() const
{
	return mouse_pos.y;
}

bool CL_InputDevice_SDLMouse::get_keycode(int keycode) const
{
	if((keycode >=0) && (keycode <=5))
		return key_states[keycode];
	return false;
}

float CL_InputDevice_SDLMouse::get_axis(int index) const
{
	return 0.0f;
}

std::string CL_InputDevice_SDLMouse::get_name() const
{
	return "System Mouse";
}

int CL_InputDevice_SDLMouse::get_axis_count() const
{
	return 0;
}

int CL_InputDevice_SDLMouse::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLMouse operations:

std::string CL_InputDevice_SDLMouse::get_key_name(int id) const
{
	switch (id)
	{
	case 0: return "Mouse left";
	case 1: return "Mouse right";
	case 2: return "Mouse middle";
	case 3: return "Mouse wheel up";
	case 4: return "Mouse wheel down";
	}

	return CL_String::format("Mouse button %1", id);
}

void CL_InputDevice_SDLMouse::handle_mouse_event(SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		CL_InputEvent cl_event;
		cl_event.device = owner->mouse;
		cl_event.mouse_pos = CL_Point(event.button.x, event.button.y);
		if (event.button.button == SDL_BUTTON_LEFT) cl_event.id = 0;
		if (event.button.button == SDL_BUTTON_RIGHT) cl_event.id = 1;
		if (event.button.button == SDL_BUTTON_MIDDLE) cl_event.id = 2;

		if (event.key.type == SDL_MOUSEBUTTONDOWN) 
		{
			cl_event.type = CL_InputEvent::pressed;
			key_states[cl_event.id] = true;
		}
		else if (event.key.type == SDL_MOUSEBUTTONUP) 
		{
			cl_event.type = CL_InputEvent::released;
			key_states[cl_event.id] = false;
		}
		// todo: keep local track of repeat counts since SDL doesnt do that for us.
		
		if (cl_event.type == CL_InputEvent::pressed)
			owner->mouse.sig_key_down().call(cl_event);
		if (cl_event.type == CL_InputEvent::released)
			owner->mouse.sig_key_up().call(cl_event);
        }
	else if (event.type == SDL_MOUSEMOTION)
	{
		CL_InputEvent cl_event;
		cl_event.type = CL_InputEvent::moved;
		cl_event.device = owner->mouse;
		mouse_pos = CL_Point(event.motion.x, event.motion.y);
		cl_event.mouse_pos = mouse_pos;
		owner->mouse.sig_move().call(cl_event);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLMouse implementation:

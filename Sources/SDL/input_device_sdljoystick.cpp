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
#include "input_device_sdljoystick.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"

#include "display_window_sdl.h"

#include <SDL/SDL.h>

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLJoystick construction:

CL_InputDevice_SDLJoystick::CL_InputDevice_SDLJoystick(CL_DisplayWindow_SDL *owner, int joystick_num) :
	owner(owner), joystick(0), my_joystick(-1), last_x(0), last_y(0)
{
	type = CL_InputDevice::joystick;

	joystick = SDL_JoystickOpen(joystick_num);
	if (joystick == 0)
	{
		throw CL_Error(SDL_GetError());
	}
	else
	{
		my_joystick = joystick_num;
		slot_sdlevent = owner->sig_sdl_joystick_event.connect(this, &CL_InputDevice_SDLJoystick::handle_joystick_event);
	}
}

CL_InputDevice_SDLJoystick::~CL_InputDevice_SDLJoystick()
{	
	if (joystick != 0)
	{
		SDL_JoystickClose(joystick);
		joystick = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLJoystick attributes:

int CL_InputDevice_SDLJoystick::get_x() const
{
	return 0;
}

int CL_InputDevice_SDLJoystick::get_y() const
{
	return 0;
}

bool CL_InputDevice_SDLJoystick::get_keycode(int keycode) const
{
	return (SDL_JoystickGetButton(joystick,keycode) == 1);
}

float CL_InputDevice_SDLJoystick::get_axis(int index) const
{
	switch (index)
	{
	case 0:
		return last_x / 32768.0f;

	case 1:
		return last_y / 32768.0f;
	}

	return 0.0f;
}

std::string CL_InputDevice_SDLJoystick::get_name() const
{
	return "Joystick";
}

int CL_InputDevice_SDLJoystick::get_axis_count() const
{
	return 2;
}

int CL_InputDevice_SDLJoystick::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLJoystick operations:

std::string CL_InputDevice_SDLJoystick::get_key_name(int id) const
{
	return CL_String::format("Joystick button %1", id);
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLJoystick implementation:

void CL_InputDevice_SDLJoystick::handle_joystick_axis_move(SDL_Event& event)
{
	CL_InputEvent cl_event;
	cl_event.device = CL_InputDevice(this);
	cl_event.type = CL_InputEvent::axis_moved;
	cl_event.id = event.jaxis.axis;
	cl_event.axis_pos = event.jaxis.value / 32768.0f;
	if(event.jaxis.axis == 0) 
	{
		last_x = event.jaxis.value;
	}
	else if(event.jaxis.axis == 1) 
	{
		last_y = event.jaxis.value;
	}
	
	cl_event.device.sig_axis_move().call(cl_event);
}

void CL_InputDevice_SDLJoystick::handle_joystick_hat_move(SDL_Event&)
{
	// todo : Implement hat move
}

void CL_InputDevice_SDLJoystick::handle_joystick_ball_move(SDL_Event&)
{
	// todo : Implement ball move
}

void CL_InputDevice_SDLJoystick::handle_joystick_button(SDL_Event& event)
{
	CL_InputEvent cl_event;
	cl_event.device = CL_InputDevice(this);
	if(event.jbutton.state == SDL_PRESSED) cl_event.type = CL_InputEvent::pressed;
	if(event.jbutton.state == SDL_RELEASED) cl_event.type = CL_InputEvent::released;
	cl_event.id = event.jbutton.button;

	if (cl_event.type == CL_InputEvent::pressed)
		cl_event.device.sig_key_down().call(cl_event);
	if (cl_event.type == CL_InputEvent::released)
		cl_event.device.sig_key_up().call(cl_event);
}

void CL_InputDevice_SDLJoystick::handle_joystick_event(SDL_Event& event)
{
	switch(event.type)
	{
	case SDL_JOYAXISMOTION:
		if(event.jaxis.which == my_joystick) handle_joystick_axis_move(event);
		break;

	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
		if(event.jbutton.which == my_joystick) handle_joystick_button(event);
		break;

	case SDL_JOYBALLMOTION:
		if(event.jball.which == my_joystick) handle_joystick_ball_move(event);
		break;

	case SDL_JOYHATMOTION:
		if(event.jhat.which == my_joystick) handle_joystick_hat_move(event);
		break;
	}	
}

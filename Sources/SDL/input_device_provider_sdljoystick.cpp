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
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/logger.h"
#include "input_device_provider_sdljoystick.h"
#include "sdl_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_SDLJoystick construction:

CL_InputDeviceProvider_SDLJoystick::CL_InputDeviceProvider_SDLJoystick(
	CL_SDLWindow *window)
: sig_provider_event(0), window(window)
{
}

CL_InputDeviceProvider_SDLJoystick::~CL_InputDeviceProvider_SDLJoystick()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_SDLJoystick attributes:

int CL_InputDeviceProvider_SDLJoystick::get_x() const
{
	return 0;
}

int CL_InputDeviceProvider_SDLJoystick::get_y() const
{
	return 0;
}

bool CL_InputDeviceProvider_SDLJoystick::get_keycode(int keycode) const
{
	if (keycode < 0 || keycode >= 128) return false;

	return 0;
}

CL_String CL_InputDeviceProvider_SDLJoystick::get_key_name(int id) const
{
	return cl_format(cl_text("Joystick button %1"), id);
}

float CL_InputDeviceProvider_SDLJoystick::get_axis(int index) const
{
	return 0.0f;
}

CL_String CL_InputDeviceProvider_SDLJoystick::get_name() const
{
	return CL_String(cl_text("Not implemented"));
}

CL_String CL_InputDeviceProvider_SDLJoystick::get_device_name() const
{
	return CL_String(cl_text("Not implemented"));
}


int CL_InputDeviceProvider_SDLJoystick::get_axis_count() const
{
	int count=0;

	return count;
}

int CL_InputDeviceProvider_SDLJoystick::get_button_count() const
{
	int count=0;

	return count;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_SDLJoystick operations:

void CL_InputDeviceProvider_SDLJoystick::set_position(int x, int y)
{
}

void CL_InputDeviceProvider_SDLJoystick::update()
{

}

bool CL_InputDeviceProvider_SDLJoystick::poll(bool peek_only)
{
	update();
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_SDLJoystick implementation:


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
**    Sean Heber
*/

#include "GL1/precomp.h"
#include "API/Display/input_event.h"
#include "API/Core/Text/string_format.h"
#include "input_device_provider_macmouse.h"
#include "gl1_window_provider_agl.h"

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_MacMouse construction:

InputDeviceProvider_MacMouse::InputDeviceProvider_MacMouse(CL_GL1WindowProvider_AGL *window)
: sig_provider_event(NULL), window(window)
{
}

InputDeviceProvider_MacMouse::~InputDeviceProvider_MacMouse()
{
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_MacMouse attributes:

int InputDeviceProvider_MacMouse::get_x() const
{
	return 0;
}

int InputDeviceProvider_MacMouse::get_y() const
{
	return 0;
}

bool InputDeviceProvider_MacMouse::get_keycode(int keycode) const
{
	return false;
}

std::string InputDeviceProvider_MacMouse::get_key_name(int id) const
{
	switch (id)
	{
	case 0: return "Mouse left";
	case 1: return "Mouse right";
	case 2: return "Mouse middle";
	case 3: return "Mouse wheel up";
	case 4: return "Mouse wheel down";
	}

	return string_format("Mouse button %1", id);
}

float InputDeviceProvider_MacMouse::get_axis(int index) const
{
	return 0.0f;
}

std::string InputDeviceProvider_MacMouse::get_name() const
{
	return "System Mouse";
}

std::string InputDeviceProvider_MacMouse::get_device_name() const
{
	return "System Mouse";
}

int InputDeviceProvider_MacMouse::get_axis_count() const
{
	return 2;
}

int InputDeviceProvider_MacMouse::get_button_count() const
{
	return 2;
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_MacMouse operations:

void InputDeviceProvider_MacMouse::set_position(int x, int y)
{
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_MacMouse implementation:

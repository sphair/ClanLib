/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    James Lammlein
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/Window/input_event.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Window/display_window.h"
#include "input_device_provider_osxmouse.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXMouse construction:

InputDeviceProvider_OSXMouse::InputDeviceProvider_OSXMouse(OpenGLWindowProvider *window)
: sig_provider_event(0), window(window)
{
	for (int i=0; i<32; i++) key_states[i] = false;
}

InputDeviceProvider_OSXMouse::~InputDeviceProvider_OSXMouse()
{
	dispose();
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXMouse attributes:

int InputDeviceProvider_OSXMouse::get_x() const
{
	throw_if_disposed();
    // TODO: Reimplement.
	return 0;
}

int InputDeviceProvider_OSXMouse::get_y() const
{
	throw_if_disposed();
    // TODO: Reimplement.
	return 0;
}

bool InputDeviceProvider_OSXMouse::get_keycode(int keycode) const
{
	throw_if_disposed();
    // TODO: Are these valid for OSX?
	if (keycode < 0 || keycode >= 32) return false;
	return key_states[keycode];
}

std::string InputDeviceProvider_OSXMouse::get_key_name(int id) const
{
	throw_if_disposed();
    // TODO: Are these valid for OSX?
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

float InputDeviceProvider_OSXMouse::get_axis(int index) const
{
	throw_if_disposed();
	return 0.0f;
}

std::string InputDeviceProvider_OSXMouse::get_name() const
{
	throw_if_disposed();
	return "System Mouse";
}

std::string InputDeviceProvider_OSXMouse::get_device_name() const
{
	throw_if_disposed();
	return "System Mouse";
}

std::vector<int> InputDeviceProvider_OSXMouse::get_axis_ids() const
{
	throw_if_disposed();
	return std::vector<int>();
}

int InputDeviceProvider_OSXMouse::get_button_count() const
{
	throw_if_disposed();
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXMouse operations:

void InputDeviceProvider_OSXMouse::set_position(int x, int y)
{
	throw_if_disposed();
	// TODO: Reimplement.
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXMouse implementation:

void InputDeviceProvider_OSXMouse::on_dispose()
{
}

}

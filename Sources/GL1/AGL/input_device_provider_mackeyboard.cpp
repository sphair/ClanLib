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
#include "input_device_provider_mackeyboard.h"
#include "gl1_window_provider_agl.h"

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_MacKeyboard construction:

InputDeviceProvider_MacKeyboard::InputDeviceProvider_MacKeyboard(CL_GL1WindowProvider_AGL *window)
: sig_provider_event(NULL), window(window)
{
}

InputDeviceProvider_MacKeyboard::~InputDeviceProvider_MacKeyboard()
{
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_MacKeyboard attributes:

bool InputDeviceProvider_MacKeyboard::get_keycode(int keycode) const
{
	// Ignore all key events when we don't have focus
	if (!window->has_focus())
		return false;

	return false;
}

std::string InputDeviceProvider_MacKeyboard::get_key_name(int virtual_key) const
{
	return std::string();
}

float InputDeviceProvider_MacKeyboard::get_axis(int index) const
{
	return 0.0f;
}

std::string InputDeviceProvider_MacKeyboard::get_name() const
{
	return "System Keyboard";
}

std::string InputDeviceProvider_MacKeyboard::get_device_name() const
{
	return "System Keyboard";
}

int InputDeviceProvider_MacKeyboard::get_axis_count() const
{
	return 0;
}

int InputDeviceProvider_MacKeyboard::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_MacKeyboard operations:

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_MacKeyboard implementation:

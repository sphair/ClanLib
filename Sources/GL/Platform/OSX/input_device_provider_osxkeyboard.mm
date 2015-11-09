/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "input_device_provider_osxkeyboard.h"

#include "API/Core/Text/string_help.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXKeyboard construction:

InputDeviceProvider_OSXKeyboard::InputDeviceProvider_OSXKeyboard(OpenGLWindowProvider *window)
    : sig_provider_event(nullptr), window(window)
{
    for (int i = 0; i < clan::keycode_count; ++i)
    {
        key_down_map[i] = false;
    }
}

InputDeviceProvider_OSXKeyboard::~InputDeviceProvider_OSXKeyboard()
{
	dispose();
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXKeyboard attributes:

bool InputDeviceProvider_OSXKeyboard::get_keycode(int keycode) const
{
    throw_if_disposed();

    bool result = false;

    if (keycode < clan::keycode_count)
    {
        result = key_down_map[keycode];
    }

    return result;
}

std::string InputDeviceProvider_OSXKeyboard::get_key_name(int virtual_key) const
{
    //
    // TODO: Reimplement.
    //

    throw_if_disposed();
    return std::string();
}

float InputDeviceProvider_OSXKeyboard::get_axis(int index) const
{
    throw_if_disposed();
    return 0.0f;
}

std::string InputDeviceProvider_OSXKeyboard::get_name() const
{
    throw_if_disposed();
    return "System Keyboard";
}

std::string InputDeviceProvider_OSXKeyboard::get_device_name() const
{
    throw_if_disposed();
    return "System Keyboard";
}

std::vector<int> InputDeviceProvider_OSXKeyboard::get_axis_ids() const
{
    throw_if_disposed();
    return std::vector<int>();
}

int InputDeviceProvider_OSXKeyboard::get_button_count() const
{
    throw_if_disposed();
    return -1;
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXKeyboard operations:

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXKeyboard implementation:

void InputDeviceProvider_OSXKeyboard::on_dispose()
{
}

void InputDeviceProvider_OSXKeyboard::on_key_event(const clan::InputCode& keycode, const clan::InputEvent::Type& type)
{
    if (keycode < clan::keycode_count)
    {
        if (type == clan::InputEvent::pressed)
        {
            key_down_map[keycode] = true;
        }
        else if (type == clan::InputEvent::released)
        {
            key_down_map[keycode] = false;
        }
    }
}

}
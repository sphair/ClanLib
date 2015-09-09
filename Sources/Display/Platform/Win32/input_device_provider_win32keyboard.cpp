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
**    Magnus Norddahl
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "input_device_provider_win32keyboard.h"
#ifdef __MINGW32__
#include "API/Display/Window/display_window.h"
#endif
#include "win32_window.h"
#include "API/Core/Text/string_help.h"

namespace clan
{
	InputDeviceProvider_Win32Keyboard::InputDeviceProvider_Win32Keyboard(Win32Window *window) : window(window)
	{
	}

	InputDeviceProvider_Win32Keyboard::~InputDeviceProvider_Win32Keyboard()
	{
		dispose();
	}

	bool InputDeviceProvider_Win32Keyboard::get_keycode(int keycode) const
	{
		throw_if_disposed();

		// Ignore all key events when we don't have focus
		if (!window->has_focus())
			return false;

		return (GetKeyState(keycode) & 0xfe) != 0;
	}

	std::string InputDeviceProvider_Win32Keyboard::get_key_name(int virtual_key) const
	{
		throw_if_disposed();

		WCHAR name[1024];
		UINT scancode = MapVirtualKey(virtual_key, 0);
		int length = GetKeyNameText(scancode << 16, name, 1024);
		return std::string(StringHelp::ucs2_to_utf8(name), length);
	}

	std::string InputDeviceProvider_Win32Keyboard::get_name() const
	{
		throw_if_disposed();
		return "System Keyboard";
	}

	std::string InputDeviceProvider_Win32Keyboard::get_device_name() const
	{
		throw_if_disposed();
		return "System Keyboard";
	}

	int InputDeviceProvider_Win32Keyboard::get_button_count() const
	{
		throw_if_disposed();
		return -1;
	}

	void InputDeviceProvider_Win32Keyboard::on_dispose()
	{
	}
}

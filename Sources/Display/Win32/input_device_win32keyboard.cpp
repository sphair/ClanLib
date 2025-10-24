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

#include <windows.h>
#include "Display/display_precomp.h"
#include "input_device_win32keyboard.h"
#include "display_window_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_Win32Keyboard construction:

CL_InputDevice_Win32Keyboard::CL_InputDevice_Win32Keyboard(CL_DisplayWindow_Win32 *window) : window(window)
{
	type = CL_InputDevice::keyboard;
}

CL_InputDevice_Win32Keyboard::~CL_InputDevice_Win32Keyboard()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_Win32Keyboard attributes:

bool CL_InputDevice_Win32Keyboard::get_keycode(int keycode) const
{
	// Ignore all key events when we don't have focus
	if (!window->has_focus())
		return false;

	return (GetKeyState(keycode) & 0xfe) != 0;
}

std::string CL_InputDevice_Win32Keyboard::get_key_name(int virtual_key) const
{
	char name[1024];
	UINT scancode = MapVirtualKey(virtual_key, 0);
	int length = GetKeyNameText(scancode << 16, name, 1024);
	return std::string(name, length);
}

float CL_InputDevice_Win32Keyboard::get_axis(int index) const
{
	return 0.0f;
}

std::string CL_InputDevice_Win32Keyboard::get_name() const
{
	return "System Keyboard";
}

int CL_InputDevice_Win32Keyboard::get_axis_count() const
{
	return 0;
}

int CL_InputDevice_Win32Keyboard::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_Win32Keyboard operations:

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_Win32Keyboard implementation:

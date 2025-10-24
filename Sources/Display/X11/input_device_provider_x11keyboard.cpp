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

#include "GL/precomp.h"
#include "input_device_provider_x11keyboard.h"
#include "x11_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Keyboard construction:

CL_InputDeviceProvider_X11Keyboard::CL_InputDeviceProvider_X11Keyboard(CL_X11Window *window)
: sig_provider_event(0), window(window)
{
	current_keys_down.clear();
}

CL_InputDeviceProvider_X11Keyboard::~CL_InputDeviceProvider_X11Keyboard()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Keyboard attributes:

bool CL_InputDeviceProvider_X11Keyboard::get_keycode(int keycode) const
{
	// Ignore all key events when we don't have focus
	if(!window->has_focus())
		return false;

	char keyboard_state[32];

	KeyCode code = XKeysymToKeycode(window->get_display(), keycode);
	XQueryKeymap(window->get_display(), keyboard_state);

	return keyboard_state[code/8] & (1 << code%8);
}

CL_String CL_InputDeviceProvider_X11Keyboard::get_key_name(int virtual_key) const
{
	// Look up key name:

	char *name = XKeysymToString(virtual_key);
	if (name && name[0] != 0) return name;

	// Unknown. Return something at least :)
	char buffer[256];
	snprintf(buffer, 256, "Unknown %d", virtual_key);
	return buffer;
}

float CL_InputDeviceProvider_X11Keyboard::get_axis(int index) const
{
	return 0.0f;
}

CL_String CL_InputDeviceProvider_X11Keyboard::get_name() const
{
	return cl_text("System Keyboard");
}

CL_String CL_InputDeviceProvider_X11Keyboard::get_device_name() const
{
	return cl_text("System Keyboard");
}

int CL_InputDeviceProvider_X11Keyboard::get_axis_count() const
{
	return 0;
}

int CL_InputDeviceProvider_X11Keyboard::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Keyboard operations:

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Keyboard implementation:

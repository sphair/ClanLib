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

#include <cstdio>
#include <cstring>
#include "API/Display/input_device.h"
#include "API/Display/input_event.h"
#include "API/Display/keys.h"
#include "input_device_x11keyboard.h"
#include "../../GL/GLX/display_window_opengl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_X11Keyboard construction:

CL_InputDevice_X11Keyboard::CL_InputDevice_X11Keyboard(CL_DisplayWindow_OpenGL *owner) :
	owner(owner)
{
	type = CL_InputDevice::keyboard;

	slot_xevent = owner->sig_xevent.connect(
		this, &CL_InputDevice_X11Keyboard::on_xevent);

	current_keys_down.clear();
}

CL_InputDevice_X11Keyboard::~CL_InputDevice_X11Keyboard()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_X11Keyboard attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_X11Keyboard operations:

std::string CL_InputDevice_X11Keyboard::get_key_name(int virtual_key) const
{
	// Look up key name:
	char *name = XKeysymToString(virtual_key);
	if (name && name[0] != 0) return name;

	// Unknown. Return something at least :)
	char buffer[256];
	snprintf(buffer, 256, "Unknown %d", virtual_key);
	return buffer;
}

bool CL_InputDevice_X11Keyboard::get_keycode(int keysym) const
{
	// Ignore all key events when we don't have focus
	if(!owner->has_focus())
		return false;

	char keyboard_state[32];

	KeyCode code = XKeysymToKeycode(owner->get_display(), keysym);
	XQueryKeymap(owner->get_display(), keyboard_state);

	return keyboard_state[code/8] & (1 << code%8);
}

float CL_InputDevice_X11Keyboard::get_axis(int index) const
{
	return 0.0f;
}

std::string CL_InputDevice_X11Keyboard::get_name() const
{
	return "System Keyboard";
}

std::string CL_InputDevice_X11Keyboard::get_device_name() const
{
  return "x11:CoreKeyboard";
}

int CL_InputDevice_X11Keyboard::get_axis_count() const
{
	return 0;
}

int CL_InputDevice_X11Keyboard::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_X11Keyboard implementation:

void CL_InputDevice_X11Keyboard::on_xevent(XEvent &event)
{
	// Only handle keyboard events.
	if (event.type != KeyPress && event.type != KeyRelease) return;

	// Figure out what key it was:
	KeySym sym = XLookupKeysym(&event.xkey, 0);

	if (sym == NoSymbol)
	{
		//we have no idea what it is, probably the numpad = on an apple keyboard or something like that.
		//Better to not send a message, otherwise it might look like a mouse message but with bad mouse coordinates
		//if the app is sharing the handler its mouse stuff. -SAR
		return;
	}
	// set state of modifier keys
	bool key_state = false;
	if (event.type == KeyPress)
		key_state = true;
	
	switch(sym)
	{
		case CL_KEY_LCONTROL: owner->left_ctrl_down = key_state; break;
		case CL_KEY_LMENU: owner->left_alt_down = key_state; break;
		case CL_KEY_LSHIFT: owner->left_shift_down = key_state; break;
		case CL_KEY_RCONTROL: owner->right_ctrl_down = key_state; break;
		case CL_KEY_RMENU: owner->right_alt_down = key_state; break;
		case CL_KEY_RSHIFT: owner->right_shift_down = key_state; break;
	}
	
	// Setup event structure for ClanLib:
	CL_InputEvent key;
	key.type = CL_InputEvent::released;
	if (event.type == KeyPress) key.type = CL_InputEvent::pressed;
	key.id = sym;
	key.device = owner->keyboard;
	if(current_keys_down.find(sym) == current_keys_down.end())
	{
		key.repeat_count = 0;
		current_keys_down.insert(KeyDownCounter(sym,0));
	}
	else
	{
		current_keys_down[sym]++;
		key.repeat_count = current_keys_down[sym];
	}
	
	// Need to somehow get proper character sequence from X11. This code
	// wont keep proper track of deadkeys:
	char buf[11];
	buf[10] = 0;
	XLookupString(&event.xkey, buf, 10, 0, 0);
	if (strlen(buf) > 0) key.str = std::string(buf, 1);

	// Emit it.
	if (key.type == CL_InputEvent::pressed)
	{
		owner->keyboard.sig_key_down().call(key);
	}
	else
	{
		owner->keyboard.sig_key_up().call(key);
		current_keys_down.erase(sym);
	}
}

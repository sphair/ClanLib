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

#include "Display/display_precomp.h"
#include "API/Display/keyboard.h"
#include "API/Display/display.h"
#include "API/Display/display_window.h"
#include "API/Display/input_context.h"
#include "API/Display/input_device.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Keyboard construction:

/////////////////////////////////////////////////////////////////////////////
// CL_Keyboard attributes:

#define get_ic() CL_Display::get_current_window()->get_ic()

int CL_Keyboard::get_device_count()
{
	return get_ic()->get_keyboard_count();
}

CL_InputDevice &CL_Keyboard::get_device(int keyboard)
{
	return get_ic()->get_keyboard(keyboard);
}
	
bool CL_Keyboard::get_keycode(int keycode, int keyboard)
{
	return get_ic()->get_keyboard(keyboard).get_keycode(keycode);
}

std::string CL_Keyboard::get_key_name(int id)
{
	return get_ic()->get_keyboard().get_key_name(id);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Keyboard operations:

/////////////////////////////////////////////////////////////////////////////
// CL_Keyboard signals:

CL_Signal_v1<const CL_InputEvent &> &CL_Keyboard::sig_key_down(int keyboard)
{
	return get_ic()->get_keyboard(keyboard).sig_key_down();
}

CL_Signal_v1<const CL_InputEvent &> &CL_Keyboard::sig_key_up(int keyboard)
{
	return get_ic()->get_keyboard(keyboard).sig_key_up();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Input implementation:

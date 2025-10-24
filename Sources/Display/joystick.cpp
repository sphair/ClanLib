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
#include "API/Display/joystick.h"
#include "API/Display/display.h"
#include "API/Display/display_window.h"
#include "API/Display/input_context.h"
#include "API/Display/input_device.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Joystick construction:

/////////////////////////////////////////////////////////////////////////////
// CL_Joystick attributes:

#define get_ic() CL_Display::get_current_window()->get_ic()

int CL_Joystick::get_device_count()
{
	return get_ic()->get_joystick_count();
}

CL_InputDevice &CL_Joystick::get_device(int joystick)
{
	return get_ic()->get_joystick(joystick);
}

bool CL_Joystick::get_keycode(int keycode, int joystick)
{
	return get_ic()->get_joystick(joystick).get_keycode(keycode);
}

std::string CL_Joystick::get_key_name(int id)
{
	return get_ic()->get_joystick().get_key_name(id);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Joystick operations:

/////////////////////////////////////////////////////////////////////////////
// CL_Joystick signals:

CL_Signal_v1<const CL_InputEvent &> &CL_Joystick::sig_key_down(int joystick)
{
	return get_ic()->get_joystick(joystick).sig_key_down();
}

CL_Signal_v1<const CL_InputEvent &> &CL_Joystick::sig_key_up(int joystick)
{
	return get_ic()->get_joystick(joystick).sig_key_up();
}

CL_Signal_v1<const CL_InputEvent &> &CL_Joystick::sig_move(int joystick)
{
	return get_ic()->get_joystick(joystick).sig_move();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Joystick implementation:

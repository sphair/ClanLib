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
#include "input_context_generic.h"
#include "input_device_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext_Generic construction:

CL_InputContext_Generic::CL_InputContext_Generic()
: ref_count(0)
{
}

CL_InputContext_Generic::~CL_InputContext_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_InputContext_Generic operations:

void CL_InputContext_Generic::add_ref()
{
	ref_count++;
}

void CL_InputContext_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

void CL_InputContext_Generic::clear()
{
	keyboards.clear();
	mice.clear();
	joysticks.clear();
}

void CL_InputContext_Generic::add_keyboard(const CL_InputDevice &keyboard)
{
	keyboard.impl->id = joysticks.size();
	keyboards.push_back(keyboard);
}

void CL_InputContext_Generic::add_mouse(const CL_InputDevice &mouse)
{
	mouse.impl->id = joysticks.size();
	mice.push_back(mouse);
}

void CL_InputContext_Generic::add_joystick(const CL_InputDevice &joystick)
{
	joystick.impl->id = joysticks.size();
	joysticks.push_back(joystick);
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext_Generic implementation:

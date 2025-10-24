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
#include "API/Core/System/error.h"
#include "API/Display/input_context.h"
#include "input_context_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext Construction:

CL_InputContext::CL_InputContext() : impl(new CL_InputContext_Generic)
{
	impl->add_ref();
}

CL_InputContext::CL_InputContext(const CL_InputContext &copy) : impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_InputContext::~CL_InputContext()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext Attributes:

int CL_InputContext::get_keyboard_count() const
{
	return impl->keyboards.size();
}

int CL_InputContext::get_mouse_count() const
{
	return impl->mice.size();
}

int CL_InputContext::get_joystick_count() const
{
	return impl->joysticks.size();
}

CL_InputDevice &CL_InputContext::get_keyboard(int keyboard)
{
	return impl->keyboards[keyboard];
}

CL_InputDevice &CL_InputContext::get_mouse(int mouse)
{
	return impl->mice[mouse];
}

CL_InputDevice &CL_InputContext::get_joystick(int joystick)
{
	return impl->joysticks[joystick];
}

CL_InputDevice &CL_InputContext::get_device(const std::string& devicename)
{
	size_t i;
	for (i = 0; i < impl->joysticks.size(); ++i)
		if (impl->joysticks[i].get_device_name() == devicename)
			return impl->joysticks[i];

	for (i = 0; i < impl->keyboards.size(); ++i)
		if (impl->keyboards[i].get_device_name() == devicename)
			return impl->keyboards[i];

	for (i = 0; i < impl->mice.size(); ++i)
		if (impl->mice[i].get_device_name() == devicename)
			return impl->mice[i];

	throw CL_Error("Couldn't find CL_InputDevice named '" + devicename + "'");
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext operations:

CL_InputContext &CL_InputContext::operator =(CL_InputContext &copy)
{
	if (impl == copy.impl) return *this;
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

void CL_InputContext::clear()
{
	impl->clear();
}

void CL_InputContext::add_keyboard(const CL_InputDevice &keyboard)
{
	impl->add_keyboard(keyboard);
}

void CL_InputContext::add_mouse(const CL_InputDevice &mouse)
{
	impl->add_mouse(mouse);
}

void CL_InputContext::add_joystick(const CL_InputDevice &joystick)
{
	impl->add_joystick(joystick);
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext implementation:


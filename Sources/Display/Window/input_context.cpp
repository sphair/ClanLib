/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "API/Core/System/exception.h"
#include "API/Display/Window/input_context.h"
#include "input_context_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext Construction:

CL_InputContext::CL_InputContext()
: impl(new CL_InputContext_Impl)
{
	impl->input_context = impl;
}

CL_InputContext::~CL_InputContext()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext Attributes:

int CL_InputContext::get_keyboard_count() const
{
	impl->throw_if_disposed();
	return impl->keyboards.size();
}

int CL_InputContext::get_mouse_count() const
{
	impl->throw_if_disposed();
	return impl->mice.size();
}

int CL_InputContext::get_joystick_count() const
{
	impl->throw_if_disposed();
	return impl->joysticks.size();
}

int CL_InputContext::get_tablet_count() const
{
	impl->throw_if_disposed();
	return impl->tablets.size();
}

CL_InputDevice &CL_InputContext::get_keyboard(int keyboard)
{
	impl->throw_if_disposed();
	return impl->keyboards[keyboard];
}

CL_InputDevice &CL_InputContext::get_mouse(int mouse)
{
	impl->throw_if_disposed();
	return impl->mice[mouse];
}

CL_InputDevice &CL_InputContext::get_joystick(int joystick)
{
	impl->throw_if_disposed();
	return impl->joysticks[joystick];
}

CL_InputDevice &CL_InputContext::get_tablet(int tablet)
{
	impl->throw_if_disposed();
	return impl->tablets[tablet];
}

CL_InputDevice &CL_InputContext::get_device(const CL_StringRef& devicename)
{
	impl->throw_if_disposed();
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

	for (i = 0; i < impl->tablets.size(); ++i)
		if (impl->tablets[i].get_device_name() == devicename)
			return impl->tablets[i];

	throw CL_Exception("Couldn't find CL_InputDevice named '" + devicename + "'");
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext operations:

CL_InputContext &CL_InputContext::operator =(CL_InputContext &copy)
{
	impl->throw_if_disposed();
	impl = copy.impl;
	return *this;
}

void CL_InputContext::clear()
{
	impl->throw_if_disposed();
	impl->clear();
}

void CL_InputContext::add_keyboard(CL_InputDevice keyboard)
{
	impl->throw_if_disposed();
	impl->add_keyboard(keyboard);
}

void CL_InputContext::add_mouse(CL_InputDevice mouse)
{
	impl->throw_if_disposed();
	impl->add_mouse(mouse);
}

void CL_InputContext::add_joystick(CL_InputDevice joystick)
{
	impl->throw_if_disposed();
	impl->add_joystick(joystick);
}

void CL_InputContext::add_tablet(CL_InputDevice tablet)
{
	impl->throw_if_disposed();
	impl->add_tablet(tablet);
}

void CL_InputContext::process_messages()
{
	impl->throw_if_disposed();
	impl->process_messages();
}

bool CL_InputContext::poll(bool peek_only)
{
	impl->throw_if_disposed();
	return (impl->poll(peek_only));
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext implementation:


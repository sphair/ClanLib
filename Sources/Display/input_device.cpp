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
#include "API/Display/input_device.h"
#include "input_device_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice construction:

CL_InputDevice::CL_InputDevice()
: impl(0)
{
}

CL_InputDevice::CL_InputDevice(const CL_InputDevice &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_InputDevice::~CL_InputDevice()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice attributes:

int CL_InputDevice::get_id() const
{
	return impl->id;
}

CL_InputDevice::Type CL_InputDevice::get_type() const
{
	return impl->type;
}

std::string CL_InputDevice::get_key_name(int id) const
{
	return impl->get_key_name(id);
}

int CL_InputDevice::get_x() const
{
	return impl->get_x();
}

int CL_InputDevice::get_y() const
{
	return impl->get_y();
}

bool CL_InputDevice::get_keycode(int keycode) const
{
	return impl->get_keycode(keycode);
}

std::string CL_InputDevice::keyid_to_string(int keycode) const
{
	return impl->keyid_to_string(keycode);
}

int CL_InputDevice::string_to_keyid(const std::string &str) const
{
	return impl->string_to_keyid(str);
}

float CL_InputDevice::get_axis(int index) const
{
	return impl->get_axis(index);
}

std::string CL_InputDevice::get_name() const
{
	return impl->get_name();
}

std::string CL_InputDevice::get_device_name() const
{
	return impl->get_device_name();
}

int CL_InputDevice::get_axis_count() const
{
	return impl->get_axis_count();
}

int CL_InputDevice::get_button_count() const
{
	return impl->get_button_count();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice operations:

CL_InputDevice &CL_InputDevice::operator =(const CL_InputDevice &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();

	return *this;
}

void CL_InputDevice::set_position(int x, int y)
{
	impl->set_position(x, y);
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice signals:

CL_Signal_v1<const CL_InputEvent &> &CL_InputDevice::sig_key_down()
{
	return impl->sig_key_down;
}

CL_Signal_v1<const CL_InputEvent &> &CL_InputDevice::sig_key_up()
{
	return impl->sig_key_up;
}

CL_Signal_v1<const CL_InputEvent &> &CL_InputDevice::sig_key_dblclk()
{
	return impl->sig_key_dblclk;
}

CL_Signal_v1<const CL_InputEvent &> &CL_InputDevice::sig_pointer_move()
{
	return impl->sig_pointer_move;
}

CL_Signal_v1<const CL_InputEvent &> &CL_InputDevice::sig_axis_move()
{
	return impl->sig_axis_move;
}

CL_Signal_v1<const CL_InputEvent &> &CL_InputDevice::sig_ball_move()
{
	return impl->sig_ball_move;
}

CL_Signal_v1<const CL_InputEvent &> &CL_InputDevice::sig_move()
{
	return impl->sig_pointer_move;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice implementation:

CL_InputDevice::CL_InputDevice(CL_InputDevice_Generic *impl)
: impl(impl)
{
	if (impl) impl->add_ref();
}

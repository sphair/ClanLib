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
#include "API/Display/input_buffer.h"
#include "API/Display/input_context.h"
#include "input_buffer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputBuffer construction:

CL_InputBuffer::CL_InputBuffer()
: impl(new CL_InputBuffer_Generic)
{
	impl->add_ref();
}

CL_InputBuffer::CL_InputBuffer(CL_InputDevice &device)
: impl(new CL_InputBuffer_Generic)
{
	add_device(device);
	impl->add_ref();
}

CL_InputBuffer::CL_InputBuffer(CL_InputContext *ic)
: impl(new CL_InputBuffer_Generic)
{
	add_devices(ic);
	impl->add_ref();
}

CL_InputBuffer::CL_InputBuffer(const CL_InputBuffer &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_InputBuffer::~CL_InputBuffer()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputBuffer attributes:

int CL_InputBuffer::keys_left() const
{
	return impl->buffer.size();
}

CL_InputEvent CL_InputBuffer::peek_key() const
{
	if (impl->buffer.empty()) return CL_InputEvent();
	return impl->buffer.front();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputBuffer operations:

CL_InputBuffer &CL_InputBuffer::operator =(const CL_InputBuffer &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

void CL_InputBuffer::add_device(CL_InputDevice &device)
{
	impl->slots.connect(device.sig_key_down(), impl, &CL_InputBuffer_Generic::on_key_down);
	impl->slots.connect(device.sig_key_up(), impl, &CL_InputBuffer_Generic::on_key_up);
}

void CL_InputBuffer::add_devices(CL_InputContext *ic)
{
	for (int k = 0; k < ic->get_keyboard_count(); k++)
	{
		impl->slots.connect(ic->get_keyboard(k).sig_key_down(), impl, &CL_InputBuffer_Generic::on_key_down);
		impl->slots.connect(ic->get_keyboard(k).sig_key_up(), impl, &CL_InputBuffer_Generic::on_key_up);
	}

	for (int m = 0; m < ic->get_mouse_count(); m++)
	{
		impl->slots.connect(ic->get_mouse(m).sig_key_down(), impl, &CL_InputBuffer_Generic::on_key_down);
		impl->slots.connect(ic->get_mouse(m).sig_key_up(), impl, &CL_InputBuffer_Generic::on_key_up);
	}

	for (int j = 0; j < ic->get_joystick_count(); j++)
	{
		impl->slots.connect(ic->get_joystick(j).sig_key_down(), impl, &CL_InputBuffer_Generic::on_key_down);
		impl->slots.connect(ic->get_joystick(j).sig_key_up(), impl, &CL_InputBuffer_Generic::on_key_up);
	}
}

CL_InputEvent CL_InputBuffer::pop_key()
{
	if (impl->buffer.empty()) return CL_InputEvent();

	CL_InputEvent k = impl->buffer.front();
	impl->buffer.pop();
	return k;
}

void CL_InputBuffer::clear()
{
	impl->buffer = std::queue<CL_InputEvent>();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputBuffer implementation:

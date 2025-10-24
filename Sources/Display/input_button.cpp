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
#include "API/Display/input_button.h"
#include "input_button_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputButton construction:

CL_InputButton::CL_InputButton()
: impl(new CL_InputButton_Generic)
{
}

CL_InputButton::CL_InputButton(const CL_InputButton &copy)
: impl(new CL_InputButton_Generic)
{
	// TODO:
}

CL_InputButton::~CL_InputButton()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputButton attributes:

int CL_InputButton::peek_pressed() const
{
	return impl->presses;
}

CL_InputEvent CL_InputButton::peek_key() const
{
	// TODO:
	return CL_InputEvent();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputButton operations:

CL_InputButton &CL_InputButton::operator =(const CL_InputButton &copy)
{
	// TODO:
	return *this;
}

void CL_InputButton::bind(const CL_KeyBinding &key)
{
	impl->bind(key);
}

void CL_InputButton::unbind(const CL_KeyBinding &key)
{
	impl->unbind(key);
}

void CL_InputButton::unbind_all()
{
	impl->bindings.clear();
}

const std::list<CL_KeyBinding> &CL_InputButton::get_bindings() const
{
	return impl->bindings;
}

int CL_InputButton::pop_pressed()
{
	int presses = impl->presses;
	impl->presses = 0;
	return presses;
}

CL_InputEvent CL_InputButton::pop_key()
{
	impl->presses = 0;
	return CL_InputEvent();
}

void CL_InputButton::clear()
{
	impl->presses = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputButton implementation:

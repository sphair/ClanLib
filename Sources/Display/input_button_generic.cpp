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
#include "input_button_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputButton_Generic construction:

/////////////////////////////////////////////////////////////////////////////
// CL_InputButton_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_InputButton_Generic operations:

void CL_InputButton_Generic::bind(const CL_KeyBinding &key)
{
	bindings.push_back(key);
	
	slots.connect(key.get_device().sig_key_down(), this, &CL_InputButton_Generic::on_input_down);
}

void CL_InputButton_Generic::unbind(const CL_KeyBinding &key)
{
	// TODO: Remove slot
	
	bindings.remove(key);
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputButton_Generic implementation:

void CL_InputButton_Generic::on_input_down(const CL_InputEvent &key)
{
	for(std::list<CL_KeyBinding>::iterator it = bindings.begin(); it != bindings.end();it++)
	{
		if(key.id == it->get_keycode())
		{
			presses++;
			return;
		}
	}
}

void CL_InputButton_Generic::on_input_up(const CL_InputEvent &key)
{
	// TODO:
	// What needs to be done exactly?
	// --MSR April 22, 2003
}

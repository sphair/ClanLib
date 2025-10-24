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

#include "precomp.h"
#include "../API/GUI/radiogroup.h"
#include "radiogroup_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_RadioGroup::CL_RadioGroup()
{
	impl = new CL_RadioGroup_Generic();
}

CL_RadioGroup::~CL_RadioGroup()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_RadioGroup::get_count() const
{
	return impl->buttons.size();
}
	
CL_RadioButton *CL_RadioGroup::get_toggled() const
{
	return impl->get_toggled();
}

const std::vector<CL_RadioButton *> &CL_RadioGroup::get_buttons() const
{
	return impl->buttons;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_RadioGroup::add(CL_RadioButton *button, bool delete_component)
{
	impl->add(button, delete_component);
}
	
void CL_RadioGroup::remove(CL_RadioButton *button)
{
	impl->remove(button);
}

void CL_RadioGroup::set_checked(CL_RadioButton *button)
{
	impl->set_checked(button);
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<CL_RadioButton *> &CL_RadioGroup::sig_selection_changed()
{
	return impl->sig_selection_changed;
}

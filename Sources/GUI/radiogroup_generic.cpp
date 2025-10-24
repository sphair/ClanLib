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
#include "../API/GUI/radiobutton.h"
#include "radiogroup_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_RadioButton *CL_RadioGroup_Generic::get_toggled() const
{
	std::vector<CL_RadioButton *>::const_iterator it;
	for(it = buttons.begin(); it != buttons.end(); ++it)
		if((*it)->is_checked())
			return (*it);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_RadioGroup_Generic::add(CL_RadioButton *button, bool delete_component)
{
	// TODO: Use delete_component
	
	buttons.push_back(button);

	slots.connect(button->sig_clicked(), this, &CL_RadioGroup_Generic::on_button_clicked, button);
}

void CL_RadioGroup_Generic::remove(CL_RadioButton *button)
{
	// TODO: Remove clicked-slot from slots as well
	
	std::vector<CL_RadioButton *>::iterator it;
	for(it = buttons.begin(); it != buttons.end(); ++it)
	{
		if((*it) == button)
		{
			buttons.erase(it);
			return;
		}	
	}
}

void CL_RadioGroup_Generic::set_checked(CL_RadioButton *button)
{
	if(button->is_checked())
		return;
		
	std::vector<CL_RadioButton *>::const_iterator it;
	for(it = buttons.begin(); it != buttons.end(); ++it)
		(*it)->set_checked((*it) == button);

	sig_selection_changed(button);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_RadioGroup_Generic::on_button_clicked(CL_RadioButton *button)
{
	std::vector<CL_RadioButton *>::iterator it;
	for(it = buttons.begin(); it != buttons.end(); ++it)
	{
		if((*it) != button)
			(*it)->set_checked(false);
		else
		{
			button->set_checked(true);
			sig_selection_changed(button);
		}
	}
}

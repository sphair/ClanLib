/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
*/

#include "precomp.h"
#include "selection.h"
#include "holder_component.h"

/////////////////////////////////////////////////////////////////////////////
// Selection Construction:

Selection::Selection()
{
}

/////////////////////////////////////////////////////////////////////////////
// Selection Attributes:

bool Selection::empty()
{
	return selected_holders.empty();
}

std::vector<HolderComponent*> Selection::get_selection()
{
	return selected_holders;
}

bool Selection::is_selected(HolderComponent *holder) const
{
	for (size_t i = 0; i < selected_holders.size(); i++)
		if (selected_holders[i] == holder)
			return true;
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// Selection Operations:

void Selection::add_holder(HolderComponent *holder)
{
	selected_holders.push_back(holder);
	signal_selection_changed.invoke();
}

void Selection::clear()
{
	selected_holders.clear();
	signal_selection_changed.invoke();
}

void Selection::remove_holder(HolderComponent *holder)
{
	std::vector<HolderComponent*>::iterator it;
	for (it=selected_holders.begin(); it != selected_holders.end(); ++it)
	{
		if ((*it) == holder)
		{
			it = selected_holders.erase(it);
			signal_selection_changed.invoke();
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Selection Events:

CL_Signal_v0 & Selection::sig_selection_changed()
{
	return signal_selection_changed;
}

/////////////////////////////////////////////////////////////////////////////
// Selection Implementation:



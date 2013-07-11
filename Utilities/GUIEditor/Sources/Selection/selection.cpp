/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "Selection/selection.h"
#include "GridComponent/grid_object.h"

Selection::Selection()
{
}

bool Selection::empty()
{
	return selected_objects.empty();
}

std::vector<GridObject*> Selection::get_selection()
{
	return selected_objects;
}

bool Selection::is_selected(GridObject *object) const
{
	for (size_t i = 0; i < selected_objects.size(); i++)
		if (selected_objects[i] == object)
			return true;
	return false;
}

void Selection::add_object(GridObject *object)
{
	selected_objects.push_back(object);
	signal_selection_changed.invoke();
}

void Selection::clear()
{
	selected_objects.clear();
	signal_selection_changed.invoke();
}

void Selection::remove_object(GridObject *object)
{
	std::vector<GridObject*>::iterator it;
	for (it=selected_objects.begin(); it != selected_objects.end(); ++it)
	{
		if ((*it) == object)
		{
			it = selected_objects.erase(it);
			signal_selection_changed.invoke();
			break;
		}
	}
}

Signal_v0 & Selection::sig_selection_changed()
{
	return signal_selection_changed;
}

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
#include "API/GUI/deck.h"
#include "deck_generic.h"
#include "API/Core/System/error.h"
/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Deck::CL_Deck()
: impl(new CL_Deck_Generic())
{
}

CL_Deck::~CL_Deck()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Component *CL_Deck::get_component(const std::string &name)
{
	std::map<std::string, CL_Component *>::iterator iter = impl->component_map.find(name);

	if (iter == impl->component_map.end())
		throw CL_Error("CL_Deck::get_component failed: " + name + " is not in the component map.");

	return (*iter).second;
}

CL_Component *CL_Deck::get_visible()
{
	return impl->active_component;
}

std::map<std::string, CL_Component *> &CL_Deck::get_map()
{
	return impl->component_map;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Deck::add(const std::string &name, CL_Component *component)
{
	if (impl->component_map.empty())
	{
		component->show(true);
		impl->active_component = component;
	}
	else
		component->show(false);

	impl->component_map[name] = component;
}

void CL_Deck::swap(const std::string &name)
{
	std::map<std::string, CL_Component *>::iterator iter = impl->component_map.find(name);

	if (iter == impl->component_map.end())
		throw CL_Error("CL_Deck::get_component failed: " + name + " is not in the component map.");

	// hide showing component.
	if (impl->active_component)
		impl->active_component->show(false);

	// show selected component.
	impl->active_component = (*iter).second;
	impl->active_component->show(true);
}

void CL_Deck::remove(const std::string &name)
{
	std::map<std::string, CL_Component *>::iterator iter = impl->component_map.find(name);

	if (iter == impl->component_map.end())
		throw CL_Error("CL_Deck::remove failed: " + name + " is not in the component map.");

	// set active_component to null if removing component is the active component.
	if (impl->active_component == (*iter).second)
		impl->active_component = 0;

	impl->component_map.erase(name);
}

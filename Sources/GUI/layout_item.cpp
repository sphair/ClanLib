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
#include "layout_item.h"
#include "layout_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_LayoutItem Construction:

CL_LayoutItem::CL_LayoutItem(CL_Component *component) : component(component)
{
}

CL_LayoutItem::CL_LayoutItem(CL_SharedPtr<CL_Layout_Generic> layout) : component(0), layout(layout)
{
}

CL_LayoutItem::~CL_LayoutItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_LayoutItem Attributes:

CL_Size CL_LayoutItem::get_preferred_size() const
{
	if (component) return component->get_preferred_size();
	return layout->get_preferred_size();
}

CL_Size CL_LayoutItem::get_minimum_size() const
{
	if (component) return component->get_minimum_size();
	return layout->get_minimum_size();
}

CL_Size CL_LayoutItem::get_maximum_size() const
{
	if (component) return component->get_maximum_size();
	return layout->get_maximum_size();
}

CL_LayoutPolicy CL_LayoutItem::get_vertical_policy() const
{
	if (component) return component->get_vertical_policy();
	return layout->get_vertical_policy();
}

CL_LayoutPolicy CL_LayoutItem::get_horizontal_policy() const
{
	if (component) return component->get_horizontal_policy();
	return layout->get_horizontal_policy();
}

/////////////////////////////////////////////////////////////////////////////
// CL_LayoutItem Operations:

void CL_LayoutItem::set_position(const CL_Rect &pos)
{
	if (component) component->set_position(pos);
	else layout->set_position(pos);
}

/////////////////////////////////////////////////////////////////////////////
// CL_LayoutItem Implementation:

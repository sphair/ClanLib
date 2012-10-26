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

#include "GUI/precomp.h"
#include "API/GUI/gui_layout_corners.h"
#include "API/GUI/gui_component.h"
#include "API/Core/Math/size.h"
#include "API/Core/Math/rect.h"
#include <vector>
#include "gui_layout_provider_corners.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUILayoutProvider_Corners Construction:

GUILayoutProvider_Corners::GUILayoutProvider_Corners()
{
}

GUILayoutProvider_Corners::~GUILayoutProvider_Corners()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUILayoutProvider_Corners Attributes:

Size GUILayoutProvider_Corners::get_preferred_size() const
{
	return preferred_size;
}

/////////////////////////////////////////////////////////////////////////////
// GUILayoutProvider_Corners Operations:

void GUILayoutProvider_Corners::add_component( GUIComponent *component,
	ComponentAnchorPoint ap_tl, int dist_tl_x, int dist_tl_y,
	ComponentAnchorPoint ap_br, int dist_br_x, int dist_br_y )
{
	LayoutData ld;
	ld.component = component;
	ld.anchor_tl = ap_tl;
	ld.anchor_br = ap_br;
	ld.dist_tl_x = dist_tl_x;
	ld.dist_tl_y = dist_tl_y;
	ld.dist_br_x = dist_br_x;
	ld.dist_br_y = dist_br_y;

	components.push_back(ld);
}

void GUILayoutProvider_Corners::remove_component(GUIComponent *component)
{
}

void GUILayoutProvider_Corners::set_geometry(const Rect &geometry)
{
	rect = geometry;

	std::vector<LayoutData>::iterator it;
	for (it = components.begin(); it != components.end(); ++it)
	{
		LayoutData &ld = (*it);

		Point tl = get_point(ld.anchor_tl, ld.dist_tl_x, ld.dist_tl_y);
		Point br = get_point(ld.anchor_br, ld.dist_br_x, ld.dist_br_y);

		ld.component->set_geometry(Rect(tl.x, tl.y, br.x, br.y));
	}
}

void GUILayoutProvider_Corners::set_preferred_size(const Size &size)
{
	preferred_size = size;
}

/////////////////////////////////////////////////////////////////////////////
// GUILayoutProvider_Corners Implementation:

Point GUILayoutProvider_Corners::get_point(const Rect &rect, ComponentAnchorPoint ap, int dist_x, int dist_y)
{
	if (ap == cl_anchor_top_left)
	{
		return Point(dist_x, dist_y);
	}
	else if (ap == cl_anchor_top_right)
	{
		return Point(rect.get_width()-dist_x, dist_y);
	}
	else if (ap == cl_anchor_bottom_left)
	{
		return Point(dist_x, rect.get_height()-dist_y);
	}
	else if (ap == cl_anchor_bottom_right)
	{
		return Point(rect.get_width()-dist_x, rect.get_height()-dist_y);
	}

	// ap == cl_anchor_relative
	return Point(rect.get_width()*dist_x, rect.get_height()*dist_y);
}

Point GUILayoutProvider_Corners::get_point(ComponentAnchorPoint ap, int dist_x, int dist_y)
{
	return get_point(rect, ap, dist_x, dist_y);
}

}

/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners_Impl Class:

struct LayoutData
{
	CL_GUIComponent *component;
	CL_ComponentAnchorPoint anchor_tl, anchor_br;
	int dist_tl_x, dist_tl_y;
	int dist_br_x, dist_br_y;
};

class CL_GUILayoutCorners_Impl
{
public:
	CL_GUILayoutCorners_Impl()
	{
	}

	CL_Point get_point(CL_ComponentAnchorPoint ap, int dist_x, int dist_y);
	std::vector<LayoutData> components;
	CL_Size preferred_size;
	CL_Rect rect;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners Construction:

CL_GUILayoutCorners::CL_GUILayoutCorners()
: impl(new CL_GUILayoutCorners_Impl)
{
}

CL_GUILayoutCorners::~CL_GUILayoutCorners()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners Attributes:

CL_Size CL_GUILayoutCorners::get_preferred_size() const
{
	return impl->preferred_size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners Operations:

void CL_GUILayoutCorners::add_component( CL_GUIComponent *component,
	CL_ComponentAnchorPoint ap_tl, int dist_tl_x, int dist_tl_y,
	CL_ComponentAnchorPoint ap_br, int dist_br_x, int dist_br_y )
{
	LayoutData ld;
	ld.component = component;
	ld.anchor_tl = ap_tl;
	ld.anchor_br = ap_br;
	ld.dist_tl_x = dist_tl_x;
	ld.dist_tl_y = dist_tl_y;
	ld.dist_br_x = dist_br_x;
	ld.dist_br_y = dist_br_y;

	impl->components.push_back(ld);
}

void CL_GUILayoutCorners::remove_component(CL_GUIComponent *component)
{
}

void CL_GUILayoutCorners::set_geometry(const CL_Rect &geometry)
{
	impl->rect = geometry;

	std::vector<LayoutData>::iterator it;
	for (it = impl->components.begin(); it != impl->components.end(); ++it)
	{
		LayoutData &ld = (*it);

		CL_Point tl = impl->get_point(ld.anchor_tl, ld.dist_tl_x, ld.dist_tl_y);
		CL_Point br = impl->get_point(ld.anchor_br, ld.dist_br_x, ld.dist_br_y);

		ld.component->set_geometry(CL_Rect(tl.x, tl.y, br.x, br.y));
	}
}

void CL_GUILayoutCorners::set_preferred_size(const CL_Size &size)
{
	impl->preferred_size = size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners Implementation:

CL_Point CL_GUILayoutCorners_Impl::get_point(CL_ComponentAnchorPoint ap, int dist_x, int dist_y)
{
	if (ap == cl_anchor_top_left)
	{
		return CL_Point(dist_x, dist_y);
	}
	else if (ap == cl_anchor_top_right)
	{
		return CL_Point(rect.get_width()-dist_x, dist_y);
	}
	else if (ap == cl_anchor_bottom_left)
	{
		return CL_Point(dist_x, rect.get_height()-dist_y);
	}
	else if (ap == cl_anchor_bottom_right)
	{
		return CL_Point(rect.get_width()-dist_x, rect.get_height()-dist_y);
	}

	// ap == cl_anchor_relative
	return CL_Point(rect.get_width()*dist_x, rect.get_height()*dist_y);
}


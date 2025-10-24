/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_layout_corners.h"
#include "API/GUI/gui_component.h"
#include "API/Core/Math/size.h"
#include "API/Core/Math/rect.h"
#include <vector>
#include "gui_layout_provider_corners.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners Construction:

CL_GUILayoutCorners::CL_GUILayoutCorners() : CL_GUILayout( new CL_GUILayoutProvider_Corners())
{
}

CL_GUILayoutCorners::~CL_GUILayoutCorners()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners Attributes:

CL_GUILayoutProvider_Corners *CL_GUILayoutCorners::get_provider() const
{
	return static_cast <CL_GUILayoutProvider_Corners *> (CL_GUILayout::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners Operations:

void CL_GUILayoutCorners::add_component( CL_GUIComponent *component,
	CL_ComponentAnchorPoint ap_tl, int dist_tl_x, int dist_tl_y,
	CL_ComponentAnchorPoint ap_br, int dist_br_x, int dist_br_y )
{
	get_provider()->add_component(component, ap_tl, dist_tl_x, dist_tl_y, ap_br, dist_br_x, dist_br_y);
}

void CL_GUILayoutCorners::remove_component(CL_GUIComponent *component)
{
	get_provider()->remove_component(component);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners Implementation:


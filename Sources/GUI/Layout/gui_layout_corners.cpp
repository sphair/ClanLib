/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUILayoutCorners Construction:

GUILayoutCorners::GUILayoutCorners() : GUILayout( new GUILayoutProvider_Corners())
{
}

GUILayoutCorners::~GUILayoutCorners()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUILayoutCorners Attributes:

GUILayoutProvider_Corners *GUILayoutCorners::get_provider() const
{
	return static_cast <GUILayoutProvider_Corners *> (GUILayout::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// GUILayoutCorners Operations:

void GUILayoutCorners::add_component( GUIComponent *component,
	ComponentAnchorPoint ap_tl, int dist_tl_x, int dist_tl_y,
	ComponentAnchorPoint ap_br, int dist_br_x, int dist_br_y )
{
	get_provider()->add_component(component, ap_tl, dist_tl_x, dist_tl_y, ap_br, dist_br_x, dist_br_y);
}

void GUILayoutCorners::remove_component(GUIComponent *component)
{
	get_provider()->remove_component(component);
}

/////////////////////////////////////////////////////////////////////////////
// GUILayoutCorners Implementation:

}

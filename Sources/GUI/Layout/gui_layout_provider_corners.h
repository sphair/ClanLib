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

#pragma once

#include "API/Core/Math/rect.h"
#include "API/GUI/gui_layout.h"
#include "API/GUI/gui_layout_corners.h"
#include "API/GUI/Providers/gui_layout_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUILayoutCorners_Impl Class:

struct LayoutData
{
	GUIComponent *component;
	ComponentAnchorPoint anchor_tl, anchor_br;
	int dist_tl_x, dist_tl_y;
	int dist_br_x, dist_br_y;
};

class GUILayoutProvider_Corners : public GUILayoutProvider
{
/// \name Construction
/// \{

public:
	GUILayoutProvider_Corners();
	~GUILayoutProvider_Corners();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the size of the dialog, as set in the editor.
	Size get_css_size() const;

/// \}
/// \name Operations
/// \{

public:

	void set_geometry(const Rect &pos);

	void set_preferred_size(const Size &size);

	void add_component( GUIComponent *component,
	ComponentAnchorPoint ap_tl, int dist_tl_x, int dist_tl_y,
	ComponentAnchorPoint ap_br, int dist_br_x, int dist_br_y );

	void remove_component(GUIComponent *component);

	static Point get_point(const Rect &rect, ComponentAnchorPoint ap, int dist_x, int dist_y);

/// \}
/// \name Implementation
/// \{

private:
	Point get_point(ComponentAnchorPoint ap, int dist_x, int dist_y);

	std::vector<LayoutData> components;
	Size preferred_size;
	Rect rect;

/// \}
};

}

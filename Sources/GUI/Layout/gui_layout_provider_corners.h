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
*/

#pragma once

#include "API/Core/Math/rect.h"
#include "API/GUI/gui_layout.h"
#include "API/GUI/gui_layout_corners.h"
#include "API/GUI/Providers/gui_layout_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutCorners_Impl Class:

struct LayoutData
{
	CL_GUIComponent *component;
	CL_ComponentAnchorPoint anchor_tl, anchor_br;
	int dist_tl_x, dist_tl_y;
	int dist_br_x, dist_br_y;
};

class CL_GUILayoutProvider_Corners : public CL_GUILayoutProvider
{
/// \name Construction
/// \{

public:
	CL_GUILayoutProvider_Corners();
	~CL_GUILayoutProvider_Corners();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the size of the dialog, as set in the editor.
	CL_Size get_preferred_size() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Destroys the provider.
	void destroy();

	void set_geometry(const CL_Rect &pos);

	void set_preferred_size(const CL_Size &size);

	void add_component( CL_GUIComponent *component,
	CL_ComponentAnchorPoint ap_tl, int dist_tl_x, int dist_tl_y,
	CL_ComponentAnchorPoint ap_br, int dist_br_x, int dist_br_y );

	void remove_component(CL_GUIComponent *component);

/// \}
/// \name Implementation
/// \{

private:
	CL_Point get_point(CL_ComponentAnchorPoint ap, int dist_x, int dist_y);

	std::vector<LayoutData> components;
	CL_Size preferred_size;
	CL_Rect rect;

/// \}
};

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
**    Magnus Norddahl
*/

#pragma once

#include "Popups/anchor_popup.h"
#include "GridComponent/grid_object.h"

class PropertyItemAnchor : public PropertyItem
{
public:
	PropertyItemAnchor() : cap_tl(cl_anchor_top_left), cap_br(cl_anchor_top_left)
	{
		name = "Anchor";
	}

	GUIComponent *activate()
	{
		return new AnchorPopup(cap_tl, cap_br, property_component);
	}

	void deactivate(GUIComponent *component)
	{
		cap_tl = static_cast<AnchorPopup*>(component)->get_anchor_tl();
		cap_br = static_cast<AnchorPopup*>(component)->get_anchor_br();
		delete component;
	}

	std::string get_value()
	{
		return string_format("TL: %1 BR: %2", to_string(cap_tl), to_string(cap_br));
	}

	std::string to_string(ComponentAnchorPoint cap)
	{
		switch (cap)
		{
		case cl_anchor_top_left:
			return "top left";
		case cl_anchor_top_right:
			return "top right";
		case cl_anchor_bottom_left:
			return "bottom left";
		case cl_anchor_bottom_right:
			return "bottom right";
		case cl_anchor_relative:
			return "relative";
		default:
			return "unknown";
		}
	}

	int get_inactive_height(Canvas &canvas, GUIThemePart &part, int width)
	{
		return part.get_font().get_text_size(canvas, get_value()).height;
	}

	void render_inactive(Canvas &canvas, GUIThemePart &part, const Rect &rect)
	{
		part.render_text(canvas, get_value(), rect);
	}

	void selection_changed(const std::vector<GridObject *> &selection)
	{
		if (!selection.empty())
		{
			GridObject *component = selection[0];
			cap_tl = component->get_anchor_tl();
			cap_br = component->get_anchor_br();
		}
	}

	void apply_changes(GridObject *component)
	{
		component->set_anchor_tl(cap_tl);
		component->set_anchor_br(cap_br);
	}

private:
	ComponentAnchorPoint cap_tl;
	ComponentAnchorPoint cap_br;
};

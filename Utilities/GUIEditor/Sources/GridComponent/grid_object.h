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
**    Magnus Norddahl
*/

#pragma once

#include "snapline.h"

class GridComponent;
class ComponentType;

class GridObject : public CL_GUIComponent
{
public:
	GridObject(GridComponent *grid_component, CL_GUIComponent *parent, int id, const CL_Vec2i &pos);

	CL_ComponentAnchorPoint get_anchor_tl();
	CL_ComponentAnchorPoint get_anchor_br();
	CL_String get_position_equation_x() const;
	CL_String get_position_equation_y() const;
	CL_String get_position_equation_x2() const;
	CL_String get_position_equation_y2() const;

	CL_GUIComponent *get_container();
	CL_GUIComponent *get_component() { return get_first_child(); }
	ComponentType *get_component_type() { return component_type; }

	GridComponent *get_grid_component() { return grid_component; }

	GridObject *get_next_sibling() { return dynamic_cast<GridObject*>(CL_GUIComponent::get_next_sibling()); }
	GridObject *get_previous_sibling() { return dynamic_cast<GridObject*>(CL_GUIComponent::get_previous_sibling()); }

	CL_Rect get_grabber_w() const;
	CL_Rect get_grabber_nw() const;
	CL_Rect get_grabber_n() const;
	CL_Rect get_grabber_ne() const;
	CL_Rect get_grabber_e() const;
	CL_Rect get_grabber_se() const;
	CL_Rect get_grabber_s() const;
	CL_Rect get_grabber_sw() const;

	std::vector<SnapLine> get_snaplines() const;

	void set_selected(bool value);
	CL_DomElement to_element(CL_DomDocument &doc);
	void set_anchor_tl(CL_ComponentAnchorPoint);
	void set_anchor_br(CL_ComponentAnchorPoint);
	void set_position_equations(const CL_String &str_x, const CL_String &str_y);
	void set_position_equations2(const CL_String &str_x, const CL_String &str_y);

	static GridObject *find_object_at(CL_GUIComponent *container, const CL_Point &pos);

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_resized();

	void save_geometry(CL_DomElement &e, CL_GUIComponent *comp);
	void save_anchors(CL_DomElement &e, CL_GUIComponent *comp);
	void save_listview(CL_DomElement &e, CL_ListView *lv);

	CL_Point get_dist(CL_ComponentAnchorPoint ap, CL_Point p, CL_Rect boundary);
	CL_Rect convert_coordinates(CL_GUIComponent *from, CL_GUIComponent *to);
	CL_GUIComponent *get_toplevel_component();
	CL_GUIComponent *get_tab_or_frame_parent(CL_GUIComponent *comp);

	CL_String pos_equation_x;
	CL_String pos_equation_y;
	CL_String pos_equation_x2;
	CL_String pos_equation_y2;

	CL_ComponentAnchorPoint anchor_tl;
	CL_ComponentAnchorPoint anchor_br;

	GridComponent *grid_component;
	ComponentType *component_type;
};

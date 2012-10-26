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
**    Magnus Norddahl
*/

#pragma once

#include "snapline.h"

class GridComponent;
class ComponentType;

class GridObject : public GUIComponent
{
public:
	GridObject(GridComponent *grid_component, GUIComponent *parent, int id, const Vec2i &pos);

	ComponentAnchorPoint get_anchor_tl();
	ComponentAnchorPoint get_anchor_br();
	std::string get_position_equation_x() const;
	std::string get_position_equation_y() const;
	std::string get_position_equation_x2() const;
	std::string get_position_equation_y2() const;

	GUIComponent *get_container();
	GUIComponent *get_component() { return get_first_child(); }
	ComponentType *get_component_type() { return component_type; }

	GridComponent *get_grid_component() { return grid_component; }

	GridObject *get_next_sibling() { return dynamic_cast<GridObject*>(GUIComponent::get_next_sibling()); }
	GridObject *get_previous_sibling() { return dynamic_cast<GridObject*>(GUIComponent::get_previous_sibling()); }

	Rect get_grabber_w() const;
	Rect get_grabber_nw() const;
	Rect get_grabber_n() const;
	Rect get_grabber_ne() const;
	Rect get_grabber_e() const;
	Rect get_grabber_se() const;
	Rect get_grabber_s() const;
	Rect get_grabber_sw() const;

	std::vector<SnapLine> get_snaplines() const;

	void set_selected(bool value);
	DomElement to_element(DomDocument &doc);
	void set_anchor_tl(ComponentAnchorPoint);
	void set_anchor_br(ComponentAnchorPoint);
	void set_position_equations(const std::string &str_x, const std::string &str_y);
	void set_position_equations2(const std::string &str_x, const std::string &str_y);

	static GridObject *find_object_at(GUIComponent *container, const Point &pos);

private:
	void on_render(GraphicContext &gc, const Rect &update_rect);
	void on_resized();

	void save_geometry(DomElement &e, GUIComponent *comp);
	void save_anchors(DomElement &e, GUIComponent *comp);
	void save_listview(DomElement &e, ListView *lv);

	Point get_dist(ComponentAnchorPoint ap, Point p, Rect boundary);
	Rect convert_coordinates(GUIComponent *from, GUIComponent *to);
	GUIComponent *get_toplevel_component();
	GUIComponent *get_tab_or_frame_parent(GUIComponent *comp);

	std::string pos_equation_x;
	std::string pos_equation_y;
	std::string pos_equation_x2;
	std::string pos_equation_y2;

	ComponentAnchorPoint anchor_tl;
	ComponentAnchorPoint anchor_br;

	GridComponent *grid_component;
	ComponentType *component_type;
};

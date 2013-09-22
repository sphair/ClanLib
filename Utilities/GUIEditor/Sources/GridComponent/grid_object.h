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
**    Magnus Norddahl
*/

#pragma once

#include "snapline.h"

class GridComponent;
class ComponentType;

class GridObject : public clan::GUIComponent
{
public:
	GridObject(GridComponent *grid_component, clan::GUIComponent *parent, int id, const clan::Vec2i &pos);

	clan::ComponentAnchorPoint get_anchor_tl();
	clan::ComponentAnchorPoint get_anchor_br();
	std::string get_position_equation_x() const;
	std::string get_position_equation_y() const;
	std::string get_position_equation_x2() const;
	std::string get_position_equation_y2() const;

	clan::GUIComponent *get_container();
	clan::GUIComponent *get_component() { return get_first_child(); }
	ComponentType *get_component_type() { return component_type; }

	GridComponent *get_grid_component() { return grid_component; }

	GridObject *get_next_sibling() { return dynamic_cast<GridObject*>(clan::GUIComponent::get_next_sibling()); }
	GridObject *get_previous_sibling() { return dynamic_cast<GridObject*>(clan::GUIComponent::get_previous_sibling()); }

	clan::Rect get_grabber_w() const;
	clan::Rect get_grabber_nw() const;
	clan::Rect get_grabber_n() const;
	clan::Rect get_grabber_ne() const;
	clan::Rect get_grabber_e() const;
	clan::Rect get_grabber_se() const;
	clan::Rect get_grabber_s() const;
	clan::Rect get_grabber_sw() const;

	std::vector<SnapLine> get_snaplines() const;

	void set_selected(bool value);
	clan::DomElement to_element(clan::DomDocument &doc);
	void set_anchor_tl(clan::ComponentAnchorPoint);
	void set_anchor_br(clan::ComponentAnchorPoint);
	void set_position_equations(const std::string &str_x, const std::string &str_y);
	void set_position_equations2(const std::string &str_x, const std::string &str_y);

	static GridObject *find_object_at(clan::GUIComponent *container, const clan::Point &pos);

private:
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	void on_resized();

	void save_geometry(clan::DomElement &e, clan::GUIComponent *comp);
	void save_anchors(clan::DomElement &e, clan::GUIComponent *comp);
	void save_listview(clan::DomElement &e, clan::ListView *lv);

	clan::Point get_dist(clan::ComponentAnchorPoint ap, clan::Point p, clan::Rect boundary);
	clan::Rect convert_coordinates(clan::GUIComponent *from, clan::GUIComponent *to);
	clan::GUIComponent *get_toplevel_component();
	clan::GUIComponent *get_tab_or_frame_parent(clan::GUIComponent *comp);

	std::string pos_equation_x;
	std::string pos_equation_y;
	std::string pos_equation_x2;
	std::string pos_equation_y2;

	clan::ComponentAnchorPoint anchor_tl;
	clan::ComponentAnchorPoint anchor_br;

	GridComponent *grid_component;
	ComponentType *component_type;
};

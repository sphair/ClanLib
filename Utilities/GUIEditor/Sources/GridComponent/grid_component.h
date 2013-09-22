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
*/

#pragma once

#include "EditState/grid_edit_state.h"

class GuiEditorWindow;
class PropertyComponent;
class GridObject;
class SnapLine;

class GridComponent : public clan::GUIComponent
{
public:
	GridComponent(clan::GUIComponent *parent, GuiEditorWindow *main_window);
	virtual ~GridComponent() { }

	GuiEditorWindow *get_main_window() { return main_window; }

	clan::Size get_dialog_size();

	const std::vector<GridObject*> &get_objects() const;
	std::vector<SnapLine> get_snaplines() const;

	clan::Vec2i snap(GridObject *object, const std::vector<SnapLine> &source_snaplines, const clan::Rect &source_rect);

	GridObject *on_add_component(int id, const clan::Vec2i &pos);
	void remove_object(GridObject *object);

	void load(const std::string &str);
	void save(const std::string &str);
	void set_boundary_size(const clan::Size &size);

	void set_netselect_box(clan::Rect netselect_box);
	void select_objects(const clan::Rect &box);

	clan::Callback_v0 func_boundary_resized;

private:
	bool on_input_pressed(const clan::InputEvent &input_event);
	bool on_input_released(const clan::InputEvent &input_event);
	bool on_input_doubleclick(const clan::InputEvent &input_event);
	bool on_input_pointer_moved(const clan::InputEvent &input_event);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	void on_render_overlay(clan::Canvas &canvas, const clan::Rect &update_rect);
	void on_resized();

	clan::InputEvent offset_event(clan::InputEvent e);
	clan::Rect object_to_grid_coords(GridObject *object, const clan::Rect &rect);
	clan::Point object_to_grid_coords(GridObject *object, const clan::Point &point);
	clan::Rect grid_to_object_coords(GridObject *object, const clan::Rect &rect);
	clan::Point grid_to_object_coords(GridObject *object, const clan::Point &point);

	clan::Rect get_boundary_grabber_se() const;
	clan::Rect get_boundary_grabber_s() const;
	clan::Rect get_boundary_grabber_e() const;

	clan::Rect load_geometry(clan::DomElement &e);

	clan::DomElement to_element(clan::DomDocument &doc);
	GridObject *find_object_at(const clan::Point &pos);
	bool deliver_input_to_tab(const clan::InputEvent &e);

	void load(clan::DomElement &element, clan::GUIComponent *parent);

	GuiEditorWindow *main_window;
	clan::GUIComponent *component_container;
	clan::GUIComponent *component_overlay;
	std::vector<GridObject*> objects;

	clan::Size boundary;
	GridEditState edit_state;
	clan::GUIThemePart part_windowframe;

	clan::Rect netselect_box;

	friend class GridEditStateNone;
	friend class GridEditStateBoundarySizing;
	friend class GridEditStateNetSelecting;
	friend class GridEditStateObjectMoving;
	friend class GridEditStateObjectSizing;
	friend class GridEditStateCreateTool;
};

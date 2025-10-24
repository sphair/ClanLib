/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "grid_edit_state.h"

class MainWindow;
class PropertyComponent;
class HolderComponent;
class SnapLine;

class GridComponent : public CL_GUIComponent
{
public:
	GridComponent(CL_GUIComponent *parent, MainWindow *main_window);
	virtual ~GridComponent() { }

	CL_Size get_dialog_size();

	const std::vector<HolderComponent*> &get_holders() const;
	std::vector<SnapLine> get_snaplines() const;

	CL_Vec2i snap(HolderComponent *holder, const std::vector<SnapLine> &source_snaplines, const CL_Rect &source_rect);

	HolderComponent *on_add_component(int id, const CL_Vec2i &pos);
	void remove_holder(HolderComponent *holder);

	void load(const CL_StringRef &str);
	void save(const CL_StringRef &str);
	void set_boundary_size(const CL_Size &size);

	void set_netselect_box(CL_Rect netselect_box);
	void select_objects(const CL_Rect &box);

	CL_Callback_v0 func_boundary_resized;

private:
	bool on_input_pressed(const CL_InputEvent &input_event);
	bool on_input_released(const CL_InputEvent &input_event);
	bool on_input_doubleclick(const CL_InputEvent &input_event);
	bool on_input_pointer_moved(const CL_InputEvent &input_event);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_render_overlay(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_resized();

	CL_InputEvent offset_event(CL_InputEvent e);
	CL_Rect holder_to_grid_coords(HolderComponent *holder, const CL_Rect &rect);
	CL_Point holder_to_grid_coords(HolderComponent *holder, const CL_Point &point);
	CL_Rect grid_to_holder_coords(HolderComponent *holder, const CL_Rect &rect);
	CL_Point grid_to_holder_coords(HolderComponent *holder, const CL_Point &point);

	CL_Rect get_boundary_grabber_se() const;
	CL_Rect get_boundary_grabber_s() const;
	CL_Rect get_boundary_grabber_e() const;

	CL_Rect load_geometry(CL_DomElement &e);

	CL_DomElement to_element(CL_DomDocument &doc);
	HolderComponent *find_holder_at(const CL_Point &pos);
	bool deliver_input_to_tab(const CL_InputEvent &e);

	void load(CL_DomElement &element, CL_GUIComponent *parent);

	MainWindow *main_window;
	CL_GUIComponent *component_container;
	CL_GUIComponent *component_overlay;
	std::vector<HolderComponent*> holders;

	CL_Size boundary;
	GridEditState edit_state;
	CL_GUIThemePart part_windowframe;

	CL_Rect netselect_box;

	friend class GridEditStateNone;
	friend class GridEditStateBoundarySizing;
	friend class GridEditStateNetSelecting;
	friend class GridEditStateObjectMoving;
	friend class GridEditStateObjectSizing;
	friend class GridEditStateCreateTool;
};

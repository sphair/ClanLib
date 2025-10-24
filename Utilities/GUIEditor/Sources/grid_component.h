/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#ifndef header_grid_component
#define header_grid_component

class MainWindow;
class PropertyComponent;
class HolderComponent;

class CL_API_GUI GridComponent : public CL_GUIComponent
{
//! Construction:
public:
	GridComponent(CL_GUIComponent *parent, MainWindow *main_window);
	
	virtual ~GridComponent() {};

//! Attributes:
public:
	CL_Rect get_boundary();

	static bool is_grid_or_child_of_grid(CL_GUIComponent *comp);

//! Operations:
public:
	void on_add_component(int id);

	void remove_holder(HolderComponent *holder);

	void load(const CL_StringRef &str);

	void save(const CL_StringRef &str);
	
	void set_boundary_size(const CL_Size &size);

//! Events:
public:

//! Implementation:
private:
	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	bool on_filter_message(CL_GUIMessage &msg);
	void on_filter_mouse_left_down(CL_GUIMessage &msg, CL_InputEvent &e);
	void on_filter_mouse_left_up(CL_GUIMessage &msg, CL_InputEvent &e);
	void on_filter_mouse_move(CL_InputEvent &e);
	void on_resized();
	CL_Rect load_geometry(CL_DomElement &e);
	void load_listview(CL_DomElement &e, CL_ListView *lv);
	CL_GUIComponent *get_holder_parent(CL_GUIComponent *comp);
	void on_tab_page_resized(CL_TabPage *page);
	CL_DomElement to_element(CL_DomDocument &doc);
	void render_grid(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_mouse_left_tab_order(CL_GUIComponent *holder);
	void clear_tab_order_indexes();

	void load(CL_DomElement &element, CL_GUIComponent *parent);

	MainWindow *main_window;
	std::vector<HolderComponent*> holders;

	CL_GUIComponent *selected_holder;
	CL_Rect boundary;
	bool resizing;
	bool moving_boundary;

	std::map<int, int> new_component_count;
	int tab_page_count;

	CL_Rect grid_rect;
	std::vector<CL_Vec2i> grid_points;

	CL_Cursor default_cursor;
};

#endif

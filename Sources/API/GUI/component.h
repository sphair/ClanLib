/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_component
#define header_component

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>

#include "../signals.h"
#include "../Core/Math/rect.h"
#include "../Core/Math/point.h"
#include "../Display/input_event.h"
#include "component_style.h"

//: Layout policies for CL_Component.
//- !group=GUI/Framework!
//- !header=gui.h!
enum CL_LayoutPolicy
{
	layout_minimum,
	layout_expanding
};

class CL_GUIManager;
class CL_InputDevice;
class CL_StyleManager;
class CL_ComponentStyle;
class CL_DomElement;
class CL_Component_Generic;
class CL_ComponentManager;
class CL_Layout;

//: Component base class.
//- !group=GUI/Framework!
//- !header=gui.h!
//- <p>The component class is the base class for any component/widget/control (or
//- whatever you call it :)) in the GUI system. It is responsible for painting
//- the component, painting of any child components of it, and it receives the
//- input for a component.</p>
class CL_API_GUI CL_Component
{
//! Construction:
public:
	//: Component Constructor
	CL_Component(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Component Constructor
	CL_Component(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Component Destructor
	virtual ~CL_Component();

//! Attributes:
public:
	//: Returns the client area of the component.
	CL_Component *get_client_area() const;

	//: Returns the parent component, or NULL if none.
	CL_Component *get_parent() const;

	//: Returns the root parent.
	CL_Component *get_root_parent();

	//: Returns the GUI manager that this component is attached to, or NULL if none.
	CL_GUIManager *get_gui_manager() const;

	//: Returns the style manager used by this component.
	CL_StyleManager *get_style_manager() const;

	//: Returns the width of the component.
	int get_width() const;

	//: Returns the height of the component.
	int get_height() const;

	//: Returns the components x position in screen coordinates.
	int get_screen_x() const;

	//: Returns the components y position in screen coordinates.
	int get_screen_y() const;

	//: Returns the components x position in client coordinates.
	int get_client_x() const;

	//: Returns the components y position in client coordinates.
	int get_client_y() const;

	//: Returns the components position in screen coordinates.
	CL_Rect get_screen_rect() const;

	//: Returns the rect with coordinates converted from client to screen
	CL_Rect client_to_screen(const CL_Rect & rect);

	//: Returns the point with coordinates converted from client to screen
	CL_Point client_to_screen(const CL_Point & point);

	//: Returns the rect with coordinates converted from screen to client
	CL_Rect screen_to_client(const CL_Rect & rect);

	//: Returns the point with coordinates converted from screen to client
	CL_Point screen_to_client(const CL_Point & point);

	//: Returns the position of this component.
	const CL_Rect &get_position() const;

	//: Returns the position of this component after extending the rect to fit all children as well
	CL_Rect get_position_with_children();

	//: Get the minimum size of the component.
	CL_Size get_minimum_size() const;

	//: Get the maximum size of the component.
	CL_Size get_maximum_size() const;

	//: Get the preferred size of the component.
	CL_Size get_preferred_size() const;

	//: Returns the vertical size policy of the component.
	const CL_LayoutPolicy &get_vertical_policy() const;

	//: Returns the horizontal size policy of the component.
	const CL_LayoutPolicy &get_horizontal_policy() const;

	//: returns the layout for this component.
	const CL_Layout &get_layout() const;

	//: Returns true if event passing is enabled.
	bool get_event_passing() const;

	//: Returns true if the passed component is a child of this component.
	bool has_child(CL_Component *component) const;

	//: Returns the list of child components.
	const std::list<CL_Component *> &get_children() const;

	//: Returns the bounding rectangle of the component's children.
	CL_Rect get_children_rect() const;

	//: Returns the component at (pos_x, pos_y).
	//- <p>This functions searches for children recursively.
	//- NULL is returned if the point is outside the component.
	//- The component itself is returned if the component has no child at the specified point.</p>
	CL_Component *get_component_at(int pos_x, int pos_y);

	//: Returns the component that currently has the focus.
	CL_Component *get_focus() const;

	//: Returns true if component has the focus.
	bool has_focus() const;

	//: Returns true if the component can be focused.
	bool is_focusable() const;
	
	//: Returns true if the mouse is currently highlighting the component.
	bool has_mouse_over() const;

	//: Returns true if component is visible.
	//- <p>Is check_parents is true, it will check the state of the parents, and only return true
	//- if all parents are visible.</p>
	bool is_visible(bool check_parents = true) const;

	//: Returns true if component accepts user input.
	//- <p>Is check_parents is true, it will check the state of the parents, and only return true
	//- if all parents are enabled.</p>
	bool is_enabled(bool check_parents = true) const;

	//: Returns true if component has mouse captured.
	bool is_mouse_captured() const;

	//: Returns the tab id of the component.
	int get_tab_id() const;

	//: Returns the component style.
	CL_ComponentStyle *get_style() const;

	//: Returns true if component is in modal state.
	bool is_modal() const;
	
	//: Returns true if component have parents which on top of gui_manager modal stack.
	//- <p>Note, only "top" of modal stack.</p>
	bool has_modal_parent() const;

	//: Returns true if the topmost flag is set.
	bool is_topmost() const;

//! Operations:
public:
	//: Set the tab id of the component.
	void set_tab_id(int id);

	//: Sets the position (relative to its parent) and size of this component.
	void set_position(const CL_Rect &new_pos);

	//: Sets the position of this component (relative to its parent).
	void set_position(int new_x, int new_y);

	//: Sets the size of this component.
	void set_size(int new_width, int new_height);

	//: Changes the width of the component.
	void set_width(int width);

	//: Changes the height of the component.
	void set_height(int height);

	//: Set the minimum size of the component.
	void set_minimum_size(const CL_Size &size);

	//: Set the maximum size of the component.
	void set_maximum_size(const CL_Size &size);

	//: Set the preferred size of the component.
	void set_preferred_size(const CL_Size &size);

	//: Set the vertical size policy of the component.
	void set_vertical_policy(const CL_LayoutPolicy &policy);

	//: Set the horizontal size policy of the component.
	void set_horizontal_policy(const CL_LayoutPolicy &policy);

	//: Set the layout for this component.
	void set_layout(const CL_Layout &layout);

	//: Enables/disables the event passing to parent.
	void set_event_passing(bool enable = true);

	//: Sets the parent of this component.
	void set_parent(CL_Component *parent);

	//: Sets the GUI manager this component is attached to.
	void set_gui_manager(CL_GUIManager *gui);

	//: Changes the visibility flag.
	void show(bool show = true);

	//: Enable/disable user input.
	void enable(bool enable = true);

	//: Enable/disable the ability to focus component.
	void set_focusable(bool focusable = true);
	
	//: Give focus to this component.
	void set_focus();

	//: Raises this component to the top of the parent component's stack. 
	//- <p>If the topmost flag is set, it will get draw before any other components. If the flag
	//- is not set, it will be draw before any other components that do not have the topmost flag
	//- set.</p>
	void raise();

	//: Lowers this component to the bottom of the parent component's stack.
	//- <p>If the topmost flag is set, it will get draw after any other components with the topmost
	//- flag set, but before any components with the flag cleared.</p>
	void lower();

	//: Capture the mouse on the currently attached GUI manager. All mouse input
	//: will be directed to this component until release_mouse() is called.
	void capture_mouse();

	//: Releases the mouse capture.
	void release_mouse();

	//: Asks the GUI to repaint itself.
	//- <p>This function doesn't redraw the GUI immidiately. Instead it raises the 
	//- update event trigger in the CL_GUIManager parent.</p>
	void update();

	//: Calls sig_begin_paint(), then sig_paint(), and finally sig_end_paint().
	void paint();

	//: Run the component as a modal component until quit() is called.
	void run();

	//: Quit the modal loop of the component.
	void quit();

	//: "Closes" this component.
	//: Hides and detaches component from parent.
	void close();

	//: Adds the specified component as a children to this component.
	//- <p>If 'delete_child' is true, the child will be deleted when the component is deleted.</p>
	void add_child(CL_Component *child, bool delete_child = false);

	//: Removes the specified child from the component.
	void remove_child(CL_Component *child);

	//: Sets a component style to the component.
	//- <p>If 'delete_style' is true, the style will be deleted when the component is deleted.</p>
	void set_style(CL_ComponentStyle *style, bool delete_style = true);

	//: Calculate a default size.
	void find_preferred_size();

	//: Returns the component with the specified tab ID.
	CL_Component *find_tab_id(int tab_id);

	//: Set the client area of the component. Not used by user - only by other components.
	void set_client_area(CL_Component *component);

	//: Sets or clears the topmost flag.
	//- <p>Components with the topmost flag set are shown above components with it cleared.</p>
	void set_topmost_flag(bool topmost);

	//: Enables automatic clipping when drawing component.
	void set_clipping(bool enable);

//! Signals:
public:
	//: Update component with a new set of component options.
	CL_Signal_v1<const CL_DomElement &> &sig_set_options();

	//: Draws the component. Must be called in a
	//: begin_paint() / end_paint() session.
	CL_Signal_v0 &sig_paint();

	//: Called prior to component drawing. Sets up a cliprect and translocates
	//: coordinates accordingly to this component.
	CL_Signal_v0 &sig_begin_paint();

	//: Called after component drawing, and draws all the children.
	CL_Signal_v0 &sig_paint_children();

	//: Cleans up when component drawing is finished (cleans up cliprects and
	//: translocation stuff).
	CL_Signal_v0 &sig_end_paint();

	//: Called when any inputdevice button is pressed (keyboard, mouse, joystick, etc).
	CL_Signal_v1<const CL_InputEvent &> &sig_input_down();

	//: Called when any inputdevice button is released (keyboard, mouse, joystick, etc)
	CL_Signal_v1< const CL_InputEvent &> &sig_input_up();

	//: Called when a key is pressed on keyboard.
	CL_Signal_v1<const CL_InputEvent &> &sig_key_down();

	//: Called when a key is released on keyboard.
	CL_Signal_v1<const CL_InputEvent &> &sig_key_up();

	//: Called when a mousebutton is clicked.
	//: The CL_InputEvent contains the coordinates and which button was pressed
	CL_Signal_v1<const CL_InputEvent &> &sig_mouse_down();

	//: Called when a mousebutton is released.
	CL_Signal_v1<const CL_InputEvent &> &sig_mouse_up();

	//: Called when a mousebutton is doubleclicked.
	CL_Signal_v1<const CL_InputEvent &> &sig_mouse_dblclk();

	//: Called when the mouse is moved.
	CL_Signal_v1<const CL_InputEvent &> &sig_mouse_move();

	//: Called when mouse enters the component area.
	CL_Signal_v0 &sig_mouse_enter();

	//: Called when mouse leaves the component area.
	CL_Signal_v0 &sig_mouse_leave();

	//: Called when component gains focus.
	CL_Signal_v0 &sig_got_focus();

	//: Called when component lose focus.
	CL_Signal_v0 &sig_lost_focus();

	//: Called when component is closed.
	CL_Signal_v0 &sig_close();
	
	//: Called when component is resized.
	//: The two parameters are the OLD width and height of the component.
	CL_Signal_v2<int, int> &sig_resize();

	//: Called when component is moved.
	//: The two parameters are the OLD positions of the component.
	CL_Signal_v2<int, int> &sig_move();

	//: Called when a child component is about to be added to the component.
	CL_Signal_v1<CL_Component *> &sig_child_before_add();

	//: Called when a child component is added to the component.
	CL_Signal_v1<CL_Component *> &sig_child_add();

	//: Called when a child component is about to be removed from the component.
	CL_Signal_v1<CL_Component *> &sig_child_before_remove();
	
	//: Called when a child component is removed from the component.
	CL_Signal_v1<CL_Component *> &sig_child_remove();

	//: Called when needing to convert parent coordinates into coordinates used by this component.
	//- <p>This is used in 3D guis to convert from screen coordinates to gui coordinates.
	//- (project the screen cordinates down on the plane that the gui is laying on)</p>
	CL_Signal_v2<int &, int &> &sig_transform_coords();
	
	//: Called when needing to find the minimum size of the component.
	//: Normally used by the styles.
	CL_Signal_v1<CL_Size &> &sig_get_minimum_size();

	//: Called when needing to find the maximum size of the component.
	//: Normally used by the styles.
	CL_Signal_v1<CL_Size &> &sig_get_maximum_size();

	//: Called when needing to find the preferred size of the component.
	//: Normally used by the styles.
	CL_Signal_v1<CL_Size &> &sig_get_preferred_size();

private:
	void delete_child (CL_Component *child);

//! Implementation:
protected:
	CL_Component_Generic *impl;

	friend class CL_Component_Generic;
	friend class CL_GUIManager_Generic;
	friend class CL_ComponentManager_Generic;
};

#endif

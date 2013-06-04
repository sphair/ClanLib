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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include <memory>
#include "../Core/Signals/callback_0.h"
#include "../Core/Signals/callback_1.h"
#include "../Core/Signals/callback_v0.h"
#include "../Core/Signals/callback_v1.h"
#include "../Core/Signals/callback_v2.h"
#include "../Core/Signals/callback_v3.h"
#include "../Core/Signals/callback_2.h"
#include "../Core/Signals/signal_v1.h"
#include "../Core/IOData/file_system.h"
#include "../Display/Window/display_window_description.h"
#include "../Display/Window/display_window.h"
#include "../Display/Render/graphic_context.h"
#include "../Display/Window/input_context.h"
#include "../Display/2D/canvas.h"
#include "gui_layout.h"
#include <vector>

namespace clan
{

class DomDocument;
class Canvas;
class InputContext;
class InputEvent;
class Cursor;
class GUIMessage;
class GUIManager;
class GUITopLevelWindow;
class GUITheme;
class ResourceManager;
class GUIConsumedKeys;
class GUIComponent_Impl;
class CSSComputedValues;

typedef DisplayWindowDescription GUITopLevelDescription;

struct VerticalTextPosition
{
	float top;
	float baseline;
	float bottom;
};

/// \brief GUI base component class.
class CL_API_GUI GUIComponent
{
/// \name Construction
/// \{
public:
	/// \brief Creates a GUI component.
	GUIComponent(GUIComponent *parent, const std::string &tag_name = "component");

	/// \brief Constructs a GUIComponent
	///
	/// \param manager = GUIManager
	/// \param description = GUITop Level Description
	GUIComponent(GUIManager *manager, GUITopLevelDescription description, const std::string &tag_name = "component");

	/// \brief Constructs a GUIComponent
	///
	/// \param owner = GUIComponent
	/// \param description = GUITop Level Description
	GUIComponent(GUIComponent *owner, GUITopLevelDescription description, const std::string &tag_name = "component");

	virtual ~GUIComponent();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the position and size of the drawable area of the component relative to its parent component.
	///
	/// If there is no parent component this function returns the position relative to the window viewport (its client area).
	Rect get_geometry() const;

	/// \brief Returns the position and size of the viewport of the component window in screen coordinates.
	Rect get_viewport() const;

	/// \brief Returns the content box area relative to the component geometry.
	Rect get_content_box() const;

	/// \brief Returns the width of the drawable area of the component.
	int get_width() const;

	/// \brief Returns the height of the drawable area of the component.
	int get_height() const;

	/// \brief Returns the size of the drawable area of the component.
	Size get_size() const;

	/// \brief Returns the position and size of the component window (window frame, borders etc).
	Rect get_window_geometry() const;

	/// \brief Returns the CSS tag name
	std::string get_tag_name() const;

	/// \brief Returns the CSS id attribute
	std::string get_id() const;

	/// \brief Returns whether a CSS class is present or not.
	bool get_class(const std::string &name) const;

	/// \brief Returns whether a CSS pseudo class is present or not.
	bool get_pseudo_class(const std::string &name) const;

	/// \brief Returns all classes currently present
	std::vector<std::string> get_classes() const;

	/// \brief Returns all pseudo classes currently present
	std::vector<std::string> get_pseudo_classes() const;

	/// \brief Returns the standard W3C CSS properties active for this component
	const CSSComputedValues &get_css_values() const;

	/// \brief Returns true if the component has the focus.
	bool has_focus() const;

	/// \brief Returns true if the components size can be changed.
	bool get_allow_resize() const;

	/// \brief Returns true if the components children are clipped to the parent component when drawn.
	bool get_clip_children() const;

	/// \brief Focus policies.
	enum FocusPolicy
	{
		focus_refuse,
		focus_local,
		focus_group,
		focus_parent
	};

	/// \brief Returns the focus policy of the component.
	FocusPolicy get_focus_policy() const;

	/// \brief Returns the name of the component group, or an empty string if no group name has been set.
	std::string get_component_group_name() const;

	/// \brief Returns true if the component is the selected item in a group of components where FocusPolicy is focus_group.
	bool is_selected_in_group() const;

	/// \brief Returns true if this component, when focused, blocks the default action of its parent dialog.
	bool get_blocks_default_action() const;

	/// \brief Returns the resource manager for the GUI resources.
	ResourceManager get_resources() const;

	/// \brief Returns the GUI manager owning this component.
	GUIManager get_gui_manager() const;

	/// \brief Returns the theme used by this component.
	GUITheme get_theme() const;

	/// \brief Returns the parent component.
	/** <p>Only child components has a parent.</p>*/
	const GUIComponent *get_parent_component() const;

	/// \brief Get Parent component
	///
	/// \return parent_component
	GUIComponent *get_parent_component();

	/// \brief Return the component owning this component.
	/** <p>Only top-level components has an owner.</p>*/
	const GUIComponent *get_owner_component() const;

	/// \brief Get Owner component
	///
	/// \return owner_component
	GUIComponent *get_owner_component();

	/// \brief Returns a list of the child components.
	std::vector<GUIComponent*> get_child_components() const;

	/// \brief Returns the first child component.
	const GUIComponent *get_first_child() const;

	/// \brief Get First child
	///
	/// \return first_child
	GUIComponent *get_first_child();

	/// \brief Returns the last child component.
	const GUIComponent *get_last_child() const;

	/// \brief Returns the next component in the component tree, or 'this', if none found.
	GUIComponent *get_next_component_in_tree();

	/// \brief Returns the previous component in the component tree, or 'this', if none found.
	GUIComponent *get_previous_component_in_tree();

	/// \brief Get Last child
	///
	/// \return last_child
	GUIComponent *get_last_child();

	/// \brief Returns a list of child components belonging to the specified group.
	std::vector<GUIComponent*> get_child_component_group(const std::string &group_name) const;

	/// \brief Returns the selected item in a component group of which this component is a member.
	GUIComponent *get_group_selected_component();

	/// \brief Find child component with the specified component ID name.
	GUIComponent *get_named_item(const std::string &id);

	/// \brief Returns true if the component has any child components.
	bool has_child_components() const;

	/// \brief Returns the previous sibling component.
	const GUIComponent *get_previous_sibling() const;

	/// \brief Get Previous sibling
	///
	/// \return previous_sibling
	GUIComponent *get_previous_sibling();

	/// \brief Returns the next sibling component.
	const GUIComponent *get_next_sibling() const;

	/// \brief Get Next sibling
	///
	/// \return next_sibling
	GUIComponent *get_next_sibling();

	/// \brief Checks if this component is a descendant of target component.
	///
	/// \return true if target component is descendant, false otherwise.
	bool is_descendant_of(GUIComponent *component);

	/// \brief Checks if this component is the ancestor of target component.
	///
	/// \return true if target component is ancestor, false otherwise.
	bool is_ancestor_of(GUIComponent *component);

	/// \brief Returns the top level component this GUIComponent is a child of.
	const GUIComponent *get_top_level_component() const;

	/// \brief Returns the top level window this GUIComponent belongs to.
	GUITopLevelWindow *get_top_level_window() const;

	/// \brief Get Top level component
	///
	/// \return top_level_component
	GUIComponent *get_top_level_component();

	/// \brief Return the graphic context for the component.
	Canvas get_canvas() const;

	/// \brief Return the input context for the component.
	InputContext get_ic();

	/// \brief Return true if the component, and all its parents are enabled.
	bool is_enabled() const;

	/// \brief Return true if the component, and all its parents are visible.
	bool is_visible() const;

	/// \brief Returns true if the component is active.
	bool is_active() const;

	/// \brief Return the component under 'point', in local viewport coordinates.
	GUIComponent *get_component_at(const Point &point);

	/// \brief Returns the preferred content width
	///
	/// Override this function if the component has non-css content.
	virtual float get_preferred_content_width();

	/// \brief Returns the preferred content height for the specified content width
	///
	/// Override this function if the component has non-css content.
	virtual float get_preferred_content_height(float width);

	/// \brief Convert the top-level window client coordinates to component coordinates.
	Point window_to_component_coords(const Point &window_point) const;

	/// \brief Convert the top-level window client coordinates to component coordinates.
	Rect window_to_component_coords(const Rect &window_rect) const;

	/// \brief Convert the component coordinates to top-level window client coordinates.
	Point component_to_window_coords(const Point &component_point) const;

	/// \brief Convert the component coordinates to top-level window client coordinates.
	Rect component_to_window_coords(const Rect &component_rect) const;

	/// \brief Convert the screen coordinates to component coordinates.
	Point screen_to_component_coords(const Point &screen_point) const;

	/// \brief Convert the component coordinates to screen coordinates.
	Point component_to_screen_coords(const Point &component_point) const;

	/// \brief Returns the layout manager set for this component. Check GUILayout.is_null() if none set.
	GUILayout get_layout() const;

	/// \brief Returns the display window in which this component is hosted.
	DisplayWindow get_display_window() const;

	/// \brief Returns true if this component will receive unhandled enter/return keypress messages.
	bool is_default();

	/// \brief Returns true if this component will receive unhandled escape keypress messages.
	bool is_cancel();

	/// \brief Returns true if this component will generate double click messages.
	bool is_double_click_enabled() const;

	/// \brief Enabled whether the GUI will constantly repaint this component when there are no other messages to process
	bool get_constant_repaint() const;

	/// \brief Gets the font
	Font get_font() const;

	/// \brief Calculates the text box using the font defined for the current state for use with render_text().
	Rect get_render_text_box(Canvas &canvas, const std::string &str) const;

	/// \brief Calculates the text size using the font defined for the current state for use with render_text().
	Size get_render_text_size(Canvas &canvas, const std::string &str) const;

	/// \brief Calculates the text size using the font defined for the current state for use with render_text_span().
	Rect get_render_text_span_box(Canvas &canvas, const std::string &str, const Rect &content_rect) const;
	
	/// \brief Returns the content shrink box of the theme part.
	Rect get_content_shrink_box() const;

	/// \brief Returns the value of a property.
	std::string get_property(const std::string &property, const std::string &default_value) const;

	/// \brief Returns the value of a property as an integer.
	int get_property_int(const std::string &property, const std::string &default_value) const;

/// \}
/// \name Events
/// \{
public:
	/// \brief void func_render(Canvas &canvas, const Rect &clip_rect)
	Callback_v2<Canvas &, const Rect &> &func_render();

	/// \brief void func_process_message(std::shared_ptr<GUIMessage> &message)
	Callback_v1<std::shared_ptr<GUIMessage> &> &func_process_message();

	/// \brief bool func_close()
	Callback_0<bool> &func_close();

	/// \brief bool func_activated()
	Callback_0<bool> &func_activated();

	/// \brief bool func_deactivated()
	Callback_0<bool> &func_deactivated();

	/// \brief bool func_focus_lost()
	Callback_0<bool> &func_focus_lost();

	/// \brief bool func_focus_gained()
	Callback_0<bool> &func_focus_gained();

	/// \brief bool func_pointer_enter()
	Callback_0<bool> &func_pointer_enter();

	/// \brief bool func_pointer_exit()
	Callback_0<bool> &func_pointer_exit();

	/// \brief Lets a component filter the messages intended for another component. Consumed messages will not reach the original target component.
	Callback_v1<std::shared_ptr<GUIMessage> &> &func_filter_message();
/*
	/// \brief Allows a component to override CSS properties
	Callback_v1<CSSComputedBox &> &func_apply_properties();
*/
	/// \brief bool func_input(const InputEvent &input_event)
	Callback_1<bool, const InputEvent &> &func_input();

	/// \brief bool func_input_pressed(const InputEvent &input_event)
	Callback_1<bool, const InputEvent &> &func_input_pressed();

	/// \brief bool func_input_released(const InputEvent &input_event)
	Callback_1<bool, const InputEvent &> &func_input_released();

	/// \brief bool func_input_doubleclick(const InputEvent &input_event)
	Callback_1<bool, const InputEvent &> &func_input_doubleclick();

	/// \brief bool func_input_pointer_moved(const InputEvent &input_event)
	Callback_1<bool, const InputEvent &> &func_input_pointer_moved();

	/// \brief bool func_visibility_change()
	Callback_v1<bool> &func_visibility_change();

	/// \brief void sig_style_changed()
	Signal_v0 &sig_style_changed();

	/// \brief void func_enablemode_changed()
	Callback_v0 &func_enablemode_changed();

	/// \brief Callback invoked when the component is resized, i.e. when set_geometry is called.
	Callback_v0 &func_resized();

	/// \brief void func_constrain_resize(Rect &geometry)
	Callback_v1<Rect &> &func_constrain_resize();

	/// \brief Callback invoked when loading a custom component from XML.
	/** The type of the component to create is passed as a parameter to the function.
	    The callback function should create the desired component and return it, or return 0, if no component is created.
	    Example definition: GUIComponent *Foo::on_create_custom_component(GUIComponent *parent, std::string type);*/
	virtual Callback_2<GUIComponent*, GUIComponent*, std::string> &func_create_custom_component();

/// \}
/// \name Operations
/// \{
public:
	/// \brief Renders the component and its children.
	void render(Canvas &canvas, const Rect &clip_rect, bool include_children = true);

	/// \brief Paints the component.
	/** <p>This function renders all components affecting the component
	    or area specified and makes the changes visible.</p>*/
	void paint();

	/// \brief Paint
	///
	/// \param clip_rect = Rect
	void paint(const Rect &clip_rect);

	/// \brief Run component in modal mode.
	int exec();

	/// \brief Break message pump.
	/** <p>This function causes GUIComponent::exec or GUIManager::exec to
	    exit with the given exit code.</p>*/
	void exit_with_code(int exit_code);

	/// \brief Set manual component position and size.
	void set_geometry(Rect geometry);

	/// \brief Reset back to automatic layout
	void reset_geometry();

	/// \brief Set component window position and size
	void set_window_geometry(Rect geometry, bool client_area = false);

	/// \brief Sets the CSS tag name
	void set_tag_name(const std::string &name); 

	/// \brief Controls the presence of a CSS class
	///
	/// \return true if changed
	bool set_class(const std::string &name, bool enable);

	/// \brief Sets the CSS id
	void set_id(const std::string &name);

	/// \brief Controls the presence of a CSS pseudo class
	///
	/// \return true if changed
	bool set_pseudo_class(const std::string &name, bool enable);

	/// \brief Re-evaluates the geometry of the component
	void update_layout();

	/// \brief Sets the component as enabled or disabled.
	void set_enabled(bool enable = true);

	/// \brief Sets the components children to be clipped to the parent component when drawn. 
	void set_clip_children(bool clip = true, const Rect &clip_rect=Rect(0,0,0,0));

	/// \brief Sets the component as visible or hidden.
	///
	/// \param visible = Visible state
	/// \param activate_root_win = In case the component is a root window, this parameter controls whether the window is activated.
	void set_visible(bool visible = true, bool activate_root_win=true);

	/// \brief Set this component focus.
	///
	/// \param enable = true = To set the focus. false = Loose the focus (if in focus)
	void set_focus(bool enable = true);

	/// \brief Make all mouse messages be redirected to this component.
	void capture_mouse(bool capture);

	/// \brief Make tablet proximity messages be redirected to this component.
	void capture_proximity(bool capture);

	/// \brief Sets the focus policy of the component.
	void set_focus_policy(FocusPolicy policy);

	/// \brief Modifies whether this component will generate double click messages or not.
	void set_double_click_enabled(bool enable);

	/// \brief Set group name
	///
	/// \param str = String Ref
	void set_component_group_name(const std::string &str);

	/// \brief Set to true if this component is the selected item in a group of components where FocusPolicy is focus_group.
	void set_selected_in_component_group(bool selected);

	/// \brief Create child components from a GUI definition file.
	void create_components(const DomDocument &gui_xml);

	/// \brief Create components
	///
	/// \param fullname = String Ref
	void create_components(const std::string &fullname);

	/// \brief Create components
	///
	/// \param file = IODevice
	void create_components(IODevice &file);

	/// \brief Create components
	///
	/// \param filename = String Ref
	/// \param dir = Virtual Directory
	void create_components(const std::string &filename, const FileSystem &fs);

	// Request an asynchronous redraw of the specified area.
	void request_repaint();

	/// \brief Request repaint
	///
	/// \param rect = Rect
	void request_repaint(Rect rect);

	/// \brief Set a clipping rectangle.
	void set_cliprect(Canvas &canvas, const Rect &rect);

	/// \brief Reset the clipping rectangle.
	void reset_cliprect(Canvas &canvas);

	/// \brief Push a clipping rectangle.
	void push_cliprect(Canvas &canvas, const Rect &rect);

	/// \brief Pop a clipping rectangle.
	void pop_cliprect(Canvas &canvas);

	/// \brief Deletes all child components.
	void delete_child_components();

	/// \brief Makes this component a child of the given component, removing it from the previous parent.
	void set_parent_component(GUIComponent *new_parent);

	/// \brief Set a layout on the component.
	void set_layout(GUILayout &layout);

	/// \brief Sets the current cursor icon.
	void set_cursor(const Cursor &cursor);

	/// \brief Set cursor
	///
	/// \param type = StandardCursor
	void set_cursor(enum StandardCursor type);

	/// \brief Set focus to the next component in tab order.
	void focus_next();

	/// \brief Set focus to the previous component in tab order.
	void focus_previous();

	/// \brief When set to true, this component will receive unhandled enter/return keypress messages.
	/** <p>If multiple components are set as 'default' handlers, the first child with the property will receive the keypress message.</p>*/
	void set_default(bool value);

	/// \brief When set to true, this component will receive unhandled escape keypress messages.
	/** <p>If multiple components are set as 'cancel' handlers, the first child with the property will receive the keypress message.</p>*/
	void set_cancel(bool value);

	/// \brief Set to true if this component, when focused, blocks the default action of the parent dialog.
	void set_blocks_default_action(bool block);

	/// \brief Enabled whether the GUI will constantly repaint this component when there are no other messages to process
	void set_constant_repaint(bool enable);

	Rect render_text_span(Canvas &canvas, const std::string &text, const Rect &content_rect);
	Rect render_text(Canvas &canvas, const std::string &text);
	Rect render_text(Canvas &canvas, const std::string &text, int xpos, int baseline);

	/// \brief Calculates the vertical text position for the current font
	VerticalTextPosition get_vertical_text_align(Canvas &canvas);

/// \}
/// \name Implementation
/// \{
public:
	std::shared_ptr<GUIComponent_Impl> impl;

private:
	/// \brief Constructs a GUIComponent
	///
	/// \param other = GUIComponent
	GUIComponent(GUIComponent &other);

	GUIComponent &operator =(const GUIComponent &other);

	Canvas dummy_canvas;
	InputContext dummy_ic;
/// \}
};

}

/// \}

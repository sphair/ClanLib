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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"
#include "../Core/Signals/callback_0.h"
#include "../Core/Signals/callback_1.h"
#include "../Core/Signals/callback_v0.h"
#include "../Core/Signals/callback_v1.h"
#include "../Core/Signals/callback_v2.h"
#include "../Core/Signals/callback_v3.h"
#include "../Core/Signals/callback_2.h"
#include "../Core/IOData/virtual_directory.h"
#include "../Display/Window/display_window_description.h"
#include "../Display/Window/display_window.h"
#include "../Display/Render/graphic_context.h"
#include "../Display/Window/input_context.h"
#include "gui_layout.h"
#include <vector>

class CL_DomDocument;
class CL_GraphicContext;
class CL_InputContext;
class CL_InputEvent;
class CL_Cursor;
class CL_GUIMessage;
class CL_GUIManager;
class CL_GUITheme;
class CL_ResourceManager;
class CL_GUIConsumedKeys;
class CL_GUIComponent_Impl;

typedef CL_DisplayWindowDescription CL_GUITopLevelDescription;

/// \brief GUI base component class.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIComponent
{
/// \name Construction
/// \{
public:
	/// \brief Creates a GUI component.
	CL_GUIComponent(CL_GUIComponent *parent);

	/// \brief Constructs a GUIComponent
	///
	/// \param manager = GUIManager
	/// \param description = GUITop Level Description
	CL_GUIComponent(CL_GUIManager *manager, CL_GUITopLevelDescription description);

	/// \brief Constructs a GUIComponent
	///
	/// \param owner = GUIComponent
	/// \param description = GUITop Level Description
	CL_GUIComponent(CL_GUIComponent *owner, CL_GUITopLevelDescription description);

	virtual ~CL_GUIComponent();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the position and size of the drawable area of the component.
	CL_Rect get_geometry() const;

	/// \brief Returns the width of the drawable area of the component.
	int get_width() const;

	/// \brief Returns the height of the drawable area of the component.
	int get_height() const;

	/// \brief Returns the size of the drawable area of the component.
	CL_Size get_size() const;

	/// \brief Returns the position and size of the component window (window frame, borders etc).
	CL_Rect get_window_geometry() const;

	/// \brief Returns the component type name.
	CL_StringRef get_type_name() const;

	/// \brief Returns the component class name.
	CL_StringRef get_class_name() const;

	/// \brief Returns the component ID name.
	CL_StringRef get_id_name() const;

	/// \brief Returns the CSS element name for the component.
	CL_StringRef get_element_name() const;

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
	CL_StringRef get_component_group_name() const;

	/// \brief Returns true if the component is the selected item in a group of components where FocusPolicy is focus_group.
	bool is_selected_in_group() const;

	/// \brief Returns true if this component, when focused, blocks the default action of its parent dialog.
	bool get_blocks_default_action() const;

	/// \brief Returns the resource manager for the GUI resources.
	CL_ResourceManager get_resources() const;

	/// \brief Returns the GUI manager owning this component.
	CL_GUIManager get_gui_manager() const;

	/// \brief Returns the theme used by this component.
	CL_GUITheme get_theme() const;

	/// \brief Returns the parent component.
	/** <p>Only child components has a parent.</p>*/
	const CL_GUIComponent *get_parent_component() const;

	/// \brief Get Parent component
	///
	/// \return parent_component
	CL_GUIComponent *get_parent_component();

	/// \brief Return the component owning this component.
	/** <p>Only top-level components has an owner.</p>*/
	const CL_GUIComponent *get_owner_component() const;

	/// \brief Get Owner component
	///
	/// \return owner_component
	CL_GUIComponent *get_owner_component();

	/// \brief Returns a list of the child components.
	std::vector<CL_GUIComponent*> get_child_components() const;

	/// \brief Returns the first child component.
	const CL_GUIComponent *get_first_child() const;

	/// \brief Get First child
	///
	/// \return first_child
	CL_GUIComponent *get_first_child();

	/// \brief Returns the last child component.
	const CL_GUIComponent *get_last_child() const;

	/// \brief Returns the next component in the component tree, or 'this', if none found.
	CL_GUIComponent *get_next_component_in_tree();

	/// \brief Returns the previous component in the component tree, or 'this', if none found.
	CL_GUIComponent *get_previous_component_in_tree();

	/// \brief Get Last child
	///
	/// \return last_child
	CL_GUIComponent *get_last_child();

	/// \brief Returns a list of child components belonging to the specified group.
	std::vector<CL_GUIComponent*> get_child_component_group(const CL_String &group_name) const;

	/// \brief Returns the selected item in a component group of which this component is a member.
	CL_GUIComponent *get_group_selected_component();

	/// \brief Find child component with the specified component ID name.
	CL_GUIComponent *get_named_item(const CL_StringRef &id);

	/// \brief Returns true if the component has any child components.
	bool has_child_components() const;

	/// \brief Returns the previous sibling component.
	const CL_GUIComponent *get_previous_sibling() const;

	/// \brief Get Previous sibling
	///
	/// \return previous_sibling
	CL_GUIComponent *get_previous_sibling();

	/// \brief Returns the next sibling component.
	const CL_GUIComponent *get_next_sibling() const;

	/// \brief Get Next sibling
	///
	/// \return next_sibling
	CL_GUIComponent *get_next_sibling();

	/// \brief Checks if this component is a descendant of target component.
	///
	/// \return true if target component is descendant, false otherwise.
	bool is_descendant_of(CL_GUIComponent *component);

	/// \brief Checks if this component is the ancestor of target component.
	///
	/// \return true if target component is ancestor, false otherwise.
	bool is_ancestor_of(CL_GUIComponent *component);

	/// \brief Returns the top level component this CL_GUIComponent is a child of.
	const CL_GUIComponent *get_top_level_component() const;

	/// \brief Get Top level component
	///
	/// \return top_level_component
	CL_GUIComponent *get_top_level_component();

	/// \brief Return the graphic context for the component.
	CL_GraphicContext& get_gc();

	/// \brief Return the input context for the component.
	CL_InputContext& get_ic();

	/// \brief Return true if the component, and all its parents are enabled.
	bool is_enabled() const;

	/// \brief Return true if the component, and all its parents are visible.
	bool is_visible() const;

	/// \brief Returns true if the component is active.
	bool is_active() const;

	/// \brief Return the component under 'point', in local viewport coordinates.
	CL_GUIComponent *get_component_at(const CL_Point &point);

	/// \brief Returns the preferred size of a render box.
	virtual CL_Size get_preferred_size() const;

	/// \brief Returns the preferred width of a render box.
	int get_preferred_width() const;

	/// \brief Returns the preferred height of a render box.
	int get_preferred_height() const;

	/// \brief Convert the top-level window client coordinates to component coordinates.
	CL_Point window_to_component_coords(const CL_Point &window_point) const;

	/// \brief Convert the top-level window client coordinates to component coordinates.
	CL_Rect window_to_component_coords(const CL_Rect &window_rect) const;

	/// \brief Convert the component coordinates to top-level window client coordinates.
	CL_Point component_to_window_coords(const CL_Point &component_point) const;

	/// \brief Convert the component coordinates to top-level window client coordinates.
	CL_Rect component_to_window_coords(const CL_Rect &component_rect) const;

	/// \brief Convert the screen coordinates to component coordinates.
	CL_Point screen_to_component_coords(const CL_Point &screen_point) const;

	/// \brief Convert the component coordinates to screen coordinates.
	CL_Point component_to_screen_coords(const CL_Point &component_point) const;

	/// \brief Returns the layout manager set for this component. Check CL_GUILayout.is_null() if none set.
	CL_GUILayout get_layout() const;

	/// \brief Returns the display window in which this component is hosted.
	CL_DisplayWindow get_display_window() const;

	/// \brief Returns true if this component will receive unhandled enter/return keypress messages.
	bool is_default();

	/// \brief Returns true if this component will receive unhandled escape keypress messages.
	bool is_cancel();

	/// \brief Enabled whether the GUI will constantly repaint this component when there are no other messages to process
	bool get_constant_repaint() const;

/// \}
/// \name Events
/// \{
public:
	/// \brief void func_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
	CL_Callback_v2<CL_GraphicContext &, const CL_Rect &> &func_render();

	/// \brief void func_process_message(const CL_GUIMessage &message)
	CL_Callback_v1<CL_GUIMessage &> &func_process_message();

	/// \brief bool func_close()
	CL_Callback_0<bool> &func_close();

	/// \brief bool func_activated()
	CL_Callback_0<bool> &func_activated();

	/// \brief bool func_deactivated()
	CL_Callback_0<bool> &func_deactivated();

	/// \brief bool func_focus_lost()
	CL_Callback_0<bool> &func_focus_lost();

	/// \brief bool func_focus_gained()
	CL_Callback_0<bool> &func_focus_gained();

	/// \brief bool func_pointer_enter()
	CL_Callback_0<bool> &func_pointer_enter();

	/// \brief bool func_pointer_exit()
	CL_Callback_0<bool> &func_pointer_exit();

	/// \brief Lets a component filter the messages intended for another component. Consumed messages will not reach the original target component.
	CL_Callback_v1<CL_GUIMessage&> &func_filter_message();

	/// \brief bool func_input(const CL_InputEvent &input_event)
	CL_Callback_1<bool, const CL_InputEvent &> &func_input();

	/// \brief bool func_input_pressed(const CL_InputEvent &input_event)
	CL_Callback_1<bool, const CL_InputEvent &> &func_input_pressed();

	/// \brief bool func_input_released(const CL_InputEvent &input_event)
	CL_Callback_1<bool, const CL_InputEvent &> &func_input_released();

	/// \brief bool func_input_doubleclick(const CL_InputEvent &input_event)
	CL_Callback_1<bool, const CL_InputEvent &> &func_input_doubleclick();

	/// \brief bool func_input_pointer_moved(const CL_InputEvent &input_event)
	CL_Callback_1<bool, const CL_InputEvent &> &func_input_pointer_moved();

	/// \brief bool func_visibility_change()
	CL_Callback_v1<bool> &func_visibility_change();

	/// \brief void func_style_changed()
	CL_Callback_v0 &func_style_changed();

	/// \brief void func_enablemode_changed()
	CL_Callback_v0 &func_enablemode_changed();

	/// \brief Callback invoked when the component is resized, i.e. when set_geometry is called.
	CL_Callback_v0 &func_resized();

	/// \brief void func_constrain_resize(CL_Rect &geometry)
	CL_Callback_v1<CL_Rect &> &func_constrain_resize();

	/// \brief Callback invoked when loading a custom component from XML.
	/** The type of the component to create is passed as a parameter to the function.
	    The callback function should create the desired component and return it, or return 0, if no component is created.
	    Example definition: CL_GUIComponent *Foo::on_create_custom_component(CL_GUIComponent *parent, CL_String type);*/
	virtual CL_Callback_2<CL_GUIComponent*, CL_GUIComponent*, CL_String> &func_create_custom_component();

/// \}
/// \name Operations
/// \{
public:
	/// \brief Renders the component and its children.
	void render(CL_GraphicContext &gc, const CL_Rect &clip_rect, bool include_children = true);

	/// \brief Paints the component.
	/** <p>This function renders all components affecting the component
	    or area specified and makes the changes visible.</p>*/
	void paint();

	/// \brief Paint
	///
	/// \param clip_rect = Rect
	void paint(const CL_Rect &clip_rect);

	/// \brief Run component in modal mode.
	int exec();

	/// \brief Break message pump.
	/** <p>This function causes CL_GUIComponent::exec or CL_GUIManager::exec to
	    exit with the given exit code.</p>*/
	void exit_with_code(int exit_code);

	/// \brief Set component position and size.
	void set_geometry(CL_Rect geometry);

	/// \brief Set component window position and size
	void set_window_geometry(CL_Rect geometry);

	/// \brief Sets the component type name. (csstype.cssclass#cssid)
	void set_type_name(const CL_StringRef &name); 

	/// \brief Sets the component class name. (csstype.cssclass#cssid)
	void set_class_name(const CL_StringRef &name);

	/// \brief Sets the component ID name. (csstype.cssclass#cssid)
	void set_id_name(const CL_StringRef &name);

	/// \brief Sets the component as enabled or disabled.
	void set_enabled(bool enable = true);

	/// \brief Sets the components children to be clipped to the parent component when drawn. 
	void set_clip_children(bool clip = true, const CL_Rect &clip_rect=CL_Rect(0,0,0,0));

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

	/// \brief Set group name
	///
	/// \param str = String Ref
	void set_component_group_name(const CL_StringRef &str);

	/// \brief Set to true if this component is the selected item in a group of components where FocusPolicy is focus_group.
	void set_selected_in_component_group(bool selected);

	/// \brief Create child components from a GUI definition file.
	void create_components(const CL_DomDocument &gui_xml);

	/// \brief Create components
	///
	/// \param fullname = String Ref
	void create_components(const CL_StringRef &fullname);

	/// \brief Create components
	///
	/// \param file = IODevice
	void create_components(CL_IODevice &file);

	/// \brief Create components
	///
	/// \param filename = String Ref
	/// \param dir = Virtual Directory
	void create_components(const CL_StringRef &filename, const CL_VirtualDirectory &dir);

	// Request an asynchronous redraw of the specified area.
	void request_repaint();

	/// \brief Request repaint
	///
	/// \param rect = Rect
	void request_repaint(CL_Rect rect);

	/// \brief Set a clipping rectangle.
	void set_cliprect(CL_GraphicContext &gc, const CL_Rect &rect);

	/// \brief Reset the clipping rectangle.
	void reset_cliprect(CL_GraphicContext &gc);

	/// \brief Push a clipping rectangle.
	void push_cliprect(CL_GraphicContext &gc, const CL_Rect &rect);

	/// \brief Pop a clipping rectangle.
	void pop_cliprect(CL_GraphicContext &gc);

	/// \brief Deletes all child components.
	void delete_child_components();

	/// \brief Makes this component a child of the given component, removing it from the previous parent.
	void set_parent_component(CL_GUIComponent *new_parent);

	/// \brief Set a layout on the component.
	void set_layout(CL_GUILayout &layout);

	/// \brief Sets the current cursor icon.
	void set_cursor(const CL_Cursor &cursor);

	/// \brief Set cursor
	///
	/// \param type = CL_StandardCursor
	void set_cursor(enum CL_StandardCursor type);

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

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_GUIComponent_Impl> impl;

	/// \brief Constructs a GUIComponent
	///
	/// \param other = GUIComponent
	CL_GUIComponent(CL_GUIComponent &other);

	CL_GUIComponent &operator =(const CL_GUIComponent &other);

	CL_GraphicContext dummy_gc;
	CL_InputContext dummy_ic;

	friend class CL_GUIManager_Impl;

	friend class CL_GUIComponent_Impl;

	friend class CL_Window;

	friend class CL_Window_Impl;
/// \}
};

/// \}

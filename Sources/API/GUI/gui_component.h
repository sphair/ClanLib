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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"
#include "../Core/Signals/callback_v0.h"
#include "../Core/Signals/callback_v1.h"
#include "../Core/Signals/callback_v2.h"
#include "../Core/Signals/callback_v3.h"
#include "../Core/Signals/callback_2.h"
#include "../Core/IOData/virtual_directory.h"
#include "../Display/Window/display_window_description.h"
#include "../Display/Window/timer.h"
#include "gui_layout.h"
#include <vector>
#include "../Display/Window/display_window.h"

class CL_DomDocument;
class CL_GraphicContext;
class CL_InputContext;
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

	CL_GUIComponent(const CL_Rect &position, CL_GUIManager *manager, CL_GUITopLevelDescription description, bool temporary=false);

	CL_GUIComponent(const CL_Rect &position, CL_GUIComponent *owner, CL_GUITopLevelDescription description);

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
		focus_parent
	};

	/// \brief Returns the focus policy of the component.
	FocusPolicy get_focus_policy() const;

	/// \brief Returns the keys the component consumes as an OR'ed integer. Bit values from ConsumedKeys enum.
	CL_GUIConsumedKeys get_consumed_keys() const;

	/// \brief Returns the resource manager for the GUI resources.
	CL_ResourceManager get_resources() const;

	/// \brief Returns the GUI manager owning this component.
	CL_GUIManager get_gui_manager() const;

	/// \brief Returns the theme used by this component.
	CL_GUITheme *get_theme() const;

	/// \brief Returns the parent component.
	/** <p>Only child components has a parent.</p>*/
	const CL_GUIComponent *get_parent_component() const;

	CL_GUIComponent *get_parent_component();

	/// \brief Return the component owning this component.
	/** <p>Only top-level components has an owner.</p>*/
	const CL_GUIComponent *get_owner_component() const;

	CL_GUIComponent *get_owner_component();

	/// \brief Returns a list of the child components.
	std::vector<CL_GUIComponent *> get_child_components() const;

	/// \brief Returns the first child component.
	const CL_GUIComponent *get_first_child() const;

	CL_GUIComponent *get_first_child();

	/// \brief Returns the last child component.
	const CL_GUIComponent *get_last_child() const;

	CL_GUIComponent *get_last_child();

	/// \brief Find child component with the specified component ID name.
	CL_GUIComponent *get_named_item(const CL_StringRef &id);

	/// \brief Returns true if the component has any child components.
	bool has_child_components() const;

	/// \brief Returns the previous sibling component.
	const CL_GUIComponent *get_previous_sibling() const;

	CL_GUIComponent *get_previous_sibling();

	/// \brief Returns the next sibling component.
	const CL_GUIComponent *get_next_sibling() const;

	CL_GUIComponent *get_next_sibling();

	/// \brief Returns the top level component this CL_GUIComponent is a child of.
	const CL_GUIComponent *get_top_level_component() const;

	CL_GUIComponent *get_top_level_component();

	/// \brief Return the graphic context for the component.
	CL_GraphicContext get_gc() const;

	/// \brief Return the input context for the component.
	CL_InputContext get_ic() const;

	/// \brief Return true if the component, and all its parents are enabled.
	bool is_enabled() const;

	/// \brief Return true if the component is in click through mode.
	bool is_clickthrough() const;

	/// \brief Return true if the component, and all its parents are visible.
	bool is_visible() const;

	/// \brief Returns true if the component is active.
	bool is_active() const;

	/// \brief Returns true if this component manages the tab order focus of its children.
	bool is_tab_order_controller() const;

	/// \brief Get position of component in tab order loop.
	/** <p>The index of the first component is 0.
	    The index of a component not in the loop is -1</p>*/
	int get_tab_order() const;

	/// \brief Return the component under 'point', in window coordinates.
	CL_GUIComponent *get_component_at(const CL_Point &point);

	/// \brief Returns the preferred size of a render box.
	virtual CL_Size get_preferred_size() const;

	/// \brief Returns the preferred width of a render box.
	int get_preferred_width() const;

	/// \brief Returns the preferred height of a render box.
	int get_preferred_height() const;

	/// \brief Convert the top-level window client coordinates to component coordinates.
	CL_Point window_to_component_coords(const CL_Point &window_point) const;

	/// \brief Convert the component coordinates to top-level window client coordinates.
	CL_Point component_to_window_coords(const CL_Point &component_point) const;

	/// \brief Convert the screen coordinates to component coordinates.
	CL_Point screen_to_component_coords(const CL_Point &screen_point) const;

	/// \brief Convert the component coordinates to screen coordinates.
	CL_Point component_to_screen_coords(const CL_Point &component_point) const;

	/// \brief Returns the layout manager set for this component. Returns 0 if none set.
	CL_GUILayout *get_layout() const;

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

	/// \brief void func_style_changed()
	CL_Callback_v0 &func_style_changed();

	/// \brief void func_enablemode_changed()
	CL_Callback_v0 &func_enablemode_changed();

	/// \brief Callback invoked when the component is resized, i.e. when set_geometry is called.
	CL_Callback_v0 &func_resized();

	CL_Callback_v1<CL_Rect &> &func_constrain_resize();

	/// \brief Callback invoked when loading a custom component from XML.
	/** The type of the component to create is passed as a paramter to the function.
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

	void paint(const CL_Rect &clip_rect);

	/// \brief Run component in modal mode.
	int exec();

	/// \brief Break message pump.
	/** <p>This function causes CL_GUIComponent::exec or CL_GUIManager::exec to
	    exit with the given exit code.</p>*/
	void exit_with_code(int exit_code);

	/// \brief Post GUI message onto the message queue.
	/** <p>This function is thread safe.</p>*/
	void post_message(const CL_GUIMessage &message);

	/// \brief Send GUI message directly to the target.
	void send_message(CL_GUIMessage &message);

	/// \brief Set component position and size.
	void set_geometry(const CL_Rect &geometry);

	/// \brief Set component window position and size
	void set_window_geometry(const CL_Rect &geometry, bool client_area=false);

	/// \brief Sets the component type name. (csstype.cssclass#cssid)
	void set_type_name(const CL_StringRef &name); 

	/// \brief Sets the component class name. (csstype.cssclass#cssid)
	void set_class_name(const CL_StringRef &name);

	/// \brief Sets the component ID name. (csstype.cssclass#cssid)
	void set_id_name(const CL_StringRef &name);

	/// \brief Sets the component as enabled or disabled.
	void set_enabled(bool enable = true);

	/// \brief Sets the component to not receive mouse events, but let the underlying components handle them.
	void set_clickthrough(bool enabled = true);

	/// \brief Sets the components children to be clipped to the parent component when drawn. 
	void set_clip_children(bool clip = true);

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

	/// \brief Create child components from a GUI definition file.
	void create_components(const CL_DomDocument &gui_xml);

	void create_components(const CL_StringRef &filename, CL_VirtualDirectory dir = CL_VirtualDirectory());

	// Request an asynchronous redraw of the specified area.
	void invalidate_rect();

	void invalidate_rect(CL_Rect rect);

	CL_Timer create_timer();

	/// \brief Set a clipping rectangle.
	void set_cliprect(const CL_Rect &rect);

	/// \brief Reset the clipping rectangle.
	void reset_cliprect();

	/// \brief Deletes all child components.
	void delete_child_components();

	/// \brief Makes this component a child of the given component, removing it from the previous parent.
	void set_parent_component(CL_GUIComponent *new_parent);

	/// \brief Set a layout on the component.
	void set_layout(CL_GUILayout *layout);

	/// \brief Sets the current cursor icon.
	void set_cursor(const CL_Cursor &cursor);

	void set_cursor(enum CL_StandardCursor type);

	/// \brief Makes this component handle focus switching of its child components when the tab key is pressed.
	void set_tab_order_controller(bool enabled);

	/// \brief Sets the position of the component in the tab order loop. Indexes must be sequential for proper operation.
	void set_tab_order(int index);

	/// \brief Set focus to the next component in tab order.
	void focus_next();

	/// \brief Set focus to the previous component in tab order.
	void focus_previous();

	/// \brief Returns 'this' or the parent component that has been set to a tab order controller.
	/** <p>Returns null if no parent handles tab order.</p>*/
	CL_GUIComponent *get_tab_order_controller();

	/// \brief Returns the child component with the requested tab order index.
	/** <p>Searches for the tab order controller of 'this' component if no 'comp' parameter specified.*/
	CL_GUIComponent *get_tab_order_component(int index, CL_GUIComponent *comp=0);

	/// \brief When set to true, this component will receive unhandled enter/return keypress messages.
	/** <p>If multiple components are set as 'default' handlers, the first child with the property will receive the keypress message.</p>*/
	void set_default(bool value);

	/// \brief When set to true, this component will receive unhandled escape keypress messages.
	/** <p>If multiple components are set as 'cancel' handlers, the first child with the property will receive the keypress message.</p>*/
	void set_cancel(bool value);

	void set_consumed_keys(CL_GUIConsumedKeys &keys);

	/// \brief Enabled whether the GUI will constantly repaint this component when there are no other messages to process
	void set_constant_repaint(bool enable);

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_GUIComponent_Impl> impl;

	CL_GUIComponent(CL_GUIComponent &other);

	CL_GUIComponent &operator =(const CL_GUIComponent &other);

	friend class CL_GUIManager;

	friend class CL_GUIManager_Impl;

	friend class CL_GUIComponent_Impl;

	friend class CL_GUIThemePart;

	friend class CL_Window;

	friend class CL_Window_Impl;
/// \}
};

/// \}

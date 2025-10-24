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
**    Kenneth Gangstoe
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"
#include "../Core/System/weakptr.h"
#include "../Core/Signals/callback_1.h"
#include "../Core/Signals/callback_2.h"
#include "../Core/Signals/signal_v1.h"
#include "../Core/Math/point.h"
#include "accelerator_table.h"

class CL_Size;
class CL_Rect;
class CL_CSSDocument;
class CL_CSSLayout;
class CL_GUIComponent;
class CL_GUIMessage;
class CL_GUITheme;
class CL_GUIWindowManager;
class CL_GUIManager_Impl;
class CL_Font;
class CL_FontDescription;
class CL_VirtualDirectory;
class CL_DisplayWindow;
class CL_ResourceManager;

/// \brief GUI manager.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIManager
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a gui manager
	///
	/// It is created using the CL_GUIWindowManagerSystem.\n
	/// You will still require to specify a theme by calling: set_theme()
	CL_GUIManager();

	/// \brief Constructs a GUIManager
	///
	/// \param impl = Shared Ptr
	CL_GUIManager(CL_SharedPtr<CL_GUIManager_Impl> impl);

	/// \brief Fully initializes a gui manager with a texture window manager, a css theme and resources.
	///
	/// \param display_window = display window to attach gui to.
	/// \param path_to_theme = Path to theme directory. It has to contain resources.xml and theme.css.
	///
	/// \return Fully initialized gui manager.
	CL_GUIManager(const CL_DisplayWindow &display_window, const CL_String &path_to_theme);

	/// \brief Fully initializes a gui manager with a system window manager, a css theme and resources.
	///
	/// \param path_to_theme = Path to theme directory. It has to contain resources.xml and theme.css.
	///
	/// \return Fully initialized gui manager.
	CL_GUIManager(const CL_String &path_to_theme);

	/// \brief Fully initializes a gui manager with a custom window manager, a css theme and resources.
	///
	/// \param window_manager = Window manager
	/// \param path_to_theme = Path to theme directory. It has to contain resources.xml and theme.css.
	///
	/// \return Fully initialized gui manager.
	CL_GUIManager(CL_GUIWindowManager &window_manager, const CL_String &path_to_theme);

	virtual ~CL_GUIManager();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if the parent is the GUI manager.
	bool is_gui_manager() const { return true; }

	/// \brief Returns the GUI theme being used.
	CL_GUITheme get_theme() const;

	/// \brief Returns the CSS document being used.
	CL_CSSDocument get_css_document() const;

	/// \brief Returns the windows manager being used.
	CL_GUIWindowManager get_window_manager() const;

	/// \brief Returns the mouse capture component
	CL_GUIComponent *get_capture_component() const;

	/// \brief Returns the currently focused component.
	CL_GUIComponent *get_focused_component();

	/// \brief Returns if a dialog message pump loop should exit.
	bool get_exit_flag() const;

	/// \brief Returns the dialog exit code.
	int get_exit_code() const;

	/// \brief Returns userdata.
	CL_UnknownSharedPtr get_userdata();

	/// \brief Get a registered user defined GUI font (set with register_font()). Returns NULL if not found
	CL_Font get_registered_font(const CL_FontDescription &desc);

	/// \brief Get Clipboard text
	///
	/// \return clipboard_text
	CL_String get_clipboard_text() const;

/// \}
/// \name Events
/// \{
public:
	/// \brief bool func_filter_message(const CL_GUIMessage &message)
	CL_Signal_v1<CL_GUIMessage &> &sig_filter_message();

	/// \brief int func_exec_handler(bool loop_until_complete)
	CL_Callback_1<int, bool> &func_exec_handler();

/// \}
/// \name Operations
/// \{
public:
	/// \brief Sets the GUI theme.
	void set_theme(CL_GUITheme &theme);

	/// \brief Sets the CSS document.
	void set_css_document(CL_CSSDocument css);

	/// \brief Sets the CSS document, by creating a CSSDocument from file automatically.
	void set_css_document(const CL_String &fullname);

	/// \brief Set css document
	///
	/// \param filename = String
	/// \param directory = Virtual Directory
	void set_css_document(const CL_String &filename, const CL_VirtualDirectory &directory);

	/// \brief Adds additional resources to the GUI resource manager
	void add_resources(const CL_ResourceManager &resources);

	/// \brief Adds additional resources to the GUI resource manager, by creating a ResourceManager from file automatically.
	void add_resources(const CL_String &filename);

	/// \brief Adds additional resources to the GUI resource manager
	void add_resources(const CL_String &filename, const CL_VirtualDirectory &directory);

	/// \brief Load layout.
	void initialize_layout_manager(const CL_String &xml_fullname, const CL_String &css_fullname);

	/// \brief Sets the windows manager.
	void set_window_manager(CL_GUIWindowManager &window_manager);

	/// \brief Processes messages until exit_with_code is called
	int exec(bool loop_until_complete = true);

	/// \brief Processes all messages available
	///
	/// \param timeout = Timeout (ms). -1 = Wait forever
	void process_messages(int timeout);

	/// \brief Sends a GUI message to the message handler target for the message.
	void dispatch_message(CL_GUIMessage message);

	/// \brief Breaks the message loop.
	void exit_with_code(int exit_code);

	/// \brief Clears the flag indicating exec() should exit its message pump loop.
	void clear_exit_flag();

	/// \brief Set the mouse capture component.
	void set_capture_component(CL_GUIComponent *component, bool state);

	/// \brief Mark the specified area to be redrawn.
	void request_repaint(const CL_Rect &rect, CL_GUIComponent *root_component);

	/// \brief Set userdata.
	void set_userdata(CL_UnknownSharedPtr ptr);

	/// \brief Registers a user defined GUI font - referenced using the specified font description.
	void register_font(const CL_Font &font, const CL_FontDescription &desc);

	/// \brief Render windows
	void render_windows();

	/// \brief Set clipboard text.
	void set_clipboard_text(const CL_StringRef &str);

	/// \brief Redirect proximity events from tablet to the specified component.
	void set_tablet_proximity_component(CL_GUIComponent *, bool state);

	/// \brief Set the accelerator table.
	void set_accelerator_table(const CL_AcceleratorTable &table);

	/// \brief Creates a css layout for a given component.
	CL_CSSLayout create_layout(CL_GUIComponent *component);

	/// \brief Checks if a component type has layout information available.
	bool has_layout(CL_GUIComponent *component);

/// \}
/// \name Implementation
/// \{
private:
	void initialize(CL_GUIWindowManager &window_manager, const CL_String & path_to_theme);

	CL_SharedPtr<CL_GUIManager_Impl> impl;

	friend class CL_GUIComponent_Impl;
	friend class CL_GUIThemePart;
	friend class CL_GUIThemePart_Impl;
/// \}
};

/// \}

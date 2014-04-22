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
**    Kenneth Gangstoe
*/


#pragma once

#include "api_gui.h"
#include "../Core/Signals/callback.h"
#include "../Core/Signals/callback.h"
#include "../Core/Signals/callback.h"
#include "../Core/Signals/signal.h"
#include "../Core/Math/point.h"
#include "accelerator_table.h"
#include <memory>

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

class Size;
class Rect;
class CSSDocument;
class CSSLayout;
class GUIComponent;
class GUIMessage;
class GUIWindowManager;
class GUIManager_Impl;
class Font;
class FontDescription;
class FileSystem;
class DisplayWindow;
class ResourceManager;
class XMLResourceDocument;

/// \brief GUI manager.
class CL_API_GUI GUIManager
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a gui manager with a system window manager. 
	///	
	/// Remember to add a theme using set_theme() or add_theme().
	GUIManager();

	/// \brief Constructs a gui manager with a texture window manager. 
	///	
	/// Remember to add a theme using set_theme() or add_theme().
	///
	/// \param display_window = display window to attach gui to.
	GUIManager(const DisplayWindow &display_window);

	/// \brief Fully constructs a gui manager with a system window manager, a css theme and resources.
	///
	/// \param path_to_css_and_resources = Path to directory. It has to contain resources.xml and theme.css.
	GUIManager(const std::string &path_to_css_and_resources);

	/// \brief Fully constructs a gui manager with a texture window manager, a css theme and resources.
	///
	/// \param display_window = display window to attach gui to.
	/// \param path_to_css_and_resources = Path to directory. It has to contain resources.xml and theme.css.
	GUIManager(const DisplayWindow &display_window, const std::string &path_to_css_and_resources);

	/// \brief Fully constructs a gui manager with a custom window manager, a css theme and resources.
	///
	/// \param window_manager = Window manager
	/// \param path_to_css_and_resources = Path to directory. It has to contain resources.xml and theme.css.
	GUIManager(GUIWindowManager &window_manager, const std::string &path_to_css_and_resources);

	/// \brief Constructs a GUIManager
	///
	/// \param impl = Shared Ptr
	GUIManager(std::shared_ptr<GUIManager_Impl> impl);

	virtual ~GUIManager();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the resource manager
	ResourceManager get_resource_manager() const;

	/// \brief Returns the CSS document being used.
	CSSDocument get_css_document() const;

	/// \brief Returns the windows manager being used.
	GUIWindowManager get_window_manager() const;

	/// \brief Returns the mouse capture component
	GUIComponent *get_capture_component() const;

	/// \brief Returns the currently focused component.
	GUIComponent *get_focused_component();

	/// \brief Returns if a dialog message pump loop should exit.
	bool get_exit_flag() const;

	/// \brief Returns the dialog exit code.
	int get_exit_code() const;

	/// \brief Get Clipboard text
	///
	/// \return clipboard_text
	std::string get_clipboard_text() const;

/// \}
/// \name Events
/// \{
public:
	/// \brief bool func_filter_message(std::shared_ptr<GUIMessage> &message)
	Signal_v1<std::shared_ptr<GUIMessage> &> &sig_filter_message();

	/// \brief int func_exec_handler()
	Callback_0<int> &func_exec_handler();

/// \}
/// \name Operations
/// \{
public:
	/// \brief Set the resource manager (other than the default one)
	void set_resource_manager(ResourceManager &resource_manager);

	/// \brief Adds a GUI theme
	///
	/// \param fullname = Filename of the theme.
	void add_theme(const std::string &fullname);

	/// \brief Adds a GUI theme
	///
	/// \param fullname = Filename of the theme.
	void add_theme(const std::string &fullname, const FileSystem &fs);

	/// \brief Sets the GUI theme
	void set_theme(CSSDocument css);

	/// \brief Sets the GUI theme
	void set_theme(const std::string &fullname);

	/// \brief Set the GUI theme
	///
	/// \param filename = String
	/// \param directory = Virtual Directory
	void set_theme(const std::string &filename, const FileSystem &fs);

	/// \brief Adds resources to the GUI resource manager
	void add_resources(const XMLResourceDocument &resources);

	/// \brief Adds additional resources to the GUI resource manager, by creating a XMLResourceDocument from file automatically.
	void add_resources(const std::string &filename);

	/// \brief Adds additional resources to the GUI resource manager
	void add_resources(const std::string &filename, const FileSystem &fs);

	/// \brief Sets the windows manager.
	void set_window_manager(GUIWindowManager &window_manager);

	/// \brief Processes messages until exit_with_code is called
	int exec();

	/// \brief Processes all messages available
	///
	/// \param timeout = Timeout (ms). -1 = Wait forever
	void process_messages(int timeout);

	/// \brief Sends a GUI message to the message handler target for the message.
	void dispatch_message(std::shared_ptr<GUIMessage> message);

	/// \brief Breaks the message loop.
	void exit_with_code(int exit_code);

	/// \brief Clears the flag indicating exec() should exit its message pump loop.
	void clear_exit_flag();

	/// \brief Set the mouse capture component.
	void set_capture_component(GUIComponent *component, bool state);

	/// \brief Mark the specified area to be redrawn.
	void request_repaint(const Rect &rect, GUIComponent *root_component);

	/// \brief Render windows
	void render_windows();

	/// \brief Set clipboard text.
	void set_clipboard_text(const std::string &str);

	/// \brief Redirect proximity events from tablet to the specified component.
	void set_tablet_proximity_component(GUIComponent *, bool state);

	/// \brief Set the accelerator table.
	void set_accelerator_table(const AcceleratorTable &table);

/// \}
/// \name Implementation
/// \{
private:
	void initialize(GUIWindowManager &window_manager, const std::string & path_to_css_and_resources);

	std::shared_ptr<GUIManager_Impl> impl;

	friend class GUIComponent_Impl;
/// \}
};

}

/// \}

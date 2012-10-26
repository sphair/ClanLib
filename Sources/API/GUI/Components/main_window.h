/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Display/Render/graphic_context.h"

namespace clan
{

class MenuBar;
class ToolBar;
class StatusBar;
class DockWindow;
class MainWindow_Impl;

/// \brief Application main window component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI MainWindow : public GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a MainWindow
	///
	/// \param manager = GUIManager
	/// \param description = GUITop Level Description
	MainWindow(GUIManager *manager, const GUITopLevelDescription &description);

	/// \brief Constructs a MainWindow
	///
	/// \param owner = GUIComponent
	/// \param description = GUITop Level Description
	MainWindow(GUIComponent *owner, const GUITopLevelDescription &description);

	virtual ~MainWindow();

/// \}
/// \name Events
/// \{

public:

	/// \brief Func close
	///
	/// \return Callback_v0
	Callback_v0 &func_close();

	Callback_v1<Rect&> &func_resize();

/// \}
/// \name Attributes
/// \{

public:

	using GUIComponent::get_named_item;

	/// \brief Find the child MainWindow with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static MainWindow *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Get Title
	///
	/// \return title
	std::string get_title() const;

	/// \brief Get Menubar
	///
	/// \return menubar
	MenuBar *get_menubar() const;

	/// \brief Get Statusbar
	///
	/// \return statusbar
	StatusBar *get_statusbar() const;

	std::vector<ToolBar *> get_toolbars() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set title
	///
	/// \param str = String Ref
	void set_title(const std::string &str);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<MainWindow_Impl> impl;
/// \}
};

}

/// \}

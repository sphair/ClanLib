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
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once


#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Display/Render/graphic_context.h"

class CL_MenuBar;
class CL_ToolBar;
class CL_StatusBar;
class CL_DockWindow;
class CL_MainWindow_Impl;

/// \brief Application main window component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_MainWindow : public CL_GUIComponent
{
/// \name Construction
/// \{

public:
	CL_MainWindow(const CL_Rect &geometry, CL_GUIManager *manager, const CL_GUITopLevelDescription &description);

	CL_MainWindow(const CL_Rect &geometry, CL_GUIComponent *owner, const CL_GUITopLevelDescription &description);

	virtual ~CL_MainWindow();


/// \}
/// \name Events
/// \{

public:
	CL_Callback_v0 &func_close();

	CL_Callback_v1<CL_Rect&> &func_resize();


/// \}
/// \name Attributes
/// \{

public:
	CL_StringRef get_title() const;

	CL_MenuBar *get_menubar() const;

	CL_StatusBar *get_statusbar() const;

	std::vector<CL_ToolBar *> get_toolbars() const;


/// \}
/// \name Operations
/// \{

public:
	void set_title(const CL_StringRef &str);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_MainWindow_Impl> impl;
/// \}
};


/// \}

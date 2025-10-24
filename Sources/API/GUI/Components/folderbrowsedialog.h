/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Kenneth Gangstoe
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"

class CL_BrowseFolderDialog_Impl;

/// \brief BrowseFolderDialog component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_BrowseFolderDialog
{
/// \name Construction
/// \{
public:
	/// \brief Constructs an browse folder dialog.
	/// \param owner = Owning GUIComponent
	CL_BrowseFolderDialog(CL_GUIComponent *owner);

	virtual ~CL_BrowseFolderDialog();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Get the full path of the directory selected.
	///
	/// \return CL_String
	CL_String get_selected_path() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Sets the initial directory that is displayed.
	/// \param path = path
	void set_initial_directory(const CL_String &path);

	/// \brief Sets the text that appears in the title bar.
	/// \param title = title
	void set_title(const CL_String &title);

	/// \brief Shows the file dialog.
	/// \return true if the user clicks the OK button of the dialog that is displayed, false otherwise.
	bool show();

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_BrowseFolderDialog_Impl> impl;
/// \}
};

/// \}

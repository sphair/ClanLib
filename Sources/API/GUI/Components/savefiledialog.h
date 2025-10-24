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

class CL_FileDialog_Impl;

/// \brief SaveFileDialog component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_SaveFileDialog
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a save file dialog.
	/// \param owner = Owning GUIComponent
	CL_SaveFileDialog(CL_GUIComponent *owner);

	virtual ~CL_SaveFileDialog();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Get the full path of the file selected.
	/// \return CL_String
	CL_String get_filename() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Sets a string containing the full path of the file selected.
	/// \param filename = filename
	void set_filename(const CL_String &filename);
	
	/// \brief Add a filter that determines what types of files are displayed.
	/// \param filter_description = filter_description
	/// \param filter_extension = filter_extension
	/// \param is_default = is_default
	void add_filter(const CL_String &filter_description, const CL_String &filter_extension, bool is_default = false);

	/// \brief Clears all filters.
	void clear_filters();

	/// \brief Sets a default filter, on a 0-based index.
	/// \param filter_index = filter_index
	void set_filter_index(int filter_index);

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
	CL_SharedPtr<CL_FileDialog_Impl> impl;
/// \}
};

/// \}

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

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once


#include "../api_gui.h"
#include "../gui_component.h"

class CL_Image;
class CL_StatusBar_Impl;

/// \brief Status bar component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_StatusBar : public CL_GUIComponent
{
/// \name Construction
/// \{

public:
	CL_StatusBar(CL_GUIComponent *parent);

	virtual ~CL_StatusBar();


/// \}
/// \name Attributes
/// \{

public:
	CL_Size get_preferred_size() const;

	CL_Callback_v0 &func_part_double_clicked(int id);

/// \}
/// \name Operations
/// \{

public:
	void set_status_text(const CL_StringRef &text);

	void set_part_text(int id, const CL_StringRef &text);

	void set_part_text(int id, const CL_Image &icon, const CL_StringRef &text);

	void add_part(int id, int width, CL_GUIComponent *component = 0);

	void show_part(int id, bool show = true);

	void remove_part(int id);

	void show_size_grip(bool show = true);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_StatusBar_Impl> impl;
/// \}
};


/// \}

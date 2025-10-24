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
#include "../../Core/Signals/callback_v0.h"

class CL_CheckBox_Impl;

/// \brief Check box component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_CheckBox : public CL_GUIComponent
{
/// \name Construction
/// \{
public:
	CL_CheckBox(CL_GUIComponent *parent);

	virtual ~CL_CheckBox();

/// \}
/// \name Attributes
/// \{
public:
	bool is_indeterminated() const;

	bool is_checked() const;

	bool is_3state() const;

	const CL_String &get_text() const;

	CL_Size get_preferred_size() const;

/// \}
/// \name Operations
/// \{
public:
	void set_indeterminated();

	void set_checked(bool enable);

	void set_3state(bool enable);

	void set_text(const CL_StringRef &text);

/// \}
/// \name Events
/// \{
public:
	CL_Callback_v0 &func_checked();
	CL_Callback_v0 &func_unchecked();
	CL_Callback_v0 &func_indeterminated();
	CL_Callback_v0 &func_state_changed();

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_CheckBox_Impl> impl;
/// \}
};


/// \}

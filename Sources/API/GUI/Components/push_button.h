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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Magnus Norddahl
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once


#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Display/Render/graphic_context.h"

class CL_Image;
class CL_PushButton_Impl;

/// \brief Push button component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_PushButton : public CL_GUIComponent
{
/// \name Construction
/// \{

public:
	CL_PushButton(CL_GUIComponent *parent);

	virtual ~CL_PushButton();


/// \}
/// \name Attributes
/// \{

public:
	bool is_pushed() const;

	bool is_toggle() const;

	bool is_flat() const;

	CL_Image get_icon() const;

	enum IconPosition
	{
		icon_left, icon_top, icon_right, icon_bottom
	};

	IconPosition get_icon_position() const;

	const CL_String &get_text() const;

	CL_Size get_preferred_size() const;


/// \}
/// \name Operations
/// \{

public:
	void set_pushed(bool enable);

	void set_toggle(bool enable);

	void set_flat(bool enable);

	void set_icon(const CL_Image &icon);

	void set_icon_position(IconPosition pos);

	void set_text(const CL_StringRef &text);


/// \}
/// \name Events
/// \{

public:
	CL_Callback_v0 &func_clicked();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_PushButton_Impl> impl;
/// \}
};


/// \}

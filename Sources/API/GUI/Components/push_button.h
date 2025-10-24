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

	/// \brief Constructs a PushButton
	///
	/// \param parent = GUIComponent
	CL_PushButton(CL_GUIComponent *parent);

	virtual ~CL_PushButton();

/// \}
/// \name Attributes
/// \{

public:

	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_PushButton with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_PushButton *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Is Pushed
	///
	/// \return true = pushed
	bool is_pushed() const;

	/// \brief Is Toggle
	///
	/// \return true = toggle
	bool is_toggle() const;

	/// \brief Is Flat
	///
	/// \return true = flat
	bool is_flat() const;

	/// \brief Get Icon
	///
	/// \return icon
	CL_Image get_icon() const;

	enum IconPosition
	{
		icon_left, icon_top, icon_right, icon_bottom
	};

	/// \brief Get Icon position
	///
	/// \return icon_position
	IconPosition get_icon_position() const;

	const CL_String &get_text() const;

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	CL_Size get_preferred_size() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set pushed
	///
	/// \param enable = bool
	void set_pushed(bool enable);

	/// \brief Set toggle
	///
	/// \param enable = bool
	void set_toggle(bool enable);

	/// \brief Set flat
	///
	/// \param enable = bool
	void set_flat(bool enable);

	/// \brief Set icon
	///
	/// \param icon = Image
	void set_icon(const CL_Image &icon);

	/// \brief Set icon position
	///
	/// \param pos = Icon Position
	void set_icon_position(IconPosition pos);

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const CL_StringRef &text);

/// \}
/// \name Events
/// \{

public:

	/// \brief Func clicked
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_clicked();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_PushButton_Impl> impl;
/// \}
};

/// \}

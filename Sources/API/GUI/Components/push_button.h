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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Magnus Norddahl
*/


#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Display/Render/graphic_context.h"

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

class Image;
class PushButton_Impl;

/// \brief Push button component.
class CL_API_GUI PushButton : public GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a PushButton
	///
	/// \param parent = GUIComponent
	PushButton(GUIComponent *parent);

	virtual ~PushButton();

/// \}
/// \name Attributes
/// \{

public:

	using GUIComponent::get_named_item;

	/// \brief Find the child PushButton with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static PushButton *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Is Pushed
	///
	/// \return true = pushed
	bool is_pushed() const;

	/// \brief Is Toggle
	///
	/// \return true = toggle
	bool is_toggle() const;

	/// \brief Get Icon
	///
	/// \return icon
	Image get_icon() const;

	enum IconPosition
	{
		icon_left, icon_top, icon_right, icon_bottom
	};

	/// \brief Get Icon position
	///
	/// \return icon_position
	IconPosition get_icon_position() const;

	const std::string &get_text() const;

	/// \brief Returns the preferred content width
	///
	/// Override this function if the component has non-css content.
	float get_preferred_content_width();

	/// \brief Returns the preferred content height for the specified content width
	///
	/// Override this function if the component has non-css content.
	float get_preferred_content_height(float width);

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

	/// \brief Set icon
	///
	/// \param icon = Image
	void set_icon(const Image &icon);

	/// \brief Set icon position
	///
	/// \param pos = Icon Position
	void set_icon_position(IconPosition pos);

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const std::string &text);

/// \}
/// \name Events
/// \{

public:

	/// \brief Func clicked
	///
	/// \return Callback_v0
	Callback_v0 &func_clicked();

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<PushButton_Impl> impl;
/// \}
};

}

/// \}

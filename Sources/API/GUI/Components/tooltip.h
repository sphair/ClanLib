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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"

class CL_GUIManager;
class CL_ToolTip_Impl;

/// \brief Tool tip component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ToolTip : public CL_GUIComponent
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a ToolTip
	///
	/// \param gui_manager = GUIManager
	CL_ToolTip(CL_GUIManager gui_manager);

	virtual ~CL_ToolTip();

/// \}
/// \name Attributes
/// \{
public:

	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_ToolTip with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_ToolTip *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Get Text
	///
	/// \return text
	CL_StringRef get_text() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const CL_StringRef &text);

	/// \brief Show
	///
	/// \param position = Point
	void show(const CL_Point &position);

	/// \brief Show delayed
	///
	/// \param position = Point
	/// \param delay_time = value
	void show_delayed(const CL_Point &position, int delay_time = 750);

	/// \brief Hide
	void hide();

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_ToolTip_Impl> impl;
/// \}
};

/// \}

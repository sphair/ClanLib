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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"

class CL_Sprite;
class CL_Frame_Impl;

/// \brief Frame component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_Frame : public CL_GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a Frame
	///
	/// \param parent = GUIComponent
	CL_Frame(CL_GUIComponent *parent);

	virtual ~CL_Frame();

/// \}
/// \name Attributes
/// \{

public:
	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_Frame with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_Frame *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Get Header icon
	///
	/// \return header_icon
	CL_Sprite get_header_icon() const;

	/// \brief Get Header text
	///
	/// \return header_text
	CL_StringRef get_header_text() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set header icon
	///
	/// \param icon = Sprite
	void set_header_icon(const CL_Sprite &icon);

	/// \brief Set header text
	///
	/// \param text = String Ref
	void set_header_text(const CL_StringRef &text);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_Frame_Impl> impl;
/// \}
};

/// \}

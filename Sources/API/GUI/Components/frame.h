/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

namespace clan
{

class Sprite;
class Frame_Impl;

/// \brief Frame component.
class CL_API_GUI Frame : public GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a Frame
	///
	/// \param parent = GUIComponent
	Frame(GUIComponent *parent);

	virtual ~Frame();

/// \}
/// \name Attributes
/// \{

public:
	using GUIComponent::get_named_item;

	/// \brief Find the child Frame with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static Frame *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Get Header icon
	///
	/// \return header_icon
	Sprite get_header_icon() const;

	/// \brief Get Header text
	///
	/// \return header_text
	std::string get_header_text() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set header icon
	///
	/// \param icon = Sprite
	void set_header_icon(const Sprite &icon);

	/// \brief Set header text
	///
	/// \param text = String Ref
	void set_header_text(const std::string &text);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<Frame_Impl> impl;
/// \}
};

}

/// \}

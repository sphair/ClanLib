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

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include <memory>

namespace clan
{

class GUIComponent;
class CSSBoxProperties;
class GUIPseudoComponent_Impl;
class Font;

/// \brief A GUI pseudo component represents a pseudo element in CSS.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI GUIPseudoComponent
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	GUIPseudoComponent();

	/// \brief Creates a GUI component.
	GUIPseudoComponent(GUIComponent *parent, const std::string &pseudo_tag_name);
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the standard W3C CSS properties active for this component
	const CSSBoxProperties &get_css_properties() const;

	/// \brief Returns the standard W3C CSS properties active for this component
	CSSBoxProperties &get_css_properties();

	/// \brief Gets the font
	Font get_font();

/// \}

/// \name Operations
/// \{
public:
	/// \brief Re-evaluates which CSS selectors match this component
	void update_style();

	// To do: provide access to GUICSSUsedValues data and/or add the box calculation helpers we had on GUIThemePart
/// \}

/// \name Implementation
/// \{
public:
	std::shared_ptr<GUIPseudoComponent_Impl> impl;
/// \}
};

}

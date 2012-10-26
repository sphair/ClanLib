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

#pragma once

#include "../gui_component.h"

namespace clan
{

class RibbonSection;

class RibbonPage : public GUIComponent
{
/// \name Construction
/// \{
public:
	RibbonPage(GUIComponent *parent, const std::string &text);

/// \}
/// \name Attributes
/// \{
public:
	bool get_show_tab() const;

/// \}
/// \name Operations
/// \{
public:
	void add_section(RibbonSection *section);

	void set_show_tab(bool value);

	void set_tab_custom_css_state(const std::string &css_state_name);

/// \}
/// \name Implementation
/// \{
private:
	void on_resized();

	std::string text;
	std::string tab_css_custom_state;
	std::vector<RibbonSection *> sections;
	Rect position;
	bool show_tab;

	friend class Ribbon;
/// \}
};

}

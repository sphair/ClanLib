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
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once

#include "API/GUI/gui_component.h"

namespace clan
{

class TabHeader_Impl;
class TabPage;

class TabHeader : public GUIComponent
{
/// \name Construction
/// \{

public:
	TabHeader(GUIComponent *parent);

	virtual ~TabHeader();


/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	void add_page(TabPage *tab_page, const std::string &label);

	void del_page(TabPage *tab_page);

	void page_renamed(TabPage *tab_page);

	void insert_page(TabPage *tab_page, int index);

	void select_page(int index);

/// \}
/// \name Callbacks
/// \{

public:
	Callback_v1<TabPage*> &func_page_selected();


/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<TabHeader_Impl> impl;
/// \}
};

}

/// \}

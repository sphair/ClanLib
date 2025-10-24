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
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once


#include "API/GUI/gui_component.h"

class CL_TabHeader_Impl;
class CL_TabPage;

class CL_TabHeader : public CL_GUIComponent
{
/// \name Construction
/// \{

public:
	CL_TabHeader(CL_GUIComponent *parent);

	virtual ~CL_TabHeader();


/// \}
/// \name Attributes
/// \{

public:
	int get_preferred_height() const;

	virtual CL_Size get_preferred_size() const;


/// \}
/// \name Operations
/// \{

public:
	void add_page(CL_TabPage *tab_page, const CL_StringRef &label);

	void del_page(CL_TabPage *tab_page);

	void page_renamed(CL_TabPage *tab_page);

	void insert_page(CL_TabPage *tab_page, int index);

	void select_page(int index);

/// \}
/// \name Callbacks
/// \{

public:
	CL_Callback_v1<CL_TabPage*> &func_page_selected();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_TabHeader_Impl> impl;
/// \}
};


/// \}

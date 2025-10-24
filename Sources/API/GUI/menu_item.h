/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_menu_item
#define header_menu_item

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "component.h"

class CL_MenuItem_Generic;
class CL_Surface;

//: Label component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_MenuItem : public CL_Component
{
//! Construction:
public:
	//: MenuItem Constructor
	CL_MenuItem(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: MenuItem Destructor
	virtual ~CL_MenuItem();

//! Attributes:
public:
	//: Returns the menu item text.
	const std::string &get_text() const;

	//: Returns true if the menu item can have an icon.
	bool use_icon() const;
	
	//: Returns true if the menu item is in toggle mode, otherwise false.
	bool is_toggling() const;

	//: Returns true if the menu item is selected
	bool is_selected() const;

//! Operations:
public:
	//: Sets the item to toggle mode (true) or normal mode (false). Default is false.
	void set_toggling(bool toggle);

	//: Sets the item to toggle mode (true) or normal mode (false). Default is false.
	void set_use_icon(bool use_icon);

	//: Sets the menu items selected status, if in toggle mode.
	void set_selected(bool selected);

	//: Sets the menu item text.
	void set_text(const std::string &text);

	//: Sets the menu item contents to a plain text containing the printed value of num.
	void set_text(int number);

	//: Sets the menu item contents to a plain text containing the printed value of num.
	void set_text(double number);

	//: Clears any menu item text. Equivalent with set_text("").
	void clear();

//! Implementation:
private:
	void init(const std::string &text);

	CL_MenuItem(const CL_MenuItem &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	
	CL_MenuItem_Generic *impl;
};  

#endif

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

#ifndef header_checkbox
#define header_checkbox

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

#include "button.h"

//: CheckBox component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_CheckBox : public CL_Button
{
//! Construction:
public:
	//: CheckBox constructor
	CL_CheckBox(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: CheckBox constructor
	CL_CheckBox(
		const CL_Point &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: CheckBox constructor
	CL_CheckBox(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: CheckBox destructor
	virtual ~CL_CheckBox();

//! Attributes:
public:
	//: Returns true if the checkbox is checked, or false if it is not checked.
	bool is_checked() const;

//! Operations:
public:
	//: Checks the checkbox if check is true, or unchecks it if check is false. 
	void set_checked(bool check = true);

//! Implementation:
private:
	CL_CheckBox(const CL_CheckBox &copy) : CL_Button(NULL, NULL) { return; } // disallow copy construction.
};  

#endif

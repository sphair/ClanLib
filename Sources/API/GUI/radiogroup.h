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

#ifndef header_radiogroup
#define header_radiogroup

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

#include "../signals.h"
#include <vector>

class CL_RadioGroup_Generic;
class CL_RadioButton;

//: RadioGroup
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_RadioGroup
{
//! Construction:
public:
	//: RadioGroup Constructor
	CL_RadioGroup();

	//: RadioGroup Destructor
	~CL_RadioGroup();

//! Attributes:
public:
	//: Returns the amount of buttons added to the group.
	int get_count() const;
	
	//: Returns the selected button if any, NULL otherwise.
	CL_RadioButton *get_toggled() const;

	//: Returns all buttons in the group.
	const std::vector<CL_RadioButton *> &get_buttons() const;
	
//! Operations:
public:
	//: Adds a component into the group.
	void add(CL_RadioButton *button, bool delete_component = false);
	
	//: Removes a component from the group.
	void remove(CL_RadioButton *button);
	
	//: Sets a radiobutton active (and unchecks any other).
	void set_checked(CL_RadioButton *button);
	
//! Signals:
public:
	//: This signal is emitted when the radio selection is changed.
	CL_Signal_v1<CL_RadioButton *> &sig_selection_changed();

//! Implementation:
private:
	CL_RadioGroup(const CL_RadioGroup &copy) { return; } // disallow copy construction.
	CL_RadioGroup_Generic *impl;
};

#endif

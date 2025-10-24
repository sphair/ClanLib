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

#ifndef header_inputdialog
#define header_inputdialog

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

#include "window.h"

class CL_InputDialog_Generic;
class CL_InputBox;
class CL_CheckBox;
class CL_Button;

//: CL_InputDialog component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_InputDialog : public CL_Window
{
//! Construction:
public:
	//: CL_InputDialog constructor
	CL_InputDialog(
		const std::string &title,
		const std::string &button1,
		const std::string &button2,
		const std::string &button3,
		CL_Component *parent,
		CL_StyleManager *style = NULL);
	
	//: CL_InputDialog destructor
	virtual ~CL_InputDialog();

//! Operations:
public:
	//: Add inputbox with label to dialog.
	//: The inputbox is deleted by the InputDialog when destroyed.
	CL_InputBox *add_input_box(
		const std::string &label = "",
		const std::string &default_value = "",
		int inputbox_width = 100);

	//: Add checkbox with label to dialog.
	//: The checkbox is deleted by the InputDialog when destroyed.
	CL_CheckBox *add_check_box(
		const std::string &text = "",
		bool checked = false,
		int check_box_width = 100);

	//: Get the pointer to one of the built in buttons (ok/cancel/etc) to further customize it
	//: 0 based.  Returns NULL if no button exists at that index
	CL_Button * get_button(int button);

	//: Returns the number of the button that was used to close the dialog.
	int get_result_button();

//! Implementation:
private:
	CL_InputDialog(const CL_InputDialog &copy) : CL_Window((CL_Component*)NULL, NULL) { return; } // disallow copy construction.

	CL_InputDialog_Generic *impl;
};

#endif

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

#ifndef header_messagebox
#define header_messagebox

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

class CL_MessageBox_Generic;

//: MessageBox component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_MessageBox : public CL_Window
{
//! Construction:
public:
	//: MessageBox constructor
	CL_MessageBox(
		const std::string &title,
		const std::string &text,
		const std::string &button1,
		const std::string &button2,
		const std::string &button3,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: MessageBox destructor
	virtual ~CL_MessageBox();

	//: Creates a messagebox.
	//: Returns the button clicked, starting at index 0.
	static int info(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	static int info(
		const std::string &title,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	static int info(
		const std::string &title,
		const std::string &text,
		const std::string &button1,
		const std::string &button2,
		const std::string &button3,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

//! Attributes:
public:
	//: Returns the index of the button clicked.
	int get_result_button() const;

//! Implementation:
private:
	CL_MessageBox(const CL_MessageBox &copy) : CL_Window((CL_Component*)NULL, NULL) { return; } // disallow copy construction.

	CL_MessageBox_Generic *impl;
};

#endif

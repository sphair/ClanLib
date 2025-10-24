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
**    Kenneth Gangstoe
**    (if your name is missing here, please add it)
*/

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_window
#define header_window

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

class CL_Window_Generic;

//: Window component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_Window : public CL_Component
{
//! Enums:
public:
	//: Button enums
	enum TitlebarButtons
	{
		no_buttons = 0,
		close_button = 1,
		maximize_button = 2,
		minimize_button = 4,
		help_button = 8
	};

//! Construction:
public:
	//: Creates a window.
	CL_Window(
		int button_enum, 
		CL_Component *parent, 
		CL_StyleManager *style = NULL);

	CL_Window(
		const std::string &title,
		int button_enum, 
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	CL_Window(
		const CL_Rect &pos,
		const std::string &title,
		int button_enum, 
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	CL_Window(
		CL_Component *parent, 
		CL_StyleManager *style = NULL);

	CL_Window(
		const std::string &title,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	CL_Window(
		const CL_Rect &pos,
		const std::string &title,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Window destructor
	virtual ~CL_Window();

//! Attributes:
public:
	//: Returns the window title.
	const std::string &get_title() const;

	//: Returns the titlebar button enum.
	TitlebarButtons get_titlebar_buttons_enum() const;

//! Operations:
public:
	//: Sets the window title.
	//param text: An std::string with the new title
	void set_title(const std::string &text);

//! Signals:
public:
	//: This signal is emitted when the close button on the titlebar is activated
	CL_Signal_v0 &sig_close_button_clicked();

	//: This signal is emitted when the minimize button on the titlebar is activated
	CL_Signal_v0 &sig_minimize_button_clicked();

	//: This signal is emitted when the maximize button on the titlebar is activated
	CL_Signal_v0 &sig_maximize_button_clicked();

	//: This signal is emitted when the help button on the titlebar is activated
	CL_Signal_v0 &sig_help_button_clicked();

//! Implementation:
private:
	CL_Window(const CL_Window &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_Window_Generic *impl;
};  

#endif

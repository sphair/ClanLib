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

#ifndef header_button
#define header_button

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

class CL_Surface;
class CL_Button_Generic;

//: Button component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_Button : public CL_Component
{
//! Construction:
public:
	//: Button Constructor
	CL_Button(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const CL_Point &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const CL_Rect &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const CL_Point &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Destructor
	virtual ~CL_Button();

//! Attributes:
public:
	//: Returns the button text.
	const std::string &get_text() const;

	//: Returns true if the button is a toggle button.
	bool is_toggle_button() const;

	//: Returns true if the button state is down, or false if it is raised.
	bool is_down() const;

	//: Returns true if the button is visually pressed down, or false if it is raised.
	//: This can be used by theme, to get immediate state of the button.
	bool is_drawn_down() const;

//! Operations:
public:
	//: Sets the button text.
	void set_text(const std::string &text);

	//: If toggle is true, pressing the button toggles it between an On and and Off state. 
	//: If toggle is false, pressing the button causes an action, then the button returns to the unpressed state. 
	void set_toggle_mode(bool toggle = true);

	//: Sets the state of the button to pressed down if enable is true or to standing up if enable is false. 
	//: No signals are not emitted by this function. 
	void set_down(bool enable = true);

	//: Toggle the button.
	//: The toggled() signal are emitted by this function.
	void toggle();

//! Signals:
public:
	//: This signal is emitted whenever a toggle button changes status.
	//param bool on: Is true if the button is on, or false otherwise.
	CL_Signal_v1<bool> &sig_toggled();

	//: This signal is emitted when the button is activated (i.e. first pressed down and then released when the mouse cursor is inside the button).
	CL_Signal_v0 &sig_clicked();
	
	//: This signal is emitted when the button is pressed down. 
	//: In most cases, you want to use sig_clicked() instead.
	CL_Signal_v0 &sig_pressed();

	//: This signal is emitted when the button is released.
	//: In most cases, you want to use sig_clicked() instead.
	CL_Signal_v0 &sig_released();

//! Implementation:
private:
	CL_Button(const CL_Button &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_Button_Generic *impl;
};

#endif

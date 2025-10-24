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

#ifndef header_button_generic
#define header_button_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/GUI/button.h"
#include "API/Core/System/clanstring.h"

class CL_Button_Generic
{
private:
	enum PressType {
		NOT_PRESSED,
		PRESSED_BY_KEYBOARD,
		PRESSED_BY_MOUSE
	};
//! Construction:
public:
	CL_Button_Generic(
		CL_Button *self,
		const std::string &text,
		bool toggle_mode);

	~CL_Button_Generic() {};

//! Attributes:
public:
	std::string text;
	bool toggle_mode;

	bool toggled;
	bool visually_down;

	PressType pressed_by;

//! Signals:
public:
	CL_Signal_v1<bool> sig_toggled;
	CL_Signal_v0 sig_clicked;
	CL_Signal_v0 sig_pressed;
	CL_Signal_v0 sig_released;

//! Callbacks:
private:
	void on_set_options(const CL_DomElement &options);
	void on_input_down(const CL_InputEvent &key);
	void on_input_up(const CL_InputEvent &key);
	void on_mouse_move(const CL_InputEvent &key);

//! Implementation:
private:
	CL_Button *button;

	CL_SlotContainer slots;
};  

#endif

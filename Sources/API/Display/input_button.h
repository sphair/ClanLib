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

//! clanDisplay="Input"
//! header=display.h

#ifndef header_inputbutton
#define header_inputbutton

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "input_event.h"
#include "key_binding.h"

class CL_InputButton_Generic;

//: Input Button interface.
//- !group=Display/Input!
//- !header=display.h!
//- Use this class to bind inputevents to this class. You can then check if any input events has
//- be activated (like a mouse-press, a certain keypress etc).
//- Useful for things like mapping SPACE and LEFT MOUSE to a Fire Action.
class CL_API_DISPLAY CL_InputButton
{
//! Construction:
public:
	//: Constructor.
	CL_InputButton();

	//: Copy constructor.
	CL_InputButton(const CL_InputButton &copy);

	//: Destructor.
	virtual ~CL_InputButton();

//! Attributes:
public:
	//: Returns the number of presses this inputbutton has been pressed.
	//- Returns - Number of key presses of this inputbutton.
	int peek_pressed() const;

	//: Returns the CL_InputEvent of the last input press.
	//- Returns - CL_InputEvent of the last input press.
	CL_InputEvent peek_key() const;

//! Operations:
public:
	//: Copy assignment operator.
	CL_InputButton &operator =(const CL_InputButton &copy);

	//: Binds a key.
	void bind(const CL_KeyBinding &key);

	//: Unbinds a key.
	void unbind(const CL_KeyBinding &key);

	//: Unbinds all keys.
	void unbind_all();

	//: Returns a list over all keys bound to this inputbutton.
	const std::list<CL_KeyBinding> &get_bindings() const;

	//: Returns the number of presses this inputbutton has been pressed.
	//: This clears the state of this inputbutton.
	//- Returns - Number of key presses of this inputbutton.
	int pop_pressed();

	//: Returns the CL_InputEvent of the last input press.
	//: This clears the state of the inputbutton.
	//- Returns - CL_InputEvent containing information on last input press.
	CL_InputEvent pop_key();

	//: Clears the state of the inputbutton.
	void clear();

//! Implementation:
private:
	CL_InputButton_Generic *impl;
};

#endif

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

#ifndef header_keybinding
#define header_keybinding

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

#include <string>

class CL_KeyBinding_Generic;
class CL_InputDevice;

//: Input Button binding interface.
//- !group=Display/Input!
//- !header=display.h!
//- Use this class to ...
class CL_API_DISPLAY CL_KeyBinding
{
//! Construction:
public:
	//: Constructor.
	CL_KeyBinding();

	//: Constructor.
	CL_KeyBinding(int keycode, const CL_InputDevice &device);

	//: Copy constructor.
	CL_KeyBinding(const CL_KeyBinding &copy);

//! Attributes:
public:
	//:
	bool operator ==(const CL_KeyBinding &other) const;

	//:
	bool operator <(const CL_KeyBinding &other) const;

	//:
	bool operator >(const CL_KeyBinding &other) const;

	//:
	CL_InputDevice &get_device() const;

	//:	
	int get_keycode() const;

	//:
	std::string get_key_name() const;

//! Operations:
public:
	//:
	CL_KeyBinding &operator =(const CL_KeyBinding &copy);

	//:
	void set_keycode(int keycode);

	//:
	void set_device(const CL_InputDevice &device);

//! Implementation:
private:
	CL_KeyBinding_Generic *impl;
};

#endif

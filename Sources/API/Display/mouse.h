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

#ifndef header_mouse
#define header_mouse

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

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include <string>
#include "../signals.h"

class CL_InputEvent;
class CL_InputDevice;
class CL_Sprite;

//: Class for handling mouse device.
//- !group=Display/Input!
//- !header=display.h!
class CL_API_DISPLAY CL_Mouse
{
//! Construction:
public:

//! Attributes:
public:
	//: Returns the number of mice available.
	static int get_device_count();

	//: Returns a mouse device.
	static CL_InputDevice &get_device(int mouse = 0);

	//: Returns the x position of a mouse device.
	static int get_x(int mouse = 0);
	
	//: Returns the y position of a mouse device.
	static int get_y(int mouse = 0);
	
	//: Returns true if the key with the specified key code is pressed.
	static bool get_keycode(int keycode, int mouse = 0);

	//: Key name for specified identifier (Mouse Left, Mouse Right).
	static std::string get_key_name(int id);

//! Operations:
public:
	//: Sets the position of a mouse device.
	static void set_position(int x, int y, int mouse = 0);

	//: Set a custom mouse cursor.
	static void set_cursor(const CL_Sprite &sprite);

	//: Shows the mouse cursor. 
	static void show();

	//: Hides the mouse cursor.
	static void hide();

//! Signals:
public:
	//: Signal emitted when mouse is pressed on the specified mouse.
	static CL_Signal_v1<const CL_InputEvent &> &sig_key_down(int mouse = 0);

	//: Signal emitted when mouse is released on the specified mouse.
	static CL_Signal_v1<const CL_InputEvent &> &sig_key_up(int mouse = 0);

	//: Signal emitted when mouse is double-clicked on the specified mouse.
	static CL_Signal_v1<const CL_InputEvent &> &sig_key_dblclk(int mouse = 0);

	//: Signal emitted when specified mouse is moved.
	static CL_Signal_v1<const CL_InputEvent &> &sig_move(int mouse = 0);

//! Implementation:
private:
};

#endif

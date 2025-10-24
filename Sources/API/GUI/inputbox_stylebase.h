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
**    AlexeyKharkov
**    (if your name is missing here, please add it)
*/

#ifndef header_inputbox_stylebase
#define header_inputbox_stylebase

#if _MSC_VER > 1000
#pragma once
#endif

#include "../GUI/inputbox.h"
#include <string>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

// -----------------------------------------------------------------------------------
class CL_InputBox_StyleBase
{
//! Construction:
public:
	//: Constructor
	CL_InputBox_StyleBase(
		CL_InputBox *inputbox, 
		CL_ResourceManager *resources, 
		std::string const & font_name, 
		int border_size);
	
	//: Destructor
	virtual ~CL_InputBox_StyleBase();

	//
	void on_input_down(const CL_InputEvent &key);
	void on_input_up(const CL_InputEvent &key);
	void on_mouse_move(const CL_InputEvent &key);

//! Implementation:
protected:
	int get_chars_width( int from, int to ) const;
	int get_mouse_position(int x, int y);

	//
	CL_InputBox *inputbox;
	CL_Font *font;

	int border_size;

	int draw_from;

	bool show_cursor;
	unsigned int cursor_blink_time;

private:
	CL_Slot slot_input_down;
	CL_Slot slot_input_up;
	CL_Slot slot_mouse_move;
};

#endif

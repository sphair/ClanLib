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
**    Kenneth Gangstø
**    (if your name is missing here, please add it)
*/

//! clanGUI="Style: Silver"
//! header=guistylesilver.h

#ifndef header_window_silver
#define header_window_silver

#ifdef CL_API_DLL
#ifdef CL_GUISTYLESILVER_EXPORT
#define CL_API_GUISTYLESILVER __declspec(dllexport)
#else
#define CL_API_GUISTYLESILVER __declspec(dllimport)
#endif
#else
#define CL_API_GUISTYLESILVER
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../GUI/window.h"
#include "../GUI/button.h"
#include "../GUI/component_style.h"
#include "../GUI/component_move_handler.h"
#include "../GUI/component_resize_handler.h"

class CL_StyleManager_Silver;
class CL_Font;

//: Window default style.
//- !group=GUI/Style: Silver!
//- !header=guistylesilver.h!
//- <p>Script attributes:</p>
//- <ul>
//- <li>font: The font used to draw the caption. [String]</li>
//- </ul>
//- <p>Note that it inherits the attributes of CL_Window and CL_Component as well.</p>
class CL_API_GUISTYLESILVER CL_Window_Silver : public CL_ComponentStyle
{
//! Construction:
public:
	//: Constructor
	CL_Window_Silver(
		CL_Window *window,
		CL_StyleManager_Silver *style);

	//: Destructor
	virtual ~CL_Window_Silver();

//! Implementation:
private:
	CL_SlotContainer slots;

	void on_paint();
	void on_resize(int old_width, int old_height);
	void on_close_button_clicked();
	void on_maximize_button_clicked();
	void on_minimize_button_clicked();
	void on_help_button_clicked();
	
	CL_Button *add_titlebar_button(
		const std::string &resource_normal, 
		const std::string &resource_down, 
		const std::string &resource_disabled,
		CL_ResourceManager *resources);

	CL_Window *window;

	CL_Button *button_close;
	CL_Button *button_maximize;
	CL_Button *button_minimize;
	CL_Button *button_help;

	CL_ComponentMoveHandler *move_handler;
	CL_ComponentResizeHandler *resize_handler;

	CL_StyleManager_Silver *style;
	
	CL_Font *font;
	CL_Font *font_disabled;

	int titlebar_height;
};

#endif

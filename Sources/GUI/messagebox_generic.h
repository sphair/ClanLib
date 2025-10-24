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

#ifndef header_dialog_generic
#define header_dialog_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/GUI/messagebox.h"
#include "API/GUI/button.h"
#include "API/GUI/label.h"

class CL_Font;

class CL_MessageBox_Generic
{
public:
	CL_MessageBox_Generic(
		CL_MessageBox *self,
		const std::string &title,
		const std::string &text,
		const std::string &button1,
		const std::string &button2,
		const std::string &button3);

	~CL_MessageBox_Generic();

	std::string text;

	CL_MessageBox *messagebox;

	CL_Font * font;
	CL_Button *button[3];
	int button_count;

	CL_Slot slot_button[3];
	CL_Slot slot_end_paint;
	void on_button(int button);
	void on_end_paint();

	int result_button;
};

#endif

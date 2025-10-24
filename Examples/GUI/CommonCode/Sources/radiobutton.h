/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include "info.h"

class RadioButton : public CL_Window
{
public:
	RadioButton(CL_GUIManager &manager);

private:
	void on_selected(CL_RadioButton *radiobutton);
	void on_unselected(CL_RadioButton *radiobutton);
	void on_group_selection_changed(CL_RadioButton *radiobutton_group, CL_RadioButton *radiobutton);
	CL_String decode_button(CL_RadioButton *radiobutton);
	void on_checked_disable(CL_CheckBox *checkbox);
	void on_unchecked_disable(CL_CheckBox *checkbox);

private:
	CL_RadioButton *radiobutton_g1b1;
	CL_RadioButton *radiobutton_g1b2;
	CL_RadioButton *radiobutton_g1b3;
	CL_RadioButton *radiobutton_g2b1;
	CL_RadioButton *radiobutton_g2b2;

	Info *info_selected;
	Info *info_unselected;
	Info *info_group_selection_changed;

	CL_CheckBox *checkbox_disable;
};

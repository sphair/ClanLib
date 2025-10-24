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

#ifndef header_radiogroup_generic
#define header_radiogroup_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "../API/signals.h"

class CL_RadioButton;

class CL_RadioGroup_Generic
{
public:
	CL_RadioGroup_Generic() {};

	void add(CL_RadioButton *button, bool delete_component);
	void remove(CL_RadioButton *button);
	void set_checked(CL_RadioButton *button);

	CL_RadioButton *get_toggled() const;
	
	CL_Signal_v1<CL_RadioButton *> sig_selection_changed;

	std::vector<CL_RadioButton *> buttons;

private:
	void on_button_clicked(CL_RadioButton *button);

	CL_SlotContainer slots;
};

#endif

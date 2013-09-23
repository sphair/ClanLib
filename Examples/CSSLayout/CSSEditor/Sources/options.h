/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "option_value.h"
#include "option_rgb.h"
#include "option_list.h"

class Options : public clan::GUIComponent
{
public:
	Options(clan::GUIManager &gui, clan::Rect gui_position);
	virtual ~Options();

	OptionValue value_margin;
	OptionValue value_padding;
	OptionValue value_border_width;
	OptionValue value_width;
	OptionValue value_height;

	OptionList list_border_style;

	OptionRGB rgb_background;
	OptionRGB rgb_border_color;
	OptionRGB rgb_color;

	OptionList list_background_image;

	bool export_selected;

private:
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	void export_clicked();

private:
	clan::PushButton *pushbutton_export;
};


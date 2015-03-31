/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "..\..\..\ThemeAero\Sources\theme.h"

class ColorWheel : public clan::View
{
public:
	ColorWheel();
	virtual ~ColorWheel();

	std::shared_ptr<clan::SliderView> slider_saturation_outer = Theme::create_slider();
	std::shared_ptr<clan::SliderView> slider_saturation_inner = Theme::create_slider();
	std::shared_ptr<clan::SliderView> slider_value_outer = Theme::create_slider();
	std::shared_ptr<clan::SliderView> slider_value_inner = Theme::create_slider();
	std::shared_ptr<clan::RadioButtonView> radiobutton_HSV = Theme::create_radiobutton();
	std::shared_ptr<clan::RadioButtonView> radiobutton_HSL = Theme::create_radiobutton();

	float saturation_outer = 1.0f;
	float saturation_inner = 0.0f;
	float value_outer = 1.0f;
	float value_inner = 0.0f;
	bool is_hsl = false;

private:
	void draw_labels(clan::Canvas &canvas);
	void draw(clan::Canvas &canvas, const clan::Pointf &center, float radius);
	void render_content(clan::Canvas &canvas) override;
	void get_options();
	float get_value(clan::SliderView *slider);
	void option_changed();
	void set_slider(clan::SliderView *component, int xpos, int ypos);

	clan::Font font;
};

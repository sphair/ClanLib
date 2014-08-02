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

#include "precomp.h"

#include "options.h"

Options::Options(clan::GUIManager &gui, clan::Rect gui_position) : clan::GUIComponent(&gui, clan::GUITopLevelDescription("Options", gui_position, false))
{
	export_selected = false;

	int base_xpos = 4;
	int base_ypos = 10;
	int gap = 20;

	value_margin.setup(this, "margin", base_xpos, base_ypos); base_ypos += gap;
	base_ypos += 8;
	value_padding.setup(this, "padding", base_xpos, base_ypos); base_ypos += gap;
	base_ypos += 8;
	std::vector<std::string> border_style_options;
	border_style_options.push_back("none");
	border_style_options.push_back("solid");
	border_style_options.push_back("thin");
	border_style_options.push_back("medium");
	border_style_options.push_back("thick");
	border_style_options.push_back("hidden");
	border_style_options.push_back("dotted");
	border_style_options.push_back("dashed");
	border_style_options.push_back("double");
	border_style_options.push_back("groove");
	border_style_options.push_back("ridge");
	border_style_options.push_back("inset");
	border_style_options.push_back("outset");

	list_border_style.setup(this, "border-style", base_xpos, base_ypos, border_style_options); base_ypos += gap;
	value_border_width.setup(this, "border-width", base_xpos, base_ypos); base_ypos += gap;
	base_ypos += 8;
	value_width.setup(this, "width", base_xpos, base_ypos); base_ypos += gap;
	value_height.setup(this, "height", base_xpos, base_ypos); base_ypos += gap;

	base_ypos += 8;

	rgb_background.setup(this, "background", base_xpos, base_ypos); base_ypos += gap;
	rgb_border_color.setup(this, "border-color", base_xpos, base_ypos); base_ypos += gap;
	rgb_color.setup(this, "color", base_xpos, base_ypos); base_ypos += gap;

	base_ypos += 8;
	std::vector<std::string> background_image_options;
	background_image_options.push_back("../../GUI/CommonCode/Resources/tux.png");
	background_image_options.push_back("../../Display/LayeredWindow/rock.png");
	background_image_options.push_back("../../Display/LayeredWindow/round_tux.png");
	background_image_options.push_back("../../Display/Basic2D/Resources/logo.png");
	background_image_options.push_back("../../Display/Shape2D/Resources/lobby_background2.png");
	list_background_image.setup(this, "background-image", base_xpos, base_ypos, background_image_options); base_ypos += gap;

	base_ypos += 8;
	pushbutton_export = new clan::PushButton(this);
	pushbutton_export->set_geometry(clan::Rect(base_xpos, base_ypos, clan::Size(128, 20)));
	pushbutton_export->set_text("Launch in Browser");
	pushbutton_export->func_clicked() = bind_member(this, &Options::export_clicked);


	func_render() = bind_member(this, &Options::on_render);

}

Options::~Options()
{

}

void Options::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	clan::Rect rect = get_geometry();
	canvas.fill_rect(rect, clan::Colorf(0.6f, 0.6f, 0.2f, 1.0f));
}

void Options::export_clicked()
{
	export_selected = true;
}

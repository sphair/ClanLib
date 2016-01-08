/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
*/

#include "precomp.h"
#include "theme_views.h"

namespace clan
{
	ThemeButtonView::ThemeButtonView()
	{
		style()->set("border-image-slice: 6 6 5 5 fill;");
		style()->set("border-image-width:6px 6px 5px 5px;");
		style()->set("border-image-repeat:stretch;");
		style()->set("border-image-source:url('Resources/button_normal.png');");
		style("hot")->set("border-image-source:url('Resources/button_hot.png');");
		style("pressed")->set("border-image-source:url('Resources/button_pressed.png');");
		style("disabled")->set("border-image-source:url('Resources/button_disabled.png');");
		label()->style()->set("margin: 5px auto; font: 13px/18px 'Segoe UI'; padding: 0 10px; color: rgb(0,0,0);");
		label()->style("disabled")->set("color: rgb(128,128,128);");
		label()->set_text_alignment(TextAlignment::center);
	}

	ThemeCheckBoxView::ThemeCheckBoxView()
	{
		style()->set("background-position:center center;");
		style()->set("background-repeat:no-repeat;");
		style()->set("background-attachment:scroll;");
		style()->set("width:13px; height:13px");
		style()->set("background-image:url('Resources/checkbox_unchecked_normal.png');");
		style("unchecked_hot")->set("background-image:url('Resources/checkbox_unchecked_hot.png');");
		style("unchecked_pressed")->set("background-image:url('Resources/checkbox_unchecked_pressed.png');");
		style("unchecked_disabled")->set("background-image:url('Resources/checkbox_unchecked_disabled.png');");
		style("checked")->set("background-image:url('Resources/checkbox_checked_normal.png');");
		style("checked_hot")->set("background-image:url('Resources/checkbox_checked_hot.png');");
		style("checked_pressed")->set("background-image:url('Resources/checkbox_checked_pressed.png');");
		style("checked_disabled")->set("background-image:url('Resources/checkbox_checked_disabled.png');");
	}

	ThemeImageView::ThemeImageView()
	{
	}

	ThemeLabelView::ThemeLabelView()
	{
		style()->set("font: 16px 'Segoe UI'; color: black");
	}

	ThemeListBoxView::ThemeListBoxView()
	{
		style()->set("margin: 7px 0; border: 1px solid black; padding: 5px; background: #f0f0f0");
	}

	ThemeListBoxLabelView::ThemeListBoxLabelView(const std::string &text)
	{
		style()->set("font: 13px/17px 'Segoe UI'; color: black; margin: 1px 0; padding: 0 2px");
		style("selected")->set("background: #7777f0; color: white;");
		style("hot")->set("background: #ccccf0; color: black");
		set_text(text);
	}

	ThemeRadioButtonView::ThemeRadioButtonView()
	{
		style()->set("background-position:center center;");
		style()->set("background-repeat:no-repeat;");
		style()->set("background-attachment:scroll;");
		style()->set("width:13px; height:13px");
		style()->set("background-image:url('Resources/radio_unchecked_normal.png');");
		style("unchecked_hot")->set("background-image:url('Resources/radio_unchecked_hot.png');");
		style("unchecked_pressed")->set("background-image:url('Resources/radio_unchecked_pressed.png');");
		style("unchecked_disabled")->set("background-image:url('Resources/radio_unchecked_disabled.png');");
		style("checked")->set("background-image:url('Resources/radio_checked_normal.png');");
		style("checked_hot")->set("background-image:url('Resources/radio_checked_hot.png');");
		style("checked_pressed")->set("background-image:url('Resources/radio_checked_pressed.png');");
		style("checked_disabled")->set("background-image:url('Resources/radio_checked_disabled.png');");
	}

	ThemeScrollView::ThemeScrollView()
	{
		scrollbar_x_view()->style()->set("flex: 0 0 main-size");
		scrollbar_x_view()->style()->set("background: rgb(232, 232, 236)");
		scrollbar_x_view()->track()->style()->set("border-image-slice: 4 0 3 0 fill;");
		scrollbar_x_view()->track()->style()->set("border-image-width:4px 0px 3px 0px;");
		scrollbar_x_view()->track()->style()->set("border-image-repeat:stretch;");
		scrollbar_x_view()->track()->style()->set("border-image-source:url('Resources/scrollbar_hori_track_normal.png');");
		scrollbar_x_view()->track()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_track_hot.png');");
		scrollbar_x_view()->track()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_track_pressed.png');");
		scrollbar_x_view()->track()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_track_disabled.png');");

		scrollbar_x_view()->thumb()->style()->set("border-image-slice: 5 5 5 5 fill;");
		scrollbar_x_view()->thumb()->style()->set("border-image-width:5px 5px 5px 5px;");
		scrollbar_x_view()->thumb()->style()->set("border-image-repeat:stretch;");
		scrollbar_x_view()->thumb()->style()->set("border-image-source:url('Resources/scrollbar_hori_thumb_normal.png');");
		scrollbar_x_view()->thumb()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_thumb_hot.png');");
		scrollbar_x_view()->thumb()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_thumb_pressed.png');");
		scrollbar_x_view()->thumb()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_thumb_disabled.png');");

		scrollbar_x_view()->thumb_grip()->style()->set("background-position:center center;");
		scrollbar_x_view()->thumb_grip()->style()->set("background-repeat:no-repeat;");
		scrollbar_x_view()->thumb_grip()->style()->set("background-attachment:scroll; ");
		scrollbar_x_view()->thumb_grip()->style()->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_normal.png');");
		scrollbar_x_view()->thumb_grip()->style("hot")->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_hot.png');");
		scrollbar_x_view()->thumb_grip()->style("pressed")->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_pressed.png');");
		scrollbar_x_view()->thumb_grip()->style("disabled")->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_disabled.png');");

		scrollbar_x_view()->button_decrement()->style()->set("width:17px; height:17px");
		scrollbar_x_view()->button_decrement()->style()->set("border-image-slice: 3 3 3 3 fill;");
		scrollbar_x_view()->button_decrement()->style()->set("border-image-width:3px 3px 3px 3px;");
		scrollbar_x_view()->button_decrement()->style()->set("border-image-repeat:stretch;");
		scrollbar_x_view()->button_decrement()->style()->set("border-image-source:url('Resources/scrollbar_hori_button_left_normal_withglyph.png');");
		scrollbar_x_view()->button_decrement()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_button_left_hot_withglyph.png');");
		scrollbar_x_view()->button_decrement()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_button_left_pressed_withglyph.png');");
		scrollbar_x_view()->button_decrement()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_button_left_disabled_withglyph.png');");

		scrollbar_x_view()->button_increment()->style()->set("width:17px; height:17px");
		scrollbar_x_view()->button_increment()->style()->set("border-image-slice: 3 3 3 3 fill;");
		scrollbar_x_view()->button_increment()->style()->set("border-image-width:3px 3px 3px 3px;");
		scrollbar_x_view()->button_increment()->style()->set("border-image-repeat:stretch;");
		scrollbar_x_view()->button_increment()->style()->set("border-image-source:url('Resources/scrollbar_hori_button_right_normal_withglyph.png');");
		scrollbar_x_view()->button_increment()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_button_right_hot_withglyph.png');");
		scrollbar_x_view()->button_increment()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_button_right_pressed_withglyph.png');");
		scrollbar_x_view()->button_increment()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_button_right_disabled_withglyph.png');");

		scrollbar_y_view()->style()->set("flex: 0 0 main-size");
		scrollbar_y_view()->style()->set("background: rgb(232, 232, 236)");
		scrollbar_y_view()->track()->style()->set("border-image-slice: 4 0 3 0 fill;");
		scrollbar_y_view()->track()->style()->set("border-image-width:4px 0px 3px 0px;");
		scrollbar_y_view()->track()->style()->set("border-image-repeat:stretch;");
		scrollbar_y_view()->track()->style()->set("border-image-source:url('Resources/scrollbar_vert_track_normal.png');");
		scrollbar_y_view()->track()->style("hot")->set("border-image-source:url('Resources/scrollbar_vert_track_hot.png');");
		scrollbar_y_view()->track()->style("pressed")->set("border-image-source:url('Resources/scrollbar_vert_track_pressed.png');");
		scrollbar_y_view()->track()->style("disabled")->set("border-image-source:url('Resources/scrollbar_vert_track_disabled.png');");

		scrollbar_y_view()->thumb()->style()->set("border-image-slice: 5 5 5 5 fill;");
		scrollbar_y_view()->thumb()->style()->set("border-image-width:5px 5px 5px 5px;");
		scrollbar_y_view()->thumb()->style()->set("border-image-repeat:stretch;");
		scrollbar_y_view()->thumb()->style()->set("border-image-source:url('Resources/scrollbar_vert_thumb_normal.png');");
		scrollbar_y_view()->thumb()->style("hot")->set("border-image-source:url('Resources/scrollbar_vert_thumb_hot.png');");
		scrollbar_y_view()->thumb()->style("pressed")->set("border-image-source:url('Resources/scrollbar_vert_thumb_pressed.png');");
		scrollbar_y_view()->thumb()->style("disabled")->set("border-image-source:url('Resources/scrollbar_vert_thumb_disabled.png');");

		scrollbar_y_view()->thumb_grip()->style()->set("background-position:center center;");
		scrollbar_y_view()->thumb_grip()->style()->set("background-repeat:no-repeat;");
		scrollbar_y_view()->thumb_grip()->style()->set("background-attachment:scroll; ");
		scrollbar_y_view()->thumb_grip()->style()->set("background-image:url('Resources/scrollbar_vert_thumb_gripper_normal.png');");
		scrollbar_y_view()->thumb_grip()->style("hot")->set("background-image:url('Resources/scrollbar_vert_thumb_gripper_hot.png');");
		scrollbar_y_view()->thumb_grip()->style("pressed")->set("background-image:url('Resources/scrollbar_vert_thumb_gripper_pressed.png');");
		scrollbar_y_view()->thumb_grip()->style("disabled")->set("background-image:url('Resources/scrollbar_vert_thumb_gripper_disabled.png');");

		scrollbar_y_view()->button_decrement()->style()->set("width:17px; height:17px");
		scrollbar_y_view()->button_decrement()->style()->set("border-image-slice: 3 3 3 3 fill;");
		scrollbar_y_view()->button_decrement()->style()->set("border-image-width:3px 3px 3px 3px;");
		scrollbar_y_view()->button_decrement()->style()->set("border-image-repeat:stretch;");
		scrollbar_y_view()->button_decrement()->style()->set("border-image-source:url('Resources/scrollbar_vert_button_left_normal_withglyph.png');");
		scrollbar_y_view()->button_decrement()->style("hot")->set("border-image-source:url('Resources/scrollbar_vert_button_left_hot_withglyph.png');");
		scrollbar_y_view()->button_decrement()->style("pressed")->set("border-image-source:url('Resources/scrollbar_vert_button_left_pressed_withglyph.png');");
		scrollbar_y_view()->button_decrement()->style("disabled")->set("border-image-source:url('Resources/scrollbar_vert_button_left_disabled_withglyph.png');");

		scrollbar_y_view()->button_increment()->style()->set("width:17px; height:17px");
		scrollbar_y_view()->button_increment()->style()->set("border-image-slice: 3 3 3 3 fill;");
		scrollbar_y_view()->button_increment()->style()->set("border-image-width:3px 3px 3px 3px;");
		scrollbar_y_view()->button_increment()->style()->set("border-image-repeat:stretch;");
		scrollbar_y_view()->button_increment()->style()->set("border-image-source:url('Resources/scrollbar_vert_button_right_normal_withglyph.png');");
		scrollbar_y_view()->button_increment()->style("hot")->set("border-image-source:url('Resources/scrollbar_vert_button_right_hot_withglyph.png');");
		scrollbar_y_view()->button_increment()->style("pressed")->set("border-image-source:url('Resources/scrollbar_vert_button_right_pressed_withglyph.png');");
		scrollbar_y_view()->button_increment()->style("disabled")->set("border-image-source:url('Resources/scrollbar_vert_button_right_disabled_withglyph.png');");
	}

	ThemeScrollBarView::ThemeScrollBarView()
	{
		set_horizontal();
		style()->set("flex: 0 0 main-size");
		style()->set("background: rgb(232, 232, 236)");
		track()->style()->set("border-image-slice: 4 0 3 0 fill;");
		track()->style()->set("border-image-width:4px 0px 3px 0px;");
		track()->style()->set("border-image-repeat:stretch;");
		track()->style()->set("border-image-source:url('Resources/scrollbar_hori_track_normal.png');");
		track()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_track_hot.png');");
		track()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_track_pressed.png');");
		track()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_track_disabled.png');");

		thumb()->style()->set("border-image-slice: 5 5 5 5 fill;");
		thumb()->style()->set("border-image-width:5px 5px 5px 5px;");
		thumb()->style()->set("border-image-repeat:stretch;");
		thumb()->style()->set("border-image-source:url('Resources/scrollbar_hori_thumb_normal.png');");
		thumb()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_thumb_hot.png');");
		thumb()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_thumb_pressed.png');");
		thumb()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_thumb_disabled.png');");

		thumb_grip()->style()->set("background-position:center center;");
		thumb_grip()->style()->set("background-repeat:no-repeat;");
		thumb_grip()->style()->set("background-attachment:scroll; ");
		thumb_grip()->style()->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_normal.png');");
		thumb_grip()->style("hot")->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_hot.png');");
		thumb_grip()->style("pressed")->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_pressed.png');");
		thumb_grip()->style("disabled")->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_disabled.png');");

		button_decrement()->style()->set("width:17px; height:17px");
		button_decrement()->style()->set("border-image-slice: 3 3 3 3 fill;");
		button_decrement()->style()->set("border-image-width:3px 3px 3px 3px;");
		button_decrement()->style()->set("border-image-repeat:stretch;");
		button_decrement()->style()->set("border-image-source:url('Resources/scrollbar_hori_button_left_normal_withglyph.png');");
		button_decrement()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_button_left_hot_withglyph.png');");
		button_decrement()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_button_left_pressed_withglyph.png');");
		button_decrement()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_button_left_disabled_withglyph.png');");

		button_increment()->style()->set("width:17px; height:17px");
		button_increment()->style()->set("border-image-slice: 3 3 3 3 fill;");
		button_increment()->style()->set("border-image-width:3px 3px 3px 3px;");
		button_increment()->style()->set("border-image-repeat:stretch;");
		button_increment()->style()->set("border-image-source:url('Resources/scrollbar_hori_button_right_normal_withglyph.png');");
		button_increment()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_button_right_hot_withglyph.png');");
		button_increment()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_button_right_pressed_withglyph.png');");
		button_increment()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_button_right_disabled_withglyph.png');");
	}

	ThemeSliderView::ThemeSliderView()
	{
		set_horizontal();

		style()->set("flex-direction: row;");
		track()->style()->set("flex: 1 1 main-size;");
		track()->style()->set("height: 4px;");
		track()->style()->set("margin: 7px 0px");
		track()->style()->set("border-image-slice: 1 2 1 1 fill;");
		track()->style()->set("border-image-width:1px 2px 1px 1px;");
		track()->style()->set("border-image-repeat:stretch;");
		track()->style()->set("border-image-source:url('Resources/slider_track.png');");

		thumb()->style()->set("position: absolute;");
		thumb()->style()->set("width:11px;");
		thumb()->style()->set("height:19px;");
		thumb()->style()->set("border-image-slice:9 3 9 2 fill;");
		thumb()->style()->set("border-image-width:9px 3px 9px 2px;");
		thumb()->style()->set("border-image-repeat:stretch;");
		thumb()->style()->set("border-image-source:url('Resources/slider_horizontal_thumb_normal.png');");
		thumb()->style("hot")->set("border-image-source:url('Resources/slider_horizontal_thumb_hot.png');");
		thumb()->style("pressed")->set("border-image-source:url('Resources/slider_horizontal_thumb_pressed.png');");
		thumb()->style("disabled")->set("border-image-source:url('Resources/slider_horizontal_thumb_disabled.png');");
	}

	ThemeSpinView::ThemeSpinView()
	{
	}

	ThemeTextFieldView::ThemeTextFieldView()
	{
	}

	ThemeTextView::ThemeTextView()
	{
	}
}

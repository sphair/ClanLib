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
#include "theme.h"

using namespace clan;

std::shared_ptr<clan::ScrollBarView> Theme::create_scrollbar()
{
	auto scrollbar = std::make_shared<clan::ScrollBarView>(false);
	scrollbar->set_horizontal();
	scrollbar->style()->set("flex: 0 0 auto");
	scrollbar->style()->set("background: rgb(232, 232, 236)");
	scrollbar->track()->style()->set("border-image-slice: 4 0 3 0 fill;");
	scrollbar->track()->style()->set("border-image-width:4px 0px 3px 0px;");
	scrollbar->track()->style()->set("border-image-repeat:stretch;");
	scrollbar->track()->style()->set("border-image-source:url('Resources/scrollbar_hori_track_normal.png');");
	scrollbar->track()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_track_hot.png');");
	scrollbar->track()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_track_pressed.png');");
	scrollbar->track()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_track_disabled.png');");

	scrollbar->thumb()->style()->set("border-image-slice: 5 5 5 5 fill;");
	scrollbar->thumb()->style()->set("border-image-width:5px 5px 5px 5px;");
	scrollbar->thumb()->style()->set("border-image-repeat:stretch;");
	scrollbar->thumb()->style()->set("border-image-source:url('Resources/scrollbar_hori_thumb_normal.png');");
	scrollbar->thumb()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_thumb_hot.png');");
	scrollbar->thumb()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_thumb_pressed.png');");
	scrollbar->thumb()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_thumb_disabled.png');");

	scrollbar->thumb_grip()->style()->set("background-position:center center;");
	scrollbar->thumb_grip()->style()->set("background-repeat:no-repeat;");
	scrollbar->thumb_grip()->style()->set("background-attachment:scroll; ");
	scrollbar->thumb_grip()->style()->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_normal.png');");
	scrollbar->thumb_grip()->style("hot")->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_hot.png');");
	scrollbar->thumb_grip()->style("pressed")->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_pressed.png');");
	scrollbar->thumb_grip()->style("disabled")->set("background-image:url('Resources/scrollbar_hori_thumb_gripper_disabled.png');");

	scrollbar->button_decrement()->style()->set("width:17px; height:17px");
	scrollbar->button_decrement()->style()->set("border-image-slice: 3 3 3 3 fill;");
	scrollbar->button_decrement()->style()->set("border-image-width:3px 3px 3px 3px;");
	scrollbar->button_decrement()->style()->set("border-image-repeat:stretch;");
	scrollbar->button_decrement()->style()->set("border-image-source:url('Resources/scrollbar_hori_button_left_normal_withglyph.png');");
	scrollbar->button_decrement()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_button_left_hot_withglyph.png');");
	scrollbar->button_decrement()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_button_left_pressed_withglyph.png');");
	scrollbar->button_decrement()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_button_left_disabled_withglyph.png');");

	scrollbar->button_increment()->style()->set("width:17px; height:17px");
	scrollbar->button_increment()->style()->set("border-image-slice: 3 3 3 3 fill;");
	scrollbar->button_increment()->style()->set("border-image-width:3px 3px 3px 3px;");
	scrollbar->button_increment()->style()->set("border-image-repeat:stretch;");
	scrollbar->button_increment()->style()->set("border-image-source:url('Resources/scrollbar_hori_button_right_normal_withglyph.png');");
	scrollbar->button_increment()->style("hot")->set("border-image-source:url('Resources/scrollbar_hori_button_right_hot_withglyph.png');");
	scrollbar->button_increment()->style("pressed")->set("border-image-source:url('Resources/scrollbar_hori_button_right_pressed_withglyph.png');");
	scrollbar->button_increment()->style("disabled")->set("border-image-source:url('Resources/scrollbar_hori_button_right_disabled_withglyph.png');");
	return scrollbar;
}

std::shared_ptr<clan::ButtonView> Theme::create_button()
{
	auto button = std::make_shared<clan::ButtonView>();
	button->style()->set("border-image-slice: 6 6 5 5 fill;");
	button->style()->set("border-image-width:6px 6px 5px 5px;");
	button->style()->set("border-image-repeat:stretch;");
	button->style()->set("border-image-source:url('Resources/button_normal.png');");
	button->style("hot")->set("border-image-source:url('Resources/button_hot.png');");
	button->style("pressed")->set("border-image-source:url('Resources/button_pressed.png');");
	button->style("disabled")->set("border-image-source:url('Resources/button_disabled.png');");
	button->label()->style()->set("margin: 5px auto; font: 13px/1.5 'Segoe UI'; padding: 3px 10px; color: rgb(0,0,0);");
	button->label()->style("disabled")->set("color: rgb(128,128,128);");
	button->label()->style("pressed")->set("font-weight: bold;");
	button->label()->set_text_alignment(TextAlignment::center);
	return button;
}

std::shared_ptr<clan::SliderView> Theme::create_slider()
{
	std::shared_ptr<clan::SliderView> slider = std::make_shared<clan::SliderView>();
	slider->set_horizontal();

	slider->style()->set("flex-direction: row;");
	slider->track()->style()->set("flex: 1 1 auto;");
	slider->track()->style()->set("height: 4px;");
	slider->track()->style()->set("margin: 7px 0px");
	slider->track()->style()->set("border-image-slice: 1 2 1 1 fill;");
	slider->track()->style()->set("border-image-width:1px 2px 1px 1px;");
	slider->track()->style()->set("border-image-repeat:stretch;");
	slider->track()->style()->set("border-image-source:url('Resources/slider_track.png');");

	slider->thumb()->style()->set("position: absolute;");
	slider->thumb()->style()->set("width:11px;");
	slider->thumb()->style()->set("height:19px;");
	slider->thumb()->style()->set("border-image-slice:9 3 9 2 fill;");
	slider->thumb()->style()->set("border-image-width:9px 3px 9px 2px;");
	slider->thumb()->style()->set("border-image-repeat:stretch;");
	slider->thumb()->style()->set("border-image-source:url('Resources/slider_horizontal_thumb_normal.png');");
	slider->thumb()->style("hot")->set("border-image-source:url('Resources/slider_horizontal_thumb_hot.png');");
	slider->thumb()->style("pressed")->set("border-image-source:url('Resources/slider_horizontal_thumb_pressed.png');");
	slider->thumb()->style("disabled")->set("border-image-source:url('Resources/slider_horizontal_thumb_disabled.png');");

	return slider;
}

std::shared_ptr<clan::CheckBoxView> Theme::create_checkbox()
{
	auto checkbox = std::make_shared<clan::CheckBoxView>();

	checkbox->style()->set("background-position:center center;");
	checkbox->style()->set("background-repeat:no-repeat;");
	checkbox->style()->set("background-attachment:scroll;");
	checkbox->style()->set("width:13px; height:13px");
	checkbox->style()->set("background-image:url('Resources/checkbox_unchecked_normal.png');");
	checkbox->style("unchecked_hot")->set("background-image:url('Resources/checkbox_unchecked_hot.png');");
	checkbox->style("unchecked_pressed")->set("background-image:url('Resources/checkbox_unchecked_pressed.png');");
	checkbox->style("unchecked_disabled")->set("background-image:url('Resources/checkbox_unchecked_disabled.png');");
	checkbox->style("checked")->set("background-image:url('Resources/checkbox_checked_normal.png');");
	checkbox->style("checked_hot")->set("background-image:url('Resources/checkbox_checked_hot.png');");
	checkbox->style("checked_pressed")->set("background-image:url('Resources/checkbox_checked_pressed.png');");
	checkbox->style("checked_disabled")->set("background-image:url('Resources/checkbox_checked_disabled.png');");
	return checkbox;
}

std::shared_ptr<clan::RadioButtonView> Theme::create_radiobutton()
{
	auto radio = std::make_shared<clan::RadioButtonView>();
	radio->style()->set("background-position:center center;");
	radio->style()->set("background-repeat:no-repeat;");
	radio->style()->set("background-attachment:scroll;");
	radio->style()->set("width:13px; height:13px");
	radio->style()->set("background-image:url('Resources/radio_unchecked_normal.png');");
	radio->style("unchecked_hot")->set("background-image:url('Resources/radio_unchecked_hot.png');");
	radio->style("unchecked_pressed")->set("background-image:url('Resources/radio_unchecked_pressed.png');");
	radio->style("unchecked_disabled")->set("background-image:url('Resources/radio_unchecked_disabled.png');");
	radio->style("checked")->set("background-image:url('Resources/radio_checked_normal.png');");
	radio->style("checked_hot")->set("background-image:url('Resources/radio_checked_hot.png');");
	radio->style("checked_pressed")->set("background-image:url('Resources/radio_checked_pressed.png');");
	radio->style("checked_disabled")->set("background-image:url('Resources/radio_checked_disabled.png');");
	return radio;
}

std::shared_ptr<clan::LabelView> Theme::create_label(bool subpixel)
{
	auto label = std::make_shared<clan::LabelView>();
	label->style()->set("font: 16px Tahoma; color: white");
	if (!subpixel)
		label->style()->set("-clan-font-rendering: anti-alias;");
	return label;
}

std::shared_ptr<clan::ListBoxView> Theme::create_listbox()
{
	auto listbox = std::make_shared<clan::ListBoxView>();
	listbox->style()->set("margin: 7px 0; border: 1px solid black; padding: 5px; background: #f0f0f0");
	return listbox;
}

std::shared_ptr<clan::LabelView> Theme::create_listbox_label(const std::string &text)
{
	auto label = std::make_shared<clan::LabelView>();
	label->style()->set("font: 13px/17px 'Segoe UI'; color: black; margin: 1px 0; padding: 0 2px");
	label->style("selected")->set("background: #7777f0; color: white;");
	label->style("hot")->set("background: #ccccf0; color: black");
	label->set_text(text);
	return label;
}

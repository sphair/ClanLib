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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "helloworld.h"

using namespace clan;

clan::ApplicationInstance<HelloWorld> clanapp;

HelloWorld::HelloWorld()
{
	// We support all display targets, in order listed here
	//clan::D3DTarget::enable();
	clan::OpenGLTarget::enable();

	// Create a source for our resources
	ResourceManager resources = FileResourceManager::create();

	// Mark this thread as the UI thread
	ui_thread = UIThread(resources);

	// Create root view and window:
	DisplayWindowDescription desc;
	desc.set_title("UICore: Hello World");
	desc.set_allow_resize(true);
	desc.set_size(Sizef(640, 600), false);
	root = std::make_shared<WindowView>(desc);

	// Exit run loop when close is clicked.
	// We have to store the return Slot because if it is destroyed the lambda function is disconnected from the signal.
	slot_close = root->sig_close().connect([&](CloseEvent &e) { RunLoop::exit(); });

	// Style the root view to use rounded corners and a bit of drop shadow
	root->style()->set("padding: 11px");
	root->style()->set("background: #efefef");
	root->style()->set("flex-direction: column");

	auto body = std::make_shared<View>();
	body->style()->set("background: white");
	body->style()->set("padding: 11px");
	body->style()->set("border-top: 5px solid #DD3B2A");
	body->style()->set("border-bottom: 5px solid #DD3B2A");
	body->style()->set("flex-direction: column");
	body->style()->set("flex: 1 1 main-size");
	root->add_subview(body);

	// Create a label with some text to have some content
	label = std::make_shared<LabelView>();
	label->style()->set("font: 20px/40px 'Ravie'");
	label->style()->set("color: #DD3B2A");
	label->set_text("Hello World!");
	body->add_subview(label);

	// React to clicking
	label->slots.connect(label->sig_pointer_press(), [&](PointerEvent &e) {
		label->set_text(label->text() + " CLICK!");
	});

	// Create a text field for our span layout
	std::shared_ptr<TextFieldView> edit = std::make_shared<TextFieldView>();
	edit->style()->set("font: 11px/20px 'Segoe UI'");
	edit->style()->set("margin: 5px");
	edit->style()->set("background: #efefef");
	edit->style()->set("border: 1px solid black");
	edit->style()->set("border-radius: 3px");
	edit->style()->set("padding: 2px 5px 2px 5px");
	edit->style()->set("width: 128px");
	edit->style()->set("box-shadow: 0 0 5px rgba(100,100,200,0.2)");
	edit->set_text("amazing!");

	// Create some text styles for the text we will write
	std::shared_ptr<Style> normal = std::make_shared<Style>();
	std::shared_ptr<Style> bold = std::make_shared<Style>();
	std::shared_ptr<Style> italic = std::make_shared<Style>();
	normal->set("font: 13px/25px 'Segoe UI'");
	bold->set("font: 13px/25px 'Segoe UI'; font-weight: bold");
	italic->set("font: 13px/25px 'Segoe UI'; font-style: italic");

	// Create a span layout views with some more complex inline formatting
	std::shared_ptr<SpanLayoutView> p1 = std::make_shared<SpanLayoutView>();
	p1->add_text("This is an example of why Sphair should never ever make fun of my ", normal);
	p1->add_text("BEAUTIFUL", bold);
	p1->add_text(" green 13.37deg gradients because he will never know what it is replaced with!", normal);
	body->add_subview(p1);

	std::shared_ptr<SpanLayoutView> p2 = std::make_shared<SpanLayoutView>();
	p2->style()->set("margin: 15px 0 5px 0");
	p2->style()->set("padding: 7px");
	p2->style()->set("border-top: 5px solid #CCE4FB");
	p2->style()->set("border-bottom: 5px solid #CCE4FB");
	p2->style()->set("background: #EDF6FF");
	p2->add_text("If you also think Sphair made a ", normal);
	p2->add_text("BIG MISTAKE", bold);
	p2->add_text(" please consider typing ", normal);
	p2->add_text("Yes, yes, yes, yes, yes, yes, yes yes, YES!", italic);
	p2->add_text(" in the text field: ", normal);
	p2->add_subview(edit);
	p2->add_text(" You know you want to!", bold);
	body->add_subview(p2);
	
	std::shared_ptr<SpanLayoutView> p3 = std::make_shared<SpanLayoutView>();
	p3->add_text("Since we both know you typed ", normal);
	p3->add_text("Yes, yes, yes..", italic);
	p3->add_text(" into the text field (who wouldn't!?), here's the amazing gradient:", normal);
	body->add_subview(p3);
	
	std::shared_ptr<View> gradient_box = std::make_shared<View>();
	gradient_box->style()->set("margin: 15px auto; width: 120px; height: 75px;");
	gradient_box->style()->set("border: 1px solid #777");
	gradient_box->style()->set("background: linear-gradient(13.37deg, #f0f0f0, rgb(120,240,120) 50%, #f0f0f0)");
	gradient_box->style()->set("box-shadow: 7px 7px 7px rgba(0,0,0,0.2)");
	body->add_subview(gradient_box);

	auto scrollbar = std::make_shared<clan::ScrollBarView>(false);
	scrollbar->set_horizontal();
	scrollbar->style()->set("flex: 0 0 main-size");
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

	//scrollbar->set_disabled();

	scrollbar->set_range(0.0, 1.0);
	scrollbar->set_position(0.5);
	scrollbar->set_page_step(0.1);
	scrollbar->set_line_step(0.01);
	body->add_subview(scrollbar);


	auto button = std::make_shared<clan::ButtonView>();

	button->style()->set("border-image-slice: 6 6 5 5 fill;");
	button->style()->set("border-image-width:6px 6px 5px 5px;");
	button->style()->set("border-image-repeat:stretch;");
	button->style()->set("border-image-source:url('Resources/button_normal.png');");
	button->style("hot")->set("border-image-source:url('Resources/button_hot.png');");
	button->style("pressed")->set("border-image-source:url('Resources/button_pressed.png');");
	button->style("disabled")->set("border-image-source:url('Resources/button_disabled.png');");
	button->label()->style()->set("margin: 5px auto; font: 13px/1.5 'Segoe UI'; padding: 3px 10px");
	button->label()->style("pressed")->set("font-weight: bold");
	button->label()->set_text_alignment(TextAlignment::center);
	button->label()->set_text("This is a button");

	body->add_subview(button);


	std::shared_ptr<clan::SliderView> slider = std::make_shared<clan::SliderView>();
	slider->set_horizontal();

	slider->style()->set("flex-direction: row;");
	slider->style()->set("background: rgb(200, 200, 255)");

	slider->track()->style()->set("flex: 1 1 main-size;");
	slider->track()->style()->set("height: 4px;");
	slider->track()->style()->set("margin: 7px 0px");
	slider->track()->style()->set("background: rgb(255, 0, 0)");
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

	//slider->set_disabled();

	slider->set_min_position(0);
	slider->set_max_position(1000);
	slider->set_tick_count(100);
	slider->set_lock_to_ticks(false);
	slider->set_page_step(100);
	slider->set_position(slider->max_position()/2);

	body->add_subview(slider);

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
	//checkbox->set_disabled();
	body->add_subview(checkbox);

	for (int cnt = 0; cnt < 3; cnt++)
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
		//radio->set_disabled(true);
		body->add_subview(radio);
	}

/*	auto spin = std::make_shared<clan::SpinView>();

	spin->style()->set("border-image-slice: 6 6 5 5 fill;");
	spin->style()->set("border-image-width:6px 6px 5px 5px;");
	spin->style()->set("border-image-repeat:stretch;");
	spin->style()->set("border-image-source:url('Resources/spin_normal.png');");
	spin->style("hot")->set("border-image-source:url('Resources/spin_hot.png');");
	spin->style("pressed")->set("border-image-source:url('Resources/spin_pressed.png');");
	spin->style("disabled")->set("border-image-source:url('Resources/spin_disabled.png');");
	body->add_subview(spin);
*/

	// Make our window visible
	root->show();
}

bool HelloWorld::update()
{
	return true;
}

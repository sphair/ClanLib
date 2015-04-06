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
#include "../../../ThemeAero/Sources/theme.h"

using namespace clan;

clan::ApplicationInstance<HelloWorld> clanapp;

HelloWorld::HelloWorld()
{
	// We support all display targets, in order listed here
	//clan::D3DTarget::enable();
	clan::OpenGLTarget::enable();

	// Create a source for our resources
	FileResourceDocument doc(FileSystem("../../ThemeAero"));
	ResourceManager resources = FileResourceManager::create(doc);

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
	body->style()->set("flex: auto");
	root->add_subview(body);

	// Create a label with some text to have some content
	label = std::make_shared<LabelView>();
	label->style()->set("flex: none");
	label->style()->set("font: 20px/40px 'Ravie'");
	label->style()->set("color: #DD3B2A");
	label->set_text("Hello World!");
	body->add_subview(label);

	// React to clicking
	label->slots.connect(label->sig_pointer_press(), [&](PointerEvent &e) {
		label->set_text(label->text() + " CLICK!");
	});

	auto scrollarea = std::make_shared<ScrollView>();
	scrollarea->style()->set("margin: 5px 0; border: 1px solid black; padding: 5px 5px;");
	scrollarea->content_view()->style()->set("flex-direction: column");
	body->add_subview(scrollarea);

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
	scrollarea->content_view()->add_subview(p1);

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
	scrollarea->content_view()->add_subview(p2);
	
	std::shared_ptr<SpanLayoutView> p3 = std::make_shared<SpanLayoutView>();
	p3->add_text("Since we both know you typed ", normal);
	p3->add_text("Yes, yes, yes..", italic);
	p3->add_text(" into the text field (who wouldn't!?), here's the amazing gradient:", normal);
	scrollarea->content_view()->add_subview(p3);
	
	std::shared_ptr<View> gradient_box = std::make_shared<View>();
	gradient_box->style()->set("margin: 15px auto; width: 120px; height: 75px;");
	gradient_box->style()->set("border: 1px solid #777");
	gradient_box->style()->set("background: linear-gradient(13.37deg, #f0f0f0, rgb(120,240,120) 50%, #f0f0f0)");
	gradient_box->style()->set("box-shadow: 7px 7px 7px rgba(0,0,0,0.2)");
	scrollarea->content_view()->add_subview(gradient_box);

	auto scrollbar = Theme::create_scrollbar();
	//scrollbar->set_disabled();
	scrollbar->set_range(0.0, 1.0);
	scrollbar->set_position(0.5);
	scrollbar->set_page_step(0.1);
	scrollbar->set_line_step(0.01);
	scrollarea->content_view()->add_subview(scrollbar);

	auto button = Theme::create_button();
	button->label()->set_text("This is a button");
	scrollarea->content_view()->add_subview(button);

	std::shared_ptr<clan::SliderView> slider = Theme::create_slider();
	//slider->set_disabled();
	slider->set_min_position(0);
	slider->set_max_position(1000);
	slider->set_tick_count(100);
	slider->set_lock_to_ticks(false);
	slider->set_page_step(100);
	slider->set_position(slider->max_position()/2);
	scrollarea->content_view()->add_subview(slider);

	auto checkbox = Theme::create_checkbox();
	//checkbox->set_disabled();
	scrollarea->content_view()->add_subview(checkbox);

	for (int cnt = 0; cnt < 3; cnt++)
	{
		auto radio = Theme::create_radiobutton();
		//radio->set_disabled(true);
		scrollarea->content_view()->add_subview(radio);
	}

	// Make our window visible
	root->show();
}

bool HelloWorld::update()
{
	return true;
}

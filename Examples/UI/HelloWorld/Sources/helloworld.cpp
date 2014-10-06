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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "helloworld.h"

using namespace clan;

// The start of the Application
int HelloWorld::start(const std::vector<std::string> &args)
{
	// Create root view and window:
	DisplayWindowDescription desc;
	desc.set_title("UICore: Hello World");
	desc.set_allow_resize(true);
	desc.set_extend_frame(16, 40, 16, 16);
	std::shared_ptr<WindowView> root = std::make_shared<WindowView>(desc);

	// Exit run loop when close is clicked.
	// We have to store the return Slot because if it is destroyed the lambda function is disconnected from the signal.
	Slot slot_close = root->sig_close().connect([&](CloseEvent &e) { exit(); });

	// Style the root view to use rounded corners and a bit of drop shadow
	root->style.set_background(Colorf(240, 240, 240, 255));
	root->style.set_padding(11.0f);
	root->style.set_border_radius(15.0f);
	root->style.set_border(Colorf(0, 0, 0), 1.0f);
	root->style.set_margin(10.0f, 35.0f, 10.0f, 10.0f);
	root->style.set_box_shadow(Colorf(0, 0, 0, 50), 0.0f, 0.0f, 20.0f);

	// Create a label with some text to have some content
	std::shared_ptr<LabelView> label = std::make_shared<LabelView>();
	FontDescription font_desc("Ravie");
	font_desc.set_height(20.0f);
	font_desc.set_line_height(40.0f);
	label->set_font(font_desc);
	label->set_text("Hello World!");
	root->add_subview(label);

	// React to clicking
	label->slots.connect(label->sig_pointer_press(), [&](PointerEvent &e) {
		label->set_text(label->text() + " CLICK!");
	});

	// Create a text field for our span layout
	std::shared_ptr<TextFieldView> edit = std::make_shared<TextFieldView>();
	FontDescription font_desc6("Ravie");
	font_desc6.set_typeface_name("Ravie");
	font_desc6.set_height(11.0f);
	font_desc6.set_line_height(20.0f);
	edit->set_font(font_desc6);
	edit->set_text("42");
	edit->style.set_margin(0.0f, 5.0f);
	edit->style.set_background(Colorf(255, 255, 255));
	edit->style.set_border(Colorf(0.0f, 0.0f, 0.0f), 1.0f);
	edit->style.set_border_radius(3.0f);
	edit->style.set_padding(5.0f, 2.0f, 5.0f, 3.0f);
	edit->style.set_width(35.0f);

	// Create a span layout view with some more complex inline formatting
	std::shared_ptr<SpanLayoutView> span = std::make_shared<SpanLayoutView>();
	FontDescription font_desc2;
	font_desc2.set_typeface_name("Segoe UI");
	font_desc2.set_height(13.0f);
	font_desc2.set_line_height(40.0f);
	Canvas canvas = SharedGCData::get_resource_canvas();
	span->add_text("This is the UI core ", Font(canvas, font_desc2));
	FontDescription font_desc3;
	font_desc3.set_typeface_name("Segoe UI");
	font_desc3.set_height(18.0f);
	font_desc3.set_line_height(40.0f);
	span->add_text("Hello World!", Font(canvas, font_desc3));
	FontDescription font_desc4;
	font_desc4.set_typeface_name("Segoe UI");
	font_desc4.set_height(13.0f);
	font_desc4.set_line_height(40.0f);
	span->add_text(" example! Here's a text field: ", Font(canvas, font_desc4));
	span->add_subview(edit);
	FontDescription font_desc5;
	font_desc5.set_typeface_name("Segoe UI");
	font_desc5.set_height(16.0f);
	font_desc5.set_line_height(40.0f);
	font_desc5.set_weight(800);
	span->add_text(" units! sdfjghsdkfj hkjsdfhg jksdhfj gkshdfk gsjdkfghsjkdfh kgjshdfkg sjkdfh gjskhf gskjdfg hkjsdfh kgjsdhfkgjhsdkjfhgksjdfhg kjsdfhgjkshdfkhgskjdf ghkjsdfsg kdfhg skjdfhgjksdh fgsdfhg kjsdhfjkghsdkjfh gkjsdhfjkgsdhfkgjhsdkfj hgksj.", Font(canvas, font_desc5));
	root->add_subview(span);

	// Make our window visible
	root->show();

	// Process messages until user exits
	run();


	return 0;
}

void HelloWorld::run()
{
	exit_flag = false;
	while (!exit_flag)
	{
		clan::KeepAlive::process(250);
	}
}

void HelloWorld::exit()
{
	exit_flag = true;
}



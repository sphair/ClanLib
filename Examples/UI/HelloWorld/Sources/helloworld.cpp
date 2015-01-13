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

// The start of the Application
int HelloWorld::start(const std::vector<std::string> &args)
{
	// Create a source for our resources
	ResourceManager resources = FileResourceManager::create();

	// Mark this thread as the UI thread
	UIThread ui_thread(resources);

	// Create root view and window:
	DisplayWindowDescription desc;
	desc.set_title("UICore: Hello World");
	desc.set_allow_resize(true);
	desc.set_type(WindowType::custom);
	desc.set_extend_frame(16, 40, 16, 16);
	std::shared_ptr<WindowView> root = std::make_shared<WindowView>(desc);

	// Exit run loop when close is clicked.
	// We have to store the return Slot because if it is destroyed the lambda function is disconnected from the signal.
	Slot slot_close = root->sig_close().connect([&](CloseEvent &e) { exit(); });

	// Style the root view to use rounded corners and a bit of drop shadow
	root->box_style.set_background(Colorf(240, 240, 240, 255));
	root->box_style.set_padding(11.0f);
	root->box_style.set_border_radius(15.0f);
	root->box_style.set_border(Colorf(0, 0, 0), 1.0f);
	root->box_style.set_margin(10.0f, 35.0f, 10.0f, 10.0f);
	root->box_style.set_box_shadow(Colorf(0, 0, 0, 50), 0.0f, 0.0f, 20.0f);

	// Create a label with some text to have some content
	std::shared_ptr<LabelView> label = std::make_shared<LabelView>();
	label->text_style().set_font("Ravie", 20.0f, 40.0f);
	label->set_text("Hello World!");
	root->add_subview(label);

	// React to clicking
	label->slots.connect(label->sig_pointer_press(), [&](PointerEvent &e) {
		label->set_text(label->text() + " CLICK!");
	});

	// Create a text field for our span layout
	std::shared_ptr<TextFieldView> edit = std::make_shared<TextFieldView>();
	edit->text_style().set_font("Segoe UI", 11.0f, 20.0f);
	edit->set_text("Text File View");
	edit->box_style.set_margin(0.0f, 5.0f);
	edit->box_style.set_background(Colorf(255, 255, 255));
	edit->box_style.set_border(Colorf(0.0f, 0.0f, 0.0f), 1.0f);
	edit->box_style.set_border_radius(3.0f);
	edit->box_style.set_padding(5.0f, 2.0f, 5.0f, 3.0f);
	edit->box_style.set_width(128.0f);

	// Create a span layout view with some more complex inline formatting
	std::shared_ptr<SpanLayoutView> span = std::make_shared<SpanLayoutView>();
	TextStyle font_desc2;
	font_desc2.set_font_family("Segoe UI");
	font_desc2.set_size(13.0f);
	font_desc2.set_line_height(40.0f);
	span->add_text("This is the UI core ", font_desc2);

	span->add_subview(edit);

	TextStyle font_desc5;
	font_desc5.set_font_family("Segoe UI");
	font_desc5.set_size(16.0f);
	font_desc5.set_line_height(40.0f);
	font_desc5.set_weight(FontWeight::extra_bold);
	span->add_text(" units!", font_desc5);
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
		clan::RunLoop::process(250);
	}
}

void HelloWorld::exit()
{
	exit_flag = true;
}



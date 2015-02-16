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
	Slot slot_close = root->sig_close().connect([&](CloseEvent &e) { RunLoop::exit(); });

	// Style the root view to use rounded corners and a bit of drop shadow
	root->style()->set("background: linear-gradient(13.37deg, #f0f0f0, rgb(120,240,120) 50%, #f0f0f0)");
	root->style()->set("padding: 11px");
	root->style()->set("border: 1px solid black");
	root->style()->set("border-radius: 15px");
	root->style()->set("margin: 35px 10px 10px 10px");
	root->style()->set("box-shadow: 0 0 20px rgba(0,0,0,0.2)");
	root->style()->set("flex-direction: column");

	// Create a label with some text to have some content
	std::shared_ptr<LabelView> label = std::make_shared<LabelView>();
	label->style()->set("font: 20px/40px 'Ravie'");
	label->set_text("Hello World!");
	root->add_subview(label);

	// React to clicking
	label->slots.connect(label->sig_pointer_press(), [&](PointerEvent &e) {
		label->set_text(label->text() + " CLICK!");
	});

	// Create a text field for our span layout
	std::shared_ptr<TextFieldView> edit = std::make_shared<TextFieldView>();
	edit->style()->set("font: 11px/20px 'Segoe UI'");
	edit->style()->set("margin: 5px 0");
	edit->style()->set("background: white");
	edit->style()->set("border: 1px solid black");
	edit->style()->set("border-radius: 3px");
	edit->style()->set("padding: 2px 5px 2px 5px");
	edit->style()->set("width: 128px");
	edit->set_text("Text File View");

	// Create a span layout view with some more complex inline formatting
	std::shared_ptr<SpanLayoutView> span = std::make_shared<SpanLayoutView>();
	std::shared_ptr<Style> text_style = std::make_shared<Style>();
	text_style->set("font: 13px/40px 'Segoe UI'");
	span->add_text("This is the UI core ", text_style);

	span->add_subview(edit);
	/*
	std::shared_ptr<ScrollBarView> scrollbar = std::make_shared<ScrollBarView>();
	scrollbar->set_horizontal();
	scrollbar->style()->set("flex: 0 0 main-size");
	scrollbar->style()->set("background: rgb(232, 232, 236)");
	scrollbar->track()->set("padding: 0 4px");
	scrollbar->track()->set("background: rgb(208, 209, 215)");
	scrollbar->thumb()->set("padding: 0 4px");
	scrollbar->thumb()->set("background: rgb(208, 209, 215)");
	scrollbar->set_range(0.0, 1.0);
	scrollbar->set_position(0.5);
	scrollbar->set_page_step(0.1);
	scrollbar->set_line_step(0.01);
	root->add_subview(scrollbar);
	*/
	std::shared_ptr<Style> text_style2 = std::make_shared<Style>();
	text_style2->set("font: 16px/40px 'Segoe UI'; font-weight: 800");
	span->add_text(" units!", text_style2);
	root->add_subview(span);

	// Make our window visible
	root->show();

	// Process messages until user exits
	RunLoop::run();

	return 0;
}

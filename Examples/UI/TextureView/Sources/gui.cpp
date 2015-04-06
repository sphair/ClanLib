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
#include "gui.h"
#include "..\..\..\ThemeAero\Sources\theme.h"

clan::ApplicationInstance<GUI> clanapp;

GUI::GUI()
{
	// We support all display targets, in order listed here
	//clan::D3DTarget::enable();
	clan::OpenGLTarget::enable();

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib TextureView GUI Example");
	desc.set_size(clan::Size(640, 640), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), [&](){quit = true; });

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &GUI::on_input_up));

	font = clan::Font("tahoma", 24);

	clan::Texture2D gui_texture = clan::Texture2D(canvas, 512, 512);
	gui_texture.set_pixel_ratio(canvas.get_pixel_ratio());
	gui_image = clan::Image(gui_texture, gui_texture.get_size());
	clan::FrameBuffer gui_framebuffer = clan::FrameBuffer(canvas);
	gui_framebuffer.attach_color(0, gui_texture);
	//gui_canvas = clan::Canvas(canvas);//, gui_framebuffer);
	gui_canvas = clan::Canvas(canvas, gui_framebuffer);

	// Mark this thread as the UI thread
	clan::FileResourceDocument doc(clan::FileSystem("../../ThemeAero"));
	clan::ResourceManager resources = clan::FileResourceManager::create(doc);
	ui_thread = clan::UIThread(resources);

	root = std::make_shared<clan::TextureView>(gui_canvas);

	root->set_event_window(window);
	root->set_cursor_window(window);

	root->set_rect(clan::Rect(0, 0, clan::Size(256, 256)));

	// Style the root view to use rounded corners and a bit of drop shadow
	root->style()->set("background: linear-gradient(13.37deg, #f0f0f0, rgb(120,240,120) 50%, #f0f0f0)");
	root->style()->set("padding: 11px");
	root->style()->set("border: 1px solid black");
	root->style()->set("border-radius: 15px");
	root->style()->set("margin: 35px 10px 10px 10px");
	root->style()->set("box-shadow: 0 0 20px rgba(0,0,0,0.2)");
	root->style()->set("flex-direction: column");

	// Create a label with some text to have some content
	label = std::make_shared<clan::LabelView>();
	label->style()->set("font: 20px/40px 'Ravie'");
	label->style("amazing")->set("background: #ffaaaa; color: #031337");
	label->style("cool")->set("background: #aaffaa; font-style: italic");
	label->style("amazing cool")->set("background: #ffffaa");
	label->set_text("Hello World!");
	label->set_state("amazing", true);
	label->set_state("cool", true);
	root->add_subview(label);

	// React to clicking
	label->slots.connect(label->sig_pointer_press(), [&](clan::PointerEvent &e) {
		label->set_text(label->text() + " CLICK!");
	});

	// Create a text field for our span layout
	std::shared_ptr<clan::TextFieldView> edit = std::make_shared<clan::TextFieldView>();
	edit->style()->set("font: 11px/20px 'Segoe UI'");
	edit->style()->set("margin: 5px 0");
	edit->style()->set("background: white");
	edit->style()->set("border: 1px solid black");
	edit->style()->set("border-radius: 3px");
	edit->style()->set("padding: 2px 5px 2px 5px");
	edit->style()->set("width: 128px");
	edit->set_text("Text File View");

	// Create a span layout view with some more complex inline formatting
	std::shared_ptr<clan::SpanLayoutView> span = std::make_shared<clan::SpanLayoutView>();
	std::shared_ptr<clan::Style> text_style = std::make_shared<clan::Style>();
	text_style->set("font: 13px/40px 'Segoe UI'");
	span->add_text("This is the UI core ", text_style);

	span->add_subview(edit);
	
	auto scrollbar = Theme::create_scrollbar();
	scrollbar->set_range(0.0, 1.0);
	scrollbar->set_position(0.5);
	scrollbar->set_page_step(0.1);
	scrollbar->set_line_step(0.01);
	root->add_subview(scrollbar);
	
	auto button = Theme::create_button();
	button->label()->set_text("This is a button");
	root->add_subview(button);

	std::shared_ptr<clan::Style> text_style2 = std::make_shared<clan::Style>();
	text_style2->set("font: 16px/40px 'Segoe UI'; font-weight: 800");
	span->add_text(" units!", text_style2);
	root->add_subview(span);

	game_time.reset();
}

bool GUI::update()
{
	game_time.update();

	canvas.clear(clan::Colorf(0.3f,0.7f,0.2f));

	//root->set_needs_layout();
	//root->set_needs_render();
	root->update();
	gui_canvas.flush();

	gui_image.draw(canvas, 0, 0);

	std::string fps = clan::string_format("%1 fps", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1));
	font.draw_text(canvas, canvas.get_width() - 200, 30, fps);

	window.flip(0);

	return !quit;
}

// A key was pressed
void GUI::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}



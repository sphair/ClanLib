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
#include "gui.h"

class DisplayResources : public clan::DisplayCache
{
public:
	clan::Resource<clan::Sprite> get_sprite(clan::Canvas &canvas, const std::string &id) override { throw clan::Exception("No sprite resources"); }
	clan::Resource<clan::Image> get_image(clan::Canvas &canvas, const std::string &id) override { throw clan::Exception("No image resources"); }
	clan::Resource<clan::Texture> get_texture(clan::GraphicContext &gc, const std::string &id) override { throw clan::Exception("No texture resources"); }
	clan::Resource<clan::CollisionOutline> get_collision(const std::string &id) override { throw clan::Exception("No collision resources"); }

	clan::Resource<clan::Font> get_font(clan::Canvas &canvas, const clan::FontDescription &desc)
	{
		std::string id = desc.get_unique_id();
		if (loaded_fonts.find(id) == loaded_fonts.end())
		{
			loaded_fonts[id] = clan::Font(canvas, desc);
		}
		return loaded_fonts[id];
	}

private:
	std::map<std::string, clan::Resource<clan::Font>> loaded_fonts;
};

// The start of the Application
int GUI::start(const std::vector<std::string> &args)
{
	quit = false;

    clan::SlotContainer sc;

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib TextureView GUI Example");
	desc.set_size(clan::Size(640, 640), true);
	desc.set_allow_resize(true);

	clan::DisplayWindow window(desc);
	clan::Canvas canvas(window);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), [&](){quit = true; });

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &GUI::on_input_up));

	clan::Font font(canvas, "tahoma", 24);

	clan::Texture2D gui_texture = clan::Texture2D(canvas, 512, 512);
	clan::Image gui_image(gui_texture, gui_texture.get_size());
	clan::FrameBuffer gui_framebuffer = clan::FrameBuffer(canvas);
	gui_framebuffer.attach_color(0, gui_texture);
	clan::Canvas gui_canvas(canvas, gui_framebuffer);

	// Create a source for our resources
	clan::ResourceManager resources;
	clan::DisplayCache::set(resources, std::make_shared<DisplayResources>());

	// Mark this thread as the UI thread
	clan::UIThread ui_thread(resources);

	std::shared_ptr<clan::TextureView> root = std::make_shared<clan::TextureView>(gui_canvas);

	// Style the root view to use rounded corners and a bit of drop shadow
	root->box_style.set_background(clan::Colorf(240, 240, 240, 255));
	root->box_style.set_padding(11.0f);
	root->box_style.set_border_radius(15.0f);
	root->box_style.set_border(clan::Colorf(0, 0, 0), 1.0f);
	root->box_style.set_margin(20.0f, 20.0f, 20.0f, 20.0f);
	root->box_style.set_box_shadow(clan::Colorf(0, 0, 0, 50), 0.0f, 0.0f, 20.0f);

	// Create a label with some text to have some content
	std::shared_ptr<clan::LabelView> label = std::make_shared<clan::LabelView>();
	label->text_style().set_font("Ravie", 20.0f, 40.0f);
	label->set_text("Hello World!");
	root->add_subview(label);

	// React to clicking
	label->slots.connect(label->sig_pointer_press(), [&](clan::PointerEvent &e) {
		label->set_text(label->text() + " CLICK!");
	});

	// Create a text field for our span layout
	std::shared_ptr<clan::TextFieldView> edit = std::make_shared<clan::TextFieldView>();
	edit->text_style().set_font("Ravie", 11.0f, 20.0f);
	edit->set_text("42");
	edit->box_style.set_margin(0.0f, 5.0f);
	edit->box_style.set_background(clan::Colorf(255, 255, 255));
	edit->box_style.set_border(clan::Colorf(0.0f, 0.0f, 0.0f), 1.0f);
	edit->box_style.set_border_radius(3.0f);
	edit->box_style.set_padding(5.0f, 2.0f, 5.0f, 3.0f);
	edit->box_style.set_width(35.0f);

	// Create a span layout view with some more complex inline formatting
	std::shared_ptr<clan::SpanLayoutView> span = std::make_shared<clan::SpanLayoutView>();
	clan::TextStyle font_desc2;
	font_desc2.set_font_family("Segoe UI");
	font_desc2.set_size(13.0f);
	font_desc2.set_line_height(40.0f);
	span->add_text("This is the UI core ", font_desc2);
	clan::TextStyle font_desc3;
	font_desc3.set_font_family("Segoe UI");
	font_desc3.set_size(18.0f);
	font_desc3.set_line_height(40.0f);
	span->add_text("Hello World!", font_desc3);
	clan::TextStyle font_desc4;
	font_desc4.set_font_family("Segoe UI");
	font_desc4.set_size(13.0f);
	font_desc4.set_line_height(40.0f);
	span->add_text(" example! Here's a text field: ", font_desc4);
	span->add_subview(edit);
	clan::TextStyle font_desc5;
	font_desc5.set_font_family("Segoe UI");
	font_desc5.set_size(16.0f);
	font_desc5.set_line_height(40.0f);
	font_desc5.set_weight(800);
	span->add_text(" units! sdfjghsdkfj hkjsdfhg jksdhfj gkshdfk gsjdkfghsjkdfh kgjshdfkg sjkdfh gjskhf gskjdfg hkjsdfh kgjsdhfkgjhsdkjfhgksjdfhg kjsdfhgjkshdfkhgskjdf ghkjsdfsg kdfhg skjdfhgjksdh fgsdfhg kjsdhfjkghsdkjfh gkjsdhfjkgsdhfkgjhsdkfj hgksj.", font_desc5);
	root->add_subview(span);

	clan::GameTime game_time;

	// Run until someone presses escape
	while (!quit)
	{
		game_time.update();

		canvas.clear(clan::Colorf(0.3f,0.7f,0.2f));

		root->set_needs_layout();
		root->set_needs_render();
		root->update();
		gui_canvas.flush();

		gui_image.draw(canvas, 64, 64);

		std::string fps = clan::string_format("%1 fps", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1));
		font.draw_text(canvas, canvas.get_width() - 200, 30, fps);

		window.flip(0);

		// This call processes user input and other events
		clan::KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void GUI::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}



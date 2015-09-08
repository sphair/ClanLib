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
#include "atlas.h"
#include "options.h"

clan::ApplicationInstance<Atlas> clanapp;

Atlas::Atlas()
{
	// We support all display targets, in order listed here
#ifdef WIN32
	clan::D3DTarget::set_current();
#endif
	clan::OpenGLTarget::set_current();

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib TextureGroup Example");
	desc.set_size(clan::Sizef(800.0f, 600.0f), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	clan::FileResourceDocument doc(clan::FileSystem("../../ThemeAero"));
	clan::ResourceManager resources = clan::FileResourceManager::create(doc);
	ui_thread = clan::UIThread(resources);

	options = std::make_shared<Options>(canvas);
	options->set_always_render();
	options->set_window(window);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), [&](){quit = true; });

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &Atlas::on_input_up));

	font = clan::Font("tahoma", 14);

}

void Atlas::scan()
{
	texture_group = clan::TextureGroup(clan::Size(options->group_size, options->group_size));
	scan_folder("../../ThemeAero/Resources");
	scan_folder("../../Display/Basic2D/Resources");

}
void Atlas::scan_folder(const std::string &folder_name)
{
	clan::DirectoryScanner scanner;
	scanner.scan(folder_name, "*.png");
	while (scanner.next())
	{
		auto name = scanner.get_name();
		auto pixelbuffer = clan::PixelBuffer(scanner.get_pathname());

		int outline_size = options->outline_size;

		// Add border is used to eliminate issues when scaling images, so that the GPU does not use the outline pixels in the adjacent image.
		auto outline_pixelbuffer = clan::PixelBufferHelp::add_border(pixelbuffer, outline_size, pixelbuffer.get_size());

		auto sub_texture = texture_group.add(canvas, outline_pixelbuffer.get_size());
		sub_texture.get_texture().set_subimage(canvas, sub_texture.get_geometry().get_top_left(), outline_pixelbuffer, outline_pixelbuffer.get_size());

		images[scanner.get_name()] = clan::Image(sub_texture.get_texture(), sub_texture.get_geometry().shrink(outline_size));
	}
}

bool Atlas::update()
{
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.2f));

	options->set_clear_background(false);
	options->set_viewport(canvas.get_size());

	if (options->options_changed)
	{
		options->options_changed = false;
		scan();
	}

	auto textures = texture_group.get_textures();
	if (!textures.empty())
	{
		clan::Image image(textures[0], textures[0].get_size());
		canvas.fill_rect(clan::Rectf(1.0f, 1.0f, image.get_size()), clan::Colorf::grey);
		canvas.draw_box(clan::Rectf(0.1f, 0.1f, image.get_size() + clan::Sizef(1.0f, 1.0f)), clan::Colorf::white);
		image.draw(canvas, 1.0f, 1.0f);
	}

	std::string test_name("logo.png");
	auto test_image = images[test_name];
	if (!test_image.is_null())
	{
		auto canvas_size = canvas.get_size();
		clan::Rectf target_box(canvas_size.width - 100.0f, canvas_size.height - 100.0f, canvas_size.width, canvas_size.height);
		canvas.draw_box(target_box, clan::Colorf::red);
		target_box.shrink(1.0f);
		test_image.draw(canvas, target_box);
		font.draw_text(canvas, target_box.left, target_box.top - 4.0f, test_name);
	}

	font.draw_text(canvas, canvas.get_width() - 200, 32, clan::string_format("Texture Pages #%1", textures.size()));
	font.draw_text(canvas, 8, canvas.get_height() - 24, "Note: It is advisable for compatibility and performance that the texture group page size");
	font.draw_text(canvas, 16, canvas.get_height() - 4, "is a number that is a power of 2, for example: 256, 512, 1024 or 2048");

	options->update();

	window.flip(1);

	return !quit;
}

// A key was pressed
void Atlas::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

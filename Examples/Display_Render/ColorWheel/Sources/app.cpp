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
**    Mark Page
*/

#include "precomp.h"
#include "app.h"
#include "colorwheel.h"
#include <cstdlib>

clan::ApplicationInstance<App> clanapp;

App::App()
{
	// We support all display targets, in order listed here
#ifdef WIN32
	clan::D3DTarget::enable();
#endif
	clan::OpenGLTarget::enable();
	
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("ColorWheel Example");
	win_desc.set_size(clan::Size( 800, 600 ), false);

	window = clan::DisplayWindow(win_desc);
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	canvas = clan::Canvas(window);

	clan::Texture2D gui_texture = clan::Texture2D(canvas, (int)std::round(512 * canvas.get_pixel_ratio()), (int)std::round(512 * canvas.get_pixel_ratio()));
	gui_texture.set_pixel_ratio(canvas.get_pixel_ratio());
	gui_image = clan::Image(gui_texture, gui_texture.get_size());
	clan::FrameBuffer gui_framebuffer = clan::FrameBuffer(canvas);
	gui_framebuffer.attach_color(0, gui_texture);
	gui_canvas = clan::Canvas(canvas, gui_framebuffer);

	clan::FileResourceDocument doc(clan::FileSystem("../../ThemeAero"));
	clan::ResourceManager resources = clan::FileResourceManager::create(doc);
	ui_thread = clan::UIThread(resources);

	root = std::make_shared<clan::TextureWindow>(gui_canvas);

	root->set_window(window);

	root->set_viewport(clan::Rectf(32.0f, 32.0f, gui_image.get_size()));

	color_wheel = std::make_shared<ColorWheel>();
	root->add_subview(color_wheel);

}

bool App::update()
{
	canvas.clear(clan::Colorf(0.0f,0.0f,0.4f));

	root->update();
	gui_image.draw(canvas, 0, 0);

	window.flip(1);

	return !quit;
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	switch (key.id)
	{
		case clan::keycode_escape:
			quit = true;
			break;
	}

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}


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
**    Mark Page
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;


class App : public clan::Application
{
public:
	App();
	bool update() override;


private:
	void on_input_up(const InputEvent &key);
	void on_window_close();

private:
	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font;

	bool quit;
};
clan::ApplicationInstance<App> clanapp;

App::App()
{
	clan::OpenGLTarget::set_current();

	quit = false;

	DisplayWindowDescription desc;

	desc.set_title("ClanLib AnimCursor Test");
	desc.set_size(Size(800, 600), true);
	window = DisplayWindow(desc);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), this, &App::on_input_up);

	// Create the canvas and get the graphic context
	canvas = Canvas(window);

	font = Font("Tahoma", 20);

	PixelBuffer pacman = ImageProviderFactory::load("pacman.png");

	CursorDescription description;
	Size size(22, 22);

	for (int frame_cnt=0; frame_cnt < 6; frame_cnt++)
	{
		PixelBuffer frame(size.width, size.height, tf_rgba8);

		frame.set_subimage(pacman,Point(0,0),Rect((frame_cnt * 28) + 4, 4, size));

		description.add_frame(frame);
		description.set_frame_delay(frame_cnt, 0.1);
	}

	Point hotspot(0,0);
	description.set_hotspot(hotspot);
	Cursor cursor(window, description);
	window.set_cursor(cursor);

}

bool App::update()
{
	canvas.clear(Colorf(0.0f,0.0f,0.5f));

	font.draw_text(canvas, 32, 32, "Observe the animated cursor");

	// Flip the display, showing on the screen what we have drawed
	// since last call to flip()
	window.flip(1);

	return !quit;
}

// A key was pressed
void App::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}



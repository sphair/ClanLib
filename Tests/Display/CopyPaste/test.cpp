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
**    Harry Storbacka
**	  Arkadiusz Kalinowski
*/

#include "test.h"

clan::ApplicationInstance<TestApp> clanapp;

TestApp::TestApp()
{
	clan::OpenGLTarget::enable();

	quit = false;

	DisplayWindowDescription desc;

	desc.set_title("Image clipboard test");
	desc.set_size(Size(800,600), true);
	window = DisplayWindow(desc);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), this, &TestApp::on_window_close);

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), this, &TestApp::on_input_up);
		
	//Create the Canvas
	canvas = Canvas(window);

	PixelBuffer to_clipboard = ImageProviderFactory::load("copy.png");
	window.set_clipboard_image(to_clipboard);

	/*		while (!window.get_ic().get_keyboard().get_keycode(KEY_ESCAPE))
	{
		window.get_gc().clear(Colorf::gray70);
		window.flip();
		RunLoop::process();
		System::sleep(50);
	}*/

/*		// Save to file
	PNGProvider::save(to_clipboard, "copy2.png");

	// Save to IODevice_Memory memory buffer
	DataBuffer data(1024*8); // 8 kb
	IODevice_Memory mem(data);
	PNGProvider::save(to_clipboard, mem);
	File file("copy3.png", File::create_always);
	DataBuffer mem_data = mem.get_data();
	file.write(mem_data.get_data(), mem_data.get_size());
*/	

	PixelBuffer from_clipboard;
	if (window.is_clipboard_image_available())
	{
		from_clipboard = window.get_clipboard_image();
		PNGProvider::save(from_clipboard, "from_clipboard.png");
	} 

	if (!from_clipboard.is_null())
	{
		Texture2D texture_image(canvas,
								from_clipboard.get_width(),
								from_clipboard.get_height(),
								from_clipboard.get_format());
		texture_image.set_image(canvas,from_clipboard);
		texture_image.set_min_filter(filter_linear);
		texture_image.set_mag_filter(filter_linear);

		image = Image(texture_image,texture_image.get_size());
	}
}

bool TestApp::update()
{
	canvas.clear(Colorf(0.0f,0.0f,0.2f));

	if (!image.is_null())
		image.draw(canvas,0,0);

	window.flip(1);

	return !quit;
}

void TestApp::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

void TestApp::on_window_close()
{
	quit = true;
}





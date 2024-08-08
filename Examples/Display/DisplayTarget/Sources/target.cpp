/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "target.h"

Target::Target(RenderTarget new_target) : render_target(new_target)
{
	clan::OpenGLContextDescription opengl_desc;

	switch (render_target)
	{
	case (legacy_gl) :
		clan::OpenGLTarget::set_current();
		opengl_desc.set_version(1, 3, true);
		clan::OpenGLTarget::set_description(opengl_desc);
		break;

	case (opengl) :
		clan::OpenGLTarget::set_current();
		opengl_desc.set_version(4, 5, true);
		clan::OpenGLTarget::set_description(opengl_desc);
		break;
#if defined(WIN32) && !defined(__MINGW32__)
	case (d3d) :
		clan::D3DTarget::set_current();
		break;
#endif
	}

	clan::DisplayWindowDescription desc;
	desc.set_title("Target Display (with resizable window)");
	desc.set_size(clan::Size(800, 600), true);
	desc.set_allow_resize(true);
	window = clan::DisplayWindow(desc);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), clan::bind_member(this, &Target::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &Target::on_input_up));

	canvas = clan::Canvas(window);

	target_font = clan::Font("tahoma", 28);
	fps_font = clan::Font("tahoma", 20);

	// Because we are using the alpha channel on fonts, we must disable subpixel rendering
	clan::FontDescription font_desc;
	font_desc.set_height(16);
	font_desc.set_subpixel(false);
	fall_font = clan::Font("tahoma", font_desc);

	game_time.reset();
}

void Target::run_demo()
{
	game_time.update();

	fontfall_ticker += game_time.get_time_elapsed();;
	if (fontfall_ticker >= 0.01f)
	{
		fontfall_ticker = 0.0f;
		font_fall.push_back(new_fontfall(canvas.get_width()));
	}

	canvas.clear(clan::Colorf(0.0f,0.0f,0.0f));

	std::string fps = clan::string_format("%1 fps", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1));
	fps_font.draw_text(canvas, canvas.get_width() - 100, 30, fps);

	std::string words = clan::string_format("%1 words", (int) font_fall.size());
	fps_font.draw_text(canvas, 100, 30, words);

	const int font_xpos = 32;
	const int font_ypos = 64;

	if (clan::OpenGLTarget::is_current())
	{
		int major,minor, version_release;
		clan::OpenGLTarget::get_opengl_version(canvas, major, minor, version_release);

		if (canvas.get_gc().get_shader_language() == clan::ShaderLanguage::fixed_function)
		{
			target_font.draw_text(canvas, font_xpos, font_ypos, clan::string_format("1) OpenGL 1.3 Compatable. Context = %1.%2 (clanGL1)", major, minor));
		}
		else
		{
			target_font.draw_text(canvas, font_xpos, font_ypos, clan::string_format("2) OpenGL 4.3 Compatable. Context = %1.%2 (clanGL3)", major, minor));
		}
	}

#if defined(WIN32) && !defined(__MINGW32__)
	if (clan::D3DTarget::is_current())
			target_font.draw_text(canvas, font_xpos, font_ypos, "3) Direct3D renderer (clanD3D)");

	fps_font.draw_text(canvas, 32, 96, "Press 1,2 or 3 to select targets, or escape to quit.");
#else
	fps_font.draw_text(canvas, 32, 96, "Press 1 or 2 to select targets, or escape to quit.");
#endif

	float max_height = (float) (canvas.get_height() + 20);
	float half_height = (float) canvas.get_height() / 2.0f;
	for (std::vector<FontFall>::iterator it = font_fall.begin(); it != font_fall.end();)
	{
		FontFall &item = *it;
		item.ypos += game_time.get_time_elapsed() * 100.0f;
		if (item.ypos >= max_height)
		{
			it = font_fall.erase(it);
		}
		else
		{
			item.color.a = item.ypos / (half_height);
			if (item.color.a > 1.0f)
				item.color.a = 1.0f;
			fall_font.draw_text(canvas, item.xpos, (int) item.ypos, item.text, item.color);
			++it;
		}
	}

	window.flip(0);

	if (window.get_keyboard().get_keycode(clan::keycode_1))
	{
		render_target = legacy_gl;
	}
	if (window.get_keyboard().get_keycode(clan::keycode_2))
	{
		render_target = opengl;
	}
#if defined(WIN32) && !defined(__MINGW32__)
	if (window.get_keyboard().get_keycode(clan::keycode_3))
	{
		render_target = d3d;
	}
#endif
	if (window.get_keyboard().get_keycode(clan::keycode_escape))
	{
		quit = true;
	}
}

FontFall Target::new_fontfall(int window_width)
{
	static std::string words[] = 
	{
		"Apple","Ball","ClanLib","Dog","Egg","Fish","Ghost","Hunter",
		"Ice cream","Jumper","Kite","Lion","Matrix","Nose","Orange","Panda",
		"Queen","Ruler","Screen saver","Trousers","Umbrella","Violin","Window","Xylophone",
		"Yacht","Zebra"
	};

	static int word_counter = 0;

	if (window_width == 0) window_width = 1;
	FontFall fontfall;
	// (Note, do not randomize the word order, because it would will make comparing display target fps more difficult)
	fontfall.text = words[++word_counter % (sizeof(words)/sizeof(std::string))];
	unsigned int value = rand();
	fontfall.xpos = (value % window_width) - 10;
	fontfall.ypos = 0.0f;
	fontfall.color.r = (rand() & 255) / 255.0f;
	fontfall.color.g = (rand() & 255) / 255.0f;
	fontfall.color.b = (rand() & 255) / 255.0f;
	fontfall.color.a = 1.0f;
	return fontfall;
}

// A key was pressed
void Target::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Target::on_window_close()
{
	quit = true;
}

/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "framerate_counter.h"

Target::Target()
{
	render_target = swrender;
}

int Target::start(const std::vector<CL_String> &args)
{
	CL_SetupGL target_opengl2;
	CL_SetupGL1 target_opengl1;
	CL_SetupSWRender target_swrender;

	// Since SWRender is compatible and fast - Use that as the default
	target_swrender.set_current();

	if (get_display_target())
	{
		switch (render_target)
		{
			case (opengl1):
				target_opengl1.set_current();
				break;

			case (opengl2):
				target_opengl2.set_current();
				break;

			case (swrender):
				target_swrender.set_current();
				break;
		}
		run_demo();
	}

	return 0;
}

bool Target::get_display_target()
{
	CL_String theme;
	if (CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (CL_FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw CL_Exception("No themes found");

	CL_GUIManager gui(theme);

	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Display Target Application");
	win_desc.set_position(CL_Rect(200, 200, 540, 440), false);
	win_desc.set_visible(false);
	CL_Window window(&gui, win_desc);
	current_window = &window;
	window.func_close().set(this, &Target::on_close, &window);

	CL_GUILayoutCorners layout;
	window.set_layout(layout);

	window.create_components("Resources/layout.xml");

	CL_PushButton *button = CL_PushButton::get_named_item(&window, "button_gl");
	button->func_clicked().set(this, &Target::on_button_clicked, opengl2);

	button = CL_PushButton::get_named_item(&window, "button_gl1");
	button->func_clicked().set(this, &Target::on_button_clicked, opengl1);

	button = CL_PushButton::get_named_item(&window, "button_swrender");
	button->func_clicked().set(this, &Target::on_button_clicked, swrender);

	window.set_visible();

	if (gui.exec())
	{
		return false;
	}
	return true;
}

void Target::on_button_clicked(RenderTarget selected_target)
{
	render_target = selected_target;
	current_window->exit_with_code(0);
}

bool Target::on_close(CL_Window *win)
{
	win->exit_with_code(1);
	return true;
}

void Target::run_demo()
{
	CL_DisplayWindowDescription desc;
	desc.set_title("Target Display (with resizable window)");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_allow_resize(true);
	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Target::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Target::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	CL_Font target_font(gc, "tahoma", 32);
	CL_Font fps_font(gc, "tahoma", 20);
	CL_Font fall_font(gc, "tahoma", 16);

	std::vector<FontFall> font_fall;

	FramerateCounter frameratecounter;

	unsigned int time_last = CL_System::get_time();
	float fontfall_ticker = 0.0f;

	quit = false;
	// Run until someone presses escape
	while (!quit)
	{
		unsigned int time_now = CL_System::get_time();
		float time_delta = (float) (time_now - time_last);
		time_last = time_now;

		fontfall_ticker += time_delta;
		if (fontfall_ticker >= 10.0f)
		{
			fontfall_ticker = 0.0f;
			font_fall.push_back(new_fontfall(gc.get_width()));
		}

		gc.clear(CL_Colorf(0.0f,0.0f,0.0f));

		CL_String fps = cl_format("%1 fps", frameratecounter.get_framerate());
		fps_font.draw_text(gc, gc.get_width() - 100, 30, fps);

		CL_String words = cl_format("%1 words", (int) font_fall.size());
		fps_font.draw_text(gc, 100, 30, words);

		const int font_xpos = 32;
		const int font_ypos = 64;
		switch (render_target)
		{
			case (opengl1):
				target_font.draw_text(gc, font_xpos, font_ypos, "OpenGL 1.3 (clanGL1)");
				break;

			case (opengl2):
				target_font.draw_text(gc, font_xpos, font_ypos, "OpenGL 2.0 and better (clanGL)");
				break;

			case (swrender):
				target_font.draw_text(gc, font_xpos, font_ypos, "Software renderer (clanSWRender)");
				break;
		}

		float max_height = (float) (gc.get_height() + 20);
		float half_height = (float) gc.get_height() / 2.0f;
		for (std::vector<FontFall>::iterator it = font_fall.begin(); it != font_fall.end();)
		{
			FontFall &item = *it;
			item.ypos += time_delta / 10.0f;
			if (item.ypos >= max_height)
			{
				it = font_fall.erase(it);
			}
			else
			{
				item.color.a = item.ypos / (half_height);
				if (item.color.a > 1.0f)
					item.color.a = 1.0f;
				fall_font.draw_text(gc, item.xpos, (int) item.ypos, item.text, item.color);
				++it;
			}
		}

		window.flip(0);
		frameratecounter.frame_shown();

		CL_KeepAlive::process(0);
	}
}

FontFall Target::new_fontfall(int window_width)
{
	static CL_String words[] = 
	{
		"Apple","Ball","ClanLib","Dog","Egg","Fish","Ghost","Hunter",
		"Ice cream","Jumper","Kite","Lion","Matrix","Nose","Orange","Panda",
		"Queen","Ruler","Screen saver","Trousers","Umbrella","Violin","Window","Xylophone",
		"Yacht","Zebra"
	};

	static int word_counter = 0;

	FontFall fontfall;
	// (Note, do not randomize the word order, because it would will make comparing display target fps more difficult)
	fontfall.text = words[++word_counter % (sizeof(words)/sizeof(CL_String))];
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
void Target::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Target::on_window_close()
{
	quit = true;
}



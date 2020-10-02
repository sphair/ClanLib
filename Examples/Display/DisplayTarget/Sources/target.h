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

#pragma once

typedef struct _FontFall
{
	std::string text;
	int xpos;
	float ypos;
	clan::Colorf color;
} FontFall;

class Target
{
public:
	enum RenderTarget
	{
		legacy_gl,
		opengl,
		d3d
	};

	Target(RenderTarget new_target);
	void run_demo();

private:
	void on_button_clicked(RenderTarget selected_target);
	void on_input_up(const clan::InputEvent &key);
	void on_window_close();

	FontFall new_fontfall(int window_width);

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::GameTime game_time;
	clan::Font target_font;
	clan::Font fps_font;
	clan::Font fall_font;
	std::vector<FontFall> font_fall;
	float fontfall_ticker = 0.0f;

public:
	RenderTarget render_target;
	bool quit = false;

};

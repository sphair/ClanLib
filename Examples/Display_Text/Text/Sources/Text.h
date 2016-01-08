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

#pragma once

class ExampleText : public clan::Application
{
public:
	ExampleText();
	bool update() override;

private:
	void on_keyboard_up(const clan::InputEvent &key);
	void on_window_close();

	void update_text(clan::Canvas &canvas_fb, clan::Font &font, std::vector<clan::SpanLayout> &layout);
	void draw_text(clan::Canvas &canvas, clan::Angle angle);

	clan::DisplayWindow window;
	clan::SlotContainer sc;
	clan::Canvas canvas;
	clan::Canvas canvas_fb;
	clan::Image texture_image;
	clan::Font font_normal;
	clan::Font font_bold;
	float angle = 0.0f;
	std::vector<clan::SpanLayout> layout;

	clan::GameTime game_time;
	bool quit = false;
	float last_fps;
	static const int text_window_size = 1024;
};

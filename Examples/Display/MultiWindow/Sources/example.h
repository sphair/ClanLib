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
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

class Example : public clan::Application
{
public:
	Example();
	bool update() override;

private:
	void on_input_up(const clan::InputEvent &key, int window_number);
	void on_window_close(clan::DisplayWindow *window);

private:
	clan::DisplayWindow window_1;
	clan::DisplayWindow window_2;
	clan::SlotContainer sc;
	clan::Canvas canvas_1;
	clan::Canvas canvas_2;
	clan::Font font;

	clan::GameTime game_time;
	float window_1_color = 0.2f;
	float window_2_color = 0.2f;
	bool quit = false;
	int font_xoffset = 0;
	int font_counter = 0;

};

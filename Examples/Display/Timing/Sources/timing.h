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

class Star
{
public:
	float xpos;
	float ypos;
	float speed;
	clan::Colorf color;
};

class Timing : public clan::Application
{
public:
	Timing();
	bool update() override;

private:
	void on_input_up(const clan::InputEvent &key);
	void on_window_close();
	void draw_graphics(clan::Canvas &canvas, float time_delta);
	void set_stars(clan::Canvas &canvas, int star_cnt);
private:
	bool quit = false;

	std::vector<Star> stars;
	clan::DisplayWindow window;
	clan::SlotContainer sc;
	clan::Canvas canvas;
	clan::GameTime game_time;

};

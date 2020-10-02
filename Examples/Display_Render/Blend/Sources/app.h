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


typedef struct _BallPosition
{
	float xpos;
	float ypos;
	float xspeed;
	float yspeed;
	bool xdir;
	bool ydir;
} BallPosition;

class Options;

class App : public clan::Application
{
public:
	App();
	bool update() override;

private:
	void on_input_up(const clan::InputEvent &key);
	void on_window_close();
	void setup_balls();
	void move_balls(float time_diff, int num_balls);
	std::string get_blendequation(clan::BlendEquation equation, const std::string &source, const std::string &dest, const std::string &source_fragment, const std::string &dest_fragment);
	std::string get_blendfunc(clan::BlendFunc blendfunc, const std::string &fragment_colour);
	void draw_equation(clan::Canvas &canvas, clan::Font &font, std::shared_ptr<Options> &options);
	std::string get_logic_operation(clan::LogicOp logic_operation, const std::string &source_fragment, const std::string &dest_fragment);

private:
	clan::DisplayWindow window;
	clan::SlotContainer sc;
	clan::Canvas canvas;
	clan::Font font;
	clan::Image image_grid;
	clan::Image image_ball;
	clan::UIThread ui_thread;
	clan::Image image_ball_premultiply_alpha;
	std::shared_ptr<Options> options;

	bool quit = false;
	float grid_space;
	float ball_alpha;
	clan::GameTime game_time;

	static const int max_balls = 9;
	BallPosition balls[max_balls];

};

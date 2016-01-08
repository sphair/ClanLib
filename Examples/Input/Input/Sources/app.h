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

#include "text_shooter.h"
#include <list>

class App : public clan::Application
{
public:
	App();
	bool update() override;

private:
	void on_input_down(const InputEvent &key);
	void on_mouse_down(const InputEvent &key);
	void on_controller_down(const InputEvent &key, int controller_number);
	void on_window_close();

	void create_shooter( const InputEvent &key, const std::string &str, bool use_red, bool use_green, bool use_blue);
	void draw_text_shooter(Canvas &canvas);

	void draw_keyboard_state(Canvas &canvas, int yoffset);
	void draw_mouse_state(Canvas &canvas, int yoffset);
	void draw_controller_state(Canvas &canvas, InputDevice controller, int controller_number, int yoffset);

private:
	bool quit = false;

	DisplayWindow window;
	SlotContainer sc;

	Font vector_font;
	clan::Font font;
	
	std::list<TextShooter> text_shooter;

	Canvas canvas;
};


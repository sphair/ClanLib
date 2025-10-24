/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	App();

	int start(const std::vector<CL_String> &args);

private:
	void on_input_down(const CL_InputEvent &key, const CL_InputState &state);
	void on_mouse_down(const CL_InputEvent &key, const CL_InputState &state);
	void on_joystick_down(const CL_InputEvent &key, const CL_InputState &state, int joystick_number);
	void on_window_close();

	void calculate_matrix(CL_GraphicContext &gc);
	void create_shooter( const CL_InputEvent &key, const CL_StringRef &str, bool use_red, bool use_green, bool use_blue);
	void draw_text_shooter(CL_GraphicContext &gc);

	void draw_keyboard_state(CL_GraphicContext &gc, int yoffset);
	void draw_mouse_state(CL_GraphicContext &gc, int yoffset);
	void draw_joystick_state(CL_GraphicContext &gc, int joystick_number, int yoffset);
	void draw_tablet_state(CL_GraphicContext &gc, int tablet_number, int yoffset);

private:
	bool quit;

	CL_DisplayWindow window;

	CL_Font_Vector vector_font;
	CL_Font font;

	CL_Mat4f projection_matrix;
	CL_Mat4f modelview_matrix;

	std::list<TextShooter> text_shooter;
};


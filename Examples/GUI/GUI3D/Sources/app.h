/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include <list>

class Window1;
class Slider;

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	App();

	int start(const std::vector<std::string> &args);

	DisplayWindow &get_window() { return window; }

	const Mat4f &get_projection_matrix() const { return projection_matrix; }

private:
	void on_input_down(const InputEvent &key);
	void on_window_close();
	void calculate_matrix();
	void control_window();

private:
	bool quit;
	DisplayWindow window;

	Mat4f projection_matrix;
	Mat4f modelview_matrix;

	Window1 *window1;
	Slider *slider_1_xrotation;
	Slider *slider_1_yrotation;
	Slider *slider_1_zrotation;
	Slider *slider_1_xtranslation;
	Slider *slider_1_ytranslation;
	Slider *slider_1_ztranslation;
};

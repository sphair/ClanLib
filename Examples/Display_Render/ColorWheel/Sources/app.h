/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	App();

	int start(const std::vector<std::string> &args);

private:
	void on_input_up(const clan::InputEvent &key);
	void on_window_close();
	void draw(clan::Canvas &canvas, const clan::Pointf &center, float radius);
	void draw_labels(clan::Canvas &canvas);
private:
	bool quit;

	float saturation_outer;
	float saturation_inner;
	float value_outer;
	float value_inner;
	bool is_hsl;

};

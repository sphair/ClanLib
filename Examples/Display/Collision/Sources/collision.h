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

#include "CollisionAPI/contour.h"
#include "CollisionAPI/collision_outline.h"

class CollisionApp : public clan::Application
{
public:
	CollisionApp();
	bool update() override;

private:
	void on_input_up(const clan::InputEvent &key);
	void draw_point_normal(clan::Canvas &canvas, const clan::Pointf &point, const clan::Pointf &normal, const clan::Colorf &color);
	void display_keys();
	void control_input();
private:
	bool quit = false;
	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font;

	double tri_x_pos = 200.0f;
	double tri_y_pos = 200.0f;
	double other_x_pos = 0;
	double other_y_pos = 0;

	// draw_limit = 0;

	bool draw_min_on_co1 = false;
	bool draw_min_on_co2 = false;
	bool draw_sub_on_co1 = false;
	bool draw_sub_on_co2 = false;
	bool draw_surfaces = false;
	bool draw_deep_point = false;
	float sub_circle_multiplier = 3.5f;

	clan::Sprite surface;
	clan::Sprite surface2;

	CollisionOutline co1;
	CollisionOutline co2;

};

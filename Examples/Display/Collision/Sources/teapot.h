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
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

class Teapot
{
public:
	Teapot();
	void create(clan::Canvas &canvas);
	void clone(const Teapot &source);
	void draw_collision_outline(clan::Canvas &canvas);
	void update(clan::Canvas &canvas, int elapsed_ms, std::vector<Teapot> &teapot_list);
	void draw_teapot(clan::Canvas &canvas);
	void set_position(int xpos, int ypos);
	void set_scale(float x_scale, float y_scale);
	void set_movement_delta(clan::Vec2f &normal, float new_speed);
	void set_color(const clan::Colorf &color);
private:
	void move(clan::Canvas &canvas, int elapsed_ms);
	bool is_collision(int xpos, int ypos, const std::vector<Teapot> &teapot_list, int &collided_teapot_offset);
	bool check_hit_other_object(int previous_xpos, int previous_ypos, int xpos, int ypos, std::vector<Teapot> &teapot_list);

private:
	clan::Sprite teapot_sprite;
	clan::CollisionOutline teapot_collision;
	int dest_xpos;
	int dest_ypos;
	float x_delta;
	float y_delta;
	float float_xpos;
	float float_ypos;
	float speed;
	bool flash;
};

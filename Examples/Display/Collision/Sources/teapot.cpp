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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "teapot.h"

Teapot::Teapot()
{
	dest_xpos = 0;
	dest_ypos = 0;
	x_delta = 0.0f;
	y_delta = 0.0f;
	float_xpos = 0.0f;
	float_ypos = 0.0f;
}

void Teapot::create(Canvas &canvas, ResourceManager &resources)
{
	SpriteDescription desc(canvas, "teapot", &resources);
	teapot_sprites = Sprite(canvas, desc);
	teapot_sprites.set_frame_delay(0, 100);

	// **** Try using "accuracy_low" or accuracy_medium" ****
	teapot_collisions = desc.get_collision_outlines(canvas, 128, accuracy_high);
}

void Teapot::draw_collision_outline(Canvas &canvas)
{
	teapot_collisions[teapot_sprites.get_current_frame()].draw( 0, canvas.get_height()/2, Colorf::limegreen, canvas);
//	teapot_collisions[teapot_sprites.get_current_frame()].draw( 0, 0, Colorf::limegreen, canvas);

}

void Teapot::clone(const Teapot &source)
{
	dest_xpos = source.dest_xpos;
	dest_ypos = source.dest_ypos;
	x_delta = source.x_delta;
	y_delta = source.y_delta;
	float_xpos = source.float_xpos;
	float_ypos = source.float_ypos;

	teapot_sprites = source.teapot_sprites.clone();

	teapot_collisions.resize(source.teapot_collisions.size());
	for (size_t cnt = 0; cnt < source.teapot_collisions.size(); cnt++)
	{
		teapot_collisions[cnt] = source.teapot_collisions[cnt].clone();
	}
}


void Teapot::update(Canvas &canvas, int elapsed_ms, std::vector<Teapot> &teapot_list)
{
	previous_teapot_animation_frame = teapot_sprites.get_current_frame();
	teapot_sprites.update(elapsed_ms);

	// If the updated animation now collides with a new object, restore the anim frame
	int collided_teapot_offset;
	if (is_collision(dest_xpos, dest_ypos, teapot_list, collided_teapot_offset))
	{
		teapot_sprites.set_frame(previous_teapot_animation_frame);	// Restore previous frame
	}


	int last_xpos = dest_xpos;
	int last_ypos = dest_ypos;
	move(canvas, elapsed_ms);


	// If the object is inside another object, ignore collision detection
	if (is_collision(last_xpos, last_ypos, teapot_list, collided_teapot_offset))
	{
		// Inside another object.  Occurs at the start when random placed objects are on top of each other
	}
	else
	{

#ifndef PIXEL_TRANSVERSAL_COLLISION_DETECTION

		// For accurate collision detection we want to check for collision for every pixel that the object transversed
		int difference_xpos = dest_xpos - last_xpos;
		int difference_ypos = dest_ypos - last_ypos;

		// Transverse x pixels or ypixels, depending which is the greater
		int positive_xpos = difference_xpos >= 0 ? difference_xpos : -difference_xpos;
		int positive_ypos = difference_ypos >= 0 ? difference_ypos : -difference_ypos;

		if (positive_xpos >= positive_ypos)
		{
			int check_last_xpos = last_xpos;
			int check_last_ypos = last_ypos;
			if (difference_xpos > 0)
			{
				for (int cnt = 0; cnt < positive_xpos; cnt++)
				{
					int check_new_xpos = last_xpos + (cnt + 1);
					int check_new_ypos = last_ypos + (difference_ypos * (cnt + 1) / positive_xpos);
					if (check_hit_other_object(check_last_xpos, check_last_ypos, check_new_xpos, check_new_ypos, teapot_list))
						break;
					check_last_xpos = check_new_xpos;
					check_last_ypos = check_new_ypos;
				}
			}
			else
			{
				for (int cnt = 0; cnt < positive_xpos; cnt++)
				{
					int check_new_xpos = last_xpos - (cnt + 1);
					int check_new_ypos = last_ypos + (difference_ypos * (cnt + 1) / positive_xpos);
					if (check_hit_other_object(check_last_xpos, check_last_ypos, check_new_xpos, check_new_ypos, teapot_list))
						break;
					check_last_xpos = check_new_xpos;
					check_last_ypos = check_new_ypos;
				}
			}
		}
		else
		{
			int check_last_xpos = last_xpos;
			int check_last_ypos = last_ypos;
			if (difference_ypos > 0)
			{
				for (int cnt = 0; cnt < positive_ypos; cnt++)
				{
					int check_new_ypos = last_ypos + (cnt + 1);
					int check_new_xpos = last_xpos + (difference_xpos * (cnt + 1) / positive_ypos);
					if (check_hit_other_object(check_last_xpos, check_last_ypos, check_new_xpos, check_new_ypos, teapot_list))
						break;
					check_last_xpos = check_new_xpos;
					check_last_ypos = check_new_ypos;
				}
			}
			else
			{
				for (int cnt = 0; cnt < positive_ypos; cnt++)
				{
					int check_new_ypos = last_ypos - (cnt + 1);
					int check_new_xpos = last_xpos + (difference_xpos * (cnt + 1) / positive_ypos);
					if (check_hit_other_object(check_last_xpos, check_last_ypos, check_new_xpos, check_new_ypos, teapot_list))
						break;
					check_last_xpos = check_new_xpos;
					check_last_ypos = check_new_ypos;
				}
			}

		}
#else
		check_hit_other_object(last_xpos,last_ypos, dest_xpos, dest_ypos, teapot_list);
#endif
	}

	// Set the translation, so other teapots can correctly collide with this one
	CollisionOutline &this_outline = teapot_collisions[teapot_sprites.get_current_frame()];
	this_outline.set_translation(dest_xpos, dest_ypos);


}

bool Teapot::check_hit_other_object(int previous_xpos, int previous_ypos, int xpos, int ypos, std::vector<Teapot> &teapot_list)
{
	int collided_teapot_offset;
	if (is_collision(xpos, ypos, teapot_list, collided_teapot_offset))
	{
		const std::vector<CollidingContours> &colpointinfo = teapot_collisions[teapot_sprites.get_current_frame()].get_collision_info();
		if (!colpointinfo.empty())
		{
			if (!colpointinfo[0].points.empty())
			{
				const CollisionPoint &point = colpointinfo[0].points[0];

				Pointf normal = point.normal;
				x_delta = normal.x * speed;
				y_delta = normal.y * speed;

				teapot_list[collided_teapot_offset].x_delta = -x_delta;
				teapot_list[collided_teapot_offset].y_delta = -y_delta;
			}
		}

		dest_xpos = previous_xpos;
		dest_ypos = previous_ypos;
		float_xpos = (float) previous_xpos;
		float_ypos = (float) previous_ypos;
		teapot_sprites.set_frame(previous_teapot_animation_frame);	// Restore previous frame
		return true;
	}
	return false;
}

bool Teapot::is_collision(int xpos, int ypos, const std::vector<Teapot> &teapot_list, int &collided_teapot_offset)
{
	CollisionOutline &this_outline = teapot_collisions[teapot_sprites.get_current_frame()];
	this_outline.set_translation(xpos, ypos);

	this_outline.enable_collision_info(false,true,false,false);

	std::vector<Teapot>::size_type cnt, max;
	max = teapot_list.size();
	for (cnt=0; cnt<max; cnt++)
	{
		const Teapot &other_teapot = teapot_list[cnt];

		if (&other_teapot == this)	// Do not collide with self
		{
			collided_teapot_offset = cnt;	// To always return a valid number
			continue;
		}

		if( this_outline.collide(other_teapot.teapot_collisions[other_teapot.teapot_sprites.get_current_frame()]) )
		{
			collided_teapot_offset = cnt;
			return true;
		}
	}
	return false;
}

void Teapot::move(Canvas &canvas, int elapsed_ms)
{
	float_xpos += x_delta * (float) elapsed_ms;
	float_ypos += y_delta * (float) elapsed_ms;

	dest_xpos = (int) float_xpos;
	dest_ypos = (int) float_ypos;

	if (dest_xpos < 0)
	{
		dest_xpos = 0;
		if (x_delta < 0)
			x_delta = -x_delta;
		float_xpos = (float) dest_xpos;
	}

	if (dest_ypos < 0)
	{
		dest_ypos = 0;
		if (y_delta < 0)
			y_delta = -y_delta;
		float_ypos = (float) dest_ypos;
	}

	int limit_x = canvas.get_width() - teapot_sprites.get_width();
	int limit_y = canvas.get_height()/2 - teapot_sprites.get_height();

	if (dest_xpos >= limit_x)
	{
		dest_xpos = limit_x;
		if (x_delta > 0)
			x_delta = -x_delta;
		float_xpos = (float) dest_xpos;
	}

	if (dest_ypos >= limit_y)
	{
		dest_ypos = limit_y;
		if (y_delta > 0)
			y_delta = -y_delta;
		float_ypos = (float) dest_ypos;
	}

}

void Teapot::draw_teapot(Canvas &canvas)
{
	teapot_sprites.draw(canvas, dest_xpos, dest_ypos);
}

void Teapot::set_frame(int frame_number)
{
	teapot_sprites.set_frame(frame_number);
}

void Teapot::set_position(int xpos, int ypos)
{
	dest_xpos = xpos;
	dest_ypos = ypos;
	float_xpos = (float) xpos;
	float_ypos = (float) ypos;
}

void Teapot::set_scale(float x_scale, float y_scale)
{
	teapot_sprites.set_scale(x_scale, y_scale);
	for (size_t cnt=0; cnt< teapot_collisions.size(); cnt++)
	{
		teapot_collisions[cnt].set_scale(x_scale, y_scale);
	}
}

void Teapot::set_movement_delta(Vec2f &normal, float new_speed)
{
	speed = new_speed;
	x_delta = normal.x * speed;
	y_delta = normal.y * speed;
}

void Teapot::set_color(const Colorf &color)
{
	teapot_sprites.set_color(color);
}


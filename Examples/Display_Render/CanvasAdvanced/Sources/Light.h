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
*/

#pragma once

class Light
{
public:
	Light(clan::Canvas &canvas)
	{
		clip = clan::Texture2D(canvas, "Resources/light.png");
		img = clan::Image(canvas, "Resources/light.png");

		clan::BlendStateDescription blend_desc;
		blend_desc.enable_blending(true);
		blend_desc.set_blend_function(
				clan::blend_zero, clan::blend_one_minus_src_alpha,
				clan::blend_zero, clan::blend_one_minus_src_alpha );

		cutout_blend = clan::BlendState(canvas, blend_desc);


		scale_x = scale_y = 1.0f;
		alive = true;
		id = 0;
		color.r = color.g = color.b = color.a = 1.0f;
	};

	~Light()
	{
	};

	void update(float micro_second)
	{
		calc_rect();
	};

	void cutout(clan::Canvas &canvas)
	{
		// Setup blending for zero blend (erase).
		canvas.set_blend_state(cutout_blend);

		// Now we draw our texture to the buffer.
		clan::Image image(canvas, clip, clip.get_size());
		
		if(USE_SCALE)
		{
			clan::Rectf r;

			// Since it's a smaller texture scaled up, we must account for that.
			r.left = hot_rect.left / CANVAS_SCALE_X;
			r.top = hot_rect.top / CANVAS_SCALE_Y;
			r.right = hot_rect.right / CANVAS_SCALE_X;
			r.bottom = hot_rect.bottom / CANVAS_SCALE_Y;
			image.draw(canvas, r);
		}
		else
			image.draw(canvas, hot_rect);

		canvas.reset_blend_state();
	};

	void draw(clan::Canvas canvas)
	{
		img.draw(canvas, hot_rect);
	};

	void calc_rect()
	{
		clan::Rect r = clip.get_size();

		// We do some work here to draw our lights at the center point location, not the top-left (there's probably a better way to do this).
		hot_rect.left = x - ((r.right*scale_x) / 2);
		hot_rect.top = y - ((r.bottom*scale_y) / 2);
		hot_rect.right = x + ((r.right*scale_x) / 2);
		hot_rect.bottom = y + ((r.bottom*scale_y) / 2);
	};

	void set_life(unsigned int time)
	{
		life.func_expired().set(this, &Light::life_expired);
		life.start(time,false);
	};

	void life_expired()
	{
		alive = false;
	};

	void set_translation(float posX, float posY) 
	{
		x = posX;
		y = posY;
	};

	void set_scale(float sizeX, float sizeY) 
	{ 
		scale_x = sizeX;
		scale_y = sizeY;
	};

	void set_color(clan::Colorf the_color) { color = the_color; img.set_color(color); };

	bool get_alive() { return alive; };

	void set_id(unsigned int val) { id = val; };
	unsigned int get_id() { return id; };

private:
	clan::Texture2D		clip;			// Our clip must interact with the frame buffer, so it's a texture.
	clan::Image		img;			// Our colored lights can be images, though, since they are just simple draws.
	float			x, y;			// Position
	float			scale_x;		// Scaling
	float			scale_y;
	unsigned int	id;				// ID (supplied from the container).
	clan::Colorf		color;			// Color of the light image.
	clan::Rectf		hot_rect;		// The dest rect for hotspot orientation.
	clan::BlendState cutout_blend;
	bool			alive;			// Am I alive?
	clan::Timer		life;			// My life timer.
};


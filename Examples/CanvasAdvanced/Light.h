#ifndef LIGHT_H
#define LIGHT_H

class Light
{
public:
	Light(CL_GraphicContext gc)
	{
		clip = CL_Texture(gc, "data/light.png");
		img = CL_Image(gc, "data/light.png");

		scale_x = scale_y = 1.0f;
		alive = true;
		id = 0;
		color.r = color.g = color.b = color.a = 1.0f;
	};

	~Light()
	{
		clip = CL_Texture();
		img = CL_Image();
	};

	void update(float micro_second)
	{
		calc_rect();
	};

	void cutout(CL_GraphicContext gc)
	{
		CL_BlendMode blender;

		// Setup blending for zero blend (erase).
		blender.enable_blending(true);
		blender.set_blend_function(
				cl_blend_zero, cl_blend_one_minus_src_alpha,
				cl_blend_zero, cl_blend_one_minus_src_alpha );
		gc.set_blend_mode(blender);

		// Now we draw our texture to the buffer.
		gc.set_texture(0, clip);
		
		if(USE_SCALE)
		{
			CL_Rectf r;

			// Since it's a smaller texture scaled up, we must account for that.
			r.left = hot_rect.left / CANVAS_SCALE_X;
			r.top = hot_rect.top / CANVAS_SCALE_Y;
			r.right = hot_rect.right / CANVAS_SCALE_X;
			r.bottom = hot_rect.bottom / CANVAS_SCALE_Y;
			CL_Draw::texture(gc, r, CL_Colorf(1.0f,1.0f,1.0f,1.0f));
		}
		else
			CL_Draw::texture(gc, hot_rect, CL_Colorf(1.0f,1.0f,1.0f,1.0f));

		gc.pop_modelview();

		gc.reset_blend_mode();
		gc.reset_texture(0);
	};

	void draw(CL_GraphicContext gc)
	{
		img.draw(gc, hot_rect);
	};

	void calc_rect()
	{
		CL_Rect r = clip.get_size();

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

	void set_color(CL_Colorf the_color) { color = the_color; img.set_color(color); };

	bool get_alive() { return alive; };

	void set_id(unsigned int val) { id = val; };
	unsigned int get_id() { return id; };

private:
	CL_Texture		clip;			// Our clip must interact with the frame buffer, so it's a texture.
	CL_Image		img;			// Our colored lights can be images, though, since they are just simple draws.
	float			x, y;			// Position
	float			scale_x;		// Scaling
	float			scale_y;
	unsigned int	id;				// ID (supplied from the container).
	CL_Colorf		color;			// Color of the light image.
	CL_Rectf		hot_rect;		// The dest rect for hotspot orientation.

	bool			alive;			// Am I alive?
	CL_Timer		life;			// My life timer.
};

#endif
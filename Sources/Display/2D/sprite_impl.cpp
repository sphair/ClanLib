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
**    Kenneth Gangstoe
**    Mark Page
*/

#include "Display/precomp.h"
#include "sprite_impl.h"
#include "render_batch_triangle.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/System/system.h"
#include "Display/Render/graphic_context_impl.h"
#include <list>
#include "API/Display/2D/canvas.h"
#include "canvas_impl.h"
#include "API/Core/IOData/path_help.h"

namespace clan
{
	Sprite_Impl::Sprite_Impl() :
		angle(Angle(0.0f, AngleUnit::radians)),
		angle_pitch(Angle(0.0f, AngleUnit::radians)),
		angle_yaw(Angle(0.0f, AngleUnit::radians)),
		base_angle(Angle(0.0f, AngleUnit::radians)),
		scale(1.0f, 1.0f),
		color(1.0f, 1.0f, 1.0f, 1.0f),
		linear_filter(true),
		translation_hotspot(0, 0),
		rotation_hotspot(0, 0),
		translation_origin(Origin::top_left),
		rotation_origin(Origin::center),
		current_frame(0),
		delta_frame(1),
		update_time_ms(0),
		id(0),
		finished(false),
		looping(false),
		play_loop(true),
		play_backward(false),
		play_pingpong(false),
		show_on_finish(Sprite::show_blank)
	{
	}

	Sprite_Impl::~Sprite_Impl()
	{
	}

	const Sprite_Impl::SpriteFrame *Sprite_Impl::get_frame(unsigned int index) const
	{
		if (index < frames.size())
			return &frames[index];
		else
			return nullptr;
	}

	Sprite_Impl::SpriteFrame *Sprite_Impl::get_frame(unsigned int index)
	{
		if (index < frames.size())
			return &frames[index];
		else
			return nullptr;
	}

	Sprite_Impl &Sprite_Impl::operator =(const Sprite_Impl &copy)
	{
		angle = copy.angle;
		angle_pitch = copy.angle_pitch;
		angle_yaw = copy.angle_yaw;
		base_angle = copy.base_angle;
		scale = copy.scale;
		color = copy.color;
		translation_hotspot = copy.translation_hotspot;
		rotation_hotspot = copy.rotation_hotspot;
		translation_origin = copy.translation_origin;
		rotation_origin = copy.rotation_origin;
		current_frame = copy.current_frame;
		delta_frame = copy.delta_frame;
		update_time_ms = copy.update_time_ms;
		id = copy.id;
		finished = copy.finished;
		play_loop = copy.play_loop;
		play_backward = copy.play_backward;
		play_pingpong = copy.play_pingpong;
		show_on_finish = copy.show_on_finish;

		frames = copy.frames;

		return *this;
	}

	void Sprite_Impl::draw(Canvas &canvas, float x, float y)
	{
		SpriteFrame &frame = frames[current_frame];
		draw(canvas, frame.position, Pointf(x, y), scale);
	}

	void Sprite_Impl::draw(Canvas &canvas, const Rectf &src, const Rectf &dest)
	{
		SpriteFrame &frame = frames[current_frame];
		draw(canvas, Rectf(frame.position.left + src.left, frame.position.top + src.top, src.get_size()),
			dest.get_top_left(),
			Pointf(dest.get_width() / src.get_width(), dest.get_height() / src.get_height())
			);
	}

	void Sprite_Impl::draw(Canvas &canvas, const Rectf &dest)
	{
		SpriteFrame &frame = frames[current_frame];
		draw(canvas, frame.position, dest.get_top_left(), Pointf(dest.get_width() / float(frame.position.get_width()), dest.get_height() / float(frame.position.get_height())));
	}

	void Sprite_Impl::draw(Canvas &canvas, const Rect &p_src, const Pointf &p_dest, const Pointf &p_scale)
	{
		SpriteFrame &frame = frames[current_frame];

		// Find size of surface:
		float src_width = (float)p_src.get_width();
		float src_height = (float)p_src.get_height();

		// Calculate translation hotspot
		Pointf target_translation_hotspot = calc_hotspot(
			translation_origin,
			(float)(translation_hotspot.x + frame.offset.x),
			(float)(translation_hotspot.y + frame.offset.y),
			src_width,
			src_height);

		// Calculate rotation hotspot:
		Pointf target_rotation_hotspot = calc_hotspot(
			rotation_origin,
			(float)(rotation_hotspot.x + frame.offset.x),
			(float)(rotation_hotspot.y + frame.offset.y),
			src_width,
			src_height);

		// Find top left point of destination rectangle and map rotation hotspot to screen coordinates:
		float destWidth = src_width * p_scale.x;
		float destHeight = src_height * p_scale.y;
		float pixDestX = p_dest.x - target_translation_hotspot.x * p_scale.x;
		float pixDestY = p_dest.y - target_translation_hotspot.y * p_scale.y;
		target_rotation_hotspot.x = float(pixDestX + target_rotation_hotspot.x * p_scale.x);
		target_rotation_hotspot.y = float(pixDestY + target_rotation_hotspot.y * p_scale.y);

		// Calculate unit vectors for rotated surface:
		// (cached for speed reasons)
		static float vect_rotate_x[2] = { 1.0f, 0.0f };
		static float vect_rotate_y[2] = { 0.0f, 1.0f };
		static Angle last_angle(0, AngleUnit::radians);

		Angle target_rotate_angle = angle - base_angle;
		if (last_angle != target_rotate_angle)
		{
			float angle_degrees = target_rotate_angle.to_degrees();
			if (angle_degrees == 0.0f)
			{
				vect_rotate_x[0] = 1.0;
				vect_rotate_x[1] = 0.0;
				vect_rotate_y[0] = 0.0;
				vect_rotate_y[1] = 1.0;
			}
			else if (angle_degrees == 90.0f)
			{
				vect_rotate_x[0] = 0.0;
				vect_rotate_x[1] = 1.0;
				vect_rotate_y[0] = -1.0;
				vect_rotate_y[1] = 0.0;
			}
			else if (angle_degrees == 180.0f)
			{
				vect_rotate_x[0] = -1.0;
				vect_rotate_x[1] = 0.0;
				vect_rotate_y[0] = 0.0;
				vect_rotate_y[1] = -1.0;
			}
			else if (angle_degrees == 270.0f)
			{
				vect_rotate_x[0] = 0.0;
				vect_rotate_x[1] = -1.0;
				vect_rotate_y[0] = 1.0;
				vect_rotate_y[1] = 0.0;
			}
			else
			{
				float angle_rad = target_rotate_angle.to_radians();
				vect_rotate_x[0] = cos(angle_rad);
				vect_rotate_x[1] = sin(angle_rad);
				vect_rotate_y[0] = cos(PI / 2 + angle_rad);
				vect_rotate_y[1] = sin(PI / 2 + angle_rad);
			}
		}

		// Calculate final source rectangle points for render:
		const Texture2D &texture = frames[current_frame].texture;
		float texture_width = texture.get_width();
		float texture_height = texture.get_height();

		Pointf texture_position[4];	// Scaled to the range of 0.0f to 1.0f
		Pointf dest_position[4];

		texture_position[0].x = (((float)p_src.left)) / texture_width;
		texture_position[1].x = (((float)p_src.left + src_width)) / texture_width;
		texture_position[2].x = (((float)p_src.left)) / texture_width;
		texture_position[3].x = (((float)p_src.left + src_width)) / texture_width;

		texture_position[0].y = (((float)p_src.top)) / texture_height;
		texture_position[1].y = (((float)p_src.top)) / texture_height;
		texture_position[2].y = (((float)p_src.top + src_height)) / texture_height;
		texture_position[3].y = (((float)p_src.top + src_height)) / texture_height;

		// Calculate final destination rectangle points for surface rectangle:
		if (target_rotate_angle.to_radians() == 0.0f)
		{
			dest_position[0].x = pixDestX;
			dest_position[1].x = pixDestX + destWidth;
			dest_position[2].x = pixDestX;
			dest_position[3].x = pixDestX + destWidth;

			dest_position[0].y = pixDestY;
			dest_position[1].y = pixDestY;
			dest_position[2].y = pixDestY + destHeight;
			dest_position[3].y = pixDestY + destHeight;
		}
		else
		{
			// Roll
			dest_position[0].x = calc_rotate_x(pixDestX, pixDestY, target_rotation_hotspot.x, target_rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
			dest_position[1].x = calc_rotate_x(pixDestX + destWidth, pixDestY, target_rotation_hotspot.x, target_rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
			dest_position[2].x = calc_rotate_x(pixDestX, pixDestY + destHeight, target_rotation_hotspot.x, target_rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
			dest_position[3].x = calc_rotate_x(pixDestX + destWidth, pixDestY + destHeight, target_rotation_hotspot.x, target_rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);

			dest_position[0].y = calc_rotate_y(pixDestX, pixDestY, target_rotation_hotspot.x, target_rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
			dest_position[1].y = calc_rotate_y(pixDestX + destWidth, pixDestY, target_rotation_hotspot.x, target_rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
			dest_position[2].y = calc_rotate_y(pixDestX, pixDestY + destHeight, target_rotation_hotspot.x, target_rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
			dest_position[3].y = calc_rotate_y(pixDestX + destWidth, pixDestY + destHeight, target_rotation_hotspot.x, target_rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		}

		// Pitch
		if (angle_pitch.to_radians() != 0.0f)
		{
			float pitch_rad = sin(PI / 2 + angle_pitch.to_radians());
			dest_position[0].y = (dest_position[0].y - target_rotation_hotspot.y) * pitch_rad + target_rotation_hotspot.y;
			dest_position[1].y = (dest_position[1].y - target_rotation_hotspot.y) * pitch_rad + target_rotation_hotspot.y;
			dest_position[2].y = (dest_position[2].y - target_rotation_hotspot.y) * pitch_rad + target_rotation_hotspot.y;
			dest_position[3].y = (dest_position[3].y - target_rotation_hotspot.y) * pitch_rad + target_rotation_hotspot.y;
		}
		// Yaw
		if (angle_yaw.to_radians() != 0.0f)
		{
			float yaw_rad = cos(angle_yaw.to_radians());
			dest_position[0].x = (dest_position[0].x - target_rotation_hotspot.x) * yaw_rad + target_rotation_hotspot.x;
			dest_position[1].x = (dest_position[1].x - target_rotation_hotspot.x) * yaw_rad + target_rotation_hotspot.x;
			dest_position[2].x = (dest_position[2].x - target_rotation_hotspot.x) * yaw_rad + target_rotation_hotspot.x;
			dest_position[3].x = (dest_position[3].x - target_rotation_hotspot.x) * yaw_rad + target_rotation_hotspot.x;
		}
		RenderBatchTriangle *batcher = canvas.impl->batcher.get_triangle_batcher();
		batcher->draw_sprite(canvas, texture_position, dest_position, frames[current_frame].texture, color);

	}

	void Sprite_Impl::add_frame(const Texture2D &texture)
	{
		SpriteFrame frame;
		frame.position = texture.get_size();
		frame.texture = texture;
		frame.delay_ms = 60;
		frame.offset = Point(0, 0);
		frames.push_back(frame);
	}

	void Sprite_Impl::add_frame(const Texture2D &texture, const Rect &rect)
	{
		SpriteFrame frame;
		frame.position = rect;
		frame.texture = texture;
		frame.delay_ms = 60;
		frame.offset = Point(0, 0);
		frames.push_back(frame);
	}

	void Sprite_Impl::add_gridclipped_frames(Canvas &canvas,
		const Texture2D &texture,
		int xpos, int ypos,
		int width, int height,
		int xarray, int yarray,
		int array_skipframes,
		int xspace, int yspace)
	{
		int ystart = ypos;
		for (int y = 0; y < yarray; y++)
		{
			int xstart = xpos;
			for (int x = 0; x < xarray; x++)
			{
				if (y == yarray - 1 && x >= xarray - array_skipframes)
					break;

				if (xstart + width > texture.get_width() || ystart + height > texture.get_height())
					throw Exception("add_gridclipped_frames: Outside texture bounds");

				add_frame(texture, Rect(xstart, ystart, xstart + width, ystart + height));
				xstart += width + xspace;
			}
			ystart += height + yspace;
		}
	}

	void Sprite_Impl::add_alphaclipped_frames(Canvas &canvas,
		const Texture2D &texture,
		int xpos, int ypos,
		float trans_limit)
	{
		PixelBuffer alpha_buffer = texture.get_pixeldata(canvas, TextureFormat::rgba8).to_cpu(canvas);

		int begin = 0;
		bool prev_trans = true;

		int alpha_width = alpha_buffer.get_width();
		int alpha_height = alpha_buffer.get_height();
		bool found_opaque = false;
		bool found_trans = false;

		std::vector<int> opaque_row_vector;
		opaque_row_vector.resize(alpha_width);

		int *opaque_row = &(opaque_row_vector[0]);
		memset(opaque_row, 0, alpha_width*sizeof(int));

		int cut_top = ypos;
		int cut_bottom = alpha_height;

		char *data = (char *)alpha_buffer.get_data();

		for (int y = ypos; y < alpha_height; y++)
		{
			bool opaque_line = false;
			Vec4ub *line = (Vec4ub *)(data + alpha_buffer.get_pitch()*y);
			for (int x = 0; x < alpha_width; x++)
			{
				if (line[x].a > trans_limit * 255)
				{
					opaque_row[x] = 1;
					opaque_line = true;
					found_opaque = true;
				}
			}

			if (opaque_line == false) // cut something of top or bottom
			{
				if (found_opaque)
				{
					cut_bottom--;
					found_trans = true;
				}
				else
					cut_top++;
			}
			else if (found_trans)
			{
				found_trans = false;
				cut_bottom = alpha_height;
			}
		}

		if (cut_top >= cut_bottom)
			throw Exception("add_alphaclipped_frames: Image contained only alpha!");

		for (int x = xpos; x < alpha_width; x++)
		{
			if (opaque_row[x] && prev_trans)
			{
				begin = x;
				prev_trans = false;
			}
			else if (!opaque_row[x] && !prev_trans)
			{
				add_frame(texture, Rect(begin, cut_top, x + 1, cut_bottom));
				prev_trans = true;
			}
		}

		if (!prev_trans)
		{
			add_frame(texture, Rect(begin, cut_top, alpha_width, cut_bottom));
		}
	}

	void Sprite_Impl::add_alphaclipped_frames_free(Canvas &canvas,
		const Texture2D &texture,
		int xpos, int ypos,
		float trans_limit)
	{
		PixelBuffer alpha_buffer = texture.get_pixeldata(canvas, TextureFormat::rgba8).to_cpu(canvas);

		int width = alpha_buffer.get_width();
		int height = alpha_buffer.get_height();

		std::vector<int> explored_vector;
		explored_vector.resize(width * height);
		int *explored = &(explored_vector[0]);
		memset(explored, 0, width * height * sizeof(int));

		Vec4ub *data = alpha_buffer.get_data<Vec4ub>();
		int x1, y1, x2, y2;
		bool more;

		for (int y = ypos; y < height; y++)
		{
			for (int x = xpos; x < width; x++)
			{
				if (explored[y*width + x] == 1) continue;
				explored[y*width + x] = 1;
				if (data[y*width + x].a <= trans_limit * 255)
					continue;

				// Initialize the bounding box to the current pixel
				x1 = x2 = x;
				y1 = y2 = y;
				more = true;
				while (more)
				{
					// Assume that there are NO opaque pixels around the current bounding box
					more = false;

					// Scan under the current bounding box and see if there any non-transparent pixels
					for (int i = x1; i <= x2; i++)
					{
						if (y2 + 1 < height)
						{
							explored[(y2 + 1)*width + i] = 1;
							if (data[(y2 + 1)*width + i].a > trans_limit * 255)
							{
								more = true;
								y2 = y2 + 1;
							}
						}
					}

					// Now scan the left and right sides of the current bounding box
					for (int j = y1; j <= y2; j++)
					{
						// Scan the right side
						if (x2 + 1 < width)
						{
							explored[j*width + x2 + 1] = 1;
							if (data[j*width + x2 + 1].a > trans_limit * 255)
							{
								more = true;
								x2 = x2 + 1;
							}
						}
						// Scan the left side
						if (x1 - 1 >= 0)
						{
							explored[j*width + x1 - 1] = 1;
							if (data[j*width + x1 - 1].a > trans_limit * 255)
							{
								more = true;
								x1 = x1 - 1;
							}
						}
					}
				}

				// Mark all pixels in the bounding box as being explored
				for (int i = x1; i <= x2; i++)
				{
					for (int j = y1; j <= y2; j++)
					{
						explored[j*width + i] = 1;
					}
				}

				add_frame(texture, Rect(x1, y1, x2, y2));
			}
		}
	}
}

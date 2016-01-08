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
*/

#pragma once

#include "API/Display/2D/sprite.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Render/texture_2d.h"
#include "render_batch_triangle.h"
#include <vector>

#define calc_rotate_x(px,py,rotation_hotspot_x,rotation_hotspot_y,rotate_x_x,rotate_y_x) \
		(rotation_hotspot_x + (px-rotation_hotspot_x) * rotate_x_x + (py-rotation_hotspot_y) * rotate_y_x)

#define calc_rotate_y(px,py,rotation_hotspot_x,rotation_hotspot_y,rotate_x_y,rotate_y_y) \
		(rotation_hotspot_y + (px-rotation_hotspot_x) * rotate_x_y + (py-rotation_hotspot_y) * rotate_y_y)

namespace clan
{
	class Sprite_Impl
	{
	public:
		Sprite_Impl();
		virtual ~Sprite_Impl();

		class SpriteFrame
		{
		public:
			/// \brief Image data
			Texture2D texture;

			/// \brief Position in surface that frame is located at.
			Rect position;

			/// \brief Draw offset
			Point offset;

			/// \brief Animation delay (in milliseconds)
			int delay_ms;
		};

		const SpriteFrame *get_frame(unsigned int index) const;
		SpriteFrame *get_frame(unsigned int index);

		void add_frame(const Texture2D &texture);
		void add_frame(const Texture2D &texture, const Rect &rect);
		void add_gridclipped_frames(Canvas &canvas, const Texture2D &texture, int xpos, int ypos, int width, int height, int xarray = 1, int yarray = 1, int array_skipframes = 0, int xspacing = 0, int yspacing = 0);
		void add_alphaclipped_frames(Canvas &canvas, const Texture2D &texture, int xpos = 0, int ypos = 0, float trans_limit = 0.05f);
		void add_alphaclipped_frames_free(Canvas &canvas, const Texture2D &texture, int xpos = 0, int ypos = 0, float trans_limit = 0.05f);

		Angle angle, angle_pitch, angle_yaw;
		Angle base_angle;

		Pointf scale;
		Colorf color;

		bool linear_filter;

		Point translation_hotspot;
		Point rotation_hotspot;
		Origin translation_origin;
		Origin rotation_origin;

		int current_frame;
		int delta_frame;
		int update_time_ms;

		int id;
		bool finished;
		bool looping;
		bool play_loop;
		bool play_backward;
		bool play_pingpong;
		Sprite::ShowOnFinish show_on_finish;

		std::vector<SpriteFrame> frames;

		/// \brief Copy assignment operator.
		Sprite_Impl &operator =(const Sprite_Impl &copy);

		bool is_visible() const
		{
			return (finished == false || show_on_finish != Sprite::show_blank) && frames.size() > 0;
		}

		void draw(Canvas &canvas, float x, float y);
		void draw(Canvas &canvas, const Rectf &src, const Rectf &dest);
		void draw(Canvas &canvas, const Rectf &dest);

		void draw(Canvas &canvas, const Rect &p_src, const Pointf &p_dest, const Pointf &p_scale);

		// inlined this function for performance reasons.
		static inline Pointf calc_hotspot(Origin origin, float hotspot_x, float hotspot_y, float size_width, float size_height)
		{
			switch (origin)
			{
			case origin_top_left:
			default:
				return Pointf(-hotspot_x, -hotspot_y);
				break;
			case origin_top_center:
				return Pointf(size_width / 2 - hotspot_x, -hotspot_y);
				break;
			case origin_top_right:
				return Pointf(size_width - hotspot_x, -hotspot_y);
				break;
			case origin_center_left:
				return Pointf(-hotspot_x, size_height / 2 - hotspot_y);
				break;
			case origin_center:
				return Pointf(size_width / 2 - hotspot_x, size_height / 2 - hotspot_y);
				break;
			case origin_center_right:
				return Pointf(size_width - hotspot_x, size_height / 2 - hotspot_y);
				break;
			case origin_bottom_left:
				return Pointf(-hotspot_x, size_height - hotspot_y);
				break;
			case origin_bottom_center:
				return Pointf(size_width / 2 - hotspot_x, size_height - hotspot_y);
				break;
			case origin_bottom_right:
				return Pointf(size_width - hotspot_x, size_height - hotspot_y);
				break;
			}
		}

		Signal<void()> sig_animation_finished;
	};
}

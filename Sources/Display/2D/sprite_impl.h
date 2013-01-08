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
**    Kenneth Gangstoe
**    (if your name is missing here, please add it)
*/

#pragma once

#include "API/Display/2D/sprite.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Render/texture_2d.h"
#include "render_batch_triangle.h"
#include <vector>

namespace clan
{

struct Surface_TargetDrawParams1
{
	float pixDestX;
	float pixDestY;
	float destWidth;
	float destHeight;
	Pointf rotation_hotspot;
};

struct Surface_DrawParams2
{
	int srcX;
	int srcY;
	int srcWidth;
	int srcHeight;
	float destX;
	float destY;
	Colorf color;
	float scale_x;
	float scale_y;
	Origin translate_origin;
	int translate_x;
	int translate_y;
	Angle rotate_angle;
	Angle rotate_pitch;
	Angle rotate_yaw;
	Origin rotate_origin;
	int rotate_x;
	int rotate_y;
	bool sub_pixel_accuracy;
};

struct Surface_DrawParams1
{
	Pointf texture_position[4];	// Scaled to the range of 0.0f to 1.0f
	Pointf dest_position[4];
	Colorf color;
};

class Sprite_Impl
{
/// \name Construction
/// \{
public:
	Sprite_Impl();

	virtual ~Sprite_Impl();

	void init(
		GraphicContext &gc,
		const std::string &resource_id,
		ResourceManager *resources, const ImageImportDescription &import_desc);

public:
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

/// \}
/// \name Attributes
/// \{
public:
	const SpriteFrame *get_frame(unsigned int index) const;
	SpriteFrame *get_frame(unsigned int index);

	Angle angle;
	Angle base_angle;
	float scale_x, scale_y;

	Colorf color;

	bool linear_filter;

	Point translation_hotspot;
	Point rotation_hotspot;
	Origin translation_origin;
	Origin rotation_origin;

	int current_frame;
	int delta_frame;

	int update_time_ms;
	unsigned int last_time_ms;

	int id;

	bool finished;
	bool looping;
	bool play_loop;
	bool play_backward;
	bool play_pingpong;
	Sprite::ShowOnFinish show_on_finish;

	std::vector<SpriteFrame> frames;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Copy assignment operator.
	Sprite_Impl &operator =(const Sprite_Impl &copy);

	bool is_visible() const
	{
		return (finished == false || show_on_finish != Sprite::show_blank) && frames.size() > 0;
	}

	int calc_time_elapsed();

	void create_textures(GraphicContext &gc, const SpriteDescription &description);

	void draw(Canvas &canvas, float x, float y);
	void draw(Canvas &canvas, const Rectf &src, const Rectf &dest);
	void draw(Canvas &canvas, const Rectf &dest);

	void draw(Canvas &canvas, const Surface_DrawParams1 &params1);
	void draw(Canvas &canvas, const Surface_DrawParams2 &params2);

	void draw_calcs_step1(
		const Surface_DrawParams2 &params2,
		Surface_TargetDrawParams1 &t_params1);
	void draw_calcs_step2(
		const Surface_DrawParams2 &params2,
		const Surface_TargetDrawParams1 *t_params1,
		Surface_DrawParams1 &params1);

	// inlined this function for performance reasons.
	static inline Pointf calc_hotspot(Origin origin, float hotspot_x, float hotspot_y, float size_width, float size_height)
	{
		switch(origin)
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

	#define calc_rotate_x(px,py,rotation_hotspot_x,rotation_hotspot_y,rotate_x_x,rotate_y_x) \
		(rotation_hotspot_x + (px-rotation_hotspot_x) * rotate_x_x + (py-rotation_hotspot_y) * rotate_y_x)

	#define calc_rotate_y(px,py,rotation_hotspot_x,rotation_hotspot_y,rotate_x_y,rotate_y_y) \
		(rotation_hotspot_y + (px-rotation_hotspot_x) * rotate_x_y + (py-rotation_hotspot_y) * rotate_y_y)

/// \}
/// \name Signals
/// \{
public:
	Signal_v0 sig_animation_finished;
/// \}
};

}

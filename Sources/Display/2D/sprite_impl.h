/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "API/Display/Render/texture.h"
#include "API/Display/Render/blend_mode.h"
#include "render_batch2d.h"
#include <vector>

struct CL_Surface_TargetDrawParams1
{
	float pixDestX;
	float pixDestY;
	float destWidth;
	float destHeight;
	CL_Pointf rotation_hotspot;
};

struct CL_Surface_DrawParams2
{
	int srcX;
	int srcY;
	int srcWidth;
	int srcHeight;
	float destX;
	float destY;
	float destZ;
	CL_Colorf color;
	float scale_x;
	float scale_y;
	CL_Origin translate_origin;
	int translate_x;
	int translate_y;
	CL_Angle rotate_angle;
	CL_Angle rotate_pitch;
	CL_Angle rotate_yaw;
	CL_Origin rotate_origin;
	int rotate_x;
	int rotate_y;
	bool sub_pixel_accuracy;
};

struct CL_Surface_DrawParams1
{
	CL_Pointf texture_position[4];	// Scaled to the range of 0.0f to 1.0f
	CL_Pointf dest_position[4];
	float destZ;
	CL_Colorf color[4];
};

class CL_Sprite_Impl
{
/// \name Construction
/// \{
public:
	CL_Sprite_Impl();

	virtual ~CL_Sprite_Impl();

	void init(
		CL_GraphicContext &gc,
		const CL_StringRef &resource_id,
		CL_ResourceManager *resources, const CL_ImageImportDescription &import_desc);

public:
	class SpriteFrame
	{
	public:
		/// \brief Image data
		CL_Texture texture;

		/// \brief Position in surface that frame is located at.
		CL_Rect position;

		/// \brief Draw offset
		CL_Point offset;

		/// \brief Animation delay (in milliseconds)
		int delay_ms;
	};

/// \}
/// \name Attributes
/// \{
public:
	const SpriteFrame *get_frame(unsigned int index) const;
	SpriteFrame *get_frame(unsigned int index);

	CL_Angle angle, angle_pitch, angle_yaw;
	CL_Angle base_angle;
	float scale_x, scale_y;

	CL_Colorf color;

	bool linear_filter;

	CL_Point translation_hotspot;
	CL_Point rotation_hotspot;
	CL_Origin translation_origin;
	CL_Origin rotation_origin;

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
	CL_Sprite::ShowOnFinish show_on_finish;

	std::vector<SpriteFrame> frames;

	CL_TextureGroup texture_group;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Copy assignment operator.
	CL_Sprite_Impl &operator =(const CL_Sprite_Impl &copy);

	bool is_visible() const
	{
		return (finished == false || show_on_finish != CL_Sprite::show_blank) && frames.size() > 0;
	}

	int calc_time_elapsed();

	void create_textures(CL_GraphicContext &gc, const CL_SpriteDescription &description);

	void draw(CL_GraphicContext &gc, float x, float y);
	void draw(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest);
	void draw(CL_GraphicContext &gc, const CL_Rectf &dest);

	void draw(CL_GraphicContext &gc, const CL_Surface_DrawParams1 &params1);
	void draw(CL_GraphicContext &gc, const CL_Surface_DrawParams2 &params2);

	void draw_calcs_step1(
		const CL_Surface_DrawParams2 &params2,
		CL_Surface_TargetDrawParams1 &t_params1);
	void draw_calcs_step2(
		const CL_Surface_DrawParams2 &params2,
		const CL_Surface_TargetDrawParams1 *t_params1,
		CL_Surface_DrawParams1 &params1);

	// inlined this function for performance reasons.
	static inline CL_Pointf calc_hotspot(CL_Origin origin, float hotspot_x, float hotspot_y, float size_width, float size_height)
	{
		switch(origin)
		{
		case origin_top_left:
		default:
			return CL_Pointf(-hotspot_x, -hotspot_y);
			break;
		case origin_top_center:
			return CL_Pointf(size_width / 2 - hotspot_x, -hotspot_y);
			break;
		case origin_top_right:
			return CL_Pointf(size_width - hotspot_x, -hotspot_y);
			break;
		case origin_center_left:
			return CL_Pointf(-hotspot_x, size_height / 2 - hotspot_y);
			break;
		case origin_center:
			return CL_Pointf(size_width / 2 - hotspot_x, size_height / 2 - hotspot_y);
			break;
		case origin_center_right:
			return CL_Pointf(size_width - hotspot_x, size_height / 2 - hotspot_y);
			break;
		case origin_bottom_left:
			return CL_Pointf(-hotspot_x, size_height - hotspot_y);
			break;
		case origin_bottom_center:
			return CL_Pointf(size_width / 2 - hotspot_x, size_height - hotspot_y);
			break;
		case origin_bottom_right:
			return CL_Pointf(size_width - hotspot_x, size_height - hotspot_y);
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
	CL_Signal_v0 sig_animation_finished;
/// \}
};

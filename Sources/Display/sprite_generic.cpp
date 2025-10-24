/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/Core/System/system.h"
#include "sprite_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Generic construction:

CL_Sprite_Generic::CL_Sprite_Generic() :
	angle(0.0f),
	angle_pitch(0.0f),
	angle_yaw(0.0f),
	base_angle(0.0f),
	scale_x(1.0f),
	scale_y(1.0f),
	red(1.0f),
	green(1.0f),
	blue(1.0f),
	alpha(1.0f),

	blend_src           (blend_src_alpha),
	blend_dest          (blend_one_minus_src_alpha),
	blendfunc_src_alpha (blend_src_alpha),
	blendfunc_dest_alpha(blend_one_minus_src_alpha),

	translation_hotspot(0,0),
	rotation_hotspot(0,0),
	translation_origin(origin_top_left),
	rotation_origin(origin_center),
	current_frame(0),
	delta_frame(1),
	update_time(0),
	last_time(0),
	id(0),
	finished(false),
	looping(false),
	play_loop(true),
	play_backward(false),
	play_pingpong(false),
	show_on_finish(CL_Sprite::show_blank)
{
}

CL_Sprite_Generic::~CL_Sprite_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Generic attributes:

CL_Sprite_Generic::SpriteFrame *CL_Sprite_Generic::get_frame(unsigned int index)
{
	if(index >= 0 && index < frames.size())
		return &frames[index];
	else
		return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Generic operations:

CL_Sprite_Generic &CL_Sprite_Generic::operator =(const CL_Sprite_Generic &copy)
{
	angle = copy.angle;
	angle_pitch = copy.angle_pitch;
	angle_yaw = copy.angle_yaw;
	base_angle = copy.base_angle;
	scale_x = copy.scale_x;
	scale_y = copy.scale_y;
	red = copy.red;
	green = copy.green;
	blue = copy.blue;
	alpha = copy.alpha;
	blend_src  = copy.blend_src;
	blend_dest = copy.blend_dest;
	blendfunc_src_alpha  = copy.blendfunc_src_alpha;
	blendfunc_dest_alpha = copy.blendfunc_dest_alpha;
	translation_hotspot = copy.translation_hotspot;
	rotation_hotspot = copy.rotation_hotspot;
	translation_origin = copy.translation_origin;
	rotation_origin = copy.rotation_origin;
	current_frame = copy.current_frame;
	delta_frame = copy.delta_frame;
	update_time = copy.update_time;
	last_time = copy.last_time;
	id = copy.id;
	finished = copy.finished;
	looping = copy.looping;
	play_loop = copy.play_loop;
	play_backward = copy.play_backward;
	play_pingpong = copy.play_pingpong;
	show_on_finish = copy.show_on_finish;

	frames = copy.frames;

	return *this;
}

float CL_Sprite_Generic::calc_time_elapsed()
{
	// Calculate amount of time since last frame
	float new_time = (float)CL_System::get_time();
	if(last_time == 0)
		last_time = new_time;

	float delta_time = (new_time - last_time) / 1000.0f;
	last_time = new_time;

	return delta_time;
}
 
/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Generic implementation:

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

#ifndef header_sprite_generic
#define header_sprite_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Display/surface.h"
#include "API/Display/blend_func.h"
#include "API/Core/Math/origin.h"
#include "API/Display/sprite.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"

#include <vector>

class CL_Sprite_Generic
{
//! Construction:
public:
	CL_Sprite_Generic();

	virtual ~CL_Sprite_Generic();

public:
	class SpriteFrame
	{
	public:
		//: Image data
		CL_Surface surface;

		//: Position in surface that frame is located at.
		CL_Rect position;

		//: Draw offset 
		CL_Point offset;

		//: Animation delay
		float delay;
	};

//! Attributes:
public:
	SpriteFrame *get_frame(unsigned int index);
	
	float angle, angle_pitch, angle_yaw;
	float base_angle;
	float scale_x, scale_y;

	float red,green,blue,alpha;

	CL_BlendFunc blend_src;
	CL_BlendFunc blend_dest;
	CL_BlendFunc blendfunc_src_alpha;
	CL_BlendFunc blendfunc_dest_alpha;

	CL_Point translation_hotspot;
	CL_Point rotation_hotspot;
	CL_Origin translation_origin;
	CL_Origin rotation_origin;

	short current_frame;
	short delta_frame;

	float update_time;
	float last_time;

	int id;
	
	bool finished;
	bool looping;
	bool play_loop;
	bool play_backward;
	bool play_pingpong;
	CL_Sprite::ShowOnFinish show_on_finish;

	std::vector<SpriteFrame> frames;

//! Operations:
public:
	//: Copy assignment operator.
	CL_Sprite_Generic &operator =(const CL_Sprite_Generic &copy);

	float calc_time_elapsed();

//! Signals:
public:
	CL_Signal_v0 sig_animation_finished;
};

#endif

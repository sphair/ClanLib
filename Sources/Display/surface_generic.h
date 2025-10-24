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

#ifndef header_surface_generic
#define header_surface_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/Math/origin.h"
#include "API/Display/blend_func.h"
#include "API/Core/Math/point.h"

class CL_Surface_Target;

class CL_Surface_Generic
{
//! Construction:
public:
	CL_Surface_Generic(CL_Surface_Target *target);

	virtual ~CL_Surface_Generic();

//! Attributes:
public:
	//: Shared data object between surfaces.
	CL_Surface_Target *target;

	float angle, angle_pitch, angle_yaw;

	float scale_x, scale_y;

	float red, green, blue, alpha;

	CL_BlendFunc blend_src;
	
	CL_BlendFunc blend_dest;

	CL_BlendFunc blendfunc_src_alpha;
	
	CL_BlendFunc blendfunc_dest_alpha;

	CL_Pointf translation_hotspot;

	CL_Pointf rotation_hotspot;

	CL_Origin translation_origin;

	CL_Origin rotation_origin;

//! Operations:
public:
	//: Copy assignment operator.
	CL_Surface_Generic &operator =(const CL_Surface_Generic &copy);

	CL_Surface_Generic *copy_on_write();

	void release_ref();

	void add_ref();

//! Implementation:
private:
	int ref_count;
};

#endif

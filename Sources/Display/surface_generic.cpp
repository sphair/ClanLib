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
#include "surface_generic.h"
#include "surface_target.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Generic construction:

CL_Surface_Generic::CL_Surface_Generic(CL_Surface_Target *target) :
	target(target),
	angle(0.0f),
	angle_pitch(0.0f),
	angle_yaw(0.0f),
	scale_x(1.0f),
	scale_y(1.0f),
	red(1.0f),
	green(1.0f),
	blue(1.0f),
	alpha(1.0f),
	blend_src(blend_src_alpha),
	blend_dest(blend_one_minus_src_alpha),
	blendfunc_src_alpha (blend_src_alpha),
	blendfunc_dest_alpha(blend_one_minus_src_alpha),
	translation_hotspot(0,0),
	rotation_hotspot(0,0),
	translation_origin(origin_top_left),
	rotation_origin(origin_center),
	ref_count(0)
{
	if (target) target->add_ref();
}	

CL_Surface_Generic::~CL_Surface_Generic()
{
	if (target) target->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Generic operations:

CL_Surface_Generic &CL_Surface_Generic::operator =(const CL_Surface_Generic &copy)
{
	if (target) target->release_ref();
	target = copy.target;
	angle = copy.angle;
	angle_pitch = copy.angle_pitch;
	angle_yaw = copy.angle_yaw;
	scale_x = copy.scale_x;
	scale_y = copy.scale_y;
	alpha   = copy.alpha;

	blend_src        = copy.blend_src;
	blend_dest       = copy.blend_dest;
	blendfunc_src_alpha  = copy.blendfunc_src_alpha;
	blendfunc_dest_alpha = copy.blendfunc_dest_alpha;

	translation_hotspot = copy.translation_hotspot;
	rotation_hotspot    = copy.rotation_hotspot;
	translation_origin  = copy.translation_origin;
	rotation_origin     = copy.rotation_origin;

	if (target) target->add_ref();

	return *this;
}

CL_Surface_Generic *CL_Surface_Generic::copy_on_write()
{
	if (ref_count == 1) return this;

	CL_Surface_Generic *copy = new CL_Surface_Generic(0);
	*copy = *this;
	copy->add_ref();
	release_ref();
	return copy;
}

void CL_Surface_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

void CL_Surface_Generic::add_ref()
{
	ref_count++;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Generic implementation:

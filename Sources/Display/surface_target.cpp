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
#include "surface_target.h"
#include "API/Display/surface.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Target construction:

CL_Surface_Target::CL_Surface_Target()
: ref_count(0)
{
}

CL_Surface_Target::~CL_Surface_Target()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Target attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Target operations:

void CL_Surface_Target::setup_target_params(
	const CL_Surface_DrawParams2 &params2,
	CL_Surface_TargetDrawParams1 &t_params1)
{
	// Find size of surface:
	float size_width  = (float) params2.srcWidth;
	float size_height = (float) params2.srcHeight;

	// Calculate translation hotspot
	CL_Pointf translation_hotspot = calc_hotspot(
		params2.translate_origin,
		(float) params2.translate_x,
		(float) params2.translate_y,
		size_width,
		size_height);

	// Calculate rotation hotspot:
	t_params1.rotation_hotspot = calc_hotspot(
		params2.rotate_origin,
		(float) params2.rotate_x,
		(float) params2.rotate_y,
		size_width,
		size_height);

	if (!params2.sub_pixel_accuracy)
	{
		// Avoid linear smoothing:
		translation_hotspot.x = float(int(translation_hotspot.x));
		translation_hotspot.y = float(int(translation_hotspot.y));
		t_params1.rotation_hotspot.x = float(int(t_params1.rotation_hotspot.x));
		t_params1.rotation_hotspot.y = float(int(t_params1.rotation_hotspot.y));
	}

	// Find top left point of destination rectangle and map rotation hotspot to screen coordinates:
	if (params2.scale_x == 1.0 && params2.scale_y == 1.0)
	{
		t_params1.destWidth = params2.srcWidth;
		t_params1.destHeight = params2.srcHeight;
		t_params1.pixDestX = params2.destX-translation_hotspot.x;
		t_params1.pixDestY = params2.destY-translation_hotspot.y;
		t_params1.rotation_hotspot.x += float(t_params1.pixDestX);
		t_params1.rotation_hotspot.y += float(t_params1.pixDestY);
	}
	else
	{
		t_params1.destWidth = params2.srcWidth * params2.scale_x;
		t_params1.destHeight = params2.srcHeight * params2.scale_y;
		t_params1.pixDestX = params2.destX-translation_hotspot.x * params2.scale_x;
		t_params1.pixDestY = params2.destY-translation_hotspot.y * params2.scale_y;
		t_params1.rotation_hotspot.x = float(t_params1.pixDestX + t_params1.rotation_hotspot.x * params2.scale_x);
		t_params1.rotation_hotspot.y = float(t_params1.pixDestY + t_params1.rotation_hotspot.y * params2.scale_y);
	}
}


void CL_Surface_Target::setup_draw_params(
	const CL_Surface_DrawParams2 & params2,
	const CL_Surface_TargetDrawParams1 *t_params1,
	CL_Surface_DrawParams1 &params1)
{

	// Calculate unit vectors for rotated surface:
	// (cached for speed reasons)
	static double vect_rotate_x[2] = { 1.0, 0.0 };
	static double vect_rotate_y[2] = { 0.0, 1.0 };
	static double last_angle = 0.0;

	if (last_angle != params2.rotate_angle)
	{
		if (params2.rotate_angle == 0.0)
		{
			vect_rotate_x[0] = 1.0;
			vect_rotate_x[1] = 0.0;
			vect_rotate_y[0] = 0.0;
			vect_rotate_y[1] = 1.0;
		}
		else if (params2.rotate_angle == 90.0)
		{
			vect_rotate_x[0] = 0.0;
			vect_rotate_x[1] = 1.0;
			vect_rotate_y[0] = -1.0;
			vect_rotate_y[1] = 0.0;
		}
		else if (params2.rotate_angle == 180.0)
		{
			vect_rotate_x[0] = -1.0;
			vect_rotate_x[1] = 0.0;
			vect_rotate_y[0] = 0.0;
			vect_rotate_y[1] = -1.0;
		}
		else if (params2.rotate_angle == 270.0)
		{
			vect_rotate_x[0] = 0.0;
			vect_rotate_x[1] = -1.0;
			vect_rotate_y[0] = 1.0;
			vect_rotate_y[1] = 0.0;
		}
		else
		{
			double angle_rad = 3.1415926f*params2.rotate_angle/180;
			vect_rotate_x[0] = cos(angle_rad);
			vect_rotate_x[1] = sin(angle_rad);
			vect_rotate_y[0] = cos(3.1415926f/2+angle_rad);
			vect_rotate_y[1] = sin(3.1415926f/2+angle_rad);
		}
	}

	// Calculate final source rectangle points for render:

	params1.srcX[0] = params2.srcX;
	params1.srcY[0] = params2.srcY;
	params1.srcX[1] = params2.srcX+params2.srcWidth;
	params1.srcY[1] = params2.srcY;
	params1.srcX[2] = params2.srcX+params2.srcWidth;
	params1.srcY[2] = params2.srcY+params2.srcHeight;
	params1.srcX[3] = params2.srcX;
	params1.srcY[3] = params2.srcY+params2.srcHeight;

	// Calculate final destination rectangle points for surface rectangle:

	if (params2.rotate_angle == 0.0)
	{
		params1.destX[0] = t_params1->pixDestX;
		params1.destY[0] = t_params1->pixDestY;
		params1.destX[1] = t_params1->pixDestX+t_params1->destWidth;
		params1.destY[1] = t_params1->pixDestY;
		params1.destX[2] = t_params1->pixDestX+t_params1->destWidth;
		params1.destY[2] = t_params1->pixDestY+t_params1->destHeight;
		params1.destX[3] = t_params1->pixDestX;
		params1.destY[3] = t_params1->pixDestY+t_params1->destHeight;
	}
	else
	{
		// roll
		params1.destX[0] = calc_rotate_x(t_params1->pixDestX, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.destY[0] = calc_rotate_y(t_params1->pixDestX, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.destX[1] = calc_rotate_x(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.destY[1] = calc_rotate_y(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.destX[2] = calc_rotate_x(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.destY[2] = calc_rotate_y(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.destX[3] = calc_rotate_x(t_params1->pixDestX, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.destY[3] = calc_rotate_y(t_params1->pixDestX, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
	}

	// pitch
	if (params2.rotate_pitch != 0.0)
	{
		double pitch_rad = sin(3.1415926f/2 + (3.1415926f*params2.rotate_pitch/180));
		params1.destY[0] = (params1.destY[0] - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.destY[1] = (params1.destY[1] - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.destY[2] = (params1.destY[2] - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.destY[3] = (params1.destY[3] - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
	}
	// yaw
	if (params2.rotate_yaw != 0.0)
	{
		double yaw_rad = cos(3.1415926f*params2.rotate_yaw/180);
		params1.destX[0] = (params1.destX[0] - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.destX[1] = (params1.destX[1] - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.destX[2] = (params1.destX[2] - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.destX[3] = (params1.destX[3] - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
	}

	params1.red[0] = params2.red;
	params1.red[1] = params2.red;
	params1.red[2] = params2.red;
	params1.red[3] = params2.red;
	params1.green[0] = params2.green;
	params1.green[1] = params2.green;
	params1.green[2] = params2.green;
	params1.green[3] = params2.green;
	params1.blue[0] = params2.blue;
	params1.blue[1] = params2.blue;
	params1.blue[2] = params2.blue;
	params1.blue[3] = params2.blue;
	params1.alpha[0] = params2.alpha;
	params1.alpha[1] = params2.alpha;
	params1.alpha[2] = params2.alpha;
	params1.alpha[3] = params2.alpha;

	params1.destZ = params2.destZ;
	params1.blend_src        = params2.blend_src;
	params1.blend_dest       = params2.blend_dest;
	params1.blendfunc_src_alpha  = params2.blendfunc_src_alpha;
	params1.blendfunc_dest_alpha = params2.blendfunc_dest_alpha;
	params1.sub_pixel_accuracy = params2.sub_pixel_accuracy;

}

void CL_Surface_Target::draw(
	CL_Surface_Generic *attributes,
	const CL_Surface_DrawParams2& params2,
	CL_GraphicContext *context)
{
	static CL_Surface_TargetDrawParams1 t_params1;

	setup_target_params(params2, t_params1);

	draw(
		attributes,
		params2,
		&t_params1,
		context);
}


void CL_Surface_Target::draw(
	CL_Surface_Generic *attributes,
	const CL_Surface_DrawParams2& params2,
	CL_Surface_TargetDrawParams1 *t_params1,
	CL_GraphicContext *context)
{
	static CL_Surface_DrawParams1 params1;

	setup_draw_params(params2, t_params1, params1);

	// Draw the surface:
	draw(attributes,
		  params1,
		  context);
}

void CL_Surface_Target::add_ref()
{
	ref_count++;
}

void CL_Surface_Target::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

int CL_Surface_Target::get_handle() {
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

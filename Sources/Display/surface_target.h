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

#ifndef header_surface_target
#define header_surface_target

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/Math/origin.h"
#include "API/Display/blend_func.h"

class CL_PixelBuffer;
class CL_Surface_Generic;
class CL_GraphicContext;
class CL_Point;
class CL_Pointf;
class CL_Sizef;
class CL_Rect;
struct CL_Surface_DrawParams1;
struct CL_Surface_DrawParams2;

struct CL_Surface_TargetDrawParams1
{
	double pixDestX;
	double pixDestY;
	double destWidth;
	double destHeight;
	CL_Pointf rotation_hotspot;
};

class CL_API_DISPLAY CL_Surface_Target
{
//! Construction:
public:
	CL_Surface_Target();

	virtual ~CL_Surface_Target();

//! Attributes:
public:
	//: Returns the width.
	virtual int get_width() const=0;
	
	//: Returns the height.
	virtual int get_height() const=0;

	//: Returns a pixelbuffer object for accessing surface data.
	virtual CL_PixelBuffer get_pixeldata()=0;

//! Operations:
public:
	//: Uploads pixel data to surface.
	//!param pos: Position on surface where pixel data should be put.
	//!param src_rect: Part of pixelbuffer that should be uploaded.
	//!param pixeldata: CL_PixelBuffer containing the image to be uploaded.
	virtual void set_pixeldata(const CL_Point &pos, const CL_Rect &src_rect, CL_PixelBuffer &pixeldata)=0;

	//: Draw surface on screen.
	virtual void draw(
		CL_Surface_Generic *attributes,
		const CL_Surface_DrawParams1& params1,
		CL_GraphicContext *gc)=0;

	//: Draw surface on screen.
	void draw(
		CL_Surface_Generic *attributes,
		const CL_Surface_DrawParams2& params2,
		CL_GraphicContext *context);

	virtual void draw(
		CL_Surface_Generic *attributes,
		const CL_Surface_DrawParams2& params2,
		CL_Surface_TargetDrawParams1 *target_params1,
		CL_GraphicContext *context);

	
	//: Utilities to calculate final drawing information

	void setup_target_params(
		const CL_Surface_DrawParams2 &params2,
		CL_Surface_TargetDrawParams1 &t_params1);

	void setup_draw_params(
		const CL_Surface_DrawParams2 & params2,
		const CL_Surface_TargetDrawParams1 *t_params1,
		CL_Surface_DrawParams1 &params1);

	
	void add_ref();

	void release_ref();

	//  Get the surface handle
	virtual int get_handle();
	
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

/*

	double calc_rotate_x(
		const double &px,
		const double &py,
		const double &rotation_hotspot_x,
		const double &rotation_hotspot_y,
		const double &rotate_x_x,
		const double &rotate_y_x)
	{
		return 
			rotation_hotspot_x +
			(px-rotation_hotspot_x) * rotate_x_x +
			(py-rotation_hotspot_y) * rotate_y_x;
	}

	double calc_rotate_y(
		const double &px,
		const double &py,
		const double &rotation_hotspot_x,
		const double &rotation_hotspot_y,
		const double &rotate_x_y,
		const double &rotate_y_y)
	{
		return 
			rotation_hotspot_y +
			(px-rotation_hotspot_x) * rotate_x_y +
			(py-rotation_hotspot_y) * rotate_y_y;
	}
*/
//! Implementation:
private:
	int ref_count;
};

#endif

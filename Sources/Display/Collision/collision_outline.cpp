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
**    Harry Storbacka
**    Magnus Norddahl
**    James Wynn
**    Emanuel Greisen
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/Display/Collision/collision_outline.h"
#include "API/Display/Collision/outline_provider_bitmap.h"
#include "API/Display/Collision/outline_provider_file.h"
#include "API/Display/Collision/outline_circle.h"
#include "API/Display/display.h"
#include "API/Display/display_window.h"
#include "API/Display/graphic_context.h"
#include "API/Display/pixel_format.h"
#include "API/Core/System/clanstring.h"
#include "API/Display/Providers/provider_factory.h"
#include "API/Display/pixel_buffer.h"
#include "collision_outline_generic.h"
#include "../surface_target.h"
#include "API/Core/Resources/resource_manager.h"
#include "Display/resourcedata_collisionoutline.h"
#include "API/Core/System/error.h"


/////////////////////////////////////////////////////////////////////////////
// CL_CollisionOutline Construction:

CL_CollisionOutline::CL_CollisionOutline()
 : impl(0)
{
	impl = new CL_CollisionOutline_Generic();
}


CL_CollisionOutline::CL_CollisionOutline(const CL_CollisionOutline &other)
 : impl(0)
{
	impl = new CL_CollisionOutline_Generic();

	*this = other;
}

CL_CollisionOutline::CL_CollisionOutline(std::vector<CL_Contour> contours, int width, int height)
 : impl(0)
{
	impl = new CL_CollisionOutline_Generic();
	impl->contours = contours;
	impl->width = width;
	impl->height = height;
}

CL_CollisionOutline::CL_CollisionOutline(
	const std::string &filename,
	int alpha_limit,
	CL_OutlineAccuracy accuracy,
	bool get_insides)
 : impl(0)
{
	if( CL_String::right(filename, 3) == "out" )
	{
		impl = new CL_CollisionOutline_Generic( new CL_OutlineProviderFile(filename), accuracy_raw );
		return;
	}

	CL_PixelBuffer pbuf = CL_ProviderFactory::load(filename);
	
	if( pbuf.get_format() == CL_PixelFormat::rgba8888 )
	{
		impl = new CL_CollisionOutline_Generic(
			new CL_OutlineProviderBitmap(pbuf, alpha_limit, get_insides), accuracy);
	}
	else
	{
		impl = new CL_CollisionOutline_Generic(
			new CL_OutlineProviderBitmap(pbuf, alpha_limit, get_insides), accuracy_raw);
	}

	set_rotation_hotspot(origin_center);
}

CL_CollisionOutline::CL_CollisionOutline(
	const std::string &resource_id,
	CL_ResourceManager *manager )
 : impl(0)
{
	resource = manager->get_resource(resource_id);
	resource.load();

	CL_ResourceData_CollisionOutline *data =
		(CL_ResourceData_CollisionOutline *) resource.get_data("collisionoutline");

	if (!data)
		throw CL_Error("Resource '" + resource_id + "' is not of type 'collisionoutline'");

	impl = new CL_CollisionOutline_Generic();
	*this = data->get_CollisionOutline();

	resource.unload();
}

CL_CollisionOutline::CL_CollisionOutline(
	CL_PixelBuffer pbuf,
	int alpha_limit,
	CL_OutlineAccuracy accuracy)
 : impl(0)
{
	if( pbuf.get_format() == CL_PixelFormat::rgba8888 )
	{
		impl = new CL_CollisionOutline_Generic(
			new CL_OutlineProviderBitmap(pbuf, alpha_limit), accuracy );
	}
	else
	{
		impl = new CL_CollisionOutline_Generic(
			new CL_OutlineProviderBitmap(pbuf, alpha_limit), accuracy_raw );
	}
	
	set_rotation_hotspot(origin_center);
}

CL_CollisionOutline::~CL_CollisionOutline()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CollisionOutline Attributes:

std::vector<CL_Contour> &CL_CollisionOutline::get_contours() const
{
	return impl->contours;
}

CL_Circlef CL_CollisionOutline::get_minimum_enclosing_disc() const
{
	return impl->minimum_enclosing_disc;
}

CL_Pointf CL_CollisionOutline::get_translation() const
{
	return impl->position;
}

float CL_CollisionOutline::get_angle() const
{
	return impl->angle;
}

CL_Pointf CL_CollisionOutline::get_scale() const
{
	return impl->scale_factor;
}

bool CL_CollisionOutline::get_inside_test() const
{
	return impl->do_inside_test;
}

void CL_CollisionOutline::get_alignment( CL_Origin &origin, float &x, float &y ) const
{
	origin = impl->translation_origin;
	x = impl->translation_offset.x;
	y = impl->translation_offset.y;
}

void CL_CollisionOutline::get_rotation_hotspot( CL_Origin &origin, float &x, float &y) const
{
	origin = impl->rotation_origin;
	x = impl->rotation_hotspot.x;
	y = impl->rotation_hotspot.y;
}

unsigned int CL_CollisionOutline::get_width() const
{
	return impl->width;
}

unsigned int CL_CollisionOutline::get_height() const
{
	return impl->height;
}

std::vector<CL_CollidingContours> &CL_CollisionOutline::get_collision_info() const
{
	return impl->collision_info;
}

void CL_CollisionOutline::set_collision_info(const std::vector<CL_CollidingContours> &colinfo)
{
	impl->collision_info = colinfo;
}

void CL_CollisionOutline::clean_collision_info()
{
	impl->collision_info.clear();
}


void CL_CollisionOutline::get_collision_info_state(bool &points, bool &normals, bool &metadata, bool &pendepth) const
{
	points = impl->collision_info_points;
	normals = impl->collision_info_normals;
	metadata = impl->collision_info_meta;
	pendepth = impl->collision_info_pen_depth;
}


/////////////////////////////////////////////////////////////////////////////
// CL_CollisionOutline Operations:

CL_CollisionOutline &CL_CollisionOutline::operator=(const CL_CollisionOutline &other)
{
	if( this == &other )
		return *this;

	delete impl;
	impl = new CL_CollisionOutline_Generic();

	impl->contours = other.get_contours();
	impl->do_inside_test = other.get_inside_test();
	impl->width = other.get_width();
	impl->height = other.get_height();
	impl->position = other.get_translation();
	impl->scale_factor = other.get_scale();
	impl->angle = other.get_angle();
	//impl->radius = other.get_radius();
	impl->minimum_enclosing_disc = other.get_minimum_enclosing_disc();

	bool points, normals, metadata, pendepths;
	other.get_collision_info_state(points,normals,metadata,pendepths);
	enable_collision_info(points,normals,metadata,pendepths);

	CL_Origin origin;
	float x, y;
	other.get_alignment(origin,x,y);

	impl->translation_origin = origin;
	impl->translation_offset.x = x;
	impl->translation_offset.y = y;

	other.get_rotation_hotspot(origin,x,y);

	impl->rotation_origin = origin;
	impl->rotation_hotspot.x = x;
	impl->rotation_hotspot.y = y;
	return *this;
}

void CL_CollisionOutline::optimize(unsigned char check_distance, float corner_angle )
{
	impl->optimize(check_distance, corner_angle);
}


void CL_CollisionOutline::draw(
	float x,
	float y,
	const CL_Color &color,
	CL_GraphicContext *gc)
{
	// Get a graphics context
	if (gc == 0) gc = CL_Display::get_current_window()->get_gc();
	
	// Draw collision outline (Contours are assumed as closed polygons, hence we use line-loop)
	for(unsigned int i = 0; i < impl->contours.size(); i++)
	{
		// Draw the contour
		unsigned int numpoints = impl->contours[i].points.size();
		for(unsigned int s = 0; s < numpoints; s++)
		{
			const CL_Pointf &p1 = impl->contours[i].points[s];
			const CL_Pointf &p2 = impl->contours[i].points[(s+1) % numpoints];
			gc->draw_line( x + p1.x+0.5f, y + p1.y+0.5f, x + p2.x+0.5f, y + p2.y+0.5f, color);
		}
		// Add points (as oposite color)
		// TODO: This did not work with SDL, the draw_pixel fails
		CL_Color colorinv(255-color.get_red(),255-color.get_green(),255-color.get_blue());
		for(unsigned int s1 = 0; s1 < numpoints; s1++)
		{
			const CL_Pointf &p1 = impl->contours[i].points[s1];
			gc->draw_pixel( int(x + p1.x+0.5f), int(y + p1.y+0.5f), colorinv);
		}
	}
}

void CL_CollisionOutline::draw_sub_circles(
	float x,
	float y,
	const CL_Color &color,
	CL_GraphicContext *gc)
{
	// Get a graphics context
	if (gc == 0) gc = CL_Display::get_current_window()->get_gc();

	// Draw the circles
	for(unsigned int i = 0; i < impl->contours.size(); i++)
	{
		unsigned int numcircles = impl->contours[i].sub_circles.size();
		for(unsigned int s = 0; s < numcircles; s++)
		{
			CL_Pointf center = impl->contours[i].sub_circles[s].position;
			float radius     = impl->contours[i].sub_circles[s].radius;
			float numsegments = 16;
			for(float e = 0; e < numsegments; e++)
			{
				float offx1 = cos(float(M_PI) * 2.0f * (e / numsegments))*radius;
				float offy1 = sin(float(M_PI) * 2.0f * (e / numsegments))*radius;
				float offx2 = cos(float(M_PI) * 2.0f * ((e+1) / numsegments))*radius;
				float offy2 = sin(float(M_PI) * 2.0f * ((e+1) / numsegments))*radius;
				CL_Pointf p1(x + center.x + offx1, y + center.y + offy1);
				CL_Pointf p2(x + center.x + offx2, y + center.y + offy2);
				gc->draw_line( p1.x+0.5f, p1.y+0.5f, p2.x+0.5f, p2.y+0.5f, color);
			}
		}
	}
}

void CL_CollisionOutline::draw_smallest_enclosing_disc(
	float x,
	float y,
	const CL_Color &color,
	CL_GraphicContext *gc)
{
	// Get a graphics context
	if (gc == 0) gc = CL_Display::get_current_window()->get_gc();

	// Draw the smallest enclosing disc
	CL_Pointf center = impl->minimum_enclosing_disc.position;
	float radius     = float(impl->minimum_enclosing_disc.radius);
	float numsegments = 24; // To make it visible if the outline has only one contour, and the contour only has one sub-circle
	for(float e = 0; e < numsegments; e++)
	{
		float offx1 = cos(float(M_PI) * 2.0f * (e / numsegments))*radius;
		float offy1 = sin(float(M_PI) * 2.0f * (e / numsegments))*radius;
		float offx2 = cos(float(M_PI) * 2.0f * ((e+1) / numsegments))*radius;
		float offy2 = sin(float(M_PI) * 2.0f * ((e+1) / numsegments))*radius;
		CL_Pointf p1(x + center.x + offx1, y + center.y + offy1);
		CL_Pointf p2(x + center.x + offx2, y + center.y + offy2);
		gc->draw_line( p1.x+0.5f, p1.y+0.5f, p2.x+0.5f, p2.y+0.5f, color);
	}
}

void CL_CollisionOutline::set_translation(float x, float y)
{
	impl->set_translation(x,y);
}

void CL_CollisionOutline::set_scale(float x, float y)
{
	impl->set_scale(x,y);
}

void CL_CollisionOutline::set_angle(float angle)
{
	impl->set_angle(angle);
}

void CL_CollisionOutline::rotate(float angle)
{
	impl->rotate(angle);
}

void CL_CollisionOutline::set_inside_test(bool value)
{
	impl->do_inside_test = value;
}

void CL_CollisionOutline::enable_collision_info(  bool points, bool normals, bool metadata, bool pen_depth)
{
	impl->collision_info_points = points;
	impl->collision_info_normals = normals;
	impl->collision_info_meta = metadata;
	impl->collision_info_pen_depth = pen_depth;
	impl->collision_info_collect = points || normals || metadata || pen_depth;
}

void CL_CollisionOutline::calculate_radius()
{
	impl->calculate_radius();
}

void CL_CollisionOutline::calculate_sub_circles(float radius_multiplier)
{
	impl->calculate_sub_circles(radius_multiplier);
}

void CL_CollisionOutline::calculate_smallest_enclosing_discs()
{
	impl->calculate_smallest_enclosing_discs();
}

void CL_CollisionOutline::calculate_convex_hulls()
{
	impl->calculate_convex_hulls();
}

void CL_CollisionOutline::save(const std::string &filename, CL_OutputSourceProvider *provider) const
{
	impl->save(filename, provider);
}

bool CL_CollisionOutline::collide(const CL_CollisionOutline &outline, bool remove_old_collision_info)
{
	return impl->collide(outline, remove_old_collision_info);
}

void CL_CollisionOutline::calculate_penetration_depth(std::vector<CL_CollidingContours> &collision_info)
{
	CL_CollisionOutline_Generic::calculate_penetration_depth(collision_info);
}

bool CL_CollisionOutline::point_inside(const CL_Pointf &point) const
{
	return impl->point_inside(point);
}

void CL_CollisionOutline::set_alignment( CL_Origin origin, float x, float y )
{
	CL_Origin old_origin = impl->translation_origin;

	// undo previous translation offset.
	impl->set_translation(-impl->translation_offset.x, -impl->translation_offset.y, true);
	
	// undo previous origin.
	if( origin != impl->translation_origin )
	{
		float fix_x = 0, fix_y = 0;

		float width = float(impl->width)/2;
		float height = float(impl->height)/2;

		switch( old_origin )
		{
		 case origin_top_left:
			fix_x = -width;
			fix_y = -height;
			break;
		 case origin_top_center:
			fix_x = 0;
			fix_y = -height;
			break;
		 case origin_top_right:
			fix_x = width;
			fix_y = -height;
			break;

		 case origin_center_left:
			fix_x = -width;
			fix_y = 0;
			break;
		 case origin_center:
			fix_x = 0;
			fix_y = 0;
			break;
		 case origin_center_right:
			fix_x = width;
			fix_y = 0;
			break;

		 case origin_bottom_left:
			fix_x = -width;
			fix_y = height;
			break;
		 case origin_bottom_center:
			fix_x = 0;
			fix_y = height;
			break;
		 case origin_bottom_right:
			fix_x = width;
			fix_y = height;
			break;
		}

		impl->set_translation(fix_x, fix_y, true); // offset outline points

		impl->rotation_hotspot.x += fix_x;
		impl->rotation_hotspot.y += fix_y;
	}

	
	impl->translation_origin = origin;
	impl->translation_offset.x = x;
	impl->translation_offset.y = y;
	
	impl->set_translation(x, y, true); // offset outline points

	// set new origin offset.
	if( origin != origin_center )
	{
		float fix_x = 0, fix_y = 0;

		float width = float(impl->width)/2;
		float height = float(impl->height)/2;

		switch( origin )
		{
		 case origin_top_left:
			fix_x = width;
			fix_y = height;
			break;
		 case origin_top_center:
			fix_x = 0;
			fix_y = height;
			break;
		 case origin_top_right:
			fix_x = -width;
			fix_y = height;
			break;

		 case origin_center_left:
			fix_x = width;
			fix_y = 0;
			break;
		 case origin_center:
			fix_x = 0;
			fix_y = 0;
			break;
		 case origin_center_right:
			fix_x = -width;
			fix_y = 0;
			break;

		 case origin_bottom_left:
			fix_x = width;
			fix_y = -height;
			break;
		 case origin_bottom_center:
			fix_x = 0;
			fix_y = -height;
			break;
		 case origin_bottom_right:
			fix_x = -width;
			fix_y = -height;
			break;
		}
		
		impl->set_translation(fix_x, fix_y, true); // offset outline points

		impl->rotation_hotspot.x += fix_x;
		impl->rotation_hotspot.y += fix_y;
	}

	impl->calculate_radius();
}

void CL_CollisionOutline::set_rotation_hotspot( CL_Origin origin, float x, float y )
{
	// undo translation origin
	CL_Origin trans_origin = impl->translation_origin;	
	set_alignment(origin_center);

	// undo the rotation of the current rotation hotspot
	float angle = impl->angle;
	impl->set_angle(0.0f);

	float fix_x = 0, fix_y = 0;

	// set new origin offset.
	if( origin != origin_center )
	{
		float width = float(impl->width)/2;
		float height = float(impl->height)/2;

		switch( origin )
		{
		 case origin_top_left:
			fix_x = -width;
			fix_y = -height;
			break;
		 case origin_top_center:
			fix_x = 0;
			fix_y = -height;
			break;
		 case origin_top_right:
			fix_x = width;
			fix_y = -height;
			break;

		 case origin_center_left:
			fix_x = -width;
			fix_y = 0;
			break;
		 case origin_center:
			fix_x = 0;
			fix_y = 0;
			break;
		 case origin_center_right:
			fix_x = width;
			fix_y = 0;
			break;

		 case origin_bottom_left:
			fix_x = -width;
			fix_y = height;
			break;
		 case origin_bottom_center:
			fix_x = 0;
			fix_y = height;
			break;
		 case origin_bottom_right:
			fix_x = width;
			fix_y = height;
			break;
		}
	}

	impl->rotation_origin = origin;

	impl->rotation_hotspot.x = fix_x + x;
	impl->rotation_hotspot.y = fix_y + y;

	// transform data using new rotation hotspot
	impl->set_angle(angle);

	set_alignment(trans_origin);

	impl->calculate_radius();
}



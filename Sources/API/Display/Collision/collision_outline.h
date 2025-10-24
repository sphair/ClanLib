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

//! clanDisplay="Collision"
//! header=display.h

#ifndef header_collision_outline
#define header_collision_outline

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

#include <string>
#include <vector>
#include "contour.h"
#include "outline_accuracy.h"
#include "../../Core/Resources/resource.h"
#include "../../Core/Math/origin.h"
#include "../../Core/Math/circle.h"
#include "../../GL/opengl_wrap.h"

class CL_CollisionOutline_Generic;
class CL_ResourceManager;
class CL_PixelBuffer;
class CL_OutputSourceProvider;
class CL_GraphicContext;
class CL_Color;


//: Collision point information structure.
//- !group=Display/Collision !
//- !header=display.h!
//- <p>Structure used for returning information about collisions</p>
struct CL_CollisionPoint
{
	//: Coordinates of the point where the contours intersected.
	CL_Pointf point;

	//: Normal vector at the point where the contours intersected.
	CL_Pointf normal;

	//: Every contour intersection has an entry and exit point. True, if this is an entry point.
	bool is_entry;

	//: Indexes of intersecting lines on the contours.
	int contour1_line_start, contour2_line_start, contour1_line_end, contour2_line_end;
};

//: Strurcture containing information about contours that collided.
//- !group=Display/Collision !
//- !header=display.h!/
//- <p>Structure used for returning information about collisions</p>
struct CL_CollidingContours
{
	const CL_Contour *contour1;
	const CL_Contour *contour2;
	bool inside;
	std::vector<CL_CollisionPoint> points;

	CL_Pointf penetration_normal;
	float penetration_depth;
	CL_Pointf contour1_deep_point;
	CL_Pointf contour2_deep_point;
	CL_CollidingContours(const CL_Contour *c1, const CL_Contour *c2, bool in=false) :
		contour1(c1),
		contour2(c2),
		inside(in),
		penetration_normal(0.0f,0.0f),
		penetration_depth(0.0f),
		contour1_deep_point(0.0f,0.0f),
		contour2_deep_point(0.0f,0.0f)
	{
		points.clear();
	}
};


//: Collision detection outline.
//- !group=Display/Collision !
//- !header=display.h!
//- <p>A collision outline is used in collision detection</p>
class CL_API_DISPLAY CL_CollisionOutline
{
//! Construction:
 public:
	//: Construct a collision outline.
	//param CL_PixelBuffer pbuf : Find alpha outline from a pixel buffer
	//param int alpha_limit : Alpha limit for pixels considered solid (collidable) 
	//param CL_OutlineAccuracy accuracy : Amount of optimization of the outline (default: medium)
	//param std::string filename : Load outline from a file. The file can be an image or a precompiled outline.
	CL_CollisionOutline();
	CL_CollisionOutline(const CL_CollisionOutline &other);
	CL_CollisionOutline( CL_PixelBuffer pbuf, int alpha_limit=128, CL_OutlineAccuracy accuracy=accuracy_medium );
	CL_CollisionOutline( const std::string &filename, int alpha_limit=128, CL_OutlineAccuracy accuracy=accuracy_medium, bool get_insides=true);
	CL_CollisionOutline( const std::string &resource_id, CL_ResourceManager *manager );
	CL_CollisionOutline(std::vector<CL_Contour> contours, int width, int height);

	~CL_CollisionOutline();

//! Attributes:
 public:
	//: Returns the contour used as an object bounding box. (an rotated rectangle)
	const CL_Contour &get_object_bounding_box() const;
	
	//: Returns the radius of the outline.
	//float get_radius() const;
	CL_Circlef get_minimum_enclosing_disc() const;
		
	//: Returns true if completely-inside test is used.
	bool get_inside_test() const;

	//: Returns the contours in the outline.
	std::vector<CL_Contour> &get_contours() const;

	//: Returns the position of the outline.
	CL_Pointf get_translation() const;

	//: Returns the scaling factor.
	CL_Pointf get_scale() const;

	//: Returns the rotation angle.
	float get_angle() const;
	
	//: Returns the width of the image this outline was created from.
	unsigned int get_width() const;

	//: Returns the height of the image this outline was created from.
	unsigned int get_height() const;

	//: Get the translation origin and hotspot of the outline.	
	void get_alignment( CL_Origin &origin, float &x, float &y ) const;

	//: Get the rotation hotspot of the outline.	
	void get_rotation_hotspot( CL_Origin &origin, float &x, float &y) const;

	//: Return the info about the collisions. (collision points, normals, pointers to contours, and indexes to lines that intersected)
	std::vector<CL_CollidingContours> &get_collision_info() const;

	//: Fetch the state of the collision testing variables.
	void get_collision_info_state(bool &points, bool &normals, bool &metadata, bool &pendepth) const;

	//: Resource owning this outline, if any.
	CL_Resource resource;
	
//! Operations:
 public:
	//: Assignment operator
	CL_CollisionOutline &operator=(const CL_CollisionOutline &other);

	//: Optimize the outline by removing redundant points.
	//param unsigned char check_distance : the distance of points compared on the outline
	//param float corner_angle : angle for a corner
	void optimize( unsigned char check_distance=3, float corner_angle=M_PI/5.0f );

	//: Draw outline on graphic context.
	//param x, y: Anchor position of where to render sprite. Actual rendering position depends on the anchor and the alignment mode.
	//param gc: Graphic context on which to render upon. If null, will use CL_Display's current graphic context.
	void draw(
		float x,
		float y,
		const CL_Color &color,
		CL_GraphicContext *gc = 0);
	
	//: Draw the subcircles surrounding the linesegments on graphic context.
	//param x, y: Anchor position of where to render cirles. Actual rendering position depends on the anchor and the alignment mode.
	//param gc: Graphic context on which to render upon. If null, will use CL_Display's current graphic context.
	void draw_sub_circles(
		float x,
		float y,
		const CL_Color &color,
		CL_GraphicContext *gc = 0);

	//: Draw the disc enclosing the entire outline.
	//param x, y: Anchor position of where to render the cirle. Actual rendering position depends on the anchor and the alignment mode.
	//param gc: Graphic context on which to render upon. If null, will use CL_Display's current graphic context.
	void draw_smallest_enclosing_disc(
		float x,
		float y,
		const CL_Color &color,
		CL_GraphicContext *gc);

	//: Set the translation hotspot of the outline.
	void set_alignment( CL_Origin origin, float x=0, float y=0 );

	//: Set the rotation hotspot of the outline.	
	void set_rotation_hotspot( CL_Origin origin, float x=0, float y=0 );

	//: Set the position of the outline.	
	void set_translation( float x, float y );
	
	//: Set the scale of the outline.	
	void set_scale( float x, float y );
	
	//: Set the angle (in degrees) of the outline.	
	void set_angle( float angle );
	
	//: Rotate the outline by angle (in degrees).
	void rotate( float angle );
	
	//: Set to true if completely-inside test should be done
	void set_inside_test( bool value );

	//: Enable collision info gathering.
	void enable_collision_info( bool points=true, bool normals=false, bool metadata=false, bool pen_depth=false);

	//: This will set the collision info of the outline
	void set_collision_info(const std::vector<CL_CollidingContours> &colinfo);

	//: This will empty the collision-info vector
	void clean_collision_info();

	//: (Re)calculate the radius of the outline.
	void calculate_radius();

	//: (Re)calculate the subcircle segmentation of the outline.
	void calculate_sub_circles(float radius_multiplier=3.5f);

	//: (Re)calculate the smallest circles enclosing every contour in the outline.
	void calculate_smallest_enclosing_discs();

	//: (Re)calculate the convex hull for every contour in the outline
	void calculate_convex_hulls();

	//: Write the outline to a file.
	//param std::string filename: Name of file.
	//param CL_OutputSourceProvider *provider: (Optional) OutputSourceProvider to use.
	void save(const std::string &filename, CL_OutputSourceProvider *provider=0) const;

	//: Returns true if outlines overlap
	//param CL_CollisionOutline outline : Outline to test against.
	bool collide( const CL_CollisionOutline &outline, bool remove_old_collision_info=true );

	//: Will calculate the penetration_depth and penetration_normal for all colliding contours.
	static void calculate_penetration_depth(std::vector<CL_CollidingContours> &collision_info);
	
	//: Returns true if a point is inside the outline.
	//param CL_Pointf &point: the point to test.
	bool point_inside( const CL_Pointf &point ) const;

//! Implementation:
 private:
	CL_CollisionOutline_Generic *impl;
};

#endif

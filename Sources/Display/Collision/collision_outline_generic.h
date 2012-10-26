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
**    Harry Storbacka
**    Magnus Norddahl
**    Emanuel Greisen
*/

#pragma once


#include "API/Display/Collision/collision_outline.h"
#include "API/Display/Collision/contour.h"
#include "API/Display/Collision/outline_circle.h"
#include "API/Display/Collision/outline_accuracy.h"
#include "API/Core/IOData/virtual_directory.h"

namespace clan
{

class OutlineProvider;

class CollisionOutline_Impl
{
/// \name Construction
/// \{

public:
	CollisionOutline_Impl();
	CollisionOutline_Impl( OutlineProvider *provider, OutlineAccuracy accuracy );

	~CollisionOutline_Impl();


/// \}
/// \name Attributes
/// \{

public:
	std::vector<Contour> contours;
	bool do_inside_test;

	int width, height;
	float angle;
	/** float radius;*/
	Circlef minimum_enclosing_disc;

	Pointf position;
	Pointf scale_factor;
	Pointf translation_offset;
	Origin translation_origin;
	Pointf rotation_hotspot;
	Origin rotation_origin;

	bool collision_info_points;
	bool collision_info_normals;
	bool collision_info_meta;
	bool collision_info_pen_depth;
	///< points || normals || meta (quick way of seeing if any info is collected)
	bool collision_info_collect;

	std::vector<CollidingContours> collision_info;


/// \}
/// \name Operations
/// \{

 public:
	void set_translation(float x, float y, bool offset_points=false);
	void set_scale(float sx, float sy);
	void set_angle(const Angle &angle);
	void rotate(const Angle &angle);

	void optimize(unsigned char check_distance, float corner_angle);
	void save(IODevice &file) const;

	bool collide( const CollisionOutline &outline, bool remove_old_collision_info);
	bool point_inside( const Pointf &point ) const;
	static bool point_inside_contour( const Pointf &point, const Contour &contour);
	bool contours_collide(const Contour &contour1, const Contour &contour2, bool do_subcirle_test=true);
	static void calculate_penetration_depth(std::vector<CollidingContours> &collision_info);

	void calculate_radius();
	void calculate_sub_circles(float radius_multiplier=3.5f);
	void calculate_smallest_enclosing_discs();
	void calculate_convex_hulls();

	inline bool line_bounding_box_overlap( const std::vector<Pointf> &rect1, const std::vector<Pointf> &rect2, int i, int j, int i2, int j2 ) const;

/// \}

/// \}
/// \name Implementation
/// \{
	OutlineProvider *provider;

/// \}
};

}

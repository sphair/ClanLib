/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Kenneth Gangstoe
**    Trigve Siver
**    Mark Page
*/

#include "precomp.h"
#include "collision_outline_impl.h"
#include "../CollisionAPI/collision_outline.h"
#include "../CollisionAPI/outline_accuracy.h"
#include "../CollisionAPI/outline_math.h"
#include <cfloat>
#include <iostream>

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

CollisionOutline_Impl::CollisionOutline_Impl()
:
	do_inside_test(false),
	width(0), height(0),
	angle(0),
	minimum_enclosing_disc(0.0f,0.0f,0.0f),
	position(0,0),
	scale_factor(1,1),
	translation_offset(0,0),
	translation_origin(origin_top_left),
	rotation_hotspot(0,0),
	rotation_origin(origin_center),
	collision_info_points(false),
	collision_info_normals(false),
	collision_info_meta(false),
	collision_info_pen_depth(false),
	collision_info_collect(false)
{
	return;
}

CollisionOutline_Impl::CollisionOutline_Impl(const std::vector<Contour> &new_contours, const Size &new_base_size, OutlineAccuracy accuracy )
:
	do_inside_test(false),
	width(0), height(0),
	angle(0),
	minimum_enclosing_disc(0.0f,0.0f,0.0f),
	position(0,0),
	scale_factor(1,1),
	translation_offset(0,0),
	translation_origin(origin_top_left),
	rotation_hotspot(0,0),
	rotation_origin(origin_center),
	collision_info_points(false),
	collision_info_normals(false),
	collision_info_meta(false),
	collision_info_pen_depth(false),
	collision_info_collect(false)
{
	contours = new_contours;
	width = new_base_size.width;
	height = new_base_size.height;
	//minimum_enclosing_disc set by calculate_radius()

	int check_distance = 3;

	switch( accuracy )
	{
	 case accuracy_high:
		optimize(check_distance, PI / 7.0f);
		break;
	 case accuracy_medium:
		optimize(check_distance, PI / 6.0f);
		break;
	 case accuracy_low:
		optimize(check_distance, PI / 5.0f);
		break;
	 case accuracy_poor:
		optimize(check_distance, PI / 4.0f);
		break;
	 case accuracy_raw:
		break;
	default:
		break;
	}
	
	calculate_radius();
	calculate_sub_circles();
}

CollisionOutline_Impl::~CollisionOutline_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CollisionOutline_Impl::set_translation(float x, float y, bool offset_points)
{
	Pointf old_position = position;

	if( !offset_points )
	{
		position.x = x;
		position.y = y;
	}

	Pointf translation;

	if( offset_points )
	{
		translation.x = x;
		translation.y = y;
	}
	else
		translation = (position - old_position);

	for (unsigned int outer_cnt = 0; outer_cnt < contours.size(); outer_cnt++)
	{
		Contour *contour_ptr = &contours[outer_cnt];
		std::vector<Pointf>::size_type point_size = contour_ptr->get_points().size();
		for (int inner_cnt = 0; inner_cnt < point_size; inner_cnt++)
		{
			contour_ptr->get_points()[inner_cnt] += translation;
		}
	}

	for (unsigned int outer_cnt = 0; outer_cnt < contours.size(); outer_cnt++)
	{
		Contour *contour_ptr = &contours[outer_cnt];
		std::vector<Pointf>::size_type sub_circles_size = contour_ptr->get_sub_circles().size();
		for (int inner_cnt = 0; inner_cnt < sub_circles_size; inner_cnt++)
		{
			contour_ptr->get_sub_circles()[inner_cnt].position += translation;
		}
	}

	minimum_enclosing_disc.position += translation;
}

void CollisionOutline_Impl::rotate(const Angle &add_angle)
{
	angle += add_angle.to_degrees();

	for (unsigned int outer_cnt = 0; outer_cnt < contours.size(); outer_cnt++)
	{
		Contour *contour_ptr = &contours[outer_cnt];
		std::vector<Pointf>::size_type point_size = contour_ptr->get_points().size();
		for (int inner_cnt = 0; inner_cnt < point_size; inner_cnt++)
		{
			contour_ptr->get_points()[inner_cnt].rotate(position+rotation_hotspot, add_angle);
		}
	}

	for (unsigned int outer_cnt = 0; outer_cnt < contours.size(); outer_cnt++)
	{
		Contour *contour_ptr = &contours[outer_cnt];
		std::vector<Pointf>::size_type sub_circles_size = contour_ptr->get_sub_circles().size();
		for (int inner_cnt = 0; inner_cnt < sub_circles_size; inner_cnt++)
		{
			contour_ptr->get_sub_circles()[inner_cnt].position.rotate(position+rotation_hotspot, add_angle);
		}
	}

	// Rotate our "radius" too
	minimum_enclosing_disc.position.rotate(position+rotation_hotspot, add_angle);
}

void CollisionOutline_Impl::set_angle(const Angle &angle)
{
	float rotate_angle = angle.to_degrees() - this->angle;
	this->angle = angle.to_degrees();

	for (unsigned int outer_cnt = 0; outer_cnt < contours.size(); outer_cnt++)
	{
		Contour *contour_ptr = &contours[outer_cnt];
		std::vector<Pointf>::size_type point_size = contour_ptr->get_points().size();
		for (int inner_cnt = 0; inner_cnt < point_size; inner_cnt++)
		{
			contour_ptr->get_points()[inner_cnt].rotate(position+rotation_hotspot, Angle(rotate_angle, angle_degrees));
		}
	}

	for (unsigned int outer_cnt = 0; outer_cnt < contours.size(); outer_cnt++)
	{
		Contour *contour_ptr = &contours[outer_cnt];
		std::vector<Pointf>::size_type sub_circles_size = contour_ptr->get_sub_circles().size();
		for (int inner_cnt = 0; inner_cnt < sub_circles_size; inner_cnt++)
		{
			contour_ptr->get_sub_circles()[inner_cnt].position.rotate(position+rotation_hotspot, Angle(rotate_angle, angle_degrees));
		}
	}

	// Rotate our "radius" too
	minimum_enclosing_disc.position.rotate(position+rotation_hotspot, Angle(rotate_angle,angle_degrees));
}

void CollisionOutline_Impl::set_scale(float new_scale_x, float new_scale_y)
{
	if( scale_factor.x == new_scale_x && scale_factor.y == new_scale_y )
		return;

	if (new_scale_x == 0 || new_scale_y == 0)
		return;

	float scale_x = new_scale_x / scale_factor.x;
	float scale_y = new_scale_y / scale_factor.y;
	
	for (unsigned int outer_cnt = 0; outer_cnt < contours.size(); outer_cnt++)
	{
		Contour *contour_ptr = &contours[outer_cnt];
		std::vector<Pointf>::size_type point_size = contour_ptr->get_points().size();
		for (int inner_cnt = 0; inner_cnt < point_size; inner_cnt++)
		{
			contour_ptr->get_points()[inner_cnt].x = position.x + ((contour_ptr->get_points()[inner_cnt].x-position.x)*scale_x);
			contour_ptr->get_points()[inner_cnt].y = position.y + ((contour_ptr->get_points()[inner_cnt].y-position.y)*scale_y);
		}
	}
	
	// we can skip this recalculation (if its a unit-scale)
	if(new_scale_x == new_scale_y)
	{
		minimum_enclosing_disc.position.x = position.x + ((minimum_enclosing_disc.position.x-position.x)*scale_x);
		minimum_enclosing_disc.position.y = position.y + ((minimum_enclosing_disc.position.y-position.y)*scale_y);
		minimum_enclosing_disc.radius *= (new_scale_x/scale_factor.x);

		// TODO: we should be able to scale these too (if 
		calculate_sub_circles();
	}
	else
	{
		calculate_sub_circles();
		calculate_radius();
	}

	scale_factor.x = new_scale_x;
	scale_factor.y = new_scale_y;
}

void CollisionOutline_Impl::calculate_radius()
{
	std::vector<Pointf> allpoints;
	std::vector<Contour>::iterator it;
	for( it = contours.begin(); it != contours.end(); ++it )
	{
		for(std::vector<Pointf>::const_iterator pit = (*it).get_points().begin(); pit != (*it).get_points().end(); pit++)
		{
			allpoints.push_back(*pit);
		}
	}
	minimum_enclosing_disc = PointSetMath::minimum_enclosing_disc(allpoints);
	minimum_enclosing_disc.radius += 0.01f; // Just to make sure.
}

void CollisionOutline_Impl::calculate_sub_circles(float radius_multiplier)
{
	/*
	 * The new way of doing it, is as follows:
	 * [Outer loop: for all contours]
	 *    - Estimate a subcircle-radius: Average of line-segment-lengths * 3.5 (maybe this should be a parameter ?)
	 *   [Loop: for all points]
	 *    - Create a subcircle for the next two points (this gives a center and a radius)
	 *      [Inner loop: as long as we have more points]
	 *        - Add the next point: maybe expand the radius, and change the center
	 *        - [if: (radius > avg-radius && more than two more points exist)]
	 *          - Break inner loop !
	 *    - Add the subcircle to the list
	**/
	std::vector<Contour>::iterator it;
	for( it = contours.begin(); it != contours.end(); ++it )
	{
		(*it).get_sub_circles().clear();

		const std::vector<Pointf> &points = (*it).get_points();

		// Test that we have at least 2 points
		if(points.size() < 2)
			throw Exception("Error: contour has less than 2 points");

		// sub_radius = average of line lenghts in the contour * 3.5
		float sub_radius = 0.0f;
		for( unsigned int j=0; j < points.size(); ++j )
		{
			int j2 = (j+1) % points.size();
			sub_radius += (points[j].distance(points[j2]));
		}
		sub_radius /= points.size();
		sub_radius *= radius_multiplier;

		unsigned int i=0;
		while(i < points.size())
		{
			// Create a circle from the first two points
			OutlineCircle circle;
			circle.start = i;    // i denotes the index of the beginning
			circle.end   = i+1;  // (i+1) denotes the index of the end: and it loops
			OutlineMath::minimum_enclosing_sub_circle(circle,points,sub_radius);
			circle.radius += 0.01f; // Just to make sure.
			
			// Add the circle
			(*it).get_sub_circles().push_back(circle);

			// update i for next circle
			i = circle.end;
		}
	}
}

void CollisionOutline_Impl::calculate_smallest_enclosing_discs()
{	
	std::vector<Contour>::iterator it;
	for( it = contours.begin(); it != contours.end(); ++it )
	{
		(*it).get_sub_circles().clear();
		Circlef tmpdisc = PointSetMath::minimum_enclosing_disc((*it).get_points());
		OutlineCircle mindisc;
		mindisc.position = tmpdisc.position;
		mindisc.radius = tmpdisc.radius;
		mindisc.radius += 0.01f; // Just to make sure.
		mindisc.start = 0;
		mindisc.end   = (*it).get_points().size(); // This is actualy the first point, but this is how to start and end the same place
		(*it).get_sub_circles().push_back(mindisc);
	}
}

void CollisionOutline_Impl::calculate_convex_hulls()
{
	std::vector<Contour>::iterator it;
	for( it = contours.begin(); it != contours.end(); ++it )
	{
		(*it).get_points() = PointSetMath::convex_hull_from_polygon((*it).get_points());
	}
	// We use smallest enclosing discs,
	// since that gives great results with a convex shapes.
	calculate_smallest_enclosing_discs();
}

void CollisionOutline_Impl::optimize(unsigned char check_distance, float corner_angle)
{
	unsigned char orig_check_distance = check_distance;

	std::vector<Contour>::iterator it;
	for( it = contours.begin(); it != contours.end(); ++it )
	{
		check_distance = orig_check_distance;

		std::vector<Pointf> &points = (*it).get_points();

		if( points.empty() ) continue;

		std::vector<Pointf> optimized;

		optimized.push_back(points.front());

		if( static_cast<int>(points.size()) < check_distance )
			check_distance = 1;

		for( unsigned int i=0; i < points.size()-check_distance; ++i )
		{
			int A_index = i-check_distance;
			int B_index = i+check_distance;

			if( A_index < 0 )
			{
				if( points.front() == points.back() )
					A_index += points.size();
				else
					A_index = 0;
			}
			if( B_index > (int)points.size() )
			{
				if( points.front() == points.back() )
					B_index -= points.size();
				else
					B_index = (int)points.size();
			}

			Pointf &A = optimized.back();
			Pointf &B = points[i];
			Pointf &C = points[B_index];

			Vec4f AB(B.x-A.x, B.y-A.y, 0, 0);	// TODO: Does this need to be Vec4f ?
			Vec4f BC(C.x-B.x, C.y-B.y, 0, 0);

			if( check_distance != 1 && AB.length3() < 2 )
				continue;

			float angle = AB.angle3(BC).to_radians(); // If things stopped working, it's because this should be degrees, not radians. Added when introducing Angle. -- HS 12.12.2008
			
			if( angle > corner_angle )
			{
				optimized.push_back(points[i]);
			}
		}

		points = optimized;
	}
}

void CollisionOutline_Impl::save(IODevice &output_source) const
{
/*	fileformat:

	uint32  type        // file type identifier	
	uint8   version     // file version	
	uint32  width       // width of the outline
	uint32  height      // height of the outline
	float32 x-pos       // of enclosing disc
	float32 y-pos       // of enclosing disc
	float32 radius      // of enclosing disc

	uint32 num_contours
		uint32 num_points contour 1
			float32 px1
			float32 py1
			float32 px2
			float32 py2
			... contour 1 data ...
		uint32 num_points contour 2
			... contour 2 data ...
		uint32 num_points contour N
			... contour N data ...
*/

	// file type identifier
	output_source.write_uint32( 0x16082004 );

	// fileformat version
	output_source.write_uint8(1);

	// width
	output_source.write_int32(width);

	// height
	output_source.write_int32(height);

	// x-pos of enclosing disc
	output_source.write_float((float)minimum_enclosing_disc.position.x);
	
	// y-pos of enclosing disc
	output_source.write_float((float)minimum_enclosing_disc.position.y);

	// radius of enclosing disc
	output_source.write_float((float)minimum_enclosing_disc.radius);
	
	// number of contours
	output_source.write_uint32(contours.size());
	
	std::vector<Contour>::const_iterator it_cont;
	for( it_cont = contours.begin(); it_cont != contours.end(); ++it_cont )
	{
		// number of points in contours
		output_source.write_uint32((*it_cont).get_points().size());
		
		std::vector<Pointf>::const_iterator it;
		for( it = (*it_cont).get_points().begin(); it != (*it_cont).get_points().end(); ++it )
		{
			// x,y of points
			output_source.write_float((float)(*it).x);
			output_source.write_float((float)(*it).y);
		}
	}
}

bool CollisionOutline_Impl::collide( const CollisionOutline &outline, bool remove_old_collision_info)
{
	if( collision_info_collect && remove_old_collision_info )
	{
		collision_info.clear();
	}

	// bounding circle test.
	float dist = minimum_enclosing_disc.position.distance(outline.get_minimum_enclosing_disc().position);
	
	if( dist > (minimum_enclosing_disc.radius + outline.get_minimum_enclosing_disc().radius ))
		return false;

	bool any_collisions = false;
	// collision sub circle test
	std::vector<Contour>::const_iterator it_contours, it_contours2;
	for( it_contours = contours.begin(); it_contours != contours.end(); ++it_contours )
	{
		for( it_contours2 = outline.get_contours().begin();
			 it_contours2 != outline.get_contours().end();
			 ++it_contours2 )
		{
			if( contours_collide( (*it_contours), (*it_contours2) ) )
			{
				if( collision_info_collect == false ) 
					return true; // don't return info about all line intersections
				any_collisions = true;
			}
			else if( do_inside_test || outline.get_inside_test() )
			{
				if( point_inside_contour((*it_contours).get_points()[0], (*it_contours2)))
				{
					if( collision_info_collect )
					{
						// Add this info to the
						collision_info.push_back(CollidingContours(&(*it_contours), &(*it_contours2), true));
					}
					else
					{
						return true; // don't return info about the collision
					}
					any_collisions = true;
				}
				if(point_inside_contour((*it_contours2).get_points()[0], (*it_contours)) )
				{
					if( collision_info_collect )
					{
						// Add this info to the
						collision_info.push_back(CollidingContours(&(*it_contours2), &(*it_contours), true));
					}
					else
					{
						return true; // don't return info about the collision
					}
					any_collisions = true;
				}
			}
		}
	}

	// Should we calculate the penetration depth
	if( !collision_info.empty() && collision_info_pen_depth && remove_old_collision_info)
	{
		// We only do this, if we have any info and if is new collision-info.
		calculate_penetration_depth(collision_info);
	}
	
	return any_collisions;
}


bool CollisionOutline_Impl::point_inside( const Pointf &point ) const
{
	float dist = minimum_enclosing_disc.position.distance(point);
	
	if( dist > minimum_enclosing_disc.radius)
		return false;

	std::vector<Contour>::const_iterator it;	
	for( it = contours.begin(); it != contours.end(); ++it )
	{
		if( point_inside_contour(point, (*it)) )
		{
			return true;
		}
	}
	
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CollisionOutline_Impl Implementation:

bool CollisionOutline_Impl::point_inside_contour( const Pointf &point, const Contour &contour )
{
	// In case the contour is inside-out (the inside of a hollow polygon) it makes no sense to do this test.
	if(contour.is_inside_contour())
		return false;
	
	LineSegment2f lineX( Pointf(point.x, point.y+0.000f), Pointf(point.x+99999.0f, point.y+0.000f) );

	// collide the line with the outline.
	int num_intersections_x = 0;

	const std::vector<Pointf> &points = contour.get_points();

	std::vector<OutlineCircle>::const_iterator it;
	for( it = contour.get_sub_circles().begin();
		 it != contour.get_sub_circles().end();
		 ++it )
	{
		const OutlineCircle &circle = (*it);

		float dist = fabs(circle.position.y - point.y);

		if( dist <= circle.radius )
		{
			// test each line segment inside the circle
		
			for( unsigned int i=circle.start; i != circle.end; ++i )
			{
				LineSegment2f line2(points[ i    % points.size()], points[(i+1) % points.size()]);

				if( lineX.intersects(line2, false) )
				{
					num_intersections_x++;
				}
			}
		}
	}

	if( num_intersections_x % 2 )
	{
		return true;
	}
	
	return false;
}

inline bool CollisionOutline_Impl::line_bounding_box_overlap( const std::vector<Pointf> &rect1, const std::vector<Pointf> &rect2, int i, int j, int i2, int j2 ) const
{
	float left   = min(rect1[i].x, rect1[i2].x);
	float right  = max(rect1[i].x, rect1[i2].x);
	float top    = min(rect1[i].y, rect1[i2].y);
	float bottom = max(rect1[i].y, rect1[i2].y);

	float r_left   = min(rect2[j].x, rect2[j2].x);
	float r_right  = max(rect2[j].x, rect2[j2].x);
	float r_top    = min(rect2[j].y, rect2[j2].y);
	float r_bottom = max(rect2[j].y, rect2[j2].y);

	return (r_left <= right && r_right >= left && r_top <= bottom && r_bottom >= top);
}

bool CollisionOutline_Impl::contours_collide(const Contour &contour1, const Contour &contour2, bool do_subcirle_test)
{
	CollidingContours metadata(&contour1, &contour2);
	
	std::vector<OutlineCircle>::const_iterator it_oc1, it_oc2;
	for( it_oc1 = contour1.get_sub_circles().begin(); it_oc1 != contour1.get_sub_circles().end(); ++it_oc1 )
	{
		for( it_oc2 = contour2.get_sub_circles().begin(); it_oc2 != contour2.get_sub_circles().end(); ++it_oc2 )
		{
			if( do_subcirle_test ? (*it_oc1).collide(*it_oc2) : true ) // outline circles collide
			{
				// test each line segment inside the colliding circles

				const std::vector<Pointf> &points1 = contour1.get_points();
				const std::vector<Pointf> &points2 = contour2.get_points();
				
				int num_points1 = points1.size();
				int num_points2 = points2.size();
				
				for( unsigned int counter_i=(*it_oc1).start; counter_i != (*it_oc1).end; ++counter_i )
				{
					int i  = counter_i % num_points1;
					int i2 = (counter_i+1) % num_points1;
					
					for( unsigned int counter_j=(*it_oc2).start; counter_j != (*it_oc2).end; ++counter_j )
					{
						int j  = counter_j % num_points2;
						int j2 = (counter_j+1) % num_points2;
						
						if( line_bounding_box_overlap(points1, points2, i, j, i2, j2) )
						{
							LineSegment2f line1(points1[i], points1[i2]);
							LineSegment2f line2(points2[j], points2[j2]);
							Pointf dest_intercept;

							bool did_intersect;
							dest_intercept = line1.get_intersection( line2, did_intersect );
							if( did_intersect )
							{
								if( collision_info_collect )
								{
									CollisionPoint collisionpoint;
									
									if ( collision_info_points )
									{
										collisionpoint.point = dest_intercept;
									}
									
									if( collision_info_normals )
									{
										collisionpoint.normal = line2.normal();
									}
									
									if( collision_info_meta )
									{
										collisionpoint.contour1_line_start = i;
										collisionpoint.contour1_line_end   = i2;
										collisionpoint.contour2_line_start = j;
										collisionpoint.contour2_line_end   = j2;
										// Found by the dot-product of line1 and the perpendicular of line2:
										{
											Pointf line1(points1[i2].x - points1[i].x, points1[i2].y - points1[i].y);
											Pointf line2(-(points2[j2].y - points2[j].y), points2[j2].x - points2[j].x);
											collisionpoint.is_entry = (line1.x * line2.x + line1.y * line2.y) < 0.0;
										}
									}
									metadata.points.push_back(collisionpoint);
								}
								else
									return true;
							}
						}
					}
				}
			}
		}
	}

	if( collision_info_collect && metadata.points.size() > 0)
	{
		// Add this info
		collision_info.push_back(metadata);
		return true;
	}

	return false;
}

void CollisionOutline_Impl::calculate_penetration_depth( std::vector< CollidingContours > & collision_info )
{
	// Figure out the pen-depth
	for(std::vector<CollidingContours>::iterator it = collision_info.begin(); it != collision_info.end(); ++it)
	{
		CollidingContours &cc = (*it);
		if(cc.points.size() % 2 != 0)
		{
			std::cout << "ERROR: we have an uneven number of collisionpoints: " << cc.points.size() << "\n";
			for(std::vector<CollisionPoint>::iterator pit = cc.points.begin(); pit != cc.points.end(); ++pit)
			{
				CollisionPoint &p1 = (*pit);
				std::cout << "\tLineSegment1:"
					<< "(" << cc.contour1->get_points()[p1.contour1_line_start].x
					<< "," << cc.contour1->get_points()[p1.contour1_line_start].y << ") - "
					<< "(" << cc.contour1->get_points()[p1.contour1_line_end].x
					<< "," << cc.contour1->get_points()[p1.contour1_line_end].y << ")\n";
				std::cout << "\tLineSegment2:"
					<< "(" << cc.contour2->get_points()[p1.contour2_line_start].x
					<< "," << cc.contour2->get_points()[p1.contour2_line_start].y << ") - "
					<< "(" << cc.contour2->get_points()[p1.contour2_line_end].x
					<< "," << cc.contour2->get_points()[p1.contour2_line_end].y << ")\n";
				std::cout << "\tColPoint:  ("<<p1.point.x<<","<<p1.point.y<<")\n";
				std::cout << "\tColNormal: ("<<p1.normal.x<<","<<p1.normal.y<<")\n";
				std::cout << "\tis_entry: " << p1.is_entry <<"\n";
				std::cout << "\tcontour1_line_start: "<<p1.contour1_line_start<<", "
					<<"contour1_line_end: "<<p1.contour1_line_end<<",\n"
					<<"\tcontour2_line_start: "<<p1.contour2_line_start<<", "
					<<"contour2_line_end: "<<p1.contour2_line_end <<"\n";
			}
			std::cout << "RORRE\n";
			continue;
		}
		// First calculate one common normal for the whole thing
		// FIXME: oposing normals might generate (0,0) as normal, and that can not be right.
		Vec4f normal;
		unsigned int cp;
		for(cp = 0; cp < cc.points.size(); cp+=2)
		{
			std::vector<Pointf> c1points;
			std::vector<Pointf> c2points;
			int firstpoint = cp;
			if(!cc.points[firstpoint].is_entry)
				firstpoint++;
			CollisionPoint p1 = cc.points[firstpoint     % cc.points.size()];
			CollisionPoint p2 = cc.points[(firstpoint+1) % cc.points.size()];

			normal.x += -(p1.point - p2.point).y;
			normal.y += (p1.point - p2.point).x;
		}
		normal.normalize3();
		cc.penetration_normal = Pointf(normal.x, normal.y);

		// Now look at each and every overlapping region
		cc.penetration_depth = 0.0;
		for(unsigned int cp2 = 0; cp2 < cc.points.size(); cp2+=2)
		{
			std::vector<Pointf> c1points;
			std::vector<Pointf> c2points;
			int firstpoint = cp2;
			if(!cc.points[firstpoint].is_entry)
				firstpoint++;

			CollisionPoint p1 = cc.points[firstpoint     % cc.points.size()];
			CollisionPoint p2 = cc.points[(firstpoint+1) % cc.points.size()];

			// Get points inside on c1
			c1points.push_back(p2.point - p1.point);
			c1points.push_back(p1.point - p1.point);
			for(int p4 = p1.contour1_line_end; p4 != p2.contour1_line_end; p4 = ((p4+1) % cc.contour1->get_points().size()))
			{
				c1points.push_back(cc.contour1->get_points()[p4] - p1.point);
				//c1points.push_back(cc.contour1->points[p]);
			}
			// Get points inside on c2
			c2points.push_back(p2.point - p1.point);
			c2points.push_back(p1.point - p1.point);
			for(int p6 = p2.contour2_line_end; p6 != p1.contour2_line_end; p6 = ((p6+1) % cc.contour2->get_points().size()))
			{
				c2points.push_back(cc.contour2->get_points()[p6] - p1.point);
				//c2points.push_back(cc.contour2->points[p]);
			}
		
			// Calculate the penetration-depth of this overlap
			float c1maxdepth = FLT_MAX;
			float c2maxdepth = FLT_MIN;
			for(unsigned int p5 = 0; p5 < c1points.size(); p5++)
			{
				// The dotproduct is the projection onto an other vector
				float newdepth = c1points[p5].x * normal.x + c1points[p5].y * normal.y;
				if(newdepth < c1maxdepth)
				{
					cc.contour1_deep_point = c1points[p5] + p1.point;
					c1maxdepth = newdepth;
				}
			}
			for(unsigned int p = 0; p < c2points.size(); p++)
			{
				// The dotproduct is the projection onto an other vector
				float newdepth = c2points[p].x * normal.x + c2points[p].y * normal.y;
				if(newdepth > c2maxdepth)
				{
					cc.contour2_deep_point = c2points[p] + p1.point;
					c2maxdepth = newdepth;
				}
			}
			cc.penetration_depth = max(cc.penetration_depth, c2maxdepth - c1maxdepth);
		}
		
		//NONO: maxpendepth = std::min(maxpendepth, 40.0f);
	}
}


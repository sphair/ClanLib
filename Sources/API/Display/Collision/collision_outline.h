/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
*/

/// \addtogroup clanDisplay_Collision clanDisplay Collision
/// \{

#pragma once

#include "../api_display.h"
#include <vector>
#include "contour.h"
#include "outline_accuracy.h"
#include "../Render/graphic_context.h"
#include "../../Core/Resources/resource.h"
#include "../../Core/Math/cl_math.h"
#include "../../Core/Math/origin.h"
#include "../../Core/Math/circle.h"
#include "../../Core/IOData/virtual_directory.h"

class CL_CollisionOutline_Generic;
class CL_ResourceManager;
class CL_OutputSourceProvider;
class CL_GraphicContext;
class CL_Color;

/// \brief Collision point information structure.
///
///  <p>Structure used for returning information about collisions</p> 
/// \xmlonly !group=Display/Collision! !header=display.h! \endxmlonly
struct CL_CollisionPoint
{
	/// \brief Coordinates of the point where the contours intersected.
	CL_Pointf point;

	/// \brief Normal vector at the point where the contours intersected.
	CL_Pointf normal;

	/// \brief Every contour intersection has an entry and exit point. True, if this is an entry point.
	bool is_entry;

	/// \brief Indexes of intersecting lines on the contours.
	int contour1_line_start, contour2_line_start, contour1_line_end, contour2_line_end;
};

/// \brief Strurcture containing information about contours that collided.
///
/// <p>Structure used for returning information about collisions</p> 
/// \xmlonly !group=Display/Collision! !header=display.h! \endxmlonly
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

	/// \brief Constructs a CollidingContours
	///
	/// \param c1 = Contour
	/// \param c2 = Contour
	/// \param in = bool
	CL_CollidingContours(const CL_Contour *c1, const CL_Contour *c2, bool in=false) :
		contour1(c1),
		contour2(c2),
		inside(in),
		penetration_normal(0.0, 0.0),
		penetration_depth(0.0),
		contour1_deep_point(0.0, 0.0),
		contour2_deep_point(0.0, 0.0)
	{
		points.clear();
	}
};

/// \brief Collision detection outline.
///
/// <p>A collision outline is used in collision detection</p> 
/// \xmlonly !group=Display/Collision! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_CollisionOutline
{
/// \name Construction
/// \{
public:
	/// \brief Construct a collision outline.
	CL_CollisionOutline();

	/// \brief Construct a collision outline.
	///
	/// \param pbuf = Find alpha outline from a pixel buffer
	/// \param alpha_limit = Alpha limit for pixels considered solid (collidable)
	/// \param accuracy = Amount of optimization of the outline (default medium)
	CL_CollisionOutline(const CL_PixelBuffer &pbuf, int alpha_limit=128, CL_OutlineAccuracy accuracy=accuracy_medium );

	/// \brief Construct a collision outline.
	///
	/// \param fullname = Full name
	/// \param alpha_limit = value
	/// \param accuracy = Outline Accuracy
	/// \param get_insides = bool
	CL_CollisionOutline(const CL_StringRef &fullname, int alpha_limit=128, CL_OutlineAccuracy accuracy=accuracy_medium, bool get_insides=true);

	/// \brief Constructs a CollisionOutline
	///
	/// \param file = IODevice
	/// \param file_extension = String
	/// \param alpha_limit = value
	/// \param accuracy = Outline Accuracy
	/// \param get_insides = bool
	CL_CollisionOutline(CL_IODevice &file, const CL_String &file_extension, int alpha_limit=128, CL_OutlineAccuracy accuracy=accuracy_medium, bool get_insides=true);

	/// \brief Constructs a CollisionOutline
	///
	/// \param filename = String Ref
	/// \param directory = Virtual Directory
	/// \param alpha_limit = value
	/// \param accuracy = Outline Accuracy
	/// \param get_insides = bool
	CL_CollisionOutline(const CL_StringRef &filename, const CL_VirtualDirectory &directory, int alpha_limit=128, CL_OutlineAccuracy accuracy=accuracy_medium, bool get_insides=true);

	/// \brief Construct a collision outline.
	///
	/// \param resource_id = The resource id
	/// \param manager = The resource manager
	CL_CollisionOutline(const CL_StringRef &resource_id, CL_ResourceManager *manager );

	/// \brief Construct a collision outline.
	///
	/// \param contours = The contours
	/// \param width = The width
	/// \param height = The height
	CL_CollisionOutline(std::vector<CL_Contour> contours, int width, int height);

	~CL_CollisionOutline();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the contour used as an object bounding box. (an rotated rectangle)
	const CL_Contour &get_object_bounding_box() const;

	/// \brief Returns the radius of the outline.
	/** float get_radius() const;*/
	CL_Circlef get_minimum_enclosing_disc() const;

	/// \brief Returns true if completely-inside test is used.
	bool get_inside_test() const;

	/// \brief Returns the contours in the outline.
	std::vector<CL_Contour> &get_contours();
	const std::vector<CL_Contour> &get_contours() const;

	/// \brief Returns the position of the outline.
	CL_Pointf get_translation() const;

	/// \brief Returns the scaling factor.
	CL_Pointf get_scale() const;

	/// \brief Returns the rotation angle.
	float get_angle() const;

	/// \brief Returns the width of the image this outline was created from.
	unsigned int get_width() const;

	/// \brief Returns the height of the image this outline was created from.
	unsigned int get_height() const;

	/// \brief Get the translation origin and hotspot of the outline.
	void get_alignment( CL_Origin &origin, float &x, float &y ) const;

	/// \brief Get the rotation hotspot of the outline.
	void get_rotation_hotspot( CL_Origin &origin, float &x, float &y) const;

	/// \brief Return the info about the collisions. (collision points, normals, pointers to contours, and indexes to lines that intersected)
	const std::vector<CL_CollidingContours> &get_collision_info() const;

	/// \brief Fetch the state of the collision testing variables.
	void get_collision_info_state(bool &points, bool &normals, bool &metadata, bool &pendepth) const;

	/// \brief Resource owning this outline, if any.
	CL_Resource resource;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Load the outline from file replacing the current data.
	///
	/// Loads precompiled outlines
	void load(const CL_StringRef &fullname);

	/// \brief Load
	///
	/// \param filename = The filename
	/// \param directory = Virtual Directory
	void load(const CL_StringRef &filename, const CL_VirtualDirectory &directory);

	/// \brief Load
	///
	/// \param file = The file
	void load(CL_IODevice &file);

	/// \brief Copy
	CL_CollisionOutline &copy(const CL_CollisionOutline &other);

	/// \brief Optimize the outline by removing redundant points.
	///
	/// \param check_distance = the distance of points compared on the outline
	/// \param corner_angle = angle for a corner
	void optimize( unsigned char check_distance=3, float corner_angle = CL_PI / 5.0 );

	/// \brief Draw outline on graphic context.
	///
	/// Actual rendering position depends on the anchor and the alignment mode.
	///
	/// \param x = Anchor position of where to render sprite. 
	/// \param y = Anchor y position
	/// \param color = The color
	/// \param gc = Graphic context on which to render upon.
	void draw(
		float x,
		float y,
		const CL_Colorf &color,
		CL_GraphicContext &gc);

	/// \brief Draw the subcircles surrounding the linesegments on graphic context.
	///
	///  Actual rendering position depends on the anchor and the alignment mode.
	///
	/// \param x = Anchor position of where to render cirles.
	/// \param y = Anchor y position
	/// \param color = The color
	/// \param gc = Graphic context on which to render upon.
	void draw_sub_circles(
		float x,
		float y,
		const CL_Colorf &color,
		CL_GraphicContext &gc);

	/// \brief Draw the disc enclosing the entire outline.
	///
	/// Actual rendering position depends on the anchor and the alignment mode.
	///
	/// \param x = Anchor position of where to render the cirle.
	/// \param y = Anchor y position
	/// \param color = The color
	/// \param gc = Graphic context on which to render upon.
	void draw_smallest_enclosing_disc(
		float x,
		float y,
		const CL_Colorf &color,
		CL_GraphicContext &gc);

	/// \brief Set the translation hotspot of the outline.
	void set_alignment( CL_Origin origin, float x=0, float y=0 );

	/// \brief Set the rotation hotspot of the outline.
	void set_rotation_hotspot( CL_Origin origin, float x=0, float y=0 );

	/// \brief Set the position of the outline.
	void set_translation( float x, float y );

	/// \brief Set the scale of the outline.
	void set_scale( float x, float y );

	/// \brief Set the angle (in degrees) of the outline.
	void set_angle(const CL_Angle &angle);

	/// \brief Rotate the outline by angle (in degrees).
	void rotate(const CL_Angle &angle);

	/// \brief Set to true if completely-inside test should be done
	void set_inside_test( bool value );

	/// \brief Enable collision info gathering.
	void enable_collision_info( bool points=true, bool normals=false, bool metadata=false, bool pen_depth=false);

	/// \brief This will set the collision info of the outline
	void set_collision_info(const std::vector<CL_CollidingContours> &colinfo);

	/// \brief This will empty the collision-info vector
	void clean_collision_info();

	/// \brief (Re)calculate the radius of the outline.
	void calculate_radius();

	/// \brief (Re)calculate the subcircle segmentation of the outline.
	void calculate_sub_circles(float radius_multiplier=3.5);

	/// \brief (Re)calculate the smallest circles enclosing every contour in the outline.
	void calculate_smallest_enclosing_discs();

	/// \brief (Re)calculate the convex hull for every contour in the outline
	void calculate_convex_hulls();

	/// \brief Write the outline to a file.
	///
	/// \param fullname = Name of file.
	void save(const CL_StringRef &fullname) const;

	/// \brief Save
	///
	/// \param filename = The filename
	/// \param directory = Virtual Directory
	void save(const CL_StringRef &filename, CL_VirtualDirectory &directory) const;

	/// \brief Save
	///
	/// \param file = The file
	void save(CL_IODevice &file) const;

	/// \brief Returns true if outlines overlap
	///
	/// \param outline = Outline to test against.
	/// \param remove_old_collision_info = set to true to remove old collision info
	bool collide( const CL_CollisionOutline &outline, bool remove_old_collision_info=true );

	/// \brief Will calculate the penetration_depth and penetration_normal for all colliding contours.
	static void calculate_penetration_depth(std::vector<CL_CollidingContours> &collision_info);

	/// \brief Returns true if a point is inside the outline.
	///
	/// \param point = the point to test.
	bool point_inside( const CL_Pointf &point ) const;

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_CollisionOutline_Generic> impl;
/// \}
};

/// \}

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
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../Image/pixel_buffer.h"

class CL_PolygonRasterizer_Impl;

/// \brief Polygon culling modes.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_CullMode
{
	cl_cull_front,
	cl_cull_back,
	cl_cull_front_and_back
};

/// \brief Polygon filling modes.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_FillMode
{
	cl_fill_point,
	cl_fill_line,
	cl_fill_polygon
};

/// \brief Front face modes.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_FaceSide
{
	cl_face_side_clockwise,
	cl_face_side_counter_clockwise
};

/// \brief Polygon rasterization description.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_PolygonRasterizer
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a polygon rasterizer description.
	CL_PolygonRasterizer();

	virtual ~CL_PolygonRasterizer();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if the polygons are anti-aliased.
	bool is_antialiased() const;

	/// \brief Returns true if cull testing is enabled.
	bool is_culled() const;

	/// \brief Returns true if points are offset.
	bool is_point_offset() const;

	/// \brief Returns true if lines are offset.
	bool is_line_offset() const;

	/// \brief Returns true if polygons are offset.
	bool is_polygon_offset() const;

	/// \brief Returns the polygon cull clipping mode.
	CL_CullMode get_face_cull_mode() const;

	/// \brief Returns the filling mode for front-facing polygons.
	CL_FillMode get_face_fill_mode_front() const;

	/// \brief Returns the filling mode for back-facing polygons.
	CL_FillMode get_face_fill_mode_back() const;

	/// \brief Returns the offsetting factor.
	float get_offset_factor() const;

	/// \brief Returns the offsetting units.
	float get_offset_units() const;

	/// \brief Returns the side considered the front of a face.
	CL_FaceSide get_front_face() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Enables/disables anti-aliasing.
	void set_antialiased(bool value);

	/// \brief Enables/disables polygon cull clipping.
	void set_culled(bool value);

	/// \brief Enables/disables point offsetting.
	void set_point_offset(bool value);

	/// \brief Enables/disables line offsetting.
	void set_line_offset(bool value);

	/// \brief Enables/disables polygon offsetting.
	void set_polygon_offset(bool value);

	/// \brief Sets the polygon cull clipping mode.
	void set_face_cull_mode(CL_CullMode value);

	/// \brief Sets the filling mode for front-facing polygons.
	void set_face_fill_mode_front(CL_FillMode value);

	/// \brief Sets the filling mode for back-facing polygons.
	void set_face_fill_mode_back(CL_FillMode value);

	/// \brief Sets which side is the front side of a face.
	void set_front_face(CL_FaceSide value);

	/// \brief Sets the offset factor.
	void set_offset_factor(float value);

	/// \brief Sets the offset units.
	void set_offset_units(float value);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_PolygonRasterizer_Impl> impl;
/// \}
};

/// \}

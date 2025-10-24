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

/// \brief Point Sprite Origin
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_PointSpriteOrigin
{
	cl_point_sprite_origin_upper_left,
	cl_point_sprite_origin_lower_left
};

class CL_Pen_Impl;

/// \brief Pen description class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Pen
{
//!Construction
public:
	/// \brief Constructs a pen description.
	CL_Pen();

	virtual ~CL_Pen();

//!Attributes
public:
	/// \brief Returns the point size
	float get_point_size() const;

	/// \brief Returns the maximum point size
	float get_max_point_size() const;

	/// \brief Returns the minimum point size
	float get_min_point_size() const;

	/// \brief Returns the treshold size for alpha fading to kick in (Requires multisampling to be enabled.)
	float get_point_fade_treshold_size() const;

	/// \brief Get the a, b and c function coefficients used in point distance attenuation calculations.
	void get_point_distance_attenuation(float &a, float &b, float &c) const;

	/// \brief Return true if antialiasing of points is enabled.
	bool is_point_antialiased() const;

	// Returns the line width.
	float get_line_width() const;

	/// \brief Returns the line stipple pattern
	unsigned short get_line_stipple_pattern() const;

	/// \brief Returns the line stipple repeat count.
	int get_line_stipple_repeat_count() const;

	/// \brief Returns true if line stippling is enabled.
	bool is_line_stippled() const;

	/// \brief Returns true if line antialiasing is enabled.
	bool is_line_antialiased() const;

	/// \brief Returns true if points are being drawn as texture point sprites.
	bool is_using_point_sprites() const;

	/// \brief Enables if points sizes is set by the vertex shader.
	bool is_using_vertex_program_point_sizes() const;

	/// \brief Returns the origin of texture point sprites.
	CL_PointSpriteOrigin get_point_sprite_origin() const;

//!Operations
public:
	/// \brief The default value is 1.0
	void set_point_size(float);

	/// \brief Point size is clamped to the min and max values
	void set_max_point_size(float);

	/// \brief Point size is clamped to the min and max values
	void set_min_point_size(float);

	/// \brief Alpha fade point once minimum size treshold reached. Requires multisampling to be enabled.
	void set_point_fade_treshold_size(float);

	/// \brief Set the a, b and c function coefficients used in point distance attenuation calculations.
	void set_point_distance_attenuation(float a, float b, float c);

	/// \brief Setting to true enables point antialiasing.
	/** <p>Antialiasing of points is disabled by default</p> */
	void enable_point_antialiasing(bool enabled);

	/// \brief Set the number of times each bit in the stipple pattern is used.
	/** <p>The initial value of the line width is 1.0.</p>*/
	void set_line_width(float);

	/// \brief Set the line stipple pattern.
	/** <p>Line stippling masks certain fragments that are produced by rasterization,
	    thus producing a non-continuous line when rendering. Each bit in the pattern is repeated
	    as specified by the function set_line_stipple_repeat_count(int).
	    The initial line stipple value is FFFF (continuos line).</p>*/
	void set_line_stipple_pattern(unsigned short);

	/// \brief Set the number of times each bit in the stipple pattern is used.
	/** <p>The initial value of the line stipple repeat count is 1. The value is clamped to
	    the range 1-256</p>*/
	void set_line_stipple_repeat_count(int);

	/// \brief Setting to true enables line Stippling.
	/** <p>Initially line stippling is disabled.</p> */
	void enable_line_stipple(bool enabled);

	/// \brief Setting to true enables line antialiasing.
	/** <p>Initially antialiasing of lines is disabled.</p>*/
	void enable_line_antialiasing(bool enabled);

	/// \brief Enables if points are being drawn as texture point sprites.
	void enable_point_sprite(bool enable);

	/// \brief Enables if points sizes is set by the vertex shader.
	void enable_vertex_program_point_size(bool enable);

	/// \brief Sets the origin of texture point sprites.
	void set_point_sprite_origin(CL_PointSpriteOrigin origin);

//!Implementation
private:
	CL_SharedPtr<CL_Pen_Impl> impl;
};

/// \}

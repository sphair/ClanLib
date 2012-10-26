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
**    Magnus Norddahl
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "graphic_context.h"

namespace clan
{

class RasterizerStateDescription_Impl;

/// \brief Rasterizer state description.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY RasterizerStateDescription
{
/// \name Construction
/// \{
public:
	RasterizerStateDescription();
	RasterizerStateDescription clone() const;
/// \}

/// \name Attributes
/// \{
public:
	bool get_culled() const;
	bool get_enable_line_antialiasing() const;
	CullMode get_face_cull_mode() const;
	FillMode get_face_fill_mode() const;
	FaceSide get_front_face() const;
	bool get_enable_scissor() const;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Enables/disables polygon cull clipping.
	void set_culled(bool value);

	/// \brief Setting to true enables line antialiasing.
	/** <p>Initially antialiasing of lines is disabled.</p>*/
	void enable_line_antialiasing(bool enabled);

	/// \brief Sets the polygon cull clipping mode.
	void set_face_cull_mode(CullMode value);

	/// \brief Sets the filling mode for polygons.
	void set_face_fill_mode(FillMode value);

	/// \brief Sets which side is the front side of a face.
	void set_front_face(FaceSide value);

	/// \brief Enables/disables if clipping rects are used
	void enable_scissor(bool enabled);
/// \}

	bool operator==(const RasterizerStateDescription &other) const;
	bool operator<(const RasterizerStateDescription &other) const;

/// \name Implementation
/// \{
private:
	std::shared_ptr<RasterizerStateDescription_Impl> impl;
/// \}
};

}

/// \}

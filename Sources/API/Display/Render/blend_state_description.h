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

class BlendStateDescription_Impl;

/// \brief Blend state description.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY BlendStateDescription
{
/// \name Construction
/// \{
public:
	BlendStateDescription();
	BlendStateDescription clone() const;
/// \}

/// \name Attributes
/// \{
public:
	bool is_blending_enabled() const;
	void get_blend_equation(BlendEquation &out_color, BlendEquation &out_alpha) const;
	void get_blend_function(BlendFunc &out_src, BlendFunc &out_dest, BlendFunc &out_src_alpha, BlendFunc &out_dest_alpha) const;
	void get_color_write(bool &out_red, bool &out_green, bool &out_blue, bool &out_alpha) const;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Enable/Disable blending
	void enable_blending(bool value);

	/// \brief Set the constant color used in the blend equations
	void set_blend_equation(BlendEquation color, BlendEquation alpha);

	/// \brief Set the blend functions
	void set_blend_function(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha);

	/// \brief Enable/disable writing to the color buffer
	void enable_color_write(bool red, bool green, bool blue, bool alpha);
/// \}

	bool operator==(const BlendStateDescription &other) const;
	bool operator<(const BlendStateDescription &other) const;

/// \name Implementation
/// \{
private:
	std::shared_ptr<BlendStateDescription_Impl> impl;
/// \}
};

}

/// \}

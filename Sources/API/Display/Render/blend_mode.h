/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_Colorf;
class CL_BlendMode_Impl;

/// \brief Blending functions.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_BlendFunc
{
	/// \brief source or destination (0, 0, 0, 0)
	cl_blend_zero,

	/// \brief source or destination (1, 1, 1, 1)
	cl_blend_one,

	/// \brief source (Rd, Gd, Bd, Ad)
	cl_blend_dest_color,

	/// \brief destination (Rs, Gs, Bs, As)
	cl_blend_src_color,

	/// \brief source (1, 1, 1, 1) - (Rd, Gd, Bd, Ad)
	cl_blend_one_minus_dest_color,

	/// \brief destination (1, 1, 1, 1) - (Rs, Gs, Bs, As)
	cl_blend_one_minus_src_color,

	/// \brief source or destination (As, As, As, As)
	cl_blend_src_alpha,

	/// \brief source or destination (1, 1, 1, 1) - (As, As, As, As)
	cl_blend_one_minus_src_alpha,

	/// \brief source or destination (Ad, Ad, Ad, Ad)
	cl_blend_dest_alpha,

	/// \brief source or destination (1, 1, 1, 1) - (Ad, Ad, Ad, Ad)
	cl_blend_one_minus_dest_alpha,

	/// \brief source (f, f, f, 1) - f = min(As, 1 - Ad)
	cl_blend_src_alpha_saturate,

	/// \brief source or destination (Rc, Gc, Bc, Ac)
	cl_blend_constant_color,

	/// \brief source or destination (1, 1, 1, 1) - (Rc, Gc, Bc, Ac)
	cl_blend_one_minus_constant_color,

	/// \brief source or destination (Ac, Ac, Ac, Ac)
	cl_blend_constant_alpha,

	/// \brief source or destination (1, 1, 1, 1) - (Ac, Ac, Ac, Ac)
	cl_blend_one_minus_constant_alpha
};

/// \brief Blending equations.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_BlendEquation
{
	cl_blend_equation_add,
	cl_blend_equation_subtract,
	cl_blend_equation_reverse_subtract,
	cl_blend_equation_min,
	cl_blend_equation_max
};

/// \brief Blend mode description.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_BlendMode
{
/// \name Construction
/// \{

public:
	CL_BlendMode();

	~CL_BlendMode();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the constant color used in the blend equations.
	const CL_Colorf &get_blend_color() const;

	/// \brief Returns the color blend equation.
	CL_BlendEquation get_blend_equation() const;

	/// \brief Returns the alpha blend equation.
	CL_BlendEquation get_blend_equation_alpha() const;

	/// \brief Returns the source color blend function.
	CL_BlendFunc get_blend_function_src() const;

	/// \brief Returns the destination color blend function.
	CL_BlendFunc get_blend_function_dest() const;

	/// \brief Returns the source alpha blend function.
	CL_BlendFunc get_blend_function_src_alpha() const;

	/// \brief Returns the destination alpha blend function.
	CL_BlendFunc get_blend_function_dest_alpha() const;

	/// \brief Returns true if blending mode is enabled.
	bool is_blend_enabled() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Returns true if the two blend mode are equal in states.
	bool operator ==(const CL_BlendMode &other) const;

	/// \brief Enable/Disable blending
	void enable_blending(bool value);

	/// \brief Set the constant color used in the blend equations
	void set_blend_color(const CL_Colorf &color);

	/// \brief Set the constant color used in the blend equations
	void set_blend_equation(CL_BlendEquation color, CL_BlendEquation alpha);

	/// \brief Set the blend functions
	void set_blend_function(CL_BlendFunc src, CL_BlendFunc dest, CL_BlendFunc src_alpha, CL_BlendFunc dest_alpha);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_BlendMode_Impl> impl;
/// \}
};

/// \}

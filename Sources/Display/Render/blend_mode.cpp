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

#include "Display/precomp.h"
#include "API/Display/Render/blend_mode.h"
#include "API/Display/2D/color.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BlendMode_Impl Class:

class CL_BlendMode_Impl
{
public:
	CL_BlendMode_Impl()	:
	  src_func(cl_blend_src_alpha),
	  dest_func(cl_blend_one_minus_src_alpha),
	  src_alpha_func(cl_blend_one), 
	  dest_alpha_func(cl_blend_one_minus_src_alpha),
	  color_equation(cl_blend_equation_add),
	  alpha_equation(cl_blend_equation_add),
	  blend_color(CL_Colorf(1.0f,1.0f,1.0f)), blending_enabled(true)
	{
	}

	~CL_BlendMode_Impl()
	{
	}

	bool operator ==(const CL_BlendMode_Impl &other) const
	{
		return
			src_func == other.src_func &&
			dest_func == other.dest_func &&
			dest_alpha_func == other.dest_alpha_func &&
			color_equation == other.color_equation &&
			alpha_equation == other.alpha_equation &&
			blend_color == other.blend_color &&
			blending_enabled == other.blending_enabled;
	}

	CL_BlendFunc src_func, dest_func, src_alpha_func, dest_alpha_func;
	CL_BlendEquation color_equation, alpha_equation;
	CL_Colorf blend_color;
	bool blending_enabled;
};

/////////////////////////////////////////////////////////////////////////////
// CL_BlendMode Construction:

CL_BlendMode::CL_BlendMode()
: impl(new CL_BlendMode_Impl)
{
	return;
}

CL_BlendMode::~CL_BlendMode()
{
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BlendMode Attributes:

const CL_Colorf &CL_BlendMode::get_blend_color() const
{
	return impl->blend_color;
}

CL_BlendEquation CL_BlendMode::get_blend_equation() const
{
	return impl->color_equation;
}

CL_BlendEquation CL_BlendMode::get_blend_equation_alpha() const
{
	return impl->alpha_equation;
}

CL_BlendFunc CL_BlendMode::get_blend_function_src() const
{
	return impl->src_func;
}

CL_BlendFunc CL_BlendMode::get_blend_function_dest() const
{
	return impl->dest_func;
}

CL_BlendFunc CL_BlendMode::get_blend_function_src_alpha() const
{
	return impl->src_alpha_func;
}

CL_BlendFunc CL_BlendMode::get_blend_function_dest_alpha() const
{
	return impl->dest_alpha_func;
}

bool CL_BlendMode::is_blend_enabled() const
{
	return impl->blending_enabled;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BlendMode Operations:

bool CL_BlendMode::operator ==(const CL_BlendMode &other) const
{
	return *impl.get() == *other.impl.get();
}

void CL_BlendMode::set_blend_color(const CL_Colorf &color)
{
	impl->blend_color = color;
}

void CL_BlendMode::set_blend_equation(CL_BlendEquation color, CL_BlendEquation alpha)
{
	impl->color_equation = color;
	impl->alpha_equation = alpha;
}

void CL_BlendMode::set_blend_function(CL_BlendFunc src, CL_BlendFunc dest, CL_BlendFunc src_alpha, CL_BlendFunc dest_alpha )
{
	impl->src_func = src;
	impl->dest_func = dest;
	impl->src_alpha_func = src_alpha;
	impl->dest_alpha_func = dest_alpha;
}

void CL_BlendMode::enable_blending(bool value)
{
	impl->blending_enabled = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BlendMode Implementation:

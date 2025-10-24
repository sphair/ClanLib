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
**    Magnus Norddahl
**    Mark Page
*/

/// \addtogroup clanSWRender_Display clanSWRender Display
/// \{

#pragma once

#include "api_swrender.h"
#include "../Core/Math/vec2.h"
#include "../Core/Math/vec4.h"
#include "../Core/Math/mat4.h"

class CL_PixelPipeline;
class CL_PixelCommand;

/// \brief Interface for commands participating in drawing primitives
///
/// \xmlonly !group=SWRender/Display! !header=swrender.h! \endxmlonly
class CL_SoftwareProgram
{
//!Operations
public:
	virtual ~CL_SoftwareProgram() { }

	virtual int get_attribute_count() const = 0;
	virtual int get_attribute_index(const CL_StringRef &name) const = 0;
	virtual CL_Vec4f get_attribute_default(int index) { return CL_Vec4f(0.0f, 0.0f, 1.0f, 1.0f); }
	virtual void set_uniform(const CL_StringRef &name, const CL_Vec4f &vec) = 0;
	virtual void set_uniform_matrix(const CL_StringRef &name, const CL_Mat4f &mat) = 0;

	virtual CL_PixelCommand *draw_triangle(CL_PixelPipeline *pipeline, const std::vector<CL_Vec4f> &attribute_values) = 0;
	virtual CL_PixelCommand *draw_sprite(CL_PixelPipeline *pipeline, const std::vector<CL_Vec4f> &attribute_values) = 0;
	virtual CL_PixelCommand *draw_line(CL_PixelPipeline *pipeline, const std::vector<CL_Vec4f> &attribute_values) = 0;
};

/// \}

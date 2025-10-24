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
**    Magnus Norddahl
**    Mark Page
*/

/// \addtogroup clanSWRender_Display clanSWRender Display
/// \{

#pragma once

#include "api_swrender.h"
#include "../Core/Math/vec2.h"
#include "../Core/Math/vec4.h"

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

	virtual CL_PixelCommand *draw_triangle(CL_PixelPipeline *pipeline, const CL_Vec2f init_points[3], const CL_Vec4f init_primcolor[3], const CL_Vec2f init_texcoords[3], int init_sampler) = 0;
	virtual CL_PixelCommand *draw_sprite(CL_PixelPipeline *pipeline, const CL_Vec2f init_points[3], const CL_Vec4f init_primcolor[3], const CL_Vec2f init_texcoords[3], int init_sampler) = 0;
	virtual CL_PixelCommand *draw_line(CL_PixelPipeline *pipeline, const CL_Vec2f init_points[2], const CL_Vec4f init_primcolor[2], const CL_Vec2f init_texcoords[2], int init_sampler) = 0;
};

/// \}

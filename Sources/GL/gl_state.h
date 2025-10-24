/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_gl_state
#define header_gl_state

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include "API/GL/opengl.h"
#include "API/Display/blend_func.h"
#include "API/Display/gradient.h"
#include "API/Core/Math/point.h"

#include "fillmode.h"

class CL_GLState
{
//! Construction:
public:
	CL_GLState() : 
		color_range_zero2one(false),
		uv_range_zero2one(false),
		sub_pixel_accuracy(false)
	{
		return;
	};

//! Attributes:
public:
	CLfloat color[4];

	int texture_handle;

	CL_BlendFunc blendfunc[4];

	CL_FillMode fillmode;

	bool color_range_zero2one;
	bool uv_range_zero2one;
	bool sub_pixel_accuracy;
	
};

#endif

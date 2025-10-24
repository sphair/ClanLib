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

#ifndef header_opengl_window_description_generic
#define header_opengl_window_description_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "Display/display_window_description_generic.h"

class CL_OpenGLWindowDescription_Generic : public CL_DisplayWindowDescription_Generic
{
//! Construction:
public:
	CL_OpenGLWindowDescription_Generic()
	{
		rgba = true;
		doublebuffer = true;
		stereo = false;
		buffer_size = 16;
		level = 0;
		aux_buffers = 0;
		red_size = 4;
		green_size = 4;
		blue_size = 4;
		alpha_size = 4;
		depth_size = 16;
		stencil_size = 8;
		accum_red_size = 0;
		accum_green_size = 0;
		accum_blue_size = 0;
		accum_alpha_size = 0;
	}

//! Attributes:
public:

	bool rgba;
	bool doublebuffer;
	bool stereo;
	int buffer_size;
	int level;
	int aux_buffers;
	int red_size;
	int green_size;
	int blue_size;
	int alpha_size;
	int depth_size;
	int stencil_size;
	int accum_red_size;
	int accum_green_size;
	int accum_blue_size;
	int accum_alpha_size;
};

#endif

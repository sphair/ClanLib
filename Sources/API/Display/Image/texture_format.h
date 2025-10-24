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

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"

/// \brief Texture format.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_TextureFormat
{
	// Standard ClanLib sized formats (most common listed first)
	cl_rgba8,
	cl_rgb8,
	cl_bgr8,	// (Not OpenGL)
	cl_a1_rgb5, // (Not OpenGL)
	cl_argb8,	// (Not OpenGL)
	cl_abgr8,	// (Not OpenGL)

	cl_color_index,	// Paletted index (Not OpenGL)

	// base internal format
	cl_stencil_index,		// For CL_RenderBuffer ?
	cl_depth_component,
	cl_depth_stencil,
	cl_red,
	cl_green,
	cl_blue,
	cl_rg,
	cl_rgb,
	cl_rgba,
	cl_bgr,
	cl_bgra,
	cl_red_integer,
	cl_green_integer,
	cl_blue_integer,
	cl_rg_integer,
	cl_rgb_integer,
	cl_rgba_integer,
	cl_bgr_integer,
	cl_bgra_integer,

	// sized internal format
	cl_stencil_index1,		// For CL_RenderBuffer ?
	cl_stencil_index4,		// For CL_RenderBuffer ?
	cl_stencil_index8,		// For CL_RenderBuffer ?
	cl_stencil_index16,		// For CL_RenderBuffer ?

	cl_r8,
	cl_r8_snorm,
	cl_r16,
	cl_r16_snorm,
	cl_rg8,
	cl_rg8_snorm,
	cl_rg16,
	cl_rg16_snorm,
	cl_r3_g3_b2,
	cl_rgb4,
	cl_rgb5,
	//cl_rgb8, (listed earlier)
	cl_rgb8_snorm,
	cl_rgb10,
	cl_rgb12,
	cl_rgb16,
	cl_rgb16_snorm,
	cl_rgba2,
	cl_rgba4,
	cl_rgb5_a1,
	//cl_rgba8, (listed earlier)
	cl_rgba8_snorm,
	cl_rgb10_a2,
	cl_rgba12,
	cl_rgba16,
	cl_rgba16_snorm,
	cl_srgb8,
	cl_srgb8_alpha8,
	cl_r16f,
	cl_rg16f,
	cl_rgb16f,
	cl_rgba16f,
	cl_r32f,
	cl_rg32f,
	cl_rgb32f,
	cl_rgba32f,
	cl_r11f_g11f_b10f,
	cl_rgb9_e5,
	cl_r8i,
	cl_r8ui,
	cl_r16i,
	cl_r16ui,
	cl_r32i,
	cl_r32ui,
	cl_rg8i,
	cl_rg8ui,
	cl_rg16i,
	cl_rg16ui,
	cl_rg32i,
	cl_rg32ui,
	cl_rgb8i,
	cl_rgb8ui,
	cl_rgb16i,
	cl_rgb16ui,
	cl_rgb32i,
	cl_rgb32ui,
	cl_rgba8i,
	cl_rgba8ui,
	cl_rgba16i,
	cl_rgba16ui,
	cl_rgba32i,
	cl_rgba32ui,
	cl_depth_component16,
	cl_depth_component24,
	cl_depth_component32,
	cl_depth_component32f,
	cl_depth24_stencil8,
	cl_depth32f_stencil8,
	cl_compressed_red,
	cl_compressed_rg,
	cl_compressed_rgb,
	cl_compressed_rgba,
	cl_compressed_srgb,
	cl_compressed_srgb_alpha,
	cl_compressed_red_rgtc1,
	cl_compressed_signed_red_rgtc1,
	cl_compressed_rg_rgtc2,
	cl_compressed_signed_rg_rgtc2

};

/// \}

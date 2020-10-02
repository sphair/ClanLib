/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#pragma once

namespace clan
{
	/// \addtogroup clanDisplay_Display clanDisplay Display
	/// \{

	/// \brief Texture format.
	enum class TextureFormat
	{
		// Standard ClanLib sized formats (most common listed first)
		rgba8,
		rgb8,
		bgra8,
		bgr8,

		// sized internal format
		stencil_index1,		// For RenderBuffer ?
		stencil_index4,		// For RenderBuffer ?
		stencil_index8,		// For RenderBuffer ?
		stencil_index16,		// For RenderBuffer ?

		r8,
		r8_snorm,
		r16,
		r16_snorm,
		rg8,
		rg8_snorm,
		rg16,
		rg16_snorm,
		r3_g3_b2,
		rgb4,
		rgb5,
		//rgb8, (listed earlier)
		rgb8_snorm,
		rgb10,
		rgb12,
		rgb16,
		rgb16_snorm,
		rgba2,
		rgba4,
		rgb5_a1,
		//rgba8, (listed earlier)
		rgba8_snorm,
		rgb10_a2,
		rgba12,
		rgba16,
		rgba16_snorm,
		srgb8,
		srgb8_alpha8,
		r16f,
		rg16f,
		rgb16f,
		rgba16f,
		r32f,
		rg32f,
		rgb32f,
		rgba32f,
		r11f_g11f_b10f,
		rgb9_e5,
		r8i,
		r8ui,
		r16i,
		r16ui,
		r32i,
		r32ui,
		rg8i,
		rg8ui,
		rg16i,
		rg16ui,
		rg32i,
		rg32ui,
		rgb8i,
		rgb8ui,
		rgb16i,
		rgb16ui,
		rgb32i,
		rgb32ui,
		rgba8i,
		rgba8ui,
		rgba16i,
		rgba16ui,
		rgba32i,
		rgba32ui,
		depth_component16,
		depth_component24,
		depth_component32,
		depth_component32f,
		depth24_stencil8,
		depth32f_stencil8,
		compressed_red,
		compressed_rg,
		compressed_rgb,
		compressed_rgba,
		compressed_srgb,
		compressed_srgb_alpha,
		compressed_red_rgtc1,
		compressed_signed_red_rgtc1,
		compressed_rg_rgtc2,
		compressed_signed_rg_rgtc2,
		compressed_rgb_s3tc_dxt1,
		compressed_rgba_s3tc_dxt1,
		compressed_rgba_s3tc_dxt3,
		compressed_rgba_s3tc_dxt5,
		compressed_srgb_s3tc_dxt1,
		compressed_srgb_alpha_s3tc_dxt1,
		compressed_srgb_alpha_s3tc_dxt3,
		compressed_srgb_alpha_s3tc_dxt5
	};

	/// \}
}

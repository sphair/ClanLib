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
**    Kevin J Bluck
*/

#include "GL/precomp.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/Core/System/exception.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/texture.h"
#include "API/Display/TargetProviders/render_window_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "opengl_graphic_context_provider.h"
#include <map>

#ifdef __APPLE__
#include <AGL/agl.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifndef WIN32
#ifdef __APPLE__
#include <AGL/agl.h>
#include <Carbon/Carbon.h>
#else
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#endif
#endif

CL_GLFunctions *CL_OpenGL::functions = 0;


void CL_OpenGL::to_opengl_textureformat(CL_TextureFormat format, CLint &gl_internal_format, CLenum &gl_pixel_format)
{
	switch (format)
	{
	// base internal format
		case cl_alpha: gl_internal_format = GL_ALPHA; gl_pixel_format = GL_ALPHA; break;
		case cl_depth_component: gl_internal_format = GL_DEPTH_COMPONENT; gl_pixel_format = GL_DEPTH_COMPONENT; break;
		//case cl_depth_stencil: gl_internal_format = GL_DEPTH_STENCIL; gl_pixel_format = GL_DEPTH_STENCIL; break;
		case cl_intensity: gl_internal_format = GL_INTENSITY; gl_pixel_format = GL_INTENSITY; break;
		case cl_luminance: gl_internal_format = GL_LUMINANCE; gl_pixel_format = GL_LUMINANCE; break;
		case cl_luminance_alpha: gl_internal_format = GL_LUMINANCE_ALPHA; gl_pixel_format = GL_LUMINANCE_ALPHA; break;
		case cl_red: gl_internal_format = GL_RED; gl_pixel_format = GL_RED; break;
		//case cl_rg: gl_internal_format = GL_RG; gl_pixel_format = GL_RG; break;
		case cl_rgb: gl_internal_format = GL_RGB; gl_pixel_format = GL_RGB; break;
		case cl_rgba: gl_internal_format = GL_RGBA; gl_pixel_format = GL_RGBA; break;
		case cl_stencil_index: gl_internal_format = GL_STENCIL_INDEX; gl_pixel_format = GL_STENCIL_INDEX; break;

	// sized internal format
		//case cl_stencil_index1: gl_internal_format = GL_STENCIL_INDEX1; gl_pixel_format = GL_STENCIL_INDEX; break;
		//case cl_stencil_index4: gl_internal_format = GL_STENCIL_INDEX4; gl_pixel_format = GL_STENCIL_INDEX; break;
		//case cl_stencil_index8: gl_internal_format = GL_STENCIL_INDEX8; gl_pixel_format = GL_STENCIL_INDEX; break;
		//case cl_stencil_index16: gl_internal_format = GL_STENCIL_INDEX16; gl_pixel_format = GL_STENCIL_INDEX; break;

		case cl_alpha4: gl_internal_format = CL_ALPHA4; gl_pixel_format = CL_ALPHA; break;
		case cl_alpha8: gl_internal_format = CL_ALPHA8; gl_pixel_format = CL_ALPHA; break;
		case cl_alpha12: gl_internal_format = CL_ALPHA12; gl_pixel_format = CL_ALPHA; break;
		case cl_alpha16: gl_internal_format = CL_ALPHA16; gl_pixel_format = CL_ALPHA; break;
		//case cl_r8: gl_internal_format = CL_R8; gl_pixel_format = CL_RED; break;
		//case cl_r16: gl_internal_format = CL_R16; gl_pixel_format = CL_RED; break;
		//case cl_rg8: gl_internal_format = CL_RG8; gl_pixel_format = CL_RG; break;
		//case cl_rg16: gl_internal_format = CL_RG16; gl_pixel_format = CL_RG; break;
		case cl_r3_g3_b2: gl_internal_format = CL_R3_G3_B2; gl_pixel_format = CL_RGB; break;
		case cl_rgb4: gl_internal_format = CL_RGB4; gl_pixel_format = CL_RGB; break;
		case cl_rgb5: gl_internal_format = CL_RGB5; gl_pixel_format = CL_RGB; break;
		case cl_rgb8: gl_internal_format = CL_RGB8; gl_pixel_format = CL_RGB; break;
		case cl_rgb10: gl_internal_format = CL_RGB10; gl_pixel_format = CL_RGB; break;
		case cl_rgb12: gl_internal_format = CL_RGB12; gl_pixel_format = CL_RGB; break;
		case cl_rgb16: gl_internal_format = CL_RGB16; gl_pixel_format = CL_RGB; break;
		case cl_rgba2: gl_internal_format = CL_RGBA2; gl_pixel_format = CL_RGBA; break;
		case cl_rgba4: gl_internal_format = CL_RGBA4; gl_pixel_format = CL_RGBA; break;
		case cl_rgb5_a1: gl_internal_format = CL_RGB5_A1; gl_pixel_format = CL_RGBA; break;
		case cl_rgba8: gl_internal_format = CL_RGBA8; gl_pixel_format = CL_RGBA; break;
		case cl_rgb10_a2: gl_internal_format = CL_RGB10_A2; gl_pixel_format = CL_RGBA; break;
		case cl_rgba12: gl_internal_format = CL_RGBA12; gl_pixel_format = CL_RGBA; break;
		case cl_rgba16: gl_internal_format = CL_RGBA16; gl_pixel_format = CL_RGBA; break;
		//case cl_srgb8: gl_internal_format = CL_SRGB8; gl_pixel_format = CL_RGB; break;
		//case cl_srgb8_alpha8: gl_internal_format = CL_SRGB8_ALPHA8; gl_pixel_format = CL_RGBA; break;
		//case cl_r16f: gl_internal_format = CL_R16F; gl_pixel_format = CL_RED; break;
		//case cl_rg16f: gl_internal_format = CL_RG16F; gl_pixel_format = CL_RG; break;
		//case cl_rgb16f: gl_internal_format = CL_RGB16F; gl_pixel_format = CL_RGB; break;
		//case cl_rgba16f: gl_internal_format = CL_RGBA16F; gl_pixel_format = CL_RGBA; break;
		//case cl_r32f: gl_internal_format = CL_R32F; gl_pixel_format = CL_RED; break;
		//case cl_rg32f: gl_internal_format = CL_RG32F; gl_pixel_format = CL_RG; break;
		//case cl_rgb32f: gl_internal_format = CL_RGB32F; gl_pixel_format = CL_RGB; break;
		//case cl_rgba32f: gl_internal_format = CL_RGBA32F; gl_pixel_format = CL_RGBA; break;
		//case cl_r11f_g11f_b10f: gl_internal_format = CL_R11F_G11F_B10F; gl_pixel_format = CL_RGB; break;
		//case cl_rgb9_e5: gl_internal_format = CL_RGB9_E5; gl_pixel_format = CL_RGB; break;
		//case cl_r8i: gl_internal_format = CL_R8I; gl_pixel_format = CL_RED; break;
		//case cl_r8ui: gl_internal_format = CL_R8UI; gl_pixel_format = CL_RED; break;
		//case cl_r16i: gl_internal_format = CL_R16I; gl_pixel_format = CL_RED; break;
		//case cl_r16ui: gl_internal_format = CL_R16UI; gl_pixel_format = CL_RED; break;
		//case cl_r32i: gl_internal_format = CL_R32I; gl_pixel_format = CL_RED; break;
		//case cl_r32ui: gl_internal_format = CL_R32UI; gl_pixel_format = CL_RED; break;
		//case cl_rg8i: gl_internal_format = CL_RG8I; gl_pixel_format = CL_RG; break;
		//case cl_rg8ui: gl_internal_format = CL_RG8UI; gl_pixel_format = CL_RG; break;
		//case cl_rg16i: gl_internal_format = CL_RG16I; gl_pixel_format = CL_RG; break;
		//case cl_rg16ui: gl_internal_format = CL_RG16UI; gl_pixel_format = CL_RG; break;
		//case cl_rg32i: gl_internal_format = CL_RG32I; gl_pixel_format = CL_RG; break;
		//case cl_rg32ui: gl_internal_format = CL_RG32UI; gl_pixel_format = CL_RG; break;
		//case cl_rgb8i: gl_internal_format = CL_RGB8I; gl_pixel_format = CL_RGB; break;
		//case cl_rgb8ui: gl_internal_format = CL_RGB8UI; gl_pixel_format = CL_RGB; break;
		//case cl_rgb16i: gl_internal_format = CL_RGB16I; gl_pixel_format = CL_RGB; break;
		//case cl_rgb16ui: gl_internal_format = CL_RGB16UI; gl_pixel_format = CL_RGB; break;
		//case cl_rgb32i: gl_internal_format = CL_RGB32I; gl_pixel_format = CL_RGB; break;
		//case cl_rgb32ui: gl_internal_format = CL_RGB32UI; gl_pixel_format = CL_RGB; break;
		//case cl_rgba8i: gl_internal_format = CL_RGBA8I; gl_pixel_format = CL_RGBA; break;
		//case cl_rgba8ui: gl_internal_format = CL_RGBA8UI; gl_pixel_format = CL_RGBA; break;
		//case cl_rgba16i: gl_internal_format = CL_RGBA16I; gl_pixel_format = CL_RGBA; break;
		//case cl_rgba16ui: gl_internal_format = CL_RGBA16UI; gl_pixel_format = CL_RGBA; break;
		//case cl_rgba32i: gl_internal_format = CL_RGBA32I; gl_pixel_format = CL_RGBA; break;
		//case cl_rgba32ui: gl_internal_format = CL_RGBA32UI; gl_pixel_format = CL_RGBA; break;
		case cl_luminance4: gl_internal_format = CL_LUMINANCE4; gl_pixel_format = CL_LUMINANCE; break;
		case cl_luminance8: gl_internal_format = CL_LUMINANCE8; gl_pixel_format = CL_LUMINANCE; break;
		case cl_luminance12: gl_internal_format = CL_LUMINANCE12; gl_pixel_format = CL_LUMINANCE; break;
		case cl_luminance16: gl_internal_format = CL_LUMINANCE16; gl_pixel_format = CL_LUMINANCE; break;
		case cl_luminance4_alpha4: gl_internal_format = CL_LUMINANCE4_ALPHA4; gl_pixel_format = CL_LUMINANCE_ALPHA; break;
		case cl_luminance6_alpha2: gl_internal_format = CL_LUMINANCE6_ALPHA2; gl_pixel_format = CL_LUMINANCE_ALPHA; break;
		case cl_luminance8_alpha8: gl_internal_format = CL_LUMINANCE8_ALPHA8; gl_pixel_format = CL_LUMINANCE_ALPHA; break;
		case cl_luminance12_alpha4: gl_internal_format = CL_LUMINANCE12_ALPHA4; gl_pixel_format = CL_LUMINANCE_ALPHA; break;
		case cl_luminance12_alpha12: gl_internal_format = CL_LUMINANCE12_ALPHA12; gl_pixel_format = CL_LUMINANCE_ALPHA; break;
		case cl_luminance16_alpha16: gl_internal_format = CL_LUMINANCE16_ALPHA16; gl_pixel_format = CL_LUMINANCE_ALPHA; break;
		case cl_intensity4: gl_internal_format = CL_INTENSITY4; gl_pixel_format = CL_INTENSITY; break;
		case cl_intensity8: gl_internal_format = CL_INTENSITY8; gl_pixel_format = CL_INTENSITY; break;
		case cl_intensity12: gl_internal_format = CL_INTENSITY12; gl_pixel_format = CL_INTENSITY; break;
		case cl_intensity16: gl_internal_format = CL_INTENSITY16; gl_pixel_format = CL_INTENSITY; break;
		case cl_depth_component16: gl_internal_format = CL_DEPTH_COMPONENT16; gl_pixel_format = CL_DEPTH_COMPONENT; break;
		case cl_depth_component24: gl_internal_format = CL_DEPTH_COMPONENT24; gl_pixel_format = CL_DEPTH_COMPONENT; break;
		case cl_depth_component32: gl_internal_format = CL_DEPTH_COMPONENT32; gl_pixel_format = CL_DEPTH_COMPONENT; break;
		//case cl_depth_component32f: gl_internal_format = CL_DEPTH_COMPONENT32F; gl_pixel_format = CL_DEPTH_COMPONENT; break;
		//case cl_depth24_stencil8: gl_internal_format = CL_DEPTH24_STENCIL8; gl_pixel_format = CL_DEPTH_STENCIL; break;
		//case cl_depth32f_stencil8: gl_internal_format = CL_DEPTH32F_STENCIL8; gl_pixel_format = CL_DEPTH_STENCIL; break;
		//case cl_sluminance: gl_internal_format = CL_SLUMINANCE; gl_pixel_format = CL_LUMINANCE; break;
		//case cl_sluminance_alpha8: gl_internal_format = CL_SLUMINANCE_ALPHA8; gl_pixel_format = CL_LUMINANCE; break;
		case cl_compressed_alpha: gl_internal_format = CL_COMPRESSED_ALPHA; gl_pixel_format = CL_ALPHA; break;
		case cl_compressed_luminance: gl_internal_format = CL_COMPRESSED_LUMINANCE; gl_pixel_format = CL_LUMINANCE; break;
		case cl_compressed_luminance_alpha: gl_internal_format = CL_COMPRESSED_LUMINANCE_ALPHA; gl_pixel_format = CL_LUMINANCE_ALPHA; break;
		case cl_compressed_intensity: gl_internal_format = CL_COMPRESSED_INTENSITY; gl_pixel_format = CL_INTENSITY; break;
		//case cl_compressed_red: gl_internal_format = CL_COMPRESSED_RED; gl_pixel_format = CL_RED; break;
		//case cl_compressed_rg: gl_internal_format = CL_COMPRESSED_RG; gl_pixel_format = CL_RG; break;
		case cl_compressed_rgb: gl_internal_format = CL_COMPRESSED_RGB; gl_pixel_format = CL_RGB; break;
		case cl_compressed_rgba: gl_internal_format = CL_COMPRESSED_RGBA; gl_pixel_format = CL_RGBA; break;
		//case cl_compressed_srgb: gl_internal_format = CL_COMPRESSED_SRGB; gl_pixel_format = CL_RGB; break;
		//case cl_compressed_srgb_alpha: gl_internal_format = CL_COMPRESSED_SRGB_ALPHA; gl_pixel_format = CL_RGBA; break;
		//case cl_compressed_sluminance: gl_internal_format = CL_COMPRESSED_SLUMINANCE; gl_pixel_format = CL_LUMINANCE; break;
		//case cl_compressed_sluminance_alpha: gl_internal_format = CL_COMPRESSED_SLUMINANCE_ALPHA; gl_pixel_format = CL_LUMINANCE_ALPHA; break;
		//case cl_compressed_red_rgtc1: gl_internal_format = CL_COMPRESSED_RED_RGTC1; gl_pixel_format = CL_RED; break;
		//case cl_compressed_signed_red_rgtc1: gl_internal_format = CL_COMPRESSED_SIGNED_RED_RGTC1; gl_pixel_format = CL_RED; break;
		//case cl_compressed_rg_rgtc2: gl_internal_format = CL_COMPRESSED_RG_RGTC2; gl_pixel_format = CL_RG; break;
		//case cl_compressed_signed_rg_rgtc2: gl_internal_format = CL_COMPRESSED_SIGNED_RG_RGTC2; gl_pixel_format = CL_RG; break;
		default:
			throw CL_Exception(cl_text("Unknown CL_TextureFormat"));
	}

}

bool CL_OpenGL::from_opengl_pixelformat(CLenum format, CLenum type, CL_PixelFormat &pf)
{
	int bits_per_component;
	if (type == CL_UNSIGNED_BYTE || type == CL_BYTE)
		bits_per_component = 8;
	else if (type == CL_SHORT || type == CL_UNSIGNED_SHORT)
		bits_per_component = 16;
	else if (type == CL_INT || type == CL_UNSIGNED_INT || type == CL_FLOAT)
		bits_per_component = 32;
	else
		return false;

	bool big = CL_Endian::is_system_big();

	if (format == CL_COLOR_INDEX)
	{
		pf.set_type(pixelformat_index);
		pf.set_depth(bits_per_component);
	}
	else if (format == CL_RED)
	{
		pf.set_depth(bits_per_component);
		pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
	}
	else if (format == CL_GREEN)
	{
		pf.set_depth(bits_per_component);
		pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
	}
	else if (format == CL_BLUE)
	{
		pf.set_depth(bits_per_component);
		pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
	}
	else if (format == CL_ALPHA)
	{
		pf.set_depth(bits_per_component);
		pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
	}
	else if (format == CL_RGB)
	{
		if (bits_per_component * 3 > 32)
			return false;
		pf.set_depth(bits_per_component * 3);
		if (!big)
		{
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
		}
		else
		{
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
		}
	}
	else if (format == CL_RGBA)
	{
		if (bits_per_component * 4 > 32)
			return false;
		pf.set_depth(bits_per_component * 4);
		if (!big)
		{
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 3));
		}
		else
		{
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 3));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
		}
	}
	else if (format == CL_BGR)
	{
		if (bits_per_component * 3 > 32)
			return false;
		pf.set_depth(bits_per_component * 3);
		if (!big)
		{
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
		}
		else
		{
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
		}
	}
	else if (format == CL_BGRA)
	{
		if (bits_per_component * 4 > 32)
			return false;
		pf.set_depth(bits_per_component * 4);
		if (!big)
		{
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 3));
		}
		else
		{
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 3));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
		}
	}
	else
		return false;

	return true;
}

bool CL_OpenGL::to_opengl_pixelformat(const CL_PixelFormat &pf, CLenum &format, CLenum &type)
{
/*
	// Types:
	CL_UNSIGNED_BYTE
	CL_BITMAP
	CL_BYTE
	CL_UNSIGNED_SHORT
	CL_SHORT
	CL_UNSIGNED_INT
	CL_INT
	CL_FLOAT
	CL_UNSIGNED_BYTE_3_3_2
	CL_UNSIGNED_BYTE_2_3_3_REV
	CL_UNSIGNED_SHORT_5_6_5
	CL_UNSIGNED_SHORT_5_6_5_REV
	CL_UNSIGNED_SHORT_4_4_4_4
	CL_UNSIGNED_SHORT_4_4_4_4_REV
	CL_UNSIGNED_SHORT_5_5_5_1
	CL_UNSIGNED_SHORT_1_5_5_5_REV
	CL_UNSIGNED_INT_8_8_8_8
	CL_UNSIGNED_INT_8_8_8_8_REV
	CL_UNSIGNED_INT_10_10_10_2
	CL_UNSIGNED_INT_2_10_10_10_REV

	// Formats:
	CL_COLOR_INDEX
	CL_STENCIL_INDEX
	CL_DEPTH_COMPONENT
	CL_RED
	CL_GREEN
	CL_BLUE
	CL_ALPHA
	CL_RGB
	CL_RGBA
	CL_BGR
	CL_BGRA
	CL_LUMINANCE
	CL_LUMINANCE_ALPHA
*/

	// indexed modes and colorkey requires special conversion to alpha and
	// therefore no OpenGL mode has a direct conversion format
	if (pf.has_colorkey() || pf.get_type() == pixelformat_index)
		return false;

	const unsigned int rm = pf.get_red_mask();
	const unsigned int gm = pf.get_green_mask();
	const unsigned int bm = pf.get_blue_mask();
	const unsigned int am = pf.get_alpha_mask();
	if (pf.get_depth() == 8)
	{
		if (rm == (0x7u << 5) && gm == (0x7u << 2) && bm == (0x3u))
		{
			type = CL_UNSIGNED_BYTE_3_3_2;
			format = CL_RGB;
			return true;
		}
		if (rm == (0x7u) && gm == (0x7u << 3) && bm == (0x3u << 6))
		{
			type = CL_UNSIGNED_BYTE_2_3_3_REV;
			format = CL_RGB;
			return true;
		}
	}
	else if (pf.get_depth() == 16)
	{
		if (rm == (0x1fu << 11) && gm == (0x3fu << 5) && bm == (0x1fu))
		{
			type = CL_UNSIGNED_SHORT_5_6_5;
			format = CL_RGB;
			return true;
		}
		if (rm == (0x1fu) && gm == (0x3fu << 5) && bm == (0x1fu << 11))
		{
			type = CL_UNSIGNED_SHORT_5_6_5_REV;
			format = CL_RGB;
			return true;
		}
		if (rm == (0x0fu << 12) && gm == (0x0fu << 8) && bm == (0x0fu << 4) && am == (0x0fu))
		{
			type = CL_UNSIGNED_SHORT_4_4_4_4;
			format = CL_RGBA;
			return true;
		}
		if (rm == (0x0fu) && gm == (0x0fu << 4) && bm == (0x0fu << 8) && am == (0x0fu << 12))
		{
			type = CL_UNSIGNED_SHORT_4_4_4_4_REV;
			format = CL_RGBA;
			return true;
		}
		if (rm == (0x1fu << 11) && gm == (0x1fu << 6) && bm == (0x1fu << 1) && am == (0x01u))
		{
			type = CL_UNSIGNED_SHORT_5_5_5_1;
			format = CL_RGBA;
			return true;
		}
		if (rm == (0x1fu) && gm == (0x1fu << 5) && bm == (0x1fu << 10) && am == (0x01u << 15))
		{
			type = CL_UNSIGNED_SHORT_1_5_5_5_REV;
			format = CL_RGBA;
			return true;
		}
	}
	else if (pf.get_depth() == 32)
	{
		if (rm == (0xfful << 24) && gm == (0xfful << 16) && bm == (0xfful << 8) && am == (0xfful))
		{
			type = CL_UNSIGNED_INT_8_8_8_8;
			format = CL_RGBA;
			return true;
		}
		if (rm == (0xfful) && gm == (0xfful << 8) && bm == (0xfful << 16) && am == (0xfful << 24))
		{
			type = CL_UNSIGNED_INT_8_8_8_8_REV;
			format = CL_RGBA;
			return true;
		}
		if (rm == (0x3fful << 22) && gm == (0x3fful << 12) && bm == (0x3fful << 2) && am == (0x03ul))
		{
			type = CL_UNSIGNED_INT_10_10_10_2;
			format = CL_RGBA;
		}
		if (rm == (0x3fful) && gm == (0x3fful << 10) && bm == (0x3fful << 20) && am == (0x03ul << 30))
		{
			type = CL_UNSIGNED_INT_2_10_10_10_REV;
			format = CL_RGBA;
		}
	}

	bool big = CL_Endian::is_system_big();

	if (pf.get_type() == pixelformat_index)
	{
		format = CL_COLOR_INDEX;
		if (pf.get_depth() == 8)
			type = CL_UNSIGNED_BYTE;
		else if (pf.get_depth() == 16)
			type = CL_UNSIGNED_SHORT;
		else if (pf.get_depth() == 32)
			type = CL_UNSIGNED_INT;
		else
			return false;
	}
	else if (pf.get_type() == pixelformat_rgba)
	{
		const int rms = pf.get_mask_shift(rm),
			gms = pf.get_mask_shift(gm),
			bms = pf.get_mask_shift(bm),
			ams = pf.get_mask_shift(am);

		const int rmb = pf.get_mask_bits(rm),
			gmb = pf.get_mask_bits(gm),
			bmb = pf.get_mask_bits(bm),
			amb = pf.get_mask_bits(am);

		typedef std::map<int, CLenum> BitsToTypeMap;
		static BitsToTypeMap bits_to_type;
		if (bits_to_type.empty())
		{
			// initialize it only once
			bits_to_type[8] = CL_UNSIGNED_BYTE;
			bits_to_type[16] = CL_UNSIGNED_SHORT;
			bits_to_type[32] = CL_UNSIGNED_INT;
		}

		if (rm && gm && bm && am)
		{
			// the bit number must be 8, 16 or 32 for component
			// they all must have the same number of bits
			if ((!(rmb == 8 || rmb == 16 || rmb == 32)) ||
				(rmb != gmb || rmb != bmb || rmb != amb))
				return false;

			const int bits = rmb; // they all have the same bits

			if (pf.get_depth() != bits * 4)
				return false;

			type = bits_to_type[bits];

			if (!big && rms == 0*bits && gms == 1*bits && bms == 2*bits && ams == 3*bits)
				format = CL_RGBA;
			else if (!big && ams == 0*bits && bms == 1*bits && gms == 2*bits && rms == 3*bits)
				format = CL_ABGR;
			else if (big && rms == 3*bits && gms == 2*bits && bms == 1*bits && ams == 0*bits)
				format = CL_RGBA;
			else if (big && ams == 3*bits && bms == 2*bits && gms == 1*bits && rms == 0*bits)
				format = CL_ABGR;
			else
				return false;
		}
		else if (rm && gm && bm && !am)
		{
			// the bit number must be 8, 16 or 32 for component
			// they all must have the same number of bits
			if ((!(rmb == 8 || rmb == 16 || rmb == 32)) ||
				(rmb != gmb || rmb != bmb))
				return false;

			const int bits = rmb; // they all have the same bits

			if (pf.get_depth() != bits * 3)
				return false;

			type = bits_to_type[bits];

			if (!big && rms == 0*bits && gms == 1*bits && bms == 2*bits)
				format = CL_RGB;
			else if (!big && bms == 0*bits && gms == 1*bits && rms == 2*bits)
				format = CL_BGR;
			else if (big && rms == 2*bits && gms == 1*bits && bms == 0*bits)
				format = CL_RGB;
			else if (big && bms == 2*bits && gms == 1*bits && rms == 0*bits)
				format = CL_BGR;
			else
				return false;
		}
		else if (rm && !gm && !bm && !am)
		{
			format = CL_RED;
			if (rms != 0 || !bits_to_type.count(rmb))
				return false;
			if (pf.get_depth() != rmb)
				return false;
			type = bits_to_type[rmb];
		}
		else if (!rm && gm && !bm && !am)
		{
			format = CL_GREEN;
			if (gms != 0 || !bits_to_type.count(gmb))
				return false;
			if (pf.get_depth() != gmb)
				return false;
			type = bits_to_type[gmb];
		}
		else if (!rm && !gm && bm && !am)
		{
			format = CL_BLUE;
			if (bms != 0 || !bits_to_type.count(bmb))
				return false;
			if (pf.get_depth() != bmb)
				return false;
			type = bits_to_type[bmb];
		}
		else if (!rm && !gm && !bm && am)
		{
			format = CL_ALPHA;
			if (ams != 0 || !bits_to_type.count(amb))
				return false;
			if (pf.get_depth() != amb)
				return false;
			type = bits_to_type[amb];
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// OpenGL context management:

#if defined(_MSC_VER)
#define cl_tls_variable _declspec(thread)
#else
#define cl_tls_variable __thread
#endif

cl_tls_variable const CL_OpenGLGraphicContextProvider * cl_active_opengl_gc = 0;
static CL_Mutex cl_function_map_mutex;

// A fix for a compiler bug with compiler version 13.00.9466
#if _MSC_VER > 1300
typedef std::map<const CL_OpenGLGraphicContextProvider * const, CL_GLFunctions *> cl_function_map_type;
#else
typedef std::map<const CL_OpenGLGraphicContextProvider *, CL_GLFunctions *> cl_function_map_type;
#endif

static cl_function_map_type cl_function_map;

CL_GLFunctions *cl_setup_binds();

CL_ProcAddress *CL_OpenGL::get_proc_address(const CL_String8& function_name)
{
	if (cl_active_opengl_gc)
	{
		return cl_active_opengl_gc->get_proc_address(function_name);
	}
	return NULL;
}

void CL_OpenGL::set_active(CL_GraphicContext &gc)
{
	set_active(static_cast<const CL_OpenGLGraphicContextProvider *>(gc.get_provider()));
}

bool CL_OpenGL::set_active()
{
	if (cl_active_opengl_gc)	// If already active, we can exit now
		return true;

	std::vector<CL_GraphicContextProvider*> &opengl_contexts = CL_SharedGCData::get_gc_providers();
	std::vector<CL_GraphicContextProvider*>::size_type count, size;
	size = opengl_contexts.size();
	for (count = 0; count < size; count++)
	{
		CL_OpenGLGraphicContextProvider *gc_provider = dynamic_cast<CL_OpenGLGraphicContextProvider*>(opengl_contexts[count]);
		if (gc_provider)
		{
			CL_OpenGL::set_active(gc_provider);
			return true;
		}
	}
	set_active(NULL);
	return false;
}

void CL_OpenGL::set_active(const CL_OpenGLGraphicContextProvider * const gc_provider)
{
	// Don't do anything if the supplied graphic context is already active.
	if (gc_provider != cl_active_opengl_gc)
	{
		if (gc_provider)
		{
			// Make render context associated with graphic context current.
			gc_provider->get_render_window().make_current();

			// Map bound functions for this graphic context. Add to static cache if necessary.
			CL_MutexSection mutex_lock(&cl_function_map_mutex);
			cl_function_map_type::iterator it;
			it = cl_function_map.find(gc_provider);
			if (it != cl_function_map.end())
			{
				CL_OpenGL::functions = it->second;
			}
			else
			{
				cl_active_opengl_gc = gc_provider;
				CL_GLFunctions *functions = cl_setup_binds();
				cl_function_map[gc_provider] = functions;
				CL_OpenGL::functions = functions;
			}
		}

		// If no graphic context provided, set no render context current.
		else
		{
#		if defined(WIN32)
			wglMakeCurrent(NULL, NULL);
#		elif defined(__APPLE__)
			aglMakeCurrent(AGL_NONE, NULL);
#		else
			//Note: glX may not even be available. Also glXGetCurrentDisplay() may fail
			// Hopefully this will not matter!
			//glXMakeCurrent(glXGetCurrentDisplay(), None, NULL);
#		endif

			// If no current context, don't map function bindings either.
			CL_OpenGL::functions = NULL;
		}

		// OK, make our context the active one
		cl_active_opengl_gc = gc_provider;
	}
}

void CL_OpenGL::remove_active(const CL_OpenGLGraphicContextProvider * const gc_provider)
{
	CL_MutexSection mutex_lock(&cl_function_map_mutex);
	cl_function_map_type::iterator it;
	it = cl_function_map.find(gc_provider);
	if (it != cl_function_map.end())
	{
		CL_GLFunctions *functions = it->second;
		cl_function_map.erase(it);
		delete functions;

		if (cl_active_opengl_gc == gc_provider)
		{
			cl_active_opengl_gc = NULL;
		}
	}
}

CL_GLFunctions *cl_setup_binds()
{
	CL_GLFunctions *functions = new CL_GLFunctions;
	memset(functions, 0, sizeof(CL_GLFunctions));

	// Binds for OpenGL 1.1:

//#ifdef GL_VERSION_1_1
#ifdef WIN32	// Only win32 static links opengl
	functions->accum = (CL_GLFunctions::ptr_glAccum) &glAccum;
	functions->alphaFunc = (CL_GLFunctions::ptr_glAlphaFunc) &glAlphaFunc;
	functions->areTexturesResident = (CL_GLFunctions::ptr_glAreTexturesResident) &glAreTexturesResident;
	functions->arrayElement = (CL_GLFunctions::ptr_glArrayElement) &glArrayElement;
	functions->begin = (CL_GLFunctions::ptr_glBegin) &glBegin;
	functions->bindTexture = (CL_GLFunctions::ptr_glBindTexture) &glBindTexture;
	functions->bitmap = (CL_GLFunctions::ptr_glBitmap) &glBitmap;
	functions->blendFunc = (CL_GLFunctions::ptr_glBlendFunc) &glBlendFunc;
	functions->callList = (CL_GLFunctions::ptr_glCallList) &glCallList;
	functions->callLists = (CL_GLFunctions::ptr_glCallLists) &glCallLists;
	functions->clear = (CL_GLFunctions::ptr_glClear) &glClear;
	functions->clearAccum = (CL_GLFunctions::ptr_glClearAccum) &glClearAccum;
	functions->clearColor = (CL_GLFunctions::ptr_glClearColor) &glClearColor;
	functions->clearDepth = (CL_GLFunctions::ptr_glClearDepth) &glClearDepth;
	functions->clearIndex = (CL_GLFunctions::ptr_glClearIndex) &glClearIndex;
	functions->clearStencil = (CL_GLFunctions::ptr_glClearStencil) &glClearStencil;
	functions->clipPlane = (CL_GLFunctions::ptr_glClipPlane) &glClipPlane;
	functions->color3b = (CL_GLFunctions::ptr_glColor3b) &glColor3b;
	functions->color3bv = (CL_GLFunctions::ptr_glColor3bv) &glColor3bv;
	functions->color3d = (CL_GLFunctions::ptr_glColor3d) &glColor3d;
	functions->color3dv = (CL_GLFunctions::ptr_glColor3dv) &glColor3dv;
	functions->color3f = (CL_GLFunctions::ptr_glColor3f) &glColor3f;
	functions->color3fv = (CL_GLFunctions::ptr_glColor3fv) &glColor3fv;
	functions->color3i = (CL_GLFunctions::ptr_glColor3i) &glColor3i;
	functions->color3iv = (CL_GLFunctions::ptr_glColor3iv) &glColor3iv;
	functions->color3s = (CL_GLFunctions::ptr_glColor3s) &glColor3s;
	functions->color3sv = (CL_GLFunctions::ptr_glColor3sv) &glColor3sv;
	functions->color3ub = (CL_GLFunctions::ptr_glColor3ub) &glColor3ub;
	functions->color3ubv = (CL_GLFunctions::ptr_glColor3ubv) &glColor3ubv;
	functions->color3ui = (CL_GLFunctions::ptr_glColor3ui) &glColor3ui;
	functions->color3uiv = (CL_GLFunctions::ptr_glColor3uiv) &glColor3uiv;
	functions->color3us = (CL_GLFunctions::ptr_glColor3us) &glColor3us;
	functions->color3usv = (CL_GLFunctions::ptr_glColor3usv) &glColor3usv;
	functions->color4b = (CL_GLFunctions::ptr_glColor4b) &glColor4b;
	functions->color4bv = (CL_GLFunctions::ptr_glColor4bv) &glColor4bv;
	functions->color4d = (CL_GLFunctions::ptr_glColor4d) &glColor4d;
	functions->color4dv = (CL_GLFunctions::ptr_glColor4dv) &glColor4dv;
	functions->color4f = (CL_GLFunctions::ptr_glColor4f) &glColor4f;
	functions->color4fv = (CL_GLFunctions::ptr_glColor4fv) &glColor4fv;
	functions->color4i = (CL_GLFunctions::ptr_glColor4i) &glColor4i;
	functions->color4iv = (CL_GLFunctions::ptr_glColor4iv) &glColor4iv;
	functions->color4s = (CL_GLFunctions::ptr_glColor4s) &glColor4s;
	functions->color4sv = (CL_GLFunctions::ptr_glColor4sv) &glColor4sv;
	functions->color4ub = (CL_GLFunctions::ptr_glColor4ub) &glColor4ub;
	functions->color4ubv = (CL_GLFunctions::ptr_glColor4ubv) &glColor4ubv;
	functions->color4ui = (CL_GLFunctions::ptr_glColor4ui) &glColor4ui;
	functions->color4uiv = (CL_GLFunctions::ptr_glColor4uiv) &glColor4uiv;
	functions->color4us = (CL_GLFunctions::ptr_glColor4us) &glColor4us;
	functions->color4usv = (CL_GLFunctions::ptr_glColor4usv) &glColor4usv;
	functions->colorMask = (CL_GLFunctions::ptr_glColorMask) &glColorMask;
	functions->colorMaterial = (CL_GLFunctions::ptr_glColorMaterial) &glColorMaterial;
	functions->colorPointer = (CL_GLFunctions::ptr_glColorPointer) &glColorPointer;
	functions->copyPixels = (CL_GLFunctions::ptr_glCopyPixels) &glCopyPixels;
	functions->copyTexImage1D = (CL_GLFunctions::ptr_glCopyTexImage1D) &glCopyTexImage1D;
	functions->copyTexImage2D = (CL_GLFunctions::ptr_glCopyTexImage2D) &glCopyTexImage2D;
	functions->copyTexSubImage1D = (CL_GLFunctions::ptr_glCopyTexSubImage1D) &glCopyTexSubImage1D;
	functions->copyTexSubImage2D = (CL_GLFunctions::ptr_glCopyTexSubImage2D) &glCopyTexSubImage2D;
	functions->cullFace = (CL_GLFunctions::ptr_glCullFace) &glCullFace;
	functions->deleteLists = (CL_GLFunctions::ptr_glDeleteLists) &glDeleteLists;
	functions->deleteTextures = (CL_GLFunctions::ptr_glDeleteTextures) &glDeleteTextures;
	functions->depthFunc = (CL_GLFunctions::ptr_glDepthFunc) &glDepthFunc;
	functions->depthMask = (CL_GLFunctions::ptr_glDepthMask) &glDepthMask;
	functions->depthRange = (CL_GLFunctions::ptr_glDepthRange) &glDepthRange;
	functions->disable = (CL_GLFunctions::ptr_glDisable) &glDisable;
	functions->disableClientState = (CL_GLFunctions::ptr_glDisableClientState) &glDisableClientState;
	functions->drawArrays = (CL_GLFunctions::ptr_glDrawArrays) &glDrawArrays;
	functions->drawBuffer = (CL_GLFunctions::ptr_glDrawBuffer) &glDrawBuffer;
	functions->drawElements = (CL_GLFunctions::ptr_glDrawElements) &glDrawElements;
	functions->drawPixels = (CL_GLFunctions::ptr_glDrawPixels) &glDrawPixels;
	functions->edgeFlag = (CL_GLFunctions::ptr_glEdgeFlag) &glEdgeFlag;
	functions->edgeFlagPointer = (CL_GLFunctions::ptr_glEdgeFlagPointer) &glEdgeFlagPointer;
	functions->edgeFlagv = (CL_GLFunctions::ptr_glEdgeFlagv) &glEdgeFlagv;
	functions->enable = (CL_GLFunctions::ptr_glEnable) &glEnable;
	functions->enableClientState = (CL_GLFunctions::ptr_glEnableClientState) &glEnableClientState;
	functions->end = (CL_GLFunctions::ptr_glEnd) &glEnd;
	functions->endList = (CL_GLFunctions::ptr_glEndList) &glEndList;
	functions->evalCoord1d = (CL_GLFunctions::ptr_glEvalCoord1d) &glEvalCoord1d;
	functions->evalCoord1dv = (CL_GLFunctions::ptr_glEvalCoord1dv) &glEvalCoord1dv;
	functions->evalCoord1f = (CL_GLFunctions::ptr_glEvalCoord1f) &glEvalCoord1f;
	functions->evalCoord1fv = (CL_GLFunctions::ptr_glEvalCoord1fv) &glEvalCoord1fv;
	functions->evalCoord2d = (CL_GLFunctions::ptr_glEvalCoord2d) &glEvalCoord2d;
	functions->evalCoord2dv = (CL_GLFunctions::ptr_glEvalCoord2dv) &glEvalCoord2dv;
	functions->evalCoord2f = (CL_GLFunctions::ptr_glEvalCoord2f) &glEvalCoord2f;
	functions->evalCoord2fv = (CL_GLFunctions::ptr_glEvalCoord2fv) &glEvalCoord2fv;
	functions->evalMesh1 = (CL_GLFunctions::ptr_glEvalMesh1) &glEvalMesh1;
	functions->evalMesh2 = (CL_GLFunctions::ptr_glEvalMesh2) &glEvalMesh2;
	functions->evalPoint1 = (CL_GLFunctions::ptr_glEvalPoint1) &glEvalPoint1;
	functions->evalPoint2 = (CL_GLFunctions::ptr_glEvalPoint2) &glEvalPoint2;
	functions->feedbackBuffer = (CL_GLFunctions::ptr_glFeedbackBuffer) &glFeedbackBuffer;
	functions->finish = (CL_GLFunctions::ptr_glFinish) &glFinish;
	functions->flush = (CL_GLFunctions::ptr_glFlush) &glFlush;
	functions->fogf = (CL_GLFunctions::ptr_glFogf) &glFogf;
	functions->fogfv = (CL_GLFunctions::ptr_glFogfv) &glFogfv;
	functions->fogi = (CL_GLFunctions::ptr_glFogi) &glFogi;
	functions->fogiv = (CL_GLFunctions::ptr_glFogiv) &glFogiv;
	functions->frontFace = (CL_GLFunctions::ptr_glFrontFace) &glFrontFace;
	functions->frustum = (CL_GLFunctions::ptr_glFrustum) &glFrustum;
	functions->genLists = (CL_GLFunctions::ptr_glGenLists) &glGenLists;
	functions->genTextures = (CL_GLFunctions::ptr_glGenTextures) &glGenTextures;
	functions->getBooleanv = (CL_GLFunctions::ptr_glGetBooleanv) &glGetBooleanv;
	functions->getClipPlane = (CL_GLFunctions::ptr_glGetClipPlane) &glGetClipPlane;
	functions->getDoublev = (CL_GLFunctions::ptr_glGetDoublev) &glGetDoublev;
	functions->getError = (CL_GLFunctions::ptr_glGetError) &glGetError;
	functions->getFloatv = (CL_GLFunctions::ptr_glGetFloatv) &glGetFloatv;
	functions->getIntegerv = (CL_GLFunctions::ptr_glGetIntegerv) &glGetIntegerv;
	functions->getLightfv = (CL_GLFunctions::ptr_glGetLightfv) &glGetLightfv;
	functions->getLightiv = (CL_GLFunctions::ptr_glGetLightiv) &glGetLightiv;
	functions->getMapdv = (CL_GLFunctions::ptr_glGetMapdv) &glGetMapdv;
	functions->getMapfv = (CL_GLFunctions::ptr_glGetMapfv) &glGetMapfv;
	functions->getMapiv = (CL_GLFunctions::ptr_glGetMapiv) &glGetMapiv;
	functions->getMaterialfv = (CL_GLFunctions::ptr_glGetMaterialfv) &glGetMaterialfv;
	functions->getMaterialiv = (CL_GLFunctions::ptr_glGetMaterialiv) &glGetMaterialiv;
	functions->getPixelMapfv = (CL_GLFunctions::ptr_glGetPixelMapfv) &glGetPixelMapfv;
	functions->getPixelMapuiv = (CL_GLFunctions::ptr_glGetPixelMapuiv) &glGetPixelMapuiv;
	functions->getPixelMapusv = (CL_GLFunctions::ptr_glGetPixelMapusv) &glGetPixelMapusv;
	functions->getPointerv = (CL_GLFunctions::ptr_glGetPointerv) &glGetPointerv;
	functions->getPolygonStipple = (CL_GLFunctions::ptr_glGetPolygonStipple) &glGetPolygonStipple;
	functions->getString = (CL_GLFunctions::ptr_glGetString) &glGetString;
	functions->getTexEnvfv = (CL_GLFunctions::ptr_glGetTexEnvfv) &glGetTexEnvfv;
	functions->getTexEnviv = (CL_GLFunctions::ptr_glGetTexEnviv) &glGetTexEnviv;
	functions->getTexGendv = (CL_GLFunctions::ptr_glGetTexGendv) &glGetTexGendv;
	functions->getTexGenfv = (CL_GLFunctions::ptr_glGetTexGenfv) &glGetTexGenfv;
	functions->getTexGeniv = (CL_GLFunctions::ptr_glGetTexGeniv) &glGetTexGeniv;
	functions->getTexImage = (CL_GLFunctions::ptr_glGetTexImage) &glGetTexImage;
	functions->getTexLevelParameterfv = (CL_GLFunctions::ptr_glGetTexLevelParameterfv) &glGetTexLevelParameterfv;
	functions->getTexLevelParameteriv = (CL_GLFunctions::ptr_glGetTexLevelParameteriv) &glGetTexLevelParameteriv;
	functions->getTexParameterfv = (CL_GLFunctions::ptr_glGetTexParameterfv) &glGetTexParameterfv;
	functions->getTexParameteriv = (CL_GLFunctions::ptr_glGetTexParameteriv) &glGetTexParameteriv;
	functions->hint = (CL_GLFunctions::ptr_glHint) &glHint;
	functions->indexMask = (CL_GLFunctions::ptr_glIndexMask) &glIndexMask;
	functions->indexPointer = (CL_GLFunctions::ptr_glIndexPointer) &glIndexPointer;
	functions->indexd = (CL_GLFunctions::ptr_glIndexd) &glIndexd;
	functions->indexdv = (CL_GLFunctions::ptr_glIndexdv) &glIndexdv;
	functions->indexf = (CL_GLFunctions::ptr_glIndexf) &glIndexf;
	functions->indexfv = (CL_GLFunctions::ptr_glIndexfv) &glIndexfv;
	functions->indexi = (CL_GLFunctions::ptr_glIndexi) &glIndexi;
	functions->indexiv = (CL_GLFunctions::ptr_glIndexiv) &glIndexiv;
	functions->indexs = (CL_GLFunctions::ptr_glIndexs) &glIndexs;
	functions->indexsv = (CL_GLFunctions::ptr_glIndexsv) &glIndexsv;
	functions->indexub = (CL_GLFunctions::ptr_glIndexub) &glIndexub;
	functions->indexubv = (CL_GLFunctions::ptr_glIndexubv) &glIndexubv;
	functions->initNames = (CL_GLFunctions::ptr_glInitNames) &glInitNames;
	functions->interleavedArrays = (CL_GLFunctions::ptr_glInterleavedArrays) &glInterleavedArrays;
	functions->isEnabled = (CL_GLFunctions::ptr_glIsEnabled) &glIsEnabled;
	functions->isList = (CL_GLFunctions::ptr_glIsList) &glIsList;
	functions->isTexture = (CL_GLFunctions::ptr_glIsTexture) &glIsTexture;
	functions->lightModelf = (CL_GLFunctions::ptr_glLightModelf) &glLightModelf;
	functions->lightModelfv = (CL_GLFunctions::ptr_glLightModelfv) &glLightModelfv;
	functions->lightModeli = (CL_GLFunctions::ptr_glLightModeli) &glLightModeli;
	functions->lightModeliv = (CL_GLFunctions::ptr_glLightModeliv) &glLightModeliv;
	functions->lightf = (CL_GLFunctions::ptr_glLightf) &glLightf;
	functions->lightfv = (CL_GLFunctions::ptr_glLightfv) &glLightfv;
	functions->lighti = (CL_GLFunctions::ptr_glLighti) &glLighti;
	functions->lightiv = (CL_GLFunctions::ptr_glLightiv) &glLightiv;
	functions->lineStipple = (CL_GLFunctions::ptr_glLineStipple) &glLineStipple;
	functions->lineWidth = (CL_GLFunctions::ptr_glLineWidth) &glLineWidth;
	functions->listBase = (CL_GLFunctions::ptr_glListBase) &glListBase;
	functions->loadIdentity = (CL_GLFunctions::ptr_glLoadIdentity) &glLoadIdentity;
	functions->loadMatrixd = (CL_GLFunctions::ptr_glLoadMatrixd) &glLoadMatrixd;
	functions->loadMatrixf = (CL_GLFunctions::ptr_glLoadMatrixf) &glLoadMatrixf;
	functions->loadName = (CL_GLFunctions::ptr_glLoadName) &glLoadName;
	functions->logicOp = (CL_GLFunctions::ptr_glLogicOp) &glLogicOp;
	functions->map1d = (CL_GLFunctions::ptr_glMap1d) &glMap1d;
	functions->map1f = (CL_GLFunctions::ptr_glMap1f) &glMap1f;
	functions->map2d = (CL_GLFunctions::ptr_glMap2d) &glMap2d;
	functions->map2f = (CL_GLFunctions::ptr_glMap2f) &glMap2f;
	functions->mapGrid1d = (CL_GLFunctions::ptr_glMapGrid1d) &glMapGrid1d;
	functions->mapGrid1f = (CL_GLFunctions::ptr_glMapGrid1f) &glMapGrid1f;
	functions->mapGrid2d = (CL_GLFunctions::ptr_glMapGrid2d) &glMapGrid2d;
	functions->mapGrid2f = (CL_GLFunctions::ptr_glMapGrid2f) &glMapGrid2f;
	functions->materialf = (CL_GLFunctions::ptr_glMaterialf) &glMaterialf;
	functions->materialfv = (CL_GLFunctions::ptr_glMaterialfv) &glMaterialfv;
	functions->materiali = (CL_GLFunctions::ptr_glMateriali) &glMateriali;
	functions->materialiv = (CL_GLFunctions::ptr_glMaterialiv) &glMaterialiv;
	functions->matrixMode = (CL_GLFunctions::ptr_glMatrixMode) &glMatrixMode;
	functions->multMatrixd = (CL_GLFunctions::ptr_glMultMatrixd) &glMultMatrixd;
	functions->multMatrixf = (CL_GLFunctions::ptr_glMultMatrixf) &glMultMatrixf;
	functions->newList = (CL_GLFunctions::ptr_glNewList) &glNewList;
	functions->normal3b = (CL_GLFunctions::ptr_glNormal3b) &glNormal3b;
	functions->normal3bv = (CL_GLFunctions::ptr_glNormal3bv) &glNormal3bv;
	functions->normal3d = (CL_GLFunctions::ptr_glNormal3d) &glNormal3d;
	functions->normal3dv = (CL_GLFunctions::ptr_glNormal3dv) &glNormal3dv;
	functions->normal3f = (CL_GLFunctions::ptr_glNormal3f) &glNormal3f;
	functions->normal3fv = (CL_GLFunctions::ptr_glNormal3fv) &glNormal3fv;
	functions->normal3i = (CL_GLFunctions::ptr_glNormal3i) &glNormal3i;
	functions->normal3iv = (CL_GLFunctions::ptr_glNormal3iv) &glNormal3iv;
	functions->normal3s = (CL_GLFunctions::ptr_glNormal3s) &glNormal3s;
	functions->normal3sv = (CL_GLFunctions::ptr_glNormal3sv) &glNormal3sv;
	functions->normalPointer = (CL_GLFunctions::ptr_glNormalPointer) &glNormalPointer;
	functions->ortho = (CL_GLFunctions::ptr_glOrtho) &glOrtho;
	functions->passThrough = (CL_GLFunctions::ptr_glPassThrough) &glPassThrough;
	functions->pixelMapfv = (CL_GLFunctions::ptr_glPixelMapfv) &glPixelMapfv;
	functions->pixelMapuiv = (CL_GLFunctions::ptr_glPixelMapuiv) &glPixelMapuiv;
	functions->pixelMapusv = (CL_GLFunctions::ptr_glPixelMapusv) &glPixelMapusv;
	functions->pixelStoref = (CL_GLFunctions::ptr_glPixelStoref) &glPixelStoref;
	functions->pixelStorei = (CL_GLFunctions::ptr_glPixelStorei) &glPixelStorei;
	functions->pixelTransferf = (CL_GLFunctions::ptr_glPixelTransferf) &glPixelTransferf;
	functions->pixelTransferi = (CL_GLFunctions::ptr_glPixelTransferi) &glPixelTransferi;
	functions->pixelZoom = (CL_GLFunctions::ptr_glPixelZoom) &glPixelZoom;
	functions->pointSize = (CL_GLFunctions::ptr_glPointSize) &glPointSize;
	functions->polygonMode = (CL_GLFunctions::ptr_glPolygonMode) &glPolygonMode;
	functions->polygonOffset = (CL_GLFunctions::ptr_glPolygonOffset) &glPolygonOffset;
	functions->polygonStipple = (CL_GLFunctions::ptr_glPolygonStipple) &glPolygonStipple;
	functions->popAttrib = (CL_GLFunctions::ptr_glPopAttrib) &glPopAttrib;
	functions->popClientAttrib = (CL_GLFunctions::ptr_glPopClientAttrib) &glPopClientAttrib;
	functions->popMatrix = (CL_GLFunctions::ptr_glPopMatrix) &glPopMatrix;
	functions->popName = (CL_GLFunctions::ptr_glPopName) &glPopName;
	functions->prioritizeTextures = (CL_GLFunctions::ptr_glPrioritizeTextures) &glPrioritizeTextures;
	functions->pushAttrib = (CL_GLFunctions::ptr_glPushAttrib) &glPushAttrib;
	functions->pushClientAttrib = (CL_GLFunctions::ptr_glPushClientAttrib) &glPushClientAttrib;
	functions->pushMatrix = (CL_GLFunctions::ptr_glPushMatrix) &glPushMatrix;
	functions->pushName = (CL_GLFunctions::ptr_glPushName) &glPushName;
	functions->rasterPos2d = (CL_GLFunctions::ptr_glRasterPos2d) &glRasterPos2d;
	functions->rasterPos2dv = (CL_GLFunctions::ptr_glRasterPos2dv) &glRasterPos2dv;
	functions->rasterPos2f = (CL_GLFunctions::ptr_glRasterPos2f) &glRasterPos2f;
	functions->rasterPos2fv = (CL_GLFunctions::ptr_glRasterPos2fv) &glRasterPos2fv;
	functions->rasterPos2i = (CL_GLFunctions::ptr_glRasterPos2i) &glRasterPos2i;
	functions->rasterPos2iv = (CL_GLFunctions::ptr_glRasterPos2iv) &glRasterPos2iv;
	functions->rasterPos2s = (CL_GLFunctions::ptr_glRasterPos2s) &glRasterPos2s;
	functions->rasterPos2sv = (CL_GLFunctions::ptr_glRasterPos2sv) &glRasterPos2sv;
	functions->rasterPos3d = (CL_GLFunctions::ptr_glRasterPos3d) &glRasterPos3d;
	functions->rasterPos3dv = (CL_GLFunctions::ptr_glRasterPos3dv) &glRasterPos3dv;
	functions->rasterPos3f = (CL_GLFunctions::ptr_glRasterPos3f) &glRasterPos3f;
	functions->rasterPos3fv = (CL_GLFunctions::ptr_glRasterPos3fv) &glRasterPos3fv;
	functions->rasterPos3i = (CL_GLFunctions::ptr_glRasterPos3i) &glRasterPos3i;
	functions->rasterPos3iv = (CL_GLFunctions::ptr_glRasterPos3iv) &glRasterPos3iv;
	functions->rasterPos3s = (CL_GLFunctions::ptr_glRasterPos3s) &glRasterPos3s;
	functions->rasterPos3sv = (CL_GLFunctions::ptr_glRasterPos3sv) &glRasterPos3sv;
	functions->rasterPos4d = (CL_GLFunctions::ptr_glRasterPos4d) &glRasterPos4d;
	functions->rasterPos4dv = (CL_GLFunctions::ptr_glRasterPos4dv) &glRasterPos4dv;
	functions->rasterPos4f = (CL_GLFunctions::ptr_glRasterPos4f) &glRasterPos4f;
	functions->rasterPos4fv = (CL_GLFunctions::ptr_glRasterPos4fv) &glRasterPos4fv;
	functions->rasterPos4i = (CL_GLFunctions::ptr_glRasterPos4i) &glRasterPos4i;
	functions->rasterPos4iv = (CL_GLFunctions::ptr_glRasterPos4iv) &glRasterPos4iv;
	functions->rasterPos4s = (CL_GLFunctions::ptr_glRasterPos4s) &glRasterPos4s;
	functions->rasterPos4sv = (CL_GLFunctions::ptr_glRasterPos4sv) &glRasterPos4sv;
	functions->readBuffer = (CL_GLFunctions::ptr_glReadBuffer) &glReadBuffer;
	functions->readPixels = (CL_GLFunctions::ptr_glReadPixels) &glReadPixels;
	functions->rectd = (CL_GLFunctions::ptr_glRectd) &glRectd;
	functions->rectdv = (CL_GLFunctions::ptr_glRectdv) &glRectdv;
	functions->rectf = (CL_GLFunctions::ptr_glRectf) &glRectf;
	functions->rectfv = (CL_GLFunctions::ptr_glRectfv) &glRectfv;
	functions->recti = (CL_GLFunctions::ptr_glRecti) &glRecti;
	functions->rectiv = (CL_GLFunctions::ptr_glRectiv) &glRectiv;
	functions->rects = (CL_GLFunctions::ptr_glRects) &glRects;
	functions->rectsv = (CL_GLFunctions::ptr_glRectsv) &glRectsv;
	functions->renderMode = (CL_GLFunctions::ptr_glRenderMode) &glRenderMode;
	functions->rotated = (CL_GLFunctions::ptr_glRotated) &glRotated;
	functions->rotatef = (CL_GLFunctions::ptr_glRotatef) &glRotatef;
	functions->scaled = (CL_GLFunctions::ptr_glScaled) &glScaled;
	functions->scalef = (CL_GLFunctions::ptr_glScalef) &glScalef;
	functions->scissor = (CL_GLFunctions::ptr_glScissor) &glScissor;
	functions->selectBuffer = (CL_GLFunctions::ptr_glSelectBuffer) &glSelectBuffer;
	functions->shadeModel = (CL_GLFunctions::ptr_glShadeModel) &glShadeModel;
	functions->stencilFunc = (CL_GLFunctions::ptr_glStencilFunc) &glStencilFunc;
	functions->stencilMask = (CL_GLFunctions::ptr_glStencilMask) &glStencilMask;
	functions->stencilOp = (CL_GLFunctions::ptr_glStencilOp) &glStencilOp;
	functions->texCoord1d = (CL_GLFunctions::ptr_glTexCoord1d) &glTexCoord1d;
	functions->texCoord1dv = (CL_GLFunctions::ptr_glTexCoord1dv) &glTexCoord1dv;
	functions->texCoord1f = (CL_GLFunctions::ptr_glTexCoord1f) &glTexCoord1f;
	functions->texCoord1fv = (CL_GLFunctions::ptr_glTexCoord1fv) &glTexCoord1fv;
	functions->texCoord1i = (CL_GLFunctions::ptr_glTexCoord1i) &glTexCoord1i;
	functions->texCoord1iv = (CL_GLFunctions::ptr_glTexCoord1iv) &glTexCoord1iv;
	functions->texCoord1s = (CL_GLFunctions::ptr_glTexCoord1s) &glTexCoord1s;
	functions->texCoord1sv = (CL_GLFunctions::ptr_glTexCoord1sv) &glTexCoord1sv;
	functions->texCoord2d = (CL_GLFunctions::ptr_glTexCoord2d) &glTexCoord2d;
	functions->texCoord2dv = (CL_GLFunctions::ptr_glTexCoord2dv) &glTexCoord2dv;
	functions->texCoord2f = (CL_GLFunctions::ptr_glTexCoord2f) &glTexCoord2f;
	functions->texCoord2fv = (CL_GLFunctions::ptr_glTexCoord2fv) &glTexCoord2fv;
	functions->texCoord2i = (CL_GLFunctions::ptr_glTexCoord2i) &glTexCoord2i;
	functions->texCoord2iv = (CL_GLFunctions::ptr_glTexCoord2iv) &glTexCoord2iv;
	functions->texCoord2s = (CL_GLFunctions::ptr_glTexCoord2s) &glTexCoord2s;
	functions->texCoord2sv = (CL_GLFunctions::ptr_glTexCoord2sv) &glTexCoord2sv;
	functions->texCoord3d = (CL_GLFunctions::ptr_glTexCoord3d) &glTexCoord3d;
	functions->texCoord3dv = (CL_GLFunctions::ptr_glTexCoord3dv) &glTexCoord3dv;
	functions->texCoord3f = (CL_GLFunctions::ptr_glTexCoord3f) &glTexCoord3f;
	functions->texCoord3fv = (CL_GLFunctions::ptr_glTexCoord3fv) &glTexCoord3fv;
	functions->texCoord3i = (CL_GLFunctions::ptr_glTexCoord3i) &glTexCoord3i;
	functions->texCoord3iv = (CL_GLFunctions::ptr_glTexCoord3iv) &glTexCoord3iv;
	functions->texCoord3s = (CL_GLFunctions::ptr_glTexCoord3s) &glTexCoord3s;
	functions->texCoord3sv = (CL_GLFunctions::ptr_glTexCoord3sv) &glTexCoord3sv;
	functions->texCoord4d = (CL_GLFunctions::ptr_glTexCoord4d) &glTexCoord4d;
	functions->texCoord4dv = (CL_GLFunctions::ptr_glTexCoord4dv) &glTexCoord4dv;
	functions->texCoord4f = (CL_GLFunctions::ptr_glTexCoord4f) &glTexCoord4f;
	functions->texCoord4fv = (CL_GLFunctions::ptr_glTexCoord4fv) &glTexCoord4fv;
	functions->texCoord4i = (CL_GLFunctions::ptr_glTexCoord4i) &glTexCoord4i;
	functions->texCoord4iv = (CL_GLFunctions::ptr_glTexCoord4iv) &glTexCoord4iv;
	functions->texCoord4s = (CL_GLFunctions::ptr_glTexCoord4s) &glTexCoord4s;
	functions->texCoord4sv = (CL_GLFunctions::ptr_glTexCoord4sv) &glTexCoord4sv;
	functions->texCoordPointer = (CL_GLFunctions::ptr_glTexCoordPointer) &glTexCoordPointer;
	functions->texEnvf = (CL_GLFunctions::ptr_glTexEnvf) &glTexEnvf;
	functions->texEnvfv = (CL_GLFunctions::ptr_glTexEnvfv) &glTexEnvfv;
	functions->texEnvi = (CL_GLFunctions::ptr_glTexEnvi) &glTexEnvi;
	functions->texEnviv = (CL_GLFunctions::ptr_glTexEnviv) &glTexEnviv;
	functions->texGend = (CL_GLFunctions::ptr_glTexGend) &glTexGend;
	functions->texGendv = (CL_GLFunctions::ptr_glTexGendv) &glTexGendv;
	functions->texGenf = (CL_GLFunctions::ptr_glTexGenf) &glTexGenf;
	functions->texGenfv = (CL_GLFunctions::ptr_glTexGenfv) &glTexGenfv;
	functions->texGeni = (CL_GLFunctions::ptr_glTexGeni) &glTexGeni;
	functions->texGeniv = (CL_GLFunctions::ptr_glTexGeniv) &glTexGeniv;
	functions->texImage1D = (CL_GLFunctions::ptr_glTexImage1D) &glTexImage1D;
	functions->texImage2D = (CL_GLFunctions::ptr_glTexImage2D) &glTexImage2D;
	functions->texParameterf = (CL_GLFunctions::ptr_glTexParameterf) &glTexParameterf;
	functions->texParameterfv = (CL_GLFunctions::ptr_glTexParameterfv) &glTexParameterfv;
	functions->texParameteri = (CL_GLFunctions::ptr_glTexParameteri) &glTexParameteri;
	functions->texParameteriv = (CL_GLFunctions::ptr_glTexParameteriv) &glTexParameteriv;
	functions->texSubImage1D = (CL_GLFunctions::ptr_glTexSubImage1D) &glTexSubImage1D;
	functions->texSubImage2D = (CL_GLFunctions::ptr_glTexSubImage2D) &glTexSubImage2D;
	functions->translated = (CL_GLFunctions::ptr_glTranslated) &glTranslated;
	functions->translatef = (CL_GLFunctions::ptr_glTranslatef) &glTranslatef;
	functions->vertex2d = (CL_GLFunctions::ptr_glVertex2d) &glVertex2d;
	functions->vertex2dv = (CL_GLFunctions::ptr_glVertex2dv) &glVertex2dv;
	functions->vertex2f = (CL_GLFunctions::ptr_glVertex2f) &glVertex2f;
	functions->vertex2fv = (CL_GLFunctions::ptr_glVertex2fv) &glVertex2fv;
	functions->vertex2i = (CL_GLFunctions::ptr_glVertex2i) &glVertex2i;
	functions->vertex2iv = (CL_GLFunctions::ptr_glVertex2iv) &glVertex2iv;
	functions->vertex2s = (CL_GLFunctions::ptr_glVertex2s) &glVertex2s;
	functions->vertex2sv = (CL_GLFunctions::ptr_glVertex2sv) &glVertex2sv;
	functions->vertex3d = (CL_GLFunctions::ptr_glVertex3d) &glVertex3d;
	functions->vertex3dv = (CL_GLFunctions::ptr_glVertex3dv) &glVertex3dv;
	functions->vertex3f = (CL_GLFunctions::ptr_glVertex3f) &glVertex3f;
	functions->vertex3fv = (CL_GLFunctions::ptr_glVertex3fv) &glVertex3fv;
	functions->vertex3i = (CL_GLFunctions::ptr_glVertex3i) &glVertex3i;
	functions->vertex3iv = (CL_GLFunctions::ptr_glVertex3iv) &glVertex3iv;
	functions->vertex3s = (CL_GLFunctions::ptr_glVertex3s) &glVertex3s;
	functions->vertex3sv = (CL_GLFunctions::ptr_glVertex3sv) &glVertex3sv;
	functions->vertex4d = (CL_GLFunctions::ptr_glVertex4d) &glVertex4d;
	functions->vertex4dv = (CL_GLFunctions::ptr_glVertex4dv) &glVertex4dv;
	functions->vertex4f = (CL_GLFunctions::ptr_glVertex4f) &glVertex4f;
	functions->vertex4fv = (CL_GLFunctions::ptr_glVertex4fv) &glVertex4fv;
	functions->vertex4i = (CL_GLFunctions::ptr_glVertex4i) &glVertex4i;
	functions->vertex4iv = (CL_GLFunctions::ptr_glVertex4iv) &glVertex4iv;
	functions->vertex4s = (CL_GLFunctions::ptr_glVertex4s) &glVertex4s;
	functions->vertex4sv = (CL_GLFunctions::ptr_glVertex4sv) &glVertex4sv;
	functions->vertexPointer = (CL_GLFunctions::ptr_glVertexPointer) &glVertexPointer;
	functions->viewport = (CL_GLFunctions::ptr_glViewport) &glViewport;
#else
	functions->accum = (CL_GLFunctions::ptr_glAccum) CL_OpenGL::get_proc_address("glAccum");
	functions->alphaFunc = (CL_GLFunctions::ptr_glAlphaFunc) CL_OpenGL::get_proc_address("glAlphaFunc");
	functions->areTexturesResident = (CL_GLFunctions::ptr_glAreTexturesResident) CL_OpenGL::get_proc_address("glAreTexturesResident");
	functions->arrayElement = (CL_GLFunctions::ptr_glArrayElement) CL_OpenGL::get_proc_address("glArrayElement");
	functions->begin = (CL_GLFunctions::ptr_glBegin) CL_OpenGL::get_proc_address("glBegin");
	functions->bindTexture = (CL_GLFunctions::ptr_glBindTexture) CL_OpenGL::get_proc_address("glBindTexture");
	functions->bitmap = (CL_GLFunctions::ptr_glBitmap) CL_OpenGL::get_proc_address("glBitmap");
	functions->blendFunc = (CL_GLFunctions::ptr_glBlendFunc) CL_OpenGL::get_proc_address("glBlendFunc");
	functions->callList = (CL_GLFunctions::ptr_glCallList) CL_OpenGL::get_proc_address("glCallList");
	functions->callLists = (CL_GLFunctions::ptr_glCallLists) CL_OpenGL::get_proc_address("glCallLists");
	functions->clear = (CL_GLFunctions::ptr_glClear) CL_OpenGL::get_proc_address("glClear");
	functions->clearAccum = (CL_GLFunctions::ptr_glClearAccum) CL_OpenGL::get_proc_address("glClearAccum");
	functions->clearColor = (CL_GLFunctions::ptr_glClearColor) CL_OpenGL::get_proc_address("glClearColor");
	functions->clearDepth = (CL_GLFunctions::ptr_glClearDepth) CL_OpenGL::get_proc_address("glClearDepth");
	functions->clearIndex = (CL_GLFunctions::ptr_glClearIndex) CL_OpenGL::get_proc_address("glClearIndex");
	functions->clearStencil = (CL_GLFunctions::ptr_glClearStencil) CL_OpenGL::get_proc_address("glClearStencil");
	functions->clipPlane = (CL_GLFunctions::ptr_glClipPlane) CL_OpenGL::get_proc_address("glClipPlane");
	functions->color3b = (CL_GLFunctions::ptr_glColor3b) CL_OpenGL::get_proc_address("glColor3b");
	functions->color3bv = (CL_GLFunctions::ptr_glColor3bv) CL_OpenGL::get_proc_address("glColor3bv");
	functions->color3d = (CL_GLFunctions::ptr_glColor3d) CL_OpenGL::get_proc_address("glColor3d");
	functions->color3dv = (CL_GLFunctions::ptr_glColor3dv) CL_OpenGL::get_proc_address("glColor3dv");
	functions->color3f = (CL_GLFunctions::ptr_glColor3f) CL_OpenGL::get_proc_address("glColor3f");
	functions->color3fv = (CL_GLFunctions::ptr_glColor3fv) CL_OpenGL::get_proc_address("glColor3fv");
	functions->color3i = (CL_GLFunctions::ptr_glColor3i) CL_OpenGL::get_proc_address("glColor3i");
	functions->color3iv = (CL_GLFunctions::ptr_glColor3iv) CL_OpenGL::get_proc_address("glColor3iv");
	functions->color3s = (CL_GLFunctions::ptr_glColor3s) CL_OpenGL::get_proc_address("glColor3s");
	functions->color3sv = (CL_GLFunctions::ptr_glColor3sv) CL_OpenGL::get_proc_address("glColor3sv");
	functions->color3ub = (CL_GLFunctions::ptr_glColor3ub) CL_OpenGL::get_proc_address("glColor3ub");
	functions->color3ubv = (CL_GLFunctions::ptr_glColor3ubv) CL_OpenGL::get_proc_address("glColor3ubv");
	functions->color3ui = (CL_GLFunctions::ptr_glColor3ui) CL_OpenGL::get_proc_address("glColor3ui");
	functions->color3uiv = (CL_GLFunctions::ptr_glColor3uiv) CL_OpenGL::get_proc_address("glColor3uiv");
	functions->color3us = (CL_GLFunctions::ptr_glColor3us) CL_OpenGL::get_proc_address("glColor3us");
	functions->color3usv = (CL_GLFunctions::ptr_glColor3usv) CL_OpenGL::get_proc_address("glColor3usv");
	functions->color4b = (CL_GLFunctions::ptr_glColor4b) CL_OpenGL::get_proc_address("glColor4b");
	functions->color4bv = (CL_GLFunctions::ptr_glColor4bv) CL_OpenGL::get_proc_address("glColor4bv");
	functions->color4d = (CL_GLFunctions::ptr_glColor4d) CL_OpenGL::get_proc_address("glColor4d");
	functions->color4dv = (CL_GLFunctions::ptr_glColor4dv) CL_OpenGL::get_proc_address("glColor4dv");
	functions->color4f = (CL_GLFunctions::ptr_glColor4f) CL_OpenGL::get_proc_address("glColor4f");
	functions->color4fv = (CL_GLFunctions::ptr_glColor4fv) CL_OpenGL::get_proc_address("glColor4fv");
	functions->color4i = (CL_GLFunctions::ptr_glColor4i) CL_OpenGL::get_proc_address("glColor4i");
	functions->color4iv = (CL_GLFunctions::ptr_glColor4iv) CL_OpenGL::get_proc_address("glColor4iv");
	functions->color4s = (CL_GLFunctions::ptr_glColor4s) CL_OpenGL::get_proc_address("glColor4s");
	functions->color4sv = (CL_GLFunctions::ptr_glColor4sv) CL_OpenGL::get_proc_address("glColor4sv");
	functions->color4ub = (CL_GLFunctions::ptr_glColor4ub) CL_OpenGL::get_proc_address("glColor4ub");
	functions->color4ubv = (CL_GLFunctions::ptr_glColor4ubv) CL_OpenGL::get_proc_address("glColor4ubv");
	functions->color4ui = (CL_GLFunctions::ptr_glColor4ui) CL_OpenGL::get_proc_address("glColor4ui");
	functions->color4uiv = (CL_GLFunctions::ptr_glColor4uiv) CL_OpenGL::get_proc_address("glColor4uiv");
	functions->color4us = (CL_GLFunctions::ptr_glColor4us) CL_OpenGL::get_proc_address("glColor4us");
	functions->color4usv = (CL_GLFunctions::ptr_glColor4usv) CL_OpenGL::get_proc_address("glColor4usv");
	functions->colorMask = (CL_GLFunctions::ptr_glColorMask) CL_OpenGL::get_proc_address("glColorMask");
	functions->colorMaterial = (CL_GLFunctions::ptr_glColorMaterial) CL_OpenGL::get_proc_address("glColorMaterial");
	functions->colorPointer = (CL_GLFunctions::ptr_glColorPointer) CL_OpenGL::get_proc_address("glColorPointer");
	functions->copyPixels = (CL_GLFunctions::ptr_glCopyPixels) CL_OpenGL::get_proc_address("glCopyPixels");
	functions->copyTexImage1D = (CL_GLFunctions::ptr_glCopyTexImage1D) CL_OpenGL::get_proc_address("glCopyTexImage1D");
	functions->copyTexImage2D = (CL_GLFunctions::ptr_glCopyTexImage2D) CL_OpenGL::get_proc_address("glCopyTexImage2D");
	functions->copyTexSubImage1D = (CL_GLFunctions::ptr_glCopyTexSubImage1D) CL_OpenGL::get_proc_address("glCopyTexSubImage1D");
	functions->copyTexSubImage2D = (CL_GLFunctions::ptr_glCopyTexSubImage2D) CL_OpenGL::get_proc_address("glCopyTexSubImage2D");
	functions->cullFace = (CL_GLFunctions::ptr_glCullFace) CL_OpenGL::get_proc_address("glCullFace");
	functions->deleteLists = (CL_GLFunctions::ptr_glDeleteLists) CL_OpenGL::get_proc_address("glDeleteLists");
	functions->deleteTextures = (CL_GLFunctions::ptr_glDeleteTextures) CL_OpenGL::get_proc_address("glDeleteTextures");
	functions->depthFunc = (CL_GLFunctions::ptr_glDepthFunc) CL_OpenGL::get_proc_address("glDepthFunc");
	functions->depthMask = (CL_GLFunctions::ptr_glDepthMask) CL_OpenGL::get_proc_address("glDepthMask");
	functions->depthRange = (CL_GLFunctions::ptr_glDepthRange) CL_OpenGL::get_proc_address("glDepthRange");
	functions->disable = (CL_GLFunctions::ptr_glDisable) CL_OpenGL::get_proc_address("glDisable");
	functions->disableClientState = (CL_GLFunctions::ptr_glDisableClientState) CL_OpenGL::get_proc_address("glDisableClientState");
	functions->drawArrays = (CL_GLFunctions::ptr_glDrawArrays) CL_OpenGL::get_proc_address("glDrawArrays");
	functions->drawBuffer = (CL_GLFunctions::ptr_glDrawBuffer) CL_OpenGL::get_proc_address("glDrawBuffer");
	functions->drawElements = (CL_GLFunctions::ptr_glDrawElements) CL_OpenGL::get_proc_address("glDrawElements");
	functions->drawPixels = (CL_GLFunctions::ptr_glDrawPixels) CL_OpenGL::get_proc_address("glDrawPixels");
	functions->edgeFlag = (CL_GLFunctions::ptr_glEdgeFlag) CL_OpenGL::get_proc_address("glEdgeFlag");
	functions->edgeFlagPointer = (CL_GLFunctions::ptr_glEdgeFlagPointer) CL_OpenGL::get_proc_address("glEdgeFlagPointer");
	functions->edgeFlagv = (CL_GLFunctions::ptr_glEdgeFlagv) CL_OpenGL::get_proc_address("glEdgeFlagv");
	functions->enable = (CL_GLFunctions::ptr_glEnable) CL_OpenGL::get_proc_address("glEnable");
	functions->enableClientState = (CL_GLFunctions::ptr_glEnableClientState) CL_OpenGL::get_proc_address("glEnableClientState");
	functions->end = (CL_GLFunctions::ptr_glEnd) CL_OpenGL::get_proc_address("glEnd");
	functions->endList = (CL_GLFunctions::ptr_glEndList) CL_OpenGL::get_proc_address("glEndList");
	functions->evalCoord1d = (CL_GLFunctions::ptr_glEvalCoord1d) CL_OpenGL::get_proc_address("glEvalCoord1d");
	functions->evalCoord1dv = (CL_GLFunctions::ptr_glEvalCoord1dv) CL_OpenGL::get_proc_address("glEvalCoord1dv");
	functions->evalCoord1f = (CL_GLFunctions::ptr_glEvalCoord1f) CL_OpenGL::get_proc_address("glEvalCoord1f");
	functions->evalCoord1fv = (CL_GLFunctions::ptr_glEvalCoord1fv) CL_OpenGL::get_proc_address("glEvalCoord1fv");
	functions->evalCoord2d = (CL_GLFunctions::ptr_glEvalCoord2d) CL_OpenGL::get_proc_address("glEvalCoord2d");
	functions->evalCoord2dv = (CL_GLFunctions::ptr_glEvalCoord2dv) CL_OpenGL::get_proc_address("glEvalCoord2dv");
	functions->evalCoord2f = (CL_GLFunctions::ptr_glEvalCoord2f) CL_OpenGL::get_proc_address("glEvalCoord2f");
	functions->evalCoord2fv = (CL_GLFunctions::ptr_glEvalCoord2fv) CL_OpenGL::get_proc_address("glEvalCoord2fv");
	functions->evalMesh1 = (CL_GLFunctions::ptr_glEvalMesh1) CL_OpenGL::get_proc_address("glEvalMesh1");
	functions->evalMesh2 = (CL_GLFunctions::ptr_glEvalMesh2) CL_OpenGL::get_proc_address("glEvalMesh2");
	functions->evalPoint1 = (CL_GLFunctions::ptr_glEvalPoint1) CL_OpenGL::get_proc_address("glEvalPoint1");
	functions->evalPoint2 = (CL_GLFunctions::ptr_glEvalPoint2) CL_OpenGL::get_proc_address("glEvalPoint2");
	functions->feedbackBuffer = (CL_GLFunctions::ptr_glFeedbackBuffer) CL_OpenGL::get_proc_address("glFeedbackBuffer");
	functions->finish = (CL_GLFunctions::ptr_glFinish) CL_OpenGL::get_proc_address("glFinish");
	functions->flush = (CL_GLFunctions::ptr_glFlush) CL_OpenGL::get_proc_address("glFlush");
	functions->fogf = (CL_GLFunctions::ptr_glFogf) CL_OpenGL::get_proc_address("glFogf");
	functions->fogfv = (CL_GLFunctions::ptr_glFogfv) CL_OpenGL::get_proc_address("glFogfv");
	functions->fogi = (CL_GLFunctions::ptr_glFogi) CL_OpenGL::get_proc_address("glFogi");
	functions->fogiv = (CL_GLFunctions::ptr_glFogiv) CL_OpenGL::get_proc_address("glFogiv");
	functions->frontFace = (CL_GLFunctions::ptr_glFrontFace) CL_OpenGL::get_proc_address("glFrontFace");
	functions->frustum = (CL_GLFunctions::ptr_glFrustum) CL_OpenGL::get_proc_address("glFrustum");
	functions->genLists = (CL_GLFunctions::ptr_glGenLists) CL_OpenGL::get_proc_address("glGenLists");
	functions->genTextures = (CL_GLFunctions::ptr_glGenTextures) CL_OpenGL::get_proc_address("glGenTextures");
	functions->getBooleanv = (CL_GLFunctions::ptr_glGetBooleanv) CL_OpenGL::get_proc_address("glGetBooleanv");
	functions->getClipPlane = (CL_GLFunctions::ptr_glGetClipPlane) CL_OpenGL::get_proc_address("glGetClipPlane");
	functions->getDoublev = (CL_GLFunctions::ptr_glGetDoublev) CL_OpenGL::get_proc_address("glGetDoublev");
	functions->getError = (CL_GLFunctions::ptr_glGetError) CL_OpenGL::get_proc_address("glGetError");
	functions->getFloatv = (CL_GLFunctions::ptr_glGetFloatv) CL_OpenGL::get_proc_address("glGetFloatv");
	functions->getIntegerv = (CL_GLFunctions::ptr_glGetIntegerv) CL_OpenGL::get_proc_address("glGetIntegerv");
	functions->getLightfv = (CL_GLFunctions::ptr_glGetLightfv) CL_OpenGL::get_proc_address("glGetLightfv");
	functions->getLightiv = (CL_GLFunctions::ptr_glGetLightiv) CL_OpenGL::get_proc_address("glGetLightiv");
	functions->getMapdv = (CL_GLFunctions::ptr_glGetMapdv) CL_OpenGL::get_proc_address("glGetMapdv");
	functions->getMapfv = (CL_GLFunctions::ptr_glGetMapfv) CL_OpenGL::get_proc_address("glGetMapfv");
	functions->getMapiv = (CL_GLFunctions::ptr_glGetMapiv) CL_OpenGL::get_proc_address("glGetMapiv");
	functions->getMaterialfv = (CL_GLFunctions::ptr_glGetMaterialfv) CL_OpenGL::get_proc_address("glGetMaterialfv");
	functions->getMaterialiv = (CL_GLFunctions::ptr_glGetMaterialiv) CL_OpenGL::get_proc_address("glGetMaterialiv");
	functions->getPixelMapfv = (CL_GLFunctions::ptr_glGetPixelMapfv) CL_OpenGL::get_proc_address("glGetPixelMapfv");
	functions->getPixelMapuiv = (CL_GLFunctions::ptr_glGetPixelMapuiv) CL_OpenGL::get_proc_address("glGetPixelMapuiv");
	functions->getPixelMapusv = (CL_GLFunctions::ptr_glGetPixelMapusv) CL_OpenGL::get_proc_address("glGetPixelMapusv");
	functions->getPointerv = (CL_GLFunctions::ptr_glGetPointerv) CL_OpenGL::get_proc_address("glGetPointerv");
	functions->getPolygonStipple = (CL_GLFunctions::ptr_glGetPolygonStipple) CL_OpenGL::get_proc_address("glGetPolygonStipple");
	functions->getString = (CL_GLFunctions::ptr_glGetString) CL_OpenGL::get_proc_address("glGetString");
	functions->getTexEnvfv = (CL_GLFunctions::ptr_glGetTexEnvfv) CL_OpenGL::get_proc_address("glGetTexEnvfv");
	functions->getTexEnviv = (CL_GLFunctions::ptr_glGetTexEnviv) CL_OpenGL::get_proc_address("glGetTexEnviv");
	functions->getTexGendv = (CL_GLFunctions::ptr_glGetTexGendv) CL_OpenGL::get_proc_address("glGetTexGendv");
	functions->getTexGenfv = (CL_GLFunctions::ptr_glGetTexGenfv) CL_OpenGL::get_proc_address("glGetTexGenfv");
	functions->getTexGeniv = (CL_GLFunctions::ptr_glGetTexGeniv) CL_OpenGL::get_proc_address("glGetTexGeniv");
	functions->getTexImage = (CL_GLFunctions::ptr_glGetTexImage) CL_OpenGL::get_proc_address("glGetTexImage");
	functions->getTexLevelParameterfv = (CL_GLFunctions::ptr_glGetTexLevelParameterfv) CL_OpenGL::get_proc_address("glGetTexLevelParameterfv");
	functions->getTexLevelParameteriv = (CL_GLFunctions::ptr_glGetTexLevelParameteriv) CL_OpenGL::get_proc_address("glGetTexLevelParameteriv");
	functions->getTexParameterfv = (CL_GLFunctions::ptr_glGetTexParameterfv) CL_OpenGL::get_proc_address("glGetTexParameterfv");
	functions->getTexParameteriv = (CL_GLFunctions::ptr_glGetTexParameteriv) CL_OpenGL::get_proc_address("glGetTexParameteriv");
	functions->hint = (CL_GLFunctions::ptr_glHint) CL_OpenGL::get_proc_address("glHint");
	functions->indexMask = (CL_GLFunctions::ptr_glIndexMask) CL_OpenGL::get_proc_address("glIndexMask");
	functions->indexPointer = (CL_GLFunctions::ptr_glIndexPointer) CL_OpenGL::get_proc_address("glIndexPointer");
	functions->indexd = (CL_GLFunctions::ptr_glIndexd) CL_OpenGL::get_proc_address("glIndexd");
	functions->indexdv = (CL_GLFunctions::ptr_glIndexdv) CL_OpenGL::get_proc_address("glIndexdv");
	functions->indexf = (CL_GLFunctions::ptr_glIndexf) CL_OpenGL::get_proc_address("glIndexf");
	functions->indexfv = (CL_GLFunctions::ptr_glIndexfv) CL_OpenGL::get_proc_address("glIndexfv");
	functions->indexi = (CL_GLFunctions::ptr_glIndexi) CL_OpenGL::get_proc_address("glIndexi");
	functions->indexiv = (CL_GLFunctions::ptr_glIndexiv) CL_OpenGL::get_proc_address("glIndexiv");
	functions->indexs = (CL_GLFunctions::ptr_glIndexs) CL_OpenGL::get_proc_address("glIndexs");
	functions->indexsv = (CL_GLFunctions::ptr_glIndexsv) CL_OpenGL::get_proc_address("glIndexsv");
	functions->indexub = (CL_GLFunctions::ptr_glIndexub) CL_OpenGL::get_proc_address("glIndexub");
	functions->indexubv = (CL_GLFunctions::ptr_glIndexubv) CL_OpenGL::get_proc_address("glIndexubv");
	functions->initNames = (CL_GLFunctions::ptr_glInitNames) CL_OpenGL::get_proc_address("glInitNames");
	functions->interleavedArrays = (CL_GLFunctions::ptr_glInterleavedArrays) CL_OpenGL::get_proc_address("glInterleavedArrays");
	functions->isEnabled = (CL_GLFunctions::ptr_glIsEnabled) CL_OpenGL::get_proc_address("glIsEnabled");
	functions->isList = (CL_GLFunctions::ptr_glIsList) CL_OpenGL::get_proc_address("glIsList");
	functions->isTexture = (CL_GLFunctions::ptr_glIsTexture) CL_OpenGL::get_proc_address("glIsTexture");
	functions->lightModelf = (CL_GLFunctions::ptr_glLightModelf) CL_OpenGL::get_proc_address("glLightModelf");
	functions->lightModelfv = (CL_GLFunctions::ptr_glLightModelfv) CL_OpenGL::get_proc_address("glLightModelfv");
	functions->lightModeli = (CL_GLFunctions::ptr_glLightModeli) CL_OpenGL::get_proc_address("glLightModeli");
	functions->lightModeliv = (CL_GLFunctions::ptr_glLightModeliv) CL_OpenGL::get_proc_address("glLightModeliv");
	functions->lightf = (CL_GLFunctions::ptr_glLightf) CL_OpenGL::get_proc_address("glLightf");
	functions->lightfv = (CL_GLFunctions::ptr_glLightfv) CL_OpenGL::get_proc_address("glLightfv");
	functions->lighti = (CL_GLFunctions::ptr_glLighti) CL_OpenGL::get_proc_address("glLighti");
	functions->lightiv = (CL_GLFunctions::ptr_glLightiv) CL_OpenGL::get_proc_address("glLightiv");
	functions->lineStipple = (CL_GLFunctions::ptr_glLineStipple) CL_OpenGL::get_proc_address("glLineStipple");
	functions->lineWidth = (CL_GLFunctions::ptr_glLineWidth) CL_OpenGL::get_proc_address("glLineWidth");
	functions->listBase = (CL_GLFunctions::ptr_glListBase) CL_OpenGL::get_proc_address("glListBase");
	functions->loadIdentity = (CL_GLFunctions::ptr_glLoadIdentity) CL_OpenGL::get_proc_address("glLoadIdentity");
	functions->loadMatrixd = (CL_GLFunctions::ptr_glLoadMatrixd) CL_OpenGL::get_proc_address("glLoadMatrixd");
	functions->loadMatrixf = (CL_GLFunctions::ptr_glLoadMatrixf) CL_OpenGL::get_proc_address("glLoadMatrixf");
	functions->loadName = (CL_GLFunctions::ptr_glLoadName) CL_OpenGL::get_proc_address("glLoadName");
	functions->logicOp = (CL_GLFunctions::ptr_glLogicOp) CL_OpenGL::get_proc_address("glLogicOp");
	functions->map1d = (CL_GLFunctions::ptr_glMap1d) CL_OpenGL::get_proc_address("glMap1d");
	functions->map1f = (CL_GLFunctions::ptr_glMap1f) CL_OpenGL::get_proc_address("glMap1f");
	functions->map2d = (CL_GLFunctions::ptr_glMap2d) CL_OpenGL::get_proc_address("glMap2d");
	functions->map2f = (CL_GLFunctions::ptr_glMap2f) CL_OpenGL::get_proc_address("glMap2f");
	functions->mapGrid1d = (CL_GLFunctions::ptr_glMapGrid1d) CL_OpenGL::get_proc_address("glMapGrid1d");
	functions->mapGrid1f = (CL_GLFunctions::ptr_glMapGrid1f) CL_OpenGL::get_proc_address("glMapGrid1f");
	functions->mapGrid2d = (CL_GLFunctions::ptr_glMapGrid2d) CL_OpenGL::get_proc_address("glMapGrid2d");
	functions->mapGrid2f = (CL_GLFunctions::ptr_glMapGrid2f) CL_OpenGL::get_proc_address("glMapGrid2f");
	functions->materialf = (CL_GLFunctions::ptr_glMaterialf) CL_OpenGL::get_proc_address("glMaterialf");
	functions->materialfv = (CL_GLFunctions::ptr_glMaterialfv) CL_OpenGL::get_proc_address("glMaterialfv");
	functions->materiali = (CL_GLFunctions::ptr_glMateriali) CL_OpenGL::get_proc_address("glMateriali");
	functions->materialiv = (CL_GLFunctions::ptr_glMaterialiv) CL_OpenGL::get_proc_address("glMaterialiv");
	functions->matrixMode = (CL_GLFunctions::ptr_glMatrixMode) CL_OpenGL::get_proc_address("glMatrixMode");
	functions->multMatrixd = (CL_GLFunctions::ptr_glMultMatrixd) CL_OpenGL::get_proc_address("glMultMatrixd");
	functions->multMatrixf = (CL_GLFunctions::ptr_glMultMatrixf) CL_OpenGL::get_proc_address("glMultMatrixf");
	functions->newList = (CL_GLFunctions::ptr_glNewList) CL_OpenGL::get_proc_address("glNewList");
	functions->normal3b = (CL_GLFunctions::ptr_glNormal3b) CL_OpenGL::get_proc_address("glNormal3b");
	functions->normal3bv = (CL_GLFunctions::ptr_glNormal3bv) CL_OpenGL::get_proc_address("glNormal3bv");
	functions->normal3d = (CL_GLFunctions::ptr_glNormal3d) CL_OpenGL::get_proc_address("glNormal3d");
	functions->normal3dv = (CL_GLFunctions::ptr_glNormal3dv) CL_OpenGL::get_proc_address("glNormal3dv");
	functions->normal3f = (CL_GLFunctions::ptr_glNormal3f) CL_OpenGL::get_proc_address("glNormal3f");
	functions->normal3fv = (CL_GLFunctions::ptr_glNormal3fv) CL_OpenGL::get_proc_address("glNormal3fv");
	functions->normal3i = (CL_GLFunctions::ptr_glNormal3i) CL_OpenGL::get_proc_address("glNormal3i");
	functions->normal3iv = (CL_GLFunctions::ptr_glNormal3iv) CL_OpenGL::get_proc_address("glNormal3iv");
	functions->normal3s = (CL_GLFunctions::ptr_glNormal3s) CL_OpenGL::get_proc_address("glNormal3s");
	functions->normal3sv = (CL_GLFunctions::ptr_glNormal3sv) CL_OpenGL::get_proc_address("glNormal3sv");
	functions->normalPointer = (CL_GLFunctions::ptr_glNormalPointer) CL_OpenGL::get_proc_address("glNormalPointer");
	functions->ortho = (CL_GLFunctions::ptr_glOrtho) CL_OpenGL::get_proc_address("glOrtho");
	functions->passThrough = (CL_GLFunctions::ptr_glPassThrough) CL_OpenGL::get_proc_address("glPassThrough");
	functions->pixelMapfv = (CL_GLFunctions::ptr_glPixelMapfv) CL_OpenGL::get_proc_address("glPixelMapfv");
	functions->pixelMapuiv = (CL_GLFunctions::ptr_glPixelMapuiv) CL_OpenGL::get_proc_address("glPixelMapuiv");
	functions->pixelMapusv = (CL_GLFunctions::ptr_glPixelMapusv) CL_OpenGL::get_proc_address("glPixelMapusv");
	functions->pixelStoref = (CL_GLFunctions::ptr_glPixelStoref) CL_OpenGL::get_proc_address("glPixelStoref");
	functions->pixelStorei = (CL_GLFunctions::ptr_glPixelStorei) CL_OpenGL::get_proc_address("glPixelStorei");
	functions->pixelTransferf = (CL_GLFunctions::ptr_glPixelTransferf) CL_OpenGL::get_proc_address("glPixelTransferf");
	functions->pixelTransferi = (CL_GLFunctions::ptr_glPixelTransferi) CL_OpenGL::get_proc_address("glPixelTransferi");
	functions->pixelZoom = (CL_GLFunctions::ptr_glPixelZoom) CL_OpenGL::get_proc_address("glPixelZoom");
	functions->pointSize = (CL_GLFunctions::ptr_glPointSize) CL_OpenGL::get_proc_address("glPointSize");
	functions->polygonMode = (CL_GLFunctions::ptr_glPolygonMode) CL_OpenGL::get_proc_address("glPolygonMode");
	functions->polygonOffset = (CL_GLFunctions::ptr_glPolygonOffset) CL_OpenGL::get_proc_address("glPolygonOffset");
	functions->polygonStipple = (CL_GLFunctions::ptr_glPolygonStipple) CL_OpenGL::get_proc_address("glPolygonStipple");
	functions->popAttrib = (CL_GLFunctions::ptr_glPopAttrib) CL_OpenGL::get_proc_address("glPopAttrib");
	functions->popClientAttrib = (CL_GLFunctions::ptr_glPopClientAttrib) CL_OpenGL::get_proc_address("glPopClientAttrib");
	functions->popMatrix = (CL_GLFunctions::ptr_glPopMatrix) CL_OpenGL::get_proc_address("glPopMatrix");
	functions->popName = (CL_GLFunctions::ptr_glPopName) CL_OpenGL::get_proc_address("glPopName");
	functions->prioritizeTextures = (CL_GLFunctions::ptr_glPrioritizeTextures) CL_OpenGL::get_proc_address("glPrioritizeTextures");
	functions->pushAttrib = (CL_GLFunctions::ptr_glPushAttrib) CL_OpenGL::get_proc_address("glPushAttrib");
	functions->pushClientAttrib = (CL_GLFunctions::ptr_glPushClientAttrib) CL_OpenGL::get_proc_address("glPushClientAttrib");
	functions->pushMatrix = (CL_GLFunctions::ptr_glPushMatrix) CL_OpenGL::get_proc_address("glPushMatrix");
	functions->pushName = (CL_GLFunctions::ptr_glPushName) CL_OpenGL::get_proc_address("glPushName");
	functions->rasterPos2d = (CL_GLFunctions::ptr_glRasterPos2d) CL_OpenGL::get_proc_address("glRasterPos2d");
	functions->rasterPos2dv = (CL_GLFunctions::ptr_glRasterPos2dv) CL_OpenGL::get_proc_address("glRasterPos2dv");
	functions->rasterPos2f = (CL_GLFunctions::ptr_glRasterPos2f) CL_OpenGL::get_proc_address("glRasterPos2f");
	functions->rasterPos2fv = (CL_GLFunctions::ptr_glRasterPos2fv) CL_OpenGL::get_proc_address("glRasterPos2fv");
	functions->rasterPos2i = (CL_GLFunctions::ptr_glRasterPos2i) CL_OpenGL::get_proc_address("glRasterPos2i");
	functions->rasterPos2iv = (CL_GLFunctions::ptr_glRasterPos2iv) CL_OpenGL::get_proc_address("glRasterPos2iv");
	functions->rasterPos2s = (CL_GLFunctions::ptr_glRasterPos2s) CL_OpenGL::get_proc_address("glRasterPos2s");
	functions->rasterPos2sv = (CL_GLFunctions::ptr_glRasterPos2sv) CL_OpenGL::get_proc_address("glRasterPos2sv");
	functions->rasterPos3d = (CL_GLFunctions::ptr_glRasterPos3d) CL_OpenGL::get_proc_address("glRasterPos3d");
	functions->rasterPos3dv = (CL_GLFunctions::ptr_glRasterPos3dv) CL_OpenGL::get_proc_address("glRasterPos3dv");
	functions->rasterPos3f = (CL_GLFunctions::ptr_glRasterPos3f) CL_OpenGL::get_proc_address("glRasterPos3f");
	functions->rasterPos3fv = (CL_GLFunctions::ptr_glRasterPos3fv) CL_OpenGL::get_proc_address("glRasterPos3fv");
	functions->rasterPos3i = (CL_GLFunctions::ptr_glRasterPos3i) CL_OpenGL::get_proc_address("glRasterPos3i");
	functions->rasterPos3iv = (CL_GLFunctions::ptr_glRasterPos3iv) CL_OpenGL::get_proc_address("glRasterPos3iv");
	functions->rasterPos3s = (CL_GLFunctions::ptr_glRasterPos3s) CL_OpenGL::get_proc_address("glRasterPos3s");
	functions->rasterPos3sv = (CL_GLFunctions::ptr_glRasterPos3sv) CL_OpenGL::get_proc_address("glRasterPos3sv");
	functions->rasterPos4d = (CL_GLFunctions::ptr_glRasterPos4d) CL_OpenGL::get_proc_address("glRasterPos4d");
	functions->rasterPos4dv = (CL_GLFunctions::ptr_glRasterPos4dv) CL_OpenGL::get_proc_address("glRasterPos4dv");
	functions->rasterPos4f = (CL_GLFunctions::ptr_glRasterPos4f) CL_OpenGL::get_proc_address("glRasterPos4f");
	functions->rasterPos4fv = (CL_GLFunctions::ptr_glRasterPos4fv) CL_OpenGL::get_proc_address("glRasterPos4fv");
	functions->rasterPos4i = (CL_GLFunctions::ptr_glRasterPos4i) CL_OpenGL::get_proc_address("glRasterPos4i");
	functions->rasterPos4iv = (CL_GLFunctions::ptr_glRasterPos4iv) CL_OpenGL::get_proc_address("glRasterPos4iv");
	functions->rasterPos4s = (CL_GLFunctions::ptr_glRasterPos4s) CL_OpenGL::get_proc_address("glRasterPos4s");
	functions->rasterPos4sv = (CL_GLFunctions::ptr_glRasterPos4sv) CL_OpenGL::get_proc_address("glRasterPos4sv");
	functions->readBuffer = (CL_GLFunctions::ptr_glReadBuffer) CL_OpenGL::get_proc_address("glReadBuffer");
	functions->readPixels = (CL_GLFunctions::ptr_glReadPixels) CL_OpenGL::get_proc_address("glReadPixels");
	functions->rectd = (CL_GLFunctions::ptr_glRectd) CL_OpenGL::get_proc_address("glRectd");
	functions->rectdv = (CL_GLFunctions::ptr_glRectdv) CL_OpenGL::get_proc_address("glRectdv");
	functions->rectf = (CL_GLFunctions::ptr_glRectf) CL_OpenGL::get_proc_address("glRectf");
	functions->rectfv = (CL_GLFunctions::ptr_glRectfv) CL_OpenGL::get_proc_address("glRectfv");
	functions->recti = (CL_GLFunctions::ptr_glRecti) CL_OpenGL::get_proc_address("glRecti");
	functions->rectiv = (CL_GLFunctions::ptr_glRectiv) CL_OpenGL::get_proc_address("glRectiv");
	functions->rects = (CL_GLFunctions::ptr_glRects) CL_OpenGL::get_proc_address("glRects");
	functions->rectsv = (CL_GLFunctions::ptr_glRectsv) CL_OpenGL::get_proc_address("glRectsv");
	functions->renderMode = (CL_GLFunctions::ptr_glRenderMode) CL_OpenGL::get_proc_address("glRenderMode");
	functions->rotated = (CL_GLFunctions::ptr_glRotated) CL_OpenGL::get_proc_address("glRotated");
	functions->rotatef = (CL_GLFunctions::ptr_glRotatef) CL_OpenGL::get_proc_address("glRotatef");
	functions->scaled = (CL_GLFunctions::ptr_glScaled) CL_OpenGL::get_proc_address("glScaled");
	functions->scalef = (CL_GLFunctions::ptr_glScalef) CL_OpenGL::get_proc_address("glScalef");
	functions->scissor = (CL_GLFunctions::ptr_glScissor) CL_OpenGL::get_proc_address("glScissor");
	functions->selectBuffer = (CL_GLFunctions::ptr_glSelectBuffer) CL_OpenGL::get_proc_address("glSelectBuffer");
	functions->shadeModel = (CL_GLFunctions::ptr_glShadeModel) CL_OpenGL::get_proc_address("glShadeModel");
	functions->stencilFunc = (CL_GLFunctions::ptr_glStencilFunc) CL_OpenGL::get_proc_address("glStencilFunc");
	functions->stencilMask = (CL_GLFunctions::ptr_glStencilMask) CL_OpenGL::get_proc_address("glStencilMask");
	functions->stencilMaskSeparate = (CL_GLFunctions::ptr_glStencilMaskSeparate) CL_OpenGL::get_proc_address("glStencilMaskSeparate");
	functions->stencilOp = (CL_GLFunctions::ptr_glStencilOp) CL_OpenGL::get_proc_address("glStencilOp");
	functions->texCoord1d = (CL_GLFunctions::ptr_glTexCoord1d) CL_OpenGL::get_proc_address("glTexCoord1d");
	functions->texCoord1dv = (CL_GLFunctions::ptr_glTexCoord1dv) CL_OpenGL::get_proc_address("glTexCoord1dv");
	functions->texCoord1f = (CL_GLFunctions::ptr_glTexCoord1f) CL_OpenGL::get_proc_address("glTexCoord1f");
	functions->texCoord1fv = (CL_GLFunctions::ptr_glTexCoord1fv) CL_OpenGL::get_proc_address("glTexCoord1fv");
	functions->texCoord1i = (CL_GLFunctions::ptr_glTexCoord1i) CL_OpenGL::get_proc_address("glTexCoord1i");
	functions->texCoord1iv = (CL_GLFunctions::ptr_glTexCoord1iv) CL_OpenGL::get_proc_address("glTexCoord1iv");
	functions->texCoord1s = (CL_GLFunctions::ptr_glTexCoord1s) CL_OpenGL::get_proc_address("glTexCoord1s");
	functions->texCoord1sv = (CL_GLFunctions::ptr_glTexCoord1sv) CL_OpenGL::get_proc_address("glTexCoord1sv");
	functions->texCoord2d = (CL_GLFunctions::ptr_glTexCoord2d) CL_OpenGL::get_proc_address("glTexCoord2d");
	functions->texCoord2dv = (CL_GLFunctions::ptr_glTexCoord2dv) CL_OpenGL::get_proc_address("glTexCoord2dv");
	functions->texCoord2f = (CL_GLFunctions::ptr_glTexCoord2f) CL_OpenGL::get_proc_address("glTexCoord2f");
	functions->texCoord2fv = (CL_GLFunctions::ptr_glTexCoord2fv) CL_OpenGL::get_proc_address("glTexCoord2fv");
	functions->texCoord2i = (CL_GLFunctions::ptr_glTexCoord2i) CL_OpenGL::get_proc_address("glTexCoord2i");
	functions->texCoord2iv = (CL_GLFunctions::ptr_glTexCoord2iv) CL_OpenGL::get_proc_address("glTexCoord2iv");
	functions->texCoord2s = (CL_GLFunctions::ptr_glTexCoord2s) CL_OpenGL::get_proc_address("glTexCoord2s");
	functions->texCoord2sv = (CL_GLFunctions::ptr_glTexCoord2sv) CL_OpenGL::get_proc_address("glTexCoord2sv");
	functions->texCoord3d = (CL_GLFunctions::ptr_glTexCoord3d) CL_OpenGL::get_proc_address("glTexCoord3d");
	functions->texCoord3dv = (CL_GLFunctions::ptr_glTexCoord3dv) CL_OpenGL::get_proc_address("glTexCoord3dv");
	functions->texCoord3f = (CL_GLFunctions::ptr_glTexCoord3f) CL_OpenGL::get_proc_address("glTexCoord3f");
	functions->texCoord3fv = (CL_GLFunctions::ptr_glTexCoord3fv) CL_OpenGL::get_proc_address("glTexCoord3fv");
	functions->texCoord3i = (CL_GLFunctions::ptr_glTexCoord3i) CL_OpenGL::get_proc_address("glTexCoord3i");
	functions->texCoord3iv = (CL_GLFunctions::ptr_glTexCoord3iv) CL_OpenGL::get_proc_address("glTexCoord3iv");
	functions->texCoord3s = (CL_GLFunctions::ptr_glTexCoord3s) CL_OpenGL::get_proc_address("glTexCoord3s");
	functions->texCoord3sv = (CL_GLFunctions::ptr_glTexCoord3sv) CL_OpenGL::get_proc_address("glTexCoord3sv");
	functions->texCoord4d = (CL_GLFunctions::ptr_glTexCoord4d) CL_OpenGL::get_proc_address("glTexCoord4d");
	functions->texCoord4dv = (CL_GLFunctions::ptr_glTexCoord4dv) CL_OpenGL::get_proc_address("glTexCoord4dv");
	functions->texCoord4f = (CL_GLFunctions::ptr_glTexCoord4f) CL_OpenGL::get_proc_address("glTexCoord4f");
	functions->texCoord4fv = (CL_GLFunctions::ptr_glTexCoord4fv) CL_OpenGL::get_proc_address("glTexCoord4fv");
	functions->texCoord4i = (CL_GLFunctions::ptr_glTexCoord4i) CL_OpenGL::get_proc_address("glTexCoord4i");
	functions->texCoord4iv = (CL_GLFunctions::ptr_glTexCoord4iv) CL_OpenGL::get_proc_address("glTexCoord4iv");
	functions->texCoord4s = (CL_GLFunctions::ptr_glTexCoord4s) CL_OpenGL::get_proc_address("glTexCoord4s");
	functions->texCoord4sv = (CL_GLFunctions::ptr_glTexCoord4sv) CL_OpenGL::get_proc_address("glTexCoord4sv");
	functions->texCoordPointer = (CL_GLFunctions::ptr_glTexCoordPointer) CL_OpenGL::get_proc_address("glTexCoordPointer");
	functions->texEnvf = (CL_GLFunctions::ptr_glTexEnvf) CL_OpenGL::get_proc_address("glTexEnvf");
	functions->texEnvfv = (CL_GLFunctions::ptr_glTexEnvfv) CL_OpenGL::get_proc_address("glTexEnvfv");
	functions->texEnvi = (CL_GLFunctions::ptr_glTexEnvi) CL_OpenGL::get_proc_address("glTexEnvi");
	functions->texEnviv = (CL_GLFunctions::ptr_glTexEnviv) CL_OpenGL::get_proc_address("glTexEnviv");
	functions->texGend = (CL_GLFunctions::ptr_glTexGend) CL_OpenGL::get_proc_address("glTexGend");
	functions->texGendv = (CL_GLFunctions::ptr_glTexGendv) CL_OpenGL::get_proc_address("glTexGendv");
	functions->texGenf = (CL_GLFunctions::ptr_glTexGenf) CL_OpenGL::get_proc_address("glTexGenf");
	functions->texGenfv = (CL_GLFunctions::ptr_glTexGenfv) CL_OpenGL::get_proc_address("glTexGenfv");
	functions->texGeni = (CL_GLFunctions::ptr_glTexGeni) CL_OpenGL::get_proc_address("glTexGeni");
	functions->texGeniv = (CL_GLFunctions::ptr_glTexGeniv) CL_OpenGL::get_proc_address("glTexGeniv");
	functions->texImage1D = (CL_GLFunctions::ptr_glTexImage1D) CL_OpenGL::get_proc_address("glTexImage1D");
	functions->texImage2D = (CL_GLFunctions::ptr_glTexImage2D) CL_OpenGL::get_proc_address("glTexImage2D");
	functions->texParameterf = (CL_GLFunctions::ptr_glTexParameterf) CL_OpenGL::get_proc_address("glTexParameterf");
	functions->texParameterfv = (CL_GLFunctions::ptr_glTexParameterfv) CL_OpenGL::get_proc_address("glTexParameterfv");
	functions->texParameteri = (CL_GLFunctions::ptr_glTexParameteri) CL_OpenGL::get_proc_address("glTexParameteri");
	functions->texParameteriv = (CL_GLFunctions::ptr_glTexParameteriv) CL_OpenGL::get_proc_address("glTexParameteriv");
	functions->texSubImage1D = (CL_GLFunctions::ptr_glTexSubImage1D) CL_OpenGL::get_proc_address("glTexSubImage1D");
	functions->texSubImage2D = (CL_GLFunctions::ptr_glTexSubImage2D) CL_OpenGL::get_proc_address("glTexSubImage2D");
	functions->translated = (CL_GLFunctions::ptr_glTranslated) CL_OpenGL::get_proc_address("glTranslated");
	functions->translatef = (CL_GLFunctions::ptr_glTranslatef) CL_OpenGL::get_proc_address("glTranslatef");
	functions->vertex2d = (CL_GLFunctions::ptr_glVertex2d) CL_OpenGL::get_proc_address("glVertex2d");
	functions->vertex2dv = (CL_GLFunctions::ptr_glVertex2dv) CL_OpenGL::get_proc_address("glVertex2dv");
	functions->vertex2f = (CL_GLFunctions::ptr_glVertex2f) CL_OpenGL::get_proc_address("glVertex2f");
	functions->vertex2fv = (CL_GLFunctions::ptr_glVertex2fv) CL_OpenGL::get_proc_address("glVertex2fv");
	functions->vertex2i = (CL_GLFunctions::ptr_glVertex2i) CL_OpenGL::get_proc_address("glVertex2i");
	functions->vertex2iv = (CL_GLFunctions::ptr_glVertex2iv) CL_OpenGL::get_proc_address("glVertex2iv");
	functions->vertex2s = (CL_GLFunctions::ptr_glVertex2s) CL_OpenGL::get_proc_address("glVertex2s");
	functions->vertex2sv = (CL_GLFunctions::ptr_glVertex2sv) CL_OpenGL::get_proc_address("glVertex2sv");
	functions->vertex3d = (CL_GLFunctions::ptr_glVertex3d) CL_OpenGL::get_proc_address("glVertex3d");
	functions->vertex3dv = (CL_GLFunctions::ptr_glVertex3dv) CL_OpenGL::get_proc_address("glVertex3dv");
	functions->vertex3f = (CL_GLFunctions::ptr_glVertex3f) CL_OpenGL::get_proc_address("glVertex3f");
	functions->vertex3fv = (CL_GLFunctions::ptr_glVertex3fv) CL_OpenGL::get_proc_address("glVertex3fv");
	functions->vertex3i = (CL_GLFunctions::ptr_glVertex3i) CL_OpenGL::get_proc_address("glVertex3i");
	functions->vertex3iv = (CL_GLFunctions::ptr_glVertex3iv) CL_OpenGL::get_proc_address("glVertex3iv");
	functions->vertex3s = (CL_GLFunctions::ptr_glVertex3s) CL_OpenGL::get_proc_address("glVertex3s");
	functions->vertex3sv = (CL_GLFunctions::ptr_glVertex3sv) CL_OpenGL::get_proc_address("glVertex3sv");
	functions->vertex4d = (CL_GLFunctions::ptr_glVertex4d) CL_OpenGL::get_proc_address("glVertex4d");
	functions->vertex4dv = (CL_GLFunctions::ptr_glVertex4dv) CL_OpenGL::get_proc_address("glVertex4dv");
	functions->vertex4f = (CL_GLFunctions::ptr_glVertex4f) CL_OpenGL::get_proc_address("glVertex4f");
	functions->vertex4fv = (CL_GLFunctions::ptr_glVertex4fv) CL_OpenGL::get_proc_address("glVertex4fv");
	functions->vertex4i = (CL_GLFunctions::ptr_glVertex4i) CL_OpenGL::get_proc_address("glVertex4i");
	functions->vertex4iv = (CL_GLFunctions::ptr_glVertex4iv) CL_OpenGL::get_proc_address("glVertex4iv");
	functions->vertex4s = (CL_GLFunctions::ptr_glVertex4s) CL_OpenGL::get_proc_address("glVertex4s");
	functions->vertex4sv = (CL_GLFunctions::ptr_glVertex4sv) CL_OpenGL::get_proc_address("glVertex4sv");
	functions->vertexPointer = (CL_GLFunctions::ptr_glVertexPointer) CL_OpenGL::get_proc_address("glVertexPointer");
	functions->viewport = (CL_GLFunctions::ptr_glViewport) CL_OpenGL::get_proc_address("glViewport");
#endif

	// Binds for OpenGL 1.2:

#ifdef __APPLE__
	functions->drawRangeElements = (CL_GLFunctions::ptr_glDrawRangeElementsEXT) &glDrawRangeElements;
	functions->texImage3D = (CL_GLFunctions::ptr_glTexImage3DEXT) &glTexImage3D;
	functions->texSubImage3D = (CL_GLFunctions::ptr_glTexSubImage3DEXT) &glTexSubImage3D;
	functions->copyTexSubImage3D = (CL_GLFunctions::ptr_glCopyTexSubImage3DEXT) &glCopyTexSubImage3D;
	functions->colorTable = (CL_GLFunctions::ptr_glColorTableSGI) &glColorTable;
	functions->copyColorTable = (CL_GLFunctions::ptr_glCopyColorTableSGI) &glCopyColorTable;
	functions->colorTableParameteriv = (CL_GLFunctions::ptr_glColorTableParameterivSGI) &glColorTableParameteriv;
	functions->colorTableParameterfv = (CL_GLFunctions::ptr_glColorTableParameterfvSGI) &glColorTableParameterfv;
	functions->getColorTable = (CL_GLFunctions::ptr_glGetColorTableSGI) &glGetColorTable;
	functions->getColorTableParameteriv = (CL_GLFunctions::ptr_glGetColorTableParameterivSGI) &glGetColorTableParameteriv;
	functions->getColorTableParameterfv = (CL_GLFunctions::ptr_glGetColorTableParameterfvSGI) &glGetColorTableParameterfv;
	functions->colorSubTable = (CL_GLFunctions::ptr_glColorSubTableEXT) &glColorSubTable;
	functions->copyColorSubTable = (CL_GLFunctions::ptr_glCopyColorSubTableEXT) &glCopyColorSubTable;
	functions->convolutionFilter1D = (CL_GLFunctions::ptr_glConvolutionFilter1DEXT) &glConvolutionFilter1D;
	functions->convolutionFilter2D = (CL_GLFunctions::ptr_glConvolutionFilter2DEXT) &glConvolutionFilter2D;
	functions->copyConvolutionFilter1D = (CL_GLFunctions::ptr_glCopyConvolutionFilter1DEXT) &glCopyConvolutionFilter1D;
	functions->copyConvolutionFilter2D = (CL_GLFunctions::ptr_glCopyConvolutionFilter2DEXT) &glCopyConvolutionFilter2D;
	functions->getConvolutionFilter = (CL_GLFunctions::ptr_glGetConvolutionFilterEXT) &glGetConvolutionFilter;
	functions->separableFilter2D = (CL_GLFunctions::ptr_glSeparableFilter2DEXT) &glSeparableFilter2D;
	functions->getSeparableFilter = (CL_GLFunctions::ptr_glGetSeparableFilterEXT) &glGetSeparableFilter;
	functions->convolutionParameteri = (CL_GLFunctions::ptr_glConvolutionParameteriEXT) &glConvolutionParameteri;
	functions->convolutionParameteriv = (CL_GLFunctions::ptr_glConvolutionParameterivEXT) &glConvolutionParameteriv;
	functions->convolutionParameterf = (CL_GLFunctions::ptr_glConvolutionParameterfEXT) &glConvolutionParameterf;
	functions->convolutionParameterfv = (CL_GLFunctions::ptr_glConvolutionParameterfvEXT) &glConvolutionParameterfv;
	functions->getConvolutionParameteriv = (CL_GLFunctions::ptr_glGetConvolutionParameterivEXT) &glGetConvolutionParameteriv;
	functions->getConvolutionParameterfv = (CL_GLFunctions::ptr_glGetConvolutionParameterfvEXT) &glGetConvolutionParameterfv;
	functions->histogram = (CL_GLFunctions::ptr_glHistogramEXT) &glHistogram;
	functions->resetHistogram = (CL_GLFunctions::ptr_glResetHistogramEXT) &glResetHistogram;
	functions->getHistogram = (CL_GLFunctions::ptr_glGetHistogramEXT) &glGetHistogram;
	functions->getHistogramParameteriv = (CL_GLFunctions::ptr_glGetHistogramParameterivEXT) &glGetHistogramParameteriv;
	functions->getHistogramParameterfv = (CL_GLFunctions::ptr_glGetHistogramParameterfvEXT) &glGetHistogramParameterfv;
	functions->minmax = (CL_GLFunctions::ptr_glMinmaxEXT) &glMinmax;
	functions->resetMinmax = (CL_GLFunctions::ptr_glResetMinmaxEXT) &glResetMinmax;
	functions->getMinmax = (CL_GLFunctions::ptr_glGetMinmaxEXT) &glGetMinmax;
	functions->getMinmaxParameteriv = (CL_GLFunctions::ptr_glGetMinmaxParameterivEXT) &glGetMinmaxParameteriv;
	functions->getMinmaxParameterfv = (CL_GLFunctions::ptr_glGetMinmaxParameterfvEXT) &glGetMinmaxParameterfv;
	functions->blendColor = (CL_GLFunctions::ptr_glBlendColorEXT) &glBlendColor;
	functions->blendEquation = (CL_GLFunctions::ptr_glBlendEquationEXT) &glBlendEquation;
#else
	functions->drawRangeElements = (CL_GLFunctions::ptr_glDrawRangeElementsEXT) CL_OpenGL::get_proc_address("glDrawRangeElementsEXT");
	functions->texImage3D = (CL_GLFunctions::ptr_glTexImage3DEXT) CL_OpenGL::get_proc_address("glTexImage3DEXT");
	functions->texSubImage3D = (CL_GLFunctions::ptr_glTexSubImage3DEXT) CL_OpenGL::get_proc_address("glTexSubImage3DEXT");
	functions->copyTexSubImage3D = (CL_GLFunctions::ptr_glCopyTexSubImage3DEXT) CL_OpenGL::get_proc_address("glCopyTexSubImage3DEXT");
	functions->colorTable = (CL_GLFunctions::ptr_glColorTableSGI) CL_OpenGL::get_proc_address("glColorTableSGI");
	functions->copyColorTable = (CL_GLFunctions::ptr_glCopyColorTableSGI) CL_OpenGL::get_proc_address("glCopyColorTableSGI");
	functions->colorTableParameteriv = (CL_GLFunctions::ptr_glColorTableParameterivSGI) CL_OpenGL::get_proc_address("glColorTableParameterivSGI");
	functions->colorTableParameterfv = (CL_GLFunctions::ptr_glColorTableParameterfvSGI) CL_OpenGL::get_proc_address("glColorTableParameterfvSGI");
	functions->getColorTable = (CL_GLFunctions::ptr_glGetColorTableSGI) CL_OpenGL::get_proc_address("glGetColorTableSGI");
	functions->getColorTableParameteriv = (CL_GLFunctions::ptr_glGetColorTableParameterivSGI) CL_OpenGL::get_proc_address("glGetColorTableParameterivSGI");
	functions->getColorTableParameterfv = (CL_GLFunctions::ptr_glGetColorTableParameterfvSGI) CL_OpenGL::get_proc_address("glGetColorTableParameterfvSGI");
	functions->colorSubTable = (CL_GLFunctions::ptr_glColorSubTableEXT) CL_OpenGL::get_proc_address("glColorSubTableEXT");
	functions->copyColorSubTable = (CL_GLFunctions::ptr_glCopyColorSubTableEXT) CL_OpenGL::get_proc_address("glCopyColorSubTableEXT");
	functions->convolutionFilter1D = (CL_GLFunctions::ptr_glConvolutionFilter1DEXT) CL_OpenGL::get_proc_address("glConvolutionFilter1DEXT");
	functions->convolutionFilter2D = (CL_GLFunctions::ptr_glConvolutionFilter2DEXT) CL_OpenGL::get_proc_address("glConvolutionFilter2DEXT");
	functions->copyConvolutionFilter1D = (CL_GLFunctions::ptr_glCopyConvolutionFilter1DEXT) CL_OpenGL::get_proc_address("glCopyConvolutionFilter1DEXT");
	functions->copyConvolutionFilter2D = (CL_GLFunctions::ptr_glCopyConvolutionFilter2DEXT) CL_OpenGL::get_proc_address("glCopyConvolutionFilter2DEXT");
	functions->getConvolutionFilter = (CL_GLFunctions::ptr_glGetConvolutionFilterEXT) CL_OpenGL::get_proc_address("glGetConvolutionFilterEXT");
	functions->separableFilter2D = (CL_GLFunctions::ptr_glSeparableFilter2DEXT) CL_OpenGL::get_proc_address("glSeparableFilter2DEXT");
	functions->getSeparableFilter = (CL_GLFunctions::ptr_glGetSeparableFilterEXT) CL_OpenGL::get_proc_address("glGetSeparableFilterEXT");
	functions->convolutionParameteri = (CL_GLFunctions::ptr_glConvolutionParameteriEXT) CL_OpenGL::get_proc_address("glConvolutionParameteriEXT");
	functions->convolutionParameteriv = (CL_GLFunctions::ptr_glConvolutionParameterivEXT) CL_OpenGL::get_proc_address("glConvolutionParameterivEXT");
	functions->convolutionParameterf = (CL_GLFunctions::ptr_glConvolutionParameterfEXT) CL_OpenGL::get_proc_address("glConvolutionParameterfEXT");
	functions->convolutionParameterfv = (CL_GLFunctions::ptr_glConvolutionParameterfvEXT) CL_OpenGL::get_proc_address("glConvolutionParameterfvEXT");
	functions->getConvolutionParameteriv = (CL_GLFunctions::ptr_glGetConvolutionParameterivEXT) CL_OpenGL::get_proc_address("glGetConvolutionParameterivEXT");
	functions->getConvolutionParameterfv = (CL_GLFunctions::ptr_glGetConvolutionParameterfvEXT) CL_OpenGL::get_proc_address("glGetConvolutionParameterfvEXT");
	functions->histogram = (CL_GLFunctions::ptr_glHistogramEXT) CL_OpenGL::get_proc_address("glHistogramEXT");
	functions->resetHistogram = (CL_GLFunctions::ptr_glResetHistogramEXT) CL_OpenGL::get_proc_address("glResetHistogramEXT");
	functions->getHistogram = (CL_GLFunctions::ptr_glGetHistogramEXT) CL_OpenGL::get_proc_address("glGetHistogramEXT");
	functions->getHistogramParameteriv = (CL_GLFunctions::ptr_glGetHistogramParameterivEXT) CL_OpenGL::get_proc_address("glGetHistogramParameterivEXT");
	functions->getHistogramParameterfv = (CL_GLFunctions::ptr_glGetHistogramParameterfvEXT) CL_OpenGL::get_proc_address("glGetHistogramParameterfvEXT");
	functions->minmax = (CL_GLFunctions::ptr_glMinmaxEXT) CL_OpenGL::get_proc_address("glMinmaxEXT");
	functions->resetMinmax = (CL_GLFunctions::ptr_glResetMinmaxEXT) CL_OpenGL::get_proc_address("glResetMinmaxEXT");
	functions->getMinmax = (CL_GLFunctions::ptr_glGetMinmaxEXT) CL_OpenGL::get_proc_address("glGetMinmaxEXT");
	functions->getMinmaxParameteriv = (CL_GLFunctions::ptr_glGetMinmaxParameterivEXT) CL_OpenGL::get_proc_address("glGetMinmaxParameterivEXT");
	functions->getMinmaxParameterfv = (CL_GLFunctions::ptr_glGetMinmaxParameterfvEXT) CL_OpenGL::get_proc_address("glGetMinmaxParameterfvEXT");
	functions->blendColor = (CL_GLFunctions::ptr_glBlendColorEXT) CL_OpenGL::get_proc_address("glBlendColorEXT");
	functions->blendEquation = (CL_GLFunctions::ptr_glBlendEquationEXT) CL_OpenGL::get_proc_address("glBlendEquationEXT");
#endif

	// Binds for OpenGL 1.2.1:

#ifdef __APPLE__
	functions->activeTexture = (CL_GLFunctions::ptr_glActiveTextureARB) &glActiveTexture;
	functions->clientActiveTexture = (CL_GLFunctions::ptr_glClientActiveTextureARB) &glClientActiveTexture;
	functions->multiTexCoord1d = (CL_GLFunctions::ptr_glMultiTexCoord1dARB) &glMultiTexCoord1d;
	functions->multiTexCoord1dv = (CL_GLFunctions::ptr_glMultiTexCoord1dvARB) &glMultiTexCoord1dv;
	functions->multiTexCoord1f = (CL_GLFunctions::ptr_glMultiTexCoord1fARB) &glMultiTexCoord1f;
	functions->multiTexCoord1fv = (CL_GLFunctions::ptr_glMultiTexCoord1fvARB) &glMultiTexCoord1fv;
	functions->multiTexCoord1i = (CL_GLFunctions::ptr_glMultiTexCoord1iARB) &glMultiTexCoord1i;
	functions->multiTexCoord1iv = (CL_GLFunctions::ptr_glMultiTexCoord1ivARB) &glMultiTexCoord1iv;
	functions->multiTexCoord1s = (CL_GLFunctions::ptr_glMultiTexCoord1sARB) &glMultiTexCoord1s;
	functions->multiTexCoord1sv = (CL_GLFunctions::ptr_glMultiTexCoord1svARB) &glMultiTexCoord1sv;
	functions->multiTexCoord2d = (CL_GLFunctions::ptr_glMultiTexCoord2dARB) &glMultiTexCoord2d;
	functions->multiTexCoord2dv = (CL_GLFunctions::ptr_glMultiTexCoord2dvARB) &glMultiTexCoord2dv;
	functions->multiTexCoord2f = (CL_GLFunctions::ptr_glMultiTexCoord2fARB) &glMultiTexCoord2f;
	functions->multiTexCoord2fv = (CL_GLFunctions::ptr_glMultiTexCoord2fvARB) &glMultiTexCoord2fv;
	functions->multiTexCoord2i = (CL_GLFunctions::ptr_glMultiTexCoord2iARB) &glMultiTexCoord2i;
	functions->multiTexCoord2iv = (CL_GLFunctions::ptr_glMultiTexCoord2ivARB) &glMultiTexCoord2iv;
	functions->multiTexCoord2s = (CL_GLFunctions::ptr_glMultiTexCoord2sARB) &glMultiTexCoord2s;
	functions->multiTexCoord2sv = (CL_GLFunctions::ptr_glMultiTexCoord2svARB) &glMultiTexCoord2sv;
	functions->multiTexCoord3d = (CL_GLFunctions::ptr_glMultiTexCoord3dARB) &glMultiTexCoord3d;
	functions->multiTexCoord3dv = (CL_GLFunctions::ptr_glMultiTexCoord3dvARB) &glMultiTexCoord3dv;
	functions->multiTexCoord3f = (CL_GLFunctions::ptr_glMultiTexCoord3fARB) &glMultiTexCoord3f;
	functions->multiTexCoord3fv = (CL_GLFunctions::ptr_glMultiTexCoord3fvARB) &glMultiTexCoord3fv;
	functions->multiTexCoord3i = (CL_GLFunctions::ptr_glMultiTexCoord3iARB) &glMultiTexCoord3i;
	functions->multiTexCoord3iv = (CL_GLFunctions::ptr_glMultiTexCoord3ivARB) &glMultiTexCoord3iv;
	functions->multiTexCoord3s = (CL_GLFunctions::ptr_glMultiTexCoord3sARB) &glMultiTexCoord3s;
	functions->multiTexCoord3sv = (CL_GLFunctions::ptr_glMultiTexCoord3svARB) &glMultiTexCoord3sv;
	functions->multiTexCoord4d = (CL_GLFunctions::ptr_glMultiTexCoord4dARB) &glMultiTexCoord4d;
	functions->multiTexCoord4dv = (CL_GLFunctions::ptr_glMultiTexCoord4dvARB) &glMultiTexCoord4dv;
	functions->multiTexCoord4f = (CL_GLFunctions::ptr_glMultiTexCoord4fARB) &glMultiTexCoord4f;
	functions->multiTexCoord4fv = (CL_GLFunctions::ptr_glMultiTexCoord4fvARB) &glMultiTexCoord4fv;
	functions->multiTexCoord4i = (CL_GLFunctions::ptr_glMultiTexCoord4iARB) &glMultiTexCoord4i;
	functions->multiTexCoord4iv = (CL_GLFunctions::ptr_glMultiTexCoord4ivARB) &glMultiTexCoord4iv;
	functions->multiTexCoord4s = (CL_GLFunctions::ptr_glMultiTexCoord4sARB) &glMultiTexCoord4s;
	functions->multiTexCoord4sv = (CL_GLFunctions::ptr_glMultiTexCoord4svARB) &glMultiTexCoord4sv;
#else
	functions->activeTexture = (CL_GLFunctions::ptr_glActiveTextureARB) CL_OpenGL::get_proc_address("glActiveTextureARB");
	functions->clientActiveTexture = (CL_GLFunctions::ptr_glClientActiveTextureARB) CL_OpenGL::get_proc_address("glClientActiveTextureARB");
	functions->multiTexCoord1d = (CL_GLFunctions::ptr_glMultiTexCoord1dARB) CL_OpenGL::get_proc_address("glMultiTexCoord1dARB");
	functions->multiTexCoord1dv = (CL_GLFunctions::ptr_glMultiTexCoord1dvARB) CL_OpenGL::get_proc_address("glMultiTexCoord1dvARB");
	functions->multiTexCoord1f = (CL_GLFunctions::ptr_glMultiTexCoord1fARB) CL_OpenGL::get_proc_address("glMultiTexCoord1fARB");
	functions->multiTexCoord1fv = (CL_GLFunctions::ptr_glMultiTexCoord1fvARB) CL_OpenGL::get_proc_address("glMultiTexCoord1fvARB");
	functions->multiTexCoord1i = (CL_GLFunctions::ptr_glMultiTexCoord1iARB) CL_OpenGL::get_proc_address("glMultiTexCoord1iARB");
	functions->multiTexCoord1iv = (CL_GLFunctions::ptr_glMultiTexCoord1ivARB) CL_OpenGL::get_proc_address("glMultiTexCoord1ivARB");
	functions->multiTexCoord1s = (CL_GLFunctions::ptr_glMultiTexCoord1sARB) CL_OpenGL::get_proc_address("glMultiTexCoord1sARB");
	functions->multiTexCoord1sv = (CL_GLFunctions::ptr_glMultiTexCoord1svARB) CL_OpenGL::get_proc_address("glMultiTexCoord1svARB");
	functions->multiTexCoord2d = (CL_GLFunctions::ptr_glMultiTexCoord2dARB) CL_OpenGL::get_proc_address("glMultiTexCoord2dARB");
	functions->multiTexCoord2dv = (CL_GLFunctions::ptr_glMultiTexCoord2dvARB) CL_OpenGL::get_proc_address("glMultiTexCoord2dvARB");
	functions->multiTexCoord2f = (CL_GLFunctions::ptr_glMultiTexCoord2fARB) CL_OpenGL::get_proc_address("glMultiTexCoord2fARB");
	functions->multiTexCoord2fv = (CL_GLFunctions::ptr_glMultiTexCoord2fvARB) CL_OpenGL::get_proc_address("glMultiTexCoord2fvARB");
	functions->multiTexCoord2i = (CL_GLFunctions::ptr_glMultiTexCoord2iARB) CL_OpenGL::get_proc_address("glMultiTexCoord2iARB");
	functions->multiTexCoord2iv = (CL_GLFunctions::ptr_glMultiTexCoord2ivARB) CL_OpenGL::get_proc_address("glMultiTexCoord2ivARB");
	functions->multiTexCoord2s = (CL_GLFunctions::ptr_glMultiTexCoord2sARB) CL_OpenGL::get_proc_address("glMultiTexCoord2sARB");
	functions->multiTexCoord2sv = (CL_GLFunctions::ptr_glMultiTexCoord2svARB) CL_OpenGL::get_proc_address("glMultiTexCoord2svARB");
	functions->multiTexCoord3d = (CL_GLFunctions::ptr_glMultiTexCoord3dARB) CL_OpenGL::get_proc_address("glMultiTexCoord3dARB");
	functions->multiTexCoord3dv = (CL_GLFunctions::ptr_glMultiTexCoord3dvARB) CL_OpenGL::get_proc_address("glMultiTexCoord3dvARB");
	functions->multiTexCoord3f = (CL_GLFunctions::ptr_glMultiTexCoord3fARB) CL_OpenGL::get_proc_address("glMultiTexCoord3fARB");
	functions->multiTexCoord3fv = (CL_GLFunctions::ptr_glMultiTexCoord3fvARB) CL_OpenGL::get_proc_address("glMultiTexCoord3fvARB");
	functions->multiTexCoord3i = (CL_GLFunctions::ptr_glMultiTexCoord3iARB) CL_OpenGL::get_proc_address("glMultiTexCoord3iARB");
	functions->multiTexCoord3iv = (CL_GLFunctions::ptr_glMultiTexCoord3ivARB) CL_OpenGL::get_proc_address("glMultiTexCoord3ivARB");
	functions->multiTexCoord3s = (CL_GLFunctions::ptr_glMultiTexCoord3sARB) CL_OpenGL::get_proc_address("glMultiTexCoord3sARB");
	functions->multiTexCoord3sv = (CL_GLFunctions::ptr_glMultiTexCoord3svARB) CL_OpenGL::get_proc_address("glMultiTexCoord3svARB");
	functions->multiTexCoord4d = (CL_GLFunctions::ptr_glMultiTexCoord4dARB) CL_OpenGL::get_proc_address("glMultiTexCoord4dARB");
	functions->multiTexCoord4dv = (CL_GLFunctions::ptr_glMultiTexCoord4dvARB) CL_OpenGL::get_proc_address("glMultiTexCoord4dvARB");
	functions->multiTexCoord4f = (CL_GLFunctions::ptr_glMultiTexCoord4fARB) CL_OpenGL::get_proc_address("glMultiTexCoord4fARB");
	functions->multiTexCoord4fv = (CL_GLFunctions::ptr_glMultiTexCoord4fvARB) CL_OpenGL::get_proc_address("glMultiTexCoord4fvARB");
	functions->multiTexCoord4i = (CL_GLFunctions::ptr_glMultiTexCoord4iARB) CL_OpenGL::get_proc_address("glMultiTexCoord4iARB");
	functions->multiTexCoord4iv = (CL_GLFunctions::ptr_glMultiTexCoord4ivARB) CL_OpenGL::get_proc_address("glMultiTexCoord4ivARB");
	functions->multiTexCoord4s = (CL_GLFunctions::ptr_glMultiTexCoord4sARB) CL_OpenGL::get_proc_address("glMultiTexCoord4sARB");
	functions->multiTexCoord4sv = (CL_GLFunctions::ptr_glMultiTexCoord4svARB) CL_OpenGL::get_proc_address("glMultiTexCoord4svARB");
#endif

	// Binds for OpenGL 1.3:

#ifdef __APPLE__
	functions->compressedTexImage1D = (CL_GLFunctions::ptr_glCompressedTexImage1DARB) &glCompressedTexImage1D;
	functions->compressedTexImage2D = (CL_GLFunctions::ptr_glCompressedTexImage2DARB) &glCompressedTexImage2D;
	functions->compressedTexImage3D = (CL_GLFunctions::ptr_glCompressedTexImage3DARB) &glCompressedTexImage3D;
	functions->compressedTexSubImage1D = (CL_GLFunctions::ptr_glCompressedTexSubImage1DARB) &glCompressedTexSubImage1D;
	functions->compressedTexSubImage2D = (CL_GLFunctions::ptr_glCompressedTexSubImage2DARB) &glCompressedTexSubImage2D;
	functions->compressedTexSubImage3D = (CL_GLFunctions::ptr_glCompressedTexSubImage3DARB) &glCompressedTexSubImage3D;
	functions->getCompressedTexImage = (CL_GLFunctions::ptr_glGetCompressedTexImageARB) &glGetCompressedTexImage;
	functions->sampleCoverage = (CL_GLFunctions::ptr_glSampleCoverageARB) &glSampleCoverage;
	functions->glLoadTransposeMatrixd = (CL_GLFunctions::ptr_glLoadTransposeMatrixdARB) &glLoadTransposeMatrixd;
	functions->glLoadTransposeMatrixf = (CL_GLFunctions::ptr_glLoadTransposeMatrixfARB) &glLoadTransposeMatrixf;
	functions->glMultTransposeMatrixd = (CL_GLFunctions::ptr_glMultTransposeMatrixdARB) &glMultTransposeMatrixd;
	functions->glMultTransposeMatrixf = (CL_GLFunctions::ptr_glMultTransposeMatrixfARB) &glMultTransposeMatrixf;
#else
	functions->compressedTexImage1D = (CL_GLFunctions::ptr_glCompressedTexImage1DARB) CL_OpenGL::get_proc_address("glCompressedTexImage1DARB");
	functions->compressedTexImage2D = (CL_GLFunctions::ptr_glCompressedTexImage2DARB) CL_OpenGL::get_proc_address("glCompressedTexImage2DARB");
	functions->compressedTexImage3D = (CL_GLFunctions::ptr_glCompressedTexImage3DARB) CL_OpenGL::get_proc_address("glCompressedTexImage3DARB");
	functions->compressedTexSubImage1D = (CL_GLFunctions::ptr_glCompressedTexSubImage1DARB) CL_OpenGL::get_proc_address("glCompressedTexSubImage1DARB");
	functions->compressedTexSubImage2D = (CL_GLFunctions::ptr_glCompressedTexSubImage2DARB) CL_OpenGL::get_proc_address("glCompressedTexSubImage2DARB");
	functions->compressedTexSubImage3D = (CL_GLFunctions::ptr_glCompressedTexSubImage3DARB) CL_OpenGL::get_proc_address("glCompressedTexSubImage3DARB");
	functions->getCompressedTexImage = (CL_GLFunctions::ptr_glGetCompressedTexImageARB) CL_OpenGL::get_proc_address("glGetCompressedTexImageARB");
	functions->sampleCoverage = (CL_GLFunctions::ptr_glSampleCoverageARB) CL_OpenGL::get_proc_address("glSampleCoverageARB");
	functions->glLoadTransposeMatrixd = (CL_GLFunctions::ptr_glLoadTransposeMatrixdARB) CL_OpenGL::get_proc_address("glLoadTransposeMatrixdARB");
	functions->glLoadTransposeMatrixf = (CL_GLFunctions::ptr_glLoadTransposeMatrixfARB) CL_OpenGL::get_proc_address("glLoadTransposeMatrixfARB");
	functions->glMultTransposeMatrixd = (CL_GLFunctions::ptr_glMultTransposeMatrixdARB) CL_OpenGL::get_proc_address("glMultTransposeMatrixdARB");
	functions->glMultTransposeMatrixf = (CL_GLFunctions::ptr_glMultTransposeMatrixfARB) CL_OpenGL::get_proc_address("glMultTransposeMatrixfARB");
#endif

	// Binds for OpenGL 1.4:

#ifdef __APPLE__
	functions->fogCoordd = (CL_GLFunctions::ptr_glFogCoorddEXT) &glFogCoordd;
	functions->fogCoorddv = (CL_GLFunctions::ptr_glFogCoorddvEXT) &glFogCoorddv;
	functions->fogCoordf = (CL_GLFunctions::ptr_glFogCoordfEXT) &glFogCoordf;
	functions->fogCoordfv = (CL_GLFunctions::ptr_glFogCoordfvEXT) &glFogCoordfv;
	functions->fogCoordPointer = (CL_GLFunctions::ptr_glFogCoordPointerEXT) &glFogCoordPointer;
	functions->multiDrawArrays = (CL_GLFunctions::ptr_glMultiDrawArraysEXT) &glMultiDrawArrays;
	functions->multiDrawElementsEXT = (CL_GLFunctions::ptr_glMultiDrawElementsEXT) &glMultiDrawElements;
	functions->pointParameterf = (CL_GLFunctions::ptr_glPointParameterfARB) &glPointParameterf;
	functions->pointParameterfv = (CL_GLFunctions::ptr_glPointParameterfvARB) &glPointParameterfv;
	functions->secondaryColor3b = (CL_GLFunctions::ptr_glSecondaryColor3bEXT) &glSecondaryColor3b;
	functions->secondaryColor3bv = (CL_GLFunctions::ptr_glSecondaryColor3bvEXT) &glSecondaryColor3bv;
	functions->secondaryColor3d = (CL_GLFunctions::ptr_glSecondaryColor3dEXT) &glSecondaryColor3d;
	functions->secondaryColor3dv = (CL_GLFunctions::ptr_glSecondaryColor3dvEXT) &glSecondaryColor3dv;
	functions->secondaryColor3f = (CL_GLFunctions::ptr_glSecondaryColor3fEXT) &glSecondaryColor3f;
	functions->secondaryColor3fv = (CL_GLFunctions::ptr_glSecondaryColor3fvEXT) &glSecondaryColor3fv;
	functions->secondaryColor3i = (CL_GLFunctions::ptr_glSecondaryColor3iEXT) &glSecondaryColor3i;
	functions->secondaryColor3iv = (CL_GLFunctions::ptr_glSecondaryColor3ivEXT) &glSecondaryColor3iv;
	functions->secondaryColor3s = (CL_GLFunctions::ptr_glSecondaryColor3sEXT) &glSecondaryColor3s;
	functions->secondaryColor3sv = (CL_GLFunctions::ptr_glSecondaryColor3svEXT) &glSecondaryColor3sv;
	functions->secondaryColor3ub = (CL_GLFunctions::ptr_glSecondaryColor3ubEXT) &glSecondaryColor3ub;
	functions->secondaryColor3ubv = (CL_GLFunctions::ptr_glSecondaryColor3ubvEXT) &glSecondaryColor3ubv;
	functions->secondaryColor3ui = (CL_GLFunctions::ptr_glSecondaryColor3uiEXT) &glSecondaryColor3ui;
	functions->secondaryColor3uiv = (CL_GLFunctions::ptr_glSecondaryColor3uivEXT) &glSecondaryColor3uiv;
	functions->secondaryColor3us = (CL_GLFunctions::ptr_glSecondaryColor3usEXT) &glSecondaryColor3us;
	functions->secondaryColor3usv = (CL_GLFunctions::ptr_glSecondaryColor3usvEXT) &glSecondaryColor3usv;
	functions->secondaryColorPointer = (CL_GLFunctions::ptr_glSecondaryColorPointerEXT) &glSecondaryColorPointer;
	functions->blendFuncSeparate = (CL_GLFunctions::ptr_glBlendFuncSeparateEXT) &glBlendFuncSeparate;
	
	// need to manually link these, since some llama linux opengl driver says its 1.4 while its not! >:(
/*	
	functions->windowPos2d = (CL_GLFunctions::ptr_glWindowPos2dARB) CL_OpenGL::get_proc_address("glWindowPos2dARB");
	functions->windowPos2dv = (CL_GLFunctions::ptr_glWindowPos2dvARB) CL_OpenGL::get_proc_address("glWindowPos2dvARB");
	functions->windowPos2f = (CL_GLFunctions::ptr_glWindowPos2fARB) CL_OpenGL::get_proc_address("glWindowPos2fARB");
	functions->windowPos2fv = (CL_GLFunctions::ptr_glWindowPos2fvARB) CL_OpenGL::get_proc_address("glWindowPos2fvARB");
	functions->windowPos2i = (CL_GLFunctions::ptr_glWindowPos2iARB) CL_OpenGL::get_proc_address("glWindowPos2iARB");
	functions->windowPos2iv = (CL_GLFunctions::ptr_glWindowPos2ivARB) CL_OpenGL::get_proc_address("glWindowPos2ivARB");
	functions->windowPos2s = (CL_GLFunctions::ptr_glWindowPos2sARB) CL_OpenGL::get_proc_address("glWindowPos2sARB");
	functions->windowPos2sv = (CL_GLFunctions::ptr_glWindowPos2svARB) CL_OpenGL::get_proc_address("glWindowPos2svARB");
	functions->windowPos3d = (CL_GLFunctions::ptr_glWindowPos3dARB) CL_OpenGL::get_proc_address("glWindowPos3dARB");
	functions->windowPos3dv = (CL_GLFunctions::ptr_glWindowPos3dvARB) CL_OpenGL::get_proc_address("glWindowPos3dvARB");
	functions->windowPos3f = (CL_GLFunctions::ptr_glWindowPos3fARB) CL_OpenGL::get_proc_address("glWindowPos3fARB");
	functions->windowPos3fv = (CL_GLFunctions::ptr_glWindowPos3fvARB) CL_OpenGL::get_proc_address("glWindowPos3fvARB");
	functions->windowPos3i = (CL_GLFunctions::ptr_glWindowPos3iARB) CL_OpenGL::get_proc_address("glWindowPos3iARB");
	functions->windowPos3iv = (CL_GLFunctions::ptr_glWindowPos3ivARB) CL_OpenGL::get_proc_address("glWindowPos3ivARB");
	functions->windowPos3s = (CL_GLFunctions::ptr_glWindowPos3sARB) CL_OpenGL::get_proc_address("glWindowPos3sARB");
	functions->windowPos3sv = (CL_GLFunctions::ptr_glWindowPos3svARB) CL_OpenGL::get_proc_address("glWindowPos3svARB");
*/
	functions->windowPos2d = &glWindowPos2d;
	functions->windowPos2dv = &glWindowPos2dv;
	functions->windowPos2f = &glWindowPos2f;
	functions->windowPos2fv = &glWindowPos2fv;
	functions->windowPos2i = &glWindowPos2i;
	functions->windowPos2iv = &glWindowPos2iv;
	functions->windowPos2s = &glWindowPos2s;
	functions->windowPos2sv = &glWindowPos2sv;
	functions->windowPos3d = &glWindowPos3d;
	functions->windowPos3dv = &glWindowPos3dv;
	functions->windowPos3f = &glWindowPos3f;
	functions->windowPos3fv = &glWindowPos3fv;
	functions->windowPos3i = &glWindowPos3i;
	functions->windowPos3iv = &glWindowPos3iv;
	functions->windowPos3s = &glWindowPos3s;
	functions->windowPos3sv = &glWindowPos3sv;
#else
	functions->fogCoordd = (CL_GLFunctions::ptr_glFogCoorddEXT) CL_OpenGL::get_proc_address("glFogCoorddEXT");
	functions->fogCoorddv = (CL_GLFunctions::ptr_glFogCoorddvEXT) CL_OpenGL::get_proc_address("glFogCoorddvEXT");
	functions->fogCoordf = (CL_GLFunctions::ptr_glFogCoordfEXT) CL_OpenGL::get_proc_address("glFogCoordfEXT");
	functions->fogCoordfv = (CL_GLFunctions::ptr_glFogCoordfvEXT) CL_OpenGL::get_proc_address("glFogCoordfvEXT");
	functions->fogCoordPointer = (CL_GLFunctions::ptr_glFogCoordPointerEXT) CL_OpenGL::get_proc_address("glFogCoordPointerEXT");
	functions->multiDrawArrays = (CL_GLFunctions::ptr_glMultiDrawArraysEXT) CL_OpenGL::get_proc_address("glMultiDrawArraysEXT");
	functions->multiDrawElementsEXT = (CL_GLFunctions::ptr_glMultiDrawElementsEXT) CL_OpenGL::get_proc_address("glMultiDrawElementsEXT");
	functions->pointParameterf = (CL_GLFunctions::ptr_glPointParameterfARB) CL_OpenGL::get_proc_address("glPointParameterfARB");
	functions->pointParameterfv = (CL_GLFunctions::ptr_glPointParameterfvARB) CL_OpenGL::get_proc_address("glPointParameterfvARB");
	functions->secondaryColor3b = (CL_GLFunctions::ptr_glSecondaryColor3bEXT) CL_OpenGL::get_proc_address("glSecondaryColor3bEXT");
	functions->secondaryColor3bv = (CL_GLFunctions::ptr_glSecondaryColor3bvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3bvEXT");
	functions->secondaryColor3d = (CL_GLFunctions::ptr_glSecondaryColor3dEXT) CL_OpenGL::get_proc_address("glSecondaryColor3dEXT");
	functions->secondaryColor3dv = (CL_GLFunctions::ptr_glSecondaryColor3dvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3dvEXT");
	functions->secondaryColor3f = (CL_GLFunctions::ptr_glSecondaryColor3fEXT) CL_OpenGL::get_proc_address("glSecondaryColor3fEXT");
	functions->secondaryColor3fv = (CL_GLFunctions::ptr_glSecondaryColor3fvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3fvEXT");
	functions->secondaryColor3i = (CL_GLFunctions::ptr_glSecondaryColor3iEXT) CL_OpenGL::get_proc_address("glSecondaryColor3iEXT");
	functions->secondaryColor3iv = (CL_GLFunctions::ptr_glSecondaryColor3ivEXT) CL_OpenGL::get_proc_address("glSecondaryColor3ivEXT");
	functions->secondaryColor3s = (CL_GLFunctions::ptr_glSecondaryColor3sEXT) CL_OpenGL::get_proc_address("glSecondaryColor3sEXT");
	functions->secondaryColor3sv = (CL_GLFunctions::ptr_glSecondaryColor3svEXT) CL_OpenGL::get_proc_address("glSecondaryColor3svEXT");
	functions->secondaryColor3ub = (CL_GLFunctions::ptr_glSecondaryColor3ubEXT) CL_OpenGL::get_proc_address("glSecondaryColor3ubEXT");
	functions->secondaryColor3ubv = (CL_GLFunctions::ptr_glSecondaryColor3ubvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3ubvEXT");
	functions->secondaryColor3ui = (CL_GLFunctions::ptr_glSecondaryColor3uiEXT) CL_OpenGL::get_proc_address("glSecondaryColor3uiEXT");
	functions->secondaryColor3uiv = (CL_GLFunctions::ptr_glSecondaryColor3uivEXT) CL_OpenGL::get_proc_address("glSecondaryColor3uivEXT");
	functions->secondaryColor3us = (CL_GLFunctions::ptr_glSecondaryColor3usEXT) CL_OpenGL::get_proc_address("glSecondaryColor3usEXT");
	functions->secondaryColor3usv = (CL_GLFunctions::ptr_glSecondaryColor3usvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3usvEXT");
	functions->secondaryColorPointer = (CL_GLFunctions::ptr_glSecondaryColorPointerEXT) CL_OpenGL::get_proc_address("glSecondaryColorPointerEXT");
	functions->blendFuncSeparate = (CL_GLFunctions::ptr_glBlendFuncSeparateEXT) CL_OpenGL::get_proc_address("glBlendFuncSeparateEXT");
	functions->windowPos2d = (CL_GLFunctions::ptr_glWindowPos2dARB) CL_OpenGL::get_proc_address("glWindowPos2dARB");
	functions->windowPos2dv = (CL_GLFunctions::ptr_glWindowPos2dvARB) CL_OpenGL::get_proc_address("glWindowPos2dvARB");
	functions->windowPos2f = (CL_GLFunctions::ptr_glWindowPos2fARB) CL_OpenGL::get_proc_address("glWindowPos2fARB");
	functions->windowPos2fv = (CL_GLFunctions::ptr_glWindowPos2fvARB) CL_OpenGL::get_proc_address("glWindowPos2fvARB");
	functions->windowPos2i = (CL_GLFunctions::ptr_glWindowPos2iARB) CL_OpenGL::get_proc_address("glWindowPos2iARB");
	functions->windowPos2iv = (CL_GLFunctions::ptr_glWindowPos2ivARB) CL_OpenGL::get_proc_address("glWindowPos2ivARB");
	functions->windowPos2s = (CL_GLFunctions::ptr_glWindowPos2sARB) CL_OpenGL::get_proc_address("glWindowPos2sARB");
	functions->windowPos2sv = (CL_GLFunctions::ptr_glWindowPos2svARB) CL_OpenGL::get_proc_address("glWindowPos2svARB");
	functions->windowPos3d = (CL_GLFunctions::ptr_glWindowPos3dARB) CL_OpenGL::get_proc_address("glWindowPos3dARB");
	functions->windowPos3dv = (CL_GLFunctions::ptr_glWindowPos3dvARB) CL_OpenGL::get_proc_address("glWindowPos3dvARB");
	functions->windowPos3f = (CL_GLFunctions::ptr_glWindowPos3fARB) CL_OpenGL::get_proc_address("glWindowPos3fARB");
	functions->windowPos3fv = (CL_GLFunctions::ptr_glWindowPos3fvARB) CL_OpenGL::get_proc_address("glWindowPos3fvARB");
	functions->windowPos3i = (CL_GLFunctions::ptr_glWindowPos3iARB) CL_OpenGL::get_proc_address("glWindowPos3iARB");
	functions->windowPos3iv = (CL_GLFunctions::ptr_glWindowPos3ivARB) CL_OpenGL::get_proc_address("glWindowPos3ivARB");
	functions->windowPos3s = (CL_GLFunctions::ptr_glWindowPos3sARB) CL_OpenGL::get_proc_address("glWindowPos3sARB");
	functions->windowPos3sv = (CL_GLFunctions::ptr_glWindowPos3svARB) CL_OpenGL::get_proc_address("glWindowPos3svARB");
#endif

	// Binds for OpenGL 1.5:

#ifdef __NOBODY_YET_HEHE__
	functions->bindBuffer = (CL_GLFunctions::ptr_glBindBufferARB) &glBindBuffer;
	functions->deleteBuffers = (CL_GLFunctions::ptr_glDeleteBuffersARB) &glDeleteBuffers;
	functions->genBuffers = (CL_GLFunctions::ptr_glGenBuffersARB) &glGenBuffers;
	functions->isBuffer = (CL_GLFunctions::ptr_glIsBufferARB) &glIsBuffer;
	functions->bufferData = (CL_GLFunctions::ptr_glBufferDataARB) &glBufferData;
	functions->bufferSubData = (CL_GLFunctions::ptr_glBufferSubDataARB) &glBufferSubData;
	functions->getBufferSubData = (CL_GLFunctions::ptr_glGetBufferSubDataARB) &glGetBufferSubData;
	functions->mapBuffer = (CL_GLFunctions::ptr_glMapBufferARB) &glMapBuffer;
	functions->unmapBuffer = (CL_GLFunctions::ptr_glUnmapBufferARB) &glUnmapBuffer;
	functions->getBufferParameteriv = (CL_GLFunctions::ptr_glGetBufferParameterivARB) &glGetBufferParameteriv;
	functions->getBufferPointerv = (CL_GLFunctions::ptr_glGetBufferPointervARB) &glGetBufferPointerv;
	functions->genQueries = (CL_GLFunctions::ptr_glGenQueriesARB) &glGenQueries;
	functions->deleteQueries = (CL_GLFunctions::ptr_glDeleteQueriesARB) &glDeleteQueries;
	functions->isQuery = (CL_GLFunctions::ptr_glIsQueryARB) &glIsQuery;
	functions->beginQuery = (CL_GLFunctions::ptr_glBeginQueryARB) &glBeginQuery;
	functions->endQuery = (CL_GLFunctions::ptr_glEndQueryARB) &glEndQuery;
	functions->getQueryiv = (CL_GLFunctions::ptr_glGetQueryivARB) &glGetQueryiv;
	functions->getQueryObjectiv = (CL_GLFunctions::ptr_glGetQueryObjectivARB) &glGetQueryObjectiv;
	functions->getQueryObjectuiv = (CL_GLFunctions::ptr_glGetQueryObjectuivARB) &glGetQueryObjectuiv;
#else
	functions->bindBuffer = (CL_GLFunctions::ptr_glBindBufferARB) CL_OpenGL::get_proc_address("glBindBufferARB");
	functions->deleteBuffers = (CL_GLFunctions::ptr_glDeleteBuffersARB) CL_OpenGL::get_proc_address("glDeleteBuffersARB");
	functions->genBuffers = (CL_GLFunctions::ptr_glGenBuffersARB) CL_OpenGL::get_proc_address("glGenBuffersARB");
	functions->isBuffer = (CL_GLFunctions::ptr_glIsBufferARB) CL_OpenGL::get_proc_address("glIsBufferARB");
	functions->bufferData = (CL_GLFunctions::ptr_glBufferDataARB) CL_OpenGL::get_proc_address("glBufferDataARB");
	functions->bufferSubData = (CL_GLFunctions::ptr_glBufferSubDataARB) CL_OpenGL::get_proc_address("glBufferSubDataARB");
	functions->getBufferSubData = (CL_GLFunctions::ptr_glGetBufferSubDataARB) CL_OpenGL::get_proc_address("glGetBufferSubDataARB");
	functions->mapBuffer = (CL_GLFunctions::ptr_glMapBufferARB) CL_OpenGL::get_proc_address("glMapBufferARB");
	functions->unmapBuffer = (CL_GLFunctions::ptr_glUnmapBufferARB) CL_OpenGL::get_proc_address("glUnmapBufferARB");
	functions->getBufferParameteriv = (CL_GLFunctions::ptr_glGetBufferParameterivARB) CL_OpenGL::get_proc_address("glGetBufferParameterivARB");
	functions->getBufferPointerv = (CL_GLFunctions::ptr_glGetBufferPointervARB) CL_OpenGL::get_proc_address("glGetBufferPointervARB");
	functions->genQueries = (CL_GLFunctions::ptr_glGenQueriesARB) CL_OpenGL::get_proc_address("glGenQueriesARB");
	functions->deleteQueries = (CL_GLFunctions::ptr_glDeleteQueriesARB) CL_OpenGL::get_proc_address("glDeleteQueriesARB");
	functions->isQuery = (CL_GLFunctions::ptr_glIsQueryARB) CL_OpenGL::get_proc_address("glIsQueryARB");
	functions->beginQuery = (CL_GLFunctions::ptr_glBeginQueryARB) CL_OpenGL::get_proc_address("glBeginQueryARB");
	functions->endQuery = (CL_GLFunctions::ptr_glEndQueryARB) CL_OpenGL::get_proc_address("glEndQueryARB");
	functions->getQueryiv = (CL_GLFunctions::ptr_glGetQueryivARB) CL_OpenGL::get_proc_address("glGetQueryivARB");
	functions->getQueryObjectiv = (CL_GLFunctions::ptr_glGetQueryObjectivARB) CL_OpenGL::get_proc_address("glGetQueryObjectivARB");
	functions->getQueryObjectuiv = (CL_GLFunctions::ptr_glGetQueryObjectuivARB) CL_OpenGL::get_proc_address("glGetQueryObjectuivARB");
#endif

	// Binds for OpenGL 2.0:

#ifdef __NOBODY_YET_HEHE__
	functions->deleteShader = (CL_GLFunctions::ptr_glDeleteObjectARB) &glDeleteShader;
	functions->deleteProgram = (CL_GLFunctions::ptr_glDeleteObjectARB) &glDeleteProgram;
	functions->getHandle = (CL_GLFunctions::ptr_glGetHandleARB) &glGetHandle;
	functions->detachShader = (CL_GLFunctions::ptr_glDetachObjectARB) &glDetachShader;
	functions->createShader = (CL_GLFunctions::ptr_glCreateShaderObjectARB) &glCreateShader;
	functions->shaderSource = (CL_GLFunctions::ptr_glShaderSourceARB) &glShaderSource;
	functions->compileShader = (CL_GLFunctions::ptr_glCompileShaderARB) &glCompileShader;
	functions->createProgram = (CL_GLFunctions::ptr_glCreateProgramObjectARB) &glCreateProgram;
	functions->attachShader = (CL_GLFunctions::ptr_glAttachObjectARB) &glAttachShader;
	functions->linkProgram = (CL_GLFunctions::ptr_glLinkProgramARB) &glLinkProgram;
	functions->useProgram = (CL_GLFunctions::ptr_glUseProgramObjectARB) &glUseProgram;
	functions->validateProgram = (CL_GLFunctions::ptr_glValidateProgramARB) &glValidateProgram;
	functions->uniform1f = (CL_GLFunctions::ptr_glUniform1fARB) &glUniform1f;
	functions->uniform2f = (CL_GLFunctions::ptr_glUniform2fARB) &glUniform2f;
	functions->uniform3f = (CL_GLFunctions::ptr_glUniform3fARB) &glUniform3f;
	functions->uniform4f = (CL_GLFunctions::ptr_glUniform4fARB) &glUniform4f;
	functions->uniform1i = (CL_GLFunctions::ptr_glUniform1iARB) &glUniform1i;
	functions->uniform2i = (CL_GLFunctions::ptr_glUniform2iARB) &glUniform2i;
	functions->uniform3i = (CL_GLFunctions::ptr_glUniform3iARB) &glUniform3i;
	functions->uniform4i = (CL_GLFunctions::ptr_glUniform4iARB) &glUniform4i;
	functions->uniform1fv = (CL_GLFunctions::ptr_glUniform1fvARB) &glUniform1fv;
	functions->uniform2fv = (CL_GLFunctions::ptr_glUniform2fvARB) &glUniform2fv;
	functions->uniform3fv = (CL_GLFunctions::ptr_glUniform3fvARB) &glUniform3fv;
	functions->uniform4fv = (CL_GLFunctions::ptr_glUniform4fvARB) &glUniform4fv;
	functions->uniform1iv = (CL_GLFunctions::ptr_glUniform1ivARB) &glUniform1iv;
	functions->uniform2iv = (CL_GLFunctions::ptr_glUniform2ivARB) &glUniform2iv;
	functions->uniform3iv = (CL_GLFunctions::ptr_glUniform3ivARB) &glUniform3iv;
	functions->uniform4iv = (CL_GLFunctions::ptr_glUniform4ivARB) &glUniform4iv;
	functions->uniformMatrix2fv = (CL_GLFunctions::ptr_glUniformMatrix2fvARB) &glUniformMatrix2fv;
	functions->uniformMatrix3fv = (CL_GLFunctions::ptr_glUniformMatrix3fvARB) &glUniformMatrix3fv;
	functions->uniformMatrix4fv = (CL_GLFunctions::ptr_glUniformMatrix4fvARB) &glUniformMatrix4fv;
	functions->getProgramfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) &glProgramfv;
	functions->getProgramiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) &glProgramiv;
	functions->getShaderfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) &glShaderfv;
	functions->getShaderiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) &glShaderiv;
	functions->getShaderInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) &glGetShaderInfoLog;
	functions->getProgramInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) &glGetProgramInfoLog;
	functions->getAttachedShaders = (CL_GLFunctions::ptr_glGetAttachedObjectsARB) &glGetAttachedShaders;
	functions->getUniformLocation = (CL_GLFunctions::ptr_glGetUniformLocationARB) &glGetUniformLocation;
	functions->getActiveUniform = (CL_GLFunctions::ptr_glGetActiveUniformARB) &glGetActiveUniform;
	functions->getUniformfv = (CL_GLFunctions::ptr_glGetUniformfvARB) &glGetUniformfv;
	functions->getUniformiv = (CL_GLFunctions::ptr_glGetUniformivARB) &glGetUniformiv;
	functions->getShaderSource = (CL_GLFunctions::ptr_glGetShaderSourceARB) &glGetShaderSource;
	functions->vertexAttrib1f = (CL_GLFunctions::ptr_glVertexAttrib1fARB) &glVertexAttrib1f;
	functions->vertexAttrib1s = (CL_GLFunctions::ptr_glVertexAttrib1sARB) &glVertexAttrib1s;
	functions->vertexAttrib1d = (CL_GLFunctions::ptr_glVertexAttrib1dARB) &glVertexAttrib1d;
	functions->vertexAttrib2f = (CL_GLFunctions::ptr_glVertexAttrib2fARB) &glVertexAttrib2f;
	functions->vertexAttrib2s = (CL_GLFunctions::ptr_glVertexAttrib2sARB) &glVertexAttrib2s;
	functions->vertexAttrib2d = (CL_GLFunctions::ptr_glVertexAttrib2dARB) &glVertexAttrib2d;
	functions->vertexAttrib3f = (CL_GLFunctions::ptr_glVertexAttrib3fARB) &glVertexAttrib3f;
	functions->vertexAttrib3s = (CL_GLFunctions::ptr_glVertexAttrib3sARB) &glVertexAttrib3s;
	functions->vertexAttrib3d = (CL_GLFunctions::ptr_glVertexAttrib3dARB) &glVertexAttrib3d;
	functions->vertexAttrib4f = (CL_GLFunctions::ptr_glVertexAttrib4fARB) &glVertexAttrib4f;
	functions->vertexAttrib4s = (CL_GLFunctions::ptr_glVertexAttrib4sARB) &glVertexAttrib4s;
	functions->vertexAttrib4d = (CL_GLFunctions::ptr_glVertexAttrib4dARB) &glVertexAttrib4d;
	functions->vertexAttrib4Nub = (CL_GLFunctions::ptr_glVertexAttrib4NubARB) &glVertexAttrib4Nub;
	functions->vertexAttrib1fv = (CL_GLFunctions::ptr_glVertexAttrib1fvARB) &glVertexAttrib1fv;
	functions->vertexAttrib1sv = (CL_GLFunctions::ptr_glVertexAttrib1svARB) &glVertexAttrib1sv;
	functions->vertexAttrib1dv = (CL_GLFunctions::ptr_glVertexAttrib1dvARB) &glVertexAttrib1dv;
	functions->vertexAttrib2fv = (CL_GLFunctions::ptr_glVertexAttrib2fvARB) &glVertexAttrib2fv;
	functions->vertexAttrib2sv = (CL_GLFunctions::ptr_glVertexAttrib2svARB) &glVertexAttrib2sv;
	functions->vertexAttrib2dv = (CL_GLFunctions::ptr_glVertexAttrib2dvARB) &glVertexAttrib2dv;
	functions->vertexAttrib3fv = (CL_GLFunctions::ptr_glVertexAttrib3fvARB) &glVertexAttrib3fv;
	functions->vertexAttrib3sv = (CL_GLFunctions::ptr_glVertexAttrib3svARB) &glVertexAttrib3sv;
	functions->vertexAttrib3dv = (CL_GLFunctions::ptr_glVertexAttrib3dvARB) &glVertexAttrib3dv;
	functions->vertexAttrib4fv = (CL_GLFunctions::ptr_glVertexAttrib4fvARB) &glVertexAttrib4fv;
	functions->vertexAttrib4sv = (CL_GLFunctions::ptr_glVertexAttrib4svARB) &glVertexAttrib4sv;
	functions->vertexAttrib4dv = (CL_GLFunctions::ptr_glVertexAttrib4dvARB) &glVertexAttrib4dv;
	functions->vertexAttrib4iv = (CL_GLFunctions::ptr_glVertexAttrib4ivARB) &glVertexAttrib4iv;
	functions->vertexAttrib4bv = (CL_GLFunctions::ptr_glVertexAttrib4bvARB) &glVertexAttrib4bv;
	functions->vertexAttrib4ubv = (CL_GLFunctions::ptr_glVertexAttrib4ubvARB) &glVertexAttrib4ubv;
	functions->vertexAttrib4usv = (CL_GLFunctions::ptr_glVertexAttrib4usvARB) &glVertexAttrib4usv;
	functions->vertexAttrib4uiv = (CL_GLFunctions::ptr_glVertexAttrib4uivARB) &glVertexAttrib4uiv;
	functions->vertexAttrib4Nbv = (CL_GLFunctions::ptr_glVertexAttrib4NbvARB) &glVertexAttrib4Nbv;
	functions->vertexAttrib4Nsv = (CL_GLFunctions::ptr_glVertexAttrib4NsvARB) &glVertexAttrib4Nsv;
	functions->vertexAttrib4Niv = (CL_GLFunctions::ptr_glVertexAttrib4NivARB) &glVertexAttrib4Niv;
	functions->vertexAttrib4Nubv = (CL_GLFunctions::ptr_glVertexAttrib4NubvARB) &glVertexAttrib4Nubv;
	functions->vertexAttrib4Nusv = (CL_GLFunctions::ptr_glVertexAttrib4NusvARB) &glVertexAttrib4Nusv;
	functions->vertexAttrib4Nuiv = (CL_GLFunctions::ptr_glVertexAttrib4NuivARB) &glVertexAttrib4Nuiv;
	functions->vertexAttribPointer = (CL_GLFunctions::ptr_glVertexAttribPointerARB) &glVertexAttribPointer;
	functions->enableVertexAttribArray = (CL_GLFunctions::ptr_glEnableVertexAttribArrayARB) &glEnableVertexAttribArray;
	functions->disableVertexAttribArray = (CL_GLFunctions::ptr_glDisableVertexAttribArrayARB) &glDisableVertexAttribArray;
	functions->bindAttribLocation = (CL_GLFunctions::ptr_glBindAttribLocationARB) &glBindAttribLocation;
	functions->getActiveAttrib = (CL_GLFunctions::ptr_glGetActiveAttribARB) &glGetActiveAttrib;
	functions->getAttribLocation = (CL_GLFunctions::ptr_glGetAttribLocationARB) &glGetAttribLocation;
	functions->getVertexAttribdv = (CL_GLFunctions::ptr_glGetVertexAttribdvARB) &glGetVertexAttribdv;
	functions->getVertexAttribfv = (CL_GLFunctions::ptr_glGetVertexAttribfvARB) &glGetVertexAttribfv;
	functions->getVertexAttribiv = (CL_GLFunctions::ptr_glGetVertexAttribivARB) &glGetVertexAttribiv;
	functions->getVertexAttribPointerv = (CL_GLFunctions::ptr_glGetVertexAttribPointervARB) &glGetVertexAttribPointerv;
	functions->drawBuffers = (CL_GLFunctions::ptr_glDrawBuffersARB) &glDrawBuffers;
	functions->stencilFuncSeparate = (CL_GLFunctions::ptr_glStencilFuncSeparateATI) &glStencilFuncSeparate;
	functions->stencilOpSeparate = (CL_GLFunctions::ptr_glStencilOpSeparateATI) &glStencilOpSeparate;
#elsif __APPLE__
	functions->deleteShader = (CL_GLFunctions::ptr_glDeleteObjectARB) CL_OpenGL::get_proc_address("glDeleteShader");
	functions->deleteProgram = (CL_GLFunctions::ptr_glDeleteObjectARB) CL_OpenGL::get_proc_address("glDeleteProgram");
	functions->getHandle = (CL_GLFunctions::ptr_glGetHandleARB) CL_OpenGL::get_proc_address("glGetHandle");
	functions->detachShader = (CL_GLFunctions::ptr_glDetachObjectARB) CL_OpenGL::get_proc_address("glDetachShader");
	functions->createShader = (CL_GLFunctions::ptr_glCreateShaderObjectARB) CL_OpenGL::get_proc_address("glCreateShader");
	functions->shaderSource = (CL_GLFunctions::ptr_glShaderSourceARB) CL_OpenGL::get_proc_address("glShaderSource");
	functions->compileShader = (CL_GLFunctions::ptr_glCompileShaderARB) CL_OpenGL::get_proc_address("glCompileShader");
	functions->createProgram = (CL_GLFunctions::ptr_glCreateProgramObjectARB) CL_OpenGL::get_proc_address("glCreateProgram");
	functions->attachShader = (CL_GLFunctions::ptr_glAttachObjectARB) CL_OpenGL::get_proc_address("glAttachShader");
	functions->linkProgram = (CL_GLFunctions::ptr_glLinkProgramARB) CL_OpenGL::get_proc_address("glLinkProgram");
	functions->useProgram = (CL_GLFunctions::ptr_glUseProgramObjectARB) CL_OpenGL::get_proc_address("glUseProgram");
	functions->validateProgram = (CL_GLFunctions::ptr_glValidateProgramARB) CL_OpenGL::get_proc_address("glValidateProgram");
	functions->uniform1f = (CL_GLFunctions::ptr_glUniform1fARB) CL_OpenGL::get_proc_address("glUniform1f");
	functions->uniform2f = (CL_GLFunctions::ptr_glUniform2fARB) CL_OpenGL::get_proc_address("glUniform2f");
	functions->uniform3f = (CL_GLFunctions::ptr_glUniform3fARB) CL_OpenGL::get_proc_address("glUniform3f");
	functions->uniform4f = (CL_GLFunctions::ptr_glUniform4fARB) CL_OpenGL::get_proc_address("glUniform4f");
	functions->uniform1i = (CL_GLFunctions::ptr_glUniform1iARB) CL_OpenGL::get_proc_address("glUniform1i");
	functions->uniform2i = (CL_GLFunctions::ptr_glUniform2iARB) CL_OpenGL::get_proc_address("glUniform2i");
	functions->uniform3i = (CL_GLFunctions::ptr_glUniform3iARB) CL_OpenGL::get_proc_address("glUniform3i");
	functions->uniform4i = (CL_GLFunctions::ptr_glUniform4iARB) CL_OpenGL::get_proc_address("glUniform4i");
	functions->uniform1fv = (CL_GLFunctions::ptr_glUniform1fvARB) CL_OpenGL::get_proc_address("glUniform1fv");
	functions->uniform2fv = (CL_GLFunctions::ptr_glUniform2fvARB) CL_OpenGL::get_proc_address("glUniform2fv");
	functions->uniform3fv = (CL_GLFunctions::ptr_glUniform3fvARB) CL_OpenGL::get_proc_address("glUniform3fv");
	functions->uniform4fv = (CL_GLFunctions::ptr_glUniform4fvARB) CL_OpenGL::get_proc_address("glUniform4fv");
	functions->uniform1iv = (CL_GLFunctions::ptr_glUniform1ivARB) CL_OpenGL::get_proc_address("glUniform1iv");
	functions->uniform2iv = (CL_GLFunctions::ptr_glUniform2ivARB) CL_OpenGL::get_proc_address("glUniform2iv");
	functions->uniform3iv = (CL_GLFunctions::ptr_glUniform3ivARB) CL_OpenGL::get_proc_address("glUniform3iv");
	functions->uniform4iv = (CL_GLFunctions::ptr_glUniform4ivARB) CL_OpenGL::get_proc_address("glUniform4iv");
	functions->uniformMatrix2fv = (CL_GLFunctions::ptr_glUniformMatrix2fvARB) CL_OpenGL::get_proc_address("glUniformMatrix2fv");
	functions->uniformMatrix3fv = (CL_GLFunctions::ptr_glUniformMatrix3fvARB) CL_OpenGL::get_proc_address("glUniformMatrix3fv");
	functions->uniformMatrix4fv = (CL_GLFunctions::ptr_glUniformMatrix4fvARB) CL_OpenGL::get_proc_address("glUniformMatrix4fv");
	functions->getProgramfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) CL_OpenGL::get_proc_address("glGetProgramfv");
	functions->getProgramiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) CL_OpenGL::get_proc_address("glGetProgramiv");
	functions->getShaderfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) CL_OpenGL::get_proc_address("glGetShaderfv");
	functions->getShaderiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) CL_OpenGL::get_proc_address("glGetShaderiv");
	functions->getShaderInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) CL_OpenGL::get_proc_address("glGetShaderInfoLog");
	functions->getProgramInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) CL_OpenGL::get_proc_address("glGetProgramInfoLog");
	functions->getAttachedShaders = (CL_GLFunctions::ptr_glGetAttachedObjectsARB) CL_OpenGL::get_proc_address("glGetAttachedShaders");
	functions->getUniformLocation = (CL_GLFunctions::ptr_glGetUniformLocationARB) CL_OpenGL::get_proc_address("glGetUniformLocation");
	functions->getActiveUniform = (CL_GLFunctions::ptr_glGetActiveUniformARB) CL_OpenGL::get_proc_address("glGetActiveUniform");
	functions->getUniformfv = (CL_GLFunctions::ptr_glGetUniformfvARB) CL_OpenGL::get_proc_address("glGetUniformfv");
	functions->getUniformiv = (CL_GLFunctions::ptr_glGetUniformivARB) CL_OpenGL::get_proc_address("glGetUniformiv");
	functions->getShaderSource = (CL_GLFunctions::ptr_glGetShaderSourceARB) CL_OpenGL::get_proc_address("glGetShaderSource");
	functions->vertexAttrib1f = (CL_GLFunctions::ptr_glVertexAttrib1fARB) CL_OpenGL::get_proc_address("glVertexAttrib1f");
	functions->vertexAttrib1s = (CL_GLFunctions::ptr_glVertexAttrib1sARB) CL_OpenGL::get_proc_address("glVertexAttrib1s");
	functions->vertexAttrib1d = (CL_GLFunctions::ptr_glVertexAttrib1dARB) CL_OpenGL::get_proc_address("glVertexAttrib1d");
	functions->vertexAttrib2f = (CL_GLFunctions::ptr_glVertexAttrib2fARB) CL_OpenGL::get_proc_address("glVertexAttrib2f");
	functions->vertexAttrib2s = (CL_GLFunctions::ptr_glVertexAttrib2sARB) CL_OpenGL::get_proc_address("glVertexAttrib2s");
	functions->vertexAttrib2d = (CL_GLFunctions::ptr_glVertexAttrib2dARB) CL_OpenGL::get_proc_address("glVertexAttrib2d");
	functions->vertexAttrib3f = (CL_GLFunctions::ptr_glVertexAttrib3fARB) CL_OpenGL::get_proc_address("glVertexAttrib3f");
	functions->vertexAttrib3s = (CL_GLFunctions::ptr_glVertexAttrib3sARB) CL_OpenGL::get_proc_address("glVertexAttrib3s");
	functions->vertexAttrib3d = (CL_GLFunctions::ptr_glVertexAttrib3dARB) CL_OpenGL::get_proc_address("glVertexAttrib3d");
	functions->vertexAttrib4f = (CL_GLFunctions::ptr_glVertexAttrib4fARB) CL_OpenGL::get_proc_address("glVertexAttrib4f");
	functions->vertexAttrib4s = (CL_GLFunctions::ptr_glVertexAttrib4sARB) CL_OpenGL::get_proc_address("glVertexAttrib4s");
	functions->vertexAttrib4d = (CL_GLFunctions::ptr_glVertexAttrib4dARB) CL_OpenGL::get_proc_address("glVertexAttrib4d");
	functions->vertexAttrib4Nub = (CL_GLFunctions::ptr_glVertexAttrib4NubARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nub");
	functions->vertexAttrib1fv = (CL_GLFunctions::ptr_glVertexAttrib1fvARB) CL_OpenGL::get_proc_address("glVertexAttrib1fv");
	functions->vertexAttrib1sv = (CL_GLFunctions::ptr_glVertexAttrib1svARB) CL_OpenGL::get_proc_address("glVertexAttrib1sv");
	functions->vertexAttrib1dv = (CL_GLFunctions::ptr_glVertexAttrib1dvARB) CL_OpenGL::get_proc_address("glVertexAttrib1dv");
	functions->vertexAttrib2fv = (CL_GLFunctions::ptr_glVertexAttrib2fvARB) CL_OpenGL::get_proc_address("glVertexAttrib2fv");
	functions->vertexAttrib2sv = (CL_GLFunctions::ptr_glVertexAttrib2svARB) CL_OpenGL::get_proc_address("glVertexAttrib2sv");
	functions->vertexAttrib2dv = (CL_GLFunctions::ptr_glVertexAttrib2dvARB) CL_OpenGL::get_proc_address("glVertexAttrib2dv");
	functions->vertexAttrib3fv = (CL_GLFunctions::ptr_glVertexAttrib3fvARB) CL_OpenGL::get_proc_address("glVertexAttrib3fv");
	functions->vertexAttrib3sv = (CL_GLFunctions::ptr_glVertexAttrib3svARB) CL_OpenGL::get_proc_address("glVertexAttrib3sv");
	functions->vertexAttrib3dv = (CL_GLFunctions::ptr_glVertexAttrib3dvARB) CL_OpenGL::get_proc_address("glVertexAttrib3dv");
	functions->vertexAttrib4fv = (CL_GLFunctions::ptr_glVertexAttrib4fvARB) CL_OpenGL::get_proc_address("glVertexAttrib4fv");
	functions->vertexAttrib4sv = (CL_GLFunctions::ptr_glVertexAttrib4svARB) CL_OpenGL::get_proc_address("glVertexAttrib4sv");
	functions->vertexAttrib4dv = (CL_GLFunctions::ptr_glVertexAttrib4dvARB) CL_OpenGL::get_proc_address("glVertexAttrib4dv");
	functions->vertexAttrib4iv = (CL_GLFunctions::ptr_glVertexAttrib4ivARB) CL_OpenGL::get_proc_address("glVertexAttrib4iv");
	functions->vertexAttrib4bv = (CL_GLFunctions::ptr_glVertexAttrib4bvARB) CL_OpenGL::get_proc_address("glVertexAttrib4bv");
	functions->vertexAttrib4ubv = (CL_GLFunctions::ptr_glVertexAttrib4ubvARB) CL_OpenGL::get_proc_address("glVertexAttrib4ubv");
	functions->vertexAttrib4usv = (CL_GLFunctions::ptr_glVertexAttrib4usvARB) CL_OpenGL::get_proc_address("glVertexAttrib4usv");
	functions->vertexAttrib4uiv = (CL_GLFunctions::ptr_glVertexAttrib4uivARB) CL_OpenGL::get_proc_address("glVertexAttrib4uiv");
	functions->vertexAttrib4Nbv = (CL_GLFunctions::ptr_glVertexAttrib4NbvARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nbv");
	functions->vertexAttrib4Nsv = (CL_GLFunctions::ptr_glVertexAttrib4NsvARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nsv");
	functions->vertexAttrib4Niv = (CL_GLFunctions::ptr_glVertexAttrib4NivARB) CL_OpenGL::get_proc_address("glVertexAttrib4Niv");
	functions->vertexAttrib4Nubv = (CL_GLFunctions::ptr_glVertexAttrib4NubvARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nubv");
	functions->vertexAttrib4Nusv = (CL_GLFunctions::ptr_glVertexAttrib4NusvARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nusv");
	functions->vertexAttrib4Nuiv = (CL_GLFunctions::ptr_glVertexAttrib4NuivARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nuiv");
	functions->vertexAttribPointer = (CL_GLFunctions::ptr_glVertexAttribPointerARB) CL_OpenGL::get_proc_address("glVertexAttribPointer");
	functions->enableVertexAttribArray = (CL_GLFunctions::ptr_glEnableVertexAttribArrayARB) CL_OpenGL::get_proc_address("glEnableVertexAttribArray");
	functions->disableVertexAttribArray = (CL_GLFunctions::ptr_glDisableVertexAttribArrayARB) CL_OpenGL::get_proc_address("glDisableVertexAttribArray");
	functions->bindAttribLocation = (CL_GLFunctions::ptr_glBindAttribLocationARB) CL_OpenGL::get_proc_address("glBindAttribLocation");
	functions->getActiveAttrib = (CL_GLFunctions::ptr_glGetActiveAttribARB) CL_OpenGL::get_proc_address("glGetActiveAttrib");
	functions->getAttribLocation = (CL_GLFunctions::ptr_glGetAttribLocationARB) CL_OpenGL::get_proc_address("glGetAttribLocation");
	functions->getVertexAttribdv = (CL_GLFunctions::ptr_glGetVertexAttribdvARB) CL_OpenGL::get_proc_address("glGetVertexAttribdv");
	functions->getVertexAttribfv = (CL_GLFunctions::ptr_glGetVertexAttribfvARB) CL_OpenGL::get_proc_address("glGetVertexAttribfv");
	functions->getVertexAttribiv = (CL_GLFunctions::ptr_glGetVertexAttribivARB) CL_OpenGL::get_proc_address("glGetVertexAttribiv");
	functions->getVertexAttribPointerv = (CL_GLFunctions::ptr_glGetVertexAttribPointervARB) CL_OpenGL::get_proc_address("glGetVertexAttribPointerv");
	functions->drawBuffers = (CL_GLFunctions::ptr_glDrawBuffersARB) CL_OpenGL::get_proc_address("glDrawBuffers");
	functions->stencilFuncSeparate = (CL_GLFunctions::ptr_glStencilFuncSeparateATI) CL_OpenGL::get_proc_address("glStencilFuncSeparate");
	functions->stencilOpSeparate = (CL_GLFunctions::ptr_glStencilOpSeparateATI) CL_OpenGL::get_proc_address("glStencilOpSeparate");
#else
	functions->deleteShader = (CL_GLFunctions::ptr_glDeleteObjectARB) CL_OpenGL::get_proc_address("glDeleteObjectARB");
	functions->deleteProgram = functions->deleteShader;
	functions->getHandle = (CL_GLFunctions::ptr_glGetHandleARB) CL_OpenGL::get_proc_address("glGetHandleARB");
	functions->detachShader = (CL_GLFunctions::ptr_glDetachObjectARB) CL_OpenGL::get_proc_address("glDetachObjectARB");
	functions->createShader = (CL_GLFunctions::ptr_glCreateShaderObjectARB) CL_OpenGL::get_proc_address("glCreateShaderObjectARB");
	functions->shaderSource = (CL_GLFunctions::ptr_glShaderSourceARB) CL_OpenGL::get_proc_address("glShaderSourceARB");
	functions->compileShader = (CL_GLFunctions::ptr_glCompileShaderARB) CL_OpenGL::get_proc_address("glCompileShaderARB");
	functions->createProgram = (CL_GLFunctions::ptr_glCreateProgramObjectARB) CL_OpenGL::get_proc_address("glCreateProgramObjectARB");
	functions->attachShader = (CL_GLFunctions::ptr_glAttachObjectARB) CL_OpenGL::get_proc_address("glAttachObjectARB");
	functions->linkProgram = (CL_GLFunctions::ptr_glLinkProgramARB) CL_OpenGL::get_proc_address("glLinkProgramARB");
	functions->useProgram = (CL_GLFunctions::ptr_glUseProgramObjectARB) CL_OpenGL::get_proc_address("glUseProgramObjectARB");
	functions->validateProgram = (CL_GLFunctions::ptr_glValidateProgramARB) CL_OpenGL::get_proc_address("glValidateProgramARB");
	functions->uniform1f = (CL_GLFunctions::ptr_glUniform1fARB) CL_OpenGL::get_proc_address("glUniform1fARB");
	functions->uniform2f = (CL_GLFunctions::ptr_glUniform2fARB) CL_OpenGL::get_proc_address("glUniform2fARB");
	functions->uniform3f = (CL_GLFunctions::ptr_glUniform3fARB) CL_OpenGL::get_proc_address("glUniform3fARB");
	functions->uniform4f = (CL_GLFunctions::ptr_glUniform4fARB) CL_OpenGL::get_proc_address("glUniform4fARB");
	functions->uniform1i = (CL_GLFunctions::ptr_glUniform1iARB) CL_OpenGL::get_proc_address("glUniform1iARB");
	functions->uniform2i = (CL_GLFunctions::ptr_glUniform2iARB) CL_OpenGL::get_proc_address("glUniform2iARB");
	functions->uniform3i = (CL_GLFunctions::ptr_glUniform3iARB) CL_OpenGL::get_proc_address("glUniform3iARB");
	functions->uniform4i = (CL_GLFunctions::ptr_glUniform4iARB) CL_OpenGL::get_proc_address("glUniform4iARB");
	functions->uniform1fv = (CL_GLFunctions::ptr_glUniform1fvARB) CL_OpenGL::get_proc_address("glUniform1fvARB");
	functions->uniform2fv = (CL_GLFunctions::ptr_glUniform2fvARB) CL_OpenGL::get_proc_address("glUniform2fvARB");
	functions->uniform3fv = (CL_GLFunctions::ptr_glUniform3fvARB) CL_OpenGL::get_proc_address("glUniform3fvARB");
	functions->uniform4fv = (CL_GLFunctions::ptr_glUniform4fvARB) CL_OpenGL::get_proc_address("glUniform4fvARB");
	functions->uniform1iv = (CL_GLFunctions::ptr_glUniform1ivARB) CL_OpenGL::get_proc_address("glUniform1ivARB");
	functions->uniform2iv = (CL_GLFunctions::ptr_glUniform2ivARB) CL_OpenGL::get_proc_address("glUniform2ivARB");
	functions->uniform3iv = (CL_GLFunctions::ptr_glUniform3ivARB) CL_OpenGL::get_proc_address("glUniform3ivARB");
	functions->uniform4iv = (CL_GLFunctions::ptr_glUniform4ivARB) CL_OpenGL::get_proc_address("glUniform4ivARB");
	functions->uniformMatrix2fv = (CL_GLFunctions::ptr_glUniformMatrix2fvARB) CL_OpenGL::get_proc_address("glUniformMatrix2fvARB");
	functions->uniformMatrix3fv = (CL_GLFunctions::ptr_glUniformMatrix3fvARB) CL_OpenGL::get_proc_address("glUniformMatrix3fvARB");
	functions->uniformMatrix4fv = (CL_GLFunctions::ptr_glUniformMatrix4fvARB) CL_OpenGL::get_proc_address("glUniformMatrix4fvARB");
	functions->getProgramfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) CL_OpenGL::get_proc_address("glGetObjectParameterfvARB");
	functions->getProgramiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) CL_OpenGL::get_proc_address("glGetObjectParameterivARB");
	functions->getShaderfv = functions->getProgramfv;
	functions->getShaderiv = functions->getProgramiv;
	functions->getShaderInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) CL_OpenGL::get_proc_address("glGetInfoLogARB");
	functions->getProgramInfoLog = functions->getShaderInfoLog;
	functions->getAttachedShaders = (CL_GLFunctions::ptr_glGetAttachedObjectsARB) CL_OpenGL::get_proc_address("glGetAttachedObjectsARB");
	functions->getUniformLocation = (CL_GLFunctions::ptr_glGetUniformLocationARB) CL_OpenGL::get_proc_address("glGetUniformLocationARB");
	functions->getActiveUniform = (CL_GLFunctions::ptr_glGetActiveUniformARB) CL_OpenGL::get_proc_address("glGetActiveUniformARB");
	functions->getUniformfv = (CL_GLFunctions::ptr_glGetUniformfvARB) CL_OpenGL::get_proc_address("glGetUniformfvARB");
	functions->getUniformiv = (CL_GLFunctions::ptr_glGetUniformivARB) CL_OpenGL::get_proc_address("glGetUniformivARB");
	functions->getShaderSource = (CL_GLFunctions::ptr_glGetShaderSourceARB) CL_OpenGL::get_proc_address("glGetShaderSourceARB");
	functions->vertexAttrib1f = (CL_GLFunctions::ptr_glVertexAttrib1fARB) CL_OpenGL::get_proc_address("glVertexAttrib1fARB");
	functions->vertexAttrib1s = (CL_GLFunctions::ptr_glVertexAttrib1sARB) CL_OpenGL::get_proc_address("glVertexAttrib1sARB");
	functions->vertexAttrib1d = (CL_GLFunctions::ptr_glVertexAttrib1dARB) CL_OpenGL::get_proc_address("glVertexAttrib1dARB");
	functions->vertexAttrib2f = (CL_GLFunctions::ptr_glVertexAttrib2fARB) CL_OpenGL::get_proc_address("glVertexAttrib2fARB");
	functions->vertexAttrib2s = (CL_GLFunctions::ptr_glVertexAttrib2sARB) CL_OpenGL::get_proc_address("glVertexAttrib2sARB");
	functions->vertexAttrib2d = (CL_GLFunctions::ptr_glVertexAttrib2dARB) CL_OpenGL::get_proc_address("glVertexAttrib2dARB");
	functions->vertexAttrib3f = (CL_GLFunctions::ptr_glVertexAttrib3fARB) CL_OpenGL::get_proc_address("glVertexAttrib3fARB");
	functions->vertexAttrib3s = (CL_GLFunctions::ptr_glVertexAttrib3sARB) CL_OpenGL::get_proc_address("glVertexAttrib3sARB");
	functions->vertexAttrib3d = (CL_GLFunctions::ptr_glVertexAttrib3dARB) CL_OpenGL::get_proc_address("glVertexAttrib3dARB");
	functions->vertexAttrib4f = (CL_GLFunctions::ptr_glVertexAttrib4fARB) CL_OpenGL::get_proc_address("glVertexAttrib4fARB");
	functions->vertexAttrib4s = (CL_GLFunctions::ptr_glVertexAttrib4sARB) CL_OpenGL::get_proc_address("glVertexAttrib4sARB");
	functions->vertexAttrib4d = (CL_GLFunctions::ptr_glVertexAttrib4dARB) CL_OpenGL::get_proc_address("glVertexAttrib4dARB");
	functions->vertexAttrib4Nub = (CL_GLFunctions::ptr_glVertexAttrib4NubARB) CL_OpenGL::get_proc_address("glVertexAttrib4NubARB");
	functions->vertexAttrib1fv = (CL_GLFunctions::ptr_glVertexAttrib1fvARB) CL_OpenGL::get_proc_address("glVertexAttrib1fvARB");
	functions->vertexAttrib1sv = (CL_GLFunctions::ptr_glVertexAttrib1svARB) CL_OpenGL::get_proc_address("glVertexAttrib1svARB");
	functions->vertexAttrib1dv = (CL_GLFunctions::ptr_glVertexAttrib1dvARB) CL_OpenGL::get_proc_address("glVertexAttrib1dvARB");
	functions->vertexAttrib2fv = (CL_GLFunctions::ptr_glVertexAttrib2fvARB) CL_OpenGL::get_proc_address("glVertexAttrib2fvARB");
	functions->vertexAttrib2sv = (CL_GLFunctions::ptr_glVertexAttrib2svARB) CL_OpenGL::get_proc_address("glVertexAttrib2svARB");
	functions->vertexAttrib2dv = (CL_GLFunctions::ptr_glVertexAttrib2dvARB) CL_OpenGL::get_proc_address("glVertexAttrib2dvARB");
	functions->vertexAttrib3fv = (CL_GLFunctions::ptr_glVertexAttrib3fvARB) CL_OpenGL::get_proc_address("glVertexAttrib3fvARB");
	functions->vertexAttrib3sv = (CL_GLFunctions::ptr_glVertexAttrib3svARB) CL_OpenGL::get_proc_address("glVertexAttrib3svARB");
	functions->vertexAttrib3dv = (CL_GLFunctions::ptr_glVertexAttrib3dvARB) CL_OpenGL::get_proc_address("glVertexAttrib3dvARB");
	functions->vertexAttrib4fv = (CL_GLFunctions::ptr_glVertexAttrib4fvARB) CL_OpenGL::get_proc_address("glVertexAttrib4fvARB");
	functions->vertexAttrib4sv = (CL_GLFunctions::ptr_glVertexAttrib4svARB) CL_OpenGL::get_proc_address("glVertexAttrib4svARB");
	functions->vertexAttrib4dv = (CL_GLFunctions::ptr_glVertexAttrib4dvARB) CL_OpenGL::get_proc_address("glVertexAttrib4dvARB");
	functions->vertexAttrib4iv = (CL_GLFunctions::ptr_glVertexAttrib4ivARB) CL_OpenGL::get_proc_address("glVertexAttrib4ivARB");
	functions->vertexAttrib4bv = (CL_GLFunctions::ptr_glVertexAttrib4bvARB) CL_OpenGL::get_proc_address("glVertexAttrib4bvARB");
	functions->vertexAttrib4ubv = (CL_GLFunctions::ptr_glVertexAttrib4ubvARB) CL_OpenGL::get_proc_address("glVertexAttrib4ubvARB");
	functions->vertexAttrib4usv = (CL_GLFunctions::ptr_glVertexAttrib4usvARB) CL_OpenGL::get_proc_address("glVertexAttrib4usvARB");
	functions->vertexAttrib4uiv = (CL_GLFunctions::ptr_glVertexAttrib4uivARB) CL_OpenGL::get_proc_address("glVertexAttrib4uivARB");
	functions->vertexAttrib4Nbv = (CL_GLFunctions::ptr_glVertexAttrib4NbvARB) CL_OpenGL::get_proc_address("glVertexAttrib4NbvARB");
	functions->vertexAttrib4Nsv = (CL_GLFunctions::ptr_glVertexAttrib4NsvARB) CL_OpenGL::get_proc_address("glVertexAttrib4NsvARB");
	functions->vertexAttrib4Niv = (CL_GLFunctions::ptr_glVertexAttrib4NivARB) CL_OpenGL::get_proc_address("glVertexAttrib4NivARB");
	functions->vertexAttrib4Nubv = (CL_GLFunctions::ptr_glVertexAttrib4NubvARB) CL_OpenGL::get_proc_address("glVertexAttrib4NubvARB");
	functions->vertexAttrib4Nusv = (CL_GLFunctions::ptr_glVertexAttrib4NusvARB) CL_OpenGL::get_proc_address("glVertexAttrib4NusvARB");
	functions->vertexAttrib4Nuiv = (CL_GLFunctions::ptr_glVertexAttrib4NuivARB) CL_OpenGL::get_proc_address("glVertexAttrib4NuivARB");
	functions->vertexAttribPointer = (CL_GLFunctions::ptr_glVertexAttribPointerARB) CL_OpenGL::get_proc_address("glVertexAttribPointerARB");
	functions->enableVertexAttribArray = (CL_GLFunctions::ptr_glEnableVertexAttribArrayARB) CL_OpenGL::get_proc_address("glEnableVertexAttribArrayARB");
	functions->disableVertexAttribArray = (CL_GLFunctions::ptr_glDisableVertexAttribArrayARB) CL_OpenGL::get_proc_address("glDisableVertexAttribArrayARB");
	functions->bindAttribLocation = (CL_GLFunctions::ptr_glBindAttribLocationARB) CL_OpenGL::get_proc_address("glBindAttribLocationARB");
	functions->getActiveAttrib = (CL_GLFunctions::ptr_glGetActiveAttribARB) CL_OpenGL::get_proc_address("glGetActiveAttribARB");
	functions->getAttribLocation = (CL_GLFunctions::ptr_glGetAttribLocationARB) CL_OpenGL::get_proc_address("glGetAttribLocationARB");
	functions->getVertexAttribdv = (CL_GLFunctions::ptr_glGetVertexAttribdvARB) CL_OpenGL::get_proc_address("glGetVertexAttribdvARB");
	functions->getVertexAttribfv = (CL_GLFunctions::ptr_glGetVertexAttribfvARB) CL_OpenGL::get_proc_address("glGetVertexAttribfvARB");
	functions->getVertexAttribiv = (CL_GLFunctions::ptr_glGetVertexAttribivARB) CL_OpenGL::get_proc_address("glGetVertexAttribivARB");
	functions->getVertexAttribPointerv = (CL_GLFunctions::ptr_glGetVertexAttribPointervARB) CL_OpenGL::get_proc_address("glGetVertexAttribPointervARB");
	functions->drawBuffers = (CL_GLFunctions::ptr_glDrawBuffersARB) CL_OpenGL::get_proc_address("glDrawBuffersARB");

	functions->stencilFuncSeparate = (CL_GLFunctions::ptr_glStencilFuncSeparateATI) CL_OpenGL::get_proc_address("glStencilFuncSeparate");
	functions->stencilOpSeparate = (CL_GLFunctions::ptr_glStencilOpSeparateATI) CL_OpenGL::get_proc_address("glStencilOpSeparate");

	if (functions->stencilFuncSeparate == NULL)
	{
		functions->stencilFuncSeparate = (CL_GLFunctions::ptr_glStencilFuncSeparateATI) CL_OpenGL::get_proc_address("glStencilFuncSeparateATI");
	}

	if (functions->stencilOpSeparate == NULL)
	{
		functions->stencilOpSeparate = (CL_GLFunctions::ptr_glStencilOpSeparateATI) CL_OpenGL::get_proc_address("glStencilOpSeparateATI");
	}

	functions->stencilMaskSeparate = (CL_GLFunctions::ptr_glStencilMaskSeparate) CL_OpenGL::get_proc_address("glStencilMaskSeparate");
#endif

	// Binds for EXT_framebuffer_object:

	functions->isRenderbuffer = (CL_GLFunctions::ptr_glIsRenderbufferEXT) CL_OpenGL::get_proc_address("glIsRenderbufferEXT");
	functions->bindRenderbuffer = (CL_GLFunctions::ptr_glBindRenderbufferEXT) CL_OpenGL::get_proc_address("glBindRenderbufferEXT");
	functions->deleteRenderbuffers = (CL_GLFunctions::ptr_glDeleteRenderbuffersEXT) CL_OpenGL::get_proc_address("glDeleteRenderbuffersEXT");
	functions->genRenderbuffers = (CL_GLFunctions::ptr_glGenRenderbuffersEXT) CL_OpenGL::get_proc_address("glGenRenderbuffersEXT");
	functions->renderbufferStorage = (CL_GLFunctions::ptr_glRenderbufferStorageEXT) CL_OpenGL::get_proc_address("glRenderbufferStorageEXT");
	functions->getRenderbufferParameteriv = (CL_GLFunctions::ptr_glGetRenderbufferParameterivEXT) CL_OpenGL::get_proc_address("glGetRenderbufferParameterivEXT");
	functions->isFramebuffer = (CL_GLFunctions::ptr_glIsFramebufferEXT) CL_OpenGL::get_proc_address("glIsFramebufferEXT");
	functions->bindFramebuffer = (CL_GLFunctions::ptr_glBindFramebufferEXT) CL_OpenGL::get_proc_address("glBindFramebufferEXT");
	functions->deleteFramebuffers = (CL_GLFunctions::ptr_glDeleteFramebuffersEXT) CL_OpenGL::get_proc_address("glDeleteFramebuffersEXT");
	functions->genFramebuffers = (CL_GLFunctions::ptr_glGenFramebuffersEXT) CL_OpenGL::get_proc_address("glGenFramebuffersEXT");
	functions->checkFramebufferStatus = (CL_GLFunctions::ptr_glCheckFramebufferStatusEXT) CL_OpenGL::get_proc_address("glCheckFramebufferStatusEXT");
	functions->framebufferTexture1D = (CL_GLFunctions::ptr_glFramebufferTexture1DEXT) CL_OpenGL::get_proc_address("glFramebufferTexture1DEXT");
	functions->framebufferTexture2D = (CL_GLFunctions::ptr_glFramebufferTexture2DEXT) CL_OpenGL::get_proc_address("glFramebufferTexture2DEXT");
	functions->framebufferTexture3D = (CL_GLFunctions::ptr_glFramebufferTexture3DEXT) CL_OpenGL::get_proc_address("glFramebufferTexture3DEXT");
	functions->framebufferRenderbuffer = (CL_GLFunctions::ptr_glFramebufferRenderbufferEXT) CL_OpenGL::get_proc_address("glFramebufferRenderbufferEXT");
	functions->getFramebufferAttachmentParameteriv = (CL_GLFunctions::ptr_glGetFramebufferAttachmentParameterivEXT) CL_OpenGL::get_proc_address("glGetFramebufferAttachmentParameterivEXT");
	functions->generateMipmap = (CL_GLFunctions::ptr_glGenerateMipmapEXT) CL_OpenGL::get_proc_address("glGenerateMipmapEXT");

	// Binds for EXT_framebuffer_blit:

	functions->blitFramebuffer = (CL_GLFunctions::ptr_glBlitFramebufferEXT) CL_OpenGL::get_proc_address("glBlitFramebufferEXT");

	return functions;
}

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
**    Kevin J Bluck
**    Mark Page
*/

#include "GL/precomp.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/mutex.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/Text/string_format.h"
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

void CL_OpenGL::check_error()
{
	CLenum last_error = clGetError();
	if (last_error != CL_NO_ERROR)
	{
		switch (last_error)
		{
			case CL_INVALID_ENUM:
				throw CL_Exception("CL_INVALID_ENUM - An unacceptable value is specified for an enumerated argument.");
			case CL_INVALID_VALUE:
				throw CL_Exception("CL_INVALID_VALUE - A numeric argument is out of range.");
			case CL_INVALID_OPERATION:
				throw CL_Exception("CL_INVALID_OPERATION - The specified operation is not allowed in the current state");
			case CL_OUT_OF_MEMORY:
				throw CL_Exception("CL_OUT_OF_MEMORY - There is not enough memory left to execute the command");
			default:
				throw CL_Exception(cl_format("Unknown OpenGL Error - %1", last_error));
		}
	}

}

int CL_OpenGL::get_textureformat_bits(CL_TextureFormat format)
{
	int count;
	switch (format)
	{
		case cl_r8: count = 8;	break; //RED 8
		case cl_r8_snorm: count = 8; break; //RED, s8
		case cl_r16: count = 16; break; //RED, 16
		case cl_r16_snorm: count = 16; break; //RED, s16
		case cl_rg8: count = 8 + 8; break; //RG, 8, 8
		case cl_rg8_snorm: count = 8 + 8; break; //RG, s8, s8
		case cl_rg16: count = 16 + 16; break; //RG, 16, 16
		case cl_rg16_snorm: count = 16 + 16; break; //RG, s16, s16
		case cl_r3_g3_b2: count = 3 + 3 + 2; break; //RGB, 3, 3, 2
		case cl_rgb4: count = 4 + 4 + 4; break; //RGB, 4, 4, 4
		case cl_rgb5: count = 5 + 5 + 5; break; //RGB, 5, 5, 5
		case cl_rgb8: count = 8 + 8 + 8; break; //RGB, 8, 8, 8
		case cl_rgb8_snorm: count = 8 + 8 + 8; break; //RGB, s8, s8, s8
		case cl_rgb10: count = 10 + 10 + 10; break; //RGB, 10, 10, 10
		case cl_rgb12: count = 12 + 12 + 12; break; //RGB, 12, 12, 12
		case cl_rgb16: count = 16 + 16 + 16; break; //RGB, 16, 16, 16
		case cl_rgb16_snorm: count = 16 + 16 + 16; break; //RGB, s16, s16, s16
		case cl_rgba2: count = 2 + 2 + 2 + 2; break; //RGBA, 2, 2, 2, 2
		case cl_rgba4: count = 4 + 4 + 4 + 4; break; //RGBA, 4, 4, 4, 4
		case cl_rgb5_a1: count = 5 + 5 + 5 + 1; break; //RGBA, 5, 5, 5, 1
		case cl_rgba8: count = 8 + 8 + 8 + 8; break; //RGBA, 8, 8, 8, 8
		case cl_rgba8_snorm: count = 8 + 8 + 8 + 8; break; //RGBA, s8, s8, s8, s8
		case cl_rgb10_a2: count = 10 + 10 + 10 + 2; break; //RGBA, 10, 10, 10, 2
		case cl_rgba12: count = 12 + 12 + 12 + 12; break; //RGBA, 12, 12, 12, 12
		case cl_rgba16: count = 16 + 16 + 16 + 16; break; //RGBA, 16, 16, 16, 16
		case cl_rgba16_snorm: count = 16 + 16 + 16 + 16; break; //RGBA, s16, s16, s16, s16
		case cl_srgb8: count = 8 + 8 + 8; break; //RGB, 8, 8, 8
		case cl_srgb8_alpha8: count = 8 + 8 + 8 + 8; break; //RGBA, 8, 8, 8, 8
		case cl_r16f: count = 16; break; //RED, f16
		case cl_rg16f: count = 16 + 16; break; //RG, f16, f16
		case cl_rgb16f: count = 16 + 16 + 16; break; //RGB, f16, f16, f16
		case cl_rgba16f: count = 16 + 16 + 16 + 16; break; //RGBA, f16, f16, f16, f16
		case cl_r32f: count = 32; break; //RED, f32
		case cl_rg32f: count = 32 + 32; break; //RG, f32, f32
		case cl_rgb32f: count = 32 + 32 + 32; break; //RGB, f32, f32, f32
		case cl_rgba32f: count = 32 + 32 + 32 + 32; break; //RGBA, f32, f32, f32, f32
		case cl_r11f_g11f_b10f: count = 11 + 11 + 10; break; //RGB, f11, f11, f10
		case cl_rgb9_e5: count = 9 + 9 + 9 + 5; break; //RGB, 9, 9, 9, 5
		case cl_r8i: count = 8; break; //RED, i8
		case cl_r8ui: count = 8; break; //RED, ui8
		case cl_r16i: count = 16; break; //RED, i16
		case cl_r16ui: count = 16; break; //RED, ui16
		case cl_r32i: count = 32; break; //RED, i32
		case cl_r32ui: count = 32; break; //RED, ui32
		case cl_rg8i: count = 8 + 8; break; //RG, i8, i8
		case cl_rg8ui: count = 8 + 8; break; //RG, ui8, ui8
		case cl_rg16i: count = 16 + 16; break; //RG, i16, i16
		case cl_rg16ui: count = 16 + 16; break; //RG, ui16, ui16
		case cl_rg32i: count = 32 + 32; break; //RG, i32, i32
		case cl_rg32ui: count = 32 + 32; break; //RG, ui32, ui32
		case cl_rgb8i: count = 8 + 8 + 8; break; //RGB, i8, i8, i8
		case cl_rgb8ui: count = 8 + 8 + 8; break; //RGB, ui8, ui8, ui8
		case cl_rgb16i: count = 16 + 16 + 16; break; //RGB, i16, i16, i16
		case cl_rgb16ui: count = 16 + 16 + 16; break; //RGB, ui16, ui16, ui16
		case cl_rgb32i: count = 32 + 32 + 32; break; //RGB, i32, i32, i32
		case cl_rgb32ui: count = 32 + 32 + 32; break; //RGB, ui32, ui32, ui32
		case cl_rgba8i: count = 8 + 8 + 8 + 8; break; //RGBA, i8, i8, i8, i8
		case cl_rgba8ui: count = 8 + 8 + 8 + 8; break; //RGBA, ui8, ui8, ui8, ui8
		case cl_rgba16i: count = 16 + 16 + 16 + 16; break; //RGBA, i16, i16, i16, i16
		case cl_rgba16ui: count = 16 + 16 + 16 + 16; break; //RGBA, ui16, ui16, ui16, ui16
		case cl_rgba32i: count = 32 + 32 + 32 + 32; break; //RGBA, i32, i32, i32, i32
		case cl_rgba32ui: count = 32 + 32 + 32 + 32; break; //RGBA, ui32, ui32, ui32, ui32
		case cl_depth_component16: count = 16; break; //DEPTH_COMPONENT, 16
		case cl_depth_component24: count = 24; break; //DEPTH_COMPONENT, 24
		case cl_depth_component32: count = 32; break; //DEPTH_COMPONENT, 32
		case cl_depth_component32f: count = 32; break; //DEPTH_COMPONENT, f32
		case cl_depth24_stencil8: count = 24 + 8; break; //DEPTH_STENCIL, 24, 8
		case cl_depth32f_stencil8: count = 32 + 8; break; //DEPTH_STENCIL, f32, 8
		default:
			throw CL_Exception("cannot obtain pixel count for this CL_TextureFormat");
	}
	return count;
}


void CL_OpenGL::to_opengl_textureformat(CL_TextureFormat format, CLint &gl_internal_format, CLenum &gl_pixel_format)
{
	switch (format)
	{
	// base internal format
		case cl_depth_component: gl_internal_format = CL_DEPTH_COMPONENT; gl_pixel_format = CL_DEPTH_COMPONENT; break;
		case cl_depth_stencil: gl_internal_format = CL_DEPTH_STENCIL; gl_pixel_format = CL_DEPTH_STENCIL; break;
		case cl_red: gl_internal_format = CL_RED; gl_pixel_format = CL_RED; break;
		case cl_green: gl_internal_format = CL_GREEN; gl_pixel_format = CL_GREEN; break;
		case cl_blue: gl_internal_format = CL_BLUE; gl_pixel_format = CL_BLUE; break;
		case cl_rg: gl_internal_format = CL_RG; gl_pixel_format = CL_RG; break;
		case cl_rgb: gl_internal_format = CL_RGB; gl_pixel_format = CL_RGB; break;
		case cl_rgba: gl_internal_format = CL_RGBA; gl_pixel_format = CL_RGBA; break;
		case cl_bgr: gl_internal_format = CL_BGR; gl_pixel_format = CL_BGR; break;
		case cl_bgra: gl_internal_format = CL_BGRA; gl_pixel_format = CL_BGRA; break;
		case cl_red_integer: gl_internal_format = CL_RED_INTEGER; gl_pixel_format = CL_RED_INTEGER; break;
		case cl_green_integer: gl_internal_format = CL_GREEN_INTEGER; gl_pixel_format = CL_GREEN_INTEGER; break;
		case cl_blue_integer: gl_internal_format = CL_BLUE_INTEGER; gl_pixel_format = CL_BLUE_INTEGER; break;
		case cl_rg_integer: gl_internal_format = CL_RG_INTEGER; gl_pixel_format = CL_RG_INTEGER; break;
		case cl_rgb_integer: gl_internal_format = CL_RGB_INTEGER; gl_pixel_format = CL_RGB_INTEGER; break;
		case cl_rgba_integer: gl_internal_format = CL_RGBA_INTEGER; gl_pixel_format = CL_RGBA_INTEGER; break;
		case cl_bgr_integer: gl_internal_format = CL_BGR_INTEGER; gl_pixel_format = CL_BGR_INTEGER; break;
		case cl_bgra_integer: gl_internal_format = CL_BGRA_INTEGER; gl_pixel_format = CL_BGRA_INTEGER; break;

		// TODO: Should this really be here?
		case cl_stencil_index: gl_internal_format = CL_STENCIL_INDEX; gl_pixel_format = CL_STENCIL_INDEX; break;

	// sized internal format

		// TODO: Should this really be here?
		case cl_stencil_index1: gl_internal_format = CL_STENCIL_INDEX1; gl_pixel_format = CL_STENCIL_INDEX; break;
		case cl_stencil_index4: gl_internal_format = CL_STENCIL_INDEX4; gl_pixel_format = CL_STENCIL_INDEX; break;
		case cl_stencil_index8: gl_internal_format = CL_STENCIL_INDEX8; gl_pixel_format = CL_STENCIL_INDEX; break;
		case cl_stencil_index16: gl_internal_format = CL_STENCIL_INDEX16; gl_pixel_format = CL_STENCIL_INDEX; break;

		case cl_r8: gl_internal_format = CL_R8; gl_pixel_format = CL_RED; break;
		case cl_r8_snorm: gl_internal_format = CL_R8_SNORM; gl_pixel_format = CL_RED; break;
		case cl_r16: gl_internal_format = CL_R16; gl_pixel_format = CL_RED; break;
		case cl_r16_snorm: gl_internal_format = CL_R16_SNORM; gl_pixel_format = CL_RED; break;
		case cl_rg8: gl_internal_format = CL_RG8; gl_pixel_format = CL_RG; break;
		case cl_rg8_snorm: gl_internal_format = CL_RG8_SNORM; gl_pixel_format = CL_RG; break;
		case cl_rg16: gl_internal_format = CL_RG16; gl_pixel_format = CL_RG; break;
		case cl_rg16_snorm: gl_internal_format = CL_RG16_SNORM; gl_pixel_format = CL_RG; break;
		case cl_r3_g3_b2: gl_internal_format = CL_R3_G3_B2; gl_pixel_format = CL_RGB; break;
		case cl_rgb4: gl_internal_format = CL_RGB4; gl_pixel_format = CL_RGB; break;
		case cl_rgb5: gl_internal_format = CL_RGB5; gl_pixel_format = CL_RGB; break;
		case cl_rgb8: gl_internal_format = CL_RGB8; gl_pixel_format = CL_RGB; break;
		case cl_rgb10: gl_internal_format = CL_RGB10; gl_pixel_format = CL_RGB; break;
		case cl_rgb12: gl_internal_format = CL_RGB12; gl_pixel_format = CL_RGB; break;
		case cl_rgb16: gl_internal_format = CL_RGB16; gl_pixel_format = CL_RGB; break;
		case cl_rgb16_snorm: gl_internal_format = CL_RGB16_SNORM; gl_pixel_format = CL_RGB; break;
		case cl_rgba2: gl_internal_format = CL_RGBA2; gl_pixel_format = CL_RGBA; break;
		case cl_rgba4: gl_internal_format = CL_RGBA4; gl_pixel_format = CL_RGBA; break;
		case cl_rgb5_a1: gl_internal_format = CL_RGB5_A1; gl_pixel_format = CL_RGBA; break;
		case cl_rgba8: gl_internal_format = CL_RGBA8; gl_pixel_format = CL_RGBA; break;
		case cl_rgba8_snorm: gl_internal_format = CL_RGBA8_SNORM; gl_pixel_format = CL_RGBA; break;
		case cl_rgb10_a2: gl_internal_format = CL_RGB10_A2; gl_pixel_format = CL_RGBA; break;
		case cl_rgba12: gl_internal_format = CL_RGBA12; gl_pixel_format = CL_RGBA; break;
		case cl_rgba16: gl_internal_format = CL_RGBA16; gl_pixel_format = CL_RGBA; break;
		case cl_rgba16_snorm: gl_internal_format = CL_RGBA16_SNORM; gl_pixel_format = CL_RGBA; break;
		case cl_srgb8: gl_internal_format = CL_SRGB8; gl_pixel_format = CL_RGB; break;
		case cl_srgb8_alpha8: gl_internal_format = CL_SRGB8_ALPHA8; gl_pixel_format = CL_RGBA; break;
		case cl_r16f: gl_internal_format = CL_R16F; gl_pixel_format = CL_RED; break;
		case cl_rg16f: gl_internal_format = CL_RG16F; gl_pixel_format = CL_RG; break;
		case cl_rgb16f: gl_internal_format = CL_RGB16F; gl_pixel_format = CL_RGB; break;
		case cl_rgba16f: gl_internal_format = CL_RGBA16F; gl_pixel_format = CL_RGBA; break;
		case cl_r32f: gl_internal_format = CL_R32F; gl_pixel_format = CL_RED; break;
		case cl_rg32f: gl_internal_format = CL_RG32F; gl_pixel_format = CL_RG; break;
		case cl_rgb32f: gl_internal_format = CL_RGB32F; gl_pixel_format = CL_RGB; break;
		case cl_rgba32f: gl_internal_format = CL_RGBA32F; gl_pixel_format = CL_RGBA; break;
		case cl_r11f_g11f_b10f: gl_internal_format = CL_R11F_G11F_B10F; gl_pixel_format = CL_RGB; break;
		case cl_rgb9_e5: gl_internal_format = CL_RGB9_E5; gl_pixel_format = CL_RGB; break;
		case cl_r8i: gl_internal_format = CL_R8I; gl_pixel_format = CL_RED; break;
		case cl_r8ui: gl_internal_format = CL_R8UI; gl_pixel_format = CL_RED; break;
		case cl_r16i: gl_internal_format = CL_R16I; gl_pixel_format = CL_RED; break;
		case cl_r16ui: gl_internal_format = CL_R16UI; gl_pixel_format = CL_RED; break;
		case cl_r32i: gl_internal_format = CL_R32I; gl_pixel_format = CL_RED; break;
		case cl_r32ui: gl_internal_format = CL_R32UI; gl_pixel_format = CL_RED; break;
		case cl_rg8i: gl_internal_format = CL_RG8I; gl_pixel_format = CL_RG; break;
		case cl_rg8ui: gl_internal_format = CL_RG8UI; gl_pixel_format = CL_RG; break;
		case cl_rg16i: gl_internal_format = CL_RG16I; gl_pixel_format = CL_RG; break;
		case cl_rg16ui: gl_internal_format = CL_RG16UI; gl_pixel_format = CL_RG; break;
		case cl_rg32i: gl_internal_format = CL_RG32I; gl_pixel_format = CL_RG; break;
		case cl_rg32ui: gl_internal_format = CL_RG32UI; gl_pixel_format = CL_RG; break;
		case cl_rgb8i: gl_internal_format = CL_RGB8I; gl_pixel_format = CL_RGB; break;
		case cl_rgb8ui: gl_internal_format = CL_RGB8UI; gl_pixel_format = CL_RGB; break;
		case cl_rgb16i: gl_internal_format = CL_RGB16I; gl_pixel_format = CL_RGB; break;
		case cl_rgb16ui: gl_internal_format = CL_RGB16UI; gl_pixel_format = CL_RGB; break;
		case cl_rgb32i: gl_internal_format = CL_RGB32I; gl_pixel_format = CL_RGB; break;
		case cl_rgb32ui: gl_internal_format = CL_RGB32UI; gl_pixel_format = CL_RGB; break;
		case cl_rgba8i: gl_internal_format = CL_RGBA8I; gl_pixel_format = CL_RGBA; break;
		case cl_rgba8ui: gl_internal_format = CL_RGBA8UI; gl_pixel_format = CL_RGBA; break;
		case cl_rgba16i: gl_internal_format = CL_RGBA16I; gl_pixel_format = CL_RGBA; break;
		case cl_rgba16ui: gl_internal_format = CL_RGBA16UI; gl_pixel_format = CL_RGBA; break;
		case cl_rgba32i: gl_internal_format = CL_RGBA32I; gl_pixel_format = CL_RGBA; break;
		case cl_rgba32ui: gl_internal_format = CL_RGBA32UI; gl_pixel_format = CL_RGBA; break;
		case cl_depth_component16: gl_internal_format = CL_DEPTH_COMPONENT16; gl_pixel_format = CL_DEPTH_COMPONENT; break;
		case cl_depth_component24: gl_internal_format = CL_DEPTH_COMPONENT24; gl_pixel_format = CL_DEPTH_COMPONENT; break;
		case cl_depth_component32: gl_internal_format = CL_DEPTH_COMPONENT32; gl_pixel_format = CL_DEPTH_COMPONENT; break;
		case cl_depth_component32f: gl_internal_format = CL_DEPTH_COMPONENT32F; gl_pixel_format = CL_DEPTH_COMPONENT; break;
		case cl_depth24_stencil8: gl_internal_format = CL_DEPTH24_STENCIL8; gl_pixel_format = CL_DEPTH_STENCIL; break;
		case cl_depth32f_stencil8: gl_internal_format = CL_DEPTH32F_STENCIL8; gl_pixel_format = CL_DEPTH_STENCIL; break;

		case cl_compressed_red: gl_internal_format = CL_COMPRESSED_RED; gl_pixel_format = CL_RED; break;
		case cl_compressed_rg: gl_internal_format = CL_COMPRESSED_RG; gl_pixel_format = CL_RG; break;
		case cl_compressed_rgb: gl_internal_format = CL_COMPRESSED_RGB; gl_pixel_format = CL_RGB; break;
		case cl_compressed_rgba: gl_internal_format = CL_COMPRESSED_RGBA; gl_pixel_format = CL_RGBA; break;
		case cl_compressed_srgb: gl_internal_format = CL_COMPRESSED_SRGB; gl_pixel_format = CL_RGB; break;
		case cl_compressed_srgb_alpha: gl_internal_format = CL_COMPRESSED_SRGB_ALPHA; gl_pixel_format = CL_RGBA; break;
		case cl_compressed_red_rgtc1: gl_internal_format = CL_COMPRESSED_RED_RGTC1; gl_pixel_format = CL_RED; break;
		case cl_compressed_signed_red_rgtc1: gl_internal_format = CL_COMPRESSED_SIGNED_RED_RGTC1; gl_pixel_format = CL_RED; break;
		case cl_compressed_rg_rgtc2: gl_internal_format = CL_COMPRESSED_RG_RGTC2; gl_pixel_format = CL_RG; break;
		case cl_compressed_signed_rg_rgtc2: gl_internal_format = CL_COMPRESSED_SIGNED_RG_RGTC2; gl_pixel_format = CL_RG; break;
		default:
			throw CL_Exception("Unknown CL_TextureFormat");
	}

}

bool CL_OpenGL::to_opengl_pixelformat(CL_TextureFormat texture_format, CLenum &format, CLenum &type)
{
	bool valid = false;

	//TODO: We should really use CL_Endian::is_system_big()

	//TODO: All the formats in this switch are not supported - Maybe they can be

	switch (texture_format)
	{
		case cl_rgba8:
		{
			valid = true;
			type = CL_UNSIGNED_INT_8_8_8_8;
			format = CL_RGBA;
			break;
		}
		case cl_rgb8:
		{
			valid = true;
			type = CL_UNSIGNED_BYTE;
			format = CL_BGR;
			break;
		}
		case cl_bgr8:
		{
			valid = true;
			type = CL_UNSIGNED_BYTE;
			format = CL_RGB;
			break;
		}
		case cl_a1_rgb5: break;
		case cl_argb8: break;
		case cl_abgr8:
		{
			valid = true;
			type = CL_UNSIGNED_INT_8_8_8_8_REV;
			format = CL_RGBA;
			break;
		}
		case cl_color_index: break;
		case cl_stencil_index: break;
		case cl_depth_component: break;
		case cl_depth_stencil: break;
		case cl_red: break;
		case cl_green: break;
		case cl_blue: break;
		case cl_rg: break;
		case cl_rgb: break;
		case cl_rgba: break;
		case cl_bgr: break;
		case cl_bgra: break;
		case cl_red_integer: break;
		case cl_green_integer: break;
		case cl_blue_integer: break;
		case cl_rg_integer: break;
		case cl_rgb_integer: break;
		case cl_rgba_integer: break;
		case cl_bgr_integer: break;
		case cl_bgra_integer: break;
		case cl_stencil_index1: break;
		case cl_stencil_index4: break;
		case cl_stencil_index8: break;
		case cl_stencil_index16: break;
		case cl_r8:
		{
			valid = true;
			type = CL_UNSIGNED_BYTE;
			format = CL_RED;
			break;
		}
		case cl_r8_snorm:
		{
			valid = true;
			type = CL_BYTE;
			format = CL_RED;
			break;
		}
		case cl_r16:
		{
			valid = true;
			type = CL_UNSIGNED_SHORT;
			format = CL_RED;
			break;
		}
		case cl_r16_snorm:
		{
			valid = true;
			type = CL_SHORT;
			format = CL_RED;
			break;
		}
		case cl_rg8: break;
		case cl_rg8_snorm: break;
		case cl_rg16: break;
		case cl_rg16_snorm: break;
		case cl_r3_g3_b2:
		{
			valid = true;
			type = CL_UNSIGNED_BYTE_3_3_2;
			format = CL_RGB;
			break;
		}
		case cl_rgb4: break;
		case cl_rgb5: break;
		case cl_rgb8_snorm:
		{
			valid = true;
			type = CL_BYTE;
			format = CL_BGR;
			break;
		}
		case cl_rgb10: break;
		case cl_rgb12: break;
		case cl_rgb16: break;
		case cl_rgb16_snorm: break;
		case cl_rgba2: break;
		case cl_rgba4:
		{
			valid = true;
			type = CL_UNSIGNED_SHORT_4_4_4_4;
			format = CL_RGBA;
			break;
		}
		case cl_rgb5_a1:
		{
			valid = true;
			type = CL_UNSIGNED_SHORT_5_5_5_1;
			format = CL_RGBA;
			break;
		}
		case cl_rgba8_snorm: break;
		case cl_rgb10_a2: break;
		case cl_rgba12: break;
		case cl_rgba16: break;
		case cl_rgba16_snorm: break;
		case cl_srgb8:
		{
			valid = true;
			type = CL_UNSIGNED_BYTE;
			format = CL_BGR;
			break;
		}
		case cl_srgb8_alpha8: break;
		case cl_r16f:
		{
			valid = true;
			type = CL_HALF_FLOAT;
			format = CL_RED;
			break;
		}
		case cl_rg16f: break;
		case cl_rgb16f: break;
		case cl_rgba16f: break;

		case cl_r32f:
		{
			valid = true;
			type = CL_HALF_FLOAT;
			format = CL_RED;
			break;
		}
		case cl_rg32f: break;
		case cl_rgb32f:
		{
			valid = true;
			type = CL_FLOAT;
			format = CL_BGR;
			break;
		}
		
		case cl_rgba32f:
		{
			valid = true;
			type = CL_FLOAT;
			format = CL_BGRA;
			break;
		}
		
		case cl_r11f_g11f_b10f: break;
		case cl_rgb9_e5: break;
		case cl_r8i:
		{
			valid = true;
			type = CL_BYTE;
			format = CL_RED;
			break;
		}
		case cl_r8ui:
		{
			valid = true;
			type = CL_UNSIGNED_BYTE;
			format = CL_RED;
			break;
		}
		case cl_r16i:
		{
			valid = true;
			type = CL_SHORT;
			format = CL_RED;
			break;
		}
		case cl_r16ui:
		{
			valid = true;
			type = CL_UNSIGNED_SHORT;
			format = CL_RED;
			break;
		}
		case cl_r32i:
		{
			valid = true;
			type = CL_INT;
			format = CL_RED;
			break;
		}
		case cl_r32ui:
		{
			valid = true;
			type = CL_UNSIGNED_INT;
			format = CL_RED;
			break;
		}
		case cl_rg8i: break;
		case cl_rg8ui: break;
		case cl_rg16i: break;
		case cl_rg16ui: break;
		case cl_rg32i: break;
		case cl_rg32ui: break;
		case cl_rgb8i:
		{
			valid = true;
			type = CL_BYTE;
			format = CL_BGR;
			break;
		}
		case cl_rgb8ui:
		{
			valid = true;
			type = CL_UNSIGNED_BYTE;
			format = CL_BGR;
			break;
		}
		case cl_rgb16i: break;
		case cl_rgb16ui: break;
		case cl_rgb32i: break;
		case cl_rgb32ui: break;
		case cl_rgba8i:
		{
			valid = true;
			type = CL_UNSIGNED_INT_8_8_8_8;
			format = CL_RGBA;
			break;
		}
		case cl_rgba8ui:
		{
			valid = true;
			type = CL_UNSIGNED_INT_8_8_8_8;
			format = CL_RGBA;
			break;
		}
		case cl_rgba16i: break;
		case cl_rgba16ui: break;
		case cl_rgba32i: break;
		case cl_rgba32ui: break;
		case cl_depth_component16: break;
		case cl_depth_component24: break;
		case cl_depth_component32: break;
		case cl_depth_component32f: break;
		case cl_depth24_stencil8: break;
		case cl_depth32f_stencil8: break;
		case cl_compressed_red: break;
		case cl_compressed_rg: break;
		case cl_compressed_rgb: break;
		case cl_compressed_rgba: break;
		case cl_compressed_srgb: break;
		case cl_compressed_srgb_alpha: break;
		case cl_compressed_red_rgtc1: break;
		case cl_compressed_signed_red_rgtc1: break;
		case cl_compressed_rg_rgtc2: break;
		case cl_compressed_signed_rg_rgtc2: break;
	}

//	if (valid)
//	{
//		CLint gl_internal_format;
//		to_opengl_textureformat(texture_format, gl_internal_format, format);
//	}

	return valid;
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

static CL_ProcAddress *cl_get_proc_address_extension(const CL_String8& function_name)
{
	CL_ProcAddress *ptr = CL_OpenGL::get_proc_address(function_name);
	if (!ptr)
	{
		ptr = CL_OpenGL::get_proc_address(function_name + "ARB");
	}
	if (!ptr)
	{
		ptr = CL_OpenGL::get_proc_address(function_name + "EXT");
	}
	if (!ptr)
	{
		ptr = CL_OpenGL::get_proc_address(function_name + "ATI");
	}
	return ptr;
}

CL_GLFunctions *cl_setup_binds()
{
	CL_GLFunctions *functions = new CL_GLFunctions;
	memset(functions, 0, sizeof(CL_GLFunctions));

	functions->cullFace = (CL_GLFunctions::ptr_glCullFace) cl_get_proc_address_extension("glCullFace");
	functions->frontFace = (CL_GLFunctions::ptr_glFrontFace) cl_get_proc_address_extension("glFrontFace");
	functions->hint = (CL_GLFunctions::ptr_glHint) cl_get_proc_address_extension("glHint");
	functions->lineWidth = (CL_GLFunctions::ptr_glLineWidth) cl_get_proc_address_extension("glLineWidth");
	functions->pointSize = (CL_GLFunctions::ptr_glPointSize) cl_get_proc_address_extension("glPointSize");
	functions->polygonMode = (CL_GLFunctions::ptr_glPolygonMode) cl_get_proc_address_extension("glPolygonMode");
	functions->scissor = (CL_GLFunctions::ptr_glScissor) cl_get_proc_address_extension("glScissor");
	functions->texParameterf = (CL_GLFunctions::ptr_glTexParameterf) cl_get_proc_address_extension("glTexParameterf");
	functions->texParameterfv = (CL_GLFunctions::ptr_glTexParameterfv) cl_get_proc_address_extension("glTexParameterfv");
	functions->texParameteri = (CL_GLFunctions::ptr_glTexParameteri) cl_get_proc_address_extension("glTexParameteri");
	functions->texParameteriv = (CL_GLFunctions::ptr_glTexParameteriv) cl_get_proc_address_extension("glTexParameteriv");
	functions->texImage1D = (CL_GLFunctions::ptr_glTexImage1D) cl_get_proc_address_extension("glTexImage1D");
	functions->texImage2D = (CL_GLFunctions::ptr_glTexImage2D) cl_get_proc_address_extension("glTexImage2D");
	functions->drawBuffer = (CL_GLFunctions::ptr_glDrawBuffer) cl_get_proc_address_extension("glDrawBuffer");
	functions->clear = (CL_GLFunctions::ptr_glClear) cl_get_proc_address_extension("glClear");
	functions->clearColor = (CL_GLFunctions::ptr_glClearColor) cl_get_proc_address_extension("glClearColor");
	functions->clearStencil = (CL_GLFunctions::ptr_glClearStencil) cl_get_proc_address_extension("glClearStencil");
	functions->clearDepth = (CL_GLFunctions::ptr_glClearDepth) cl_get_proc_address_extension("glClearDepth");
	functions->stencilMask = (CL_GLFunctions::ptr_glStencilMask) cl_get_proc_address_extension("glStencilMask");
	functions->colorMask = (CL_GLFunctions::ptr_glColorMask) cl_get_proc_address_extension("glColorMask");
	functions->depthMask = (CL_GLFunctions::ptr_glDepthMask) cl_get_proc_address_extension("glDepthMask");
	functions->disable = (CL_GLFunctions::ptr_glDisable) cl_get_proc_address_extension("glDisable");
	functions->enable = (CL_GLFunctions::ptr_glEnable) cl_get_proc_address_extension("glEnable");
	functions->finish = (CL_GLFunctions::ptr_glFinish) cl_get_proc_address_extension("glFinish");
	functions->flush = (CL_GLFunctions::ptr_glFlush) cl_get_proc_address_extension("glFlush");
	functions->blendFunc = (CL_GLFunctions::ptr_glBlendFunc) cl_get_proc_address_extension("glBlendFunc");
	functions->logicOp = (CL_GLFunctions::ptr_glLogicOp) cl_get_proc_address_extension("glLogicOp");
	functions->stencilFunc = (CL_GLFunctions::ptr_glStencilFunc) cl_get_proc_address_extension("glStencilFunc");
	functions->stencilOp = (CL_GLFunctions::ptr_glStencilOp) cl_get_proc_address_extension("glStencilOp");
	functions->depthFunc = (CL_GLFunctions::ptr_glDepthFunc) cl_get_proc_address_extension("glDepthFunc");
	functions->pixelStoref = (CL_GLFunctions::ptr_glPixelStoref) cl_get_proc_address_extension("glPixelStoref");
	functions->pixelStorei = (CL_GLFunctions::ptr_glPixelStorei) cl_get_proc_address_extension("glPixelStorei");
	functions->readBuffer = (CL_GLFunctions::ptr_glReadBuffer) cl_get_proc_address_extension("glReadBuffer");
	functions->readPixels = (CL_GLFunctions::ptr_glReadPixels) cl_get_proc_address_extension("glReadPixels");
	functions->getBooleanv = (CL_GLFunctions::ptr_glGetBooleanv) cl_get_proc_address_extension("glGetBooleanv");
	functions->getDoublev = (CL_GLFunctions::ptr_glGetDoublev) cl_get_proc_address_extension("glGetDoublev");
	functions->getError = (CL_GLFunctions::ptr_glGetError) cl_get_proc_address_extension("glGetError");
	functions->getFloatv = (CL_GLFunctions::ptr_glGetFloatv) cl_get_proc_address_extension("glGetFloatv");
	functions->getIntegerv = (CL_GLFunctions::ptr_glGetIntegerv) cl_get_proc_address_extension("glGetIntegerv");
	functions->getString = (CL_GLFunctions::ptr_glGetString) cl_get_proc_address_extension("glGetString");
	functions->getTexImage = (CL_GLFunctions::ptr_glGetTexImage) cl_get_proc_address_extension("glGetTexImage");
	functions->getTexParameterfv = (CL_GLFunctions::ptr_glGetTexParameterfv) cl_get_proc_address_extension("glGetTexParameterfv");
	functions->getTexParameteriv = (CL_GLFunctions::ptr_glGetTexParameteriv) cl_get_proc_address_extension("glGetTexParameteriv");
	functions->getTexLevelParameterfv = (CL_GLFunctions::ptr_glGetTexLevelParameterfv) cl_get_proc_address_extension("glGetTexLevelParameterfv");
	functions->getTexLevelParameteriv = (CL_GLFunctions::ptr_glGetTexLevelParameteriv) cl_get_proc_address_extension("glGetTexLevelParameteriv");
	functions->isEnabled = (CL_GLFunctions::ptr_glIsEnabled) cl_get_proc_address_extension("glIsEnabled");
	functions->depthRange = (CL_GLFunctions::ptr_glDepthRange) cl_get_proc_address_extension("glDepthRange");
	functions->viewport = (CL_GLFunctions::ptr_glViewport) cl_get_proc_address_extension("glViewport");
	functions->drawArrays = (CL_GLFunctions::ptr_glDrawArrays) cl_get_proc_address_extension("glDrawArrays");
	functions->drawElements = (CL_GLFunctions::ptr_glDrawElements) cl_get_proc_address_extension("glDrawElements");
	functions->getPointerv = (CL_GLFunctions::ptr_glGetPointerv) cl_get_proc_address_extension("glGetPointerv");
	functions->polygonOffset = (CL_GLFunctions::ptr_glPolygonOffset) cl_get_proc_address_extension("glPolygonOffset");
	functions->copyTexImage1D = (CL_GLFunctions::ptr_glCopyTexImage1D) cl_get_proc_address_extension("glCopyTexImage1D");
	functions->copyTexImage2D = (CL_GLFunctions::ptr_glCopyTexImage2D) cl_get_proc_address_extension("glCopyTexImage2D");
	functions->copyTexSubImage1D = (CL_GLFunctions::ptr_glCopyTexSubImage1D) cl_get_proc_address_extension("glCopyTexSubImage1D");
	functions->copyTexSubImage2D = (CL_GLFunctions::ptr_glCopyTexSubImage2D) cl_get_proc_address_extension("glCopyTexSubImage2D");
	functions->texSubImage1D = (CL_GLFunctions::ptr_glTexSubImage1D) cl_get_proc_address_extension("glTexSubImage1D");
	functions->texSubImage2D = (CL_GLFunctions::ptr_glTexSubImage2D) cl_get_proc_address_extension("glTexSubImage2D");
	functions->bindTexture = (CL_GLFunctions::ptr_glBindTexture) cl_get_proc_address_extension("glBindTexture");
	functions->deleteTextures = (CL_GLFunctions::ptr_glDeleteTextures) cl_get_proc_address_extension("glDeleteTextures");
	functions->genTextures = (CL_GLFunctions::ptr_glGenTextures) cl_get_proc_address_extension("glGenTextures");
	functions->isTexture = (CL_GLFunctions::ptr_glIsTexture) cl_get_proc_address_extension("glIsTexture");
	functions->blendColor = (CL_GLFunctions::ptr_glBlendColor) cl_get_proc_address_extension("glBlendColor");
	functions->blendEquation = (CL_GLFunctions::ptr_glBlendEquation) cl_get_proc_address_extension("glBlendEquation");
	functions->drawRangeElements = (CL_GLFunctions::ptr_glDrawRangeElements) cl_get_proc_address_extension("glDrawRangeElements");
	functions->texImage3D = (CL_GLFunctions::ptr_glTexImage3D) cl_get_proc_address_extension("glTexImage3D");
	functions->texSubImage3D = (CL_GLFunctions::ptr_glTexSubImage3D) cl_get_proc_address_extension("glTexSubImage3D");
	functions->copyTexSubImage3D = (CL_GLFunctions::ptr_glCopyTexSubImage3D) cl_get_proc_address_extension("glCopyTexSubImage3D");
	functions->activeTexture = (CL_GLFunctions::ptr_glActiveTexture) cl_get_proc_address_extension("glActiveTexture");
	functions->sampleCoverage = (CL_GLFunctions::ptr_glSampleCoverage) cl_get_proc_address_extension("glSampleCoverage");
	functions->compressedTexImage3D = (CL_GLFunctions::ptr_glCompressedTexImage3D) cl_get_proc_address_extension("glCompressedTexImage3D");
	functions->compressedTexImage2D = (CL_GLFunctions::ptr_glCompressedTexImage2D) cl_get_proc_address_extension("glCompressedTexImage2D");
	functions->compressedTexImage1D = (CL_GLFunctions::ptr_glCompressedTexImage1D) cl_get_proc_address_extension("glCompressedTexImage1D");
	functions->compressedTexSubImage3D = (CL_GLFunctions::ptr_glCompressedTexSubImage3D) cl_get_proc_address_extension("glCompressedTexSubImage3D");
	functions->compressedTexSubImage2D = (CL_GLFunctions::ptr_glCompressedTexSubImage2D) cl_get_proc_address_extension("glCompressedTexSubImage2D");
	functions->compressedTexSubImage1D = (CL_GLFunctions::ptr_glCompressedTexSubImage1D) cl_get_proc_address_extension("glCompressedTexSubImage1D");
	functions->getCompressedTexImage = (CL_GLFunctions::ptr_glGetCompressedTexImage) cl_get_proc_address_extension("glGetCompressedTexImage");
	functions->blendFuncSeparate = (CL_GLFunctions::ptr_glBlendFuncSeparate) cl_get_proc_address_extension("glBlendFuncSeparate");
	functions->multiDrawArrays = (CL_GLFunctions::ptr_glMultiDrawArrays) cl_get_proc_address_extension("glMultiDrawArrays");
	functions->multiDrawElements = (CL_GLFunctions::ptr_glMultiDrawElements) cl_get_proc_address_extension("glMultiDrawElements");
	functions->pointParameterf = (CL_GLFunctions::ptr_glPointParameterf) cl_get_proc_address_extension("glPointParameterf");
	functions->pointParameterfv = (CL_GLFunctions::ptr_glPointParameterfv) cl_get_proc_address_extension("glPointParameterfv");
	functions->pointParameteri = (CL_GLFunctions::ptr_glPointParameteri) cl_get_proc_address_extension("glPointParameteri");
	functions->pointParameteriv = (CL_GLFunctions::ptr_glPointParameteriv) cl_get_proc_address_extension("glPointParameteriv");
	functions->genQueries = (CL_GLFunctions::ptr_glGenQueries) cl_get_proc_address_extension("glGenQueries");
	functions->deleteQueries = (CL_GLFunctions::ptr_glDeleteQueries) cl_get_proc_address_extension("glDeleteQueries");
	functions->isQuery = (CL_GLFunctions::ptr_glIsQuery) cl_get_proc_address_extension("glIsQuery");
	functions->beginQuery = (CL_GLFunctions::ptr_glBeginQuery) cl_get_proc_address_extension("glBeginQuery");
	functions->endQuery = (CL_GLFunctions::ptr_glEndQuery) cl_get_proc_address_extension("glEndQuery");
	functions->getQueryiv = (CL_GLFunctions::ptr_glGetQueryiv) cl_get_proc_address_extension("glGetQueryiv");
	functions->getQueryObjectiv = (CL_GLFunctions::ptr_glGetQueryObjectiv) cl_get_proc_address_extension("glGetQueryObjectiv");
	functions->getQueryObjectuiv = (CL_GLFunctions::ptr_glGetQueryObjectuiv) cl_get_proc_address_extension("glGetQueryObjectuiv");
	functions->bindBuffer = (CL_GLFunctions::ptr_glBindBuffer) cl_get_proc_address_extension("glBindBuffer");
	functions->deleteBuffers = (CL_GLFunctions::ptr_glDeleteBuffers) cl_get_proc_address_extension("glDeleteBuffers");
	functions->genBuffers = (CL_GLFunctions::ptr_glGenBuffers) cl_get_proc_address_extension("glGenBuffers");
	functions->isBuffer = (CL_GLFunctions::ptr_glIsBuffer) cl_get_proc_address_extension("glIsBuffer");
	functions->bufferData = (CL_GLFunctions::ptr_glBufferData) cl_get_proc_address_extension("glBufferData");
	functions->bufferSubData = (CL_GLFunctions::ptr_glBufferSubData) cl_get_proc_address_extension("glBufferSubData");
	functions->getBufferSubData = (CL_GLFunctions::ptr_glGetBufferSubData) cl_get_proc_address_extension("glGetBufferSubData");
	functions->mapBuffer = (CL_GLFunctions::ptr_glMapBuffer) cl_get_proc_address_extension("glMapBuffer");
	functions->unmapBuffer = (CL_GLFunctions::ptr_glUnmapBuffer) cl_get_proc_address_extension("glUnmapBuffer");
	functions->getBufferParameteriv = (CL_GLFunctions::ptr_glGetBufferParameteriv) cl_get_proc_address_extension("glGetBufferParameteriv");
	functions->getBufferPointerv = (CL_GLFunctions::ptr_glGetBufferPointerv) cl_get_proc_address_extension("glGetBufferPointerv");
	functions->blendEquationSeparate = (CL_GLFunctions::ptr_glBlendEquationSeparate) cl_get_proc_address_extension("glBlendEquationSeparate");
	functions->drawBuffers = (CL_GLFunctions::ptr_glDrawBuffers) cl_get_proc_address_extension("glDrawBuffers");
	functions->stencilOpSeparate = (CL_GLFunctions::ptr_glStencilOpSeparate) cl_get_proc_address_extension("glStencilOpSeparate");
	functions->stencilFuncSeparate = (CL_GLFunctions::ptr_glStencilFuncSeparate) cl_get_proc_address_extension("glStencilFuncSeparate");
	functions->stencilMaskSeparate = (CL_GLFunctions::ptr_glStencilMaskSeparate) cl_get_proc_address_extension("glStencilMaskSeparate");
	functions->attachShader = (CL_GLFunctions::ptr_glAttachShader) cl_get_proc_address_extension("glAttachShader");
	functions->bindAttribLocation = (CL_GLFunctions::ptr_glBindAttribLocation) cl_get_proc_address_extension("glBindAttribLocation");
	functions->compileShader = (CL_GLFunctions::ptr_glCompileShader) cl_get_proc_address_extension("glCompileShader");
	functions->createProgram = (CL_GLFunctions::ptr_glCreateProgram) cl_get_proc_address_extension("glCreateProgram");
	functions->createShader = (CL_GLFunctions::ptr_glCreateShader) cl_get_proc_address_extension("glCreateShader");
	functions->deleteProgram = (CL_GLFunctions::ptr_glDeleteProgram) cl_get_proc_address_extension("glDeleteProgram");
	functions->deleteShader = (CL_GLFunctions::ptr_glDeleteShader) cl_get_proc_address_extension("glDeleteShader");
	functions->detachShader = (CL_GLFunctions::ptr_glDetachShader) cl_get_proc_address_extension("glDetachShader");
	functions->disableVertexAttribArray = (CL_GLFunctions::ptr_glDisableVertexAttribArray) cl_get_proc_address_extension("glDisableVertexAttribArray");
	functions->enableVertexAttribArray = (CL_GLFunctions::ptr_glEnableVertexAttribArray) cl_get_proc_address_extension("glEnableVertexAttribArray");
	functions->getActiveAttrib = (CL_GLFunctions::ptr_glGetActiveAttrib) cl_get_proc_address_extension("glGetActiveAttrib");
	functions->getActiveUniform = (CL_GLFunctions::ptr_glGetActiveUniform) cl_get_proc_address_extension("glGetActiveUniform");
	functions->getAttachedShaders = (CL_GLFunctions::ptr_glGetAttachedShaders) cl_get_proc_address_extension("glGetAttachedShaders");
	functions->getAttribLocation = (CL_GLFunctions::ptr_glGetAttribLocation) cl_get_proc_address_extension("glGetAttribLocation");
	functions->getProgramiv = (CL_GLFunctions::ptr_glGetProgramiv) cl_get_proc_address_extension("glGetProgramiv");
	functions->getProgramInfoLog = (CL_GLFunctions::ptr_glGetProgramInfoLog) cl_get_proc_address_extension("glGetProgramInfoLog");
	functions->getShaderiv = (CL_GLFunctions::ptr_glGetShaderiv) cl_get_proc_address_extension("glGetShaderiv");
	functions->getShaderInfoLog = (CL_GLFunctions::ptr_glGetShaderInfoLog) cl_get_proc_address_extension("glGetShaderInfoLog");
	functions->getShaderSource = (CL_GLFunctions::ptr_glGetShaderSource) cl_get_proc_address_extension("glGetShaderSource");
	functions->getUniformLocation = (CL_GLFunctions::ptr_glGetUniformLocation) cl_get_proc_address_extension("glGetUniformLocation");
	functions->getUniformfv = (CL_GLFunctions::ptr_glGetUniformfv) cl_get_proc_address_extension("glGetUniformfv");
	functions->getUniformiv = (CL_GLFunctions::ptr_glGetUniformiv) cl_get_proc_address_extension("glGetUniformiv");
	functions->getVertexAttribdv = (CL_GLFunctions::ptr_glGetVertexAttribdv) cl_get_proc_address_extension("glGetVertexAttribdv");
	functions->getVertexAttribfv = (CL_GLFunctions::ptr_glGetVertexAttribfv) cl_get_proc_address_extension("glGetVertexAttribfv");
	functions->getVertexAttribiv = (CL_GLFunctions::ptr_glGetVertexAttribiv) cl_get_proc_address_extension("glGetVertexAttribiv");
	functions->getVertexAttribPointerv = (CL_GLFunctions::ptr_glGetVertexAttribPointerv) cl_get_proc_address_extension("glGetVertexAttribPointerv");
	functions->isProgram = (CL_GLFunctions::ptr_glIsProgram) cl_get_proc_address_extension("glIsProgram");
	functions->isShader = (CL_GLFunctions::ptr_glIsShader) cl_get_proc_address_extension("glIsShader");
	functions->linkProgram = (CL_GLFunctions::ptr_glLinkProgram) cl_get_proc_address_extension("glLinkProgram");
	functions->shaderSource = (CL_GLFunctions::ptr_glShaderSource) cl_get_proc_address_extension("glShaderSource");
	functions->useProgram = (CL_GLFunctions::ptr_glUseProgram) cl_get_proc_address_extension("glUseProgram");
	functions->uniform1f = (CL_GLFunctions::ptr_glUniform1f) cl_get_proc_address_extension("glUniform1f");
	functions->uniform2f = (CL_GLFunctions::ptr_glUniform2f) cl_get_proc_address_extension("glUniform2f");
	functions->uniform3f = (CL_GLFunctions::ptr_glUniform3f) cl_get_proc_address_extension("glUniform3f");
	functions->uniform4f = (CL_GLFunctions::ptr_glUniform4f) cl_get_proc_address_extension("glUniform4f");
	functions->uniform1i = (CL_GLFunctions::ptr_glUniform1i) cl_get_proc_address_extension("glUniform1i");
	functions->uniform2i = (CL_GLFunctions::ptr_glUniform2i) cl_get_proc_address_extension("glUniform2i");
	functions->uniform3i = (CL_GLFunctions::ptr_glUniform3i) cl_get_proc_address_extension("glUniform3i");
	functions->uniform4i = (CL_GLFunctions::ptr_glUniform4i) cl_get_proc_address_extension("glUniform4i");
	functions->uniform1fv = (CL_GLFunctions::ptr_glUniform1fv) cl_get_proc_address_extension("glUniform1fv");
	functions->uniform2fv = (CL_GLFunctions::ptr_glUniform2fv) cl_get_proc_address_extension("glUniform2fv");
	functions->uniform3fv = (CL_GLFunctions::ptr_glUniform3fv) cl_get_proc_address_extension("glUniform3fv");
	functions->uniform4fv = (CL_GLFunctions::ptr_glUniform4fv) cl_get_proc_address_extension("glUniform4fv");
	functions->uniform1iv = (CL_GLFunctions::ptr_glUniform1iv) cl_get_proc_address_extension("glUniform1iv");
	functions->uniform2iv = (CL_GLFunctions::ptr_glUniform2iv) cl_get_proc_address_extension("glUniform2iv");
	functions->uniform3iv = (CL_GLFunctions::ptr_glUniform3iv) cl_get_proc_address_extension("glUniform3iv");
	functions->uniform4iv = (CL_GLFunctions::ptr_glUniform4iv) cl_get_proc_address_extension("glUniform4iv");
	functions->uniformMatrix2fv = (CL_GLFunctions::ptr_glUniformMatrix2fv) cl_get_proc_address_extension("glUniformMatrix2fv");
	functions->uniformMatrix3fv = (CL_GLFunctions::ptr_glUniformMatrix3fv) cl_get_proc_address_extension("glUniformMatrix3fv");
	functions->uniformMatrix4fv = (CL_GLFunctions::ptr_glUniformMatrix4fv) cl_get_proc_address_extension("glUniformMatrix4fv");
	functions->validateProgram = (CL_GLFunctions::ptr_glValidateProgram) cl_get_proc_address_extension("glValidateProgram");
	functions->vertexAttrib1d = (CL_GLFunctions::ptr_glVertexAttrib1d) cl_get_proc_address_extension("glVertexAttrib1d");
	functions->vertexAttrib1dv = (CL_GLFunctions::ptr_glVertexAttrib1dv) cl_get_proc_address_extension("glVertexAttrib1dv");
	functions->vertexAttrib1f = (CL_GLFunctions::ptr_glVertexAttrib1f) cl_get_proc_address_extension("glVertexAttrib1f");
	functions->vertexAttrib1fv = (CL_GLFunctions::ptr_glVertexAttrib1fv) cl_get_proc_address_extension("glVertexAttrib1fv");
	functions->vertexAttrib1s = (CL_GLFunctions::ptr_glVertexAttrib1s) cl_get_proc_address_extension("glVertexAttrib1s");
	functions->vertexAttrib1sv = (CL_GLFunctions::ptr_glVertexAttrib1sv) cl_get_proc_address_extension("glVertexAttrib1sv");
	functions->vertexAttrib2d = (CL_GLFunctions::ptr_glVertexAttrib2d) cl_get_proc_address_extension("glVertexAttrib2d");
	functions->vertexAttrib2dv = (CL_GLFunctions::ptr_glVertexAttrib2dv) cl_get_proc_address_extension("glVertexAttrib2dv");
	functions->vertexAttrib2f = (CL_GLFunctions::ptr_glVertexAttrib2f) cl_get_proc_address_extension("glVertexAttrib2f");
	functions->vertexAttrib2fv = (CL_GLFunctions::ptr_glVertexAttrib2fv) cl_get_proc_address_extension("glVertexAttrib2fv");
	functions->vertexAttrib2s = (CL_GLFunctions::ptr_glVertexAttrib2s) cl_get_proc_address_extension("glVertexAttrib2s");
	functions->vertexAttrib2sv = (CL_GLFunctions::ptr_glVertexAttrib2sv) cl_get_proc_address_extension("glVertexAttrib2sv");
	functions->vertexAttrib3d = (CL_GLFunctions::ptr_glVertexAttrib3d) cl_get_proc_address_extension("glVertexAttrib3d");
	functions->vertexAttrib3dv = (CL_GLFunctions::ptr_glVertexAttrib3dv) cl_get_proc_address_extension("glVertexAttrib3dv");
	functions->vertexAttrib3f = (CL_GLFunctions::ptr_glVertexAttrib3f) cl_get_proc_address_extension("glVertexAttrib3f");
	functions->vertexAttrib3fv = (CL_GLFunctions::ptr_glVertexAttrib3fv) cl_get_proc_address_extension("glVertexAttrib3fv");
	functions->vertexAttrib3s = (CL_GLFunctions::ptr_glVertexAttrib3s) cl_get_proc_address_extension("glVertexAttrib3s");
	functions->vertexAttrib3sv = (CL_GLFunctions::ptr_glVertexAttrib3sv) cl_get_proc_address_extension("glVertexAttrib3sv");
	functions->vertexAttrib4Nbv = (CL_GLFunctions::ptr_glVertexAttrib4Nbv) cl_get_proc_address_extension("glVertexAttrib4Nbv");
	functions->vertexAttrib4Niv = (CL_GLFunctions::ptr_glVertexAttrib4Niv) cl_get_proc_address_extension("glVertexAttrib4Niv");
	functions->vertexAttrib4Nsv = (CL_GLFunctions::ptr_glVertexAttrib4Nsv) cl_get_proc_address_extension("glVertexAttrib4Nsv");
	functions->vertexAttrib4Nub = (CL_GLFunctions::ptr_glVertexAttrib4Nub) cl_get_proc_address_extension("glVertexAttrib4Nub");
	functions->vertexAttrib4Nubv = (CL_GLFunctions::ptr_glVertexAttrib4Nubv) cl_get_proc_address_extension("glVertexAttrib4Nubv");
	functions->vertexAttrib4Nuiv = (CL_GLFunctions::ptr_glVertexAttrib4Nuiv) cl_get_proc_address_extension("glVertexAttrib4Nuiv");
	functions->vertexAttrib4Nusv = (CL_GLFunctions::ptr_glVertexAttrib4Nusv) cl_get_proc_address_extension("glVertexAttrib4Nusv");
	functions->vertexAttrib4bv = (CL_GLFunctions::ptr_glVertexAttrib4bv) cl_get_proc_address_extension("glVertexAttrib4bv");
	functions->vertexAttrib4d = (CL_GLFunctions::ptr_glVertexAttrib4d) cl_get_proc_address_extension("glVertexAttrib4d");
	functions->vertexAttrib4dv = (CL_GLFunctions::ptr_glVertexAttrib4dv) cl_get_proc_address_extension("glVertexAttrib4dv");
	functions->vertexAttrib4f = (CL_GLFunctions::ptr_glVertexAttrib4f) cl_get_proc_address_extension("glVertexAttrib4f");
	functions->vertexAttrib4fv = (CL_GLFunctions::ptr_glVertexAttrib4fv) cl_get_proc_address_extension("glVertexAttrib4fv");
	functions->vertexAttrib4iv = (CL_GLFunctions::ptr_glVertexAttrib4iv) cl_get_proc_address_extension("glVertexAttrib4iv");
	functions->vertexAttrib4s = (CL_GLFunctions::ptr_glVertexAttrib4s) cl_get_proc_address_extension("glVertexAttrib4s");
	functions->vertexAttrib4sv = (CL_GLFunctions::ptr_glVertexAttrib4sv) cl_get_proc_address_extension("glVertexAttrib4sv");
	functions->vertexAttrib4ubv = (CL_GLFunctions::ptr_glVertexAttrib4ubv) cl_get_proc_address_extension("glVertexAttrib4ubv");
	functions->vertexAttrib4uiv = (CL_GLFunctions::ptr_glVertexAttrib4uiv) cl_get_proc_address_extension("glVertexAttrib4uiv");
	functions->vertexAttrib4usv = (CL_GLFunctions::ptr_glVertexAttrib4usv) cl_get_proc_address_extension("glVertexAttrib4usv");
	functions->vertexAttribPointer = (CL_GLFunctions::ptr_glVertexAttribPointer) cl_get_proc_address_extension("glVertexAttribPointer");
	functions->uniformMatrix2x3fv = (CL_GLFunctions::ptr_glUniformMatrix2x3fv) cl_get_proc_address_extension("glUniformMatrix2x3fv");
	functions->uniformMatrix3x2fv = (CL_GLFunctions::ptr_glUniformMatrix3x2fv) cl_get_proc_address_extension("glUniformMatrix3x2fv");
	functions->uniformMatrix2x4fv = (CL_GLFunctions::ptr_glUniformMatrix2x4fv) cl_get_proc_address_extension("glUniformMatrix2x4fv");
	functions->uniformMatrix4x2fv = (CL_GLFunctions::ptr_glUniformMatrix4x2fv) cl_get_proc_address_extension("glUniformMatrix4x2fv");
	functions->uniformMatrix3x4fv = (CL_GLFunctions::ptr_glUniformMatrix3x4fv) cl_get_proc_address_extension("glUniformMatrix3x4fv");
	functions->uniformMatrix4x3fv = (CL_GLFunctions::ptr_glUniformMatrix4x3fv) cl_get_proc_address_extension("glUniformMatrix4x3fv");
	functions->colorMaski = (CL_GLFunctions::ptr_glColorMaski) cl_get_proc_address_extension("glColorMaski");
	functions->getBooleani_v = (CL_GLFunctions::ptr_glGetBooleani_v) cl_get_proc_address_extension("glGetBooleani_v");
	functions->getIntegeri_v = (CL_GLFunctions::ptr_glGetIntegeri_v) cl_get_proc_address_extension("glGetIntegeri_v");
	functions->enablei = (CL_GLFunctions::ptr_glEnablei) cl_get_proc_address_extension("glEnablei");
	functions->disablei = (CL_GLFunctions::ptr_glDisablei) cl_get_proc_address_extension("glDisablei");
	functions->isEnabledi = (CL_GLFunctions::ptr_glIsEnabledi) cl_get_proc_address_extension("glIsEnabledi");
	functions->beginTransformFeedback = (CL_GLFunctions::ptr_glBeginTransformFeedback) cl_get_proc_address_extension("glBeginTransformFeedback");
	functions->endTransformFeedback = (CL_GLFunctions::ptr_glEndTransformFeedback) cl_get_proc_address_extension("glEndTransformFeedback");
	functions->bindBufferRange = (CL_GLFunctions::ptr_glBindBufferRange) cl_get_proc_address_extension("glBindBufferRange");
	functions->bindBufferBase = (CL_GLFunctions::ptr_glBindBufferBase) cl_get_proc_address_extension("glBindBufferBase");
	functions->transformFeedbackVaryings = (CL_GLFunctions::ptr_glTransformFeedbackVaryings) cl_get_proc_address_extension("glTransformFeedbackVaryings");
	functions->getTransformFeedbackVarying = (CL_GLFunctions::ptr_glGetTransformFeedbackVarying) cl_get_proc_address_extension("glGetTransformFeedbackVarying");
	functions->clampColor = (CL_GLFunctions::ptr_glClampColor) cl_get_proc_address_extension("glClampColor");
	functions->beginConditionalRender = (CL_GLFunctions::ptr_glBeginConditionalRender) cl_get_proc_address_extension("glBeginConditionalRender");
	functions->endConditionalRender = (CL_GLFunctions::ptr_glEndConditionalRender) cl_get_proc_address_extension("glEndConditionalRender");
	functions->vertexAttribIPointer = (CL_GLFunctions::ptr_glVertexAttribIPointer) cl_get_proc_address_extension("glVertexAttribIPointer");
	functions->getVertexAttribIiv = (CL_GLFunctions::ptr_glGetVertexAttribIiv) cl_get_proc_address_extension("glGetVertexAttribIiv");
	functions->getVertexAttribIuiv = (CL_GLFunctions::ptr_glGetVertexAttribIuiv) cl_get_proc_address_extension("glGetVertexAttribIuiv");
	functions->vertexAttribI1i = (CL_GLFunctions::ptr_glVertexAttribI1i) cl_get_proc_address_extension("glVertexAttribI1i");
	functions->vertexAttribI2i = (CL_GLFunctions::ptr_glVertexAttribI2i) cl_get_proc_address_extension("glVertexAttribI2i");
	functions->vertexAttribI3i = (CL_GLFunctions::ptr_glVertexAttribI3i) cl_get_proc_address_extension("glVertexAttribI3i");
	functions->vertexAttribI4i = (CL_GLFunctions::ptr_glVertexAttribI4i) cl_get_proc_address_extension("glVertexAttribI4i");
	functions->vertexAttribI1ui = (CL_GLFunctions::ptr_glVertexAttribI1ui) cl_get_proc_address_extension("glVertexAttribI1ui");
	functions->vertexAttribI2ui = (CL_GLFunctions::ptr_glVertexAttribI2ui) cl_get_proc_address_extension("glVertexAttribI2ui");
	functions->vertexAttribI3ui = (CL_GLFunctions::ptr_glVertexAttribI3ui) cl_get_proc_address_extension("glVertexAttribI3ui");
	functions->vertexAttribI4ui = (CL_GLFunctions::ptr_glVertexAttribI4ui) cl_get_proc_address_extension("glVertexAttribI4ui");
	functions->vertexAttribI1iv = (CL_GLFunctions::ptr_glVertexAttribI1iv) cl_get_proc_address_extension("glVertexAttribI1iv");
	functions->vertexAttribI2iv = (CL_GLFunctions::ptr_glVertexAttribI2iv) cl_get_proc_address_extension("glVertexAttribI2iv");
	functions->vertexAttribI3iv = (CL_GLFunctions::ptr_glVertexAttribI3iv) cl_get_proc_address_extension("glVertexAttribI3iv");
	functions->vertexAttribI4iv = (CL_GLFunctions::ptr_glVertexAttribI4iv) cl_get_proc_address_extension("glVertexAttribI4iv");
	functions->vertexAttribI1uiv = (CL_GLFunctions::ptr_glVertexAttribI1uiv) cl_get_proc_address_extension("glVertexAttribI1uiv");
	functions->vertexAttribI2uiv = (CL_GLFunctions::ptr_glVertexAttribI2uiv) cl_get_proc_address_extension("glVertexAttribI2uiv");
	functions->vertexAttribI3uiv = (CL_GLFunctions::ptr_glVertexAttribI3uiv) cl_get_proc_address_extension("glVertexAttribI3uiv");
	functions->vertexAttribI4uiv = (CL_GLFunctions::ptr_glVertexAttribI4uiv) cl_get_proc_address_extension("glVertexAttribI4uiv");
	functions->vertexAttribI4bv = (CL_GLFunctions::ptr_glVertexAttribI4bv) cl_get_proc_address_extension("glVertexAttribI4bv");
	functions->vertexAttribI4sv = (CL_GLFunctions::ptr_glVertexAttribI4sv) cl_get_proc_address_extension("glVertexAttribI4sv");
	functions->vertexAttribI4ubv = (CL_GLFunctions::ptr_glVertexAttribI4ubv) cl_get_proc_address_extension("glVertexAttribI4ubv");
	functions->vertexAttribI4usv = (CL_GLFunctions::ptr_glVertexAttribI4usv) cl_get_proc_address_extension("glVertexAttribI4usv");
	functions->getUniformuiv = (CL_GLFunctions::ptr_glGetUniformuiv) cl_get_proc_address_extension("glGetUniformuiv");
	functions->bindFragDataLocation = (CL_GLFunctions::ptr_glBindFragDataLocation) cl_get_proc_address_extension("glBindFragDataLocation");
	functions->getFragDataLocation = (CL_GLFunctions::ptr_glGetFragDataLocation) cl_get_proc_address_extension("glGetFragDataLocation");
	functions->uniform1ui = (CL_GLFunctions::ptr_glUniform1ui) cl_get_proc_address_extension("glUniform1ui");
	functions->uniform2ui = (CL_GLFunctions::ptr_glUniform2ui) cl_get_proc_address_extension("glUniform2ui");
	functions->uniform3ui = (CL_GLFunctions::ptr_glUniform3ui) cl_get_proc_address_extension("glUniform3ui");
	functions->uniform4ui = (CL_GLFunctions::ptr_glUniform4ui) cl_get_proc_address_extension("glUniform4ui");
	functions->uniform1uiv = (CL_GLFunctions::ptr_glUniform1uiv) cl_get_proc_address_extension("glUniform1uiv");
	functions->uniform2uiv = (CL_GLFunctions::ptr_glUniform2uiv) cl_get_proc_address_extension("glUniform2uiv");
	functions->uniform3uiv = (CL_GLFunctions::ptr_glUniform3uiv) cl_get_proc_address_extension("glUniform3uiv");
	functions->uniform4uiv = (CL_GLFunctions::ptr_glUniform4uiv) cl_get_proc_address_extension("glUniform4uiv");
	functions->texParameterIiv = (CL_GLFunctions::ptr_glTexParameterIiv) cl_get_proc_address_extension("glTexParameterIiv");
	functions->texParameterIuiv = (CL_GLFunctions::ptr_glTexParameterIuiv) cl_get_proc_address_extension("glTexParameterIuiv");
	functions->getTexParameterIiv = (CL_GLFunctions::ptr_glGetTexParameterIiv) cl_get_proc_address_extension("glGetTexParameterIiv");
	functions->getTexParameterIuiv = (CL_GLFunctions::ptr_glGetTexParameterIuiv) cl_get_proc_address_extension("glGetTexParameterIuiv");
	functions->clearBufferiv = (CL_GLFunctions::ptr_glClearBufferiv) cl_get_proc_address_extension("glClearBufferiv");
	functions->clearBufferuiv = (CL_GLFunctions::ptr_glClearBufferuiv) cl_get_proc_address_extension("glClearBufferuiv");
	functions->clearBufferfv = (CL_GLFunctions::ptr_glClearBufferfv) cl_get_proc_address_extension("glClearBufferfv");
	functions->clearBufferfi = (CL_GLFunctions::ptr_glClearBufferfi) cl_get_proc_address_extension("glClearBufferfi");
	functions->getStringi = (CL_GLFunctions::ptr_glGetStringi) cl_get_proc_address_extension("glGetStringi");
	functions->drawArraysInstanced = (CL_GLFunctions::ptr_glDrawArraysInstanced) cl_get_proc_address_extension("glDrawArraysInstanced");
	functions->drawElementsInstanced = (CL_GLFunctions::ptr_glDrawElementsInstanced) cl_get_proc_address_extension("glDrawElementsInstanced");
	functions->texBuffer = (CL_GLFunctions::ptr_glTexBuffer) cl_get_proc_address_extension("glTexBuffer");
	functions->primitiveRestartIndex = (CL_GLFunctions::ptr_glPrimitiveRestartIndex) cl_get_proc_address_extension("glPrimitiveRestartIndex");
	functions->getInteger64i_v = (CL_GLFunctions::ptr_glGetInteger64i_v) cl_get_proc_address_extension("glGetInteger64i_v");
	functions->getBufferParameteri64v = (CL_GLFunctions::ptr_glGetBufferParameteri64v) cl_get_proc_address_extension("glGetBufferParameteri64v");
	functions->programParameteri = (CL_GLFunctions::ptr_glProgramParameteri) cl_get_proc_address_extension("glProgramParameteri");
	functions->framebufferTexture = (CL_GLFunctions::ptr_glFramebufferTexture) cl_get_proc_address_extension("glFramebufferTexture");
	functions->isRenderbuffer = (CL_GLFunctions::ptr_glIsRenderbuffer) cl_get_proc_address_extension("glIsRenderbuffer");
	functions->bindRenderbuffer = (CL_GLFunctions::ptr_glBindRenderbuffer) cl_get_proc_address_extension("glBindRenderbuffer");
	functions->deleteRenderbuffers = (CL_GLFunctions::ptr_glDeleteRenderbuffers) cl_get_proc_address_extension("glDeleteRenderbuffers");
	functions->genRenderbuffers = (CL_GLFunctions::ptr_glGenRenderbuffers) cl_get_proc_address_extension("glGenRenderbuffers");
	functions->renderbufferStorage = (CL_GLFunctions::ptr_glRenderbufferStorage) cl_get_proc_address_extension("glRenderbufferStorage");
	functions->getRenderbufferParameteriv = (CL_GLFunctions::ptr_glGetRenderbufferParameteriv) cl_get_proc_address_extension("glGetRenderbufferParameteriv");
	functions->isFramebuffer = (CL_GLFunctions::ptr_glIsFramebuffer) cl_get_proc_address_extension("glIsFramebuffer");
	functions->bindFramebuffer = (CL_GLFunctions::ptr_glBindFramebuffer) cl_get_proc_address_extension("glBindFramebuffer");
	functions->deleteFramebuffers = (CL_GLFunctions::ptr_glDeleteFramebuffers) cl_get_proc_address_extension("glDeleteFramebuffers");
	functions->genFramebuffers = (CL_GLFunctions::ptr_glGenFramebuffers) cl_get_proc_address_extension("glGenFramebuffers");
	functions->checkFramebufferStatus = (CL_GLFunctions::ptr_glCheckFramebufferStatus) cl_get_proc_address_extension("glCheckFramebufferStatus");
	functions->framebufferTexture1D = (CL_GLFunctions::ptr_glFramebufferTexture1D) cl_get_proc_address_extension("glFramebufferTexture1D");
	functions->framebufferTexture2D = (CL_GLFunctions::ptr_glFramebufferTexture2D) cl_get_proc_address_extension("glFramebufferTexture2D");
	functions->framebufferTexture3D = (CL_GLFunctions::ptr_glFramebufferTexture3D) cl_get_proc_address_extension("glFramebufferTexture3D");
	functions->framebufferRenderbuffer = (CL_GLFunctions::ptr_glFramebufferRenderbuffer) cl_get_proc_address_extension("glFramebufferRenderbuffer");
	functions->getFramebufferAttachmentParameteriv = (CL_GLFunctions::ptr_glGetFramebufferAttachmentParameteriv) cl_get_proc_address_extension("glGetFramebufferAttachmentParameteriv");
	functions->generateMipmap = (CL_GLFunctions::ptr_glGenerateMipmap) cl_get_proc_address_extension("glGenerateMipmap");
	functions->blitFramebuffer = (CL_GLFunctions::ptr_glBlitFramebuffer) cl_get_proc_address_extension("glBlitFramebuffer");
	functions->renderbufferStorageMultisample = (CL_GLFunctions::ptr_glRenderbufferStorageMultisample) cl_get_proc_address_extension("glRenderbufferStorageMultisample");
	functions->framebufferTextureLayer = (CL_GLFunctions::ptr_glFramebufferTextureLayer) cl_get_proc_address_extension("glFramebufferTextureLayer");
	functions->mapBufferRange = (CL_GLFunctions::ptr_glMapBufferRange) cl_get_proc_address_extension("glMapBufferRange");
	functions->flushMappedBufferRange = (CL_GLFunctions::ptr_glFlushMappedBufferRange) cl_get_proc_address_extension("glFlushMappedBufferRange");
	functions->bindVertexArray = (CL_GLFunctions::ptr_glBindVertexArray) cl_get_proc_address_extension("glBindVertexArray");
	functions->deleteVertexArrays = (CL_GLFunctions::ptr_glDeleteVertexArrays) cl_get_proc_address_extension("glDeleteVertexArrays");
	functions->genVertexArrays = (CL_GLFunctions::ptr_glGenVertexArrays) cl_get_proc_address_extension("glGenVertexArrays");
	functions->isVertexArray = (CL_GLFunctions::ptr_glIsVertexArray) cl_get_proc_address_extension("glIsVertexArray");
	functions->getUniformIndices = (CL_GLFunctions::ptr_glGetUniformIndices) cl_get_proc_address_extension("glGetUniformIndices");
	functions->getActiveUniformsiv = (CL_GLFunctions::ptr_glGetActiveUniformsiv) cl_get_proc_address_extension("glGetActiveUniformsiv");
	functions->getActiveUniformName = (CL_GLFunctions::ptr_glGetActiveUniformName) cl_get_proc_address_extension("glGetActiveUniformName");
	functions->getUniformBlockIndex = (CL_GLFunctions::ptr_glGetUniformBlockIndex) cl_get_proc_address_extension("glGetUniformBlockIndex");
	functions->getActiveUniformBlockiv = (CL_GLFunctions::ptr_glGetActiveUniformBlockiv) cl_get_proc_address_extension("glGetActiveUniformBlockiv");
	functions->getActiveUniformBlockName = (CL_GLFunctions::ptr_glGetActiveUniformBlockName) cl_get_proc_address_extension("glGetActiveUniformBlockName");
	functions->uniformBlockBinding = (CL_GLFunctions::ptr_glUniformBlockBinding) cl_get_proc_address_extension("glUniformBlockBinding");
	functions->copyBufferSubData = (CL_GLFunctions::ptr_glCopyBufferSubData) cl_get_proc_address_extension("glCopyBufferSubData");
	functions->drawElementsBaseVertex = (CL_GLFunctions::ptr_glDrawElementsBaseVertex) cl_get_proc_address_extension("glDrawElementsBaseVertex");
	functions->drawRangeElementsBaseVertex = (CL_GLFunctions::ptr_glDrawRangeElementsBaseVertex) cl_get_proc_address_extension("glDrawRangeElementsBaseVertex");
	functions->drawElementsInstancedBaseVertex = (CL_GLFunctions::ptr_glDrawElementsInstancedBaseVertex) cl_get_proc_address_extension("glDrawElementsInstancedBaseVertex");
	functions->multiDrawElementsBaseVertex = (CL_GLFunctions::ptr_glMultiDrawElementsBaseVertex) cl_get_proc_address_extension("glMultiDrawElementsBaseVertex");
	functions->provokingVertex = (CL_GLFunctions::ptr_glProvokingVertex) cl_get_proc_address_extension("glProvokingVertex");
	functions->fenceSync = (CL_GLFunctions::ptr_glFenceSync) cl_get_proc_address_extension("glFenceSync");
	functions->isSync = (CL_GLFunctions::ptr_glIsSync) cl_get_proc_address_extension("glIsSync");
	functions->deleteSync = (CL_GLFunctions::ptr_glDeleteSync) cl_get_proc_address_extension("glDeleteSync");
	functions->clientWaitSync = (CL_GLFunctions::ptr_glClientWaitSync) cl_get_proc_address_extension("glClientWaitSync");
	functions->waitSync = (CL_GLFunctions::ptr_glWaitSync) cl_get_proc_address_extension("glWaitSync");
	functions->getInteger64v = (CL_GLFunctions::ptr_glGetInteger64v) cl_get_proc_address_extension("glGetInteger64v");
	functions->getSynciv = (CL_GLFunctions::ptr_glGetSynciv) cl_get_proc_address_extension("glGetSynciv");
	functions->texImage2DMultisample = (CL_GLFunctions::ptr_glTexImage2DMultisample) cl_get_proc_address_extension("glTexImage2DMultisample");
	functions->texImage3DMultisample = (CL_GLFunctions::ptr_glTexImage3DMultisample) cl_get_proc_address_extension("glTexImage3DMultisample");
	functions->getMultisamplefv = (CL_GLFunctions::ptr_glGetMultisamplefv) cl_get_proc_address_extension("glGetMultisamplefv");
	functions->sampleMaski = (CL_GLFunctions::ptr_glSampleMaski) cl_get_proc_address_extension("glSampleMaski");
	functions->blendEquationi = (CL_GLFunctions::ptr_glBlendEquationi) cl_get_proc_address_extension("glBlendEquationi");
	functions->blendEquationSeparatei = (CL_GLFunctions::ptr_glBlendEquationSeparatei) cl_get_proc_address_extension("glBlendEquationSeparatei");
	functions->blendFunci = (CL_GLFunctions::ptr_glBlendFunci) cl_get_proc_address_extension("glBlendFunci");
	functions->blendFuncSeparatei = (CL_GLFunctions::ptr_glBlendFuncSeparatei) cl_get_proc_address_extension("glBlendFuncSeparatei");
	functions->minSampleShading = (CL_GLFunctions::ptr_glMinSampleShading) cl_get_proc_address_extension("glMinSampleShading");
	functions->bindFragDataLocationIndexed = (CL_GLFunctions::ptr_glBindFragDataLocationIndexed) cl_get_proc_address_extension("glBindFragDataLocationIndexed");
	functions->getFragDataIndex = (CL_GLFunctions::ptr_glGetFragDataIndex) cl_get_proc_address_extension("glGetFragDataIndex");
	functions->genSamplers = (CL_GLFunctions::ptr_glGenSamplers) cl_get_proc_address_extension("glGenSamplers");
	functions->deleteSamplers = (CL_GLFunctions::ptr_glDeleteSamplers) cl_get_proc_address_extension("glDeleteSamplers");
	functions->isSampler = (CL_GLFunctions::ptr_glIsSampler) cl_get_proc_address_extension("glIsSampler");
	functions->bindSampler = (CL_GLFunctions::ptr_glBindSampler) cl_get_proc_address_extension("glBindSampler");
	functions->samplerParameteri = (CL_GLFunctions::ptr_glSamplerParameteri) cl_get_proc_address_extension("glSamplerParameteri");
	functions->samplerParameteriv = (CL_GLFunctions::ptr_glSamplerParameteriv) cl_get_proc_address_extension("glSamplerParameteriv");
	functions->samplerParameterf = (CL_GLFunctions::ptr_glSamplerParameterf) cl_get_proc_address_extension("glSamplerParameterf");
	functions->samplerParameterfv = (CL_GLFunctions::ptr_glSamplerParameterfv) cl_get_proc_address_extension("glSamplerParameterfv");
	functions->samplerParameterIiv = (CL_GLFunctions::ptr_glSamplerParameterIiv) cl_get_proc_address_extension("glSamplerParameterIiv");
	functions->samplerParameterIuiv = (CL_GLFunctions::ptr_glSamplerParameterIuiv) cl_get_proc_address_extension("glSamplerParameterIuiv");
	functions->getSamplerParameteriv = (CL_GLFunctions::ptr_glGetSamplerParameteriv) cl_get_proc_address_extension("glGetSamplerParameteriv");
	functions->getSamplerParameterIiv = (CL_GLFunctions::ptr_glGetSamplerParameterIiv) cl_get_proc_address_extension("glGetSamplerParameterIiv");
	functions->getSamplerParameterfv = (CL_GLFunctions::ptr_glGetSamplerParameterfv) cl_get_proc_address_extension("glGetSamplerParameterfv");
	functions->getSamplerParameterIfv = (CL_GLFunctions::ptr_glGetSamplerParameterIfv) cl_get_proc_address_extension("glGetSamplerParameterIfv");
	functions->queryCounter = (CL_GLFunctions::ptr_glQueryCounter) cl_get_proc_address_extension("glQueryCounter");
	functions->getQueryObjecti64v = (CL_GLFunctions::ptr_glGetQueryObjecti64v) cl_get_proc_address_extension("glGetQueryObjecti64v");
	functions->getQueryObjectui64v = (CL_GLFunctions::ptr_glGetQueryObjectui64v) cl_get_proc_address_extension("glGetQueryObjectui64v");
	functions->vertexP2ui = (CL_GLFunctions::ptr_glVertexP2ui) cl_get_proc_address_extension("glVertexP2ui");
	functions->vertexP2uiv = (CL_GLFunctions::ptr_glVertexP2uiv) cl_get_proc_address_extension("glVertexP2uiv");
	functions->vertexP3ui = (CL_GLFunctions::ptr_glVertexP3ui) cl_get_proc_address_extension("glVertexP3ui");
	functions->vertexP3uiv = (CL_GLFunctions::ptr_glVertexP3uiv) cl_get_proc_address_extension("glVertexP3uiv");
	functions->vertexP4ui = (CL_GLFunctions::ptr_glVertexP4ui) cl_get_proc_address_extension("glVertexP4ui");
	functions->vertexP4uiv = (CL_GLFunctions::ptr_glVertexP4uiv) cl_get_proc_address_extension("glVertexP4uiv");
	functions->texCoordP1ui = (CL_GLFunctions::ptr_glTexCoordP1ui) cl_get_proc_address_extension("glTexCoordP1ui");
	functions->texCoordP1uiv = (CL_GLFunctions::ptr_glTexCoordP1uiv) cl_get_proc_address_extension("glTexCoordP1uiv");
	functions->texCoordP2ui = (CL_GLFunctions::ptr_glTexCoordP2ui) cl_get_proc_address_extension("glTexCoordP2ui");
	functions->texCoordP2uiv = (CL_GLFunctions::ptr_glTexCoordP2uiv) cl_get_proc_address_extension("glTexCoordP2uiv");
	functions->texCoordP3ui = (CL_GLFunctions::ptr_glTexCoordP3ui) cl_get_proc_address_extension("glTexCoordP3ui");
	functions->texCoordP3uiv = (CL_GLFunctions::ptr_glTexCoordP3uiv) cl_get_proc_address_extension("glTexCoordP3uiv");
	functions->texCoordP4ui = (CL_GLFunctions::ptr_glTexCoordP4ui) cl_get_proc_address_extension("glTexCoordP4ui");
	functions->texCoordP4uiv = (CL_GLFunctions::ptr_glTexCoordP4uiv) cl_get_proc_address_extension("glTexCoordP4uiv");
	functions->multiTexCoordP1ui = (CL_GLFunctions::ptr_glMultiTexCoordP1ui) cl_get_proc_address_extension("glMultiTexCoordP1ui");
	functions->multiTexCoordP1uiv = (CL_GLFunctions::ptr_glMultiTexCoordP1uiv) cl_get_proc_address_extension("glMultiTexCoordP1uiv");
	functions->multiTexCoordP2ui = (CL_GLFunctions::ptr_glMultiTexCoordP2ui) cl_get_proc_address_extension("glMultiTexCoordP2ui");
	functions->multiTexCoordP2uiv = (CL_GLFunctions::ptr_glMultiTexCoordP2uiv) cl_get_proc_address_extension("glMultiTexCoordP2uiv");
	functions->multiTexCoordP3ui = (CL_GLFunctions::ptr_glMultiTexCoordP3ui) cl_get_proc_address_extension("glMultiTexCoordP3ui");
	functions->multiTexCoordP3uiv = (CL_GLFunctions::ptr_glMultiTexCoordP3uiv) cl_get_proc_address_extension("glMultiTexCoordP3uiv");
	functions->multiTexCoordP4ui = (CL_GLFunctions::ptr_glMultiTexCoordP4ui) cl_get_proc_address_extension("glMultiTexCoordP4ui");
	functions->multiTexCoordP4uiv = (CL_GLFunctions::ptr_glMultiTexCoordP4uiv) cl_get_proc_address_extension("glMultiTexCoordP4uiv");
	functions->normalP3ui = (CL_GLFunctions::ptr_glNormalP3ui) cl_get_proc_address_extension("glNormalP3ui");
	functions->normalP3uiv = (CL_GLFunctions::ptr_glNormalP3uiv) cl_get_proc_address_extension("glNormalP3uiv");
	functions->colorP3ui = (CL_GLFunctions::ptr_glColorP3ui) cl_get_proc_address_extension("glColorP3ui");
	functions->colorP3uiv = (CL_GLFunctions::ptr_glColorP3uiv) cl_get_proc_address_extension("glColorP3uiv");
	functions->colorP4ui = (CL_GLFunctions::ptr_glColorP4ui) cl_get_proc_address_extension("glColorP4ui");
	functions->colorP4uiv = (CL_GLFunctions::ptr_glColorP4uiv) cl_get_proc_address_extension("glColorP4uiv");
	functions->secondaryColorP3ui = (CL_GLFunctions::ptr_glSecondaryColorP3ui) cl_get_proc_address_extension("glSecondaryColorP3ui");
	functions->secondaryColorP3uiv = (CL_GLFunctions::ptr_glSecondaryColorP3uiv) cl_get_proc_address_extension("glSecondaryColorP3uiv");
	functions->vertexAttribP1ui = (CL_GLFunctions::ptr_glVertexAttribP1ui) cl_get_proc_address_extension("glVertexAttribP1ui");
	functions->vertexAttribP1uiv = (CL_GLFunctions::ptr_glVertexAttribP1uiv) cl_get_proc_address_extension("glVertexAttribP1uiv");
	functions->vertexAttribP2ui = (CL_GLFunctions::ptr_glVertexAttribP2ui) cl_get_proc_address_extension("glVertexAttribP2ui");
	functions->vertexAttribP2uiv = (CL_GLFunctions::ptr_glVertexAttribP2uiv) cl_get_proc_address_extension("glVertexAttribP2uiv");
	functions->vertexAttribP3ui = (CL_GLFunctions::ptr_glVertexAttribP3ui) cl_get_proc_address_extension("glVertexAttribP3ui");
	functions->vertexAttribP3uiv = (CL_GLFunctions::ptr_glVertexAttribP3uiv) cl_get_proc_address_extension("glVertexAttribP3uiv");
	functions->vertexAttribP4ui = (CL_GLFunctions::ptr_glVertexAttribP4ui) cl_get_proc_address_extension("glVertexAttribP4ui");
	functions->vertexAttribP4uiv = (CL_GLFunctions::ptr_glVertexAttribP4uiv) cl_get_proc_address_extension("glVertexAttribP4uiv");
	functions->drawArraysIndirect = (CL_GLFunctions::ptr_glDrawArraysIndirect) cl_get_proc_address_extension("glDrawArraysIndirect");
	functions->drawElementsIndirect = (CL_GLFunctions::ptr_glDrawElementsIndirect) cl_get_proc_address_extension("glDrawElementsIndirect");
	functions->uniform1d = (CL_GLFunctions::ptr_glUniform1d) cl_get_proc_address_extension("glUniform1d");
	functions->uniform2d = (CL_GLFunctions::ptr_glUniform2d) cl_get_proc_address_extension("glUniform2d");
	functions->uniform3d = (CL_GLFunctions::ptr_glUniform3d) cl_get_proc_address_extension("glUniform3d");
	functions->uniform4d = (CL_GLFunctions::ptr_glUniform4d) cl_get_proc_address_extension("glUniform4d");
	functions->uniform1dv = (CL_GLFunctions::ptr_glUniform1dv) cl_get_proc_address_extension("glUniform1dv");
	functions->uniform2dv = (CL_GLFunctions::ptr_glUniform2dv) cl_get_proc_address_extension("glUniform2dv");
	functions->uniform3dv = (CL_GLFunctions::ptr_glUniform3dv) cl_get_proc_address_extension("glUniform3dv");
	functions->uniform4dv = (CL_GLFunctions::ptr_glUniform4dv) cl_get_proc_address_extension("glUniform4dv");
	functions->uniformMatrix2dv = (CL_GLFunctions::ptr_glUniformMatrix2dv) cl_get_proc_address_extension("glUniformMatrix2dv");
	functions->uniformMatrix3dv = (CL_GLFunctions::ptr_glUniformMatrix3dv) cl_get_proc_address_extension("glUniformMatrix3dv");
	functions->uniformMatrix4dv = (CL_GLFunctions::ptr_glUniformMatrix4dv) cl_get_proc_address_extension("glUniformMatrix4dv");
	functions->uniformMatrix2x3dv = (CL_GLFunctions::ptr_glUniformMatrix2x3dv) cl_get_proc_address_extension("glUniformMatrix2x3dv");
	functions->uniformMatrix2x4dv = (CL_GLFunctions::ptr_glUniformMatrix2x4dv) cl_get_proc_address_extension("glUniformMatrix2x4dv");
	functions->uniformMatrix3x2dv = (CL_GLFunctions::ptr_glUniformMatrix3x2dv) cl_get_proc_address_extension("glUniformMatrix3x2dv");
	functions->uniformMatrix3x4dv = (CL_GLFunctions::ptr_glUniformMatrix3x4dv) cl_get_proc_address_extension("glUniformMatrix3x4dv");
	functions->uniformMatrix4x2dv = (CL_GLFunctions::ptr_glUniformMatrix4x2dv) cl_get_proc_address_extension("glUniformMatrix4x2dv");
	functions->uniformMatrix4x3dv = (CL_GLFunctions::ptr_glUniformMatrix4x3dv) cl_get_proc_address_extension("glUniformMatrix4x3dv");
	functions->getUniformdv = (CL_GLFunctions::ptr_glGetUniformdv) cl_get_proc_address_extension("glGetUniformdv");
	functions->programUniform1dEXT = (CL_GLFunctions::ptr_glProgramUniform1dEXT) cl_get_proc_address_extension("glProgramUniform1dEXT");
	functions->programUniform2dEXT = (CL_GLFunctions::ptr_glProgramUniform2dEXT) cl_get_proc_address_extension("glProgramUniform2dEXT");
	functions->programUniform3dEXT = (CL_GLFunctions::ptr_glProgramUniform3dEXT) cl_get_proc_address_extension("glProgramUniform3dEXT");
	functions->programUniform4dEXT = (CL_GLFunctions::ptr_glProgramUniform4dEXT) cl_get_proc_address_extension("glProgramUniform4dEXT");
	functions->programUniform1dvEXT = (CL_GLFunctions::ptr_glProgramUniform1dvEXT) cl_get_proc_address_extension("glProgramUniform1dvEXT");
	functions->programUniform2dvEXT = (CL_GLFunctions::ptr_glProgramUniform2dvEXT) cl_get_proc_address_extension("glProgramUniform2dvEXT");
	functions->programUniform3dvEXT = (CL_GLFunctions::ptr_glProgramUniform3dvEXT) cl_get_proc_address_extension("glProgramUniform3dvEXT");
	functions->programUniform4dvEXT = (CL_GLFunctions::ptr_glProgramUniform4dvEXT) cl_get_proc_address_extension("glProgramUniform4dvEXT");
	functions->programUniformMatrix2dvEXT = (CL_GLFunctions::ptr_glProgramUniformMatrix2dvEXT) cl_get_proc_address_extension("glProgramUniformMatrix2dvEXT");
	functions->programUniformMatrix3dvEXT = (CL_GLFunctions::ptr_glProgramUniformMatrix3dvEXT) cl_get_proc_address_extension("glProgramUniformMatrix3dvEXT");
	functions->programUniformMatrix4dvEXT = (CL_GLFunctions::ptr_glProgramUniformMatrix4dvEXT) cl_get_proc_address_extension("glProgramUniformMatrix4dvEXT");
	functions->programUniformMatrix2x3dvEXT = (CL_GLFunctions::ptr_glProgramUniformMatrix2x3dvEXT) cl_get_proc_address_extension("glProgramUniformMatrix2x3dvEXT");
	functions->programUniformMatrix2x4dvEXT = (CL_GLFunctions::ptr_glProgramUniformMatrix2x4dvEXT) cl_get_proc_address_extension("glProgramUniformMatrix2x4dvEXT");
	functions->programUniformMatrix3x2dvEXT = (CL_GLFunctions::ptr_glProgramUniformMatrix3x2dvEXT) cl_get_proc_address_extension("glProgramUniformMatrix3x2dvEXT");
	functions->programUniformMatrix3x4dvEXT = (CL_GLFunctions::ptr_glProgramUniformMatrix3x4dvEXT) cl_get_proc_address_extension("glProgramUniformMatrix3x4dvEXT");
	functions->programUniformMatrix4x2dvEXT = (CL_GLFunctions::ptr_glProgramUniformMatrix4x2dvEXT) cl_get_proc_address_extension("glProgramUniformMatrix4x2dvEXT");
	functions->programUniformMatrix4x3dvEXT = (CL_GLFunctions::ptr_glProgramUniformMatrix4x3dvEXT) cl_get_proc_address_extension("glProgramUniformMatrix4x3dvEXT");
	functions->getSubroutineUniformLocation = (CL_GLFunctions::ptr_glGetSubroutineUniformLocation) cl_get_proc_address_extension("glGetSubroutineUniformLocation");
	functions->getSubroutineIndex = (CL_GLFunctions::ptr_glGetSubroutineIndex) cl_get_proc_address_extension("glGetSubroutineIndex");
	functions->getActiveSubroutineUniformiv = (CL_GLFunctions::ptr_glGetActiveSubroutineUniformiv) cl_get_proc_address_extension("glGetActiveSubroutineUniformiv");
	functions->getActiveSubroutineUniformName = (CL_GLFunctions::ptr_glGetActiveSubroutineUniformName) cl_get_proc_address_extension("glGetActiveSubroutineUniformName");
	functions->getActiveSubroutineName = (CL_GLFunctions::ptr_glGetActiveSubroutineName) cl_get_proc_address_extension("glGetActiveSubroutineName");
	functions->uniformSubroutinesuiv = (CL_GLFunctions::ptr_glUniformSubroutinesuiv) cl_get_proc_address_extension("glUniformSubroutinesuiv");
	functions->getUniformSubroutineuiv = (CL_GLFunctions::ptr_glGetUniformSubroutineuiv) cl_get_proc_address_extension("glGetUniformSubroutineuiv");
	functions->getProgramStageiv = (CL_GLFunctions::ptr_glGetProgramStageiv) cl_get_proc_address_extension("glGetProgramStageiv");
	functions->patchParameteri = (CL_GLFunctions::ptr_glPatchParameteri) cl_get_proc_address_extension("glPatchParameteri");
	functions->patchParameterfv = (CL_GLFunctions::ptr_glPatchParameterfv) cl_get_proc_address_extension("glPatchParameterfv");
	functions->bindTransformFeedback = (CL_GLFunctions::ptr_glBindTransformFeedback) cl_get_proc_address_extension("glBindTransformFeedback");
	functions->deleteTransformFeedbacks = (CL_GLFunctions::ptr_glDeleteTransformFeedbacks) cl_get_proc_address_extension("glDeleteTransformFeedbacks");
	functions->genTransformFeedbacks = (CL_GLFunctions::ptr_glGenTransformFeedbacks) cl_get_proc_address_extension("glGenTransformFeedbacks");
	functions->isTransformFeedback = (CL_GLFunctions::ptr_glIsTransformFeedback) cl_get_proc_address_extension("glIsTransformFeedback");
	functions->pauseTransformFeedback = (CL_GLFunctions::ptr_glPauseTransformFeedback) cl_get_proc_address_extension("glPauseTransformFeedback");
	functions->resumeTransformFeedback = (CL_GLFunctions::ptr_glResumeTransformFeedback) cl_get_proc_address_extension("glResumeTransformFeedback");
	functions->drawTransformFeedback = (CL_GLFunctions::ptr_glDrawTransformFeedback) cl_get_proc_address_extension("glDrawTransformFeedback");
	functions->drawTransformFeedbackStream = (CL_GLFunctions::ptr_glDrawTransformFeedbackStream) cl_get_proc_address_extension("glDrawTransformFeedbackStream");
	functions->beginQueryIndexed = (CL_GLFunctions::ptr_glBeginQueryIndexed) cl_get_proc_address_extension("glBeginQueryIndexed");
	functions->endQueryIndexed = (CL_GLFunctions::ptr_glEndQueryIndexed) cl_get_proc_address_extension("glEndQueryIndexed");
	functions->getQueryIndexediv = (CL_GLFunctions::ptr_glGetQueryIndexediv) cl_get_proc_address_extension("glGetQueryIndexediv");

#ifdef WIN32	// Only win32 static links opengl
	if (!functions->bindTexture) functions->bindTexture = (CL_GLFunctions::ptr_glBindTexture) &glBindTexture;
	if (!functions->blendFunc) functions->blendFunc = (CL_GLFunctions::ptr_glBlendFunc) &glBlendFunc;
	if (!functions->clear) functions->clear = (CL_GLFunctions::ptr_glClear) &glClear;
	if (!functions->clearColor) functions->clearColor = (CL_GLFunctions::ptr_glClearColor) &glClearColor;
	if (!functions->clearDepth) functions->clearDepth = (CL_GLFunctions::ptr_glClearDepth) &glClearDepth;
	if (!functions->clearStencil) functions->clearStencil = (CL_GLFunctions::ptr_glClearStencil) &glClearStencil;
	if (!functions->colorMask) functions->colorMask = (CL_GLFunctions::ptr_glColorMask) &glColorMask;
	if (!functions->copyTexImage1D) functions->copyTexImage1D = (CL_GLFunctions::ptr_glCopyTexImage1D) &glCopyTexImage1D;
	if (!functions->copyTexImage2D) functions->copyTexImage2D = (CL_GLFunctions::ptr_glCopyTexImage2D) &glCopyTexImage2D;
	if (!functions->copyTexSubImage1D) functions->copyTexSubImage1D = (CL_GLFunctions::ptr_glCopyTexSubImage1D) &glCopyTexSubImage1D;
	if (!functions->copyTexSubImage2D) functions->copyTexSubImage2D = (CL_GLFunctions::ptr_glCopyTexSubImage2D) &glCopyTexSubImage2D;
	if (!functions->cullFace) functions->cullFace = (CL_GLFunctions::ptr_glCullFace) &glCullFace;
	if (!functions->deleteTextures) functions->deleteTextures = (CL_GLFunctions::ptr_glDeleteTextures) &glDeleteTextures;
	if (!functions->depthFunc) functions->depthFunc = (CL_GLFunctions::ptr_glDepthFunc) &glDepthFunc;
	if (!functions->depthMask) functions->depthMask = (CL_GLFunctions::ptr_glDepthMask) &glDepthMask;
	if (!functions->depthRange) functions->depthRange = (CL_GLFunctions::ptr_glDepthRange) &glDepthRange;
	if (!functions->disable) functions->disable = (CL_GLFunctions::ptr_glDisable) &glDisable;
	if (!functions->drawArrays) functions->drawArrays = (CL_GLFunctions::ptr_glDrawArrays) &glDrawArrays;
	if (!functions->drawBuffer) functions->drawBuffer = (CL_GLFunctions::ptr_glDrawBuffer) &glDrawBuffer;
	if (!functions->drawElements) functions->drawElements = (CL_GLFunctions::ptr_glDrawElements) &glDrawElements;
	if (!functions->enable) functions->enable = (CL_GLFunctions::ptr_glEnable) &glEnable;
	if (!functions->finish) functions->finish = (CL_GLFunctions::ptr_glFinish) &glFinish;
	if (!functions->flush) functions->flush = (CL_GLFunctions::ptr_glFlush) &glFlush;
	if (!functions->frontFace) functions->frontFace = (CL_GLFunctions::ptr_glFrontFace) &glFrontFace;
	if (!functions->genTextures) functions->genTextures = (CL_GLFunctions::ptr_glGenTextures) &glGenTextures;
	if (!functions->getBooleanv) functions->getBooleanv = (CL_GLFunctions::ptr_glGetBooleanv) &glGetBooleanv;
	if (!functions->getDoublev) functions->getDoublev = (CL_GLFunctions::ptr_glGetDoublev) &glGetDoublev;
	if (!functions->getError) functions->getError = (CL_GLFunctions::ptr_glGetError) &glGetError;
	if (!functions->getFloatv) functions->getFloatv = (CL_GLFunctions::ptr_glGetFloatv) &glGetFloatv;
	if (!functions->getIntegerv) functions->getIntegerv = (CL_GLFunctions::ptr_glGetIntegerv) &glGetIntegerv;
	if (!functions->getPointerv) functions->getPointerv = (CL_GLFunctions::ptr_glGetPointerv) &glGetPointerv;
	if (!functions->getString) functions->getString = (CL_GLFunctions::ptr_glGetString) &glGetString;
	if (!functions->getTexImage) functions->getTexImage = (CL_GLFunctions::ptr_glGetTexImage) &glGetTexImage;
	if (!functions->getTexLevelParameterfv) functions->getTexLevelParameterfv = (CL_GLFunctions::ptr_glGetTexLevelParameterfv) &glGetTexLevelParameterfv;
	if (!functions->getTexLevelParameteriv) functions->getTexLevelParameteriv = (CL_GLFunctions::ptr_glGetTexLevelParameteriv) &glGetTexLevelParameteriv;
	if (!functions->getTexParameterfv) functions->getTexParameterfv = (CL_GLFunctions::ptr_glGetTexParameterfv) &glGetTexParameterfv;
	if (!functions->getTexParameteriv) functions->getTexParameteriv = (CL_GLFunctions::ptr_glGetTexParameteriv) &glGetTexParameteriv;
	if (!functions->hint) functions->hint = (CL_GLFunctions::ptr_glHint) &glHint;
	if (!functions->isEnabled) functions->isEnabled = (CL_GLFunctions::ptr_glIsEnabled) &glIsEnabled;
	if (!functions->isTexture) functions->isTexture = (CL_GLFunctions::ptr_glIsTexture) &glIsTexture;
	if (!functions->lineWidth) functions->lineWidth = (CL_GLFunctions::ptr_glLineWidth) &glLineWidth;
	if (!functions->logicOp) functions->logicOp = (CL_GLFunctions::ptr_glLogicOp) &glLogicOp;
	if (!functions->pixelStoref) functions->pixelStoref = (CL_GLFunctions::ptr_glPixelStoref) &glPixelStoref;
	if (!functions->pixelStorei) functions->pixelStorei = (CL_GLFunctions::ptr_glPixelStorei) &glPixelStorei;
	if (!functions->pointSize) functions->pointSize = (CL_GLFunctions::ptr_glPointSize) &glPointSize;
	if (!functions->polygonMode) functions->polygonMode = (CL_GLFunctions::ptr_glPolygonMode) &glPolygonMode;
	if (!functions->polygonOffset) functions->polygonOffset = (CL_GLFunctions::ptr_glPolygonOffset) &glPolygonOffset;
	if (!functions->readBuffer) functions->readBuffer = (CL_GLFunctions::ptr_glReadBuffer) &glReadBuffer;
	if (!functions->readPixels) functions->readPixels = (CL_GLFunctions::ptr_glReadPixels) &glReadPixels;
	if (!functions->scissor) functions->scissor = (CL_GLFunctions::ptr_glScissor) &glScissor;
	if (!functions->stencilFunc) functions->stencilFunc = (CL_GLFunctions::ptr_glStencilFunc) &glStencilFunc;
	if (!functions->stencilMask) functions->stencilMask = (CL_GLFunctions::ptr_glStencilMask) &glStencilMask;
	if (!functions->stencilOp) functions->stencilOp = (CL_GLFunctions::ptr_glStencilOp) &glStencilOp;
	if (!functions->texImage1D) functions->texImage1D = (CL_GLFunctions::ptr_glTexImage1D) &glTexImage1D;
	if (!functions->texImage2D) functions->texImage2D = (CL_GLFunctions::ptr_glTexImage2D) &glTexImage2D;
	if (!functions->texParameterf) functions->texParameterf = (CL_GLFunctions::ptr_glTexParameterf) &glTexParameterf;
	if (!functions->texParameterfv) functions->texParameterfv = (CL_GLFunctions::ptr_glTexParameterfv) &glTexParameterfv;
	if (!functions->texParameteri) functions->texParameteri = (CL_GLFunctions::ptr_glTexParameteri) &glTexParameteri;
	if (!functions->texParameteriv) functions->texParameteriv = (CL_GLFunctions::ptr_glTexParameteriv) &glTexParameteriv;
	if (!functions->texSubImage1D) functions->texSubImage1D = (CL_GLFunctions::ptr_glTexSubImage1D) &glTexSubImage1D;
	if (!functions->texSubImage2D) functions->texSubImage2D = (CL_GLFunctions::ptr_glTexSubImage2D) &glTexSubImage2D;
	if (!functions->viewport) functions->viewport = (CL_GLFunctions::ptr_glViewport) &glViewport;

#endif

#ifdef __APPLE__
	// *** NOTE - Some of these should be commented out, as they are legacy if (!functions) functions that are not in the OpenGL 3 spec

	if (!functions->drawRangeElements) functions->drawRangeElements = (CL_GLFunctions::ptr_glDrawRangeElementsEXT) &glDrawRangeElements;
	if (!functions->texImage3D) functions->texImage3D = (CL_GLFunctions::ptr_glTexImage3DEXT) &glTexImage3D;
	if (!functions->texSubImage3D) functions->texSubImage3D = (CL_GLFunctions::ptr_glTexSubImage3DEXT) &glTexSubImage3D;
	if (!functions->copyTexSubImage3D) functions->copyTexSubImage3D = (CL_GLFunctions::ptr_glCopyTexSubImage3DEXT) &glCopyTexSubImage3D;
	if (!functions->colorTable) functions->colorTable = (CL_GLFunctions::ptr_glColorTableSGI) &glColorTable;
	if (!functions->copyColorTable) functions->copyColorTable = (CL_GLFunctions::ptr_glCopyColorTableSGI) &glCopyColorTable;
	if (!functions->colorTableParameteriv) functions->colorTableParameteriv = (CL_GLFunctions::ptr_glColorTableParameterivSGI) &glColorTableParameteriv;
	if (!functions->colorTableParameterfv) functions->colorTableParameterfv = (CL_GLFunctions::ptr_glColorTableParameterfvSGI) &glColorTableParameterfv;
	if (!functions->getColorTable) functions->getColorTable = (CL_GLFunctions::ptr_glGetColorTableSGI) &glGetColorTable;
	if (!functions->getColorTableParameteriv) functions->getColorTableParameteriv = (CL_GLFunctions::ptr_glGetColorTableParameterivSGI) &glGetColorTableParameteriv;
	if (!functions->getColorTableParameterfv) functions->getColorTableParameterfv = (CL_GLFunctions::ptr_glGetColorTableParameterfvSGI) &glGetColorTableParameterfv;
	if (!functions->colorSubTable) functions->colorSubTable = (CL_GLFunctions::ptr_glColorSubTableEXT) &glColorSubTable;
	if (!functions->copyColorSubTable) functions->copyColorSubTable = (CL_GLFunctions::ptr_glCopyColorSubTableEXT) &glCopyColorSubTable;
	if (!functions->convolutionFilter1D) functions->convolutionFilter1D = (CL_GLFunctions::ptr_glConvolutionFilter1DEXT) &glConvolutionFilter1D;
	if (!functions->convolutionFilter2D) functions->convolutionFilter2D = (CL_GLFunctions::ptr_glConvolutionFilter2DEXT) &glConvolutionFilter2D;
	if (!functions->copyConvolutionFilter1D) functions->copyConvolutionFilter1D = (CL_GLFunctions::ptr_glCopyConvolutionFilter1DEXT) &glCopyConvolutionFilter1D;
	if (!functions->copyConvolutionFilter2D) functions->copyConvolutionFilter2D = (CL_GLFunctions::ptr_glCopyConvolutionFilter2DEXT) &glCopyConvolutionFilter2D;
	if (!functions->getConvolutionFilter) functions->getConvolutionFilter = (CL_GLFunctions::ptr_glGetConvolutionFilterEXT) &glGetConvolutionFilter;
	if (!functions->separableFilter2D) functions->separableFilter2D = (CL_GLFunctions::ptr_glSeparableFilter2DEXT) &glSeparableFilter2D;
	if (!functions->getSeparableFilter) functions->getSeparableFilter = (CL_GLFunctions::ptr_glGetSeparableFilterEXT) &glGetSeparableFilter;
	if (!functions->convolutionParameteri) functions->convolutionParameteri = (CL_GLFunctions::ptr_glConvolutionParameteriEXT) &glConvolutionParameteri;
	if (!functions->convolutionParameteriv) functions->convolutionParameteriv = (CL_GLFunctions::ptr_glConvolutionParameterivEXT) &glConvolutionParameteriv;
	if (!functions->convolutionParameterf) functions->convolutionParameterf = (CL_GLFunctions::ptr_glConvolutionParameterfEXT) &glConvolutionParameterf;
	if (!functions->convolutionParameterfv) functions->convolutionParameterfv = (CL_GLFunctions::ptr_glConvolutionParameterfvEXT) &glConvolutionParameterfv;
	if (!functions->getConvolutionParameteriv) functions->getConvolutionParameteriv = (CL_GLFunctions::ptr_glGetConvolutionParameterivEXT) &glGetConvolutionParameteriv;
	if (!functions->getConvolutionParameterfv) functions->getConvolutionParameterfv = (CL_GLFunctions::ptr_glGetConvolutionParameterfvEXT) &glGetConvolutionParameterfv;
	if (!functions->histogram) functions->histogram = (CL_GLFunctions::ptr_glHistogramEXT) &glHistogram;
	if (!functions->resetHistogram) functions->resetHistogram = (CL_GLFunctions::ptr_glResetHistogramEXT) &glResetHistogram;
	if (!functions->getHistogram) functions->getHistogram = (CL_GLFunctions::ptr_glGetHistogramEXT) &glGetHistogram;
	if (!functions->getHistogramParameteriv) functions->getHistogramParameteriv = (CL_GLFunctions::ptr_glGetHistogramParameterivEXT) &glGetHistogramParameteriv;
	if (!functions->getHistogramParameterfv) functions->getHistogramParameterfv = (CL_GLFunctions::ptr_glGetHistogramParameterfvEXT) &glGetHistogramParameterfv;
	if (!functions->minmax) functions->minmax = (CL_GLFunctions::ptr_glMinmaxEXT) &glMinmax;
	if (!functions->resetMinmax) functions->resetMinmax = (CL_GLFunctions::ptr_glResetMinmaxEXT) &glResetMinmax;
	if (!functions->getMinmax) functions->getMinmax = (CL_GLFunctions::ptr_glGetMinmaxEXT) &glGetMinmax;
	if (!functions->getMinmaxParameteriv) functions->getMinmaxParameteriv = (CL_GLFunctions::ptr_glGetMinmaxParameterivEXT) &glGetMinmaxParameteriv;
	if (!functions->getMinmaxParameterfv) functions->getMinmaxParameterfv = (CL_GLFunctions::ptr_glGetMinmaxParameterfvEXT) &glGetMinmaxParameterfv;
	if (!functions->blendColor) functions->blendColor = (CL_GLFunctions::ptr_glBlendColorEXT) &glBlendColor;
	if (!functions->blendEquation) functions->blendEquation = (CL_GLFunctions::ptr_glBlendEquationEXT) &glBlendEquation;
	if (!functions->activeTexture) functions->activeTexture = (CL_GLFunctions::ptr_glActiveTextureARB) &glActiveTexture;
	if (!functions->clientActiveTexture) functions->clientActiveTexture = (CL_GLFunctions::ptr_glClientActiveTextureARB) &glClientActiveTexture;
	if (!functions->multiTexCoord1d) functions->multiTexCoord1d = (CL_GLFunctions::ptr_glMultiTexCoord1dARB) &glMultiTexCoord1d;
	if (!functions->multiTexCoord1dv) functions->multiTexCoord1dv = (CL_GLFunctions::ptr_glMultiTexCoord1dvARB) &glMultiTexCoord1dv;
	if (!functions->multiTexCoord1f) functions->multiTexCoord1f = (CL_GLFunctions::ptr_glMultiTexCoord1fARB) &glMultiTexCoord1f;
	if (!functions->multiTexCoord1fv) functions->multiTexCoord1fv = (CL_GLFunctions::ptr_glMultiTexCoord1fvARB) &glMultiTexCoord1fv;
	if (!functions->multiTexCoord1i) functions->multiTexCoord1i = (CL_GLFunctions::ptr_glMultiTexCoord1iARB) &glMultiTexCoord1i;
	if (!functions->multiTexCoord1iv) functions->multiTexCoord1iv = (CL_GLFunctions::ptr_glMultiTexCoord1ivARB) &glMultiTexCoord1iv;
	if (!functions->multiTexCoord1s) functions->multiTexCoord1s = (CL_GLFunctions::ptr_glMultiTexCoord1sARB) &glMultiTexCoord1s;
	if (!functions->multiTexCoord1sv) functions->multiTexCoord1sv = (CL_GLFunctions::ptr_glMultiTexCoord1svARB) &glMultiTexCoord1sv;
	if (!functions->multiTexCoord2d) functions->multiTexCoord2d = (CL_GLFunctions::ptr_glMultiTexCoord2dARB) &glMultiTexCoord2d;
	if (!functions->multiTexCoord2dv) functions->multiTexCoord2dv = (CL_GLFunctions::ptr_glMultiTexCoord2dvARB) &glMultiTexCoord2dv;
	if (!functions->multiTexCoord2f) functions->multiTexCoord2f = (CL_GLFunctions::ptr_glMultiTexCoord2fARB) &glMultiTexCoord2f;
	if (!functions->multiTexCoord2fv) functions->multiTexCoord2fv = (CL_GLFunctions::ptr_glMultiTexCoord2fvARB) &glMultiTexCoord2fv;
	if (!functions->multiTexCoord2i) functions->multiTexCoord2i = (CL_GLFunctions::ptr_glMultiTexCoord2iARB) &glMultiTexCoord2i;
	if (!functions->multiTexCoord2iv) functions->multiTexCoord2iv = (CL_GLFunctions::ptr_glMultiTexCoord2ivARB) &glMultiTexCoord2iv;
	if (!functions->multiTexCoord2s) functions->multiTexCoord2s = (CL_GLFunctions::ptr_glMultiTexCoord2sARB) &glMultiTexCoord2s;
	if (!functions->multiTexCoord2sv) functions->multiTexCoord2sv = (CL_GLFunctions::ptr_glMultiTexCoord2svARB) &glMultiTexCoord2sv;
	if (!functions->multiTexCoord3d) functions->multiTexCoord3d = (CL_GLFunctions::ptr_glMultiTexCoord3dARB) &glMultiTexCoord3d;
	if (!functions->multiTexCoord3dv) functions->multiTexCoord3dv = (CL_GLFunctions::ptr_glMultiTexCoord3dvARB) &glMultiTexCoord3dv;
	if (!functions->multiTexCoord3f) functions->multiTexCoord3f = (CL_GLFunctions::ptr_glMultiTexCoord3fARB) &glMultiTexCoord3f;
	if (!functions->multiTexCoord3fv) functions->multiTexCoord3fv = (CL_GLFunctions::ptr_glMultiTexCoord3fvARB) &glMultiTexCoord3fv;
	if (!functions->multiTexCoord3i) functions->multiTexCoord3i = (CL_GLFunctions::ptr_glMultiTexCoord3iARB) &glMultiTexCoord3i;
	if (!functions->multiTexCoord3iv) functions->multiTexCoord3iv = (CL_GLFunctions::ptr_glMultiTexCoord3ivARB) &glMultiTexCoord3iv;
	if (!functions->multiTexCoord3s) functions->multiTexCoord3s = (CL_GLFunctions::ptr_glMultiTexCoord3sARB) &glMultiTexCoord3s;
	if (!functions->multiTexCoord3sv) functions->multiTexCoord3sv = (CL_GLFunctions::ptr_glMultiTexCoord3svARB) &glMultiTexCoord3sv;
	if (!functions->multiTexCoord4d) functions->multiTexCoord4d = (CL_GLFunctions::ptr_glMultiTexCoord4dARB) &glMultiTexCoord4d;
	if (!functions->multiTexCoord4dv) functions->multiTexCoord4dv = (CL_GLFunctions::ptr_glMultiTexCoord4dvARB) &glMultiTexCoord4dv;
	if (!functions->multiTexCoord4f) functions->multiTexCoord4f = (CL_GLFunctions::ptr_glMultiTexCoord4fARB) &glMultiTexCoord4f;
	if (!functions->multiTexCoord4fv) functions->multiTexCoord4fv = (CL_GLFunctions::ptr_glMultiTexCoord4fvARB) &glMultiTexCoord4fv;
	if (!functions->multiTexCoord4i) functions->multiTexCoord4i = (CL_GLFunctions::ptr_glMultiTexCoord4iARB) &glMultiTexCoord4i;
	if (!functions->multiTexCoord4iv) functions->multiTexCoord4iv = (CL_GLFunctions::ptr_glMultiTexCoord4ivARB) &glMultiTexCoord4iv;
	if (!functions->multiTexCoord4s) functions->multiTexCoord4s = (CL_GLFunctions::ptr_glMultiTexCoord4sARB) &glMultiTexCoord4s;
	if (!functions->multiTexCoord4sv) functions->multiTexCoord4sv = (CL_GLFunctions::ptr_glMultiTexCoord4svARB) &glMultiTexCoord4sv;
	if (!functions->compressedTexImage1D) functions->compressedTexImage1D = (CL_GLFunctions::ptr_glCompressedTexImage1DARB) &glCompressedTexImage1D;
	if (!functions->compressedTexImage2D) functions->compressedTexImage2D = (CL_GLFunctions::ptr_glCompressedTexImage2DARB) &glCompressedTexImage2D;
	if (!functions->compressedTexImage3D) functions->compressedTexImage3D = (CL_GLFunctions::ptr_glCompressedTexImage3DARB) &glCompressedTexImage3D;
	if (!functions->compressedTexSubImage1D) functions->compressedTexSubImage1D = (CL_GLFunctions::ptr_glCompressedTexSubImage1DARB) &glCompressedTexSubImage1D;
	if (!functions->compressedTexSubImage2D) functions->compressedTexSubImage2D = (CL_GLFunctions::ptr_glCompressedTexSubImage2DARB) &glCompressedTexSubImage2D;
	if (!functions->compressedTexSubImage3D) functions->compressedTexSubImage3D = (CL_GLFunctions::ptr_glCompressedTexSubImage3DARB) &glCompressedTexSubImage3D;
	if (!functions->getCompressedTexImage) functions->getCompressedTexImage = (CL_GLFunctions::ptr_glGetCompressedTexImageARB) &glGetCompressedTexImage;
	if (!functions->sampleCoverage) functions->sampleCoverage = (CL_GLFunctions::ptr_glSampleCoverageARB) &glSampleCoverage;
	if (!functions->glLoadTransposeMatrixd) functions->glLoadTransposeMatrixd = (CL_GLFunctions::ptr_glLoadTransposeMatrixdARB) &glLoadTransposeMatrixd;
	if (!functions->glLoadTransposeMatrixf) functions->glLoadTransposeMatrixf = (CL_GLFunctions::ptr_glLoadTransposeMatrixfARB) &glLoadTransposeMatrixf;
	if (!functions->glMultTransposeMatrixd) functions->glMultTransposeMatrixd = (CL_GLFunctions::ptr_glMultTransposeMatrixdARB) &glMultTransposeMatrixd;
	if (!functions->glMultTransposeMatrixf) functions->glMultTransposeMatrixf = (CL_GLFunctions::ptr_glMultTransposeMatrixfARB) &glMultTransposeMatrixf;
	if (!functions->fogCoordd) functions->fogCoordd = (CL_GLFunctions::ptr_glFogCoorddEXT) &glFogCoordd;
	if (!functions->fogCoorddv) functions->fogCoorddv = (CL_GLFunctions::ptr_glFogCoorddvEXT) &glFogCoorddv;
	if (!functions->fogCoordf) functions->fogCoordf = (CL_GLFunctions::ptr_glFogCoordfEXT) &glFogCoordf;
	if (!functions->fogCoordfv) functions->fogCoordfv = (CL_GLFunctions::ptr_glFogCoordfvEXT) &glFogCoordfv;
	if (!functions->fogCoordPointer) functions->fogCoordPointer = (CL_GLFunctions::ptr_glFogCoordPointerEXT) &glFogCoordPointer;
	if (!functions->multiDrawArrays) functions->multiDrawArrays = (CL_GLFunctions::ptr_glMultiDrawArraysEXT) &glMultiDrawArrays;
	if (!functions->multiDrawElementsEXT) functions->multiDrawElementsEXT = (CL_GLFunctions::ptr_glMultiDrawElementsEXT) &glMultiDrawElements;
	if (!functions->pointParameterf) functions->pointParameterf = (CL_GLFunctions::ptr_glPointParameterfARB) &glPointParameterf;
	if (!functions->pointParameterfv) functions->pointParameterfv = (CL_GLFunctions::ptr_glPointParameterfvARB) &glPointParameterfv;
	if (!functions->secondaryColor3b) functions->secondaryColor3b = (CL_GLFunctions::ptr_glSecondaryColor3bEXT) &glSecondaryColor3b;
	if (!functions->secondaryColor3bv) functions->secondaryColor3bv = (CL_GLFunctions::ptr_glSecondaryColor3bvEXT) &glSecondaryColor3bv;
	if (!functions->secondaryColor3d) functions->secondaryColor3d = (CL_GLFunctions::ptr_glSecondaryColor3dEXT) &glSecondaryColor3d;
	if (!functions->secondaryColor3dv) functions->secondaryColor3dv = (CL_GLFunctions::ptr_glSecondaryColor3dvEXT) &glSecondaryColor3dv;
	if (!functions->secondaryColor3f) functions->secondaryColor3f = (CL_GLFunctions::ptr_glSecondaryColor3fEXT) &glSecondaryColor3f;
	if (!functions->secondaryColor3fv) functions->secondaryColor3fv = (CL_GLFunctions::ptr_glSecondaryColor3fvEXT) &glSecondaryColor3fv;
	if (!functions->secondaryColor3i) functions->secondaryColor3i = (CL_GLFunctions::ptr_glSecondaryColor3iEXT) &glSecondaryColor3i;
	if (!functions->secondaryColor3iv) functions->secondaryColor3iv = (CL_GLFunctions::ptr_glSecondaryColor3ivEXT) &glSecondaryColor3iv;
	if (!functions->secondaryColor3s) functions->secondaryColor3s = (CL_GLFunctions::ptr_glSecondaryColor3sEXT) &glSecondaryColor3s;
	if (!functions->secondaryColor3sv) functions->secondaryColor3sv = (CL_GLFunctions::ptr_glSecondaryColor3svEXT) &glSecondaryColor3sv;
	if (!functions->secondaryColor3ub) functions->secondaryColor3ub = (CL_GLFunctions::ptr_glSecondaryColor3ubEXT) &glSecondaryColor3ub;
	if (!functions->secondaryColor3ubv) functions->secondaryColor3ubv = (CL_GLFunctions::ptr_glSecondaryColor3ubvEXT) &glSecondaryColor3ubv;
	if (!functions->secondaryColor3ui) functions->secondaryColor3ui = (CL_GLFunctions::ptr_glSecondaryColor3uiEXT) &glSecondaryColor3ui;
	if (!functions->secondaryColor3uiv) functions->secondaryColor3uiv = (CL_GLFunctions::ptr_glSecondaryColor3uivEXT) &glSecondaryColor3uiv;
	if (!functions->secondaryColor3us) functions->secondaryColor3us = (CL_GLFunctions::ptr_glSecondaryColor3usEXT) &glSecondaryColor3us;
	if (!functions->secondaryColor3usv) functions->secondaryColor3usv = (CL_GLFunctions::ptr_glSecondaryColor3usvEXT) &glSecondaryColor3usv;
	if (!functions->secondaryColorPointer) functions->secondaryColorPointer = (CL_GLFunctions::ptr_glSecondaryColorPointerEXT) &glSecondaryColorPointer;
	if (!functions->blendFuncSeparate) functions->blendFuncSeparate = (CL_GLFunctions::ptr_glBlendFuncSeparateEXT) &glBlendFuncSeparate;
	if (!functions->windowPos2d) functions->windowPos2d = &glWindowPos2d;
	if (!functions->windowPos2dv) functions->windowPos2dv = &glWindowPos2dv;
	if (!functions->windowPos2f) functions->windowPos2f = &glWindowPos2f;
	if (!functions->windowPos2fv) functions->windowPos2fv = &glWindowPos2fv;
	if (!functions->windowPos2i) functions->windowPos2i = &glWindowPos2i;
	if (!functions->windowPos2iv) functions->windowPos2iv = &glWindowPos2iv;
	if (!functions->windowPos2s) functions->windowPos2s = &glWindowPos2s;
	if (!functions->windowPos2sv) functions->windowPos2sv = &glWindowPos2sv;
	if (!functions->windowPos3d) functions->windowPos3d = &glWindowPos3d;
	if (!functions->windowPos3dv) functions->windowPos3dv = &glWindowPos3dv;
	if (!functions->windowPos3f) functions->windowPos3f = &glWindowPos3f;
	if (!functions->windowPos3fv) functions->windowPos3fv = &glWindowPos3fv;
	if (!functions->windowPos3i) functions->windowPos3i = &glWindowPos3i;
	if (!functions->windowPos3iv) functions->windowPos3iv = &glWindowPos3iv;
	if (!functions->windowPos3s) functions->windowPos3s = &glWindowPos3s;
	if (!functions->windowPos3sv) functions->windowPos3sv = &glWindowPos3sv;
#endif

	return functions;
}


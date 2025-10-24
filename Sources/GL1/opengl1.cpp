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

#include "GL1/precomp.h"
#include "API/GL1/opengl1.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/mutex.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/texture.h"
#include "API/Display/TargetProviders/render_window_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_graphic_context_provider.h"
#include <map>

#ifdef __APPLE__
#include <AGL/agl.h>
#include <OpenGLES/ES2/gl.h>
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

CL_GL1Functions *CL_GL1::functions = 0;

int CL_GL1::get_textureformat_bits(CL_TextureFormat format)
{
	int count;
	switch (format)
	{
		case cl_r8: count = 8;	break; //RED 8
		case cl_r16: count = 16; break; //RED, 16
		case cl_rg8: count = 8 + 8; break; //RG, 8, 8
		case cl_rg16: count = 16 + 16; break; //RG, 16, 16
		case cl_r3_g3_b2: count = 3 + 3 + 2; break; //RGB, 3, 3, 2
		case cl_rgb4: count = 4 + 4 + 4; break; //RGB, 4, 4, 4
		case cl_rgb5: count = 5 + 5 + 5; break; //RGB, 5, 5, 5
		case cl_rgb8: count = 8 + 8 + 8; break; //RGB, 8, 8, 8
		case cl_rgb10: count = 10 + 10 + 10; break; //RGB, 10, 10, 10
		case cl_rgb12: count = 12 + 12 + 12; break; //RGB, 12, 12, 12
		case cl_rgb16: count = 16 + 16 + 16; break; //RGB, 16, 16, 16
		case cl_rgba2: count = 2 + 2 + 2 + 2; break; //RGBA, 2, 2, 2, 2
		case cl_rgba4: count = 4 + 4 + 4 + 4; break; //RGBA, 4, 4, 4, 4
		case cl_rgb5_a1: count = 5 + 5 + 5 + 1; break; //RGBA, 5, 5, 5, 1
		case cl_rgba8: count = 8 + 8 + 8 + 8; break; //RGBA, 8, 8, 8, 8
		case cl_rgb10_a2: count = 10 + 10 + 10 + 2; break; //RGBA, 10, 10, 10, 2
		case cl_rgba12: count = 12 + 12 + 12 + 12; break; //RGBA, 12, 12, 12, 12
		case cl_rgba16: count = 16 + 16 + 16 + 16; break; //RGBA, 16, 16, 16, 16
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
			throw CL_Exception("cannot object pixel count for this CL_TextureFormat");
	}
	return count;
}

void CL_GL1::to_opengl_textureformat(CL_TextureFormat format, GLint &gl_internal_format, GLenum &gl_pixel_format)
{
	switch (format)
	{
	// base internal format
		case cl_depth_component: gl_internal_format = GL_DEPTH_COMPONENT; gl_pixel_format = GL_DEPTH_COMPONENT; break;
		case cl_red: gl_internal_format = GL_RED; gl_pixel_format = GL_RED; break;
		case cl_rgb: gl_internal_format = GL_RGB; gl_pixel_format = GL_RGB; break;
		case cl_rgba: gl_internal_format = GL_RGBA; gl_pixel_format = GL_RGBA; break;
		case cl_stencil_index: gl_internal_format = GL_STENCIL_INDEX; gl_pixel_format = GL_STENCIL_INDEX; break;

	// sized internal format
		case cl_r3_g3_b2: gl_internal_format = GL_R3_G3_B2; gl_pixel_format = GL_RGB; break;
		case cl_rgb4: gl_internal_format = GL_RGB4; gl_pixel_format = GL_RGB; break;
		case cl_rgb5: gl_internal_format = GL_RGB5; gl_pixel_format = GL_RGB; break;
		case cl_rgb8: gl_internal_format = GL_RGB8; gl_pixel_format = GL_RGB; break;
		case cl_rgb10: gl_internal_format = GL_RGB10; gl_pixel_format = GL_RGB; break;
		case cl_rgb12: gl_internal_format = GL_RGB12; gl_pixel_format = GL_RGB; break;
		case cl_rgb16: gl_internal_format = GL_RGB16; gl_pixel_format = GL_RGB; break;
		case cl_rgba2: gl_internal_format = GL_RGBA2; gl_pixel_format = GL_RGBA; break;
		case cl_rgba4: gl_internal_format = GL_RGBA4; gl_pixel_format = GL_RGBA; break;
		case cl_rgb5_a1: gl_internal_format = GL_RGB5_A1; gl_pixel_format = GL_RGBA; break;
		case cl_rgba8: gl_internal_format = GL_RGBA8; gl_pixel_format = GL_RGBA; break;
		case cl_rgb10_a2: gl_internal_format = GL_RGB10_A2; gl_pixel_format = GL_RGBA; break;
		case cl_rgba12: gl_internal_format = GL_RGBA12; gl_pixel_format = GL_RGBA; break;
		case cl_rgba16: gl_internal_format = GL_RGBA16; gl_pixel_format = GL_RGBA; break;
		case cl_depth_component16: gl_internal_format = GL_DEPTH_COMPONENT16; gl_pixel_format = GL_DEPTH_COMPONENT; break;
		case cl_depth_component24: gl_internal_format = GL_DEPTH_COMPONENT24; gl_pixel_format = GL_DEPTH_COMPONENT; break;
		case cl_depth_component32: gl_internal_format = GL_DEPTH_COMPONENT32; gl_pixel_format = GL_DEPTH_COMPONENT; break;
		case cl_compressed_rgb: gl_internal_format = GL_COMPRESSED_RGB; gl_pixel_format = GL_RGB; break;
		case cl_compressed_rgba: gl_internal_format = GL_COMPRESSED_RGBA; gl_pixel_format = GL_RGBA; break;
		default:
			throw CL_Exception("Unknown CL_TextureFormat");
	}

}


bool CL_GL1::to_opengl_pixelformat(const CL_PixelBuffer &pbuffer, GLenum &format, GLenum &type)
{
/*
	// Types:
	GL_UNSIGNED_BYTE
	GL_BITMAP
	GL_BYTE
	GL_UNSIGNED_SHORT
	GL_SHORT
	GL_UNSIGNED_INT
	GL_INT
	GL_FLOAT
	GL_UNSIGNED_BYTE_3_3_2
	GL_UNSIGNED_BYTE_2_3_3_REV
	GL_UNSIGNED_SHORT_5_6_5
	GL_UNSIGNED_SHORT_5_6_5_REV
	GL_UNSIGNED_SHORT_4_4_4_4
	GL_UNSIGNED_SHORT_4_4_4_4_REV
	GL_UNSIGNED_SHORT_5_5_5_1
	GL_UNSIGNED_SHORT_1_5_5_5_REV
	GL_UNSIGNED_INT_8_8_8_8
	GL_UNSIGNED_INT_8_8_8_8_REV
	GL_UNSIGNED_INT_10_10_10_2
	GL_UNSIGNED_INT_2_10_10_10_REV

	// Formats:
	GL_COLOR_INDEX
	GL_STENCIL_INDEX
	GL_DEPTH_COMPONENT
	GL_RED
	GL_GREEN
	GL_BLUE
	GL_ALPHA
	GL_RGB
	GL_RGBA
	GL_BGR
	GL_BGRA
	GL_LUMINANCE
	GL_LUMINANCE_ALPHA
*/

	// indexed modes and colorkey requires special conversion to alpha and
	// therefore no OpenGL mode has a direct conversion format
	if (pbuffer.has_colorkey() || pbuffer.get_format() == cl_color_index)
		return false;

	const unsigned int rm = pbuffer.get_red_mask();
	const unsigned int gm = pbuffer.get_green_mask();
	const unsigned int bm = pbuffer.get_blue_mask();
	const unsigned int am = pbuffer.get_alpha_mask();
	if (pbuffer.get_bytes_per_pixel() == 1)
	{
		if (rm == (0x7u << 5) && gm == (0x7u << 2) && bm == (0x3u))
		{
			type = GL_UNSIGNED_BYTE_3_3_2;
			format = GL_RGB;
			return true;
		}
		if (rm == (0x7u) && gm == (0x7u << 3) && bm == (0x3u << 6))
		{
			type = GL_UNSIGNED_BYTE_2_3_3_REV;
			format = GL_RGB;
			return true;
		}
	}
	if (pbuffer.get_bytes_per_pixel() == 2)
	{
		if (rm == (0x1fu << 11) && gm == (0x3fu << 5) && bm == (0x1fu))
		{
			type = GL_UNSIGNED_SHORT_5_6_5;
			format = GL_RGB;
			return true;
		}
		if (rm == (0x1fu) && gm == (0x3fu << 5) && bm == (0x1fu << 11))
		{
			type = GL_UNSIGNED_SHORT_5_6_5_REV;
			format = GL_RGB;
			return true;
		}
		if (rm == (0x0fu << 12) && gm == (0x0fu << 8) && bm == (0x0fu << 4) && am == (0x0fu))
		{
			type = GL_UNSIGNED_SHORT_4_4_4_4;
			format = GL_RGBA;
			return true;
		}
		if (rm == (0x0fu) && gm == (0x0fu << 4) && bm == (0x0fu << 8) && am == (0x0fu << 12))
		{
			type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
			format = GL_RGBA;
			return true;
		}
		if (rm == (0x1fu << 11) && gm == (0x1fu << 6) && bm == (0x1fu << 1) && am == (0x01u))
		{
			type = GL_UNSIGNED_SHORT_5_5_5_1;
			format = GL_RGBA;
			return true;
		}
		if (rm == (0x1fu) && gm == (0x1fu << 5) && bm == (0x1fu << 10) && am == (0x01u << 15))
		{
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
			format = GL_RGBA;
			return true;
		}
	}
	if (pbuffer.get_bytes_per_pixel() == 4)
	{
		if (rm == (0xfful << 24) && gm == (0xfful << 16) && bm == (0xfful << 8) && am == (0xfful))
		{
			type = GL_UNSIGNED_INT_8_8_8_8;
			format = GL_RGBA;
			return true;
		}
		if (rm == (0xfful) && gm == (0xfful << 8) && bm == (0xfful << 16) && am == (0xfful << 24))
		{
			type = GL_UNSIGNED_INT_8_8_8_8_REV;
			format = GL_RGBA;
			return true;
		}
		if (rm == (0x3fful << 22) && gm == (0x3fful << 12) && bm == (0x3fful << 2) && am == (0x03ul))
		{
			type = GL_UNSIGNED_INT_10_10_10_2;
			format = GL_RGBA;
		}
		if (rm == (0x3fful) && gm == (0x3fful << 10) && bm == (0x3fful << 20) && am == (0x03ul << 30))
		{
			type = GL_UNSIGNED_INT_2_10_10_10_REV;
			format = GL_RGBA;
		}
	}

	bool big = CL_Endian::is_system_big();

	if (pbuffer.get_format() == cl_color_index)
	{
		format = GL_COLOR_INDEX;
		if (pbuffer.get_bytes_per_pixel() == 1)
			type = GL_UNSIGNED_BYTE;
		else if (pbuffer.get_bytes_per_pixel() == 2)
			type = GL_UNSIGNED_SHORT;
		else if (pbuffer.get_bytes_per_pixel() == 4)
			type = GL_UNSIGNED_INT;
		else
			return false;
	}
	else
	{
		const int rms = CL_PixelFormat::get_mask_shift(rm),
			gms = CL_PixelFormat::get_mask_shift(gm),
			bms = CL_PixelFormat::get_mask_shift(bm),
			ams = CL_PixelFormat::get_mask_shift(am);

		const int rmb = CL_PixelFormat::get_mask_bits(rm),
			gmb = CL_PixelFormat::get_mask_bits(gm),
			bmb = CL_PixelFormat::get_mask_bits(bm),
			amb = CL_PixelFormat::get_mask_bits(am);

		typedef std::map<int, GLenum> BitsToTypeMap;
		static BitsToTypeMap bits_to_type;
		if (bits_to_type.empty())
		{
			// initialize it only once
			bits_to_type[8] = GL_UNSIGNED_BYTE;
			bits_to_type[16] = GL_UNSIGNED_SHORT;
			bits_to_type[32] = GL_UNSIGNED_INT;
		}

		if (rm && gm && bm && am)
		{
			// the bit number must be 8, 16 or 32 for component
			// they all must have the same number of bits
			if ((!(rmb == 8 || rmb == 16 || rmb == 32)) ||
				(rmb != gmb || rmb != bmb || rmb != amb))
				return false;

			const int bits = rmb; // they all have the same bits

			if ((pbuffer.get_bytes_per_pixel() *8 ) != bits * 4)
				return false;

			type = bits_to_type[bits];

			if (!big && rms == 0*bits && gms == 1*bits && bms == 2*bits && ams == 3*bits)
				format = GL_RGBA;
			else if (!big && ams == 0*bits && bms == 1*bits && gms == 2*bits && rms == 3*bits)
				format = GL_ABGR;
			else if (big && rms == 3*bits && gms == 2*bits && bms == 1*bits && ams == 0*bits)
				format = GL_RGBA;
			else if (big && ams == 3*bits && bms == 2*bits && gms == 1*bits && rms == 0*bits)
				format = GL_ABGR;
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

			if ((pbuffer.get_bytes_per_pixel() * 8) != bits * 3)
				return false;

			type = bits_to_type[bits];

			if (!big && rms == 0*bits && gms == 1*bits && bms == 2*bits)
				format = GL_RGB;
			else if (!big && bms == 0*bits && gms == 1*bits && rms == 2*bits)
				format = GL_BGR;
			else if (big && rms == 2*bits && gms == 1*bits && bms == 0*bits)
				format = GL_RGB;
			else if (big && bms == 2*bits && gms == 1*bits && rms == 0*bits)
				format = GL_BGR;
			else
				return false;
		}
		else if (rm && !gm && !bm && !am)
		{
			format = GL_RED;
			if (rms != 0 || !bits_to_type.count(rmb))
				return false;
			if ((pbuffer.get_bytes_per_pixel() * 8) != rmb)
				return false;
			type = bits_to_type[rmb];
		}
		else if (!rm && gm && !bm && !am)
		{
			format = GL_GREEN;
			if (gms != 0 || !bits_to_type.count(gmb))
				return false;
			if ((pbuffer.get_bytes_per_pixel() * 8) != gmb)
				return false;
			type = bits_to_type[gmb];
		}
		else if (!rm && !gm && bm && !am)
		{
			format = GL_BLUE;
			if (bms != 0 || !bits_to_type.count(bmb))
				return false;
			if ((pbuffer.get_bytes_per_pixel() * 8) != bmb)
				return false;
			type = bits_to_type[bmb];
		}
		else if (!rm && !gm && !bm && am)
		{
			format = GL_ALPHA;
			if (ams != 0 || !bits_to_type.count(amb))
				return false;
			if ((pbuffer.get_bytes_per_pixel() * 8) != amb)
				return false;
			type = bits_to_type[amb];
		}
		else
			return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// OpenGL context management:

#if defined(_MSC_VER)
#define cl_tls_variable _declspec(thread)
#else
#define cl_tls_variable __thread
#endif

cl_tls_variable const CL_GL1GraphicContextProvider * cl_gl1_active_opengl_gc = 0;
static CL_Mutex cl_gl1_function_map_mutex;

// A fix for a compiler bug with compiler version 13.00.9466
#if _MSC_VER > 1300
typedef std::map<const CL_GL1GraphicContextProvider * const, CL_GL1Functions *> cl_gl1_function_map_type;
#else
typedef std::map<const CL_GL1GraphicContextProvider *, CL_GL1Functions *> cl_gl1_function_map_type;
#endif

static cl_gl1_function_map_type cl_gl1_function_map;

CL_GL1Functions *cl_gl1_setup_binds();

CL_GL1ProcAddress *CL_GL1::get_proc_address(const CL_String8& function_name)
{
	if (cl_gl1_active_opengl_gc)
	{
		return cl_gl1_active_opengl_gc->get_proc_address(function_name);
	}
	return NULL;
}

void CL_GL1::set_active(CL_GraphicContext &gc)
{
	set_active(static_cast<const CL_GL1GraphicContextProvider *>(gc.get_provider()));
}

bool CL_GL1::set_active()
{
	if (cl_gl1_active_opengl_gc)	// If already active, we can exit now
		return true;

	std::vector<CL_GraphicContextProvider*> &opengl_contexts = CL_SharedGCData::get_gc_providers();
	std::vector<CL_GraphicContextProvider*>::size_type count, size;
	size = opengl_contexts.size();
	for (count = 0; count < size; count++)
	{
		CL_GL1GraphicContextProvider *gc_provider = dynamic_cast<CL_GL1GraphicContextProvider*>(opengl_contexts[count]);
		if (gc_provider)
		{
			CL_GL1::set_active(gc_provider);
			return true;
		}
	}
	set_active(NULL);
	return false;
}

void CL_GL1::set_active(const CL_GL1GraphicContextProvider * const gc_provider)
{
	// Don't do anything if the supplied graphic context is already active.
	if (gc_provider != cl_gl1_active_opengl_gc)
	{
		if (gc_provider)
		{
			// Make render context associated with graphic context current.
			gc_provider->get_render_window().make_current();

			// Map bound functions for this graphic context. Add to static cache if necessary.
			CL_MutexSection mutex_lock(&cl_gl1_function_map_mutex);
			cl_gl1_function_map_type::iterator it;
			it = cl_gl1_function_map.find(gc_provider);
			if (it != cl_gl1_function_map.end())
			{
				CL_GL1::functions = it->second;
			}
			else
			{
				cl_gl1_active_opengl_gc = gc_provider;
				CL_GL1Functions *functions = cl_gl1_setup_binds();
				cl_gl1_function_map[gc_provider] = functions;
				CL_GL1::functions = functions;
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
			CL_GL1::functions = NULL;
		}

		// OK, make our context the active one
		cl_gl1_active_opengl_gc = gc_provider;
	}
}

void CL_GL1::remove_active(const CL_GL1GraphicContextProvider * const gc_provider)
{
	CL_MutexSection mutex_lock(&cl_gl1_function_map_mutex);
	cl_gl1_function_map_type::iterator it;
	it = cl_gl1_function_map.find(gc_provider);
	if (it != cl_gl1_function_map.end())
	{
		CL_GL1Functions *functions = it->second;
		cl_gl1_function_map.erase(it);
		delete functions;

		if (cl_gl1_active_opengl_gc == gc_provider)
		{
			cl_gl1_active_opengl_gc = NULL;
		}
	}
}

CL_GL1Functions *cl_gl1_setup_binds()
{
	CL_GL1Functions *functions = new CL_GL1Functions;
	memset(functions, 0, sizeof(CL_GL1Functions));

	// Binds for OpenGL 1.1:

//#ifdef GL_VERSION_1_1
#ifdef WIN32	// Only win32 static links opengl

	functions->accum = (CL_GL1Functions::ptr_glAccum) &glAccum;
	functions->alphaFunc = (CL_GL1Functions::ptr_glAlphaFunc) &glAlphaFunc;
	functions->areTexturesResident = (CL_GL1Functions::ptr_glAreTexturesResident) &glAreTexturesResident;
	functions->arrayElement = (CL_GL1Functions::ptr_glArrayElement) &glArrayElement;
	functions->begin = (CL_GL1Functions::ptr_glBegin) &glBegin;
	functions->bindTexture = (CL_GL1Functions::ptr_glBindTexture) &glBindTexture;
	functions->bitmap = (CL_GL1Functions::ptr_glBitmap) &glBitmap;
	functions->blendFunc = (CL_GL1Functions::ptr_glBlendFunc) &glBlendFunc;
	functions->callList = (CL_GL1Functions::ptr_glCallList) &glCallList;
	functions->callLists = (CL_GL1Functions::ptr_glCallLists) &glCallLists;
	functions->clear = (CL_GL1Functions::ptr_glClear) &glClear;
	functions->clearAccum = (CL_GL1Functions::ptr_glClearAccum) &glClearAccum;
	functions->clearColor = (CL_GL1Functions::ptr_glClearColor) &glClearColor;
	functions->clearDepth = (CL_GL1Functions::ptr_glClearDepth) &glClearDepth;
	functions->clearIndex = (CL_GL1Functions::ptr_glClearIndex) &glClearIndex;
	functions->clearStencil = (CL_GL1Functions::ptr_glClearStencil) &glClearStencil;
	functions->clipPlane = (CL_GL1Functions::ptr_glClipPlane) &glClipPlane;
	functions->color3b = (CL_GL1Functions::ptr_glColor3b) &glColor3b;
	functions->color3bv = (CL_GL1Functions::ptr_glColor3bv) &glColor3bv;
	functions->color3d = (CL_GL1Functions::ptr_glColor3d) &glColor3d;
	functions->color3dv = (CL_GL1Functions::ptr_glColor3dv) &glColor3dv;
	functions->color3f = (CL_GL1Functions::ptr_glColor3f) &glColor3f;
	functions->color3fv = (CL_GL1Functions::ptr_glColor3fv) &glColor3fv;
	functions->color3i = (CL_GL1Functions::ptr_glColor3i) &glColor3i;
	functions->color3iv = (CL_GL1Functions::ptr_glColor3iv) &glColor3iv;
	functions->color3s = (CL_GL1Functions::ptr_glColor3s) &glColor3s;
	functions->color3sv = (CL_GL1Functions::ptr_glColor3sv) &glColor3sv;
	functions->color3ub = (CL_GL1Functions::ptr_glColor3ub) &glColor3ub;
	functions->color3ubv = (CL_GL1Functions::ptr_glColor3ubv) &glColor3ubv;
	functions->color3ui = (CL_GL1Functions::ptr_glColor3ui) &glColor3ui;
	functions->color3uiv = (CL_GL1Functions::ptr_glColor3uiv) &glColor3uiv;
	functions->color3us = (CL_GL1Functions::ptr_glColor3us) &glColor3us;
	functions->color3usv = (CL_GL1Functions::ptr_glColor3usv) &glColor3usv;
	functions->color4b = (CL_GL1Functions::ptr_glColor4b) &glColor4b;
	functions->color4bv = (CL_GL1Functions::ptr_glColor4bv) &glColor4bv;
	functions->color4d = (CL_GL1Functions::ptr_glColor4d) &glColor4d;
	functions->color4dv = (CL_GL1Functions::ptr_glColor4dv) &glColor4dv;
	functions->color4f = (CL_GL1Functions::ptr_glColor4f) &glColor4f;
	functions->color4fv = (CL_GL1Functions::ptr_glColor4fv) &glColor4fv;
	functions->color4i = (CL_GL1Functions::ptr_glColor4i) &glColor4i;
	functions->color4iv = (CL_GL1Functions::ptr_glColor4iv) &glColor4iv;
	functions->color4s = (CL_GL1Functions::ptr_glColor4s) &glColor4s;
	functions->color4sv = (CL_GL1Functions::ptr_glColor4sv) &glColor4sv;
	functions->color4ub = (CL_GL1Functions::ptr_glColor4ub) &glColor4ub;
	functions->color4ubv = (CL_GL1Functions::ptr_glColor4ubv) &glColor4ubv;
	functions->color4ui = (CL_GL1Functions::ptr_glColor4ui) &glColor4ui;
	functions->color4uiv = (CL_GL1Functions::ptr_glColor4uiv) &glColor4uiv;
	functions->color4us = (CL_GL1Functions::ptr_glColor4us) &glColor4us;
	functions->color4usv = (CL_GL1Functions::ptr_glColor4usv) &glColor4usv;
	functions->colorMask = (CL_GL1Functions::ptr_glColorMask) &glColorMask;
	functions->colorMaterial = (CL_GL1Functions::ptr_glColorMaterial) &glColorMaterial;
	functions->colorPointer = (CL_GL1Functions::ptr_glColorPointer) &glColorPointer;
	functions->copyPixels = (CL_GL1Functions::ptr_glCopyPixels) &glCopyPixels;
	functions->copyTexImage1D = (CL_GL1Functions::ptr_glCopyTexImage1D) &glCopyTexImage1D;
	functions->copyTexImage2D = (CL_GL1Functions::ptr_glCopyTexImage2D) &glCopyTexImage2D;
	functions->copyTexSubImage1D = (CL_GL1Functions::ptr_glCopyTexSubImage1D) &glCopyTexSubImage1D;
	functions->copyTexSubImage2D = (CL_GL1Functions::ptr_glCopyTexSubImage2D) &glCopyTexSubImage2D;
	functions->cullFace = (CL_GL1Functions::ptr_glCullFace) &glCullFace;
	functions->deleteLists = (CL_GL1Functions::ptr_glDeleteLists) &glDeleteLists;
	functions->deleteTextures = (CL_GL1Functions::ptr_glDeleteTextures) &glDeleteTextures;
	functions->depthFunc = (CL_GL1Functions::ptr_glDepthFunc) &glDepthFunc;
	functions->depthMask = (CL_GL1Functions::ptr_glDepthMask) &glDepthMask;
	functions->depthRange = (CL_GL1Functions::ptr_glDepthRange) &glDepthRange;
	functions->disable = (CL_GL1Functions::ptr_glDisable) &glDisable;
	functions->disableClientState = (CL_GL1Functions::ptr_glDisableClientState) &glDisableClientState;
	functions->drawArrays = (CL_GL1Functions::ptr_glDrawArrays) &glDrawArrays;
	functions->drawBuffer = (CL_GL1Functions::ptr_glDrawBuffer) &glDrawBuffer;
	functions->drawElements = (CL_GL1Functions::ptr_glDrawElements) &glDrawElements;
	functions->drawPixels = (CL_GL1Functions::ptr_glDrawPixels) &glDrawPixels;
	functions->edgeFlag = (CL_GL1Functions::ptr_glEdgeFlag) &glEdgeFlag;
	functions->edgeFlagPointer = (CL_GL1Functions::ptr_glEdgeFlagPointer) &glEdgeFlagPointer;
	functions->edgeFlagv = (CL_GL1Functions::ptr_glEdgeFlagv) &glEdgeFlagv;
	functions->enable = (CL_GL1Functions::ptr_glEnable) &glEnable;
	functions->enableClientState = (CL_GL1Functions::ptr_glEnableClientState) &glEnableClientState;
	functions->end = (CL_GL1Functions::ptr_glEnd) &glEnd;
	functions->endList = (CL_GL1Functions::ptr_glEndList) &glEndList;
	functions->evalCoord1d = (CL_GL1Functions::ptr_glEvalCoord1d) &glEvalCoord1d;
	functions->evalCoord1dv = (CL_GL1Functions::ptr_glEvalCoord1dv) &glEvalCoord1dv;
	functions->evalCoord1f = (CL_GL1Functions::ptr_glEvalCoord1f) &glEvalCoord1f;
	functions->evalCoord1fv = (CL_GL1Functions::ptr_glEvalCoord1fv) &glEvalCoord1fv;
	functions->evalCoord2d = (CL_GL1Functions::ptr_glEvalCoord2d) &glEvalCoord2d;
	functions->evalCoord2dv = (CL_GL1Functions::ptr_glEvalCoord2dv) &glEvalCoord2dv;
	functions->evalCoord2f = (CL_GL1Functions::ptr_glEvalCoord2f) &glEvalCoord2f;
	functions->evalCoord2fv = (CL_GL1Functions::ptr_glEvalCoord2fv) &glEvalCoord2fv;
	functions->evalMesh1 = (CL_GL1Functions::ptr_glEvalMesh1) &glEvalMesh1;
	functions->evalMesh2 = (CL_GL1Functions::ptr_glEvalMesh2) &glEvalMesh2;
	functions->evalPoint1 = (CL_GL1Functions::ptr_glEvalPoint1) &glEvalPoint1;
	functions->evalPoint2 = (CL_GL1Functions::ptr_glEvalPoint2) &glEvalPoint2;
	functions->feedbackBuffer = (CL_GL1Functions::ptr_glFeedbackBuffer) &glFeedbackBuffer;
	functions->finish = (CL_GL1Functions::ptr_glFinish) &glFinish;
	functions->flush = (CL_GL1Functions::ptr_glFlush) &glFlush;
	functions->fogf = (CL_GL1Functions::ptr_glFogf) &glFogf;
	functions->fogfv = (CL_GL1Functions::ptr_glFogfv) &glFogfv;
	functions->fogi = (CL_GL1Functions::ptr_glFogi) &glFogi;
	functions->fogiv = (CL_GL1Functions::ptr_glFogiv) &glFogiv;
	functions->frontFace = (CL_GL1Functions::ptr_glFrontFace) &glFrontFace;
	functions->frustum = (CL_GL1Functions::ptr_glFrustum) &glFrustum;
	functions->genLists = (CL_GL1Functions::ptr_glGenLists) &glGenLists;
	functions->genTextures = (CL_GL1Functions::ptr_glGenTextures) &glGenTextures;
	functions->getBooleanv = (CL_GL1Functions::ptr_glGetBooleanv) &glGetBooleanv;
	functions->getClipPlane = (CL_GL1Functions::ptr_glGetClipPlane) &glGetClipPlane;
	functions->getDoublev = (CL_GL1Functions::ptr_glGetDoublev) &glGetDoublev;
	functions->getError = (CL_GL1Functions::ptr_glGetError) &glGetError;
	functions->getFloatv = (CL_GL1Functions::ptr_glGetFloatv) &glGetFloatv;
	functions->getIntegerv = (CL_GL1Functions::ptr_glGetIntegerv) &glGetIntegerv;
	functions->getLightfv = (CL_GL1Functions::ptr_glGetLightfv) &glGetLightfv;
	functions->getLightiv = (CL_GL1Functions::ptr_glGetLightiv) &glGetLightiv;
	functions->getMapdv = (CL_GL1Functions::ptr_glGetMapdv) &glGetMapdv;
	functions->getMapfv = (CL_GL1Functions::ptr_glGetMapfv) &glGetMapfv;
	functions->getMapiv = (CL_GL1Functions::ptr_glGetMapiv) &glGetMapiv;
	functions->getMaterialfv = (CL_GL1Functions::ptr_glGetMaterialfv) &glGetMaterialfv;
	functions->getMaterialiv = (CL_GL1Functions::ptr_glGetMaterialiv) &glGetMaterialiv;
	functions->getPixelMapfv = (CL_GL1Functions::ptr_glGetPixelMapfv) &glGetPixelMapfv;
	functions->getPixelMapuiv = (CL_GL1Functions::ptr_glGetPixelMapuiv) &glGetPixelMapuiv;
	functions->getPixelMapusv = (CL_GL1Functions::ptr_glGetPixelMapusv) &glGetPixelMapusv;
	functions->getPointerv = (CL_GL1Functions::ptr_glGetPointerv) &glGetPointerv;
	functions->getPolygonStipple = (CL_GL1Functions::ptr_glGetPolygonStipple) &glGetPolygonStipple;
	functions->getString = (CL_GL1Functions::ptr_glGetString) &glGetString;
	functions->getTexEnvfv = (CL_GL1Functions::ptr_glGetTexEnvfv) &glGetTexEnvfv;
	functions->getTexEnviv = (CL_GL1Functions::ptr_glGetTexEnviv) &glGetTexEnviv;
	functions->getTexGendv = (CL_GL1Functions::ptr_glGetTexGendv) &glGetTexGendv;
	functions->getTexGenfv = (CL_GL1Functions::ptr_glGetTexGenfv) &glGetTexGenfv;
	functions->getTexGeniv = (CL_GL1Functions::ptr_glGetTexGeniv) &glGetTexGeniv;
	functions->getTexImage = (CL_GL1Functions::ptr_glGetTexImage) &glGetTexImage;
	functions->getTexLevelParameterfv = (CL_GL1Functions::ptr_glGetTexLevelParameterfv) &glGetTexLevelParameterfv;
	functions->getTexLevelParameteriv = (CL_GL1Functions::ptr_glGetTexLevelParameteriv) &glGetTexLevelParameteriv;
	functions->getTexParameterfv = (CL_GL1Functions::ptr_glGetTexParameterfv) &glGetTexParameterfv;
	functions->getTexParameteriv = (CL_GL1Functions::ptr_glGetTexParameteriv) &glGetTexParameteriv;
	functions->hint = (CL_GL1Functions::ptr_glHint) &glHint;
	functions->indexMask = (CL_GL1Functions::ptr_glIndexMask) &glIndexMask;
	functions->indexPointer = (CL_GL1Functions::ptr_glIndexPointer) &glIndexPointer;
	functions->indexd = (CL_GL1Functions::ptr_glIndexd) &glIndexd;
	functions->indexdv = (CL_GL1Functions::ptr_glIndexdv) &glIndexdv;
	functions->indexf = (CL_GL1Functions::ptr_glIndexf) &glIndexf;
	functions->indexfv = (CL_GL1Functions::ptr_glIndexfv) &glIndexfv;
	functions->indexi = (CL_GL1Functions::ptr_glIndexi) &glIndexi;
	functions->indexiv = (CL_GL1Functions::ptr_glIndexiv) &glIndexiv;
	functions->indexs = (CL_GL1Functions::ptr_glIndexs) &glIndexs;
	functions->indexsv = (CL_GL1Functions::ptr_glIndexsv) &glIndexsv;
	functions->indexub = (CL_GL1Functions::ptr_glIndexub) &glIndexub;
	functions->indexubv = (CL_GL1Functions::ptr_glIndexubv) &glIndexubv;
	functions->initNames = (CL_GL1Functions::ptr_glInitNames) &glInitNames;
	functions->interleavedArrays = (CL_GL1Functions::ptr_glInterleavedArrays) &glInterleavedArrays;
	functions->isEnabled = (CL_GL1Functions::ptr_glIsEnabled) &glIsEnabled;
	functions->isList = (CL_GL1Functions::ptr_glIsList) &glIsList;
	functions->isTexture = (CL_GL1Functions::ptr_glIsTexture) &glIsTexture;
	functions->lightModelf = (CL_GL1Functions::ptr_glLightModelf) &glLightModelf;
	functions->lightModelfv = (CL_GL1Functions::ptr_glLightModelfv) &glLightModelfv;
	functions->lightModeli = (CL_GL1Functions::ptr_glLightModeli) &glLightModeli;
	functions->lightModeliv = (CL_GL1Functions::ptr_glLightModeliv) &glLightModeliv;
	functions->lightf = (CL_GL1Functions::ptr_glLightf) &glLightf;
	functions->lightfv = (CL_GL1Functions::ptr_glLightfv) &glLightfv;
	functions->lighti = (CL_GL1Functions::ptr_glLighti) &glLighti;
	functions->lightiv = (CL_GL1Functions::ptr_glLightiv) &glLightiv;
	functions->lineStipple = (CL_GL1Functions::ptr_glLineStipple) &glLineStipple;
	functions->lineWidth = (CL_GL1Functions::ptr_glLineWidth) &glLineWidth;
	functions->listBase = (CL_GL1Functions::ptr_glListBase) &glListBase;
	functions->loadIdentity = (CL_GL1Functions::ptr_glLoadIdentity) &glLoadIdentity;
	functions->loadMatrixd = (CL_GL1Functions::ptr_glLoadMatrixd) &glLoadMatrixd;
	functions->loadMatrixf = (CL_GL1Functions::ptr_glLoadMatrixf) &glLoadMatrixf;
	functions->loadName = (CL_GL1Functions::ptr_glLoadName) &glLoadName;
	functions->logicOp = (CL_GL1Functions::ptr_glLogicOp) &glLogicOp;
	functions->map1d = (CL_GL1Functions::ptr_glMap1d) &glMap1d;
	functions->map1f = (CL_GL1Functions::ptr_glMap1f) &glMap1f;
	functions->map2d = (CL_GL1Functions::ptr_glMap2d) &glMap2d;
	functions->map2f = (CL_GL1Functions::ptr_glMap2f) &glMap2f;
	functions->mapGrid1d = (CL_GL1Functions::ptr_glMapGrid1d) &glMapGrid1d;
	functions->mapGrid1f = (CL_GL1Functions::ptr_glMapGrid1f) &glMapGrid1f;
	functions->mapGrid2d = (CL_GL1Functions::ptr_glMapGrid2d) &glMapGrid2d;
	functions->mapGrid2f = (CL_GL1Functions::ptr_glMapGrid2f) &glMapGrid2f;
	functions->materialf = (CL_GL1Functions::ptr_glMaterialf) &glMaterialf;
	functions->materialfv = (CL_GL1Functions::ptr_glMaterialfv) &glMaterialfv;
	functions->materiali = (CL_GL1Functions::ptr_glMateriali) &glMateriali;
	functions->materialiv = (CL_GL1Functions::ptr_glMaterialiv) &glMaterialiv;
	functions->matrixMode = (CL_GL1Functions::ptr_glMatrixMode) &glMatrixMode;
	functions->multMatrixd = (CL_GL1Functions::ptr_glMultMatrixd) &glMultMatrixd;
	functions->multMatrixf = (CL_GL1Functions::ptr_glMultMatrixf) &glMultMatrixf;
	functions->newList = (CL_GL1Functions::ptr_glNewList) &glNewList;
	functions->normal3b = (CL_GL1Functions::ptr_glNormal3b) &glNormal3b;
	functions->normal3bv = (CL_GL1Functions::ptr_glNormal3bv) &glNormal3bv;
	functions->normal3d = (CL_GL1Functions::ptr_glNormal3d) &glNormal3d;
	functions->normal3dv = (CL_GL1Functions::ptr_glNormal3dv) &glNormal3dv;
	functions->normal3f = (CL_GL1Functions::ptr_glNormal3f) &glNormal3f;
	functions->normal3fv = (CL_GL1Functions::ptr_glNormal3fv) &glNormal3fv;
	functions->normal3i = (CL_GL1Functions::ptr_glNormal3i) &glNormal3i;
	functions->normal3iv = (CL_GL1Functions::ptr_glNormal3iv) &glNormal3iv;
	functions->normal3s = (CL_GL1Functions::ptr_glNormal3s) &glNormal3s;
	functions->normal3sv = (CL_GL1Functions::ptr_glNormal3sv) &glNormal3sv;
	functions->normalPointer = (CL_GL1Functions::ptr_glNormalPointer) &glNormalPointer;
	functions->ortho = (CL_GL1Functions::ptr_glOrtho) &glOrtho;
	functions->passThrough = (CL_GL1Functions::ptr_glPassThrough) &glPassThrough;
	functions->pixelMapfv = (CL_GL1Functions::ptr_glPixelMapfv) &glPixelMapfv;
	functions->pixelMapuiv = (CL_GL1Functions::ptr_glPixelMapuiv) &glPixelMapuiv;
	functions->pixelMapusv = (CL_GL1Functions::ptr_glPixelMapusv) &glPixelMapusv;
	functions->pixelStoref = (CL_GL1Functions::ptr_glPixelStoref) &glPixelStoref;
	functions->pixelStorei = (CL_GL1Functions::ptr_glPixelStorei) &glPixelStorei;
	functions->pixelTransferf = (CL_GL1Functions::ptr_glPixelTransferf) &glPixelTransferf;
	functions->pixelTransferi = (CL_GL1Functions::ptr_glPixelTransferi) &glPixelTransferi;
	functions->pixelZoom = (CL_GL1Functions::ptr_glPixelZoom) &glPixelZoom;
	functions->pointSize = (CL_GL1Functions::ptr_glPointSize) &glPointSize;
	functions->polygonMode = (CL_GL1Functions::ptr_glPolygonMode) &glPolygonMode;
	functions->polygonOffset = (CL_GL1Functions::ptr_glPolygonOffset) &glPolygonOffset;
	functions->polygonStipple = (CL_GL1Functions::ptr_glPolygonStipple) &glPolygonStipple;
	functions->popAttrib = (CL_GL1Functions::ptr_glPopAttrib) &glPopAttrib;
	functions->popClientAttrib = (CL_GL1Functions::ptr_glPopClientAttrib) &glPopClientAttrib;
	functions->popMatrix = (CL_GL1Functions::ptr_glPopMatrix) &glPopMatrix;
	functions->popName = (CL_GL1Functions::ptr_glPopName) &glPopName;
	functions->prioritizeTextures = (CL_GL1Functions::ptr_glPrioritizeTextures) &glPrioritizeTextures;
	functions->pushAttrib = (CL_GL1Functions::ptr_glPushAttrib) &glPushAttrib;
	functions->pushClientAttrib = (CL_GL1Functions::ptr_glPushClientAttrib) &glPushClientAttrib;
	functions->pushMatrix = (CL_GL1Functions::ptr_glPushMatrix) &glPushMatrix;
	functions->pushName = (CL_GL1Functions::ptr_glPushName) &glPushName;
	functions->rasterPos2d = (CL_GL1Functions::ptr_glRasterPos2d) &glRasterPos2d;
	functions->rasterPos2dv = (CL_GL1Functions::ptr_glRasterPos2dv) &glRasterPos2dv;
	functions->rasterPos2f = (CL_GL1Functions::ptr_glRasterPos2f) &glRasterPos2f;
	functions->rasterPos2fv = (CL_GL1Functions::ptr_glRasterPos2fv) &glRasterPos2fv;
	functions->rasterPos2i = (CL_GL1Functions::ptr_glRasterPos2i) &glRasterPos2i;
	functions->rasterPos2iv = (CL_GL1Functions::ptr_glRasterPos2iv) &glRasterPos2iv;
	functions->rasterPos2s = (CL_GL1Functions::ptr_glRasterPos2s) &glRasterPos2s;
	functions->rasterPos2sv = (CL_GL1Functions::ptr_glRasterPos2sv) &glRasterPos2sv;
	functions->rasterPos3d = (CL_GL1Functions::ptr_glRasterPos3d) &glRasterPos3d;
	functions->rasterPos3dv = (CL_GL1Functions::ptr_glRasterPos3dv) &glRasterPos3dv;
	functions->rasterPos3f = (CL_GL1Functions::ptr_glRasterPos3f) &glRasterPos3f;
	functions->rasterPos3fv = (CL_GL1Functions::ptr_glRasterPos3fv) &glRasterPos3fv;
	functions->rasterPos3i = (CL_GL1Functions::ptr_glRasterPos3i) &glRasterPos3i;
	functions->rasterPos3iv = (CL_GL1Functions::ptr_glRasterPos3iv) &glRasterPos3iv;
	functions->rasterPos3s = (CL_GL1Functions::ptr_glRasterPos3s) &glRasterPos3s;
	functions->rasterPos3sv = (CL_GL1Functions::ptr_glRasterPos3sv) &glRasterPos3sv;
	functions->rasterPos4d = (CL_GL1Functions::ptr_glRasterPos4d) &glRasterPos4d;
	functions->rasterPos4dv = (CL_GL1Functions::ptr_glRasterPos4dv) &glRasterPos4dv;
	functions->rasterPos4f = (CL_GL1Functions::ptr_glRasterPos4f) &glRasterPos4f;
	functions->rasterPos4fv = (CL_GL1Functions::ptr_glRasterPos4fv) &glRasterPos4fv;
	functions->rasterPos4i = (CL_GL1Functions::ptr_glRasterPos4i) &glRasterPos4i;
	functions->rasterPos4iv = (CL_GL1Functions::ptr_glRasterPos4iv) &glRasterPos4iv;
	functions->rasterPos4s = (CL_GL1Functions::ptr_glRasterPos4s) &glRasterPos4s;
	functions->rasterPos4sv = (CL_GL1Functions::ptr_glRasterPos4sv) &glRasterPos4sv;
	functions->readBuffer = (CL_GL1Functions::ptr_glReadBuffer) &glReadBuffer;
	functions->readPixels = (CL_GL1Functions::ptr_glReadPixels) &glReadPixels;
	functions->rectd = (CL_GL1Functions::ptr_glRectd) &glRectd;
	functions->rectdv = (CL_GL1Functions::ptr_glRectdv) &glRectdv;
	functions->rectf = (CL_GL1Functions::ptr_glRectf) &glRectf;
	functions->rectfv = (CL_GL1Functions::ptr_glRectfv) &glRectfv;
	functions->recti = (CL_GL1Functions::ptr_glRecti) &glRecti;
	functions->rectiv = (CL_GL1Functions::ptr_glRectiv) &glRectiv;
	functions->rects = (CL_GL1Functions::ptr_glRects) &glRects;
	functions->rectsv = (CL_GL1Functions::ptr_glRectsv) &glRectsv;
	functions->renderMode = (CL_GL1Functions::ptr_glRenderMode) &glRenderMode;
	functions->rotated = (CL_GL1Functions::ptr_glRotated) &glRotated;
	functions->rotatef = (CL_GL1Functions::ptr_glRotatef) &glRotatef;
	functions->scaled = (CL_GL1Functions::ptr_glScaled) &glScaled;
	functions->scalef = (CL_GL1Functions::ptr_glScalef) &glScalef;
	functions->scissor = (CL_GL1Functions::ptr_glScissor) &glScissor;
	functions->selectBuffer = (CL_GL1Functions::ptr_glSelectBuffer) &glSelectBuffer;
	functions->shadeModel = (CL_GL1Functions::ptr_glShadeModel) &glShadeModel;
	functions->stencilFunc = (CL_GL1Functions::ptr_glStencilFunc) &glStencilFunc;
	functions->stencilMask = (CL_GL1Functions::ptr_glStencilMask) &glStencilMask;
	functions->stencilOp = (CL_GL1Functions::ptr_glStencilOp) &glStencilOp;
	functions->texCoord1d = (CL_GL1Functions::ptr_glTexCoord1d) &glTexCoord1d;
	functions->texCoord1dv = (CL_GL1Functions::ptr_glTexCoord1dv) &glTexCoord1dv;
	functions->texCoord1f = (CL_GL1Functions::ptr_glTexCoord1f) &glTexCoord1f;
	functions->texCoord1fv = (CL_GL1Functions::ptr_glTexCoord1fv) &glTexCoord1fv;
	functions->texCoord1i = (CL_GL1Functions::ptr_glTexCoord1i) &glTexCoord1i;
	functions->texCoord1iv = (CL_GL1Functions::ptr_glTexCoord1iv) &glTexCoord1iv;
	functions->texCoord1s = (CL_GL1Functions::ptr_glTexCoord1s) &glTexCoord1s;
	functions->texCoord1sv = (CL_GL1Functions::ptr_glTexCoord1sv) &glTexCoord1sv;
	functions->texCoord2d = (CL_GL1Functions::ptr_glTexCoord2d) &glTexCoord2d;
	functions->texCoord2dv = (CL_GL1Functions::ptr_glTexCoord2dv) &glTexCoord2dv;
	functions->texCoord2f = (CL_GL1Functions::ptr_glTexCoord2f) &glTexCoord2f;
	functions->texCoord2fv = (CL_GL1Functions::ptr_glTexCoord2fv) &glTexCoord2fv;
	functions->texCoord2i = (CL_GL1Functions::ptr_glTexCoord2i) &glTexCoord2i;
	functions->texCoord2iv = (CL_GL1Functions::ptr_glTexCoord2iv) &glTexCoord2iv;
	functions->texCoord2s = (CL_GL1Functions::ptr_glTexCoord2s) &glTexCoord2s;
	functions->texCoord2sv = (CL_GL1Functions::ptr_glTexCoord2sv) &glTexCoord2sv;
	functions->texCoord3d = (CL_GL1Functions::ptr_glTexCoord3d) &glTexCoord3d;
	functions->texCoord3dv = (CL_GL1Functions::ptr_glTexCoord3dv) &glTexCoord3dv;
	functions->texCoord3f = (CL_GL1Functions::ptr_glTexCoord3f) &glTexCoord3f;
	functions->texCoord3fv = (CL_GL1Functions::ptr_glTexCoord3fv) &glTexCoord3fv;
	functions->texCoord3i = (CL_GL1Functions::ptr_glTexCoord3i) &glTexCoord3i;
	functions->texCoord3iv = (CL_GL1Functions::ptr_glTexCoord3iv) &glTexCoord3iv;
	functions->texCoord3s = (CL_GL1Functions::ptr_glTexCoord3s) &glTexCoord3s;
	functions->texCoord3sv = (CL_GL1Functions::ptr_glTexCoord3sv) &glTexCoord3sv;
	functions->texCoord4d = (CL_GL1Functions::ptr_glTexCoord4d) &glTexCoord4d;
	functions->texCoord4dv = (CL_GL1Functions::ptr_glTexCoord4dv) &glTexCoord4dv;
	functions->texCoord4f = (CL_GL1Functions::ptr_glTexCoord4f) &glTexCoord4f;
	functions->texCoord4fv = (CL_GL1Functions::ptr_glTexCoord4fv) &glTexCoord4fv;
	functions->texCoord4i = (CL_GL1Functions::ptr_glTexCoord4i) &glTexCoord4i;
	functions->texCoord4iv = (CL_GL1Functions::ptr_glTexCoord4iv) &glTexCoord4iv;
	functions->texCoord4s = (CL_GL1Functions::ptr_glTexCoord4s) &glTexCoord4s;
	functions->texCoord4sv = (CL_GL1Functions::ptr_glTexCoord4sv) &glTexCoord4sv;
	functions->texCoordPointer = (CL_GL1Functions::ptr_glTexCoordPointer) &glTexCoordPointer;
	functions->texEnvf = (CL_GL1Functions::ptr_glTexEnvf) &glTexEnvf;
	functions->texEnvfv = (CL_GL1Functions::ptr_glTexEnvfv) &glTexEnvfv;
	functions->texEnvi = (CL_GL1Functions::ptr_glTexEnvi) &glTexEnvi;
	functions->texEnviv = (CL_GL1Functions::ptr_glTexEnviv) &glTexEnviv;
	functions->texGend = (CL_GL1Functions::ptr_glTexGend) &glTexGend;
	functions->texGendv = (CL_GL1Functions::ptr_glTexGendv) &glTexGendv;
	functions->texGenf = (CL_GL1Functions::ptr_glTexGenf) &glTexGenf;
	functions->texGenfv = (CL_GL1Functions::ptr_glTexGenfv) &glTexGenfv;
	functions->texGeni = (CL_GL1Functions::ptr_glTexGeni) &glTexGeni;
	functions->texGeniv = (CL_GL1Functions::ptr_glTexGeniv) &glTexGeniv;
	functions->texImage1D = (CL_GL1Functions::ptr_glTexImage1D) &glTexImage1D;
	functions->texImage2D = (CL_GL1Functions::ptr_glTexImage2D) &glTexImage2D;
	functions->texParameterf = (CL_GL1Functions::ptr_glTexParameterf) &glTexParameterf;
	functions->texParameterfv = (CL_GL1Functions::ptr_glTexParameterfv) &glTexParameterfv;
	functions->texParameteri = (CL_GL1Functions::ptr_glTexParameteri) &glTexParameteri;
	functions->texParameteriv = (CL_GL1Functions::ptr_glTexParameteriv) &glTexParameteriv;
	functions->texSubImage1D = (CL_GL1Functions::ptr_glTexSubImage1D) &glTexSubImage1D;
	functions->texSubImage2D = (CL_GL1Functions::ptr_glTexSubImage2D) &glTexSubImage2D;
	functions->translated = (CL_GL1Functions::ptr_glTranslated) &glTranslated;
	functions->translatef = (CL_GL1Functions::ptr_glTranslatef) &glTranslatef;
	functions->vertex2d = (CL_GL1Functions::ptr_glVertex2d) &glVertex2d;
	functions->vertex2dv = (CL_GL1Functions::ptr_glVertex2dv) &glVertex2dv;
	functions->vertex2f = (CL_GL1Functions::ptr_glVertex2f) &glVertex2f;
	functions->vertex2fv = (CL_GL1Functions::ptr_glVertex2fv) &glVertex2fv;
	functions->vertex2i = (CL_GL1Functions::ptr_glVertex2i) &glVertex2i;
	functions->vertex2iv = (CL_GL1Functions::ptr_glVertex2iv) &glVertex2iv;
	functions->vertex2s = (CL_GL1Functions::ptr_glVertex2s) &glVertex2s;
	functions->vertex2sv = (CL_GL1Functions::ptr_glVertex2sv) &glVertex2sv;
	functions->vertex3d = (CL_GL1Functions::ptr_glVertex3d) &glVertex3d;
	functions->vertex3dv = (CL_GL1Functions::ptr_glVertex3dv) &glVertex3dv;
	functions->vertex3f = (CL_GL1Functions::ptr_glVertex3f) &glVertex3f;
	functions->vertex3fv = (CL_GL1Functions::ptr_glVertex3fv) &glVertex3fv;
	functions->vertex3i = (CL_GL1Functions::ptr_glVertex3i) &glVertex3i;
	functions->vertex3iv = (CL_GL1Functions::ptr_glVertex3iv) &glVertex3iv;
	functions->vertex3s = (CL_GL1Functions::ptr_glVertex3s) &glVertex3s;
	functions->vertex3sv = (CL_GL1Functions::ptr_glVertex3sv) &glVertex3sv;
	functions->vertex4d = (CL_GL1Functions::ptr_glVertex4d) &glVertex4d;
	functions->vertex4dv = (CL_GL1Functions::ptr_glVertex4dv) &glVertex4dv;
	functions->vertex4f = (CL_GL1Functions::ptr_glVertex4f) &glVertex4f;
	functions->vertex4fv = (CL_GL1Functions::ptr_glVertex4fv) &glVertex4fv;
	functions->vertex4i = (CL_GL1Functions::ptr_glVertex4i) &glVertex4i;
	functions->vertex4iv = (CL_GL1Functions::ptr_glVertex4iv) &glVertex4iv;
	functions->vertex4s = (CL_GL1Functions::ptr_glVertex4s) &glVertex4s;
	functions->vertex4sv = (CL_GL1Functions::ptr_glVertex4sv) &glVertex4sv;
	functions->vertexPointer = (CL_GL1Functions::ptr_glVertexPointer) &glVertexPointer;
	functions->viewport = (CL_GL1Functions::ptr_glViewport) &glViewport;
#else
	functions->accum = (CL_GL1Functions::ptr_glAccum) CL_GL1::get_proc_address("glAccum");
	functions->alphaFunc = (CL_GL1Functions::ptr_glAlphaFunc) CL_GL1::get_proc_address("glAlphaFunc");
	functions->areTexturesResident = (CL_GL1Functions::ptr_glAreTexturesResident) CL_GL1::get_proc_address("glAreTexturesResident");
	functions->arrayElement = (CL_GL1Functions::ptr_glArrayElement) CL_GL1::get_proc_address("glArrayElement");
	functions->begin = (CL_GL1Functions::ptr_glBegin) CL_GL1::get_proc_address("glBegin");
	functions->bindTexture = (CL_GL1Functions::ptr_glBindTexture) CL_GL1::get_proc_address("glBindTexture");
	functions->bitmap = (CL_GL1Functions::ptr_glBitmap) CL_GL1::get_proc_address("glBitmap");
	functions->blendFunc = (CL_GL1Functions::ptr_glBlendFunc) CL_GL1::get_proc_address("glBlendFunc");
	functions->callList = (CL_GL1Functions::ptr_glCallList) CL_GL1::get_proc_address("glCallList");
	functions->callLists = (CL_GL1Functions::ptr_glCallLists) CL_GL1::get_proc_address("glCallLists");
	functions->clear = (CL_GL1Functions::ptr_glClear) CL_GL1::get_proc_address("glClear");
	functions->clearAccum = (CL_GL1Functions::ptr_glClearAccum) CL_GL1::get_proc_address("glClearAccum");
	functions->clearColor = (CL_GL1Functions::ptr_glClearColor) CL_GL1::get_proc_address("glClearColor");
	functions->clearDepth = (CL_GL1Functions::ptr_glClearDepth) CL_GL1::get_proc_address("glClearDepth");
	functions->clearIndex = (CL_GL1Functions::ptr_glClearIndex) CL_GL1::get_proc_address("glClearIndex");
	functions->clearStencil = (CL_GL1Functions::ptr_glClearStencil) CL_GL1::get_proc_address("glClearStencil");
	functions->clipPlane = (CL_GL1Functions::ptr_glClipPlane) CL_GL1::get_proc_address("glClipPlane");
	functions->color3b = (CL_GL1Functions::ptr_glColor3b) CL_GL1::get_proc_address("glColor3b");
	functions->color3bv = (CL_GL1Functions::ptr_glColor3bv) CL_GL1::get_proc_address("glColor3bv");
	functions->color3d = (CL_GL1Functions::ptr_glColor3d) CL_GL1::get_proc_address("glColor3d");
	functions->color3dv = (CL_GL1Functions::ptr_glColor3dv) CL_GL1::get_proc_address("glColor3dv");
	functions->color3f = (CL_GL1Functions::ptr_glColor3f) CL_GL1::get_proc_address("glColor3f");
	functions->color3fv = (CL_GL1Functions::ptr_glColor3fv) CL_GL1::get_proc_address("glColor3fv");
	functions->color3i = (CL_GL1Functions::ptr_glColor3i) CL_GL1::get_proc_address("glColor3i");
	functions->color3iv = (CL_GL1Functions::ptr_glColor3iv) CL_GL1::get_proc_address("glColor3iv");
	functions->color3s = (CL_GL1Functions::ptr_glColor3s) CL_GL1::get_proc_address("glColor3s");
	functions->color3sv = (CL_GL1Functions::ptr_glColor3sv) CL_GL1::get_proc_address("glColor3sv");
	functions->color3ub = (CL_GL1Functions::ptr_glColor3ub) CL_GL1::get_proc_address("glColor3ub");
	functions->color3ubv = (CL_GL1Functions::ptr_glColor3ubv) CL_GL1::get_proc_address("glColor3ubv");
	functions->color3ui = (CL_GL1Functions::ptr_glColor3ui) CL_GL1::get_proc_address("glColor3ui");
	functions->color3uiv = (CL_GL1Functions::ptr_glColor3uiv) CL_GL1::get_proc_address("glColor3uiv");
	functions->color3us = (CL_GL1Functions::ptr_glColor3us) CL_GL1::get_proc_address("glColor3us");
	functions->color3usv = (CL_GL1Functions::ptr_glColor3usv) CL_GL1::get_proc_address("glColor3usv");
	functions->color4b = (CL_GL1Functions::ptr_glColor4b) CL_GL1::get_proc_address("glColor4b");
	functions->color4bv = (CL_GL1Functions::ptr_glColor4bv) CL_GL1::get_proc_address("glColor4bv");
	functions->color4d = (CL_GL1Functions::ptr_glColor4d) CL_GL1::get_proc_address("glColor4d");
	functions->color4dv = (CL_GL1Functions::ptr_glColor4dv) CL_GL1::get_proc_address("glColor4dv");
	functions->color4f = (CL_GL1Functions::ptr_glColor4f) CL_GL1::get_proc_address("glColor4f");
	functions->color4fv = (CL_GL1Functions::ptr_glColor4fv) CL_GL1::get_proc_address("glColor4fv");
	functions->color4i = (CL_GL1Functions::ptr_glColor4i) CL_GL1::get_proc_address("glColor4i");
	functions->color4iv = (CL_GL1Functions::ptr_glColor4iv) CL_GL1::get_proc_address("glColor4iv");
	functions->color4s = (CL_GL1Functions::ptr_glColor4s) CL_GL1::get_proc_address("glColor4s");
	functions->color4sv = (CL_GL1Functions::ptr_glColor4sv) CL_GL1::get_proc_address("glColor4sv");
	functions->color4ub = (CL_GL1Functions::ptr_glColor4ub) CL_GL1::get_proc_address("glColor4ub");
	functions->color4ubv = (CL_GL1Functions::ptr_glColor4ubv) CL_GL1::get_proc_address("glColor4ubv");
	functions->color4ui = (CL_GL1Functions::ptr_glColor4ui) CL_GL1::get_proc_address("glColor4ui");
	functions->color4uiv = (CL_GL1Functions::ptr_glColor4uiv) CL_GL1::get_proc_address("glColor4uiv");
	functions->color4us = (CL_GL1Functions::ptr_glColor4us) CL_GL1::get_proc_address("glColor4us");
	functions->color4usv = (CL_GL1Functions::ptr_glColor4usv) CL_GL1::get_proc_address("glColor4usv");
	functions->colorMask = (CL_GL1Functions::ptr_glColorMask) CL_GL1::get_proc_address("glColorMask");
	functions->colorMaterial = (CL_GL1Functions::ptr_glColorMaterial) CL_GL1::get_proc_address("glColorMaterial");
	functions->colorPointer = (CL_GL1Functions::ptr_glColorPointer) CL_GL1::get_proc_address("glColorPointer");
	functions->copyPixels = (CL_GL1Functions::ptr_glCopyPixels) CL_GL1::get_proc_address("glCopyPixels");
	functions->copyTexImage1D = (CL_GL1Functions::ptr_glCopyTexImage1D) CL_GL1::get_proc_address("glCopyTexImage1D");
	functions->copyTexImage2D = (CL_GL1Functions::ptr_glCopyTexImage2D) CL_GL1::get_proc_address("glCopyTexImage2D");
	functions->copyTexSubImage1D = (CL_GL1Functions::ptr_glCopyTexSubImage1D) CL_GL1::get_proc_address("glCopyTexSubImage1D");
	functions->copyTexSubImage2D = (CL_GL1Functions::ptr_glCopyTexSubImage2D) CL_GL1::get_proc_address("glCopyTexSubImage2D");
	functions->cullFace = (CL_GL1Functions::ptr_glCullFace) CL_GL1::get_proc_address("glCullFace");
	functions->deleteLists = (CL_GL1Functions::ptr_glDeleteLists) CL_GL1::get_proc_address("glDeleteLists");
	functions->deleteTextures = (CL_GL1Functions::ptr_glDeleteTextures) CL_GL1::get_proc_address("glDeleteTextures");
	functions->depthFunc = (CL_GL1Functions::ptr_glDepthFunc) CL_GL1::get_proc_address("glDepthFunc");
	functions->depthMask = (CL_GL1Functions::ptr_glDepthMask) CL_GL1::get_proc_address("glDepthMask");
	functions->depthRange = (CL_GL1Functions::ptr_glDepthRange) CL_GL1::get_proc_address("glDepthRange");
	functions->disable = (CL_GL1Functions::ptr_glDisable) CL_GL1::get_proc_address("glDisable");
	functions->disableClientState = (CL_GL1Functions::ptr_glDisableClientState) CL_GL1::get_proc_address("glDisableClientState");
	functions->drawArrays = (CL_GL1Functions::ptr_glDrawArrays) CL_GL1::get_proc_address("glDrawArrays");
	functions->drawBuffer = (CL_GL1Functions::ptr_glDrawBuffer) CL_GL1::get_proc_address("glDrawBuffer");
	functions->drawElements = (CL_GL1Functions::ptr_glDrawElements) CL_GL1::get_proc_address("glDrawElements");
	functions->drawPixels = (CL_GL1Functions::ptr_glDrawPixels) CL_GL1::get_proc_address("glDrawPixels");
	functions->edgeFlag = (CL_GL1Functions::ptr_glEdgeFlag) CL_GL1::get_proc_address("glEdgeFlag");
	functions->edgeFlagPointer = (CL_GL1Functions::ptr_glEdgeFlagPointer) CL_GL1::get_proc_address("glEdgeFlagPointer");
	functions->edgeFlagv = (CL_GL1Functions::ptr_glEdgeFlagv) CL_GL1::get_proc_address("glEdgeFlagv");
	functions->enable = (CL_GL1Functions::ptr_glEnable) CL_GL1::get_proc_address("glEnable");
	functions->enableClientState = (CL_GL1Functions::ptr_glEnableClientState) CL_GL1::get_proc_address("glEnableClientState");
	functions->end = (CL_GL1Functions::ptr_glEnd) CL_GL1::get_proc_address("glEnd");
	functions->endList = (CL_GL1Functions::ptr_glEndList) CL_GL1::get_proc_address("glEndList");
	functions->evalCoord1d = (CL_GL1Functions::ptr_glEvalCoord1d) CL_GL1::get_proc_address("glEvalCoord1d");
	functions->evalCoord1dv = (CL_GL1Functions::ptr_glEvalCoord1dv) CL_GL1::get_proc_address("glEvalCoord1dv");
	functions->evalCoord1f = (CL_GL1Functions::ptr_glEvalCoord1f) CL_GL1::get_proc_address("glEvalCoord1f");
	functions->evalCoord1fv = (CL_GL1Functions::ptr_glEvalCoord1fv) CL_GL1::get_proc_address("glEvalCoord1fv");
	functions->evalCoord2d = (CL_GL1Functions::ptr_glEvalCoord2d) CL_GL1::get_proc_address("glEvalCoord2d");
	functions->evalCoord2dv = (CL_GL1Functions::ptr_glEvalCoord2dv) CL_GL1::get_proc_address("glEvalCoord2dv");
	functions->evalCoord2f = (CL_GL1Functions::ptr_glEvalCoord2f) CL_GL1::get_proc_address("glEvalCoord2f");
	functions->evalCoord2fv = (CL_GL1Functions::ptr_glEvalCoord2fv) CL_GL1::get_proc_address("glEvalCoord2fv");
	functions->evalMesh1 = (CL_GL1Functions::ptr_glEvalMesh1) CL_GL1::get_proc_address("glEvalMesh1");
	functions->evalMesh2 = (CL_GL1Functions::ptr_glEvalMesh2) CL_GL1::get_proc_address("glEvalMesh2");
	functions->evalPoint1 = (CL_GL1Functions::ptr_glEvalPoint1) CL_GL1::get_proc_address("glEvalPoint1");
	functions->evalPoint2 = (CL_GL1Functions::ptr_glEvalPoint2) CL_GL1::get_proc_address("glEvalPoint2");
	functions->feedbackBuffer = (CL_GL1Functions::ptr_glFeedbackBuffer) CL_GL1::get_proc_address("glFeedbackBuffer");
	functions->finish = (CL_GL1Functions::ptr_glFinish) CL_GL1::get_proc_address("glFinish");
	functions->flush = (CL_GL1Functions::ptr_glFlush) CL_GL1::get_proc_address("glFlush");
	functions->fogf = (CL_GL1Functions::ptr_glFogf) CL_GL1::get_proc_address("glFogf");
	functions->fogfv = (CL_GL1Functions::ptr_glFogfv) CL_GL1::get_proc_address("glFogfv");
	functions->fogi = (CL_GL1Functions::ptr_glFogi) CL_GL1::get_proc_address("glFogi");
	functions->fogiv = (CL_GL1Functions::ptr_glFogiv) CL_GL1::get_proc_address("glFogiv");
	functions->frontFace = (CL_GL1Functions::ptr_glFrontFace) CL_GL1::get_proc_address("glFrontFace");
	functions->frustum = (CL_GL1Functions::ptr_glFrustum) CL_GL1::get_proc_address("glFrustum");
	functions->genLists = (CL_GL1Functions::ptr_glGenLists) CL_GL1::get_proc_address("glGenLists");
	functions->genTextures = (CL_GL1Functions::ptr_glGenTextures) CL_GL1::get_proc_address("glGenTextures");
	functions->getBooleanv = (CL_GL1Functions::ptr_glGetBooleanv) CL_GL1::get_proc_address("glGetBooleanv");
	functions->getClipPlane = (CL_GL1Functions::ptr_glGetClipPlane) CL_GL1::get_proc_address("glGetClipPlane");
	functions->getDoublev = (CL_GL1Functions::ptr_glGetDoublev) CL_GL1::get_proc_address("glGetDoublev");
	functions->getError = (CL_GL1Functions::ptr_glGetError) CL_GL1::get_proc_address("glGetError");
	functions->getFloatv = (CL_GL1Functions::ptr_glGetFloatv) CL_GL1::get_proc_address("glGetFloatv");
	functions->getIntegerv = (CL_GL1Functions::ptr_glGetIntegerv) CL_GL1::get_proc_address("glGetIntegerv");
	functions->getLightfv = (CL_GL1Functions::ptr_glGetLightfv) CL_GL1::get_proc_address("glGetLightfv");
	functions->getLightiv = (CL_GL1Functions::ptr_glGetLightiv) CL_GL1::get_proc_address("glGetLightiv");
	functions->getMapdv = (CL_GL1Functions::ptr_glGetMapdv) CL_GL1::get_proc_address("glGetMapdv");
	functions->getMapfv = (CL_GL1Functions::ptr_glGetMapfv) CL_GL1::get_proc_address("glGetMapfv");
	functions->getMapiv = (CL_GL1Functions::ptr_glGetMapiv) CL_GL1::get_proc_address("glGetMapiv");
	functions->getMaterialfv = (CL_GL1Functions::ptr_glGetMaterialfv) CL_GL1::get_proc_address("glGetMaterialfv");
	functions->getMaterialiv = (CL_GL1Functions::ptr_glGetMaterialiv) CL_GL1::get_proc_address("glGetMaterialiv");
	functions->getPixelMapfv = (CL_GL1Functions::ptr_glGetPixelMapfv) CL_GL1::get_proc_address("glGetPixelMapfv");
	functions->getPixelMapuiv = (CL_GL1Functions::ptr_glGetPixelMapuiv) CL_GL1::get_proc_address("glGetPixelMapuiv");
	functions->getPixelMapusv = (CL_GL1Functions::ptr_glGetPixelMapusv) CL_GL1::get_proc_address("glGetPixelMapusv");
	functions->getPointerv = (CL_GL1Functions::ptr_glGetPointerv) CL_GL1::get_proc_address("glGetPointerv");
	functions->getPolygonStipple = (CL_GL1Functions::ptr_glGetPolygonStipple) CL_GL1::get_proc_address("glGetPolygonStipple");
	functions->getString = (CL_GL1Functions::ptr_glGetString) CL_GL1::get_proc_address("glGetString");
	functions->getTexEnvfv = (CL_GL1Functions::ptr_glGetTexEnvfv) CL_GL1::get_proc_address("glGetTexEnvfv");
	functions->getTexEnviv = (CL_GL1Functions::ptr_glGetTexEnviv) CL_GL1::get_proc_address("glGetTexEnviv");
	functions->getTexGendv = (CL_GL1Functions::ptr_glGetTexGendv) CL_GL1::get_proc_address("glGetTexGendv");
	functions->getTexGenfv = (CL_GL1Functions::ptr_glGetTexGenfv) CL_GL1::get_proc_address("glGetTexGenfv");
	functions->getTexGeniv = (CL_GL1Functions::ptr_glGetTexGeniv) CL_GL1::get_proc_address("glGetTexGeniv");
	functions->getTexImage = (CL_GL1Functions::ptr_glGetTexImage) CL_GL1::get_proc_address("glGetTexImage");
	functions->getTexLevelParameterfv = (CL_GL1Functions::ptr_glGetTexLevelParameterfv) CL_GL1::get_proc_address("glGetTexLevelParameterfv");
	functions->getTexLevelParameteriv = (CL_GL1Functions::ptr_glGetTexLevelParameteriv) CL_GL1::get_proc_address("glGetTexLevelParameteriv");
	functions->getTexParameterfv = (CL_GL1Functions::ptr_glGetTexParameterfv) CL_GL1::get_proc_address("glGetTexParameterfv");
	functions->getTexParameteriv = (CL_GL1Functions::ptr_glGetTexParameteriv) CL_GL1::get_proc_address("glGetTexParameteriv");
	functions->hint = (CL_GL1Functions::ptr_glHint) CL_GL1::get_proc_address("glHint");
	functions->indexMask = (CL_GL1Functions::ptr_glIndexMask) CL_GL1::get_proc_address("glIndexMask");
	functions->indexPointer = (CL_GL1Functions::ptr_glIndexPointer) CL_GL1::get_proc_address("glIndexPointer");
	functions->indexd = (CL_GL1Functions::ptr_glIndexd) CL_GL1::get_proc_address("glIndexd");
	functions->indexdv = (CL_GL1Functions::ptr_glIndexdv) CL_GL1::get_proc_address("glIndexdv");
	functions->indexf = (CL_GL1Functions::ptr_glIndexf) CL_GL1::get_proc_address("glIndexf");
	functions->indexfv = (CL_GL1Functions::ptr_glIndexfv) CL_GL1::get_proc_address("glIndexfv");
	functions->indexi = (CL_GL1Functions::ptr_glIndexi) CL_GL1::get_proc_address("glIndexi");
	functions->indexiv = (CL_GL1Functions::ptr_glIndexiv) CL_GL1::get_proc_address("glIndexiv");
	functions->indexs = (CL_GL1Functions::ptr_glIndexs) CL_GL1::get_proc_address("glIndexs");
	functions->indexsv = (CL_GL1Functions::ptr_glIndexsv) CL_GL1::get_proc_address("glIndexsv");
	functions->indexub = (CL_GL1Functions::ptr_glIndexub) CL_GL1::get_proc_address("glIndexub");
	functions->indexubv = (CL_GL1Functions::ptr_glIndexubv) CL_GL1::get_proc_address("glIndexubv");
	functions->initNames = (CL_GL1Functions::ptr_glInitNames) CL_GL1::get_proc_address("glInitNames");
	functions->interleavedArrays = (CL_GL1Functions::ptr_glInterleavedArrays) CL_GL1::get_proc_address("glInterleavedArrays");
	functions->isEnabled = (CL_GL1Functions::ptr_glIsEnabled) CL_GL1::get_proc_address("glIsEnabled");
	functions->isList = (CL_GL1Functions::ptr_glIsList) CL_GL1::get_proc_address("glIsList");
	functions->isTexture = (CL_GL1Functions::ptr_glIsTexture) CL_GL1::get_proc_address("glIsTexture");
	functions->lightModelf = (CL_GL1Functions::ptr_glLightModelf) CL_GL1::get_proc_address("glLightModelf");
	functions->lightModelfv = (CL_GL1Functions::ptr_glLightModelfv) CL_GL1::get_proc_address("glLightModelfv");
	functions->lightModeli = (CL_GL1Functions::ptr_glLightModeli) CL_GL1::get_proc_address("glLightModeli");
	functions->lightModeliv = (CL_GL1Functions::ptr_glLightModeliv) CL_GL1::get_proc_address("glLightModeliv");
	functions->lightf = (CL_GL1Functions::ptr_glLightf) CL_GL1::get_proc_address("glLightf");
	functions->lightfv = (CL_GL1Functions::ptr_glLightfv) CL_GL1::get_proc_address("glLightfv");
	functions->lighti = (CL_GL1Functions::ptr_glLighti) CL_GL1::get_proc_address("glLighti");
	functions->lightiv = (CL_GL1Functions::ptr_glLightiv) CL_GL1::get_proc_address("glLightiv");
	functions->lineStipple = (CL_GL1Functions::ptr_glLineStipple) CL_GL1::get_proc_address("glLineStipple");
	functions->lineWidth = (CL_GL1Functions::ptr_glLineWidth) CL_GL1::get_proc_address("glLineWidth");
	functions->listBase = (CL_GL1Functions::ptr_glListBase) CL_GL1::get_proc_address("glListBase");
	functions->loadIdentity = (CL_GL1Functions::ptr_glLoadIdentity) CL_GL1::get_proc_address("glLoadIdentity");
	functions->loadMatrixd = (CL_GL1Functions::ptr_glLoadMatrixd) CL_GL1::get_proc_address("glLoadMatrixd");
	functions->loadMatrixf = (CL_GL1Functions::ptr_glLoadMatrixf) CL_GL1::get_proc_address("glLoadMatrixf");
	functions->loadName = (CL_GL1Functions::ptr_glLoadName) CL_GL1::get_proc_address("glLoadName");
	functions->logicOp = (CL_GL1Functions::ptr_glLogicOp) CL_GL1::get_proc_address("glLogicOp");
	functions->map1d = (CL_GL1Functions::ptr_glMap1d) CL_GL1::get_proc_address("glMap1d");
	functions->map1f = (CL_GL1Functions::ptr_glMap1f) CL_GL1::get_proc_address("glMap1f");
	functions->map2d = (CL_GL1Functions::ptr_glMap2d) CL_GL1::get_proc_address("glMap2d");
	functions->map2f = (CL_GL1Functions::ptr_glMap2f) CL_GL1::get_proc_address("glMap2f");
	functions->mapGrid1d = (CL_GL1Functions::ptr_glMapGrid1d) CL_GL1::get_proc_address("glMapGrid1d");
	functions->mapGrid1f = (CL_GL1Functions::ptr_glMapGrid1f) CL_GL1::get_proc_address("glMapGrid1f");
	functions->mapGrid2d = (CL_GL1Functions::ptr_glMapGrid2d) CL_GL1::get_proc_address("glMapGrid2d");
	functions->mapGrid2f = (CL_GL1Functions::ptr_glMapGrid2f) CL_GL1::get_proc_address("glMapGrid2f");
	functions->materialf = (CL_GL1Functions::ptr_glMaterialf) CL_GL1::get_proc_address("glMaterialf");
	functions->materialfv = (CL_GL1Functions::ptr_glMaterialfv) CL_GL1::get_proc_address("glMaterialfv");
	functions->materiali = (CL_GL1Functions::ptr_glMateriali) CL_GL1::get_proc_address("glMateriali");
	functions->materialiv = (CL_GL1Functions::ptr_glMaterialiv) CL_GL1::get_proc_address("glMaterialiv");
	functions->matrixMode = (CL_GL1Functions::ptr_glMatrixMode) CL_GL1::get_proc_address("glMatrixMode");
	functions->multMatrixd = (CL_GL1Functions::ptr_glMultMatrixd) CL_GL1::get_proc_address("glMultMatrixd");
	functions->multMatrixf = (CL_GL1Functions::ptr_glMultMatrixf) CL_GL1::get_proc_address("glMultMatrixf");
	functions->newList = (CL_GL1Functions::ptr_glNewList) CL_GL1::get_proc_address("glNewList");
	functions->normal3b = (CL_GL1Functions::ptr_glNormal3b) CL_GL1::get_proc_address("glNormal3b");
	functions->normal3bv = (CL_GL1Functions::ptr_glNormal3bv) CL_GL1::get_proc_address("glNormal3bv");
	functions->normal3d = (CL_GL1Functions::ptr_glNormal3d) CL_GL1::get_proc_address("glNormal3d");
	functions->normal3dv = (CL_GL1Functions::ptr_glNormal3dv) CL_GL1::get_proc_address("glNormal3dv");
	functions->normal3f = (CL_GL1Functions::ptr_glNormal3f) CL_GL1::get_proc_address("glNormal3f");
	functions->normal3fv = (CL_GL1Functions::ptr_glNormal3fv) CL_GL1::get_proc_address("glNormal3fv");
	functions->normal3i = (CL_GL1Functions::ptr_glNormal3i) CL_GL1::get_proc_address("glNormal3i");
	functions->normal3iv = (CL_GL1Functions::ptr_glNormal3iv) CL_GL1::get_proc_address("glNormal3iv");
	functions->normal3s = (CL_GL1Functions::ptr_glNormal3s) CL_GL1::get_proc_address("glNormal3s");
	functions->normal3sv = (CL_GL1Functions::ptr_glNormal3sv) CL_GL1::get_proc_address("glNormal3sv");
	functions->normalPointer = (CL_GL1Functions::ptr_glNormalPointer) CL_GL1::get_proc_address("glNormalPointer");
	functions->ortho = (CL_GL1Functions::ptr_glOrtho) CL_GL1::get_proc_address("glOrtho");
	functions->passThrough = (CL_GL1Functions::ptr_glPassThrough) CL_GL1::get_proc_address("glPassThrough");
	functions->pixelMapfv = (CL_GL1Functions::ptr_glPixelMapfv) CL_GL1::get_proc_address("glPixelMapfv");
	functions->pixelMapuiv = (CL_GL1Functions::ptr_glPixelMapuiv) CL_GL1::get_proc_address("glPixelMapuiv");
	functions->pixelMapusv = (CL_GL1Functions::ptr_glPixelMapusv) CL_GL1::get_proc_address("glPixelMapusv");
	functions->pixelStoref = (CL_GL1Functions::ptr_glPixelStoref) CL_GL1::get_proc_address("glPixelStoref");
	functions->pixelStorei = (CL_GL1Functions::ptr_glPixelStorei) CL_GL1::get_proc_address("glPixelStorei");
	functions->pixelTransferf = (CL_GL1Functions::ptr_glPixelTransferf) CL_GL1::get_proc_address("glPixelTransferf");
	functions->pixelTransferi = (CL_GL1Functions::ptr_glPixelTransferi) CL_GL1::get_proc_address("glPixelTransferi");
	functions->pixelZoom = (CL_GL1Functions::ptr_glPixelZoom) CL_GL1::get_proc_address("glPixelZoom");
	functions->pointSize = (CL_GL1Functions::ptr_glPointSize) CL_GL1::get_proc_address("glPointSize");
	functions->polygonMode = (CL_GL1Functions::ptr_glPolygonMode) CL_GL1::get_proc_address("glPolygonMode");
	functions->polygonOffset = (CL_GL1Functions::ptr_glPolygonOffset) CL_GL1::get_proc_address("glPolygonOffset");
	functions->polygonStipple = (CL_GL1Functions::ptr_glPolygonStipple) CL_GL1::get_proc_address("glPolygonStipple");
	functions->popAttrib = (CL_GL1Functions::ptr_glPopAttrib) CL_GL1::get_proc_address("glPopAttrib");
	functions->popClientAttrib = (CL_GL1Functions::ptr_glPopClientAttrib) CL_GL1::get_proc_address("glPopClientAttrib");
	functions->popMatrix = (CL_GL1Functions::ptr_glPopMatrix) CL_GL1::get_proc_address("glPopMatrix");
	functions->popName = (CL_GL1Functions::ptr_glPopName) CL_GL1::get_proc_address("glPopName");
	functions->prioritizeTextures = (CL_GL1Functions::ptr_glPrioritizeTextures) CL_GL1::get_proc_address("glPrioritizeTextures");
	functions->pushAttrib = (CL_GL1Functions::ptr_glPushAttrib) CL_GL1::get_proc_address("glPushAttrib");
	functions->pushClientAttrib = (CL_GL1Functions::ptr_glPushClientAttrib) CL_GL1::get_proc_address("glPushClientAttrib");
	functions->pushMatrix = (CL_GL1Functions::ptr_glPushMatrix) CL_GL1::get_proc_address("glPushMatrix");
	functions->pushName = (CL_GL1Functions::ptr_glPushName) CL_GL1::get_proc_address("glPushName");
	functions->rasterPos2d = (CL_GL1Functions::ptr_glRasterPos2d) CL_GL1::get_proc_address("glRasterPos2d");
	functions->rasterPos2dv = (CL_GL1Functions::ptr_glRasterPos2dv) CL_GL1::get_proc_address("glRasterPos2dv");
	functions->rasterPos2f = (CL_GL1Functions::ptr_glRasterPos2f) CL_GL1::get_proc_address("glRasterPos2f");
	functions->rasterPos2fv = (CL_GL1Functions::ptr_glRasterPos2fv) CL_GL1::get_proc_address("glRasterPos2fv");
	functions->rasterPos2i = (CL_GL1Functions::ptr_glRasterPos2i) CL_GL1::get_proc_address("glRasterPos2i");
	functions->rasterPos2iv = (CL_GL1Functions::ptr_glRasterPos2iv) CL_GL1::get_proc_address("glRasterPos2iv");
	functions->rasterPos2s = (CL_GL1Functions::ptr_glRasterPos2s) CL_GL1::get_proc_address("glRasterPos2s");
	functions->rasterPos2sv = (CL_GL1Functions::ptr_glRasterPos2sv) CL_GL1::get_proc_address("glRasterPos2sv");
	functions->rasterPos3d = (CL_GL1Functions::ptr_glRasterPos3d) CL_GL1::get_proc_address("glRasterPos3d");
	functions->rasterPos3dv = (CL_GL1Functions::ptr_glRasterPos3dv) CL_GL1::get_proc_address("glRasterPos3dv");
	functions->rasterPos3f = (CL_GL1Functions::ptr_glRasterPos3f) CL_GL1::get_proc_address("glRasterPos3f");
	functions->rasterPos3fv = (CL_GL1Functions::ptr_glRasterPos3fv) CL_GL1::get_proc_address("glRasterPos3fv");
	functions->rasterPos3i = (CL_GL1Functions::ptr_glRasterPos3i) CL_GL1::get_proc_address("glRasterPos3i");
	functions->rasterPos3iv = (CL_GL1Functions::ptr_glRasterPos3iv) CL_GL1::get_proc_address("glRasterPos3iv");
	functions->rasterPos3s = (CL_GL1Functions::ptr_glRasterPos3s) CL_GL1::get_proc_address("glRasterPos3s");
	functions->rasterPos3sv = (CL_GL1Functions::ptr_glRasterPos3sv) CL_GL1::get_proc_address("glRasterPos3sv");
	functions->rasterPos4d = (CL_GL1Functions::ptr_glRasterPos4d) CL_GL1::get_proc_address("glRasterPos4d");
	functions->rasterPos4dv = (CL_GL1Functions::ptr_glRasterPos4dv) CL_GL1::get_proc_address("glRasterPos4dv");
	functions->rasterPos4f = (CL_GL1Functions::ptr_glRasterPos4f) CL_GL1::get_proc_address("glRasterPos4f");
	functions->rasterPos4fv = (CL_GL1Functions::ptr_glRasterPos4fv) CL_GL1::get_proc_address("glRasterPos4fv");
	functions->rasterPos4i = (CL_GL1Functions::ptr_glRasterPos4i) CL_GL1::get_proc_address("glRasterPos4i");
	functions->rasterPos4iv = (CL_GL1Functions::ptr_glRasterPos4iv) CL_GL1::get_proc_address("glRasterPos4iv");
	functions->rasterPos4s = (CL_GL1Functions::ptr_glRasterPos4s) CL_GL1::get_proc_address("glRasterPos4s");
	functions->rasterPos4sv = (CL_GL1Functions::ptr_glRasterPos4sv) CL_GL1::get_proc_address("glRasterPos4sv");
	functions->readBuffer = (CL_GL1Functions::ptr_glReadBuffer) CL_GL1::get_proc_address("glReadBuffer");
	functions->readPixels = (CL_GL1Functions::ptr_glReadPixels) CL_GL1::get_proc_address("glReadPixels");
	functions->rectd = (CL_GL1Functions::ptr_glRectd) CL_GL1::get_proc_address("glRectd");
	functions->rectdv = (CL_GL1Functions::ptr_glRectdv) CL_GL1::get_proc_address("glRectdv");
	functions->rectf = (CL_GL1Functions::ptr_glRectf) CL_GL1::get_proc_address("glRectf");
	functions->rectfv = (CL_GL1Functions::ptr_glRectfv) CL_GL1::get_proc_address("glRectfv");
	functions->recti = (CL_GL1Functions::ptr_glRecti) CL_GL1::get_proc_address("glRecti");
	functions->rectiv = (CL_GL1Functions::ptr_glRectiv) CL_GL1::get_proc_address("glRectiv");
	functions->rects = (CL_GL1Functions::ptr_glRects) CL_GL1::get_proc_address("glRects");
	functions->rectsv = (CL_GL1Functions::ptr_glRectsv) CL_GL1::get_proc_address("glRectsv");
	functions->renderMode = (CL_GL1Functions::ptr_glRenderMode) CL_GL1::get_proc_address("glRenderMode");
	functions->rotated = (CL_GL1Functions::ptr_glRotated) CL_GL1::get_proc_address("glRotated");
	functions->rotatef = (CL_GL1Functions::ptr_glRotatef) CL_GL1::get_proc_address("glRotatef");
	functions->scaled = (CL_GL1Functions::ptr_glScaled) CL_GL1::get_proc_address("glScaled");
	functions->scalef = (CL_GL1Functions::ptr_glScalef) CL_GL1::get_proc_address("glScalef");
	functions->scissor = (CL_GL1Functions::ptr_glScissor) CL_GL1::get_proc_address("glScissor");
	functions->selectBuffer = (CL_GL1Functions::ptr_glSelectBuffer) CL_GL1::get_proc_address("glSelectBuffer");
	functions->shadeModel = (CL_GL1Functions::ptr_glShadeModel) CL_GL1::get_proc_address("glShadeModel");
	functions->stencilFunc = (CL_GL1Functions::ptr_glStencilFunc) CL_GL1::get_proc_address("glStencilFunc");
	functions->stencilMask = (CL_GL1Functions::ptr_glStencilMask) CL_GL1::get_proc_address("glStencilMask");
	functions->stencilMaskSeparate = (CL_GL1Functions::ptr_glStencilMaskSeparate) CL_GL1::get_proc_address("glStencilMaskSeparate");
	functions->stencilOp = (CL_GL1Functions::ptr_glStencilOp) CL_GL1::get_proc_address("glStencilOp");
	functions->texCoord1d = (CL_GL1Functions::ptr_glTexCoord1d) CL_GL1::get_proc_address("glTexCoord1d");
	functions->texCoord1dv = (CL_GL1Functions::ptr_glTexCoord1dv) CL_GL1::get_proc_address("glTexCoord1dv");
	functions->texCoord1f = (CL_GL1Functions::ptr_glTexCoord1f) CL_GL1::get_proc_address("glTexCoord1f");
	functions->texCoord1fv = (CL_GL1Functions::ptr_glTexCoord1fv) CL_GL1::get_proc_address("glTexCoord1fv");
	functions->texCoord1i = (CL_GL1Functions::ptr_glTexCoord1i) CL_GL1::get_proc_address("glTexCoord1i");
	functions->texCoord1iv = (CL_GL1Functions::ptr_glTexCoord1iv) CL_GL1::get_proc_address("glTexCoord1iv");
	functions->texCoord1s = (CL_GL1Functions::ptr_glTexCoord1s) CL_GL1::get_proc_address("glTexCoord1s");
	functions->texCoord1sv = (CL_GL1Functions::ptr_glTexCoord1sv) CL_GL1::get_proc_address("glTexCoord1sv");
	functions->texCoord2d = (CL_GL1Functions::ptr_glTexCoord2d) CL_GL1::get_proc_address("glTexCoord2d");
	functions->texCoord2dv = (CL_GL1Functions::ptr_glTexCoord2dv) CL_GL1::get_proc_address("glTexCoord2dv");
	functions->texCoord2f = (CL_GL1Functions::ptr_glTexCoord2f) CL_GL1::get_proc_address("glTexCoord2f");
	functions->texCoord2fv = (CL_GL1Functions::ptr_glTexCoord2fv) CL_GL1::get_proc_address("glTexCoord2fv");
	functions->texCoord2i = (CL_GL1Functions::ptr_glTexCoord2i) CL_GL1::get_proc_address("glTexCoord2i");
	functions->texCoord2iv = (CL_GL1Functions::ptr_glTexCoord2iv) CL_GL1::get_proc_address("glTexCoord2iv");
	functions->texCoord2s = (CL_GL1Functions::ptr_glTexCoord2s) CL_GL1::get_proc_address("glTexCoord2s");
	functions->texCoord2sv = (CL_GL1Functions::ptr_glTexCoord2sv) CL_GL1::get_proc_address("glTexCoord2sv");
	functions->texCoord3d = (CL_GL1Functions::ptr_glTexCoord3d) CL_GL1::get_proc_address("glTexCoord3d");
	functions->texCoord3dv = (CL_GL1Functions::ptr_glTexCoord3dv) CL_GL1::get_proc_address("glTexCoord3dv");
	functions->texCoord3f = (CL_GL1Functions::ptr_glTexCoord3f) CL_GL1::get_proc_address("glTexCoord3f");
	functions->texCoord3fv = (CL_GL1Functions::ptr_glTexCoord3fv) CL_GL1::get_proc_address("glTexCoord3fv");
	functions->texCoord3i = (CL_GL1Functions::ptr_glTexCoord3i) CL_GL1::get_proc_address("glTexCoord3i");
	functions->texCoord3iv = (CL_GL1Functions::ptr_glTexCoord3iv) CL_GL1::get_proc_address("glTexCoord3iv");
	functions->texCoord3s = (CL_GL1Functions::ptr_glTexCoord3s) CL_GL1::get_proc_address("glTexCoord3s");
	functions->texCoord3sv = (CL_GL1Functions::ptr_glTexCoord3sv) CL_GL1::get_proc_address("glTexCoord3sv");
	functions->texCoord4d = (CL_GL1Functions::ptr_glTexCoord4d) CL_GL1::get_proc_address("glTexCoord4d");
	functions->texCoord4dv = (CL_GL1Functions::ptr_glTexCoord4dv) CL_GL1::get_proc_address("glTexCoord4dv");
	functions->texCoord4f = (CL_GL1Functions::ptr_glTexCoord4f) CL_GL1::get_proc_address("glTexCoord4f");
	functions->texCoord4fv = (CL_GL1Functions::ptr_glTexCoord4fv) CL_GL1::get_proc_address("glTexCoord4fv");
	functions->texCoord4i = (CL_GL1Functions::ptr_glTexCoord4i) CL_GL1::get_proc_address("glTexCoord4i");
	functions->texCoord4iv = (CL_GL1Functions::ptr_glTexCoord4iv) CL_GL1::get_proc_address("glTexCoord4iv");
	functions->texCoord4s = (CL_GL1Functions::ptr_glTexCoord4s) CL_GL1::get_proc_address("glTexCoord4s");
	functions->texCoord4sv = (CL_GL1Functions::ptr_glTexCoord4sv) CL_GL1::get_proc_address("glTexCoord4sv");
	functions->texCoordPointer = (CL_GL1Functions::ptr_glTexCoordPointer) CL_GL1::get_proc_address("glTexCoordPointer");
	functions->texEnvf = (CL_GL1Functions::ptr_glTexEnvf) CL_GL1::get_proc_address("glTexEnvf");
	functions->texEnvfv = (CL_GL1Functions::ptr_glTexEnvfv) CL_GL1::get_proc_address("glTexEnvfv");
	functions->texEnvi = (CL_GL1Functions::ptr_glTexEnvi) CL_GL1::get_proc_address("glTexEnvi");
	functions->texEnviv = (CL_GL1Functions::ptr_glTexEnviv) CL_GL1::get_proc_address("glTexEnviv");
	functions->texGend = (CL_GL1Functions::ptr_glTexGend) CL_GL1::get_proc_address("glTexGend");
	functions->texGendv = (CL_GL1Functions::ptr_glTexGendv) CL_GL1::get_proc_address("glTexGendv");
	functions->texGenf = (CL_GL1Functions::ptr_glTexGenf) CL_GL1::get_proc_address("glTexGenf");
	functions->texGenfv = (CL_GL1Functions::ptr_glTexGenfv) CL_GL1::get_proc_address("glTexGenfv");
	functions->texGeni = (CL_GL1Functions::ptr_glTexGeni) CL_GL1::get_proc_address("glTexGeni");
	functions->texGeniv = (CL_GL1Functions::ptr_glTexGeniv) CL_GL1::get_proc_address("glTexGeniv");
	functions->texImage1D = (CL_GL1Functions::ptr_glTexImage1D) CL_GL1::get_proc_address("glTexImage1D");
	functions->texImage2D = (CL_GL1Functions::ptr_glTexImage2D) CL_GL1::get_proc_address("glTexImage2D");
	functions->texParameterf = (CL_GL1Functions::ptr_glTexParameterf) CL_GL1::get_proc_address("glTexParameterf");
	functions->texParameterfv = (CL_GL1Functions::ptr_glTexParameterfv) CL_GL1::get_proc_address("glTexParameterfv");
	functions->texParameteri = (CL_GL1Functions::ptr_glTexParameteri) CL_GL1::get_proc_address("glTexParameteri");
	functions->texParameteriv = (CL_GL1Functions::ptr_glTexParameteriv) CL_GL1::get_proc_address("glTexParameteriv");
	functions->texSubImage1D = (CL_GL1Functions::ptr_glTexSubImage1D) CL_GL1::get_proc_address("glTexSubImage1D");
	functions->texSubImage2D = (CL_GL1Functions::ptr_glTexSubImage2D) CL_GL1::get_proc_address("glTexSubImage2D");
	functions->translated = (CL_GL1Functions::ptr_glTranslated) CL_GL1::get_proc_address("glTranslated");
	functions->translatef = (CL_GL1Functions::ptr_glTranslatef) CL_GL1::get_proc_address("glTranslatef");
	functions->vertex2d = (CL_GL1Functions::ptr_glVertex2d) CL_GL1::get_proc_address("glVertex2d");
	functions->vertex2dv = (CL_GL1Functions::ptr_glVertex2dv) CL_GL1::get_proc_address("glVertex2dv");
	functions->vertex2f = (CL_GL1Functions::ptr_glVertex2f) CL_GL1::get_proc_address("glVertex2f");
	functions->vertex2fv = (CL_GL1Functions::ptr_glVertex2fv) CL_GL1::get_proc_address("glVertex2fv");
	functions->vertex2i = (CL_GL1Functions::ptr_glVertex2i) CL_GL1::get_proc_address("glVertex2i");
	functions->vertex2iv = (CL_GL1Functions::ptr_glVertex2iv) CL_GL1::get_proc_address("glVertex2iv");
	functions->vertex2s = (CL_GL1Functions::ptr_glVertex2s) CL_GL1::get_proc_address("glVertex2s");
	functions->vertex2sv = (CL_GL1Functions::ptr_glVertex2sv) CL_GL1::get_proc_address("glVertex2sv");
	functions->vertex3d = (CL_GL1Functions::ptr_glVertex3d) CL_GL1::get_proc_address("glVertex3d");
	functions->vertex3dv = (CL_GL1Functions::ptr_glVertex3dv) CL_GL1::get_proc_address("glVertex3dv");
	functions->vertex3f = (CL_GL1Functions::ptr_glVertex3f) CL_GL1::get_proc_address("glVertex3f");
	functions->vertex3fv = (CL_GL1Functions::ptr_glVertex3fv) CL_GL1::get_proc_address("glVertex3fv");
	functions->vertex3i = (CL_GL1Functions::ptr_glVertex3i) CL_GL1::get_proc_address("glVertex3i");
	functions->vertex3iv = (CL_GL1Functions::ptr_glVertex3iv) CL_GL1::get_proc_address("glVertex3iv");
	functions->vertex3s = (CL_GL1Functions::ptr_glVertex3s) CL_GL1::get_proc_address("glVertex3s");
	functions->vertex3sv = (CL_GL1Functions::ptr_glVertex3sv) CL_GL1::get_proc_address("glVertex3sv");
	functions->vertex4d = (CL_GL1Functions::ptr_glVertex4d) CL_GL1::get_proc_address("glVertex4d");
	functions->vertex4dv = (CL_GL1Functions::ptr_glVertex4dv) CL_GL1::get_proc_address("glVertex4dv");
	functions->vertex4f = (CL_GL1Functions::ptr_glVertex4f) CL_GL1::get_proc_address("glVertex4f");
	functions->vertex4fv = (CL_GL1Functions::ptr_glVertex4fv) CL_GL1::get_proc_address("glVertex4fv");
	functions->vertex4i = (CL_GL1Functions::ptr_glVertex4i) CL_GL1::get_proc_address("glVertex4i");
	functions->vertex4iv = (CL_GL1Functions::ptr_glVertex4iv) CL_GL1::get_proc_address("glVertex4iv");
	functions->vertex4s = (CL_GL1Functions::ptr_glVertex4s) CL_GL1::get_proc_address("glVertex4s");
	functions->vertex4sv = (CL_GL1Functions::ptr_glVertex4sv) CL_GL1::get_proc_address("glVertex4sv");
	functions->vertexPointer = (CL_GL1Functions::ptr_glVertexPointer) CL_GL1::get_proc_address("glVertexPointer");
	functions->viewport = (CL_GL1Functions::ptr_glViewport) CL_GL1::get_proc_address("glViewport");
#endif

	// Binds for OpenGL 1.2:

#ifdef __APPLE__
	functions->drawRangeElements = (CL_GL1Functions::ptr_glDrawRangeElementsEXT) &glDrawRangeElements;
	functions->texImage3D = (CL_GL1Functions::ptr_glTexImage3DEXT) &glTexImage3D;
	functions->texSubImage3D = (CL_GL1Functions::ptr_glTexSubImage3DEXT) &glTexSubImage3D;
	functions->copyTexSubImage3D = (CL_GL1Functions::ptr_glCopyTexSubImage3DEXT) &glCopyTexSubImage3D;
	functions->colorTable = (CL_GL1Functions::ptr_glColorTableSGI) &glColorTable;
	functions->copyColorTable = (CL_GL1Functions::ptr_glCopyColorTableSGI) &glCopyColorTable;
	functions->colorTableParameteriv = (CL_GL1Functions::ptr_glColorTableParameterivSGI) &glColorTableParameteriv;
	functions->colorTableParameterfv = (CL_GL1Functions::ptr_glColorTableParameterfvSGI) &glColorTableParameterfv;
	functions->getColorTable = (CL_GL1Functions::ptr_glGetColorTableSGI) &glGetColorTable;
	functions->getColorTableParameteriv = (CL_GL1Functions::ptr_glGetColorTableParameterivSGI) &glGetColorTableParameteriv;
	functions->getColorTableParameterfv = (CL_GL1Functions::ptr_glGetColorTableParameterfvSGI) &glGetColorTableParameterfv;
	functions->colorSubTable = (CL_GL1Functions::ptr_glColorSubTableEXT) &glColorSubTable;
	functions->copyColorSubTable = (CL_GL1Functions::ptr_glCopyColorSubTableEXT) &glCopyColorSubTable;
	functions->convolutionFilter1D = (CL_GL1Functions::ptr_glConvolutionFilter1DEXT) &glConvolutionFilter1D;
	functions->convolutionFilter2D = (CL_GL1Functions::ptr_glConvolutionFilter2DEXT) &glConvolutionFilter2D;
	functions->copyConvolutionFilter1D = (CL_GL1Functions::ptr_glCopyConvolutionFilter1DEXT) &glCopyConvolutionFilter1D;
	functions->copyConvolutionFilter2D = (CL_GL1Functions::ptr_glCopyConvolutionFilter2DEXT) &glCopyConvolutionFilter2D;
	functions->getConvolutionFilter = (CL_GL1Functions::ptr_glGetConvolutionFilterEXT) &glGetConvolutionFilter;
	functions->separableFilter2D = (CL_GL1Functions::ptr_glSeparableFilter2DEXT) &glSeparableFilter2D;
	functions->getSeparableFilter = (CL_GL1Functions::ptr_glGetSeparableFilterEXT) &glGetSeparableFilter;
	functions->convolutionParameteri = (CL_GL1Functions::ptr_glConvolutionParameteriEXT) &glConvolutionParameteri;
	functions->convolutionParameteriv = (CL_GL1Functions::ptr_glConvolutionParameterivEXT) &glConvolutionParameteriv;
	functions->convolutionParameterf = (CL_GL1Functions::ptr_glConvolutionParameterfEXT) &glConvolutionParameterf;
	functions->convolutionParameterfv = (CL_GL1Functions::ptr_glConvolutionParameterfvEXT) &glConvolutionParameterfv;
	functions->getConvolutionParameteriv = (CL_GL1Functions::ptr_glGetConvolutionParameterivEXT) &glGetConvolutionParameteriv;
	functions->getConvolutionParameterfv = (CL_GL1Functions::ptr_glGetConvolutionParameterfvEXT) &glGetConvolutionParameterfv;
	functions->histogram = (CL_GL1Functions::ptr_glHistogramEXT) &glHistogram;
	functions->resetHistogram = (CL_GL1Functions::ptr_glResetHistogramEXT) &glResetHistogram;
	functions->getHistogram = (CL_GL1Functions::ptr_glGetHistogramEXT) &glGetHistogram;
	functions->getHistogramParameteriv = (CL_GL1Functions::ptr_glGetHistogramParameterivEXT) &glGetHistogramParameteriv;
	functions->getHistogramParameterfv = (CL_GL1Functions::ptr_glGetHistogramParameterfvEXT) &glGetHistogramParameterfv;
	functions->minmax = (CL_GL1Functions::ptr_glMinmaxEXT) &glMinmax;
	functions->resetMinmax = (CL_GL1Functions::ptr_glResetMinmaxEXT) &glResetMinmax;
	functions->getMinmax = (CL_GL1Functions::ptr_glGetMinmaxEXT) &glGetMinmax;
	functions->getMinmaxParameteriv = (CL_GL1Functions::ptr_glGetMinmaxParameterivEXT) &glGetMinmaxParameteriv;
	functions->getMinmaxParameterfv = (CL_GL1Functions::ptr_glGetMinmaxParameterfvEXT) &glGetMinmaxParameterfv;
	functions->blendColor = (CL_GL1Functions::ptr_glBlendColorEXT) &glBlendColor;
	functions->blendEquation = (CL_GL1Functions::ptr_glBlendEquationEXT) &glBlendEquation;
#else
	functions->drawRangeElements = (CL_GL1Functions::ptr_glDrawRangeElementsEXT) CL_GL1::get_proc_address("glDrawRangeElementsEXT");
	functions->texImage3D = (CL_GL1Functions::ptr_glTexImage3DEXT) CL_GL1::get_proc_address("glTexImage3DEXT");
	functions->texSubImage3D = (CL_GL1Functions::ptr_glTexSubImage3DEXT) CL_GL1::get_proc_address("glTexSubImage3DEXT");
	functions->copyTexSubImage3D = (CL_GL1Functions::ptr_glCopyTexSubImage3DEXT) CL_GL1::get_proc_address("glCopyTexSubImage3DEXT");
	functions->colorTable = (CL_GL1Functions::ptr_glColorTableSGI) CL_GL1::get_proc_address("glColorTableSGI");
	functions->copyColorTable = (CL_GL1Functions::ptr_glCopyColorTableSGI) CL_GL1::get_proc_address("glCopyColorTableSGI");
	functions->colorTableParameteriv = (CL_GL1Functions::ptr_glColorTableParameterivSGI) CL_GL1::get_proc_address("glColorTableParameterivSGI");
	functions->colorTableParameterfv = (CL_GL1Functions::ptr_glColorTableParameterfvSGI) CL_GL1::get_proc_address("glColorTableParameterfvSGI");
	functions->getColorTable = (CL_GL1Functions::ptr_glGetColorTableSGI) CL_GL1::get_proc_address("glGetColorTableSGI");
	functions->getColorTableParameteriv = (CL_GL1Functions::ptr_glGetColorTableParameterivSGI) CL_GL1::get_proc_address("glGetColorTableParameterivSGI");
	functions->getColorTableParameterfv = (CL_GL1Functions::ptr_glGetColorTableParameterfvSGI) CL_GL1::get_proc_address("glGetColorTableParameterfvSGI");
	functions->colorSubTable = (CL_GL1Functions::ptr_glColorSubTableEXT) CL_GL1::get_proc_address("glColorSubTableEXT");
	functions->copyColorSubTable = (CL_GL1Functions::ptr_glCopyColorSubTableEXT) CL_GL1::get_proc_address("glCopyColorSubTableEXT");
	functions->convolutionFilter1D = (CL_GL1Functions::ptr_glConvolutionFilter1DEXT) CL_GL1::get_proc_address("glConvolutionFilter1DEXT");
	functions->convolutionFilter2D = (CL_GL1Functions::ptr_glConvolutionFilter2DEXT) CL_GL1::get_proc_address("glConvolutionFilter2DEXT");
	functions->copyConvolutionFilter1D = (CL_GL1Functions::ptr_glCopyConvolutionFilter1DEXT) CL_GL1::get_proc_address("glCopyConvolutionFilter1DEXT");
	functions->copyConvolutionFilter2D = (CL_GL1Functions::ptr_glCopyConvolutionFilter2DEXT) CL_GL1::get_proc_address("glCopyConvolutionFilter2DEXT");
	functions->getConvolutionFilter = (CL_GL1Functions::ptr_glGetConvolutionFilterEXT) CL_GL1::get_proc_address("glGetConvolutionFilterEXT");
	functions->separableFilter2D = (CL_GL1Functions::ptr_glSeparableFilter2DEXT) CL_GL1::get_proc_address("glSeparableFilter2DEXT");
	functions->getSeparableFilter = (CL_GL1Functions::ptr_glGetSeparableFilterEXT) CL_GL1::get_proc_address("glGetSeparableFilterEXT");
	functions->convolutionParameteri = (CL_GL1Functions::ptr_glConvolutionParameteriEXT) CL_GL1::get_proc_address("glConvolutionParameteriEXT");
	functions->convolutionParameteriv = (CL_GL1Functions::ptr_glConvolutionParameterivEXT) CL_GL1::get_proc_address("glConvolutionParameterivEXT");
	functions->convolutionParameterf = (CL_GL1Functions::ptr_glConvolutionParameterfEXT) CL_GL1::get_proc_address("glConvolutionParameterfEXT");
	functions->convolutionParameterfv = (CL_GL1Functions::ptr_glConvolutionParameterfvEXT) CL_GL1::get_proc_address("glConvolutionParameterfvEXT");
	functions->getConvolutionParameteriv = (CL_GL1Functions::ptr_glGetConvolutionParameterivEXT) CL_GL1::get_proc_address("glGetConvolutionParameterivEXT");
	functions->getConvolutionParameterfv = (CL_GL1Functions::ptr_glGetConvolutionParameterfvEXT) CL_GL1::get_proc_address("glGetConvolutionParameterfvEXT");
	functions->histogram = (CL_GL1Functions::ptr_glHistogramEXT) CL_GL1::get_proc_address("glHistogramEXT");
	functions->resetHistogram = (CL_GL1Functions::ptr_glResetHistogramEXT) CL_GL1::get_proc_address("glResetHistogramEXT");
	functions->getHistogram = (CL_GL1Functions::ptr_glGetHistogramEXT) CL_GL1::get_proc_address("glGetHistogramEXT");
	functions->getHistogramParameteriv = (CL_GL1Functions::ptr_glGetHistogramParameterivEXT) CL_GL1::get_proc_address("glGetHistogramParameterivEXT");
	functions->getHistogramParameterfv = (CL_GL1Functions::ptr_glGetHistogramParameterfvEXT) CL_GL1::get_proc_address("glGetHistogramParameterfvEXT");
	functions->minmax = (CL_GL1Functions::ptr_glMinmaxEXT) CL_GL1::get_proc_address("glMinmaxEXT");
	functions->resetMinmax = (CL_GL1Functions::ptr_glResetMinmaxEXT) CL_GL1::get_proc_address("glResetMinmaxEXT");
	functions->getMinmax = (CL_GL1Functions::ptr_glGetMinmaxEXT) CL_GL1::get_proc_address("glGetMinmaxEXT");
	functions->getMinmaxParameteriv = (CL_GL1Functions::ptr_glGetMinmaxParameterivEXT) CL_GL1::get_proc_address("glGetMinmaxParameterivEXT");
	functions->getMinmaxParameterfv = (CL_GL1Functions::ptr_glGetMinmaxParameterfvEXT) CL_GL1::get_proc_address("glGetMinmaxParameterfvEXT");
	functions->blendColor = (CL_GL1Functions::ptr_glBlendColorEXT) CL_GL1::get_proc_address("glBlendColorEXT");
	functions->blendEquation = (CL_GL1Functions::ptr_glBlendEquationEXT) CL_GL1::get_proc_address("glBlendEquationEXT");
#endif

	// Binds for OpenGL 1.2.1:

#ifdef __APPLE__
	functions->activeTexture = (CL_GL1Functions::ptr_glActiveTextureARB) &glActiveTexture;
	functions->clientActiveTexture = (CL_GL1Functions::ptr_glClientActiveTextureARB) &glClientActiveTexture;
	functions->multiTexCoord1d = (CL_GL1Functions::ptr_glMultiTexCoord1dARB) &glMultiTexCoord1d;
	functions->multiTexCoord1dv = (CL_GL1Functions::ptr_glMultiTexCoord1dvARB) &glMultiTexCoord1dv;
	functions->multiTexCoord1f = (CL_GL1Functions::ptr_glMultiTexCoord1fARB) &glMultiTexCoord1f;
	functions->multiTexCoord1fv = (CL_GL1Functions::ptr_glMultiTexCoord1fvARB) &glMultiTexCoord1fv;
	functions->multiTexCoord1i = (CL_GL1Functions::ptr_glMultiTexCoord1iARB) &glMultiTexCoord1i;
	functions->multiTexCoord1iv = (CL_GL1Functions::ptr_glMultiTexCoord1ivARB) &glMultiTexCoord1iv;
	functions->multiTexCoord1s = (CL_GL1Functions::ptr_glMultiTexCoord1sARB) &glMultiTexCoord1s;
	functions->multiTexCoord1sv = (CL_GL1Functions::ptr_glMultiTexCoord1svARB) &glMultiTexCoord1sv;
	functions->multiTexCoord2d = (CL_GL1Functions::ptr_glMultiTexCoord2dARB) &glMultiTexCoord2d;
	functions->multiTexCoord2dv = (CL_GL1Functions::ptr_glMultiTexCoord2dvARB) &glMultiTexCoord2dv;
	functions->multiTexCoord2f = (CL_GL1Functions::ptr_glMultiTexCoord2fARB) &glMultiTexCoord2f;
	functions->multiTexCoord2fv = (CL_GL1Functions::ptr_glMultiTexCoord2fvARB) &glMultiTexCoord2fv;
	functions->multiTexCoord2i = (CL_GL1Functions::ptr_glMultiTexCoord2iARB) &glMultiTexCoord2i;
	functions->multiTexCoord2iv = (CL_GL1Functions::ptr_glMultiTexCoord2ivARB) &glMultiTexCoord2iv;
	functions->multiTexCoord2s = (CL_GL1Functions::ptr_glMultiTexCoord2sARB) &glMultiTexCoord2s;
	functions->multiTexCoord2sv = (CL_GL1Functions::ptr_glMultiTexCoord2svARB) &glMultiTexCoord2sv;
	functions->multiTexCoord3d = (CL_GL1Functions::ptr_glMultiTexCoord3dARB) &glMultiTexCoord3d;
	functions->multiTexCoord3dv = (CL_GL1Functions::ptr_glMultiTexCoord3dvARB) &glMultiTexCoord3dv;
	functions->multiTexCoord3f = (CL_GL1Functions::ptr_glMultiTexCoord3fARB) &glMultiTexCoord3f;
	functions->multiTexCoord3fv = (CL_GL1Functions::ptr_glMultiTexCoord3fvARB) &glMultiTexCoord3fv;
	functions->multiTexCoord3i = (CL_GL1Functions::ptr_glMultiTexCoord3iARB) &glMultiTexCoord3i;
	functions->multiTexCoord3iv = (CL_GL1Functions::ptr_glMultiTexCoord3ivARB) &glMultiTexCoord3iv;
	functions->multiTexCoord3s = (CL_GL1Functions::ptr_glMultiTexCoord3sARB) &glMultiTexCoord3s;
	functions->multiTexCoord3sv = (CL_GL1Functions::ptr_glMultiTexCoord3svARB) &glMultiTexCoord3sv;
	functions->multiTexCoord4d = (CL_GL1Functions::ptr_glMultiTexCoord4dARB) &glMultiTexCoord4d;
	functions->multiTexCoord4dv = (CL_GL1Functions::ptr_glMultiTexCoord4dvARB) &glMultiTexCoord4dv;
	functions->multiTexCoord4f = (CL_GL1Functions::ptr_glMultiTexCoord4fARB) &glMultiTexCoord4f;
	functions->multiTexCoord4fv = (CL_GL1Functions::ptr_glMultiTexCoord4fvARB) &glMultiTexCoord4fv;
	functions->multiTexCoord4i = (CL_GL1Functions::ptr_glMultiTexCoord4iARB) &glMultiTexCoord4i;
	functions->multiTexCoord4iv = (CL_GL1Functions::ptr_glMultiTexCoord4ivARB) &glMultiTexCoord4iv;
	functions->multiTexCoord4s = (CL_GL1Functions::ptr_glMultiTexCoord4sARB) &glMultiTexCoord4s;
	functions->multiTexCoord4sv = (CL_GL1Functions::ptr_glMultiTexCoord4svARB) &glMultiTexCoord4sv;
#else
	functions->activeTexture = (CL_GL1Functions::ptr_glActiveTextureARB) CL_GL1::get_proc_address("glActiveTextureARB");
	functions->clientActiveTexture = (CL_GL1Functions::ptr_glClientActiveTextureARB) CL_GL1::get_proc_address("glClientActiveTextureARB");
	functions->multiTexCoord1d = (CL_GL1Functions::ptr_glMultiTexCoord1dARB) CL_GL1::get_proc_address("glMultiTexCoord1dARB");
	functions->multiTexCoord1dv = (CL_GL1Functions::ptr_glMultiTexCoord1dvARB) CL_GL1::get_proc_address("glMultiTexCoord1dvARB");
	functions->multiTexCoord1f = (CL_GL1Functions::ptr_glMultiTexCoord1fARB) CL_GL1::get_proc_address("glMultiTexCoord1fARB");
	functions->multiTexCoord1fv = (CL_GL1Functions::ptr_glMultiTexCoord1fvARB) CL_GL1::get_proc_address("glMultiTexCoord1fvARB");
	functions->multiTexCoord1i = (CL_GL1Functions::ptr_glMultiTexCoord1iARB) CL_GL1::get_proc_address("glMultiTexCoord1iARB");
	functions->multiTexCoord1iv = (CL_GL1Functions::ptr_glMultiTexCoord1ivARB) CL_GL1::get_proc_address("glMultiTexCoord1ivARB");
	functions->multiTexCoord1s = (CL_GL1Functions::ptr_glMultiTexCoord1sARB) CL_GL1::get_proc_address("glMultiTexCoord1sARB");
	functions->multiTexCoord1sv = (CL_GL1Functions::ptr_glMultiTexCoord1svARB) CL_GL1::get_proc_address("glMultiTexCoord1svARB");
	functions->multiTexCoord2d = (CL_GL1Functions::ptr_glMultiTexCoord2dARB) CL_GL1::get_proc_address("glMultiTexCoord2dARB");
	functions->multiTexCoord2dv = (CL_GL1Functions::ptr_glMultiTexCoord2dvARB) CL_GL1::get_proc_address("glMultiTexCoord2dvARB");
	functions->multiTexCoord2f = (CL_GL1Functions::ptr_glMultiTexCoord2fARB) CL_GL1::get_proc_address("glMultiTexCoord2fARB");
	functions->multiTexCoord2fv = (CL_GL1Functions::ptr_glMultiTexCoord2fvARB) CL_GL1::get_proc_address("glMultiTexCoord2fvARB");
	functions->multiTexCoord2i = (CL_GL1Functions::ptr_glMultiTexCoord2iARB) CL_GL1::get_proc_address("glMultiTexCoord2iARB");
	functions->multiTexCoord2iv = (CL_GL1Functions::ptr_glMultiTexCoord2ivARB) CL_GL1::get_proc_address("glMultiTexCoord2ivARB");
	functions->multiTexCoord2s = (CL_GL1Functions::ptr_glMultiTexCoord2sARB) CL_GL1::get_proc_address("glMultiTexCoord2sARB");
	functions->multiTexCoord2sv = (CL_GL1Functions::ptr_glMultiTexCoord2svARB) CL_GL1::get_proc_address("glMultiTexCoord2svARB");
	functions->multiTexCoord3d = (CL_GL1Functions::ptr_glMultiTexCoord3dARB) CL_GL1::get_proc_address("glMultiTexCoord3dARB");
	functions->multiTexCoord3dv = (CL_GL1Functions::ptr_glMultiTexCoord3dvARB) CL_GL1::get_proc_address("glMultiTexCoord3dvARB");
	functions->multiTexCoord3f = (CL_GL1Functions::ptr_glMultiTexCoord3fARB) CL_GL1::get_proc_address("glMultiTexCoord3fARB");
	functions->multiTexCoord3fv = (CL_GL1Functions::ptr_glMultiTexCoord3fvARB) CL_GL1::get_proc_address("glMultiTexCoord3fvARB");
	functions->multiTexCoord3i = (CL_GL1Functions::ptr_glMultiTexCoord3iARB) CL_GL1::get_proc_address("glMultiTexCoord3iARB");
	functions->multiTexCoord3iv = (CL_GL1Functions::ptr_glMultiTexCoord3ivARB) CL_GL1::get_proc_address("glMultiTexCoord3ivARB");
	functions->multiTexCoord3s = (CL_GL1Functions::ptr_glMultiTexCoord3sARB) CL_GL1::get_proc_address("glMultiTexCoord3sARB");
	functions->multiTexCoord3sv = (CL_GL1Functions::ptr_glMultiTexCoord3svARB) CL_GL1::get_proc_address("glMultiTexCoord3svARB");
	functions->multiTexCoord4d = (CL_GL1Functions::ptr_glMultiTexCoord4dARB) CL_GL1::get_proc_address("glMultiTexCoord4dARB");
	functions->multiTexCoord4dv = (CL_GL1Functions::ptr_glMultiTexCoord4dvARB) CL_GL1::get_proc_address("glMultiTexCoord4dvARB");
	functions->multiTexCoord4f = (CL_GL1Functions::ptr_glMultiTexCoord4fARB) CL_GL1::get_proc_address("glMultiTexCoord4fARB");
	functions->multiTexCoord4fv = (CL_GL1Functions::ptr_glMultiTexCoord4fvARB) CL_GL1::get_proc_address("glMultiTexCoord4fvARB");
	functions->multiTexCoord4i = (CL_GL1Functions::ptr_glMultiTexCoord4iARB) CL_GL1::get_proc_address("glMultiTexCoord4iARB");
	functions->multiTexCoord4iv = (CL_GL1Functions::ptr_glMultiTexCoord4ivARB) CL_GL1::get_proc_address("glMultiTexCoord4ivARB");
	functions->multiTexCoord4s = (CL_GL1Functions::ptr_glMultiTexCoord4sARB) CL_GL1::get_proc_address("glMultiTexCoord4sARB");
	functions->multiTexCoord4sv = (CL_GL1Functions::ptr_glMultiTexCoord4svARB) CL_GL1::get_proc_address("glMultiTexCoord4svARB");
#endif

	// Binds for OpenGL 1.3:

#ifdef __APPLE__
	functions->compressedTexImage1D = (CL_GL1Functions::ptr_glCompressedTexImage1DARB) &glCompressedTexImage1D;
	functions->compressedTexImage2D = (CL_GL1Functions::ptr_glCompressedTexImage2DARB) &glCompressedTexImage2D;
	functions->compressedTexImage3D = (CL_GL1Functions::ptr_glCompressedTexImage3DARB) &glCompressedTexImage3D;
	functions->compressedTexSubImage1D = (CL_GL1Functions::ptr_glCompressedTexSubImage1DARB) &glCompressedTexSubImage1D;
	functions->compressedTexSubImage2D = (CL_GL1Functions::ptr_glCompressedTexSubImage2DARB) &glCompressedTexSubImage2D;
	functions->compressedTexSubImage3D = (CL_GL1Functions::ptr_glCompressedTexSubImage3DARB) &glCompressedTexSubImage3D;
	functions->getCompressedTexImage = (CL_GL1Functions::ptr_glGetCompressedTexImageARB) &glGetCompressedTexImage;
	functions->sampleCoverage = (CL_GL1Functions::ptr_glSampleCoverageARB) &glSampleCoverage;
	functions->glLoadTransposeMatrixd = (CL_GL1Functions::ptr_glLoadTransposeMatrixdARB) &glLoadTransposeMatrixd;
	functions->glLoadTransposeMatrixf = (CL_GL1Functions::ptr_glLoadTransposeMatrixfARB) &glLoadTransposeMatrixf;
	functions->glMultTransposeMatrixd = (CL_GL1Functions::ptr_glMultTransposeMatrixdARB) &glMultTransposeMatrixd;
	functions->glMultTransposeMatrixf = (CL_GL1Functions::ptr_glMultTransposeMatrixfARB) &glMultTransposeMatrixf;
#else
	functions->compressedTexImage1D = (CL_GL1Functions::ptr_glCompressedTexImage1DARB) CL_GL1::get_proc_address("glCompressedTexImage1DARB");
	functions->compressedTexImage2D = (CL_GL1Functions::ptr_glCompressedTexImage2DARB) CL_GL1::get_proc_address("glCompressedTexImage2DARB");
	functions->compressedTexImage3D = (CL_GL1Functions::ptr_glCompressedTexImage3DARB) CL_GL1::get_proc_address("glCompressedTexImage3DARB");
	functions->compressedTexSubImage1D = (CL_GL1Functions::ptr_glCompressedTexSubImage1DARB) CL_GL1::get_proc_address("glCompressedTexSubImage1DARB");
	functions->compressedTexSubImage2D = (CL_GL1Functions::ptr_glCompressedTexSubImage2DARB) CL_GL1::get_proc_address("glCompressedTexSubImage2DARB");
	functions->compressedTexSubImage3D = (CL_GL1Functions::ptr_glCompressedTexSubImage3DARB) CL_GL1::get_proc_address("glCompressedTexSubImage3DARB");
	functions->getCompressedTexImage = (CL_GL1Functions::ptr_glGetCompressedTexImageARB) CL_GL1::get_proc_address("glGetCompressedTexImageARB");
	functions->sampleCoverage = (CL_GL1Functions::ptr_glSampleCoverageARB) CL_GL1::get_proc_address("glSampleCoverageARB");
	functions->glLoadTransposeMatrixd = (CL_GL1Functions::ptr_glLoadTransposeMatrixdARB) CL_GL1::get_proc_address("glLoadTransposeMatrixdARB");
	functions->glLoadTransposeMatrixf = (CL_GL1Functions::ptr_glLoadTransposeMatrixfARB) CL_GL1::get_proc_address("glLoadTransposeMatrixfARB");
	functions->glMultTransposeMatrixd = (CL_GL1Functions::ptr_glMultTransposeMatrixdARB) CL_GL1::get_proc_address("glMultTransposeMatrixdARB");
	functions->glMultTransposeMatrixf = (CL_GL1Functions::ptr_glMultTransposeMatrixfARB) CL_GL1::get_proc_address("glMultTransposeMatrixfARB");
#endif

#ifdef WIN32
	functions->wglCreatePbufferARB = (CL_GL1Functions::ptr_wglCreatePbufferARB) CL_GL1::get_proc_address("wglCreatePbufferARB");
	functions->wglGetPbufferDCARB = (CL_GL1Functions::ptr_wglGetPbufferDCARB) CL_GL1::get_proc_address("wglGetPbufferDCARB");
	functions->wglReleasePbufferDCARB = (CL_GL1Functions::ptr_wglReleasePbufferDCARB) CL_GL1::get_proc_address("wglReleasePbufferDCARB");
	functions->wglDestroyPbufferARB = (CL_GL1Functions::ptr_wglDestroyPbufferARB) CL_GL1::get_proc_address("wglDestroyPbufferARB");
	functions->wglQueryPbufferARB = (CL_GL1Functions::ptr_wglQueryPbufferARB) CL_GL1::get_proc_address("wglQueryPbufferARB");
#endif

	return functions;
}

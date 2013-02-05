/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "LegacyGL/precomp.h"
#include "opengl1.h"
#include "opengl1_wrap.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/mutex.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/texture.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_graphic_context_provider.h"
#include <map>

#include <GL/gl.h>

#ifndef WIN32
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#endif

namespace clan
{

cl_tls_variable GL1Functions *GL1::functions = 0;

void GL1::to_opengl_textureformat(TextureFormat format, GLint &gl_internal_format, GLenum &gl_pixel_format)
{
	switch (format)
	{
	// base internal format

	// sized internal format

		// TODO: Should this really be here?
		case tf_stencil_index1: gl_internal_format = GL_STENCIL_INDEX1; gl_pixel_format = GL_STENCIL_INDEX; break;
		case tf_stencil_index4: gl_internal_format = GL_STENCIL_INDEX4; gl_pixel_format = GL_STENCIL_INDEX; break;
		case tf_stencil_index8: gl_internal_format = GL_STENCIL_INDEX8; gl_pixel_format = GL_STENCIL_INDEX; break;
		case tf_stencil_index16: gl_internal_format = GL_STENCIL_INDEX16; gl_pixel_format = GL_STENCIL_INDEX; break;

		//case tf_r8: gl_internal_format = GL_R8; gl_pixel_format = GL_RED; break;
		//case tf_r8_snorm: gl_internal_format = GL_R8_SNORM; gl_pixel_format = GL_RED; break;
		//case tf_r16: gl_internal_format = GL_R16; gl_pixel_format = GL_RED; break;
		//case tf_r16_snorm: gl_internal_format = GL_R16_SNORM; gl_pixel_format = GL_RED; break;
		//case tf_rg8: gl_internal_format = GL_RG8; gl_pixel_format = GL_RG; break;
		//case tf_rg8_snorm: gl_internal_format = GL_RG8_SNORM; gl_pixel_format = GL_RG; break;
		//case tf_rg16: gl_internal_format = GL_RG16; gl_pixel_format = GL_RG; break;
		//case tf_rg16_snorm: gl_internal_format = GL_RG16_SNORM; gl_pixel_format = GL_RG; break;
		case tf_r3_g3_b2: gl_internal_format = GL_R3_G3_B2; gl_pixel_format = GL_RGB; break;
		case tf_rgb4: gl_internal_format = GL_RGB4; gl_pixel_format = GL_RGB; break;
		case tf_rgb5: gl_internal_format = GL_RGB5; gl_pixel_format = GL_RGB; break;
		case tf_rgb8: gl_internal_format = GL_RGB8; gl_pixel_format = GL_RGB; break;
		case tf_rgb10: gl_internal_format = GL_RGB10; gl_pixel_format = GL_RGB; break;
		case tf_rgb12: gl_internal_format = GL_RGB12; gl_pixel_format = GL_RGB; break;
		case tf_rgb16: gl_internal_format = GL_RGB16; gl_pixel_format = GL_RGB; break;
		//case tf_rgb16_snorm: gl_internal_format = GL_RGB16_SNORM; gl_pixel_format = GL_RGB; break;
		case tf_rgba2: gl_internal_format = GL_RGBA2; gl_pixel_format = GL_RGBA; break;
		case tf_rgba4: gl_internal_format = GL_RGBA4; gl_pixel_format = GL_RGBA; break;
		case tf_rgb5_a1: gl_internal_format = GL_RGB5_A1; gl_pixel_format = GL_RGBA; break;
		case tf_rgba8: gl_internal_format = GL_RGBA8; gl_pixel_format = GL_RGBA; break;
		case tf_bgra8: gl_internal_format = GL_RGBA8; gl_pixel_format = GL_BGRA; break;
		case tf_bgr8: gl_internal_format = GL_RGB8; gl_pixel_format = GL_BGR; break;
		//case tf_rgba8_snorm: gl_internal_format = GL_RGBA8_SNORM; gl_pixel_format = GL_RGBA; break;
		case tf_rgb10_a2: gl_internal_format = GL_RGB10_A2; gl_pixel_format = GL_RGBA; break;
		case tf_rgba12: gl_internal_format = GL_RGBA12; gl_pixel_format = GL_RGBA; break;
		case tf_rgba16: gl_internal_format = GL_RGBA16; gl_pixel_format = GL_RGBA; break;
		//case tf_rgba16_snorm: gl_internal_format = GL_RGBA16_SNORM; gl_pixel_format = GL_RGBA; break;
		//case tf_srgb8: gl_internal_format = GL_SRGB8; gl_pixel_format = GL_RGB; break;
		//case tf_srgb8_alpha8: gl_internal_format = GL_SRGB8_ALPHA8; gl_pixel_format = GL_RGBA; break;
		//case tf_r16f: gl_internal_format = GL_R16F; gl_pixel_format = GL_RED; break;
		//case tf_rg16f: gl_internal_format = GL_RG16F; gl_pixel_format = GL_RG; break;
		//case tf_rgb16f: gl_internal_format = GL_RGB16F; gl_pixel_format = GL_RGB; break;
		//case tf_rgba16f: gl_internal_format = GL_RGBA16F; gl_pixel_format = GL_RGBA; break;
		//case tf_r32f: gl_internal_format = GL_R32F; gl_pixel_format = GL_RED; break;
		//case tf_rg32f: gl_internal_format = GL_RG32F; gl_pixel_format = GL_RG; break;
		//case tf_rgb32f: gl_internal_format = GL_RGB32F; gl_pixel_format = GL_RGB; break;
		//case tf_rgba32f: gl_internal_format = GL_RGBA32F; gl_pixel_format = GL_RGBA; break;
		//case tf_r11f_g11f_b10f: gl_internal_format = GL_R11F_G11F_B10F; gl_pixel_format = GL_RGB; break;
		//case tf_rgb9_e5: gl_internal_format = GL_RGB9_E5; gl_pixel_format = GL_RGB; break;
		//case tf_r8i: gl_internal_format = GL_R8I; gl_pixel_format = GL_RED; break;
		//case tf_r8ui: gl_internal_format = GL_R8UI; gl_pixel_format = GL_RED; break;
		//case tf_r16i: gl_internal_format = GL_R16I; gl_pixel_format = GL_RED; break;
		//case tf_r16ui: gl_internal_format = GL_R16UI; gl_pixel_format = GL_RED; break;
		//case tf_r32i: gl_internal_format = GL_R32I; gl_pixel_format = GL_RED; break;
		//case tf_r32ui: gl_internal_format = GL_R32UI; gl_pixel_format = GL_RED; break;
		//case tf_rg8i: gl_internal_format = GL_RG8I; gl_pixel_format = GL_RG; break;
		//case tf_rg8ui: gl_internal_format = GL_RG8UI; gl_pixel_format = GL_RG; break;
		//case tf_rg16i: gl_internal_format = GL_RG16I; gl_pixel_format = GL_RG; break;
		//case tf_rg16ui: gl_internal_format = GL_RG16UI; gl_pixel_format = GL_RG; break;
		//case tf_rg32i: gl_internal_format = GL_RG32I; gl_pixel_format = GL_RG; break;
		//case tf_rg32ui: gl_internal_format = GL_RG32UI; gl_pixel_format = GL_RG; break;
		//case tf_rgb8i: gl_internal_format = GL_RGB8I; gl_pixel_format = GL_RGB; break;
		//case tf_rgb8ui: gl_internal_format = GL_RGB8UI; gl_pixel_format = GL_RGB; break;
		//case tf_rgb16i: gl_internal_format = GL_RGB16I; gl_pixel_format = GL_RGB; break;
		//case tf_rgb16ui: gl_internal_format = GL_RGB16UI; gl_pixel_format = GL_RGB; break;
		//case tf_rgb32i: gl_internal_format = GL_RGB32I; gl_pixel_format = GL_RGB; break;
		//case tf_rgb32ui: gl_internal_format = GL_RGB32UI; gl_pixel_format = GL_RGB; break;
		//case tf_rgba8i: gl_internal_format = GL_RGBA8I; gl_pixel_format = GL_RGBA; break;
		//case tf_rgba8ui: gl_internal_format = GL_RGBA8UI; gl_pixel_format = GL_RGBA; break;
		//case tf_rgba16i: gl_internal_format = GL_RGBA16I; gl_pixel_format = GL_RGBA; break;
		//case tf_rgba16ui: gl_internal_format = GL_RGBA16UI; gl_pixel_format = GL_RGBA; break;
		//case tf_rgba32i: gl_internal_format = GL_RGBA32I; gl_pixel_format = GL_RGBA; break;
		//case tf_rgba32ui: gl_internal_format = GL_RGBA32UI; gl_pixel_format = GL_RGBA; break;
		case tf_depth_component16: gl_internal_format = GL_DEPTH_COMPONENT16; gl_pixel_format = GL_DEPTH_COMPONENT; break;
		case tf_depth_component24: gl_internal_format = GL_DEPTH_COMPONENT24; gl_pixel_format = GL_DEPTH_COMPONENT; break;
		case tf_depth_component32: gl_internal_format = GL_DEPTH_COMPONENT32; gl_pixel_format = GL_DEPTH_COMPONENT; break;
		//case tf_depth_component32f: gl_internal_format = GL_DEPTH_COMPONENT32F; gl_pixel_format = GL_DEPTH_COMPONENT; break;
		//case tf_depth24_stencil8: gl_internal_format = GL_DEPTH24_STENCIL8; gl_pixel_format = GL_DEPTH_STENCIL; break;
		//case tf_depth32f_stencil8: gl_internal_format = GL_DEPTH32F_STENCIL8; gl_pixel_format = GL_DEPTH_STENCIL; break;

		//case tf_compressed_red: gl_internal_format = GL_COMPRESSED_RED; gl_pixel_format = GL_RED; break;
		//case tf_compressed_rg: gl_internal_format = GL_COMPRESSED_RG; gl_pixel_format = GL_RG; break;
		case tf_compressed_rgb: gl_internal_format = GL_COMPRESSED_RGB; gl_pixel_format = GL_RGB; break;
		case tf_compressed_rgba: gl_internal_format = GL_COMPRESSED_RGBA; gl_pixel_format = GL_RGBA; break;
		//case tf_compressed_srgb: gl_internal_format = GL_COMPRESSED_SRGB; gl_pixel_format = GL_RGB; break;
		//case tf_compressed_srgb_alpha: gl_internal_format = GL_COMPRESSED_SRGB_ALPHA; gl_pixel_format = GL_RGBA; break;
		//case tf_compressed_red_rgtc1: gl_internal_format = GL_COMPRESSED_RED_RGTC1; gl_pixel_format = GL_RED; break;
		//case tf_compressed_signed_red_rgtc1: gl_internal_format = GL_COMPRESSED_SIGNED_RED_RGTC1; gl_pixel_format = GL_RED; break;
		//case tf_compressed_rg_rgtc2: gl_internal_format = GL_COMPRESSED_RG_RGTC2; gl_pixel_format = GL_RG; break;
		//case tf_compressed_signed_rg_rgtc2: gl_internal_format = GL_COMPRESSED_SIGNED_RG_RGTC2; gl_pixel_format = GL_RG; break;

		case tf_compressed_rgb_s3tc_dxt1: gl_internal_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; gl_pixel_format = GL_RGB; break;
		case tf_compressed_rgba_s3tc_dxt1: gl_internal_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; gl_pixel_format = GL_RGBA; break;
		case tf_compressed_rgba_s3tc_dxt3: gl_internal_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; gl_pixel_format = GL_RGBA; break;
		case tf_compressed_rgba_s3tc_dxt5: gl_internal_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; gl_pixel_format = GL_RGBA; break;
		//case tf_compressed_srgb_s3tc_dxt1: gl_internal_format = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT; gl_pixel_format = GL_RGB; break;
		//case tf_compressed_srgb_alpha_s3tc_dxt1: gl_internal_format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT; gl_pixel_format = GL_RGBA; break;
		//case tf_compressed_srgb_alpha_s3tc_dxt3: gl_internal_format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT; gl_pixel_format = GL_RGBA; break;
		//case tf_compressed_srgb_alpha_s3tc_dxt5: gl_internal_format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT; gl_pixel_format = GL_RGBA; break;

		default:
			throw Exception(string_format("Unsupported TextureFormat (%1)", format));
	}
}

bool GL1::to_opengl_pixelformat(TextureFormat texture_format, GLenum &format, GLenum &type)
{
	bool valid = false;

	//TODO: We should really use Endian::is_system_big()

	//TODO: All the formats in this switch are not supported - Maybe they can be

	switch (texture_format)
	{
		case tf_rgba8:
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_RGBA;
			break;
		}
		case tf_rgb8:
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_RGB;
			break;
		}
		case tf_bgr8: 
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_BGR;
			break;
		}
		case tf_bgra8: 
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_BGRA;
			break;
		}
		case tf_stencil_index1: break;
		case tf_stencil_index4: break;
		case tf_stencil_index8: break;
		case tf_stencil_index16: break;
		case tf_r8:
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_RED;
			break;
		}
		case tf_r8_snorm:
		{
			valid = true;
			type = GL_BYTE;
			format = GL_RED;
			break;
		}
		case tf_r16:
		{
			valid = true;
			type = GL_UNSIGNED_SHORT;
			format = GL_RED;
			break;
		}
		case tf_r16_snorm:
		{
			valid = true;
			type = GL_SHORT;
			format = GL_RED;
			break;
		}
		case tf_rg8: break;
		case tf_rg8_snorm: break;
		case tf_rg16: break;
		case tf_rg16_snorm: break;
		case tf_r3_g3_b2:
		{
			valid = true;
			type = GL_UNSIGNED_BYTE_3_3_2;
			format = GL_RGB;
			break;
		}
		case tf_rgb4: break;
		case tf_rgb5: break;
		case tf_rgb8_snorm:
		{
			valid = true;
			type = GL_BYTE;
			format = GL_RGB;
			break;
		}
		case tf_rgb10: break;
		case tf_rgb12: break;
		case tf_rgb16: break;
		case tf_rgb16_snorm: break;
		case tf_rgba2: break;
		case tf_rgba4:
		{
			valid = true;
			type = GL_UNSIGNED_SHORT_4_4_4_4;
			format = GL_RGBA;
			break;
		}
		case tf_rgb5_a1:
		{
			valid = true;
			type = GL_UNSIGNED_SHORT_5_5_5_1;
			format = GL_RGBA;
			break;
		}
		case tf_rgba8_snorm: break;
		case tf_rgb10_a2: break;
		case tf_rgba12: break;
		case tf_rgba16: break;
		case tf_rgba16_snorm: break;
		case tf_srgb8:
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_RGB;
			break;
		}
		case tf_srgb8_alpha8:
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_RGBA;
			break;
		}
		case tf_r16f: break;
		case tf_rg16f: break;
		case tf_rgb16f: break;
		case tf_rgba16f: break;

		case tf_r32f:
		{
			valid = true;
			type = GL_FLOAT;
			format = GL_RED;
			break;
		}
		case tf_rg32f: break;
		case tf_rgb32f:
		{
			valid = true;
			type = GL_FLOAT;
			format = GL_RGB;
			break;
		}
		
		case tf_rgba32f:
		{
			valid = true;
			type = GL_FLOAT;
			format = GL_RGBA;
			break;
		}
		
		case tf_r11f_g11f_b10f: break;
		case tf_rgb9_e5: break;
		case tf_r8i:
		{
			valid = true;
			type = GL_BYTE;
			format = GL_RED;
			break;
		}
		case tf_r8ui:
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_RED;
			break;
		}
		case tf_r16i:
		{
			valid = true;
			type = GL_SHORT;
			format = GL_RED;
			break;
		}
		case tf_r16ui:
		{
			valid = true;
			type = GL_UNSIGNED_SHORT;
			format = GL_RED;
			break;
		}
		case tf_r32i:
		{
			valid = true;
			type = GL_INT;
			format = GL_RED;
			break;
		}
		case tf_r32ui:
		{
			valid = true;
			type = GL_UNSIGNED_INT;
			format = GL_RED;
			break;
		}
		case tf_rg8i: break;
		case tf_rg8ui: break;
		case tf_rg16i: break;
		case tf_rg16ui: break;
		case tf_rg32i: break;
		case tf_rg32ui: break;
		case tf_rgb8i:
		{
			valid = true;
			type = GL_BYTE;
			format = GL_RGB;
			break;
		}
		case tf_rgb8ui:
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_RGB;
			break;
		}
		case tf_rgb16i: break;
		case tf_rgb16ui: break;
		case tf_rgb32i: break;
		case tf_rgb32ui: break;
		case tf_rgba8i:
		{
			valid = true;
			type = GL_BYTE;
			format = GL_RGBA;
			break;
		}
		case tf_rgba8ui:
		{
			valid = true;
			type = GL_UNSIGNED_BYTE;
			format = GL_RGBA;
			break;
		}
		case tf_rgba16i: break;
		case tf_rgba16ui: break;
		case tf_rgba32i: break;
		case tf_rgba32ui: break;
		case tf_depth_component16: break;
		case tf_depth_component24: break;
		case tf_depth_component32: break;
		case tf_depth_component32f: break;
		case tf_depth24_stencil8: break;
		case tf_depth32f_stencil8: break;
		case tf_compressed_red: break;
		case tf_compressed_rg: break;
		case tf_compressed_rgb: break;
		case tf_compressed_rgba: break;
		case tf_compressed_srgb: break;
		case tf_compressed_srgb_alpha: break;
		case tf_compressed_red_rgtc1: break;
		case tf_compressed_signed_red_rgtc1: break;
		case tf_compressed_rg_rgtc2: break;
		case tf_compressed_signed_rg_rgtc2: break;

		case tf_compressed_rgb_s3tc_dxt1: break;
		case tf_compressed_rgba_s3tc_dxt1: break;
		case tf_compressed_rgba_s3tc_dxt3: break;
		case tf_compressed_rgba_s3tc_dxt5: break;
		case tf_compressed_srgb_s3tc_dxt1: break;
		case tf_compressed_srgb_alpha_s3tc_dxt1: break;
		case tf_compressed_srgb_alpha_s3tc_dxt3: break;
		case tf_compressed_srgb_alpha_s3tc_dxt5: break;
	}

	return valid;
}

bool GL1::to_opengl_pixelformat(const PixelBuffer &pbuffer, GLenum &format, GLenum &type)
{
	return to_opengl_pixelformat(pbuffer.get_format(), format, type);
}

/////////////////////////////////////////////////////////////////////////////
// OpenGL context management:

cl_tls_variable const GL1GraphicContextProvider * gl1_active_opengl_gc = 0;
static Mutex gl1_function_map_mutex;

// A fix for a compiler bug with compiler version 13.00.9466
#if _MSC_VER > 1300
typedef std::map<const GL1GraphicContextProvider * const, GL1Functions *> gl1_function_map_type;
#else
typedef std::map<const GL1GraphicContextProvider *, GL1Functions *> gl1_function_map_type;
#endif

static gl1_function_map_type gl1_function_map;

GL1Functions *gl1_setup_binds();

GL1ProcAddress *GL1::get_proc_address(const std::string& function_name)
{
	if (gl1_active_opengl_gc)
	{
		return gl1_active_opengl_gc->get_proc_address(function_name);
	}
	return NULL;
}

void GL1::set_active(GraphicContext &gc)
{
	set_active(static_cast<const GL1GraphicContextProvider *>(gc.get_provider()));
}

bool GL1::set_active()
{
	if (gl1_active_opengl_gc)	// If already active, we can exit now
		return true;

	std::unique_ptr<MutexSection> mutex_section;
	GraphicContextProvider* shared_provider = SharedGCData::get_provider(mutex_section);
	if (shared_provider)
	{
		GL1GraphicContextProvider *gc_provider = dynamic_cast<GL1GraphicContextProvider*>(shared_provider);
		if (gc_provider)
		{
			GL1::set_active(gc_provider);
			return true;
		}
	}
	set_active(NULL);
	return false;
}

void GL1::set_active(const GL1GraphicContextProvider * const gc_provider)
{
	// Don't do anything if the supplied graphic context is already active.
	if (gc_provider != gl1_active_opengl_gc)
	{
		if (gc_provider)
		{
			// Make render context associated with graphic context current.
			gc_provider->get_render_window().make_current();

			// Map bound functions for this graphic context. Add to static cache if necessary.
			MutexSection mutex_lock(&gl1_function_map_mutex);
			gl1_function_map_type::iterator it;
			it = gl1_function_map.find(gc_provider);
			if (it != gl1_function_map.end())
			{
				GL1::functions = it->second;
			}
			else
			{
				gl1_active_opengl_gc = gc_provider;
				GL1Functions *functions = gl1_setup_binds();
				gl1_function_map[gc_provider] = functions;
				GL1::functions = functions;
			}
		}

		// If no graphic context provided, set no render context current.
		else
		{
#		if defined(WIN32)
			wglMakeCurrent(NULL, NULL);
#		else
			//Note: glX may not even be available. Also glXGetCurrentDisplay() may fail
			// Hopefully this will not matter!
			//glXMakeCurrent(glXGetCurrentDisplay(), None, NULL);
#		endif

			// If no current context, don't map function bindings either.
			GL1::functions = NULL;
		}

		// OK, make our context the active one
		gl1_active_opengl_gc = gc_provider;
	}
}

void GL1::remove_active(const GL1GraphicContextProvider * const gc_provider)
{
	MutexSection mutex_lock(&gl1_function_map_mutex);
	gl1_function_map_type::iterator it;
	it = gl1_function_map.find(gc_provider);
	if (it != gl1_function_map.end())
	{
		GL1Functions *functions = it->second;
		gl1_function_map.erase(it);
		delete functions;

		if (gl1_active_opengl_gc == gc_provider)
		{
			set_active(NULL);
		}
	}
}

GL1Functions *gl1_setup_binds()
{
	GL1Functions *functions = new GL1Functions;
	memset(functions, 0, sizeof(GL1Functions));

	// Binds for OpenGL 1.1:

//#ifdef GL_VERSION_1_1
#ifdef WIN32	// Only win32 static links opengl

	functions->accum = (GL1Functions::ptr_glAccum) &glAccum;
	functions->alphaFunc = (GL1Functions::ptr_glAlphaFunc) &glAlphaFunc;
	functions->areTexturesResident = (GL1Functions::ptr_glAreTexturesResident) &glAreTexturesResident;
	functions->arrayElement = (GL1Functions::ptr_glArrayElement) &glArrayElement;
	functions->begin = (GL1Functions::ptr_glBegin) &glBegin;
	functions->bindTexture = (GL1Functions::ptr_glBindTexture) &glBindTexture;
	functions->bitmap = (GL1Functions::ptr_glBitmap) &glBitmap;
	functions->blendFunc = (GL1Functions::ptr_glBlendFunc) &glBlendFunc;
	functions->callList = (GL1Functions::ptr_glCallList) &glCallList;
	functions->callLists = (GL1Functions::ptr_glCallLists) &glCallLists;
	functions->clear = (GL1Functions::ptr_glClear) &glClear;
	functions->clearAccum = (GL1Functions::ptr_glClearAccum) &glClearAccum;
	functions->clearColor = (GL1Functions::ptr_glClearColor) &glClearColor;
	functions->clearDepth = (GL1Functions::ptr_glClearDepth) &glClearDepth;
	functions->clearIndex = (GL1Functions::ptr_glClearIndex) &glClearIndex;
	functions->clearStencil = (GL1Functions::ptr_glClearStencil) &glClearStencil;
	functions->clipPlane = (GL1Functions::ptr_glClipPlane) &glClipPlane;
	functions->color3b = (GL1Functions::ptr_glColor3b) &glColor3b;
	functions->color3bv = (GL1Functions::ptr_glColor3bv) &glColor3bv;
	functions->color3d = (GL1Functions::ptr_glColor3d) &glColor3d;
	functions->color3dv = (GL1Functions::ptr_glColor3dv) &glColor3dv;
	functions->color3f = (GL1Functions::ptr_glColor3f) &glColor3f;
	functions->color3fv = (GL1Functions::ptr_glColor3fv) &glColor3fv;
	functions->color3i = (GL1Functions::ptr_glColor3i) &glColor3i;
	functions->color3iv = (GL1Functions::ptr_glColor3iv) &glColor3iv;
	functions->color3s = (GL1Functions::ptr_glColor3s) &glColor3s;
	functions->color3sv = (GL1Functions::ptr_glColor3sv) &glColor3sv;
	functions->color3ub = (GL1Functions::ptr_glColor3ub) &glColor3ub;
	functions->color3ubv = (GL1Functions::ptr_glColor3ubv) &glColor3ubv;
	functions->color3ui = (GL1Functions::ptr_glColor3ui) &glColor3ui;
	functions->color3uiv = (GL1Functions::ptr_glColor3uiv) &glColor3uiv;
	functions->color3us = (GL1Functions::ptr_glColor3us) &glColor3us;
	functions->color3usv = (GL1Functions::ptr_glColor3usv) &glColor3usv;
	functions->color4b = (GL1Functions::ptr_glColor4b) &glColor4b;
	functions->color4bv = (GL1Functions::ptr_glColor4bv) &glColor4bv;
	functions->color4d = (GL1Functions::ptr_glColor4d) &glColor4d;
	functions->color4dv = (GL1Functions::ptr_glColor4dv) &glColor4dv;
	functions->color4f = (GL1Functions::ptr_glColor4f) &glColor4f;
	functions->color4fv = (GL1Functions::ptr_glColor4fv) &glColor4fv;
	functions->color4i = (GL1Functions::ptr_glColor4i) &glColor4i;
	functions->color4iv = (GL1Functions::ptr_glColor4iv) &glColor4iv;
	functions->color4s = (GL1Functions::ptr_glColor4s) &glColor4s;
	functions->color4sv = (GL1Functions::ptr_glColor4sv) &glColor4sv;
	functions->color4ub = (GL1Functions::ptr_glColor4ub) &glColor4ub;
	functions->color4ubv = (GL1Functions::ptr_glColor4ubv) &glColor4ubv;
	functions->color4ui = (GL1Functions::ptr_glColor4ui) &glColor4ui;
	functions->color4uiv = (GL1Functions::ptr_glColor4uiv) &glColor4uiv;
	functions->color4us = (GL1Functions::ptr_glColor4us) &glColor4us;
	functions->color4usv = (GL1Functions::ptr_glColor4usv) &glColor4usv;
	functions->colorMask = (GL1Functions::ptr_glColorMask) &glColorMask;
	functions->colorMaterial = (GL1Functions::ptr_glColorMaterial) &glColorMaterial;
	functions->colorPointer = (GL1Functions::ptr_glColorPointer) &glColorPointer;
	functions->copyPixels = (GL1Functions::ptr_glCopyPixels) &glCopyPixels;
	functions->copyTexImage1D = (GL1Functions::ptr_glCopyTexImage1D) &glCopyTexImage1D;
	functions->copyTexImage2D = (GL1Functions::ptr_glCopyTexImage2D) &glCopyTexImage2D;
	functions->copyTexSubImage1D = (GL1Functions::ptr_glCopyTexSubImage1D) &glCopyTexSubImage1D;
	functions->copyTexSubImage2D = (GL1Functions::ptr_glCopyTexSubImage2D) &glCopyTexSubImage2D;
	functions->cullFace = (GL1Functions::ptr_glCullFace) &glCullFace;
	functions->deleteLists = (GL1Functions::ptr_glDeleteLists) &glDeleteLists;
	functions->deleteTextures = (GL1Functions::ptr_glDeleteTextures) &glDeleteTextures;
	functions->depthFunc = (GL1Functions::ptr_glDepthFunc) &glDepthFunc;
	functions->depthMask = (GL1Functions::ptr_glDepthMask) &glDepthMask;
	functions->depthRange = (GL1Functions::ptr_glDepthRange) &glDepthRange;
	functions->disable = (GL1Functions::ptr_glDisable) &glDisable;
	functions->disableClientState = (GL1Functions::ptr_glDisableClientState) &glDisableClientState;
	functions->drawArrays = (GL1Functions::ptr_glDrawArrays) &glDrawArrays;
	functions->drawBuffer = (GL1Functions::ptr_glDrawBuffer) &glDrawBuffer;
	functions->drawElements = (GL1Functions::ptr_glDrawElements) &glDrawElements;
	functions->drawPixels = (GL1Functions::ptr_glDrawPixels) &glDrawPixels;
	functions->edgeFlag = (GL1Functions::ptr_glEdgeFlag) &glEdgeFlag;
	functions->edgeFlagPointer = (GL1Functions::ptr_glEdgeFlagPointer) &glEdgeFlagPointer;
	functions->edgeFlagv = (GL1Functions::ptr_glEdgeFlagv) &glEdgeFlagv;
	functions->enable = (GL1Functions::ptr_glEnable) &glEnable;
	functions->enableClientState = (GL1Functions::ptr_glEnableClientState) &glEnableClientState;
	functions->end = (GL1Functions::ptr_glEnd) &glEnd;
	functions->endList = (GL1Functions::ptr_glEndList) &glEndList;
	functions->evalCoord1d = (GL1Functions::ptr_glEvalCoord1d) &glEvalCoord1d;
	functions->evalCoord1dv = (GL1Functions::ptr_glEvalCoord1dv) &glEvalCoord1dv;
	functions->evalCoord1f = (GL1Functions::ptr_glEvalCoord1f) &glEvalCoord1f;
	functions->evalCoord1fv = (GL1Functions::ptr_glEvalCoord1fv) &glEvalCoord1fv;
	functions->evalCoord2d = (GL1Functions::ptr_glEvalCoord2d) &glEvalCoord2d;
	functions->evalCoord2dv = (GL1Functions::ptr_glEvalCoord2dv) &glEvalCoord2dv;
	functions->evalCoord2f = (GL1Functions::ptr_glEvalCoord2f) &glEvalCoord2f;
	functions->evalCoord2fv = (GL1Functions::ptr_glEvalCoord2fv) &glEvalCoord2fv;
	functions->evalMesh1 = (GL1Functions::ptr_glEvalMesh1) &glEvalMesh1;
	functions->evalMesh2 = (GL1Functions::ptr_glEvalMesh2) &glEvalMesh2;
	functions->evalPoint1 = (GL1Functions::ptr_glEvalPoint1) &glEvalPoint1;
	functions->evalPoint2 = (GL1Functions::ptr_glEvalPoint2) &glEvalPoint2;
	functions->feedbackBuffer = (GL1Functions::ptr_glFeedbackBuffer) &glFeedbackBuffer;
	functions->finish = (GL1Functions::ptr_glFinish) &glFinish;
	functions->flush = (GL1Functions::ptr_glFlush) &glFlush;
	functions->fogf = (GL1Functions::ptr_glFogf) &glFogf;
	functions->fogfv = (GL1Functions::ptr_glFogfv) &glFogfv;
	functions->fogi = (GL1Functions::ptr_glFogi) &glFogi;
	functions->fogiv = (GL1Functions::ptr_glFogiv) &glFogiv;
	functions->frontFace = (GL1Functions::ptr_glFrontFace) &glFrontFace;
	functions->frustum = (GL1Functions::ptr_glFrustum) &glFrustum;
	functions->genLists = (GL1Functions::ptr_glGenLists) &glGenLists;
	functions->genTextures = (GL1Functions::ptr_glGenTextures) &glGenTextures;
	functions->getBooleanv = (GL1Functions::ptr_glGetBooleanv) &glGetBooleanv;
	functions->getClipPlane = (GL1Functions::ptr_glGetClipPlane) &glGetClipPlane;
	functions->getDoublev = (GL1Functions::ptr_glGetDoublev) &glGetDoublev;
	functions->getError = (GL1Functions::ptr_glGetError) &glGetError;
	functions->getFloatv = (GL1Functions::ptr_glGetFloatv) &glGetFloatv;
	functions->getIntegerv = (GL1Functions::ptr_glGetIntegerv) &glGetIntegerv;
	functions->getLightfv = (GL1Functions::ptr_glGetLightfv) &glGetLightfv;
	functions->getLightiv = (GL1Functions::ptr_glGetLightiv) &glGetLightiv;
	functions->getMapdv = (GL1Functions::ptr_glGetMapdv) &glGetMapdv;
	functions->getMapfv = (GL1Functions::ptr_glGetMapfv) &glGetMapfv;
	functions->getMapiv = (GL1Functions::ptr_glGetMapiv) &glGetMapiv;
	functions->getMaterialfv = (GL1Functions::ptr_glGetMaterialfv) &glGetMaterialfv;
	functions->getMaterialiv = (GL1Functions::ptr_glGetMaterialiv) &glGetMaterialiv;
	functions->getPixelMapfv = (GL1Functions::ptr_glGetPixelMapfv) &glGetPixelMapfv;
	functions->getPixelMapuiv = (GL1Functions::ptr_glGetPixelMapuiv) &glGetPixelMapuiv;
	functions->getPixelMapusv = (GL1Functions::ptr_glGetPixelMapusv) &glGetPixelMapusv;
	functions->getPointerv = (GL1Functions::ptr_glGetPointerv) &glGetPointerv;
	functions->getPolygonStipple = (GL1Functions::ptr_glGetPolygonStipple) &glGetPolygonStipple;
	functions->getString = (GL1Functions::ptr_glGetString) &glGetString;
	functions->getTexEnvfv = (GL1Functions::ptr_glGetTexEnvfv) &glGetTexEnvfv;
	functions->getTexEnviv = (GL1Functions::ptr_glGetTexEnviv) &glGetTexEnviv;
	functions->getTexGendv = (GL1Functions::ptr_glGetTexGendv) &glGetTexGendv;
	functions->getTexGenfv = (GL1Functions::ptr_glGetTexGenfv) &glGetTexGenfv;
	functions->getTexGeniv = (GL1Functions::ptr_glGetTexGeniv) &glGetTexGeniv;
	functions->getTexImage = (GL1Functions::ptr_glGetTexImage) &glGetTexImage;
	functions->getTexLevelParameterfv = (GL1Functions::ptr_glGetTexLevelParameterfv) &glGetTexLevelParameterfv;
	functions->getTexLevelParameteriv = (GL1Functions::ptr_glGetTexLevelParameteriv) &glGetTexLevelParameteriv;
	functions->getTexParameterfv = (GL1Functions::ptr_glGetTexParameterfv) &glGetTexParameterfv;
	functions->getTexParameteriv = (GL1Functions::ptr_glGetTexParameteriv) &glGetTexParameteriv;
	functions->hint = (GL1Functions::ptr_glHint) &glHint;
	functions->indexMask = (GL1Functions::ptr_glIndexMask) &glIndexMask;
	functions->indexPointer = (GL1Functions::ptr_glIndexPointer) &glIndexPointer;
	functions->indexd = (GL1Functions::ptr_glIndexd) &glIndexd;
	functions->indexdv = (GL1Functions::ptr_glIndexdv) &glIndexdv;
	functions->indexf = (GL1Functions::ptr_glIndexf) &glIndexf;
	functions->indexfv = (GL1Functions::ptr_glIndexfv) &glIndexfv;
	functions->indexi = (GL1Functions::ptr_glIndexi) &glIndexi;
	functions->indexiv = (GL1Functions::ptr_glIndexiv) &glIndexiv;
	functions->indexs = (GL1Functions::ptr_glIndexs) &glIndexs;
	functions->indexsv = (GL1Functions::ptr_glIndexsv) &glIndexsv;
	functions->indexub = (GL1Functions::ptr_glIndexub) &glIndexub;
	functions->indexubv = (GL1Functions::ptr_glIndexubv) &glIndexubv;
	functions->initNames = (GL1Functions::ptr_glInitNames) &glInitNames;
	functions->interleavedArrays = (GL1Functions::ptr_glInterleavedArrays) &glInterleavedArrays;
	functions->isEnabled = (GL1Functions::ptr_glIsEnabled) &glIsEnabled;
	functions->isList = (GL1Functions::ptr_glIsList) &glIsList;
	functions->isTexture = (GL1Functions::ptr_glIsTexture) &glIsTexture;
	functions->lightModelf = (GL1Functions::ptr_glLightModelf) &glLightModelf;
	functions->lightModelfv = (GL1Functions::ptr_glLightModelfv) &glLightModelfv;
	functions->lightModeli = (GL1Functions::ptr_glLightModeli) &glLightModeli;
	functions->lightModeliv = (GL1Functions::ptr_glLightModeliv) &glLightModeliv;
	functions->lightf = (GL1Functions::ptr_glLightf) &glLightf;
	functions->lightfv = (GL1Functions::ptr_glLightfv) &glLightfv;
	functions->lighti = (GL1Functions::ptr_glLighti) &glLighti;
	functions->lightiv = (GL1Functions::ptr_glLightiv) &glLightiv;
	functions->lineStipple = (GL1Functions::ptr_glLineStipple) &glLineStipple;
	functions->lineWidth = (GL1Functions::ptr_glLineWidth) &glLineWidth;
	functions->listBase = (GL1Functions::ptr_glListBase) &glListBase;
	functions->loadIdentity = (GL1Functions::ptr_glLoadIdentity) &glLoadIdentity;
	functions->loadMatrixd = (GL1Functions::ptr_glLoadMatrixd) &glLoadMatrixd;
	functions->loadMatrixf = (GL1Functions::ptr_glLoadMatrixf) &glLoadMatrixf;
	functions->loadName = (GL1Functions::ptr_glLoadName) &glLoadName;
	functions->logicOp = (GL1Functions::ptr_glLogicOp) &glLogicOp;
	functions->map1d = (GL1Functions::ptr_glMap1d) &glMap1d;
	functions->map1f = (GL1Functions::ptr_glMap1f) &glMap1f;
	functions->map2d = (GL1Functions::ptr_glMap2d) &glMap2d;
	functions->map2f = (GL1Functions::ptr_glMap2f) &glMap2f;
	functions->mapGrid1d = (GL1Functions::ptr_glMapGrid1d) &glMapGrid1d;
	functions->mapGrid1f = (GL1Functions::ptr_glMapGrid1f) &glMapGrid1f;
	functions->mapGrid2d = (GL1Functions::ptr_glMapGrid2d) &glMapGrid2d;
	functions->mapGrid2f = (GL1Functions::ptr_glMapGrid2f) &glMapGrid2f;
	functions->materialf = (GL1Functions::ptr_glMaterialf) &glMaterialf;
	functions->materialfv = (GL1Functions::ptr_glMaterialfv) &glMaterialfv;
	functions->materiali = (GL1Functions::ptr_glMateriali) &glMateriali;
	functions->materialiv = (GL1Functions::ptr_glMaterialiv) &glMaterialiv;
	functions->matrixMode = (GL1Functions::ptr_glMatrixMode) &glMatrixMode;
	functions->multMatrixd = (GL1Functions::ptr_glMultMatrixd) &glMultMatrixd;
	functions->multMatrixf = (GL1Functions::ptr_glMultMatrixf) &glMultMatrixf;
	functions->newList = (GL1Functions::ptr_glNewList) &glNewList;
	functions->normal3b = (GL1Functions::ptr_glNormal3b) &glNormal3b;
	functions->normal3bv = (GL1Functions::ptr_glNormal3bv) &glNormal3bv;
	functions->normal3d = (GL1Functions::ptr_glNormal3d) &glNormal3d;
	functions->normal3dv = (GL1Functions::ptr_glNormal3dv) &glNormal3dv;
	functions->normal3f = (GL1Functions::ptr_glNormal3f) &glNormal3f;
	functions->normal3fv = (GL1Functions::ptr_glNormal3fv) &glNormal3fv;
	functions->normal3i = (GL1Functions::ptr_glNormal3i) &glNormal3i;
	functions->normal3iv = (GL1Functions::ptr_glNormal3iv) &glNormal3iv;
	functions->normal3s = (GL1Functions::ptr_glNormal3s) &glNormal3s;
	functions->normal3sv = (GL1Functions::ptr_glNormal3sv) &glNormal3sv;
	functions->normalPointer = (GL1Functions::ptr_glNormalPointer) &glNormalPointer;
	functions->ortho = (GL1Functions::ptr_glOrtho) &glOrtho;
	functions->passThrough = (GL1Functions::ptr_glPassThrough) &glPassThrough;
	functions->pixelMapfv = (GL1Functions::ptr_glPixelMapfv) &glPixelMapfv;
	functions->pixelMapuiv = (GL1Functions::ptr_glPixelMapuiv) &glPixelMapuiv;
	functions->pixelMapusv = (GL1Functions::ptr_glPixelMapusv) &glPixelMapusv;
	functions->pixelStoref = (GL1Functions::ptr_glPixelStoref) &glPixelStoref;
	functions->pixelStorei = (GL1Functions::ptr_glPixelStorei) &glPixelStorei;
	functions->pixelTransferf = (GL1Functions::ptr_glPixelTransferf) &glPixelTransferf;
	functions->pixelTransferi = (GL1Functions::ptr_glPixelTransferi) &glPixelTransferi;
	functions->pixelZoom = (GL1Functions::ptr_glPixelZoom) &glPixelZoom;
	functions->pointSize = (GL1Functions::ptr_glPointSize) &glPointSize;
	functions->polygonMode = (GL1Functions::ptr_glPolygonMode) &glPolygonMode;
	functions->polygonOffset = (GL1Functions::ptr_glPolygonOffset) &glPolygonOffset;
	functions->polygonStipple = (GL1Functions::ptr_glPolygonStipple) &glPolygonStipple;
	functions->popAttrib = (GL1Functions::ptr_glPopAttrib) &glPopAttrib;
	functions->popClientAttrib = (GL1Functions::ptr_glPopClientAttrib) &glPopClientAttrib;
	functions->popMatrix = (GL1Functions::ptr_glPopMatrix) &glPopMatrix;
	functions->popName = (GL1Functions::ptr_glPopName) &glPopName;
	functions->prioritizeTextures = (GL1Functions::ptr_glPrioritizeTextures) &glPrioritizeTextures;
	functions->pushAttrib = (GL1Functions::ptr_glPushAttrib) &glPushAttrib;
	functions->pushClientAttrib = (GL1Functions::ptr_glPushClientAttrib) &glPushClientAttrib;
	functions->pushMatrix = (GL1Functions::ptr_glPushMatrix) &glPushMatrix;
	functions->pushName = (GL1Functions::ptr_glPushName) &glPushName;
	functions->rasterPos2d = (GL1Functions::ptr_glRasterPos2d) &glRasterPos2d;
	functions->rasterPos2dv = (GL1Functions::ptr_glRasterPos2dv) &glRasterPos2dv;
	functions->rasterPos2f = (GL1Functions::ptr_glRasterPos2f) &glRasterPos2f;
	functions->rasterPos2fv = (GL1Functions::ptr_glRasterPos2fv) &glRasterPos2fv;
	functions->rasterPos2i = (GL1Functions::ptr_glRasterPos2i) &glRasterPos2i;
	functions->rasterPos2iv = (GL1Functions::ptr_glRasterPos2iv) &glRasterPos2iv;
	functions->rasterPos2s = (GL1Functions::ptr_glRasterPos2s) &glRasterPos2s;
	functions->rasterPos2sv = (GL1Functions::ptr_glRasterPos2sv) &glRasterPos2sv;
	functions->rasterPos3d = (GL1Functions::ptr_glRasterPos3d) &glRasterPos3d;
	functions->rasterPos3dv = (GL1Functions::ptr_glRasterPos3dv) &glRasterPos3dv;
	functions->rasterPos3f = (GL1Functions::ptr_glRasterPos3f) &glRasterPos3f;
	functions->rasterPos3fv = (GL1Functions::ptr_glRasterPos3fv) &glRasterPos3fv;
	functions->rasterPos3i = (GL1Functions::ptr_glRasterPos3i) &glRasterPos3i;
	functions->rasterPos3iv = (GL1Functions::ptr_glRasterPos3iv) &glRasterPos3iv;
	functions->rasterPos3s = (GL1Functions::ptr_glRasterPos3s) &glRasterPos3s;
	functions->rasterPos3sv = (GL1Functions::ptr_glRasterPos3sv) &glRasterPos3sv;
	functions->rasterPos4d = (GL1Functions::ptr_glRasterPos4d) &glRasterPos4d;
	functions->rasterPos4dv = (GL1Functions::ptr_glRasterPos4dv) &glRasterPos4dv;
	functions->rasterPos4f = (GL1Functions::ptr_glRasterPos4f) &glRasterPos4f;
	functions->rasterPos4fv = (GL1Functions::ptr_glRasterPos4fv) &glRasterPos4fv;
	functions->rasterPos4i = (GL1Functions::ptr_glRasterPos4i) &glRasterPos4i;
	functions->rasterPos4iv = (GL1Functions::ptr_glRasterPos4iv) &glRasterPos4iv;
	functions->rasterPos4s = (GL1Functions::ptr_glRasterPos4s) &glRasterPos4s;
	functions->rasterPos4sv = (GL1Functions::ptr_glRasterPos4sv) &glRasterPos4sv;
	functions->readBuffer = (GL1Functions::ptr_glReadBuffer) &glReadBuffer;
	functions->readPixels = (GL1Functions::ptr_glReadPixels) &glReadPixels;
	functions->rectd = (GL1Functions::ptr_glRectd) &glRectd;
	functions->rectdv = (GL1Functions::ptr_glRectdv) &glRectdv;
	functions->rectf = (GL1Functions::ptr_glRectf) &glRectf;
	functions->rectfv = (GL1Functions::ptr_glRectfv) &glRectfv;
	functions->recti = (GL1Functions::ptr_glRecti) &glRecti;
	functions->rectiv = (GL1Functions::ptr_glRectiv) &glRectiv;
	functions->rects = (GL1Functions::ptr_glRects) &glRects;
	functions->rectsv = (GL1Functions::ptr_glRectsv) &glRectsv;
	functions->renderMode = (GL1Functions::ptr_glRenderMode) &glRenderMode;
	functions->rotated = (GL1Functions::ptr_glRotated) &glRotated;
	functions->rotatef = (GL1Functions::ptr_glRotatef) &glRotatef;
	functions->scaled = (GL1Functions::ptr_glScaled) &glScaled;
	functions->scalef = (GL1Functions::ptr_glScalef) &glScalef;
	functions->scissor = (GL1Functions::ptr_glScissor) &glScissor;
	functions->selectBuffer = (GL1Functions::ptr_glSelectBuffer) &glSelectBuffer;
	functions->shadeModel = (GL1Functions::ptr_glShadeModel) &glShadeModel;
	functions->stencilFunc = (GL1Functions::ptr_glStencilFunc) &glStencilFunc;
	functions->stencilMask = (GL1Functions::ptr_glStencilMask) &glStencilMask;
	functions->stencilOp = (GL1Functions::ptr_glStencilOp) &glStencilOp;
	functions->texCoord1d = (GL1Functions::ptr_glTexCoord1d) &glTexCoord1d;
	functions->texCoord1dv = (GL1Functions::ptr_glTexCoord1dv) &glTexCoord1dv;
	functions->texCoord1f = (GL1Functions::ptr_glTexCoord1f) &glTexCoord1f;
	functions->texCoord1fv = (GL1Functions::ptr_glTexCoord1fv) &glTexCoord1fv;
	functions->texCoord1i = (GL1Functions::ptr_glTexCoord1i) &glTexCoord1i;
	functions->texCoord1iv = (GL1Functions::ptr_glTexCoord1iv) &glTexCoord1iv;
	functions->texCoord1s = (GL1Functions::ptr_glTexCoord1s) &glTexCoord1s;
	functions->texCoord1sv = (GL1Functions::ptr_glTexCoord1sv) &glTexCoord1sv;
	functions->texCoord2d = (GL1Functions::ptr_glTexCoord2d) &glTexCoord2d;
	functions->texCoord2dv = (GL1Functions::ptr_glTexCoord2dv) &glTexCoord2dv;
	functions->texCoord2f = (GL1Functions::ptr_glTexCoord2f) &glTexCoord2f;
	functions->texCoord2fv = (GL1Functions::ptr_glTexCoord2fv) &glTexCoord2fv;
	functions->texCoord2i = (GL1Functions::ptr_glTexCoord2i) &glTexCoord2i;
	functions->texCoord2iv = (GL1Functions::ptr_glTexCoord2iv) &glTexCoord2iv;
	functions->texCoord2s = (GL1Functions::ptr_glTexCoord2s) &glTexCoord2s;
	functions->texCoord2sv = (GL1Functions::ptr_glTexCoord2sv) &glTexCoord2sv;
	functions->texCoord3d = (GL1Functions::ptr_glTexCoord3d) &glTexCoord3d;
	functions->texCoord3dv = (GL1Functions::ptr_glTexCoord3dv) &glTexCoord3dv;
	functions->texCoord3f = (GL1Functions::ptr_glTexCoord3f) &glTexCoord3f;
	functions->texCoord3fv = (GL1Functions::ptr_glTexCoord3fv) &glTexCoord3fv;
	functions->texCoord3i = (GL1Functions::ptr_glTexCoord3i) &glTexCoord3i;
	functions->texCoord3iv = (GL1Functions::ptr_glTexCoord3iv) &glTexCoord3iv;
	functions->texCoord3s = (GL1Functions::ptr_glTexCoord3s) &glTexCoord3s;
	functions->texCoord3sv = (GL1Functions::ptr_glTexCoord3sv) &glTexCoord3sv;
	functions->texCoord4d = (GL1Functions::ptr_glTexCoord4d) &glTexCoord4d;
	functions->texCoord4dv = (GL1Functions::ptr_glTexCoord4dv) &glTexCoord4dv;
	functions->texCoord4f = (GL1Functions::ptr_glTexCoord4f) &glTexCoord4f;
	functions->texCoord4fv = (GL1Functions::ptr_glTexCoord4fv) &glTexCoord4fv;
	functions->texCoord4i = (GL1Functions::ptr_glTexCoord4i) &glTexCoord4i;
	functions->texCoord4iv = (GL1Functions::ptr_glTexCoord4iv) &glTexCoord4iv;
	functions->texCoord4s = (GL1Functions::ptr_glTexCoord4s) &glTexCoord4s;
	functions->texCoord4sv = (GL1Functions::ptr_glTexCoord4sv) &glTexCoord4sv;
	functions->texCoordPointer = (GL1Functions::ptr_glTexCoordPointer) &glTexCoordPointer;
	functions->texEnvf = (GL1Functions::ptr_glTexEnvf) &glTexEnvf;
	functions->texEnvfv = (GL1Functions::ptr_glTexEnvfv) &glTexEnvfv;
	functions->texEnvi = (GL1Functions::ptr_glTexEnvi) &glTexEnvi;
	functions->texEnviv = (GL1Functions::ptr_glTexEnviv) &glTexEnviv;
	functions->texGend = (GL1Functions::ptr_glTexGend) &glTexGend;
	functions->texGendv = (GL1Functions::ptr_glTexGendv) &glTexGendv;
	functions->texGenf = (GL1Functions::ptr_glTexGenf) &glTexGenf;
	functions->texGenfv = (GL1Functions::ptr_glTexGenfv) &glTexGenfv;
	functions->texGeni = (GL1Functions::ptr_glTexGeni) &glTexGeni;
	functions->texGeniv = (GL1Functions::ptr_glTexGeniv) &glTexGeniv;
	functions->texImage1D = (GL1Functions::ptr_glTexImage1D) &glTexImage1D;
	functions->texImage2D = (GL1Functions::ptr_glTexImage2D) &glTexImage2D;
	functions->texParameterf = (GL1Functions::ptr_glTexParameterf) &glTexParameterf;
	functions->texParameterfv = (GL1Functions::ptr_glTexParameterfv) &glTexParameterfv;
	functions->texParameteri = (GL1Functions::ptr_glTexParameteri) &glTexParameteri;
	functions->texParameteriv = (GL1Functions::ptr_glTexParameteriv) &glTexParameteriv;
	functions->texSubImage1D = (GL1Functions::ptr_glTexSubImage1D) &glTexSubImage1D;
	functions->texSubImage2D = (GL1Functions::ptr_glTexSubImage2D) &glTexSubImage2D;
	functions->translated = (GL1Functions::ptr_glTranslated) &glTranslated;
	functions->translatef = (GL1Functions::ptr_glTranslatef) &glTranslatef;
	functions->vertex2d = (GL1Functions::ptr_glVertex2d) &glVertex2d;
	functions->vertex2dv = (GL1Functions::ptr_glVertex2dv) &glVertex2dv;
	functions->vertex2f = (GL1Functions::ptr_glVertex2f) &glVertex2f;
	functions->vertex2fv = (GL1Functions::ptr_glVertex2fv) &glVertex2fv;
	functions->vertex2i = (GL1Functions::ptr_glVertex2i) &glVertex2i;
	functions->vertex2iv = (GL1Functions::ptr_glVertex2iv) &glVertex2iv;
	functions->vertex2s = (GL1Functions::ptr_glVertex2s) &glVertex2s;
	functions->vertex2sv = (GL1Functions::ptr_glVertex2sv) &glVertex2sv;
	functions->vertex3d = (GL1Functions::ptr_glVertex3d) &glVertex3d;
	functions->vertex3dv = (GL1Functions::ptr_glVertex3dv) &glVertex3dv;
	functions->vertex3f = (GL1Functions::ptr_glVertex3f) &glVertex3f;
	functions->vertex3fv = (GL1Functions::ptr_glVertex3fv) &glVertex3fv;
	functions->vertex3i = (GL1Functions::ptr_glVertex3i) &glVertex3i;
	functions->vertex3iv = (GL1Functions::ptr_glVertex3iv) &glVertex3iv;
	functions->vertex3s = (GL1Functions::ptr_glVertex3s) &glVertex3s;
	functions->vertex3sv = (GL1Functions::ptr_glVertex3sv) &glVertex3sv;
	functions->vertex4d = (GL1Functions::ptr_glVertex4d) &glVertex4d;
	functions->vertex4dv = (GL1Functions::ptr_glVertex4dv) &glVertex4dv;
	functions->vertex4f = (GL1Functions::ptr_glVertex4f) &glVertex4f;
	functions->vertex4fv = (GL1Functions::ptr_glVertex4fv) &glVertex4fv;
	functions->vertex4i = (GL1Functions::ptr_glVertex4i) &glVertex4i;
	functions->vertex4iv = (GL1Functions::ptr_glVertex4iv) &glVertex4iv;
	functions->vertex4s = (GL1Functions::ptr_glVertex4s) &glVertex4s;
	functions->vertex4sv = (GL1Functions::ptr_glVertex4sv) &glVertex4sv;
	functions->vertexPointer = (GL1Functions::ptr_glVertexPointer) &glVertexPointer;
	functions->viewport = (GL1Functions::ptr_glViewport) &glViewport;
#else
	functions->accum = (GL1Functions::ptr_glAccum) GL1::get_proc_address("glAccum");
	functions->alphaFunc = (GL1Functions::ptr_glAlphaFunc) GL1::get_proc_address("glAlphaFunc");
	functions->areTexturesResident = (GL1Functions::ptr_glAreTexturesResident) GL1::get_proc_address("glAreTexturesResident");
	functions->arrayElement = (GL1Functions::ptr_glArrayElement) GL1::get_proc_address("glArrayElement");
	functions->begin = (GL1Functions::ptr_glBegin) GL1::get_proc_address("glBegin");
	functions->bindTexture = (GL1Functions::ptr_glBindTexture) GL1::get_proc_address("glBindTexture");
	functions->bitmap = (GL1Functions::ptr_glBitmap) GL1::get_proc_address("glBitmap");
	functions->blendFunc = (GL1Functions::ptr_glBlendFunc) GL1::get_proc_address("glBlendFunc");
	functions->callList = (GL1Functions::ptr_glCallList) GL1::get_proc_address("glCallList");
	functions->callLists = (GL1Functions::ptr_glCallLists) GL1::get_proc_address("glCallLists");
	functions->clear = (GL1Functions::ptr_glClear) GL1::get_proc_address("glClear");
	functions->clearAccum = (GL1Functions::ptr_glClearAccum) GL1::get_proc_address("glClearAccum");
	functions->clearColor = (GL1Functions::ptr_glClearColor) GL1::get_proc_address("glClearColor");
	functions->clearDepth = (GL1Functions::ptr_glClearDepth) GL1::get_proc_address("glClearDepth");
	functions->clearIndex = (GL1Functions::ptr_glClearIndex) GL1::get_proc_address("glClearIndex");
	functions->clearStencil = (GL1Functions::ptr_glClearStencil) GL1::get_proc_address("glClearStencil");
	functions->clipPlane = (GL1Functions::ptr_glClipPlane) GL1::get_proc_address("glClipPlane");
	functions->color3b = (GL1Functions::ptr_glColor3b) GL1::get_proc_address("glColor3b");
	functions->color3bv = (GL1Functions::ptr_glColor3bv) GL1::get_proc_address("glColor3bv");
	functions->color3d = (GL1Functions::ptr_glColor3d) GL1::get_proc_address("glColor3d");
	functions->color3dv = (GL1Functions::ptr_glColor3dv) GL1::get_proc_address("glColor3dv");
	functions->color3f = (GL1Functions::ptr_glColor3f) GL1::get_proc_address("glColor3f");
	functions->color3fv = (GL1Functions::ptr_glColor3fv) GL1::get_proc_address("glColor3fv");
	functions->color3i = (GL1Functions::ptr_glColor3i) GL1::get_proc_address("glColor3i");
	functions->color3iv = (GL1Functions::ptr_glColor3iv) GL1::get_proc_address("glColor3iv");
	functions->color3s = (GL1Functions::ptr_glColor3s) GL1::get_proc_address("glColor3s");
	functions->color3sv = (GL1Functions::ptr_glColor3sv) GL1::get_proc_address("glColor3sv");
	functions->color3ub = (GL1Functions::ptr_glColor3ub) GL1::get_proc_address("glColor3ub");
	functions->color3ubv = (GL1Functions::ptr_glColor3ubv) GL1::get_proc_address("glColor3ubv");
	functions->color3ui = (GL1Functions::ptr_glColor3ui) GL1::get_proc_address("glColor3ui");
	functions->color3uiv = (GL1Functions::ptr_glColor3uiv) GL1::get_proc_address("glColor3uiv");
	functions->color3us = (GL1Functions::ptr_glColor3us) GL1::get_proc_address("glColor3us");
	functions->color3usv = (GL1Functions::ptr_glColor3usv) GL1::get_proc_address("glColor3usv");
	functions->color4b = (GL1Functions::ptr_glColor4b) GL1::get_proc_address("glColor4b");
	functions->color4bv = (GL1Functions::ptr_glColor4bv) GL1::get_proc_address("glColor4bv");
	functions->color4d = (GL1Functions::ptr_glColor4d) GL1::get_proc_address("glColor4d");
	functions->color4dv = (GL1Functions::ptr_glColor4dv) GL1::get_proc_address("glColor4dv");
	functions->color4f = (GL1Functions::ptr_glColor4f) GL1::get_proc_address("glColor4f");
	functions->color4fv = (GL1Functions::ptr_glColor4fv) GL1::get_proc_address("glColor4fv");
	functions->color4i = (GL1Functions::ptr_glColor4i) GL1::get_proc_address("glColor4i");
	functions->color4iv = (GL1Functions::ptr_glColor4iv) GL1::get_proc_address("glColor4iv");
	functions->color4s = (GL1Functions::ptr_glColor4s) GL1::get_proc_address("glColor4s");
	functions->color4sv = (GL1Functions::ptr_glColor4sv) GL1::get_proc_address("glColor4sv");
	functions->color4ub = (GL1Functions::ptr_glColor4ub) GL1::get_proc_address("glColor4ub");
	functions->color4ubv = (GL1Functions::ptr_glColor4ubv) GL1::get_proc_address("glColor4ubv");
	functions->color4ui = (GL1Functions::ptr_glColor4ui) GL1::get_proc_address("glColor4ui");
	functions->color4uiv = (GL1Functions::ptr_glColor4uiv) GL1::get_proc_address("glColor4uiv");
	functions->color4us = (GL1Functions::ptr_glColor4us) GL1::get_proc_address("glColor4us");
	functions->color4usv = (GL1Functions::ptr_glColor4usv) GL1::get_proc_address("glColor4usv");
	functions->colorMask = (GL1Functions::ptr_glColorMask) GL1::get_proc_address("glColorMask");
	functions->colorMaterial = (GL1Functions::ptr_glColorMaterial) GL1::get_proc_address("glColorMaterial");
	functions->colorPointer = (GL1Functions::ptr_glColorPointer) GL1::get_proc_address("glColorPointer");
	functions->copyPixels = (GL1Functions::ptr_glCopyPixels) GL1::get_proc_address("glCopyPixels");
	functions->copyTexImage1D = (GL1Functions::ptr_glCopyTexImage1D) GL1::get_proc_address("glCopyTexImage1D");
	functions->copyTexImage2D = (GL1Functions::ptr_glCopyTexImage2D) GL1::get_proc_address("glCopyTexImage2D");
	functions->copyTexSubImage1D = (GL1Functions::ptr_glCopyTexSubImage1D) GL1::get_proc_address("glCopyTexSubImage1D");
	functions->copyTexSubImage2D = (GL1Functions::ptr_glCopyTexSubImage2D) GL1::get_proc_address("glCopyTexSubImage2D");
	functions->cullFace = (GL1Functions::ptr_glCullFace) GL1::get_proc_address("glCullFace");
	functions->deleteLists = (GL1Functions::ptr_glDeleteLists) GL1::get_proc_address("glDeleteLists");
	functions->deleteTextures = (GL1Functions::ptr_glDeleteTextures) GL1::get_proc_address("glDeleteTextures");
	functions->depthFunc = (GL1Functions::ptr_glDepthFunc) GL1::get_proc_address("glDepthFunc");
	functions->depthMask = (GL1Functions::ptr_glDepthMask) GL1::get_proc_address("glDepthMask");
	functions->depthRange = (GL1Functions::ptr_glDepthRange) GL1::get_proc_address("glDepthRange");
	functions->disable = (GL1Functions::ptr_glDisable) GL1::get_proc_address("glDisable");
	functions->disableClientState = (GL1Functions::ptr_glDisableClientState) GL1::get_proc_address("glDisableClientState");
	functions->drawArrays = (GL1Functions::ptr_glDrawArrays) GL1::get_proc_address("glDrawArrays");
	functions->drawBuffer = (GL1Functions::ptr_glDrawBuffer) GL1::get_proc_address("glDrawBuffer");
	functions->drawElements = (GL1Functions::ptr_glDrawElements) GL1::get_proc_address("glDrawElements");
	functions->drawPixels = (GL1Functions::ptr_glDrawPixels) GL1::get_proc_address("glDrawPixels");
	functions->edgeFlag = (GL1Functions::ptr_glEdgeFlag) GL1::get_proc_address("glEdgeFlag");
	functions->edgeFlagPointer = (GL1Functions::ptr_glEdgeFlagPointer) GL1::get_proc_address("glEdgeFlagPointer");
	functions->edgeFlagv = (GL1Functions::ptr_glEdgeFlagv) GL1::get_proc_address("glEdgeFlagv");
	functions->enable = (GL1Functions::ptr_glEnable) GL1::get_proc_address("glEnable");
	functions->enableClientState = (GL1Functions::ptr_glEnableClientState) GL1::get_proc_address("glEnableClientState");
	functions->end = (GL1Functions::ptr_glEnd) GL1::get_proc_address("glEnd");
	functions->endList = (GL1Functions::ptr_glEndList) GL1::get_proc_address("glEndList");
	functions->evalCoord1d = (GL1Functions::ptr_glEvalCoord1d) GL1::get_proc_address("glEvalCoord1d");
	functions->evalCoord1dv = (GL1Functions::ptr_glEvalCoord1dv) GL1::get_proc_address("glEvalCoord1dv");
	functions->evalCoord1f = (GL1Functions::ptr_glEvalCoord1f) GL1::get_proc_address("glEvalCoord1f");
	functions->evalCoord1fv = (GL1Functions::ptr_glEvalCoord1fv) GL1::get_proc_address("glEvalCoord1fv");
	functions->evalCoord2d = (GL1Functions::ptr_glEvalCoord2d) GL1::get_proc_address("glEvalCoord2d");
	functions->evalCoord2dv = (GL1Functions::ptr_glEvalCoord2dv) GL1::get_proc_address("glEvalCoord2dv");
	functions->evalCoord2f = (GL1Functions::ptr_glEvalCoord2f) GL1::get_proc_address("glEvalCoord2f");
	functions->evalCoord2fv = (GL1Functions::ptr_glEvalCoord2fv) GL1::get_proc_address("glEvalCoord2fv");
	functions->evalMesh1 = (GL1Functions::ptr_glEvalMesh1) GL1::get_proc_address("glEvalMesh1");
	functions->evalMesh2 = (GL1Functions::ptr_glEvalMesh2) GL1::get_proc_address("glEvalMesh2");
	functions->evalPoint1 = (GL1Functions::ptr_glEvalPoint1) GL1::get_proc_address("glEvalPoint1");
	functions->evalPoint2 = (GL1Functions::ptr_glEvalPoint2) GL1::get_proc_address("glEvalPoint2");
	functions->feedbackBuffer = (GL1Functions::ptr_glFeedbackBuffer) GL1::get_proc_address("glFeedbackBuffer");
	functions->finish = (GL1Functions::ptr_glFinish) GL1::get_proc_address("glFinish");
	functions->flush = (GL1Functions::ptr_glFlush) GL1::get_proc_address("glFlush");
	functions->fogf = (GL1Functions::ptr_glFogf) GL1::get_proc_address("glFogf");
	functions->fogfv = (GL1Functions::ptr_glFogfv) GL1::get_proc_address("glFogfv");
	functions->fogi = (GL1Functions::ptr_glFogi) GL1::get_proc_address("glFogi");
	functions->fogiv = (GL1Functions::ptr_glFogiv) GL1::get_proc_address("glFogiv");
	functions->frontFace = (GL1Functions::ptr_glFrontFace) GL1::get_proc_address("glFrontFace");
	functions->frustum = (GL1Functions::ptr_glFrustum) GL1::get_proc_address("glFrustum");
	functions->genLists = (GL1Functions::ptr_glGenLists) GL1::get_proc_address("glGenLists");
	functions->genTextures = (GL1Functions::ptr_glGenTextures) GL1::get_proc_address("glGenTextures");
	functions->getBooleanv = (GL1Functions::ptr_glGetBooleanv) GL1::get_proc_address("glGetBooleanv");
	functions->getClipPlane = (GL1Functions::ptr_glGetClipPlane) GL1::get_proc_address("glGetClipPlane");
	functions->getDoublev = (GL1Functions::ptr_glGetDoublev) GL1::get_proc_address("glGetDoublev");
	functions->getError = (GL1Functions::ptr_glGetError) GL1::get_proc_address("glGetError");
	functions->getFloatv = (GL1Functions::ptr_glGetFloatv) GL1::get_proc_address("glGetFloatv");
	functions->getIntegerv = (GL1Functions::ptr_glGetIntegerv) GL1::get_proc_address("glGetIntegerv");
	functions->getLightfv = (GL1Functions::ptr_glGetLightfv) GL1::get_proc_address("glGetLightfv");
	functions->getLightiv = (GL1Functions::ptr_glGetLightiv) GL1::get_proc_address("glGetLightiv");
	functions->getMapdv = (GL1Functions::ptr_glGetMapdv) GL1::get_proc_address("glGetMapdv");
	functions->getMapfv = (GL1Functions::ptr_glGetMapfv) GL1::get_proc_address("glGetMapfv");
	functions->getMapiv = (GL1Functions::ptr_glGetMapiv) GL1::get_proc_address("glGetMapiv");
	functions->getMaterialfv = (GL1Functions::ptr_glGetMaterialfv) GL1::get_proc_address("glGetMaterialfv");
	functions->getMaterialiv = (GL1Functions::ptr_glGetMaterialiv) GL1::get_proc_address("glGetMaterialiv");
	functions->getPixelMapfv = (GL1Functions::ptr_glGetPixelMapfv) GL1::get_proc_address("glGetPixelMapfv");
	functions->getPixelMapuiv = (GL1Functions::ptr_glGetPixelMapuiv) GL1::get_proc_address("glGetPixelMapuiv");
	functions->getPixelMapusv = (GL1Functions::ptr_glGetPixelMapusv) GL1::get_proc_address("glGetPixelMapusv");
	functions->getPointerv = (GL1Functions::ptr_glGetPointerv) GL1::get_proc_address("glGetPointerv");
	functions->getPolygonStipple = (GL1Functions::ptr_glGetPolygonStipple) GL1::get_proc_address("glGetPolygonStipple");
	functions->getString = (GL1Functions::ptr_glGetString) GL1::get_proc_address("glGetString");
	functions->getTexEnvfv = (GL1Functions::ptr_glGetTexEnvfv) GL1::get_proc_address("glGetTexEnvfv");
	functions->getTexEnviv = (GL1Functions::ptr_glGetTexEnviv) GL1::get_proc_address("glGetTexEnviv");
	functions->getTexGendv = (GL1Functions::ptr_glGetTexGendv) GL1::get_proc_address("glGetTexGendv");
	functions->getTexGenfv = (GL1Functions::ptr_glGetTexGenfv) GL1::get_proc_address("glGetTexGenfv");
	functions->getTexGeniv = (GL1Functions::ptr_glGetTexGeniv) GL1::get_proc_address("glGetTexGeniv");
	functions->getTexImage = (GL1Functions::ptr_glGetTexImage) GL1::get_proc_address("glGetTexImage");
	functions->getTexLevelParameterfv = (GL1Functions::ptr_glGetTexLevelParameterfv) GL1::get_proc_address("glGetTexLevelParameterfv");
	functions->getTexLevelParameteriv = (GL1Functions::ptr_glGetTexLevelParameteriv) GL1::get_proc_address("glGetTexLevelParameteriv");
	functions->getTexParameterfv = (GL1Functions::ptr_glGetTexParameterfv) GL1::get_proc_address("glGetTexParameterfv");
	functions->getTexParameteriv = (GL1Functions::ptr_glGetTexParameteriv) GL1::get_proc_address("glGetTexParameteriv");
	functions->hint = (GL1Functions::ptr_glHint) GL1::get_proc_address("glHint");
	functions->indexMask = (GL1Functions::ptr_glIndexMask) GL1::get_proc_address("glIndexMask");
	functions->indexPointer = (GL1Functions::ptr_glIndexPointer) GL1::get_proc_address("glIndexPointer");
	functions->indexd = (GL1Functions::ptr_glIndexd) GL1::get_proc_address("glIndexd");
	functions->indexdv = (GL1Functions::ptr_glIndexdv) GL1::get_proc_address("glIndexdv");
	functions->indexf = (GL1Functions::ptr_glIndexf) GL1::get_proc_address("glIndexf");
	functions->indexfv = (GL1Functions::ptr_glIndexfv) GL1::get_proc_address("glIndexfv");
	functions->indexi = (GL1Functions::ptr_glIndexi) GL1::get_proc_address("glIndexi");
	functions->indexiv = (GL1Functions::ptr_glIndexiv) GL1::get_proc_address("glIndexiv");
	functions->indexs = (GL1Functions::ptr_glIndexs) GL1::get_proc_address("glIndexs");
	functions->indexsv = (GL1Functions::ptr_glIndexsv) GL1::get_proc_address("glIndexsv");
	functions->indexub = (GL1Functions::ptr_glIndexub) GL1::get_proc_address("glIndexub");
	functions->indexubv = (GL1Functions::ptr_glIndexubv) GL1::get_proc_address("glIndexubv");
	functions->initNames = (GL1Functions::ptr_glInitNames) GL1::get_proc_address("glInitNames");
	functions->interleavedArrays = (GL1Functions::ptr_glInterleavedArrays) GL1::get_proc_address("glInterleavedArrays");
	functions->isEnabled = (GL1Functions::ptr_glIsEnabled) GL1::get_proc_address("glIsEnabled");
	functions->isList = (GL1Functions::ptr_glIsList) GL1::get_proc_address("glIsList");
	functions->isTexture = (GL1Functions::ptr_glIsTexture) GL1::get_proc_address("glIsTexture");
	functions->lightModelf = (GL1Functions::ptr_glLightModelf) GL1::get_proc_address("glLightModelf");
	functions->lightModelfv = (GL1Functions::ptr_glLightModelfv) GL1::get_proc_address("glLightModelfv");
	functions->lightModeli = (GL1Functions::ptr_glLightModeli) GL1::get_proc_address("glLightModeli");
	functions->lightModeliv = (GL1Functions::ptr_glLightModeliv) GL1::get_proc_address("glLightModeliv");
	functions->lightf = (GL1Functions::ptr_glLightf) GL1::get_proc_address("glLightf");
	functions->lightfv = (GL1Functions::ptr_glLightfv) GL1::get_proc_address("glLightfv");
	functions->lighti = (GL1Functions::ptr_glLighti) GL1::get_proc_address("glLighti");
	functions->lightiv = (GL1Functions::ptr_glLightiv) GL1::get_proc_address("glLightiv");
	functions->lineStipple = (GL1Functions::ptr_glLineStipple) GL1::get_proc_address("glLineStipple");
	functions->lineWidth = (GL1Functions::ptr_glLineWidth) GL1::get_proc_address("glLineWidth");
	functions->listBase = (GL1Functions::ptr_glListBase) GL1::get_proc_address("glListBase");
	functions->loadIdentity = (GL1Functions::ptr_glLoadIdentity) GL1::get_proc_address("glLoadIdentity");
	functions->loadMatrixd = (GL1Functions::ptr_glLoadMatrixd) GL1::get_proc_address("glLoadMatrixd");
	functions->loadMatrixf = (GL1Functions::ptr_glLoadMatrixf) GL1::get_proc_address("glLoadMatrixf");
	functions->loadName = (GL1Functions::ptr_glLoadName) GL1::get_proc_address("glLoadName");
	functions->logicOp = (GL1Functions::ptr_glLogicOp) GL1::get_proc_address("glLogicOp");
	functions->map1d = (GL1Functions::ptr_glMap1d) GL1::get_proc_address("glMap1d");
	functions->map1f = (GL1Functions::ptr_glMap1f) GL1::get_proc_address("glMap1f");
	functions->map2d = (GL1Functions::ptr_glMap2d) GL1::get_proc_address("glMap2d");
	functions->map2f = (GL1Functions::ptr_glMap2f) GL1::get_proc_address("glMap2f");
	functions->mapGrid1d = (GL1Functions::ptr_glMapGrid1d) GL1::get_proc_address("glMapGrid1d");
	functions->mapGrid1f = (GL1Functions::ptr_glMapGrid1f) GL1::get_proc_address("glMapGrid1f");
	functions->mapGrid2d = (GL1Functions::ptr_glMapGrid2d) GL1::get_proc_address("glMapGrid2d");
	functions->mapGrid2f = (GL1Functions::ptr_glMapGrid2f) GL1::get_proc_address("glMapGrid2f");
	functions->materialf = (GL1Functions::ptr_glMaterialf) GL1::get_proc_address("glMaterialf");
	functions->materialfv = (GL1Functions::ptr_glMaterialfv) GL1::get_proc_address("glMaterialfv");
	functions->materiali = (GL1Functions::ptr_glMateriali) GL1::get_proc_address("glMateriali");
	functions->materialiv = (GL1Functions::ptr_glMaterialiv) GL1::get_proc_address("glMaterialiv");
	functions->matrixMode = (GL1Functions::ptr_glMatrixMode) GL1::get_proc_address("glMatrixMode");
	functions->multMatrixd = (GL1Functions::ptr_glMultMatrixd) GL1::get_proc_address("glMultMatrixd");
	functions->multMatrixf = (GL1Functions::ptr_glMultMatrixf) GL1::get_proc_address("glMultMatrixf");
	functions->newList = (GL1Functions::ptr_glNewList) GL1::get_proc_address("glNewList");
	functions->normal3b = (GL1Functions::ptr_glNormal3b) GL1::get_proc_address("glNormal3b");
	functions->normal3bv = (GL1Functions::ptr_glNormal3bv) GL1::get_proc_address("glNormal3bv");
	functions->normal3d = (GL1Functions::ptr_glNormal3d) GL1::get_proc_address("glNormal3d");
	functions->normal3dv = (GL1Functions::ptr_glNormal3dv) GL1::get_proc_address("glNormal3dv");
	functions->normal3f = (GL1Functions::ptr_glNormal3f) GL1::get_proc_address("glNormal3f");
	functions->normal3fv = (GL1Functions::ptr_glNormal3fv) GL1::get_proc_address("glNormal3fv");
	functions->normal3i = (GL1Functions::ptr_glNormal3i) GL1::get_proc_address("glNormal3i");
	functions->normal3iv = (GL1Functions::ptr_glNormal3iv) GL1::get_proc_address("glNormal3iv");
	functions->normal3s = (GL1Functions::ptr_glNormal3s) GL1::get_proc_address("glNormal3s");
	functions->normal3sv = (GL1Functions::ptr_glNormal3sv) GL1::get_proc_address("glNormal3sv");
	functions->normalPointer = (GL1Functions::ptr_glNormalPointer) GL1::get_proc_address("glNormalPointer");
	functions->ortho = (GL1Functions::ptr_glOrtho) GL1::get_proc_address("glOrtho");
	functions->passThrough = (GL1Functions::ptr_glPassThrough) GL1::get_proc_address("glPassThrough");
	functions->pixelMapfv = (GL1Functions::ptr_glPixelMapfv) GL1::get_proc_address("glPixelMapfv");
	functions->pixelMapuiv = (GL1Functions::ptr_glPixelMapuiv) GL1::get_proc_address("glPixelMapuiv");
	functions->pixelMapusv = (GL1Functions::ptr_glPixelMapusv) GL1::get_proc_address("glPixelMapusv");
	functions->pixelStoref = (GL1Functions::ptr_glPixelStoref) GL1::get_proc_address("glPixelStoref");
	functions->pixelStorei = (GL1Functions::ptr_glPixelStorei) GL1::get_proc_address("glPixelStorei");
	functions->pixelTransferf = (GL1Functions::ptr_glPixelTransferf) GL1::get_proc_address("glPixelTransferf");
	functions->pixelTransferi = (GL1Functions::ptr_glPixelTransferi) GL1::get_proc_address("glPixelTransferi");
	functions->pixelZoom = (GL1Functions::ptr_glPixelZoom) GL1::get_proc_address("glPixelZoom");
	functions->pointSize = (GL1Functions::ptr_glPointSize) GL1::get_proc_address("glPointSize");
	functions->polygonMode = (GL1Functions::ptr_glPolygonMode) GL1::get_proc_address("glPolygonMode");
	functions->polygonOffset = (GL1Functions::ptr_glPolygonOffset) GL1::get_proc_address("glPolygonOffset");
	functions->polygonStipple = (GL1Functions::ptr_glPolygonStipple) GL1::get_proc_address("glPolygonStipple");
	functions->popAttrib = (GL1Functions::ptr_glPopAttrib) GL1::get_proc_address("glPopAttrib");
	functions->popClientAttrib = (GL1Functions::ptr_glPopClientAttrib) GL1::get_proc_address("glPopClientAttrib");
	functions->popMatrix = (GL1Functions::ptr_glPopMatrix) GL1::get_proc_address("glPopMatrix");
	functions->popName = (GL1Functions::ptr_glPopName) GL1::get_proc_address("glPopName");
	functions->prioritizeTextures = (GL1Functions::ptr_glPrioritizeTextures) GL1::get_proc_address("glPrioritizeTextures");
	functions->pushAttrib = (GL1Functions::ptr_glPushAttrib) GL1::get_proc_address("glPushAttrib");
	functions->pushClientAttrib = (GL1Functions::ptr_glPushClientAttrib) GL1::get_proc_address("glPushClientAttrib");
	functions->pushMatrix = (GL1Functions::ptr_glPushMatrix) GL1::get_proc_address("glPushMatrix");
	functions->pushName = (GL1Functions::ptr_glPushName) GL1::get_proc_address("glPushName");
	functions->rasterPos2d = (GL1Functions::ptr_glRasterPos2d) GL1::get_proc_address("glRasterPos2d");
	functions->rasterPos2dv = (GL1Functions::ptr_glRasterPos2dv) GL1::get_proc_address("glRasterPos2dv");
	functions->rasterPos2f = (GL1Functions::ptr_glRasterPos2f) GL1::get_proc_address("glRasterPos2f");
	functions->rasterPos2fv = (GL1Functions::ptr_glRasterPos2fv) GL1::get_proc_address("glRasterPos2fv");
	functions->rasterPos2i = (GL1Functions::ptr_glRasterPos2i) GL1::get_proc_address("glRasterPos2i");
	functions->rasterPos2iv = (GL1Functions::ptr_glRasterPos2iv) GL1::get_proc_address("glRasterPos2iv");
	functions->rasterPos2s = (GL1Functions::ptr_glRasterPos2s) GL1::get_proc_address("glRasterPos2s");
	functions->rasterPos2sv = (GL1Functions::ptr_glRasterPos2sv) GL1::get_proc_address("glRasterPos2sv");
	functions->rasterPos3d = (GL1Functions::ptr_glRasterPos3d) GL1::get_proc_address("glRasterPos3d");
	functions->rasterPos3dv = (GL1Functions::ptr_glRasterPos3dv) GL1::get_proc_address("glRasterPos3dv");
	functions->rasterPos3f = (GL1Functions::ptr_glRasterPos3f) GL1::get_proc_address("glRasterPos3f");
	functions->rasterPos3fv = (GL1Functions::ptr_glRasterPos3fv) GL1::get_proc_address("glRasterPos3fv");
	functions->rasterPos3i = (GL1Functions::ptr_glRasterPos3i) GL1::get_proc_address("glRasterPos3i");
	functions->rasterPos3iv = (GL1Functions::ptr_glRasterPos3iv) GL1::get_proc_address("glRasterPos3iv");
	functions->rasterPos3s = (GL1Functions::ptr_glRasterPos3s) GL1::get_proc_address("glRasterPos3s");
	functions->rasterPos3sv = (GL1Functions::ptr_glRasterPos3sv) GL1::get_proc_address("glRasterPos3sv");
	functions->rasterPos4d = (GL1Functions::ptr_glRasterPos4d) GL1::get_proc_address("glRasterPos4d");
	functions->rasterPos4dv = (GL1Functions::ptr_glRasterPos4dv) GL1::get_proc_address("glRasterPos4dv");
	functions->rasterPos4f = (GL1Functions::ptr_glRasterPos4f) GL1::get_proc_address("glRasterPos4f");
	functions->rasterPos4fv = (GL1Functions::ptr_glRasterPos4fv) GL1::get_proc_address("glRasterPos4fv");
	functions->rasterPos4i = (GL1Functions::ptr_glRasterPos4i) GL1::get_proc_address("glRasterPos4i");
	functions->rasterPos4iv = (GL1Functions::ptr_glRasterPos4iv) GL1::get_proc_address("glRasterPos4iv");
	functions->rasterPos4s = (GL1Functions::ptr_glRasterPos4s) GL1::get_proc_address("glRasterPos4s");
	functions->rasterPos4sv = (GL1Functions::ptr_glRasterPos4sv) GL1::get_proc_address("glRasterPos4sv");
	functions->readBuffer = (GL1Functions::ptr_glReadBuffer) GL1::get_proc_address("glReadBuffer");
	functions->readPixels = (GL1Functions::ptr_glReadPixels) GL1::get_proc_address("glReadPixels");
	functions->rectd = (GL1Functions::ptr_glRectd) GL1::get_proc_address("glRectd");
	functions->rectdv = (GL1Functions::ptr_glRectdv) GL1::get_proc_address("glRectdv");
	functions->rectf = (GL1Functions::ptr_glRectf) GL1::get_proc_address("glRectf");
	functions->rectfv = (GL1Functions::ptr_glRectfv) GL1::get_proc_address("glRectfv");
	functions->recti = (GL1Functions::ptr_glRecti) GL1::get_proc_address("glRecti");
	functions->rectiv = (GL1Functions::ptr_glRectiv) GL1::get_proc_address("glRectiv");
	functions->rects = (GL1Functions::ptr_glRects) GL1::get_proc_address("glRects");
	functions->rectsv = (GL1Functions::ptr_glRectsv) GL1::get_proc_address("glRectsv");
	functions->renderMode = (GL1Functions::ptr_glRenderMode) GL1::get_proc_address("glRenderMode");
	functions->rotated = (GL1Functions::ptr_glRotated) GL1::get_proc_address("glRotated");
	functions->rotatef = (GL1Functions::ptr_glRotatef) GL1::get_proc_address("glRotatef");
	functions->scaled = (GL1Functions::ptr_glScaled) GL1::get_proc_address("glScaled");
	functions->scalef = (GL1Functions::ptr_glScalef) GL1::get_proc_address("glScalef");
	functions->scissor = (GL1Functions::ptr_glScissor) GL1::get_proc_address("glScissor");
	functions->selectBuffer = (GL1Functions::ptr_glSelectBuffer) GL1::get_proc_address("glSelectBuffer");
	functions->shadeModel = (GL1Functions::ptr_glShadeModel) GL1::get_proc_address("glShadeModel");
	functions->stencilFunc = (GL1Functions::ptr_glStencilFunc) GL1::get_proc_address("glStencilFunc");
	functions->stencilMask = (GL1Functions::ptr_glStencilMask) GL1::get_proc_address("glStencilMask");
	functions->stencilMaskSeparate = (GL1Functions::ptr_glStencilMaskSeparate) GL1::get_proc_address("glStencilMaskSeparate");
	functions->stencilOp = (GL1Functions::ptr_glStencilOp) GL1::get_proc_address("glStencilOp");
	functions->texCoord1d = (GL1Functions::ptr_glTexCoord1d) GL1::get_proc_address("glTexCoord1d");
	functions->texCoord1dv = (GL1Functions::ptr_glTexCoord1dv) GL1::get_proc_address("glTexCoord1dv");
	functions->texCoord1f = (GL1Functions::ptr_glTexCoord1f) GL1::get_proc_address("glTexCoord1f");
	functions->texCoord1fv = (GL1Functions::ptr_glTexCoord1fv) GL1::get_proc_address("glTexCoord1fv");
	functions->texCoord1i = (GL1Functions::ptr_glTexCoord1i) GL1::get_proc_address("glTexCoord1i");
	functions->texCoord1iv = (GL1Functions::ptr_glTexCoord1iv) GL1::get_proc_address("glTexCoord1iv");
	functions->texCoord1s = (GL1Functions::ptr_glTexCoord1s) GL1::get_proc_address("glTexCoord1s");
	functions->texCoord1sv = (GL1Functions::ptr_glTexCoord1sv) GL1::get_proc_address("glTexCoord1sv");
	functions->texCoord2d = (GL1Functions::ptr_glTexCoord2d) GL1::get_proc_address("glTexCoord2d");
	functions->texCoord2dv = (GL1Functions::ptr_glTexCoord2dv) GL1::get_proc_address("glTexCoord2dv");
	functions->texCoord2f = (GL1Functions::ptr_glTexCoord2f) GL1::get_proc_address("glTexCoord2f");
	functions->texCoord2fv = (GL1Functions::ptr_glTexCoord2fv) GL1::get_proc_address("glTexCoord2fv");
	functions->texCoord2i = (GL1Functions::ptr_glTexCoord2i) GL1::get_proc_address("glTexCoord2i");
	functions->texCoord2iv = (GL1Functions::ptr_glTexCoord2iv) GL1::get_proc_address("glTexCoord2iv");
	functions->texCoord2s = (GL1Functions::ptr_glTexCoord2s) GL1::get_proc_address("glTexCoord2s");
	functions->texCoord2sv = (GL1Functions::ptr_glTexCoord2sv) GL1::get_proc_address("glTexCoord2sv");
	functions->texCoord3d = (GL1Functions::ptr_glTexCoord3d) GL1::get_proc_address("glTexCoord3d");
	functions->texCoord3dv = (GL1Functions::ptr_glTexCoord3dv) GL1::get_proc_address("glTexCoord3dv");
	functions->texCoord3f = (GL1Functions::ptr_glTexCoord3f) GL1::get_proc_address("glTexCoord3f");
	functions->texCoord3fv = (GL1Functions::ptr_glTexCoord3fv) GL1::get_proc_address("glTexCoord3fv");
	functions->texCoord3i = (GL1Functions::ptr_glTexCoord3i) GL1::get_proc_address("glTexCoord3i");
	functions->texCoord3iv = (GL1Functions::ptr_glTexCoord3iv) GL1::get_proc_address("glTexCoord3iv");
	functions->texCoord3s = (GL1Functions::ptr_glTexCoord3s) GL1::get_proc_address("glTexCoord3s");
	functions->texCoord3sv = (GL1Functions::ptr_glTexCoord3sv) GL1::get_proc_address("glTexCoord3sv");
	functions->texCoord4d = (GL1Functions::ptr_glTexCoord4d) GL1::get_proc_address("glTexCoord4d");
	functions->texCoord4dv = (GL1Functions::ptr_glTexCoord4dv) GL1::get_proc_address("glTexCoord4dv");
	functions->texCoord4f = (GL1Functions::ptr_glTexCoord4f) GL1::get_proc_address("glTexCoord4f");
	functions->texCoord4fv = (GL1Functions::ptr_glTexCoord4fv) GL1::get_proc_address("glTexCoord4fv");
	functions->texCoord4i = (GL1Functions::ptr_glTexCoord4i) GL1::get_proc_address("glTexCoord4i");
	functions->texCoord4iv = (GL1Functions::ptr_glTexCoord4iv) GL1::get_proc_address("glTexCoord4iv");
	functions->texCoord4s = (GL1Functions::ptr_glTexCoord4s) GL1::get_proc_address("glTexCoord4s");
	functions->texCoord4sv = (GL1Functions::ptr_glTexCoord4sv) GL1::get_proc_address("glTexCoord4sv");
	functions->texCoordPointer = (GL1Functions::ptr_glTexCoordPointer) GL1::get_proc_address("glTexCoordPointer");
	functions->texEnvf = (GL1Functions::ptr_glTexEnvf) GL1::get_proc_address("glTexEnvf");
	functions->texEnvfv = (GL1Functions::ptr_glTexEnvfv) GL1::get_proc_address("glTexEnvfv");
	functions->texEnvi = (GL1Functions::ptr_glTexEnvi) GL1::get_proc_address("glTexEnvi");
	functions->texEnviv = (GL1Functions::ptr_glTexEnviv) GL1::get_proc_address("glTexEnviv");
	functions->texGend = (GL1Functions::ptr_glTexGend) GL1::get_proc_address("glTexGend");
	functions->texGendv = (GL1Functions::ptr_glTexGendv) GL1::get_proc_address("glTexGendv");
	functions->texGenf = (GL1Functions::ptr_glTexGenf) GL1::get_proc_address("glTexGenf");
	functions->texGenfv = (GL1Functions::ptr_glTexGenfv) GL1::get_proc_address("glTexGenfv");
	functions->texGeni = (GL1Functions::ptr_glTexGeni) GL1::get_proc_address("glTexGeni");
	functions->texGeniv = (GL1Functions::ptr_glTexGeniv) GL1::get_proc_address("glTexGeniv");
	functions->texImage1D = (GL1Functions::ptr_glTexImage1D) GL1::get_proc_address("glTexImage1D");
	functions->texImage2D = (GL1Functions::ptr_glTexImage2D) GL1::get_proc_address("glTexImage2D");
	functions->texParameterf = (GL1Functions::ptr_glTexParameterf) GL1::get_proc_address("glTexParameterf");
	functions->texParameterfv = (GL1Functions::ptr_glTexParameterfv) GL1::get_proc_address("glTexParameterfv");
	functions->texParameteri = (GL1Functions::ptr_glTexParameteri) GL1::get_proc_address("glTexParameteri");
	functions->texParameteriv = (GL1Functions::ptr_glTexParameteriv) GL1::get_proc_address("glTexParameteriv");
	functions->texSubImage1D = (GL1Functions::ptr_glTexSubImage1D) GL1::get_proc_address("glTexSubImage1D");
	functions->texSubImage2D = (GL1Functions::ptr_glTexSubImage2D) GL1::get_proc_address("glTexSubImage2D");
	functions->translated = (GL1Functions::ptr_glTranslated) GL1::get_proc_address("glTranslated");
	functions->translatef = (GL1Functions::ptr_glTranslatef) GL1::get_proc_address("glTranslatef");
	functions->vertex2d = (GL1Functions::ptr_glVertex2d) GL1::get_proc_address("glVertex2d");
	functions->vertex2dv = (GL1Functions::ptr_glVertex2dv) GL1::get_proc_address("glVertex2dv");
	functions->vertex2f = (GL1Functions::ptr_glVertex2f) GL1::get_proc_address("glVertex2f");
	functions->vertex2fv = (GL1Functions::ptr_glVertex2fv) GL1::get_proc_address("glVertex2fv");
	functions->vertex2i = (GL1Functions::ptr_glVertex2i) GL1::get_proc_address("glVertex2i");
	functions->vertex2iv = (GL1Functions::ptr_glVertex2iv) GL1::get_proc_address("glVertex2iv");
	functions->vertex2s = (GL1Functions::ptr_glVertex2s) GL1::get_proc_address("glVertex2s");
	functions->vertex2sv = (GL1Functions::ptr_glVertex2sv) GL1::get_proc_address("glVertex2sv");
	functions->vertex3d = (GL1Functions::ptr_glVertex3d) GL1::get_proc_address("glVertex3d");
	functions->vertex3dv = (GL1Functions::ptr_glVertex3dv) GL1::get_proc_address("glVertex3dv");
	functions->vertex3f = (GL1Functions::ptr_glVertex3f) GL1::get_proc_address("glVertex3f");
	functions->vertex3fv = (GL1Functions::ptr_glVertex3fv) GL1::get_proc_address("glVertex3fv");
	functions->vertex3i = (GL1Functions::ptr_glVertex3i) GL1::get_proc_address("glVertex3i");
	functions->vertex3iv = (GL1Functions::ptr_glVertex3iv) GL1::get_proc_address("glVertex3iv");
	functions->vertex3s = (GL1Functions::ptr_glVertex3s) GL1::get_proc_address("glVertex3s");
	functions->vertex3sv = (GL1Functions::ptr_glVertex3sv) GL1::get_proc_address("glVertex3sv");
	functions->vertex4d = (GL1Functions::ptr_glVertex4d) GL1::get_proc_address("glVertex4d");
	functions->vertex4dv = (GL1Functions::ptr_glVertex4dv) GL1::get_proc_address("glVertex4dv");
	functions->vertex4f = (GL1Functions::ptr_glVertex4f) GL1::get_proc_address("glVertex4f");
	functions->vertex4fv = (GL1Functions::ptr_glVertex4fv) GL1::get_proc_address("glVertex4fv");
	functions->vertex4i = (GL1Functions::ptr_glVertex4i) GL1::get_proc_address("glVertex4i");
	functions->vertex4iv = (GL1Functions::ptr_glVertex4iv) GL1::get_proc_address("glVertex4iv");
	functions->vertex4s = (GL1Functions::ptr_glVertex4s) GL1::get_proc_address("glVertex4s");
	functions->vertex4sv = (GL1Functions::ptr_glVertex4sv) GL1::get_proc_address("glVertex4sv");
	functions->vertexPointer = (GL1Functions::ptr_glVertexPointer) GL1::get_proc_address("glVertexPointer");
	functions->viewport = (GL1Functions::ptr_glViewport) GL1::get_proc_address("glViewport");
#endif

	// Binds for OpenGL 1.2:
	functions->drawRangeElements = (GL1Functions::ptr_glDrawRangeElementsEXT) GL1::get_proc_address("glDrawRangeElementsEXT");
	functions->texImage3D = (GL1Functions::ptr_glTexImage3DEXT) GL1::get_proc_address("glTexImage3DEXT");
	functions->texSubImage3D = (GL1Functions::ptr_glTexSubImage3DEXT) GL1::get_proc_address("glTexSubImage3DEXT");
	functions->copyTexSubImage3D = (GL1Functions::ptr_glCopyTexSubImage3DEXT) GL1::get_proc_address("glCopyTexSubImage3DEXT");
	functions->colorTable = (GL1Functions::ptr_glColorTableSGI) GL1::get_proc_address("glColorTableSGI");
	functions->copyColorTable = (GL1Functions::ptr_glCopyColorTableSGI) GL1::get_proc_address("glCopyColorTableSGI");
	functions->colorTableParameteriv = (GL1Functions::ptr_glColorTableParameterivSGI) GL1::get_proc_address("glColorTableParameterivSGI");
	functions->colorTableParameterfv = (GL1Functions::ptr_glColorTableParameterfvSGI) GL1::get_proc_address("glColorTableParameterfvSGI");
	functions->getColorTable = (GL1Functions::ptr_glGetColorTableSGI) GL1::get_proc_address("glGetColorTableSGI");
	functions->getColorTableParameteriv = (GL1Functions::ptr_glGetColorTableParameterivSGI) GL1::get_proc_address("glGetColorTableParameterivSGI");
	functions->getColorTableParameterfv = (GL1Functions::ptr_glGetColorTableParameterfvSGI) GL1::get_proc_address("glGetColorTableParameterfvSGI");
	functions->colorSubTable = (GL1Functions::ptr_glColorSubTableEXT) GL1::get_proc_address("glColorSubTableEXT");
	functions->copyColorSubTable = (GL1Functions::ptr_glCopyColorSubTableEXT) GL1::get_proc_address("glCopyColorSubTableEXT");
	functions->convolutionFilter1D = (GL1Functions::ptr_glConvolutionFilter1DEXT) GL1::get_proc_address("glConvolutionFilter1DEXT");
	functions->convolutionFilter2D = (GL1Functions::ptr_glConvolutionFilter2DEXT) GL1::get_proc_address("glConvolutionFilter2DEXT");
	functions->copyConvolutionFilter1D = (GL1Functions::ptr_glCopyConvolutionFilter1DEXT) GL1::get_proc_address("glCopyConvolutionFilter1DEXT");
	functions->copyConvolutionFilter2D = (GL1Functions::ptr_glCopyConvolutionFilter2DEXT) GL1::get_proc_address("glCopyConvolutionFilter2DEXT");
	functions->getConvolutionFilter = (GL1Functions::ptr_glGetConvolutionFilterEXT) GL1::get_proc_address("glGetConvolutionFilterEXT");
	functions->separableFilter2D = (GL1Functions::ptr_glSeparableFilter2DEXT) GL1::get_proc_address("glSeparableFilter2DEXT");
	functions->getSeparableFilter = (GL1Functions::ptr_glGetSeparableFilterEXT) GL1::get_proc_address("glGetSeparableFilterEXT");
	functions->convolutionParameteri = (GL1Functions::ptr_glConvolutionParameteriEXT) GL1::get_proc_address("glConvolutionParameteriEXT");
	functions->convolutionParameteriv = (GL1Functions::ptr_glConvolutionParameterivEXT) GL1::get_proc_address("glConvolutionParameterivEXT");
	functions->convolutionParameterf = (GL1Functions::ptr_glConvolutionParameterfEXT) GL1::get_proc_address("glConvolutionParameterfEXT");
	functions->convolutionParameterfv = (GL1Functions::ptr_glConvolutionParameterfvEXT) GL1::get_proc_address("glConvolutionParameterfvEXT");
	functions->getConvolutionParameteriv = (GL1Functions::ptr_glGetConvolutionParameterivEXT) GL1::get_proc_address("glGetConvolutionParameterivEXT");
	functions->getConvolutionParameterfv = (GL1Functions::ptr_glGetConvolutionParameterfvEXT) GL1::get_proc_address("glGetConvolutionParameterfvEXT");
	functions->histogram = (GL1Functions::ptr_glHistogramEXT) GL1::get_proc_address("glHistogramEXT");
	functions->resetHistogram = (GL1Functions::ptr_glResetHistogramEXT) GL1::get_proc_address("glResetHistogramEXT");
	functions->getHistogram = (GL1Functions::ptr_glGetHistogramEXT) GL1::get_proc_address("glGetHistogramEXT");
	functions->getHistogramParameteriv = (GL1Functions::ptr_glGetHistogramParameterivEXT) GL1::get_proc_address("glGetHistogramParameterivEXT");
	functions->getHistogramParameterfv = (GL1Functions::ptr_glGetHistogramParameterfvEXT) GL1::get_proc_address("glGetHistogramParameterfvEXT");
	functions->minmax = (GL1Functions::ptr_glMinmaxEXT) GL1::get_proc_address("glMinmaxEXT");
	functions->resetMinmax = (GL1Functions::ptr_glResetMinmaxEXT) GL1::get_proc_address("glResetMinmaxEXT");
	functions->getMinmax = (GL1Functions::ptr_glGetMinmaxEXT) GL1::get_proc_address("glGetMinmaxEXT");
	functions->getMinmaxParameteriv = (GL1Functions::ptr_glGetMinmaxParameterivEXT) GL1::get_proc_address("glGetMinmaxParameterivEXT");
	functions->getMinmaxParameterfv = (GL1Functions::ptr_glGetMinmaxParameterfvEXT) GL1::get_proc_address("glGetMinmaxParameterfvEXT");
	functions->blendColor = (GL1Functions::ptr_glBlendColorEXT) GL1::get_proc_address("glBlendColorEXT");
	functions->blendEquation = (GL1Functions::ptr_glBlendEquationEXT) GL1::get_proc_address("glBlendEquationEXT");

	// Binds for OpenGL 1.2.1:
	functions->activeTexture = (GL1Functions::ptr_glActiveTextureARB) GL1::get_proc_address("glActiveTextureARB");
	functions->clientActiveTexture = (GL1Functions::ptr_glClientActiveTextureARB) GL1::get_proc_address("glClientActiveTextureARB");
	functions->multiTexCoord1d = (GL1Functions::ptr_glMultiTexCoord1dARB) GL1::get_proc_address("glMultiTexCoord1dARB");
	functions->multiTexCoord1dv = (GL1Functions::ptr_glMultiTexCoord1dvARB) GL1::get_proc_address("glMultiTexCoord1dvARB");
	functions->multiTexCoord1f = (GL1Functions::ptr_glMultiTexCoord1fARB) GL1::get_proc_address("glMultiTexCoord1fARB");
	functions->multiTexCoord1fv = (GL1Functions::ptr_glMultiTexCoord1fvARB) GL1::get_proc_address("glMultiTexCoord1fvARB");
	functions->multiTexCoord1i = (GL1Functions::ptr_glMultiTexCoord1iARB) GL1::get_proc_address("glMultiTexCoord1iARB");
	functions->multiTexCoord1iv = (GL1Functions::ptr_glMultiTexCoord1ivARB) GL1::get_proc_address("glMultiTexCoord1ivARB");
	functions->multiTexCoord1s = (GL1Functions::ptr_glMultiTexCoord1sARB) GL1::get_proc_address("glMultiTexCoord1sARB");
	functions->multiTexCoord1sv = (GL1Functions::ptr_glMultiTexCoord1svARB) GL1::get_proc_address("glMultiTexCoord1svARB");
	functions->multiTexCoord2d = (GL1Functions::ptr_glMultiTexCoord2dARB) GL1::get_proc_address("glMultiTexCoord2dARB");
	functions->multiTexCoord2dv = (GL1Functions::ptr_glMultiTexCoord2dvARB) GL1::get_proc_address("glMultiTexCoord2dvARB");
	functions->multiTexCoord2f = (GL1Functions::ptr_glMultiTexCoord2fARB) GL1::get_proc_address("glMultiTexCoord2fARB");
	functions->multiTexCoord2fv = (GL1Functions::ptr_glMultiTexCoord2fvARB) GL1::get_proc_address("glMultiTexCoord2fvARB");
	functions->multiTexCoord2i = (GL1Functions::ptr_glMultiTexCoord2iARB) GL1::get_proc_address("glMultiTexCoord2iARB");
	functions->multiTexCoord2iv = (GL1Functions::ptr_glMultiTexCoord2ivARB) GL1::get_proc_address("glMultiTexCoord2ivARB");
	functions->multiTexCoord2s = (GL1Functions::ptr_glMultiTexCoord2sARB) GL1::get_proc_address("glMultiTexCoord2sARB");
	functions->multiTexCoord2sv = (GL1Functions::ptr_glMultiTexCoord2svARB) GL1::get_proc_address("glMultiTexCoord2svARB");
	functions->multiTexCoord3d = (GL1Functions::ptr_glMultiTexCoord3dARB) GL1::get_proc_address("glMultiTexCoord3dARB");
	functions->multiTexCoord3dv = (GL1Functions::ptr_glMultiTexCoord3dvARB) GL1::get_proc_address("glMultiTexCoord3dvARB");
	functions->multiTexCoord3f = (GL1Functions::ptr_glMultiTexCoord3fARB) GL1::get_proc_address("glMultiTexCoord3fARB");
	functions->multiTexCoord3fv = (GL1Functions::ptr_glMultiTexCoord3fvARB) GL1::get_proc_address("glMultiTexCoord3fvARB");
	functions->multiTexCoord3i = (GL1Functions::ptr_glMultiTexCoord3iARB) GL1::get_proc_address("glMultiTexCoord3iARB");
	functions->multiTexCoord3iv = (GL1Functions::ptr_glMultiTexCoord3ivARB) GL1::get_proc_address("glMultiTexCoord3ivARB");
	functions->multiTexCoord3s = (GL1Functions::ptr_glMultiTexCoord3sARB) GL1::get_proc_address("glMultiTexCoord3sARB");
	functions->multiTexCoord3sv = (GL1Functions::ptr_glMultiTexCoord3svARB) GL1::get_proc_address("glMultiTexCoord3svARB");
	functions->multiTexCoord4d = (GL1Functions::ptr_glMultiTexCoord4dARB) GL1::get_proc_address("glMultiTexCoord4dARB");
	functions->multiTexCoord4dv = (GL1Functions::ptr_glMultiTexCoord4dvARB) GL1::get_proc_address("glMultiTexCoord4dvARB");
	functions->multiTexCoord4f = (GL1Functions::ptr_glMultiTexCoord4fARB) GL1::get_proc_address("glMultiTexCoord4fARB");
	functions->multiTexCoord4fv = (GL1Functions::ptr_glMultiTexCoord4fvARB) GL1::get_proc_address("glMultiTexCoord4fvARB");
	functions->multiTexCoord4i = (GL1Functions::ptr_glMultiTexCoord4iARB) GL1::get_proc_address("glMultiTexCoord4iARB");
	functions->multiTexCoord4iv = (GL1Functions::ptr_glMultiTexCoord4ivARB) GL1::get_proc_address("glMultiTexCoord4ivARB");
	functions->multiTexCoord4s = (GL1Functions::ptr_glMultiTexCoord4sARB) GL1::get_proc_address("glMultiTexCoord4sARB");
	functions->multiTexCoord4sv = (GL1Functions::ptr_glMultiTexCoord4svARB) GL1::get_proc_address("glMultiTexCoord4svARB");
	// Binds for OpenGL 1.3:

	functions->compressedTexImage1D = (GL1Functions::ptr_glCompressedTexImage1DARB) GL1::get_proc_address("glCompressedTexImage1DARB");
	functions->compressedTexImage2D = (GL1Functions::ptr_glCompressedTexImage2DARB) GL1::get_proc_address("glCompressedTexImage2DARB");
	functions->compressedTexImage3D = (GL1Functions::ptr_glCompressedTexImage3DARB) GL1::get_proc_address("glCompressedTexImage3DARB");
	functions->compressedTexSubImage1D = (GL1Functions::ptr_glCompressedTexSubImage1DARB) GL1::get_proc_address("glCompressedTexSubImage1DARB");
	functions->compressedTexSubImage2D = (GL1Functions::ptr_glCompressedTexSubImage2DARB) GL1::get_proc_address("glCompressedTexSubImage2DARB");
	functions->compressedTexSubImage3D = (GL1Functions::ptr_glCompressedTexSubImage3DARB) GL1::get_proc_address("glCompressedTexSubImage3DARB");
	functions->getCompressedTexImage = (GL1Functions::ptr_glGetCompressedTexImageARB) GL1::get_proc_address("glGetCompressedTexImageARB");
	functions->sampleCoverage = (GL1Functions::ptr_glSampleCoverageARB) GL1::get_proc_address("glSampleCoverageARB");
	functions->glLoadTransposeMatrixd = (GL1Functions::ptr_glLoadTransposeMatrixdARB) GL1::get_proc_address("glLoadTransposeMatrixdARB");
	functions->glLoadTransposeMatrixf = (GL1Functions::ptr_glLoadTransposeMatrixfARB) GL1::get_proc_address("glLoadTransposeMatrixfARB");
	functions->glMultTransposeMatrixd = (GL1Functions::ptr_glMultTransposeMatrixdARB) GL1::get_proc_address("glMultTransposeMatrixdARB");
	functions->glMultTransposeMatrixf = (GL1Functions::ptr_glMultTransposeMatrixfARB) GL1::get_proc_address("glMultTransposeMatrixfARB");

#ifdef WIN32
	functions->wglCreatePbufferARB = (GL1Functions::ptr_wglCreatePbufferARB) GL1::get_proc_address("wglCreatePbufferARB");
	functions->wglGetPbufferDCARB = (GL1Functions::ptr_wglGetPbufferDCARB) GL1::get_proc_address("wglGetPbufferDCARB");
	functions->wglReleasePbufferDCARB = (GL1Functions::ptr_wglReleasePbufferDCARB) GL1::get_proc_address("wglReleasePbufferDCARB");
	functions->wglDestroyPbufferARB = (GL1Functions::ptr_wglDestroyPbufferARB) GL1::get_proc_address("wglDestroyPbufferARB");
	functions->wglQueryPbufferARB = (GL1Functions::ptr_wglQueryPbufferARB) GL1::get_proc_address("wglQueryPbufferARB");
#endif

	// OpenGL 1.4, loaded as an extension. Required to make ClanLib usable for certain blending operations
	functions->blendFuncSeparate = (GL1Functions::ptr_glBlendFuncSeparate) GL1::get_proc_address("glBlendFuncSeparate");
	if (!functions->blendFuncSeparate)
	{
		functions->blendFuncSeparate = (GL1Functions::ptr_glBlendFuncSeparate) GL1::get_proc_address("glBlendFuncSeparateEXT");
	}


	return functions;
}

}

/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/GL/opengl_target.h"
#include "API/Core/System/exception.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/texture.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "Display/Render/graphic_context_impl.h"
#include "GL3/gl3_graphic_context_provider.h"
#include "GL3/gl3_texture_provider.h"
#include <map>

#if defined(__IOS__)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(__APPLE__)
#include "Platform/OSX/opengl_window_provider_osx.h"
#elif defined(__ANDROID__)
#include <EGL/egl.h>
#include <GLES/gl.h>
#else
#include <GL/gl.h>
#endif

#if !defined(WIN32) && !defined(__APPLE__) && !defined(__ANDROID__)
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#include "Platform/GLX/opengl_window_provider_glx.h"
#endif

namespace clan
{
	cl_tls_variable GLFunctions *OpenGL::functions = nullptr;
	cl_tls_variable const OpenGLGraphicContextProvider * cl_active_opengl_gc = nullptr;
	static std::recursive_mutex cl_function_map_mutex;

	// A fix for a compiler bug with compiler version 13.00.9466
	#if _MSC_VER > 1300
	typedef std::map<const OpenGLGraphicContextProvider * const, GLFunctions *> cl_function_map_type;
	#else
	typedef std::map<const OpenGLGraphicContextProvider *, GLFunctions *> cl_function_map_type;
	#endif

	static cl_function_map_type cl_function_map;

	GLFunctions *cl_setup_binds();


	void OpenGL::check_error()
	{
		GLenum last_error = glGetError();
		if (last_error != GL_NO_ERROR)
		{
			switch (last_error)
			{
				case GL_INVALID_ENUM:
					throw Exception("OpenGL INVALID_ENUM - An unacceptable value is specified for an enumerated argument.");
				case GL_INVALID_VALUE:
					throw Exception("OpenGL INVALID_VALUE - A numeric argument is out of range.");
				case GL_INVALID_OPERATION:
					throw Exception("OpenGL INVALID_OPERATION - The specified operation is not allowed in the current state");
				case GL_OUT_OF_MEMORY:
					throw Exception("OpenGL OUT_OF_MEMORY - There is not enough memory left to execute the command");
				default:
					throw Exception(string_format("Unknown OpenGL Error - %1", last_error));
			}
		}

	}

	std::vector<MessageLog_GL> OpenGL::get_message_log(GLuint numMsgs)
	{
		if (!glGetDebugMessageLog)
			return std::vector<MessageLog_GL>();

		GLint maxMsgLen = 0;
		glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);

		std::vector<GLchar> msgData(numMsgs * maxMsgLen);
		std::vector<GLenum> sources(numMsgs);
		std::vector<GLenum> types(numMsgs);
		std::vector<GLenum> severities(numMsgs);
		std::vector<GLuint> ids(numMsgs);
		std::vector<GLsizei> lengths(numMsgs);

		GLuint numFound = glGetDebugMessageLog(numMsgs, msgData.size(), &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);

		sources.resize(numFound);
		types.resize(numFound);
		severities.resize(numFound);
		ids.resize(numFound);
		lengths.resize(numFound);

		std::vector<std::string> messages;
		messages.reserve(numFound);

		std::vector<GLchar>::iterator currPos = msgData.begin();
		for (size_t msg = 0; msg < lengths.size(); ++msg)
		{
			messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
			currPos = currPos + lengths[msg];
		}

		std::vector<MessageLog_GL> log;
		log.resize(numFound);
		for (unsigned int cnt = 0; cnt < numFound; cnt++)
		{
			log[cnt].source = sources[cnt];
			log[cnt].type = types[cnt];
			log[cnt].severity = severities[cnt];
			log[cnt].id = ids[cnt];
			log[cnt].message = messages[cnt];
		}
		return log;

	}

	TextureFormat_GL OpenGL::get_textureformat(TextureFormat format)
	{
		TextureFormat_GL tf;
		tf.valid = true;
		tf.texture_format = format;

		const GLenum fixme = GL_UNSIGNED_BYTE;	// For texture formats that need a monkey to update

		// OpenGL ES 2 only supports a very limited set of formats
		// format: GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA
		// type: GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_5_5_5_1

		switch (format)
		{
	#ifdef __IOS__
			case tf_rgb8: tf.internal_format = GL_RGB8; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_rgb8ui: tf.internal_format = GL_RGB8UI; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_rgba8i: tf.internal_format = GL_RGBA8I; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_BYTE; break;
			case tf_rgba8ui: tf.internal_format = GL_RGBA8UI; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;

	#else

			case tf_stencil_index1: tf.internal_format = GL_STENCIL_INDEX1; tf.pixel_format = GL_STENCIL_INDEX; tf.pixel_datatype = fixme; break;
			case tf_stencil_index4: tf.internal_format = GL_STENCIL_INDEX4; tf.pixel_format = GL_STENCIL_INDEX; tf.pixel_datatype = fixme; break;
			case tf_stencil_index8: tf.internal_format = GL_STENCIL_INDEX8; tf.pixel_format = GL_STENCIL_INDEX; tf.pixel_datatype = fixme; break;
			case tf_stencil_index16: tf.internal_format = GL_STENCIL_INDEX16; tf.pixel_format = GL_STENCIL_INDEX; tf.pixel_datatype = fixme; break;
			case tf_r8: tf.internal_format = GL_R8; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_r8_snorm: tf.internal_format = GL_R8_SNORM; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_BYTE; break;
			case tf_r16: tf.internal_format = GL_R16; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_UNSIGNED_SHORT; break;
			case tf_r16_snorm: tf.internal_format = GL_R16_SNORM; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_SHORT; break;
			case tf_rg8: tf.internal_format = GL_RG8; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_rg8_snorm: tf.internal_format = GL_RG8_SNORM; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_BYTE; break;
			case tf_rg16: tf.internal_format = GL_RG16; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_UNSIGNED_SHORT; break;
			case tf_rg16_snorm: tf.internal_format = GL_RG16_SNORM; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_SHORT; break;
			case tf_r3_g3_b2: tf.internal_format = GL_R3_G3_B2; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_UNSIGNED_BYTE_3_3_2; break;
			case tf_rgb4: tf.internal_format = GL_RGB4; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_SHORT_4_4_4_4; break;
			case tf_rgb5: tf.internal_format = GL_RGB5; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_SHORT_5_5_5_1; break;
			case tf_rgb8: tf.internal_format = GL_RGB8; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_rgb8_snorm: tf.internal_format = GL_RGB8_SNORM; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_BYTE; break;
			case tf_rgb10: tf.internal_format = GL_RGB10; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_INT_10_10_10_2; break;
			case tf_rgb12: tf.internal_format = GL_RGB12; tf.pixel_format = GL_RGB; tf.pixel_datatype = fixme; break;
			case tf_rgb16: tf.internal_format = GL_RGB16; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_UNSIGNED_SHORT; break;
			case tf_rgb16_snorm: tf.internal_format = GL_RGB16_SNORM; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_SHORT; break;
			case tf_rgba2: tf.internal_format = GL_RGBA2; tf.pixel_format = GL_RGBA; tf.pixel_datatype = fixme; break;
			case tf_rgba4: tf.internal_format = GL_RGBA4; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_SHORT_4_4_4_4; break;
			case tf_rgb5_a1: tf.internal_format = GL_RGB5_A1; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_SHORT_5_5_5_1; break;
			case tf_rgba8: tf.internal_format = GL_RGBA8; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_rgba8_snorm: tf.internal_format = GL_RGBA8_SNORM; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_BYTE; break;
			case tf_bgra8: tf.internal_format = GL_RGBA8; tf.pixel_format = GL_BGRA; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_bgr8: tf.internal_format = GL_RGB8; tf.pixel_format = GL_BGR; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_rgb10_a2: tf.internal_format = GL_RGB10_A2; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_INT_10_10_10_2; break;
			case tf_rgba12: tf.internal_format = GL_RGBA12; tf.pixel_format = GL_RGBA; tf.pixel_datatype = fixme; break;
			case tf_rgba16: tf.internal_format = GL_RGBA16; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_SHORT; break;
			case tf_rgba16_snorm: tf.internal_format = GL_RGBA16_SNORM; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_SHORT; break;
			case tf_srgb8: tf.internal_format = GL_SRGB8; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_srgb8_alpha8: tf.internal_format = GL_SRGB8_ALPHA8; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_r16f: tf.internal_format = GL_R16F; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_HALF_FLOAT; break;
			case tf_rg16f: tf.internal_format = GL_RG16F; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_HALF_FLOAT; break;
			case tf_rgb16f: tf.internal_format = GL_RGB16F; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_HALF_FLOAT; break;
			case tf_rgba16f: tf.internal_format = GL_RGBA16F; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_HALF_FLOAT; break;
			case tf_r32f: tf.internal_format = GL_R32F; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_FLOAT; break;
			case tf_rg32f: tf.internal_format = GL_RG32F; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_FLOAT; break;
			case tf_rgb32f: tf.internal_format = GL_RGB32F; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_FLOAT; break;
			case tf_rgba32f: tf.internal_format = GL_RGBA32F; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_FLOAT; break;
			case tf_r11f_g11f_b10f: tf.internal_format = GL_R11F_G11F_B10F; tf.pixel_format = GL_RGB; tf.pixel_datatype = fixme; break;
			case tf_rgb9_e5: tf.internal_format = GL_RGB9_E5; tf.pixel_format = GL_RGB; tf.pixel_datatype = fixme; break;
			case tf_r8i: tf.internal_format = GL_R8I; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_BYTE; break;
			case tf_r8ui: tf.internal_format = GL_R8UI; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_r16i: tf.internal_format = GL_R16I; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_SHORT; break;
			case tf_r16ui: tf.internal_format = GL_R16UI; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_UNSIGNED_SHORT; break;
			case tf_r32i: tf.internal_format = GL_R32I; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_INT; break;
			case tf_r32ui: tf.internal_format = GL_R32UI; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_UNSIGNED_INT; break;
			case tf_rg8i: tf.internal_format = GL_RG8I; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_BYTE; break;
			case tf_rg8ui: tf.internal_format = GL_RG8UI; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_rg16i: tf.internal_format = GL_RG16I; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_BYTE; break;
			case tf_rg16ui: tf.internal_format = GL_RG16UI; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_UNSIGNED_SHORT; break;
			case tf_rg32i: tf.internal_format = GL_RG32I; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_INT; break;
			case tf_rg32ui: tf.internal_format = GL_RG32UI; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_UNSIGNED_INT; break;
			case tf_rgb8i: tf.internal_format = GL_RGB8I; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_BYTE; break;
			case tf_rgb8ui: tf.internal_format = GL_RGB8UI; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_rgb16i: tf.internal_format = GL_RGB16I; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_SHORT; break;
			case tf_rgb16ui: tf.internal_format = GL_RGB16UI; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_UNSIGNED_SHORT; break;
			case tf_rgb32i: tf.internal_format = GL_RGB32I; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_INT; break;
			case tf_rgb32ui: tf.internal_format = GL_RGB32UI; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_UNSIGNED_INT; break;
			case tf_rgba8i: tf.internal_format = GL_RGBA8I; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_BYTE; break;
			case tf_rgba8ui: tf.internal_format = GL_RGBA8UI; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_BYTE; break;
			case tf_rgba16i: tf.internal_format = GL_RGBA16I; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_SHORT; break;
			case tf_rgba16ui: tf.internal_format = GL_RGBA16UI; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_SHORT; break;
			case tf_rgba32i: tf.internal_format = GL_RGBA32I; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_INT; break;
			case tf_rgba32ui: tf.internal_format = GL_RGBA32UI; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_UNSIGNED_INT; break;
			case tf_depth_component16: tf.internal_format = GL_DEPTH_COMPONENT16; tf.pixel_format = GL_DEPTH_COMPONENT; tf.pixel_datatype = fixme; break;
			case tf_depth_component24: tf.internal_format = GL_DEPTH_COMPONENT24; tf.pixel_format = GL_DEPTH_COMPONENT; tf.pixel_datatype = fixme; break;
			case tf_depth_component32: tf.internal_format = GL_DEPTH_COMPONENT32; tf.pixel_format = GL_DEPTH_COMPONENT; tf.pixel_datatype = fixme; break;
			case tf_depth_component32f: tf.internal_format = GL_DEPTH_COMPONENT32F; tf.pixel_format = GL_DEPTH_COMPONENT; tf.pixel_datatype = fixme; break;
			case tf_depth24_stencil8: tf.internal_format = GL_DEPTH24_STENCIL8; tf.pixel_format = GL_DEPTH_STENCIL; tf.pixel_datatype = fixme; break;
			case tf_depth32f_stencil8: tf.internal_format = GL_DEPTH32F_STENCIL8; tf.pixel_format = GL_DEPTH_STENCIL; tf.pixel_datatype = fixme; break;
			case tf_compressed_red: tf.internal_format = GL_COMPRESSED_RED; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_COMPRESSED_RED; break;
			case tf_compressed_rg: tf.internal_format = GL_COMPRESSED_RG; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_COMPRESSED_RG; break;
			case tf_compressed_rgb: tf.internal_format = GL_COMPRESSED_RGB; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_COMPRESSED_RGB; break;
			case tf_compressed_rgba: tf.internal_format = GL_COMPRESSED_RGBA; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_COMPRESSED_RGBA; break;
			case tf_compressed_srgb: tf.internal_format = GL_COMPRESSED_SRGB; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_COMPRESSED_SRGB; break;
			case tf_compressed_srgb_alpha: tf.internal_format = GL_COMPRESSED_SRGB_ALPHA; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_COMPRESSED_SRGB_ALPHA; break;
			case tf_compressed_red_rgtc1: tf.internal_format = GL_COMPRESSED_RED_RGTC1; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_COMPRESSED_RED_RGTC1; break;
			case tf_compressed_signed_red_rgtc1: tf.internal_format = GL_COMPRESSED_SIGNED_RED_RGTC1; tf.pixel_format = GL_RED; tf.pixel_datatype = GL_COMPRESSED_SIGNED_RED_RGTC1; break;
			case tf_compressed_rg_rgtc2: tf.internal_format = GL_COMPRESSED_RG_RGTC2; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_COMPRESSED_RG_RGTC2; break;
			case tf_compressed_signed_rg_rgtc2: tf.internal_format = GL_COMPRESSED_SIGNED_RG_RGTC2; tf.pixel_format = GL_RG; tf.pixel_datatype = GL_COMPRESSED_SIGNED_RG_RGTC2; break;
			case tf_compressed_rgb_s3tc_dxt1: tf.internal_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
			case tf_compressed_rgba_s3tc_dxt1: tf.internal_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
			case tf_compressed_rgba_s3tc_dxt3: tf.internal_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
			case tf_compressed_rgba_s3tc_dxt5: tf.internal_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
			case tf_compressed_srgb_s3tc_dxt1: tf.internal_format = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT; tf.pixel_format = GL_RGB; tf.pixel_datatype = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT; break;
			case tf_compressed_srgb_alpha_s3tc_dxt1: tf.internal_format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT; break;
			case tf_compressed_srgb_alpha_s3tc_dxt3: tf.internal_format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT; break;
			case tf_compressed_srgb_alpha_s3tc_dxt5: tf.internal_format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT; tf.pixel_format = GL_RGBA; tf.pixel_datatype = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT; break;
	#endif
			default:
				tf.valid = false;
				tf.internal_format = 0;
				tf.pixel_format = 0;
				tf.pixel_datatype = 0;
				break;
		}
		return tf;
	}

	ProcAddress *OpenGL::get_proc_address(const std::string& function_name)
	{
	#ifdef WIN32
		return (void (*)())wglGetProcAddress(function_name.c_str());
	#else
		// Note Win32 can also do this, maybe it should
		if (cl_active_opengl_gc)
			return cl_active_opengl_gc->get_proc_address(function_name);

	#endif
		return nullptr;
	}

	void OpenGL::set_active(GraphicContext &gc)
	{
		gc.impl->set_active();
		OpenGLTarget::set_active_context(gc);
	}

	bool OpenGL::set_active()
	{
		if (cl_active_opengl_gc)	// If already active, we can exit now
			return true;

		std::unique_ptr<std::unique_lock<std::recursive_mutex>> mutex_section;
		GraphicContextProvider* shared_provider = SharedGCData::get_provider(mutex_section);
		if (shared_provider)
		{
			OpenGLGraphicContextProvider *gc_provider = dynamic_cast<OpenGLGraphicContextProvider*>(shared_provider);
			if (gc_provider)
			{
				OpenGL::set_active(gc_provider);
				return true;
			}
		}
		set_active(nullptr);
		return false;
	}

	void OpenGL::set_active(const OpenGLGraphicContextProvider * const gc_provider)
	{
		// Don't do anything if the supplied graphic context is already active.
	//#ifndef __APPLE__ // temp hack to see if iOS changes the current context behind our back
		if (gc_provider != cl_active_opengl_gc)
	//#endif
		{
			if (gc_provider)
			{
				// Make render context associated with graphic context current.
				gc_provider->make_current();

				// Map bound functions for this graphic context. Add to static cache if necessary.
				std::unique_lock<std::recursive_mutex> mutex_lock(cl_function_map_mutex);
				cl_function_map_type::iterator it;
				it = cl_function_map.find(gc_provider);
				if (it != cl_function_map.end())
				{
					OpenGL::functions = it->second;
				}
				else
				{
					cl_active_opengl_gc = gc_provider;
					GLFunctions *functions = cl_setup_binds();
					cl_function_map[gc_provider] = functions;
					OpenGL::functions = functions;
				}
			}

			// If no graphic context provided, set no render context current.
			else
			{
	#		if defined(WIN32)
				wglMakeCurrent(NULL, NULL);
	#		elif defined(__IOS__)
				[EAGLContext setCurrentContext:nil];
	#		elif defined(__APPLE__)
            
	#		else
				//Note: glX may not even be available. Also glXGetCurrentDisplay() may fail
				// Hopefully this will not matter!
				//glXMakeCurrent(glXGetCurrentDisplay(), None, NULL);
	#		endif

				// If no current context, don't map function bindings either.
				OpenGL::functions = nullptr;
			}

			// OK, make our context the active one
			cl_active_opengl_gc = gc_provider;
		}
	}

	void OpenGL::remove_active(const OpenGLGraphicContextProvider * const gc_provider)
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(cl_function_map_mutex);
		cl_function_map_type::iterator it;
		it = cl_function_map.find(gc_provider);
		if (it != cl_function_map.end())
		{
			GLFunctions *functions = it->second;
			cl_function_map.erase(it);
			delete functions;

			if (cl_active_opengl_gc == gc_provider)
			{
				set_active(nullptr);
			}
		}
	}

	static ProcAddress *cl_get_proc_address_extension(const std::string& function_name)
	{
		ProcAddress *ptr = OpenGL::get_proc_address(function_name);
		if (!ptr)
		{
			ptr = OpenGL::get_proc_address(function_name + "ARB");
		}
		if (!ptr)
		{
			ptr = OpenGL::get_proc_address(function_name + "EXT");
		}
		if (!ptr)
		{
			ptr = OpenGL::get_proc_address(function_name + "ATI");
		}
		return ptr;
	}

	GLFunctions *cl_setup_binds()
	{
		auto functions = new GLFunctions;
		memset(functions, 0, sizeof(GLFunctions));

		functions->cullFace = (GLFunctions::ptr_glCullFace) cl_get_proc_address_extension("glCullFace");
		functions->frontFace = (GLFunctions::ptr_glFrontFace) cl_get_proc_address_extension("glFrontFace");
		functions->hint = (GLFunctions::ptr_glHint) cl_get_proc_address_extension("glHint");
		functions->lineWidth = (GLFunctions::ptr_glLineWidth) cl_get_proc_address_extension("glLineWidth");
		functions->pointSize = (GLFunctions::ptr_glPointSize) cl_get_proc_address_extension("glPointSize");
		functions->polygonMode = (GLFunctions::ptr_glPolygonMode) cl_get_proc_address_extension("glPolygonMode");
		functions->scissor = (GLFunctions::ptr_glScissor) cl_get_proc_address_extension("glScissor");
		functions->texParameterf = (GLFunctions::ptr_glTexParameterf) cl_get_proc_address_extension("glTexParameterf");
		functions->texParameterfv = (GLFunctions::ptr_glTexParameterfv) cl_get_proc_address_extension("glTexParameterfv");
		functions->texParameteri = (GLFunctions::ptr_glTexParameteri) cl_get_proc_address_extension("glTexParameteri");
		functions->texParameteriv = (GLFunctions::ptr_glTexParameteriv) cl_get_proc_address_extension("glTexParameteriv");
		functions->texImage1D = (GLFunctions::ptr_glTexImage1D) cl_get_proc_address_extension("glTexImage1D");
		functions->texImage2D = (GLFunctions::ptr_glTexImage2D) cl_get_proc_address_extension("glTexImage2D");
		functions->drawBuffer = (GLFunctions::ptr_glDrawBuffer) cl_get_proc_address_extension("glDrawBuffer");
		functions->clear = (GLFunctions::ptr_glClear) cl_get_proc_address_extension("glClear");
		functions->clearColor = (GLFunctions::ptr_glClearColor) cl_get_proc_address_extension("glClearColor");
		functions->clearStencil = (GLFunctions::ptr_glClearStencil) cl_get_proc_address_extension("glClearStencil");
		functions->clearDepth = (GLFunctions::ptr_glClearDepth) cl_get_proc_address_extension("glClearDepth");
		functions->stencilMask = (GLFunctions::ptr_glStencilMask) cl_get_proc_address_extension("glStencilMask");
		functions->colorMask = (GLFunctions::ptr_glColorMask) cl_get_proc_address_extension("glColorMask");
		functions->depthMask = (GLFunctions::ptr_glDepthMask) cl_get_proc_address_extension("glDepthMask");
		functions->disable = (GLFunctions::ptr_glDisable) cl_get_proc_address_extension("glDisable");
		functions->enable = (GLFunctions::ptr_glEnable) cl_get_proc_address_extension("glEnable");
		functions->finish = (GLFunctions::ptr_glFinish) cl_get_proc_address_extension("glFinish");
		functions->flush = (GLFunctions::ptr_glFlush) cl_get_proc_address_extension("glFlush");
		functions->blendFunc = (GLFunctions::ptr_glBlendFunc) cl_get_proc_address_extension("glBlendFunc");
		functions->logicOp = (GLFunctions::ptr_glLogicOp) cl_get_proc_address_extension("glLogicOp");
		functions->stencilFunc = (GLFunctions::ptr_glStencilFunc) cl_get_proc_address_extension("glStencilFunc");
		functions->stencilOp = (GLFunctions::ptr_glStencilOp) cl_get_proc_address_extension("glStencilOp");
		functions->depthFunc = (GLFunctions::ptr_glDepthFunc) cl_get_proc_address_extension("glDepthFunc");
		functions->pixelStoref = (GLFunctions::ptr_glPixelStoref) cl_get_proc_address_extension("glPixelStoref");
		functions->pixelStorei = (GLFunctions::ptr_glPixelStorei) cl_get_proc_address_extension("glPixelStorei");
		functions->readBuffer = (GLFunctions::ptr_glReadBuffer) cl_get_proc_address_extension("glReadBuffer");
		functions->readPixels = (GLFunctions::ptr_glReadPixels) cl_get_proc_address_extension("glReadPixels");
		functions->getBooleanv = (GLFunctions::ptr_glGetBooleanv) cl_get_proc_address_extension("glGetBooleanv");
		functions->getDoublev = (GLFunctions::ptr_glGetDoublev) cl_get_proc_address_extension("glGetDoublev");
		functions->getError = (GLFunctions::ptr_glGetError) cl_get_proc_address_extension("glGetError");
		functions->getFloatv = (GLFunctions::ptr_glGetFloatv) cl_get_proc_address_extension("glGetFloatv");
		functions->getIntegerv = (GLFunctions::ptr_glGetIntegerv) cl_get_proc_address_extension("glGetIntegerv");
		functions->getString = (GLFunctions::ptr_glGetString) cl_get_proc_address_extension("glGetString");
		functions->getTexImage = (GLFunctions::ptr_glGetTexImage) cl_get_proc_address_extension("glGetTexImage");
		functions->getTexParameterfv = (GLFunctions::ptr_glGetTexParameterfv) cl_get_proc_address_extension("glGetTexParameterfv");
		functions->getTexParameteriv = (GLFunctions::ptr_glGetTexParameteriv) cl_get_proc_address_extension("glGetTexParameteriv");
		functions->getTexLevelParameterfv = (GLFunctions::ptr_glGetTexLevelParameterfv) cl_get_proc_address_extension("glGetTexLevelParameterfv");
		functions->getTexLevelParameteriv = (GLFunctions::ptr_glGetTexLevelParameteriv) cl_get_proc_address_extension("glGetTexLevelParameteriv");
		functions->isEnabled = (GLFunctions::ptr_glIsEnabled) cl_get_proc_address_extension("glIsEnabled");
		functions->depthRange = (GLFunctions::ptr_glDepthRange) cl_get_proc_address_extension("glDepthRange");
		functions->viewport = (GLFunctions::ptr_glViewport) cl_get_proc_address_extension("glViewport");
		functions->drawArrays = (GLFunctions::ptr_glDrawArrays) cl_get_proc_address_extension("glDrawArrays");
		functions->drawElements = (GLFunctions::ptr_glDrawElements) cl_get_proc_address_extension("glDrawElements");
		functions->getPointerv = (GLFunctions::ptr_glGetPointerv) cl_get_proc_address_extension("glGetPointerv");
		functions->polygonOffset = (GLFunctions::ptr_glPolygonOffset) cl_get_proc_address_extension("glPolygonOffset");
		functions->copyTexImage1D = (GLFunctions::ptr_glCopyTexImage1D) cl_get_proc_address_extension("glCopyTexImage1D");
		functions->copyTexImage2D = (GLFunctions::ptr_glCopyTexImage2D) cl_get_proc_address_extension("glCopyTexImage2D");
		functions->copyTexSubImage1D = (GLFunctions::ptr_glCopyTexSubImage1D) cl_get_proc_address_extension("glCopyTexSubImage1D");
		functions->copyTexSubImage2D = (GLFunctions::ptr_glCopyTexSubImage2D) cl_get_proc_address_extension("glCopyTexSubImage2D");
		functions->texSubImage1D = (GLFunctions::ptr_glTexSubImage1D) cl_get_proc_address_extension("glTexSubImage1D");
		functions->texSubImage2D = (GLFunctions::ptr_glTexSubImage2D) cl_get_proc_address_extension("glTexSubImage2D");
		functions->bindTexture = (GLFunctions::ptr_glBindTexture) cl_get_proc_address_extension("glBindTexture");
		functions->deleteTextures = (GLFunctions::ptr_glDeleteTextures) cl_get_proc_address_extension("glDeleteTextures");
		functions->genTextures = (GLFunctions::ptr_glGenTextures) cl_get_proc_address_extension("glGenTextures");
		functions->isTexture = (GLFunctions::ptr_glIsTexture) cl_get_proc_address_extension("glIsTexture");
		functions->blendColor = (GLFunctions::ptr_glBlendColor) cl_get_proc_address_extension("glBlendColor");
		functions->blendEquation = (GLFunctions::ptr_glBlendEquation) cl_get_proc_address_extension("glBlendEquation");
		functions->drawRangeElements = (GLFunctions::ptr_glDrawRangeElements) cl_get_proc_address_extension("glDrawRangeElements");
		functions->texImage3D = (GLFunctions::ptr_glTexImage3D) cl_get_proc_address_extension("glTexImage3D");
		functions->texSubImage3D = (GLFunctions::ptr_glTexSubImage3D) cl_get_proc_address_extension("glTexSubImage3D");
		functions->copyTexSubImage3D = (GLFunctions::ptr_glCopyTexSubImage3D) cl_get_proc_address_extension("glCopyTexSubImage3D");
		functions->activeTexture = (GLFunctions::ptr_glActiveTexture) cl_get_proc_address_extension("glActiveTexture");
		functions->sampleCoverage = (GLFunctions::ptr_glSampleCoverage) cl_get_proc_address_extension("glSampleCoverage");
		functions->compressedTexImage3D = (GLFunctions::ptr_glCompressedTexImage3D) cl_get_proc_address_extension("glCompressedTexImage3D");
		functions->compressedTexImage2D = (GLFunctions::ptr_glCompressedTexImage2D) cl_get_proc_address_extension("glCompressedTexImage2D");
		functions->compressedTexImage1D = (GLFunctions::ptr_glCompressedTexImage1D) cl_get_proc_address_extension("glCompressedTexImage1D");
		functions->compressedTexSubImage3D = (GLFunctions::ptr_glCompressedTexSubImage3D) cl_get_proc_address_extension("glCompressedTexSubImage3D");
		functions->compressedTexSubImage2D = (GLFunctions::ptr_glCompressedTexSubImage2D) cl_get_proc_address_extension("glCompressedTexSubImage2D");
		functions->compressedTexSubImage1D = (GLFunctions::ptr_glCompressedTexSubImage1D) cl_get_proc_address_extension("glCompressedTexSubImage1D");
		functions->getCompressedTexImage = (GLFunctions::ptr_glGetCompressedTexImage) cl_get_proc_address_extension("glGetCompressedTexImage");
		functions->blendFuncSeparate = (GLFunctions::ptr_glBlendFuncSeparate) cl_get_proc_address_extension("glBlendFuncSeparate");
		functions->multiDrawArrays = (GLFunctions::ptr_glMultiDrawArrays) cl_get_proc_address_extension("glMultiDrawArrays");
		functions->multiDrawElements = (GLFunctions::ptr_glMultiDrawElements) cl_get_proc_address_extension("glMultiDrawElements");
		functions->pointParameterf = (GLFunctions::ptr_glPointParameterf) cl_get_proc_address_extension("glPointParameterf");
		functions->pointParameterfv = (GLFunctions::ptr_glPointParameterfv) cl_get_proc_address_extension("glPointParameterfv");
		functions->pointParameteri = (GLFunctions::ptr_glPointParameteri) cl_get_proc_address_extension("glPointParameteri");
		functions->pointParameteriv = (GLFunctions::ptr_glPointParameteriv) cl_get_proc_address_extension("glPointParameteriv");
		functions->genQueries = (GLFunctions::ptr_glGenQueries) cl_get_proc_address_extension("glGenQueries");
		functions->deleteQueries = (GLFunctions::ptr_glDeleteQueries) cl_get_proc_address_extension("glDeleteQueries");
		functions->isQuery = (GLFunctions::ptr_glIsQuery) cl_get_proc_address_extension("glIsQuery");
		functions->beginQuery = (GLFunctions::ptr_glBeginQuery) cl_get_proc_address_extension("glBeginQuery");
		functions->endQuery = (GLFunctions::ptr_glEndQuery) cl_get_proc_address_extension("glEndQuery");
		functions->getQueryiv = (GLFunctions::ptr_glGetQueryiv) cl_get_proc_address_extension("glGetQueryiv");
		functions->getQueryObjectiv = (GLFunctions::ptr_glGetQueryObjectiv) cl_get_proc_address_extension("glGetQueryObjectiv");
		functions->getQueryObjectuiv = (GLFunctions::ptr_glGetQueryObjectuiv) cl_get_proc_address_extension("glGetQueryObjectuiv");
		functions->bindBuffer = (GLFunctions::ptr_glBindBuffer) cl_get_proc_address_extension("glBindBuffer");
		functions->deleteBuffers = (GLFunctions::ptr_glDeleteBuffers) cl_get_proc_address_extension("glDeleteBuffers");
		functions->genBuffers = (GLFunctions::ptr_glGenBuffers) cl_get_proc_address_extension("glGenBuffers");
		functions->isBuffer = (GLFunctions::ptr_glIsBuffer) cl_get_proc_address_extension("glIsBuffer");
		functions->bufferData = (GLFunctions::ptr_glBufferData) cl_get_proc_address_extension("glBufferData");
		functions->bufferSubData = (GLFunctions::ptr_glBufferSubData) cl_get_proc_address_extension("glBufferSubData");
		functions->getBufferSubData = (GLFunctions::ptr_glGetBufferSubData) cl_get_proc_address_extension("glGetBufferSubData");
		functions->mapBuffer = (GLFunctions::ptr_glMapBuffer) cl_get_proc_address_extension("glMapBuffer");
		functions->unmapBuffer = (GLFunctions::ptr_glUnmapBuffer) cl_get_proc_address_extension("glUnmapBuffer");
		functions->getBufferParameteriv = (GLFunctions::ptr_glGetBufferParameteriv) cl_get_proc_address_extension("glGetBufferParameteriv");
		functions->getBufferPointerv = (GLFunctions::ptr_glGetBufferPointerv) cl_get_proc_address_extension("glGetBufferPointerv");
		functions->blendEquationSeparate = (GLFunctions::ptr_glBlendEquationSeparate) cl_get_proc_address_extension("glBlendEquationSeparate");
		functions->drawBuffers = (GLFunctions::ptr_glDrawBuffers) cl_get_proc_address_extension("glDrawBuffers");
		functions->stencilOpSeparate = (GLFunctions::ptr_glStencilOpSeparate) cl_get_proc_address_extension("glStencilOpSeparate");
		functions->stencilFuncSeparate = (GLFunctions::ptr_glStencilFuncSeparate) cl_get_proc_address_extension("glStencilFuncSeparate");
		functions->stencilMaskSeparate = (GLFunctions::ptr_glStencilMaskSeparate) cl_get_proc_address_extension("glStencilMaskSeparate");
		functions->attachShader = (GLFunctions::ptr_glAttachShader) cl_get_proc_address_extension("glAttachShader");
		functions->bindAttribLocation = (GLFunctions::ptr_glBindAttribLocation) cl_get_proc_address_extension("glBindAttribLocation");
		functions->compileShader = (GLFunctions::ptr_glCompileShader) cl_get_proc_address_extension("glCompileShader");
		functions->createProgram = (GLFunctions::ptr_glCreateProgram) cl_get_proc_address_extension("glCreateProgram");
		functions->createShader = (GLFunctions::ptr_glCreateShader) cl_get_proc_address_extension("glCreateShader");
		functions->deleteProgram = (GLFunctions::ptr_glDeleteProgram) cl_get_proc_address_extension("glDeleteProgram");
		functions->deleteShader = (GLFunctions::ptr_glDeleteShader) cl_get_proc_address_extension("glDeleteShader");
		functions->detachShader = (GLFunctions::ptr_glDetachShader) cl_get_proc_address_extension("glDetachShader");
		functions->disableVertexAttribArray = (GLFunctions::ptr_glDisableVertexAttribArray) cl_get_proc_address_extension("glDisableVertexAttribArray");
		functions->enableVertexAttribArray = (GLFunctions::ptr_glEnableVertexAttribArray) cl_get_proc_address_extension("glEnableVertexAttribArray");
		functions->getActiveAttrib = (GLFunctions::ptr_glGetActiveAttrib) cl_get_proc_address_extension("glGetActiveAttrib");
		functions->getActiveUniform = (GLFunctions::ptr_glGetActiveUniform) cl_get_proc_address_extension("glGetActiveUniform");
		functions->getAttachedShaders = (GLFunctions::ptr_glGetAttachedShaders) cl_get_proc_address_extension("glGetAttachedShaders");
		functions->getAttribLocation = (GLFunctions::ptr_glGetAttribLocation) cl_get_proc_address_extension("glGetAttribLocation");
		functions->getProgramiv = (GLFunctions::ptr_glGetProgramiv) cl_get_proc_address_extension("glGetProgramiv");
		functions->getProgramInfoLog = (GLFunctions::ptr_glGetProgramInfoLog) cl_get_proc_address_extension("glGetProgramInfoLog");
		functions->getShaderiv = (GLFunctions::ptr_glGetShaderiv) cl_get_proc_address_extension("glGetShaderiv");
		functions->getShaderInfoLog = (GLFunctions::ptr_glGetShaderInfoLog) cl_get_proc_address_extension("glGetShaderInfoLog");
		functions->getShaderSource = (GLFunctions::ptr_glGetShaderSource) cl_get_proc_address_extension("glGetShaderSource");
		functions->getUniformLocation = (GLFunctions::ptr_glGetUniformLocation) cl_get_proc_address_extension("glGetUniformLocation");
		functions->getUniformfv = (GLFunctions::ptr_glGetUniformfv) cl_get_proc_address_extension("glGetUniformfv");
		functions->getUniformiv = (GLFunctions::ptr_glGetUniformiv) cl_get_proc_address_extension("glGetUniformiv");
		functions->getVertexAttribdv = (GLFunctions::ptr_glGetVertexAttribdv) cl_get_proc_address_extension("glGetVertexAttribdv");
		functions->getVertexAttribfv = (GLFunctions::ptr_glGetVertexAttribfv) cl_get_proc_address_extension("glGetVertexAttribfv");
		functions->getVertexAttribiv = (GLFunctions::ptr_glGetVertexAttribiv) cl_get_proc_address_extension("glGetVertexAttribiv");
		functions->getVertexAttribPointerv = (GLFunctions::ptr_glGetVertexAttribPointerv) cl_get_proc_address_extension("glGetVertexAttribPointerv");
		functions->isProgram = (GLFunctions::ptr_glIsProgram) cl_get_proc_address_extension("glIsProgram");
		functions->isShader = (GLFunctions::ptr_glIsShader) cl_get_proc_address_extension("glIsShader");
		functions->linkProgram = (GLFunctions::ptr_glLinkProgram) cl_get_proc_address_extension("glLinkProgram");
		functions->shaderSource = (GLFunctions::ptr_glShaderSource) cl_get_proc_address_extension("glShaderSource");
		functions->useProgram = (GLFunctions::ptr_glUseProgram) cl_get_proc_address_extension("glUseProgram");
		functions->uniform1f = (GLFunctions::ptr_glUniform1f) cl_get_proc_address_extension("glUniform1f");
		functions->uniform2f = (GLFunctions::ptr_glUniform2f) cl_get_proc_address_extension("glUniform2f");
		functions->uniform3f = (GLFunctions::ptr_glUniform3f) cl_get_proc_address_extension("glUniform3f");
		functions->uniform4f = (GLFunctions::ptr_glUniform4f) cl_get_proc_address_extension("glUniform4f");
		functions->uniform1i = (GLFunctions::ptr_glUniform1i) cl_get_proc_address_extension("glUniform1i");
		functions->uniform2i = (GLFunctions::ptr_glUniform2i) cl_get_proc_address_extension("glUniform2i");
		functions->uniform3i = (GLFunctions::ptr_glUniform3i) cl_get_proc_address_extension("glUniform3i");
		functions->uniform4i = (GLFunctions::ptr_glUniform4i) cl_get_proc_address_extension("glUniform4i");
		functions->uniform1fv = (GLFunctions::ptr_glUniform1fv) cl_get_proc_address_extension("glUniform1fv");
		functions->uniform2fv = (GLFunctions::ptr_glUniform2fv) cl_get_proc_address_extension("glUniform2fv");
		functions->uniform3fv = (GLFunctions::ptr_glUniform3fv) cl_get_proc_address_extension("glUniform3fv");
		functions->uniform4fv = (GLFunctions::ptr_glUniform4fv) cl_get_proc_address_extension("glUniform4fv");
		functions->uniform1iv = (GLFunctions::ptr_glUniform1iv) cl_get_proc_address_extension("glUniform1iv");
		functions->uniform2iv = (GLFunctions::ptr_glUniform2iv) cl_get_proc_address_extension("glUniform2iv");
		functions->uniform3iv = (GLFunctions::ptr_glUniform3iv) cl_get_proc_address_extension("glUniform3iv");
		functions->uniform4iv = (GLFunctions::ptr_glUniform4iv) cl_get_proc_address_extension("glUniform4iv");
		functions->uniformMatrix2fv = (GLFunctions::ptr_glUniformMatrix2fv) cl_get_proc_address_extension("glUniformMatrix2fv");
		functions->uniformMatrix3fv = (GLFunctions::ptr_glUniformMatrix3fv) cl_get_proc_address_extension("glUniformMatrix3fv");
		functions->uniformMatrix4fv = (GLFunctions::ptr_glUniformMatrix4fv) cl_get_proc_address_extension("glUniformMatrix4fv");
		functions->validateProgram = (GLFunctions::ptr_glValidateProgram) cl_get_proc_address_extension("glValidateProgram");
		functions->vertexAttrib1d = (GLFunctions::ptr_glVertexAttrib1d) cl_get_proc_address_extension("glVertexAttrib1d");
		functions->vertexAttrib1dv = (GLFunctions::ptr_glVertexAttrib1dv) cl_get_proc_address_extension("glVertexAttrib1dv");
		functions->vertexAttrib1f = (GLFunctions::ptr_glVertexAttrib1f) cl_get_proc_address_extension("glVertexAttrib1f");
		functions->vertexAttrib1fv = (GLFunctions::ptr_glVertexAttrib1fv) cl_get_proc_address_extension("glVertexAttrib1fv");
		functions->vertexAttrib1s = (GLFunctions::ptr_glVertexAttrib1s) cl_get_proc_address_extension("glVertexAttrib1s");
		functions->vertexAttrib1sv = (GLFunctions::ptr_glVertexAttrib1sv) cl_get_proc_address_extension("glVertexAttrib1sv");
		functions->vertexAttrib2d = (GLFunctions::ptr_glVertexAttrib2d) cl_get_proc_address_extension("glVertexAttrib2d");
		functions->vertexAttrib2dv = (GLFunctions::ptr_glVertexAttrib2dv) cl_get_proc_address_extension("glVertexAttrib2dv");
		functions->vertexAttrib2f = (GLFunctions::ptr_glVertexAttrib2f) cl_get_proc_address_extension("glVertexAttrib2f");
		functions->vertexAttrib2fv = (GLFunctions::ptr_glVertexAttrib2fv) cl_get_proc_address_extension("glVertexAttrib2fv");
		functions->vertexAttrib2s = (GLFunctions::ptr_glVertexAttrib2s) cl_get_proc_address_extension("glVertexAttrib2s");
		functions->vertexAttrib2sv = (GLFunctions::ptr_glVertexAttrib2sv) cl_get_proc_address_extension("glVertexAttrib2sv");
		functions->vertexAttrib3d = (GLFunctions::ptr_glVertexAttrib3d) cl_get_proc_address_extension("glVertexAttrib3d");
		functions->vertexAttrib3dv = (GLFunctions::ptr_glVertexAttrib3dv) cl_get_proc_address_extension("glVertexAttrib3dv");
		functions->vertexAttrib3f = (GLFunctions::ptr_glVertexAttrib3f) cl_get_proc_address_extension("glVertexAttrib3f");
		functions->vertexAttrib3fv = (GLFunctions::ptr_glVertexAttrib3fv) cl_get_proc_address_extension("glVertexAttrib3fv");
		functions->vertexAttrib3s = (GLFunctions::ptr_glVertexAttrib3s) cl_get_proc_address_extension("glVertexAttrib3s");
		functions->vertexAttrib3sv = (GLFunctions::ptr_glVertexAttrib3sv) cl_get_proc_address_extension("glVertexAttrib3sv");
		functions->vertexAttrib4Nbv = (GLFunctions::ptr_glVertexAttrib4Nbv) cl_get_proc_address_extension("glVertexAttrib4Nbv");
		functions->vertexAttrib4Niv = (GLFunctions::ptr_glVertexAttrib4Niv) cl_get_proc_address_extension("glVertexAttrib4Niv");
		functions->vertexAttrib4Nsv = (GLFunctions::ptr_glVertexAttrib4Nsv) cl_get_proc_address_extension("glVertexAttrib4Nsv");
		functions->vertexAttrib4Nub = (GLFunctions::ptr_glVertexAttrib4Nub) cl_get_proc_address_extension("glVertexAttrib4Nub");
		functions->vertexAttrib4Nubv = (GLFunctions::ptr_glVertexAttrib4Nubv) cl_get_proc_address_extension("glVertexAttrib4Nubv");
		functions->vertexAttrib4Nuiv = (GLFunctions::ptr_glVertexAttrib4Nuiv) cl_get_proc_address_extension("glVertexAttrib4Nuiv");
		functions->vertexAttrib4Nusv = (GLFunctions::ptr_glVertexAttrib4Nusv) cl_get_proc_address_extension("glVertexAttrib4Nusv");
		functions->vertexAttrib4bv = (GLFunctions::ptr_glVertexAttrib4bv) cl_get_proc_address_extension("glVertexAttrib4bv");
		functions->vertexAttrib4d = (GLFunctions::ptr_glVertexAttrib4d) cl_get_proc_address_extension("glVertexAttrib4d");
		functions->vertexAttrib4dv = (GLFunctions::ptr_glVertexAttrib4dv) cl_get_proc_address_extension("glVertexAttrib4dv");
		functions->vertexAttrib4f = (GLFunctions::ptr_glVertexAttrib4f) cl_get_proc_address_extension("glVertexAttrib4f");
		functions->vertexAttrib4fv = (GLFunctions::ptr_glVertexAttrib4fv) cl_get_proc_address_extension("glVertexAttrib4fv");
		functions->vertexAttrib4iv = (GLFunctions::ptr_glVertexAttrib4iv) cl_get_proc_address_extension("glVertexAttrib4iv");
		functions->vertexAttrib4s = (GLFunctions::ptr_glVertexAttrib4s) cl_get_proc_address_extension("glVertexAttrib4s");
		functions->vertexAttrib4sv = (GLFunctions::ptr_glVertexAttrib4sv) cl_get_proc_address_extension("glVertexAttrib4sv");
		functions->vertexAttrib4ubv = (GLFunctions::ptr_glVertexAttrib4ubv) cl_get_proc_address_extension("glVertexAttrib4ubv");
		functions->vertexAttrib4uiv = (GLFunctions::ptr_glVertexAttrib4uiv) cl_get_proc_address_extension("glVertexAttrib4uiv");
		functions->vertexAttrib4usv = (GLFunctions::ptr_glVertexAttrib4usv) cl_get_proc_address_extension("glVertexAttrib4usv");
		functions->vertexAttribPointer = (GLFunctions::ptr_glVertexAttribPointer) cl_get_proc_address_extension("glVertexAttribPointer");
		functions->uniformMatrix2x3fv = (GLFunctions::ptr_glUniformMatrix2x3fv) cl_get_proc_address_extension("glUniformMatrix2x3fv");
		functions->uniformMatrix3x2fv = (GLFunctions::ptr_glUniformMatrix3x2fv) cl_get_proc_address_extension("glUniformMatrix3x2fv");
		functions->uniformMatrix2x4fv = (GLFunctions::ptr_glUniformMatrix2x4fv) cl_get_proc_address_extension("glUniformMatrix2x4fv");
		functions->uniformMatrix4x2fv = (GLFunctions::ptr_glUniformMatrix4x2fv) cl_get_proc_address_extension("glUniformMatrix4x2fv");
		functions->uniformMatrix3x4fv = (GLFunctions::ptr_glUniformMatrix3x4fv) cl_get_proc_address_extension("glUniformMatrix3x4fv");
		functions->uniformMatrix4x3fv = (GLFunctions::ptr_glUniformMatrix4x3fv) cl_get_proc_address_extension("glUniformMatrix4x3fv");
		functions->colorMaski = (GLFunctions::ptr_glColorMaski) cl_get_proc_address_extension("glColorMaski");
		functions->getBooleani_v = (GLFunctions::ptr_glGetBooleani_v) cl_get_proc_address_extension("glGetBooleani_v");
		functions->getIntegeri_v = (GLFunctions::ptr_glGetIntegeri_v) cl_get_proc_address_extension("glGetIntegeri_v");
		functions->enablei = (GLFunctions::ptr_glEnablei) cl_get_proc_address_extension("glEnablei");
		functions->disablei = (GLFunctions::ptr_glDisablei) cl_get_proc_address_extension("glDisablei");
		functions->isEnabledi = (GLFunctions::ptr_glIsEnabledi) cl_get_proc_address_extension("glIsEnabledi");
		functions->beginTransformFeedback = (GLFunctions::ptr_glBeginTransformFeedback) cl_get_proc_address_extension("glBeginTransformFeedback");
		functions->endTransformFeedback = (GLFunctions::ptr_glEndTransformFeedback) cl_get_proc_address_extension("glEndTransformFeedback");
		functions->bindBufferRange = (GLFunctions::ptr_glBindBufferRange) cl_get_proc_address_extension("glBindBufferRange");
		functions->bindBufferBase = (GLFunctions::ptr_glBindBufferBase) cl_get_proc_address_extension("glBindBufferBase");
		functions->transformFeedbackVaryings = (GLFunctions::ptr_glTransformFeedbackVaryings) cl_get_proc_address_extension("glTransformFeedbackVaryings");
		functions->getTransformFeedbackVarying = (GLFunctions::ptr_glGetTransformFeedbackVarying) cl_get_proc_address_extension("glGetTransformFeedbackVarying");
		functions->clampColor = (GLFunctions::ptr_glClampColor) cl_get_proc_address_extension("glClampColor");
		functions->beginConditionalRender = (GLFunctions::ptr_glBeginConditionalRender) cl_get_proc_address_extension("glBeginConditionalRender");
		functions->endConditionalRender = (GLFunctions::ptr_glEndConditionalRender) cl_get_proc_address_extension("glEndConditionalRender");
		functions->vertexAttribIPointer = (GLFunctions::ptr_glVertexAttribIPointer) cl_get_proc_address_extension("glVertexAttribIPointer");
		functions->getVertexAttribIiv = (GLFunctions::ptr_glGetVertexAttribIiv) cl_get_proc_address_extension("glGetVertexAttribIiv");
		functions->getVertexAttribIuiv = (GLFunctions::ptr_glGetVertexAttribIuiv) cl_get_proc_address_extension("glGetVertexAttribIuiv");
		functions->vertexAttribI1i = (GLFunctions::ptr_glVertexAttribI1i) cl_get_proc_address_extension("glVertexAttribI1i");
		functions->vertexAttribI2i = (GLFunctions::ptr_glVertexAttribI2i) cl_get_proc_address_extension("glVertexAttribI2i");
		functions->vertexAttribI3i = (GLFunctions::ptr_glVertexAttribI3i) cl_get_proc_address_extension("glVertexAttribI3i");
		functions->vertexAttribI4i = (GLFunctions::ptr_glVertexAttribI4i) cl_get_proc_address_extension("glVertexAttribI4i");
		functions->vertexAttribI1ui = (GLFunctions::ptr_glVertexAttribI1ui) cl_get_proc_address_extension("glVertexAttribI1ui");
		functions->vertexAttribI2ui = (GLFunctions::ptr_glVertexAttribI2ui) cl_get_proc_address_extension("glVertexAttribI2ui");
		functions->vertexAttribI3ui = (GLFunctions::ptr_glVertexAttribI3ui) cl_get_proc_address_extension("glVertexAttribI3ui");
		functions->vertexAttribI4ui = (GLFunctions::ptr_glVertexAttribI4ui) cl_get_proc_address_extension("glVertexAttribI4ui");
		functions->vertexAttribI1iv = (GLFunctions::ptr_glVertexAttribI1iv) cl_get_proc_address_extension("glVertexAttribI1iv");
		functions->vertexAttribI2iv = (GLFunctions::ptr_glVertexAttribI2iv) cl_get_proc_address_extension("glVertexAttribI2iv");
		functions->vertexAttribI3iv = (GLFunctions::ptr_glVertexAttribI3iv) cl_get_proc_address_extension("glVertexAttribI3iv");
		functions->vertexAttribI4iv = (GLFunctions::ptr_glVertexAttribI4iv) cl_get_proc_address_extension("glVertexAttribI4iv");
		functions->vertexAttribI1uiv = (GLFunctions::ptr_glVertexAttribI1uiv) cl_get_proc_address_extension("glVertexAttribI1uiv");
		functions->vertexAttribI2uiv = (GLFunctions::ptr_glVertexAttribI2uiv) cl_get_proc_address_extension("glVertexAttribI2uiv");
		functions->vertexAttribI3uiv = (GLFunctions::ptr_glVertexAttribI3uiv) cl_get_proc_address_extension("glVertexAttribI3uiv");
		functions->vertexAttribI4uiv = (GLFunctions::ptr_glVertexAttribI4uiv) cl_get_proc_address_extension("glVertexAttribI4uiv");
		functions->vertexAttribI4bv = (GLFunctions::ptr_glVertexAttribI4bv) cl_get_proc_address_extension("glVertexAttribI4bv");
		functions->vertexAttribI4sv = (GLFunctions::ptr_glVertexAttribI4sv) cl_get_proc_address_extension("glVertexAttribI4sv");
		functions->vertexAttribI4ubv = (GLFunctions::ptr_glVertexAttribI4ubv) cl_get_proc_address_extension("glVertexAttribI4ubv");
		functions->vertexAttribI4usv = (GLFunctions::ptr_glVertexAttribI4usv) cl_get_proc_address_extension("glVertexAttribI4usv");
		functions->getUniformuiv = (GLFunctions::ptr_glGetUniformuiv) cl_get_proc_address_extension("glGetUniformuiv");
		functions->bindFragDataLocation = (GLFunctions::ptr_glBindFragDataLocation) cl_get_proc_address_extension("glBindFragDataLocation");
		functions->getFragDataLocation = (GLFunctions::ptr_glGetFragDataLocation) cl_get_proc_address_extension("glGetFragDataLocation");
		functions->uniform1ui = (GLFunctions::ptr_glUniform1ui) cl_get_proc_address_extension("glUniform1ui");
		functions->uniform2ui = (GLFunctions::ptr_glUniform2ui) cl_get_proc_address_extension("glUniform2ui");
		functions->uniform3ui = (GLFunctions::ptr_glUniform3ui) cl_get_proc_address_extension("glUniform3ui");
		functions->uniform4ui = (GLFunctions::ptr_glUniform4ui) cl_get_proc_address_extension("glUniform4ui");
		functions->uniform1uiv = (GLFunctions::ptr_glUniform1uiv) cl_get_proc_address_extension("glUniform1uiv");
		functions->uniform2uiv = (GLFunctions::ptr_glUniform2uiv) cl_get_proc_address_extension("glUniform2uiv");
		functions->uniform3uiv = (GLFunctions::ptr_glUniform3uiv) cl_get_proc_address_extension("glUniform3uiv");
		functions->uniform4uiv = (GLFunctions::ptr_glUniform4uiv) cl_get_proc_address_extension("glUniform4uiv");
		functions->texParameterIiv = (GLFunctions::ptr_glTexParameterIiv) cl_get_proc_address_extension("glTexParameterIiv");
		functions->texParameterIuiv = (GLFunctions::ptr_glTexParameterIuiv) cl_get_proc_address_extension("glTexParameterIuiv");
		functions->getTexParameterIiv = (GLFunctions::ptr_glGetTexParameterIiv) cl_get_proc_address_extension("glGetTexParameterIiv");
		functions->getTexParameterIuiv = (GLFunctions::ptr_glGetTexParameterIuiv) cl_get_proc_address_extension("glGetTexParameterIuiv");
		functions->clearBufferiv = (GLFunctions::ptr_glClearBufferiv) cl_get_proc_address_extension("glClearBufferiv");
		functions->clearBufferuiv = (GLFunctions::ptr_glClearBufferuiv) cl_get_proc_address_extension("glClearBufferuiv");
		functions->clearBufferfv = (GLFunctions::ptr_glClearBufferfv) cl_get_proc_address_extension("glClearBufferfv");
		functions->clearBufferfi = (GLFunctions::ptr_glClearBufferfi) cl_get_proc_address_extension("glClearBufferfi");
		functions->getStringi = (GLFunctions::ptr_glGetStringi) cl_get_proc_address_extension("glGetStringi");
		functions->drawArraysInstanced = (GLFunctions::ptr_glDrawArraysInstanced) cl_get_proc_address_extension("glDrawArraysInstanced");
		functions->drawElementsInstanced = (GLFunctions::ptr_glDrawElementsInstanced) cl_get_proc_address_extension("glDrawElementsInstanced");
		functions->texBuffer = (GLFunctions::ptr_glTexBuffer) cl_get_proc_address_extension("glTexBuffer");
		functions->primitiveRestartIndex = (GLFunctions::ptr_glPrimitiveRestartIndex) cl_get_proc_address_extension("glPrimitiveRestartIndex");
		functions->getInteger64i_v = (GLFunctions::ptr_glGetInteger64i_v) cl_get_proc_address_extension("glGetInteger64i_v");
		functions->getBufferParameteri64v = (GLFunctions::ptr_glGetBufferParameteri64v) cl_get_proc_address_extension("glGetBufferParameteri64v");
		functions->framebufferTexture = (GLFunctions::ptr_glFramebufferTexture) cl_get_proc_address_extension("glFramebufferTexture");
		functions->vertexAttribDivisor = (GLFunctions::ptr_glVertexAttribDivisor) cl_get_proc_address_extension("glVertexAttribDivisor");
		functions->minSampleShading = (GLFunctions::ptr_glMinSampleShading) cl_get_proc_address_extension("glMinSampleShading");
		functions->blendEquationi = (GLFunctions::ptr_glBlendEquationi) cl_get_proc_address_extension("glBlendEquationi");
		functions->blendEquationSeparatei = (GLFunctions::ptr_glBlendEquationSeparatei) cl_get_proc_address_extension("glBlendEquationSeparatei");
		functions->blendFunci = (GLFunctions::ptr_glBlendFunci) cl_get_proc_address_extension("glBlendFunci");
		functions->blendFuncSeparatei = (GLFunctions::ptr_glBlendFuncSeparatei) cl_get_proc_address_extension("glBlendFuncSeparatei");
		functions->isRenderbuffer = (GLFunctions::ptr_glIsRenderbuffer) cl_get_proc_address_extension("glIsRenderbuffer");
		functions->bindRenderbuffer = (GLFunctions::ptr_glBindRenderbuffer) cl_get_proc_address_extension("glBindRenderbuffer");
		functions->deleteRenderbuffers = (GLFunctions::ptr_glDeleteRenderbuffers) cl_get_proc_address_extension("glDeleteRenderbuffers");
		functions->genRenderbuffers = (GLFunctions::ptr_glGenRenderbuffers) cl_get_proc_address_extension("glGenRenderbuffers");
		functions->renderbufferStorage = (GLFunctions::ptr_glRenderbufferStorage) cl_get_proc_address_extension("glRenderbufferStorage");
		functions->getRenderbufferParameteriv = (GLFunctions::ptr_glGetRenderbufferParameteriv) cl_get_proc_address_extension("glGetRenderbufferParameteriv");
		functions->isFramebuffer = (GLFunctions::ptr_glIsFramebuffer) cl_get_proc_address_extension("glIsFramebuffer");
		functions->bindFramebuffer = (GLFunctions::ptr_glBindFramebuffer) cl_get_proc_address_extension("glBindFramebuffer");
		functions->deleteFramebuffers = (GLFunctions::ptr_glDeleteFramebuffers) cl_get_proc_address_extension("glDeleteFramebuffers");
		functions->genFramebuffers = (GLFunctions::ptr_glGenFramebuffers) cl_get_proc_address_extension("glGenFramebuffers");
		functions->checkFramebufferStatus = (GLFunctions::ptr_glCheckFramebufferStatus) cl_get_proc_address_extension("glCheckFramebufferStatus");
		functions->framebufferTexture1D = (GLFunctions::ptr_glFramebufferTexture1D) cl_get_proc_address_extension("glFramebufferTexture1D");
		functions->framebufferTexture2D = (GLFunctions::ptr_glFramebufferTexture2D) cl_get_proc_address_extension("glFramebufferTexture2D");
		functions->framebufferTexture3D = (GLFunctions::ptr_glFramebufferTexture3D) cl_get_proc_address_extension("glFramebufferTexture3D");
		functions->framebufferRenderbuffer = (GLFunctions::ptr_glFramebufferRenderbuffer) cl_get_proc_address_extension("glFramebufferRenderbuffer");
		functions->getFramebufferAttachmentParameteriv = (GLFunctions::ptr_glGetFramebufferAttachmentParameteriv) cl_get_proc_address_extension("glGetFramebufferAttachmentParameteriv");
		functions->generateMipmap = (GLFunctions::ptr_glGenerateMipmap) cl_get_proc_address_extension("glGenerateMipmap");
		functions->blitFramebuffer = (GLFunctions::ptr_glBlitFramebuffer) cl_get_proc_address_extension("glBlitFramebuffer");
		functions->renderbufferStorageMultisample = (GLFunctions::ptr_glRenderbufferStorageMultisample) cl_get_proc_address_extension("glRenderbufferStorageMultisample");
		functions->framebufferTextureLayer = (GLFunctions::ptr_glFramebufferTextureLayer) cl_get_proc_address_extension("glFramebufferTextureLayer");
		functions->mapBufferRange = (GLFunctions::ptr_glMapBufferRange) cl_get_proc_address_extension("glMapBufferRange");
		functions->flushMappedBufferRange = (GLFunctions::ptr_glFlushMappedBufferRange) cl_get_proc_address_extension("glFlushMappedBufferRange");
		functions->bindVertexArray = (GLFunctions::ptr_glBindVertexArray) cl_get_proc_address_extension("glBindVertexArray");
		functions->deleteVertexArrays = (GLFunctions::ptr_glDeleteVertexArrays) cl_get_proc_address_extension("glDeleteVertexArrays");
		functions->genVertexArrays = (GLFunctions::ptr_glGenVertexArrays) cl_get_proc_address_extension("glGenVertexArrays");
		functions->isVertexArray = (GLFunctions::ptr_glIsVertexArray) cl_get_proc_address_extension("glIsVertexArray");
		functions->getUniformIndices = (GLFunctions::ptr_glGetUniformIndices) cl_get_proc_address_extension("glGetUniformIndices");
		functions->getActiveUniformsiv = (GLFunctions::ptr_glGetActiveUniformsiv) cl_get_proc_address_extension("glGetActiveUniformsiv");
		functions->getActiveUniformName = (GLFunctions::ptr_glGetActiveUniformName) cl_get_proc_address_extension("glGetActiveUniformName");
		functions->getUniformBlockIndex = (GLFunctions::ptr_glGetUniformBlockIndex) cl_get_proc_address_extension("glGetUniformBlockIndex");
		functions->getActiveUniformBlockiv = (GLFunctions::ptr_glGetActiveUniformBlockiv) cl_get_proc_address_extension("glGetActiveUniformBlockiv");
		functions->getActiveUniformBlockName = (GLFunctions::ptr_glGetActiveUniformBlockName) cl_get_proc_address_extension("glGetActiveUniformBlockName");
		functions->uniformBlockBinding = (GLFunctions::ptr_glUniformBlockBinding) cl_get_proc_address_extension("glUniformBlockBinding");
		functions->copyBufferSubData = (GLFunctions::ptr_glCopyBufferSubData) cl_get_proc_address_extension("glCopyBufferSubData");
		functions->drawElementsBaseVertex = (GLFunctions::ptr_glDrawElementsBaseVertex) cl_get_proc_address_extension("glDrawElementsBaseVertex");
		functions->drawRangeElementsBaseVertex = (GLFunctions::ptr_glDrawRangeElementsBaseVertex) cl_get_proc_address_extension("glDrawRangeElementsBaseVertex");
		functions->drawElementsInstancedBaseVertex = (GLFunctions::ptr_glDrawElementsInstancedBaseVertex) cl_get_proc_address_extension("glDrawElementsInstancedBaseVertex");
		functions->multiDrawElementsBaseVertex = (GLFunctions::ptr_glMultiDrawElementsBaseVertex) cl_get_proc_address_extension("glMultiDrawElementsBaseVertex");
		functions->provokingVertex = (GLFunctions::ptr_glProvokingVertex) cl_get_proc_address_extension("glProvokingVertex");
		functions->fenceSync = (GLFunctions::ptr_glFenceSync) cl_get_proc_address_extension("glFenceSync");
		functions->isSync = (GLFunctions::ptr_glIsSync) cl_get_proc_address_extension("glIsSync");
		functions->deleteSync = (GLFunctions::ptr_glDeleteSync) cl_get_proc_address_extension("glDeleteSync");
		functions->clientWaitSync = (GLFunctions::ptr_glClientWaitSync) cl_get_proc_address_extension("glClientWaitSync");
		functions->waitSync = (GLFunctions::ptr_glWaitSync) cl_get_proc_address_extension("glWaitSync");
		functions->getInteger64v = (GLFunctions::ptr_glGetInteger64v) cl_get_proc_address_extension("glGetInteger64v");
		functions->getSynciv = (GLFunctions::ptr_glGetSynciv) cl_get_proc_address_extension("glGetSynciv");
		functions->texImage2DMultisample = (GLFunctions::ptr_glTexImage2DMultisample) cl_get_proc_address_extension("glTexImage2DMultisample");
		functions->texImage3DMultisample = (GLFunctions::ptr_glTexImage3DMultisample) cl_get_proc_address_extension("glTexImage3DMultisample");
		functions->getMultisamplefv = (GLFunctions::ptr_glGetMultisamplefv) cl_get_proc_address_extension("glGetMultisamplefv");
		functions->sampleMaski = (GLFunctions::ptr_glSampleMaski) cl_get_proc_address_extension("glSampleMaski");
		functions->blendEquationiARB = (GLFunctions::ptr_glBlendEquationiARB) cl_get_proc_address_extension("glBlendEquationiARB");
		functions->blendEquationSeparateiARB = (GLFunctions::ptr_glBlendEquationSeparateiARB) cl_get_proc_address_extension("glBlendEquationSeparateiARB");
		functions->blendFunciARB = (GLFunctions::ptr_glBlendFunciARB) cl_get_proc_address_extension("glBlendFunciARB");
		functions->blendFuncSeparateiARB = (GLFunctions::ptr_glBlendFuncSeparateiARB) cl_get_proc_address_extension("glBlendFuncSeparateiARB");
		functions->minSampleShadingARB = (GLFunctions::ptr_glMinSampleShadingARB) cl_get_proc_address_extension("glMinSampleShadingARB");
		functions->namedStringARB = (GLFunctions::ptr_glNamedStringARB) cl_get_proc_address_extension("glNamedStringARB");
		functions->deleteNamedStringARB = (GLFunctions::ptr_glDeleteNamedStringARB) cl_get_proc_address_extension("glDeleteNamedStringARB");
		functions->compileShaderIncludeARB = (GLFunctions::ptr_glCompileShaderIncludeARB) cl_get_proc_address_extension("glCompileShaderIncludeARB");
		functions->isNamedStringARB = (GLFunctions::ptr_glIsNamedStringARB) cl_get_proc_address_extension("glIsNamedStringARB");
		functions->getNamedStringARB = (GLFunctions::ptr_glGetNamedStringARB) cl_get_proc_address_extension("glGetNamedStringARB");
		functions->getNamedStringivARB = (GLFunctions::ptr_glGetNamedStringivARB) cl_get_proc_address_extension("glGetNamedStringivARB");
		functions->bindFragDataLocationIndexed = (GLFunctions::ptr_glBindFragDataLocationIndexed) cl_get_proc_address_extension("glBindFragDataLocationIndexed");
		functions->getFragDataIndex = (GLFunctions::ptr_glGetFragDataIndex) cl_get_proc_address_extension("glGetFragDataIndex");
		functions->genSamplers = (GLFunctions::ptr_glGenSamplers) cl_get_proc_address_extension("glGenSamplers");
		functions->deleteSamplers = (GLFunctions::ptr_glDeleteSamplers) cl_get_proc_address_extension("glDeleteSamplers");
		functions->isSampler = (GLFunctions::ptr_glIsSampler) cl_get_proc_address_extension("glIsSampler");
		functions->bindSampler = (GLFunctions::ptr_glBindSampler) cl_get_proc_address_extension("glBindSampler");
		functions->samplerParameteri = (GLFunctions::ptr_glSamplerParameteri) cl_get_proc_address_extension("glSamplerParameteri");
		functions->samplerParameteriv = (GLFunctions::ptr_glSamplerParameteriv) cl_get_proc_address_extension("glSamplerParameteriv");
		functions->samplerParameterf = (GLFunctions::ptr_glSamplerParameterf) cl_get_proc_address_extension("glSamplerParameterf");
		functions->samplerParameterfv = (GLFunctions::ptr_glSamplerParameterfv) cl_get_proc_address_extension("glSamplerParameterfv");
		functions->samplerParameterIiv = (GLFunctions::ptr_glSamplerParameterIiv) cl_get_proc_address_extension("glSamplerParameterIiv");
		functions->samplerParameterIuiv = (GLFunctions::ptr_glSamplerParameterIuiv) cl_get_proc_address_extension("glSamplerParameterIuiv");
		functions->getSamplerParameteriv = (GLFunctions::ptr_glGetSamplerParameteriv) cl_get_proc_address_extension("glGetSamplerParameteriv");
		functions->getSamplerParameterIiv = (GLFunctions::ptr_glGetSamplerParameterIiv) cl_get_proc_address_extension("glGetSamplerParameterIiv");
		functions->getSamplerParameterfv = (GLFunctions::ptr_glGetSamplerParameterfv) cl_get_proc_address_extension("glGetSamplerParameterfv");
		functions->getSamplerParameterIuiv = (GLFunctions::ptr_glGetSamplerParameterIuiv) cl_get_proc_address_extension("glGetSamplerParameterIuiv");
		functions->queryCounter = (GLFunctions::ptr_glQueryCounter) cl_get_proc_address_extension("glQueryCounter");
		functions->getQueryObjecti64v = (GLFunctions::ptr_glGetQueryObjecti64v) cl_get_proc_address_extension("glGetQueryObjecti64v");
		functions->getQueryObjectui64v = (GLFunctions::ptr_glGetQueryObjectui64v) cl_get_proc_address_extension("glGetQueryObjectui64v");
		functions->vertexP2ui = (GLFunctions::ptr_glVertexP2ui) cl_get_proc_address_extension("glVertexP2ui");
		functions->vertexP2uiv = (GLFunctions::ptr_glVertexP2uiv) cl_get_proc_address_extension("glVertexP2uiv");
		functions->vertexP3ui = (GLFunctions::ptr_glVertexP3ui) cl_get_proc_address_extension("glVertexP3ui");
		functions->vertexP3uiv = (GLFunctions::ptr_glVertexP3uiv) cl_get_proc_address_extension("glVertexP3uiv");
		functions->vertexP4ui = (GLFunctions::ptr_glVertexP4ui) cl_get_proc_address_extension("glVertexP4ui");
		functions->vertexP4uiv = (GLFunctions::ptr_glVertexP4uiv) cl_get_proc_address_extension("glVertexP4uiv");
		functions->texCoordP1ui = (GLFunctions::ptr_glTexCoordP1ui) cl_get_proc_address_extension("glTexCoordP1ui");
		functions->texCoordP1uiv = (GLFunctions::ptr_glTexCoordP1uiv) cl_get_proc_address_extension("glTexCoordP1uiv");
		functions->texCoordP2ui = (GLFunctions::ptr_glTexCoordP2ui) cl_get_proc_address_extension("glTexCoordP2ui");
		functions->texCoordP2uiv = (GLFunctions::ptr_glTexCoordP2uiv) cl_get_proc_address_extension("glTexCoordP2uiv");
		functions->texCoordP3ui = (GLFunctions::ptr_glTexCoordP3ui) cl_get_proc_address_extension("glTexCoordP3ui");
		functions->texCoordP3uiv = (GLFunctions::ptr_glTexCoordP3uiv) cl_get_proc_address_extension("glTexCoordP3uiv");
		functions->texCoordP4ui = (GLFunctions::ptr_glTexCoordP4ui) cl_get_proc_address_extension("glTexCoordP4ui");
		functions->texCoordP4uiv = (GLFunctions::ptr_glTexCoordP4uiv) cl_get_proc_address_extension("glTexCoordP4uiv");
		functions->multiTexCoordP1ui = (GLFunctions::ptr_glMultiTexCoordP1ui) cl_get_proc_address_extension("glMultiTexCoordP1ui");
		functions->multiTexCoordP1uiv = (GLFunctions::ptr_glMultiTexCoordP1uiv) cl_get_proc_address_extension("glMultiTexCoordP1uiv");
		functions->multiTexCoordP2ui = (GLFunctions::ptr_glMultiTexCoordP2ui) cl_get_proc_address_extension("glMultiTexCoordP2ui");
		functions->multiTexCoordP2uiv = (GLFunctions::ptr_glMultiTexCoordP2uiv) cl_get_proc_address_extension("glMultiTexCoordP2uiv");
		functions->multiTexCoordP3ui = (GLFunctions::ptr_glMultiTexCoordP3ui) cl_get_proc_address_extension("glMultiTexCoordP3ui");
		functions->multiTexCoordP3uiv = (GLFunctions::ptr_glMultiTexCoordP3uiv) cl_get_proc_address_extension("glMultiTexCoordP3uiv");
		functions->multiTexCoordP4ui = (GLFunctions::ptr_glMultiTexCoordP4ui) cl_get_proc_address_extension("glMultiTexCoordP4ui");
		functions->multiTexCoordP4uiv = (GLFunctions::ptr_glMultiTexCoordP4uiv) cl_get_proc_address_extension("glMultiTexCoordP4uiv");
		functions->normalP3ui = (GLFunctions::ptr_glNormalP3ui) cl_get_proc_address_extension("glNormalP3ui");
		functions->normalP3uiv = (GLFunctions::ptr_glNormalP3uiv) cl_get_proc_address_extension("glNormalP3uiv");
		functions->colorP3ui = (GLFunctions::ptr_glColorP3ui) cl_get_proc_address_extension("glColorP3ui");
		functions->colorP3uiv = (GLFunctions::ptr_glColorP3uiv) cl_get_proc_address_extension("glColorP3uiv");
		functions->colorP4ui = (GLFunctions::ptr_glColorP4ui) cl_get_proc_address_extension("glColorP4ui");
		functions->colorP4uiv = (GLFunctions::ptr_glColorP4uiv) cl_get_proc_address_extension("glColorP4uiv");
		functions->secondaryColorP3ui = (GLFunctions::ptr_glSecondaryColorP3ui) cl_get_proc_address_extension("glSecondaryColorP3ui");
		functions->secondaryColorP3uiv = (GLFunctions::ptr_glSecondaryColorP3uiv) cl_get_proc_address_extension("glSecondaryColorP3uiv");
		functions->vertexAttribP1ui = (GLFunctions::ptr_glVertexAttribP1ui) cl_get_proc_address_extension("glVertexAttribP1ui");
		functions->vertexAttribP1uiv = (GLFunctions::ptr_glVertexAttribP1uiv) cl_get_proc_address_extension("glVertexAttribP1uiv");
		functions->vertexAttribP2ui = (GLFunctions::ptr_glVertexAttribP2ui) cl_get_proc_address_extension("glVertexAttribP2ui");
		functions->vertexAttribP2uiv = (GLFunctions::ptr_glVertexAttribP2uiv) cl_get_proc_address_extension("glVertexAttribP2uiv");
		functions->vertexAttribP3ui = (GLFunctions::ptr_glVertexAttribP3ui) cl_get_proc_address_extension("glVertexAttribP3ui");
		functions->vertexAttribP3uiv = (GLFunctions::ptr_glVertexAttribP3uiv) cl_get_proc_address_extension("glVertexAttribP3uiv");
		functions->vertexAttribP4ui = (GLFunctions::ptr_glVertexAttribP4ui) cl_get_proc_address_extension("glVertexAttribP4ui");
		functions->vertexAttribP4uiv = (GLFunctions::ptr_glVertexAttribP4uiv) cl_get_proc_address_extension("glVertexAttribP4uiv");
		functions->drawArraysIndirect = (GLFunctions::ptr_glDrawArraysIndirect) cl_get_proc_address_extension("glDrawArraysIndirect");
		functions->drawElementsIndirect = (GLFunctions::ptr_glDrawElementsIndirect) cl_get_proc_address_extension("glDrawElementsIndirect");
		functions->uniform1d = (GLFunctions::ptr_glUniform1d) cl_get_proc_address_extension("glUniform1d");
		functions->uniform2d = (GLFunctions::ptr_glUniform2d) cl_get_proc_address_extension("glUniform2d");
		functions->uniform3d = (GLFunctions::ptr_glUniform3d) cl_get_proc_address_extension("glUniform3d");
		functions->uniform4d = (GLFunctions::ptr_glUniform4d) cl_get_proc_address_extension("glUniform4d");
		functions->uniform1dv = (GLFunctions::ptr_glUniform1dv) cl_get_proc_address_extension("glUniform1dv");
		functions->uniform2dv = (GLFunctions::ptr_glUniform2dv) cl_get_proc_address_extension("glUniform2dv");
		functions->uniform3dv = (GLFunctions::ptr_glUniform3dv) cl_get_proc_address_extension("glUniform3dv");
		functions->uniform4dv = (GLFunctions::ptr_glUniform4dv) cl_get_proc_address_extension("glUniform4dv");
		functions->uniformMatrix2dv = (GLFunctions::ptr_glUniformMatrix2dv) cl_get_proc_address_extension("glUniformMatrix2dv");
		functions->uniformMatrix3dv = (GLFunctions::ptr_glUniformMatrix3dv) cl_get_proc_address_extension("glUniformMatrix3dv");
		functions->uniformMatrix4dv = (GLFunctions::ptr_glUniformMatrix4dv) cl_get_proc_address_extension("glUniformMatrix4dv");
		functions->uniformMatrix2x3dv = (GLFunctions::ptr_glUniformMatrix2x3dv) cl_get_proc_address_extension("glUniformMatrix2x3dv");
		functions->uniformMatrix2x4dv = (GLFunctions::ptr_glUniformMatrix2x4dv) cl_get_proc_address_extension("glUniformMatrix2x4dv");
		functions->uniformMatrix3x2dv = (GLFunctions::ptr_glUniformMatrix3x2dv) cl_get_proc_address_extension("glUniformMatrix3x2dv");
		functions->uniformMatrix3x4dv = (GLFunctions::ptr_glUniformMatrix3x4dv) cl_get_proc_address_extension("glUniformMatrix3x4dv");
		functions->uniformMatrix4x2dv = (GLFunctions::ptr_glUniformMatrix4x2dv) cl_get_proc_address_extension("glUniformMatrix4x2dv");
		functions->uniformMatrix4x3dv = (GLFunctions::ptr_glUniformMatrix4x3dv) cl_get_proc_address_extension("glUniformMatrix4x3dv");
		functions->getUniformdv = (GLFunctions::ptr_glGetUniformdv) cl_get_proc_address_extension("glGetUniformdv");
		functions->getSubroutineUniformLocation = (GLFunctions::ptr_glGetSubroutineUniformLocation) cl_get_proc_address_extension("glGetSubroutineUniformLocation");
		functions->getSubroutineIndex = (GLFunctions::ptr_glGetSubroutineIndex) cl_get_proc_address_extension("glGetSubroutineIndex");
		functions->getActiveSubroutineUniformiv = (GLFunctions::ptr_glGetActiveSubroutineUniformiv) cl_get_proc_address_extension("glGetActiveSubroutineUniformiv");
		functions->getActiveSubroutineUniformName = (GLFunctions::ptr_glGetActiveSubroutineUniformName) cl_get_proc_address_extension("glGetActiveSubroutineUniformName");
		functions->getActiveSubroutineName = (GLFunctions::ptr_glGetActiveSubroutineName) cl_get_proc_address_extension("glGetActiveSubroutineName");
		functions->uniformSubroutinesuiv = (GLFunctions::ptr_glUniformSubroutinesuiv) cl_get_proc_address_extension("glUniformSubroutinesuiv");
		functions->getUniformSubroutineuiv = (GLFunctions::ptr_glGetUniformSubroutineuiv) cl_get_proc_address_extension("glGetUniformSubroutineuiv");
		functions->getProgramStageiv = (GLFunctions::ptr_glGetProgramStageiv) cl_get_proc_address_extension("glGetProgramStageiv");
		functions->patchParameteri = (GLFunctions::ptr_glPatchParameteri) cl_get_proc_address_extension("glPatchParameteri");
		functions->patchParameterfv = (GLFunctions::ptr_glPatchParameterfv) cl_get_proc_address_extension("glPatchParameterfv");
		functions->bindTransformFeedback = (GLFunctions::ptr_glBindTransformFeedback) cl_get_proc_address_extension("glBindTransformFeedback");
		functions->deleteTransformFeedbacks = (GLFunctions::ptr_glDeleteTransformFeedbacks) cl_get_proc_address_extension("glDeleteTransformFeedbacks");
		functions->genTransformFeedbacks = (GLFunctions::ptr_glGenTransformFeedbacks) cl_get_proc_address_extension("glGenTransformFeedbacks");
		functions->isTransformFeedback = (GLFunctions::ptr_glIsTransformFeedback) cl_get_proc_address_extension("glIsTransformFeedback");
		functions->pauseTransformFeedback = (GLFunctions::ptr_glPauseTransformFeedback) cl_get_proc_address_extension("glPauseTransformFeedback");
		functions->resumeTransformFeedback = (GLFunctions::ptr_glResumeTransformFeedback) cl_get_proc_address_extension("glResumeTransformFeedback");
		functions->drawTransformFeedback = (GLFunctions::ptr_glDrawTransformFeedback) cl_get_proc_address_extension("glDrawTransformFeedback");
		functions->drawTransformFeedbackStream = (GLFunctions::ptr_glDrawTransformFeedbackStream) cl_get_proc_address_extension("glDrawTransformFeedbackStream");
		functions->beginQueryIndexed = (GLFunctions::ptr_glBeginQueryIndexed) cl_get_proc_address_extension("glBeginQueryIndexed");
		functions->endQueryIndexed = (GLFunctions::ptr_glEndQueryIndexed) cl_get_proc_address_extension("glEndQueryIndexed");
		functions->getQueryIndexediv = (GLFunctions::ptr_glGetQueryIndexediv) cl_get_proc_address_extension("glGetQueryIndexediv");
		functions->releaseShaderCompiler = (GLFunctions::ptr_glReleaseShaderCompiler) cl_get_proc_address_extension("glReleaseShaderCompiler");
		functions->shaderBinary = (GLFunctions::ptr_glShaderBinary) cl_get_proc_address_extension("glShaderBinary");
		functions->getShaderPrecisionFormat = (GLFunctions::ptr_glGetShaderPrecisionFormat) cl_get_proc_address_extension("glGetShaderPrecisionFormat");
		functions->depthRangef = (GLFunctions::ptr_glDepthRangef) cl_get_proc_address_extension("glDepthRangef");
		functions->clearDepthf = (GLFunctions::ptr_glClearDepthf) cl_get_proc_address_extension("glClearDepthf");
		functions->getProgramBinary = (GLFunctions::ptr_glGetProgramBinary) cl_get_proc_address_extension("glGetProgramBinary");
		functions->programBinary = (GLFunctions::ptr_glProgramBinary) cl_get_proc_address_extension("glProgramBinary");
		functions->programParameteri = (GLFunctions::ptr_glProgramParameteri) cl_get_proc_address_extension("glProgramParameteri");
		functions->useProgramStages = (GLFunctions::ptr_glUseProgramStages) cl_get_proc_address_extension("glUseProgramStages");
		functions->activeShaderProgram = (GLFunctions::ptr_glActiveShaderProgram) cl_get_proc_address_extension("glActiveShaderProgram");
		functions->createShaderProgramv = (GLFunctions::ptr_glCreateShaderProgramv) cl_get_proc_address_extension("glCreateShaderProgramv");
		functions->bindProgramPipeline = (GLFunctions::ptr_glBindProgramPipeline) cl_get_proc_address_extension("glBindProgramPipeline");
		functions->deleteProgramPipelines = (GLFunctions::ptr_glDeleteProgramPipelines) cl_get_proc_address_extension("glDeleteProgramPipelines");
		functions->genProgramPipelines = (GLFunctions::ptr_glGenProgramPipelines) cl_get_proc_address_extension("glGenProgramPipelines");
		functions->isProgramPipeline = (GLFunctions::ptr_glIsProgramPipeline) cl_get_proc_address_extension("glIsProgramPipeline");
		functions->getProgramPipelineiv = (GLFunctions::ptr_glGetProgramPipelineiv) cl_get_proc_address_extension("glGetProgramPipelineiv");
		functions->programUniform1i = (GLFunctions::ptr_glProgramUniform1i) cl_get_proc_address_extension("glProgramUniform1i");
		functions->programUniform1iv = (GLFunctions::ptr_glProgramUniform1iv) cl_get_proc_address_extension("glProgramUniform1iv");
		functions->programUniform1f = (GLFunctions::ptr_glProgramUniform1f) cl_get_proc_address_extension("glProgramUniform1f");
		functions->programUniform1fv = (GLFunctions::ptr_glProgramUniform1fv) cl_get_proc_address_extension("glProgramUniform1fv");
		functions->programUniform1d = (GLFunctions::ptr_glProgramUniform1d) cl_get_proc_address_extension("glProgramUniform1d");
		functions->programUniform1dv = (GLFunctions::ptr_glProgramUniform1dv) cl_get_proc_address_extension("glProgramUniform1dv");
		functions->programUniform1ui = (GLFunctions::ptr_glProgramUniform1ui) cl_get_proc_address_extension("glProgramUniform1ui");
		functions->programUniform1uiv = (GLFunctions::ptr_glProgramUniform1uiv) cl_get_proc_address_extension("glProgramUniform1uiv");
		functions->programUniform2i = (GLFunctions::ptr_glProgramUniform2i) cl_get_proc_address_extension("glProgramUniform2i");
		functions->programUniform2iv = (GLFunctions::ptr_glProgramUniform2iv) cl_get_proc_address_extension("glProgramUniform2iv");
		functions->programUniform2f = (GLFunctions::ptr_glProgramUniform2f) cl_get_proc_address_extension("glProgramUniform2f");
		functions->programUniform2fv = (GLFunctions::ptr_glProgramUniform2fv) cl_get_proc_address_extension("glProgramUniform2fv");
		functions->programUniform2d = (GLFunctions::ptr_glProgramUniform2d) cl_get_proc_address_extension("glProgramUniform2d");
		functions->programUniform2dv = (GLFunctions::ptr_glProgramUniform2dv) cl_get_proc_address_extension("glProgramUniform2dv");
		functions->programUniform2ui = (GLFunctions::ptr_glProgramUniform2ui) cl_get_proc_address_extension("glProgramUniform2ui");
		functions->programUniform2uiv = (GLFunctions::ptr_glProgramUniform2uiv) cl_get_proc_address_extension("glProgramUniform2uiv");
		functions->programUniform3i = (GLFunctions::ptr_glProgramUniform3i) cl_get_proc_address_extension("glProgramUniform3i");
		functions->programUniform3iv = (GLFunctions::ptr_glProgramUniform3iv) cl_get_proc_address_extension("glProgramUniform3iv");
		functions->programUniform3f = (GLFunctions::ptr_glProgramUniform3f) cl_get_proc_address_extension("glProgramUniform3f");
		functions->programUniform3fv = (GLFunctions::ptr_glProgramUniform3fv) cl_get_proc_address_extension("glProgramUniform3fv");
		functions->programUniform3d = (GLFunctions::ptr_glProgramUniform3d) cl_get_proc_address_extension("glProgramUniform3d");
		functions->programUniform3dv = (GLFunctions::ptr_glProgramUniform3dv) cl_get_proc_address_extension("glProgramUniform3dv");
		functions->programUniform3ui = (GLFunctions::ptr_glProgramUniform3ui) cl_get_proc_address_extension("glProgramUniform3ui");
		functions->programUniform3uiv = (GLFunctions::ptr_glProgramUniform3uiv) cl_get_proc_address_extension("glProgramUniform3uiv");
		functions->programUniform4i = (GLFunctions::ptr_glProgramUniform4i) cl_get_proc_address_extension("glProgramUniform4i");
		functions->programUniform4iv = (GLFunctions::ptr_glProgramUniform4iv) cl_get_proc_address_extension("glProgramUniform4iv");
		functions->programUniform4f = (GLFunctions::ptr_glProgramUniform4f) cl_get_proc_address_extension("glProgramUniform4f");
		functions->programUniform4fv = (GLFunctions::ptr_glProgramUniform4fv) cl_get_proc_address_extension("glProgramUniform4fv");
		functions->programUniform4d = (GLFunctions::ptr_glProgramUniform4d) cl_get_proc_address_extension("glProgramUniform4d");
		functions->programUniform4dv = (GLFunctions::ptr_glProgramUniform4dv) cl_get_proc_address_extension("glProgramUniform4dv");
		functions->programUniform4ui = (GLFunctions::ptr_glProgramUniform4ui) cl_get_proc_address_extension("glProgramUniform4ui");
		functions->programUniform4uiv = (GLFunctions::ptr_glProgramUniform4uiv) cl_get_proc_address_extension("glProgramUniform4uiv");
		functions->programUniformMatrix2fv = (GLFunctions::ptr_glProgramUniformMatrix2fv) cl_get_proc_address_extension("glProgramUniformMatrix2fv");
		functions->programUniformMatrix3fv = (GLFunctions::ptr_glProgramUniformMatrix3fv) cl_get_proc_address_extension("glProgramUniformMatrix3fv");
		functions->programUniformMatrix4fv = (GLFunctions::ptr_glProgramUniformMatrix4fv) cl_get_proc_address_extension("glProgramUniformMatrix4fv");
		functions->programUniformMatrix2dv = (GLFunctions::ptr_glProgramUniformMatrix2dv) cl_get_proc_address_extension("glProgramUniformMatrix2dv");
		functions->programUniformMatrix3dv = (GLFunctions::ptr_glProgramUniformMatrix3dv) cl_get_proc_address_extension("glProgramUniformMatrix3dv");
		functions->programUniformMatrix4dv = (GLFunctions::ptr_glProgramUniformMatrix4dv) cl_get_proc_address_extension("glProgramUniformMatrix4dv");
		functions->programUniformMatrix2x3fv = (GLFunctions::ptr_glProgramUniformMatrix2x3fv) cl_get_proc_address_extension("glProgramUniformMatrix2x3fv");
		functions->programUniformMatrix3x2fv = (GLFunctions::ptr_glProgramUniformMatrix3x2fv) cl_get_proc_address_extension("glProgramUniformMatrix3x2fv");
		functions->programUniformMatrix2x4fv = (GLFunctions::ptr_glProgramUniformMatrix2x4fv) cl_get_proc_address_extension("glProgramUniformMatrix2x4fv");
		functions->programUniformMatrix4x2fv = (GLFunctions::ptr_glProgramUniformMatrix4x2fv) cl_get_proc_address_extension("glProgramUniformMatrix4x2fv");
		functions->programUniformMatrix3x4fv = (GLFunctions::ptr_glProgramUniformMatrix3x4fv) cl_get_proc_address_extension("glProgramUniformMatrix3x4fv");
		functions->programUniformMatrix4x3fv = (GLFunctions::ptr_glProgramUniformMatrix4x3fv) cl_get_proc_address_extension("glProgramUniformMatrix4x3fv");
		functions->programUniformMatrix2x3dv = (GLFunctions::ptr_glProgramUniformMatrix2x3dv) cl_get_proc_address_extension("glProgramUniformMatrix2x3dv");
		functions->programUniformMatrix3x2dv = (GLFunctions::ptr_glProgramUniformMatrix3x2dv) cl_get_proc_address_extension("glProgramUniformMatrix3x2dv");
		functions->programUniformMatrix2x4dv = (GLFunctions::ptr_glProgramUniformMatrix2x4dv) cl_get_proc_address_extension("glProgramUniformMatrix2x4dv");
		functions->programUniformMatrix4x2dv = (GLFunctions::ptr_glProgramUniformMatrix4x2dv) cl_get_proc_address_extension("glProgramUniformMatrix4x2dv");
		functions->programUniformMatrix3x4dv = (GLFunctions::ptr_glProgramUniformMatrix3x4dv) cl_get_proc_address_extension("glProgramUniformMatrix3x4dv");
		functions->programUniformMatrix4x3dv = (GLFunctions::ptr_glProgramUniformMatrix4x3dv) cl_get_proc_address_extension("glProgramUniformMatrix4x3dv");
		functions->validateProgramPipeline = (GLFunctions::ptr_glValidateProgramPipeline) cl_get_proc_address_extension("glValidateProgramPipeline");
		functions->getProgramPipelineInfoLog = (GLFunctions::ptr_glGetProgramPipelineInfoLog) cl_get_proc_address_extension("glGetProgramPipelineInfoLog");
		functions->vertexAttribL1d = (GLFunctions::ptr_glVertexAttribL1d) cl_get_proc_address_extension("glVertexAttribL1d");
		functions->vertexAttribL2d = (GLFunctions::ptr_glVertexAttribL2d) cl_get_proc_address_extension("glVertexAttribL2d");
		functions->vertexAttribL3d = (GLFunctions::ptr_glVertexAttribL3d) cl_get_proc_address_extension("glVertexAttribL3d");
		functions->vertexAttribL4d = (GLFunctions::ptr_glVertexAttribL4d) cl_get_proc_address_extension("glVertexAttribL4d");
		functions->vertexAttribL1dv = (GLFunctions::ptr_glVertexAttribL1dv) cl_get_proc_address_extension("glVertexAttribL1dv");
		functions->vertexAttribL2dv = (GLFunctions::ptr_glVertexAttribL2dv) cl_get_proc_address_extension("glVertexAttribL2dv");
		functions->vertexAttribL3dv = (GLFunctions::ptr_glVertexAttribL3dv) cl_get_proc_address_extension("glVertexAttribL3dv");
		functions->vertexAttribL4dv = (GLFunctions::ptr_glVertexAttribL4dv) cl_get_proc_address_extension("glVertexAttribL4dv");
		functions->vertexAttribLPointer = (GLFunctions::ptr_glVertexAttribLPointer) cl_get_proc_address_extension("glVertexAttribLPointer");
		functions->getVertexAttribLdv = (GLFunctions::ptr_glGetVertexAttribLdv) cl_get_proc_address_extension("glGetVertexAttribLdv");
		functions->viewportArrayv = (GLFunctions::ptr_glViewportArrayv) cl_get_proc_address_extension("glViewportArrayv");
		functions->viewportIndexedf = (GLFunctions::ptr_glViewportIndexedf) cl_get_proc_address_extension("glViewportIndexedf");
		functions->viewportIndexedfv = (GLFunctions::ptr_glViewportIndexedfv) cl_get_proc_address_extension("glViewportIndexedfv");
		functions->scissorArrayv = (GLFunctions::ptr_glScissorArrayv) cl_get_proc_address_extension("glScissorArrayv");
		functions->scissorIndexed = (GLFunctions::ptr_glScissorIndexed) cl_get_proc_address_extension("glScissorIndexed");
		functions->scissorIndexedv = (GLFunctions::ptr_glScissorIndexedv) cl_get_proc_address_extension("glScissorIndexedv");
		functions->depthRangeArrayv = (GLFunctions::ptr_glDepthRangeArrayv) cl_get_proc_address_extension("glDepthRangeArrayv");
		functions->depthRangeIndexed = (GLFunctions::ptr_glDepthRangeIndexed) cl_get_proc_address_extension("glDepthRangeIndexed");
		functions->getFloati_v = (GLFunctions::ptr_glGetFloati_v) cl_get_proc_address_extension("glGetFloati_v");
		functions->getDoublei_v = (GLFunctions::ptr_glGetDoublei_v) cl_get_proc_address_extension("glGetDoublei_v");
		functions->createSyncFromCLeventARB = (GLFunctions::ptr_glCreateSyncFromCLeventARB) cl_get_proc_address_extension("glCreateSyncFromCLeventARB");
		functions->debugMessageControlARB = (GLFunctions::ptr_glDebugMessageControlARB) cl_get_proc_address_extension("glDebugMessageControlARB");
		functions->debugMessageInsertARB = (GLFunctions::ptr_glDebugMessageInsertARB) cl_get_proc_address_extension("glDebugMessageInsertARB");
		functions->debugMessageCallbackARB = (GLFunctions::ptr_glDebugMessageCallbackARB) cl_get_proc_address_extension("glDebugMessageCallbackARB");
		functions->getDebugMessageLogARB = (GLFunctions::ptr_glGetDebugMessageLogARB) cl_get_proc_address_extension("glGetDebugMessageLogARB");
		functions->getGraphicsResetStatusARB = (GLFunctions::ptr_glGetGraphicsResetStatusARB) cl_get_proc_address_extension("glGetGraphicsResetStatusARB");
		functions->getnMapdvARB = (GLFunctions::ptr_glGetnMapdvARB) cl_get_proc_address_extension("glGetnMapdvARB");
		functions->getnMapfvARB = (GLFunctions::ptr_glGetnMapfvARB) cl_get_proc_address_extension("glGetnMapfvARB");
		functions->getnMapivARB = (GLFunctions::ptr_glGetnMapivARB) cl_get_proc_address_extension("glGetnMapivARB");
		functions->getnPixelMapfvARB = (GLFunctions::ptr_glGetnPixelMapfvARB) cl_get_proc_address_extension("glGetnPixelMapfvARB");
		functions->getnPixelMapuivARB = (GLFunctions::ptr_glGetnPixelMapuivARB) cl_get_proc_address_extension("glGetnPixelMapuivARB");
		functions->getnPixelMapusvARB = (GLFunctions::ptr_glGetnPixelMapusvARB) cl_get_proc_address_extension("glGetnPixelMapusvARB");
		functions->getnPolygonStippleARB = (GLFunctions::ptr_glGetnPolygonStippleARB) cl_get_proc_address_extension("glGetnPolygonStippleARB");
		functions->getnColorTableARB = (GLFunctions::ptr_glGetnColorTableARB) cl_get_proc_address_extension("glGetnColorTableARB");
		functions->getnConvolutionFilterARB = (GLFunctions::ptr_glGetnConvolutionFilterARB) cl_get_proc_address_extension("glGetnConvolutionFilterARB");
		functions->getnSeparableFilterARB = (GLFunctions::ptr_glGetnSeparableFilterARB) cl_get_proc_address_extension("glGetnSeparableFilterARB");
		functions->getnHistogramARB = (GLFunctions::ptr_glGetnHistogramARB) cl_get_proc_address_extension("glGetnHistogramARB");
		functions->getnMinmaxARB = (GLFunctions::ptr_glGetnMinmaxARB) cl_get_proc_address_extension("glGetnMinmaxARB");
		functions->getnTexImageARB = (GLFunctions::ptr_glGetnTexImageARB) cl_get_proc_address_extension("glGetnTexImageARB");
		functions->readnPixelsARB = (GLFunctions::ptr_glReadnPixelsARB) cl_get_proc_address_extension("glReadnPixelsARB");
		functions->getnCompressedTexImageARB = (GLFunctions::ptr_glGetnCompressedTexImageARB) cl_get_proc_address_extension("glGetnCompressedTexImageARB");
		functions->getnUniformfvARB = (GLFunctions::ptr_glGetnUniformfvARB) cl_get_proc_address_extension("glGetnUniformfvARB");
		functions->getnUniformivARB = (GLFunctions::ptr_glGetnUniformivARB) cl_get_proc_address_extension("glGetnUniformivARB");
		functions->getnUniformuivARB = (GLFunctions::ptr_glGetnUniformuivARB) cl_get_proc_address_extension("glGetnUniformuivARB");
		functions->getnUniformdvARB = (GLFunctions::ptr_glGetnUniformdvARB) cl_get_proc_address_extension("glGetnUniformdvARB");
		functions->drawArraysInstancedBaseInstance = (GLFunctions::ptr_glDrawArraysInstancedBaseInstance) cl_get_proc_address_extension("glDrawArraysInstancedBaseInstance");
		functions->drawElementsInstancedBaseInstance = (GLFunctions::ptr_glDrawElementsInstancedBaseInstance) cl_get_proc_address_extension("glDrawElementsInstancedBaseInstance");
		functions->drawElementsInstancedBaseVertexBaseInstance = (GLFunctions::ptr_glDrawElementsInstancedBaseVertexBaseInstance) cl_get_proc_address_extension("glDrawElementsInstancedBaseVertexBaseInstance");
		functions->drawTransformFeedbackInstanced = (GLFunctions::ptr_glDrawTransformFeedbackInstanced) cl_get_proc_address_extension("glDrawTransformFeedbackInstanced");
		functions->drawTransformFeedbackStreamInstanced = (GLFunctions::ptr_glDrawTransformFeedbackStreamInstanced) cl_get_proc_address_extension("glDrawTransformFeedbackStreamInstanced");
		functions->getInternalformativ = (GLFunctions::ptr_glGetInternalformativ) cl_get_proc_address_extension("glGetInternalformativ");
		functions->getActiveAtomicCounterBufferiv = (GLFunctions::ptr_glGetActiveAtomicCounterBufferiv) cl_get_proc_address_extension("glGetActiveAtomicCounterBufferiv");
		functions->bindImageTexture = (GLFunctions::ptr_glBindImageTexture) cl_get_proc_address_extension("glBindImageTexture");
		functions->memoryBarrier = (GLFunctions::ptr_glMemoryBarrier) cl_get_proc_address_extension("glMemoryBarrier");
		functions->texStorage1D = (GLFunctions::ptr_glTexStorage1D) cl_get_proc_address_extension("glTexStorage1D");
		functions->texStorage2D = (GLFunctions::ptr_glTexStorage2D) cl_get_proc_address_extension("glTexStorage2D");
		functions->texStorage3D = (GLFunctions::ptr_glTexStorage3D) cl_get_proc_address_extension("glTexStorage3D");
		functions->textureStorage1DEXT = (GLFunctions::ptr_glTextureStorage1DEXT) cl_get_proc_address_extension("glTextureStorage1DEXT");
		functions->textureStorage2DEXT = (GLFunctions::ptr_glTextureStorage2DEXT) cl_get_proc_address_extension("glTextureStorage2DEXT");
		functions->textureStorage3DEXT = (GLFunctions::ptr_glTextureStorage3DEXT) cl_get_proc_address_extension("glTextureStorage3DEXT");
		functions->debugMessageControl = (GLFunctions::ptr_glDebugMessageControl) cl_get_proc_address_extension("glDebugMessageControl");
		functions->debugMessageInsert = (GLFunctions::ptr_glDebugMessageInsert) cl_get_proc_address_extension("glDebugMessageInsert");
		functions->debugMessageCallback = (GLFunctions::ptr_glDebugMessageCallback) cl_get_proc_address_extension("glDebugMessageCallback");
		functions->getDebugMessageLog = (GLFunctions::ptr_glGetDebugMessageLog) cl_get_proc_address_extension("glGetDebugMessageLog");
		functions->pushDebugGroup = (GLFunctions::ptr_glPushDebugGroup) cl_get_proc_address_extension("glPushDebugGroup");
		functions->popDebugGroup = (GLFunctions::ptr_glPopDebugGroup) cl_get_proc_address_extension("glPopDebugGroup");
		functions->objectLabel = (GLFunctions::ptr_glObjectLabel) cl_get_proc_address_extension("glObjectLabel");
		functions->getObjectLabel = (GLFunctions::ptr_glGetObjectLabel) cl_get_proc_address_extension("glGetObjectLabel");
		functions->objectPtrLabel = (GLFunctions::ptr_glObjectPtrLabel) cl_get_proc_address_extension("glObjectPtrLabel");
		functions->getObjectPtrLabel = (GLFunctions::ptr_glGetObjectPtrLabel) cl_get_proc_address_extension("glGetObjectPtrLabel");
		functions->clearBufferData = (GLFunctions::ptr_glClearBufferData) cl_get_proc_address_extension("glClearBufferData");
		functions->clearBufferSubData = (GLFunctions::ptr_glClearBufferSubData) cl_get_proc_address_extension("glClearBufferSubData");
		functions->clearNamedBufferDataEXT = (GLFunctions::ptr_glClearNamedBufferDataEXT) cl_get_proc_address_extension("glClearNamedBufferDataEXT");
		functions->clearNamedBufferSubDataEXT = (GLFunctions::ptr_glClearNamedBufferSubDataEXT) cl_get_proc_address_extension("glClearNamedBufferSubDataEXT");
		functions->dispatchCompute = (GLFunctions::ptr_glDispatchCompute) cl_get_proc_address_extension("glDispatchCompute");
		functions->dispatchComputeIndirect = (GLFunctions::ptr_glDispatchComputeIndirect) cl_get_proc_address_extension("glDispatchComputeIndirect");
		functions->copyImageSubData = (GLFunctions::ptr_glCopyImageSubData) cl_get_proc_address_extension("glCopyImageSubData");
		functions->framebufferParameteri = (GLFunctions::ptr_glFramebufferParameteri) cl_get_proc_address_extension("glFramebufferParameteri");
		functions->getFramebufferParameteriv = (GLFunctions::ptr_glGetFramebufferParameteriv) cl_get_proc_address_extension("glGetFramebufferParameteriv");
		functions->namedFramebufferParameteriEXT = (GLFunctions::ptr_glNamedFramebufferParameteriEXT) cl_get_proc_address_extension("glNamedFramebufferParameteriEXT");
		functions->getNamedFramebufferParameterivEXT = (GLFunctions::ptr_glGetNamedFramebufferParameterivEXT) cl_get_proc_address_extension("glGetNamedFramebufferParameterivEXT");
		functions->getInternalformati64v = (GLFunctions::ptr_glGetInternalformati64v) cl_get_proc_address_extension("glGetInternalformati64v");
		functions->invalidateTexSubImage = (GLFunctions::ptr_glInvalidateTexSubImage) cl_get_proc_address_extension("glInvalidateTexSubImage");
		functions->invalidateTexImage = (GLFunctions::ptr_glInvalidateTexImage) cl_get_proc_address_extension("glInvalidateTexImage");
		functions->invalidateBufferSubData = (GLFunctions::ptr_glInvalidateBufferSubData) cl_get_proc_address_extension("glInvalidateBufferSubData");
		functions->invalidateBufferData = (GLFunctions::ptr_glInvalidateBufferData) cl_get_proc_address_extension("glInvalidateBufferData");
		functions->invalidateFramebuffer = (GLFunctions::ptr_glInvalidateFramebuffer) cl_get_proc_address_extension("glInvalidateFramebuffer");
		functions->invalidateSubFramebuffer = (GLFunctions::ptr_glInvalidateSubFramebuffer) cl_get_proc_address_extension("glInvalidateSubFramebuffer");
		functions->multiDrawArraysIndirect = (GLFunctions::ptr_glMultiDrawArraysIndirect) cl_get_proc_address_extension("glMultiDrawArraysIndirect");
		functions->multiDrawElementsIndirect = (GLFunctions::ptr_glMultiDrawElementsIndirect) cl_get_proc_address_extension("glMultiDrawElementsIndirect");
		functions->getProgramInterfaceiv = (GLFunctions::ptr_glGetProgramInterfaceiv) cl_get_proc_address_extension("glGetProgramInterfaceiv");
		functions->getProgramResourceIndex = (GLFunctions::ptr_glGetProgramResourceIndex) cl_get_proc_address_extension("glGetProgramResourceIndex");
		functions->getProgramResourceName = (GLFunctions::ptr_glGetProgramResourceName) cl_get_proc_address_extension("glGetProgramResourceName");
		functions->getProgramResourceiv = (GLFunctions::ptr_glGetProgramResourceiv) cl_get_proc_address_extension("glGetProgramResourceiv");
		functions->getProgramResourceLocation = (GLFunctions::ptr_glGetProgramResourceLocation) cl_get_proc_address_extension("glGetProgramResourceLocation");
		functions->getProgramResourceLocationIndex = (GLFunctions::ptr_glGetProgramResourceLocationIndex) cl_get_proc_address_extension("glGetProgramResourceLocationIndex");
		functions->shaderStorageBlockBinding = (GLFunctions::ptr_glShaderStorageBlockBinding) cl_get_proc_address_extension("glShaderStorageBlockBinding");
		functions->texBufferRange = (GLFunctions::ptr_glTexBufferRange) cl_get_proc_address_extension("glTexBufferRange");
		functions->textureBufferRangeEXT = (GLFunctions::ptr_glTextureBufferRangeEXT) cl_get_proc_address_extension("glTextureBufferRangeEXT");
		functions->texStorage2DMultisample = (GLFunctions::ptr_glTexStorage2DMultisample) cl_get_proc_address_extension("glTexStorage2DMultisample");
		functions->texStorage3DMultisample = (GLFunctions::ptr_glTexStorage3DMultisample) cl_get_proc_address_extension("glTexStorage3DMultisample");
		functions->textureStorage2DMultisampleEXT = (GLFunctions::ptr_glTextureStorage2DMultisampleEXT) cl_get_proc_address_extension("glTextureStorage2DMultisampleEXT");
		functions->textureStorage3DMultisampleEXT = (GLFunctions::ptr_glTextureStorage3DMultisampleEXT) cl_get_proc_address_extension("glTextureStorage3DMultisampleEXT");
		functions->textureView = (GLFunctions::ptr_glTextureView) cl_get_proc_address_extension("glTextureView");
		functions->bindVertexBuffer = (GLFunctions::ptr_glBindVertexBuffer) cl_get_proc_address_extension("glBindVertexBuffer");
		functions->vertexAttribFormat = (GLFunctions::ptr_glVertexAttribFormat) cl_get_proc_address_extension("glVertexAttribFormat");
		functions->vertexAttribIFormat = (GLFunctions::ptr_glVertexAttribIFormat) cl_get_proc_address_extension("glVertexAttribIFormat");
		functions->vertexAttribLFormat = (GLFunctions::ptr_glVertexAttribLFormat) cl_get_proc_address_extension("glVertexAttribLFormat");
		functions->vertexAttribBinding = (GLFunctions::ptr_glVertexAttribBinding) cl_get_proc_address_extension("glVertexAttribBinding");
		functions->vertexBindingDivisor = (GLFunctions::ptr_glVertexBindingDivisor) cl_get_proc_address_extension("glVertexBindingDivisor");
		functions->vertexArrayBindVertexBufferEXT = (GLFunctions::ptr_glVertexArrayBindVertexBufferEXT) cl_get_proc_address_extension("glVertexArrayBindVertexBufferEXT");
		functions->vertexArrayVertexAttribFormatEXT = (GLFunctions::ptr_glVertexArrayVertexAttribFormatEXT) cl_get_proc_address_extension("glVertexArrayVertexAttribFormatEXT");
		functions->vertexArrayVertexAttribIFormatEXT = (GLFunctions::ptr_glVertexArrayVertexAttribIFormatEXT) cl_get_proc_address_extension("glVertexArrayVertexAttribIFormatEXT");
		functions->vertexArrayVertexAttribLFormatEXT = (GLFunctions::ptr_glVertexArrayVertexAttribLFormatEXT) cl_get_proc_address_extension("glVertexArrayVertexAttribLFormatEXT");
		functions->vertexArrayVertexAttribBindingEXT = (GLFunctions::ptr_glVertexArrayVertexAttribBindingEXT) cl_get_proc_address_extension("glVertexArrayVertexAttribBindingEXT");
		functions->vertexArrayVertexBindingDivisorEXT = (GLFunctions::ptr_glVertexArrayVertexBindingDivisorEXT) cl_get_proc_address_extension("glVertexArrayVertexBindingDivisorEXT");

	// OpenGL 4.4
		functions->BufferStorage = (GLFunctions::ptr_glBufferStorage) cl_get_proc_address_extension("glBufferStorage");
		functions->ClearTexImage = (GLFunctions::ptr_glClearTexImage) cl_get_proc_address_extension("glClearTexImage");
		functions->ClearTexSubImage = (GLFunctions::ptr_glClearTexSubImage) cl_get_proc_address_extension("glClearTexSubImage");
		functions->BindBuffersBase = (GLFunctions::ptr_glBindBuffersBase) cl_get_proc_address_extension("glBindBuffersBase");
		functions->BindBuffersRange = (GLFunctions::ptr_glBindBuffersRange) cl_get_proc_address_extension("glBindBuffersRange");
		functions->BindTextures = (GLFunctions::ptr_glBindTextures) cl_get_proc_address_extension("glBindTextures");
		functions->BindSamplers = (GLFunctions::ptr_glBindSamplers) cl_get_proc_address_extension("glBindSamplers");
		functions->BindImageTextures = (GLFunctions::ptr_glBindImageTextures) cl_get_proc_address_extension("glBindImageTextures");
		functions->BindVertexBuffers = (GLFunctions::ptr_glBindVertexBuffers) cl_get_proc_address_extension("glBindVertexBuffers");

	// OpenGL 4.5
		functions->ClipControl = (GLFunctions::ptr_glClipControl) cl_get_proc_address_extension("glClipControl");
		functions->CreateTransformFeedbacks = (GLFunctions::ptr_glCreateTransformFeedbacks) cl_get_proc_address_extension("glCreateTransformFeedbacks");
		functions->TransformFeedbackBufferBase = (GLFunctions::ptr_glTransformFeedbackBufferBase) cl_get_proc_address_extension("glTransformFeedbackBufferBase");
		functions->TransformFeedbackBufferRange = (GLFunctions::ptr_glTransformFeedbackBufferRange) cl_get_proc_address_extension("glTransformFeedbackBufferRange");
		functions->GetTransformFeedbackiv = (GLFunctions::ptr_glGetTransformFeedbackiv) cl_get_proc_address_extension("glGetTransformFeedbackiv");
		functions->GetTransformFeedbacki_v = (GLFunctions::ptr_glGetTransformFeedbacki_v) cl_get_proc_address_extension("glGetTransformFeedbacki_v");
		functions->GetTransformFeedbacki64_v = (GLFunctions::ptr_glGetTransformFeedbacki64_v) cl_get_proc_address_extension("glGetTransformFeedbacki64_v");
		functions->CreateBuffers = (GLFunctions::ptr_glCreateBuffers) cl_get_proc_address_extension("glCreateBuffers");
		functions->NamedBufferStorage = (GLFunctions::ptr_glNamedBufferStorage) cl_get_proc_address_extension("glNamedBufferStorage");
		functions->NamedBufferData = (GLFunctions::ptr_glNamedBufferData) cl_get_proc_address_extension("glNamedBufferData");
		functions->NamedBufferSubData = (GLFunctions::ptr_glNamedBufferSubData) cl_get_proc_address_extension("glNamedBufferSubData");
		functions->CopyNamedBufferSubData = (GLFunctions::ptr_glCopyNamedBufferSubData) cl_get_proc_address_extension("glCopyNamedBufferSubData");
		functions->ClearNamedBufferData = (GLFunctions::ptr_glClearNamedBufferData) cl_get_proc_address_extension("glClearNamedBufferData");
		functions->ClearNamedBufferSubData = (GLFunctions::ptr_glClearNamedBufferSubData) cl_get_proc_address_extension("glClearNamedBufferSubData");
		functions->MapNamedBuffer = (GLFunctions::ptr_glMapNamedBuffer) cl_get_proc_address_extension("glMapNamedBuffer");
		functions->MapNamedBufferRange = (GLFunctions::ptr_glMapNamedBufferRange) cl_get_proc_address_extension("glMapNamedBufferRange");
		functions->UnmapNamedBuffer = (GLFunctions::ptr_glUnmapNamedBuffer) cl_get_proc_address_extension("glUnmapNamedBuffer");
		functions->FlushMappedNamedBufferRange = (GLFunctions::ptr_glFlushMappedNamedBufferRange) cl_get_proc_address_extension("glFlushMappedNamedBufferRange");
		functions->GetNamedBufferParameteriv = (GLFunctions::ptr_glGetNamedBufferParameteriv) cl_get_proc_address_extension("glGetNamedBufferParameteriv");
		functions->GetNamedBufferParameteri64v = (GLFunctions::ptr_glGetNamedBufferParameteri64v) cl_get_proc_address_extension("glGetNamedBufferParameteri64v");
		functions->GetNamedBufferPointerv = (GLFunctions::ptr_glGetNamedBufferPointerv) cl_get_proc_address_extension("glGetNamedBufferPointerv");
		functions->GetNamedBufferSubData = (GLFunctions::ptr_glGetNamedBufferSubData) cl_get_proc_address_extension("glGetNamedBufferSubData");
		functions->CreateFramebuffers = (GLFunctions::ptr_glCreateFramebuffers) cl_get_proc_address_extension("glCreateFramebuffers");
		functions->NamedFramebufferRenderbuffer = (GLFunctions::ptr_glNamedFramebufferRenderbuffer) cl_get_proc_address_extension("glNamedFramebufferRenderbuffer");
		functions->NamedFramebufferParameteri = (GLFunctions::ptr_glNamedFramebufferParameteri) cl_get_proc_address_extension("glNamedFramebufferParameteri");
		functions->NamedFramebufferTexture = (GLFunctions::ptr_glNamedFramebufferTexture) cl_get_proc_address_extension("glNamedFramebufferTexture");
		functions->NamedFramebufferTextureLayer = (GLFunctions::ptr_glNamedFramebufferTextureLayer) cl_get_proc_address_extension("glNamedFramebufferTextureLayer");
		functions->NamedFramebufferDrawBuffer = (GLFunctions::ptr_glNamedFramebufferDrawBuffer) cl_get_proc_address_extension("glNamedFramebufferDrawBuffer");
		functions->NamedFramebufferDrawBuffers = (GLFunctions::ptr_glNamedFramebufferDrawBuffers) cl_get_proc_address_extension("glNamedFramebufferDrawBuffers");
		functions->NamedFramebufferReadBuffer = (GLFunctions::ptr_glNamedFramebufferReadBuffer) cl_get_proc_address_extension("glNamedFramebufferReadBuffer");
		functions->InvalidateNamedFramebufferData = (GLFunctions::ptr_glInvalidateNamedFramebufferData) cl_get_proc_address_extension("glInvalidateNamedFramebufferData");
		functions->InvalidateNamedFramebufferSubData = (GLFunctions::ptr_glInvalidateNamedFramebufferSubData) cl_get_proc_address_extension("glInvalidateNamedFramebufferSubData");
		functions->ClearNamedFramebufferiv = (GLFunctions::ptr_glClearNamedFramebufferiv) cl_get_proc_address_extension("glClearNamedFramebufferiv");
		functions->ClearNamedFramebufferuiv = (GLFunctions::ptr_glClearNamedFramebufferuiv) cl_get_proc_address_extension("glClearNamedFramebufferuiv");
		functions->ClearNamedFramebufferfv = (GLFunctions::ptr_glClearNamedFramebufferfv) cl_get_proc_address_extension("glClearNamedFramebufferfv");
		functions->ClearNamedFramebufferfi = (GLFunctions::ptr_glClearNamedFramebufferfi) cl_get_proc_address_extension("glClearNamedFramebufferfi");
		functions->BlitNamedFramebuffer = (GLFunctions::ptr_glBlitNamedFramebuffer) cl_get_proc_address_extension("glBlitNamedFramebuffer");
		functions->CheckNamedFramebufferStatus = (GLFunctions::ptr_glCheckNamedFramebufferStatus) cl_get_proc_address_extension("glCheckNamedFramebufferStatus");
		functions->GetNamedFramebufferParameteriv = (GLFunctions::ptr_glGetNamedFramebufferParameteriv) cl_get_proc_address_extension("glGetNamedFramebufferParameteriv");
		functions->GetNamedFramebufferAttachmentParameteriv = (GLFunctions::ptr_glGetNamedFramebufferAttachmentParameteriv) cl_get_proc_address_extension("glGetNamedFramebufferAttachmentParameteriv");
		functions->CreateRenderbuffers = (GLFunctions::ptr_glCreateRenderbuffers) cl_get_proc_address_extension("glCreateRenderbuffers");
		functions->NamedRenderbufferStorage = (GLFunctions::ptr_glNamedRenderbufferStorage) cl_get_proc_address_extension("glNamedRenderbufferStorage");
		functions->NamedRenderbufferStorageMultisample = (GLFunctions::ptr_glNamedRenderbufferStorageMultisample) cl_get_proc_address_extension("glNamedRenderbufferStorageMultisample");
		functions->GetNamedRenderbufferParameteriv = (GLFunctions::ptr_glGetNamedRenderbufferParameteriv) cl_get_proc_address_extension("glGetNamedRenderbufferParameteriv");
		functions->CreateTextures = (GLFunctions::ptr_glCreateTextures) cl_get_proc_address_extension("glCreateTextures");
		functions->TextureBuffer = (GLFunctions::ptr_glTextureBuffer) cl_get_proc_address_extension("glTextureBuffer");
		functions->TextureBufferRange = (GLFunctions::ptr_glTextureBufferRange) cl_get_proc_address_extension("glTextureBufferRange");
		functions->TextureStorage1D = (GLFunctions::ptr_glTextureStorage1D) cl_get_proc_address_extension("glTextureStorage1D");
		functions->TextureStorage2D = (GLFunctions::ptr_glTextureStorage2D) cl_get_proc_address_extension("glTextureStorage2D");
		functions->TextureStorage3D = (GLFunctions::ptr_glTextureStorage3D) cl_get_proc_address_extension("glTextureStorage3D");
		functions->TextureStorage2DMultisample = (GLFunctions::ptr_glTextureStorage2DMultisample) cl_get_proc_address_extension("glTextureStorage2DMultisample");
		functions->TextureStorage3DMultisample = (GLFunctions::ptr_glTextureStorage3DMultisample) cl_get_proc_address_extension("glTextureStorage3DMultisample");
		functions->TextureSubImage1D = (GLFunctions::ptr_glTextureSubImage1D) cl_get_proc_address_extension("glTextureSubImage1D");
		functions->TextureSubImage2D = (GLFunctions::ptr_glTextureSubImage2D) cl_get_proc_address_extension("glTextureSubImage2D");
		functions->TextureSubImage3D = (GLFunctions::ptr_glTextureSubImage3D) cl_get_proc_address_extension("glTextureSubImage3D");
		functions->CompressedTextureSubImage1D = (GLFunctions::ptr_glCompressedTextureSubImage1D) cl_get_proc_address_extension("glCompressedTextureSubImage1D");
		functions->CompressedTextureSubImage2D = (GLFunctions::ptr_glCompressedTextureSubImage2D) cl_get_proc_address_extension("glCompressedTextureSubImage2D");
		functions->CompressedTextureSubImage3D = (GLFunctions::ptr_glCompressedTextureSubImage3D) cl_get_proc_address_extension("glCompressedTextureSubImage3D");
		functions->CopyTextureSubImage1D = (GLFunctions::ptr_glCopyTextureSubImage1D) cl_get_proc_address_extension("glCopyTextureSubImage1D");
		functions->CopyTextureSubImage2D = (GLFunctions::ptr_glCopyTextureSubImage2D) cl_get_proc_address_extension("glCopyTextureSubImage2D");
		functions->CopyTextureSubImage3D = (GLFunctions::ptr_glCopyTextureSubImage3D) cl_get_proc_address_extension("glCopyTextureSubImage3D");
		functions->TextureParameterf = (GLFunctions::ptr_glTextureParameterf) cl_get_proc_address_extension("glTextureParameterf");
		functions->TextureParameterfv = (GLFunctions::ptr_glTextureParameterfv) cl_get_proc_address_extension("glTextureParameterfv");
		functions->TextureParameteri = (GLFunctions::ptr_glTextureParameteri) cl_get_proc_address_extension("glTextureParameteri");
		functions->TextureParameterIiv = (GLFunctions::ptr_glTextureParameterIiv) cl_get_proc_address_extension("glTextureParameterIiv");
		functions->TextureParameterIuiv = (GLFunctions::ptr_glTextureParameterIuiv) cl_get_proc_address_extension("glTextureParameterIuiv");
		functions->TextureParameteriv = (GLFunctions::ptr_glTextureParameteriv) cl_get_proc_address_extension("glTextureParameteriv");
		functions->GenerateTextureMipmap = (GLFunctions::ptr_glGenerateTextureMipmap) cl_get_proc_address_extension("glGenerateTextureMipmap");
		functions->BindTextureUnit = (GLFunctions::ptr_glBindTextureUnit) cl_get_proc_address_extension("glBindTextureUnit");
		functions->GetTextureImage = (GLFunctions::ptr_glGetTextureImage) cl_get_proc_address_extension("glGetTextureImage");
		functions->GetCompressedTextureImage = (GLFunctions::ptr_glGetCompressedTextureImage) cl_get_proc_address_extension("glGetCompressedTextureImage");
		functions->GetTextureLevelParameterfv = (GLFunctions::ptr_glGetTextureLevelParameterfv) cl_get_proc_address_extension("glGetTextureLevelParameterfv");
		functions->GetTextureLevelParameteriv = (GLFunctions::ptr_glGetTextureLevelParameteriv) cl_get_proc_address_extension("glGetTextureLevelParameteriv");
		functions->GetTextureParameterfv = (GLFunctions::ptr_glGetTextureParameterfv) cl_get_proc_address_extension("glGetTextureParameterfv");
		functions->GetTextureParameterIiv = (GLFunctions::ptr_glGetTextureParameterIiv) cl_get_proc_address_extension("glGetTextureParameterIiv");
		functions->GetTextureParameterIuiv = (GLFunctions::ptr_glGetTextureParameterIuiv) cl_get_proc_address_extension("glGetTextureParameterIuiv");
		functions->GetTextureParameteriv = (GLFunctions::ptr_glGetTextureParameteriv) cl_get_proc_address_extension("glGetTextureParameteriv");
		functions->CreateVertexArrays = (GLFunctions::ptr_glCreateVertexArrays) cl_get_proc_address_extension("glCreateVertexArrays");
		functions->DisableVertexArrayAttrib = (GLFunctions::ptr_glDisableVertexArrayAttrib) cl_get_proc_address_extension("glDisableVertexArrayAttrib");
		functions->EnableVertexArrayAttrib = (GLFunctions::ptr_glEnableVertexArrayAttrib) cl_get_proc_address_extension("glEnableVertexArrayAttrib");
		functions->VertexArrayElementBuffer = (GLFunctions::ptr_glVertexArrayElementBuffer) cl_get_proc_address_extension("glVertexArrayElementBuffer");
		functions->VertexArrayVertexBuffer = (GLFunctions::ptr_glVertexArrayVertexBuffer) cl_get_proc_address_extension("glVertexArrayVertexBuffer");
		functions->VertexArrayVertexBuffers = (GLFunctions::ptr_glVertexArrayVertexBuffers) cl_get_proc_address_extension("glVertexArrayVertexBuffers");
		functions->VertexArrayAttribBinding = (GLFunctions::ptr_glVertexArrayAttribBinding) cl_get_proc_address_extension("glVertexArrayAttribBinding");
		functions->VertexArrayAttribFormat = (GLFunctions::ptr_glVertexArrayAttribFormat) cl_get_proc_address_extension("glVertexArrayAttribFormat");
		functions->VertexArrayAttribIFormat = (GLFunctions::ptr_glVertexArrayAttribIFormat) cl_get_proc_address_extension("glVertexArrayAttribIFormat");
		functions->VertexArrayAttribLFormat = (GLFunctions::ptr_glVertexArrayAttribLFormat) cl_get_proc_address_extension("glVertexArrayAttribLFormat");
		functions->VertexArrayBindingDivisor = (GLFunctions::ptr_glVertexArrayBindingDivisor) cl_get_proc_address_extension("glVertexArrayBindingDivisor");
		functions->GetVertexArrayiv = (GLFunctions::ptr_glGetVertexArrayiv) cl_get_proc_address_extension("glGetVertexArrayiv");
		functions->GetVertexArrayIndexediv = (GLFunctions::ptr_glGetVertexArrayIndexediv) cl_get_proc_address_extension("glGetVertexArrayIndexediv");
		functions->GetVertexArrayIndexed64iv = (GLFunctions::ptr_glGetVertexArrayIndexed64iv) cl_get_proc_address_extension("glGetVertexArrayIndexed64iv");
		functions->CreateSamplers = (GLFunctions::ptr_glCreateSamplers) cl_get_proc_address_extension("glCreateSamplers");
		functions->CreateProgramPipelines = (GLFunctions::ptr_glCreateProgramPipelines) cl_get_proc_address_extension("glCreateProgramPipelines");
		functions->CreateQueries = (GLFunctions::ptr_glCreateQueries) cl_get_proc_address_extension("glCreateQueries");
		functions->GetQueryBufferObjecti64v = (GLFunctions::ptr_glGetQueryBufferObjecti64v) cl_get_proc_address_extension("glGetQueryBufferObjecti64v");
		functions->GetQueryBufferObjectiv = (GLFunctions::ptr_glGetQueryBufferObjectiv) cl_get_proc_address_extension("glGetQueryBufferObjectiv");
		functions->GetQueryBufferObjectui64v = (GLFunctions::ptr_glGetQueryBufferObjectui64v) cl_get_proc_address_extension("glGetQueryBufferObjectui64v");
		functions->GetQueryBufferObjectuiv = (GLFunctions::ptr_glGetQueryBufferObjectuiv) cl_get_proc_address_extension("glGetQueryBufferObjectuiv");
		functions->MemoryBarrierByRegion = (GLFunctions::ptr_glMemoryBarrierByRegion) cl_get_proc_address_extension("glMemoryBarrierByRegion");
		functions->GetTextureSubImage = (GLFunctions::ptr_glGetTextureSubImage) cl_get_proc_address_extension("glGetTextureSubImage");
		functions->GetCompressedTextureSubImage = (GLFunctions::ptr_glGetCompressedTextureSubImage) cl_get_proc_address_extension("glGetCompressedTextureSubImage");
		functions->GetGraphicsResetStatus = (GLFunctions::ptr_glGetGraphicsResetStatus) cl_get_proc_address_extension("glGetGraphicsResetStatus");
		functions->GetnCompressedTexImage = (GLFunctions::ptr_glGetnCompressedTexImage) cl_get_proc_address_extension("glGetnCompressedTexImage");
		functions->GetnTexImage = (GLFunctions::ptr_glGetnTexImage) cl_get_proc_address_extension("glGetnTexImage");
		functions->GetnUniformdv = (GLFunctions::ptr_glGetnUniformdv) cl_get_proc_address_extension("glGetnUniformdv");
		functions->GetnUniformfv = (GLFunctions::ptr_glGetnUniformfv) cl_get_proc_address_extension("glGetnUniformfv");
		functions->GetnUniformiv = (GLFunctions::ptr_glGetnUniformiv) cl_get_proc_address_extension("glGetnUniformiv");
		functions->GetnUniformuiv = (GLFunctions::ptr_glGetnUniformuiv) cl_get_proc_address_extension("glGetnUniformuiv");
		functions->ReadnPixels = (GLFunctions::ptr_glReadnPixels) cl_get_proc_address_extension("glReadnPixels");
		functions->GetnMapdv = (GLFunctions::ptr_glGetnMapdv) cl_get_proc_address_extension("glGetnMapdv");
		functions->GetnMapfv = (GLFunctions::ptr_glGetnMapfv) cl_get_proc_address_extension("glGetnMapfv");
		functions->GetnMapiv = (GLFunctions::ptr_glGetnMapiv) cl_get_proc_address_extension("glGetnMapiv");
		functions->GetnPixelMapfv = (GLFunctions::ptr_glGetnPixelMapfv) cl_get_proc_address_extension("glGetnPixelMapfv");
		functions->GetnPixelMapuiv = (GLFunctions::ptr_glGetnPixelMapuiv) cl_get_proc_address_extension("glGetnPixelMapuiv");
		functions->GetnPixelMapusv = (GLFunctions::ptr_glGetnPixelMapusv) cl_get_proc_address_extension("glGetnPixelMapusv");
		functions->GetnPolygonStipple = (GLFunctions::ptr_glGetnPolygonStipple) cl_get_proc_address_extension("glGetnPolygonStipple");
		functions->GetnColorTable = (GLFunctions::ptr_glGetnColorTable) cl_get_proc_address_extension("glGetnColorTable");
		functions->GetnConvolutionFilter = (GLFunctions::ptr_glGetnConvolutionFilter) cl_get_proc_address_extension("glGetnConvolutionFilter");
		functions->GetnSeparableFilter = (GLFunctions::ptr_glGetnSeparableFilter) cl_get_proc_address_extension("glGetnSeparableFilter");
		functions->GetnHistogram = (GLFunctions::ptr_glGetnHistogram) cl_get_proc_address_extension("glGetnHistogram");
		functions->GetnMinmax = (GLFunctions::ptr_glGetnMinmax) cl_get_proc_address_extension("glGetnMinmax");
		functions->TextureBarrier = (GLFunctions::ptr_glTextureBarrier) cl_get_proc_address_extension("glTextureBarrier");

	#ifdef WIN32	// Only win32 static links opengl

		// We require the real address of static links, so undef these...
		#undef glBindTexture
		#undef glBlendFunc
		#undef glClear
		#undef glClearColor
		#undef glClearDepth
		#undef glClearStencil
		#undef glColorMask
		#undef glCopyTexImage1D
		#undef glCopyTexImage2D
		#undef glCopyTexSubImage1D
		#undef glCopyTexSubImage2D
		#undef glCullFace
		#undef glDeleteTextures
		#undef glDepthFunc
		#undef glDepthMask
		#undef glDepthRange
		#undef glDisable
		#undef glDrawArrays
		#undef glDrawBuffer
		#undef glDrawElements
		#undef glEnable
		#undef glFinish
		#undef glFlush
		#undef glFrontFace
		#undef glGenTextures
		#undef glGetBooleanv
		#undef glGetDoublev
		#undef glGetError
		#undef glGetFloatv
		#undef glGetIntegerv
		#undef glGetPointerv
		#undef glGetString
		#undef glGetTexImage
		#undef glGetTexLevelParameterfv
		#undef glGetTexLevelParameteriv
		#undef glGetTexParameterfv
		#undef glGetTexParameteriv
		#undef glHint
		#undef glIsEnabled
		#undef glIsTexture
		#undef glLineWidth
		#undef glLogicOp
		#undef glPixelStoref
		#undef glPixelStorei
		#undef glPointSize
		#undef glPolygonMode
		#undef glPolygonOffset
		#undef glReadBuffer
		#undef glReadPixels
		#undef glScissor
		#undef glStencilFunc
		#undef glStencilMask
		#undef glStencilOp
		#undef glTexImage1D
		#undef glTexImage2D
		#undef glTexParameterf
		#undef glTexParameterfv
		#undef glTexParameteri
		#undef glTexParameteriv
		#undef glTexSubImage1D
		#undef glTexSubImage2D
		#undef glViewport
	
		if (!functions->bindTexture) functions->bindTexture = (GLFunctions::ptr_glBindTexture) &glBindTexture;
		if (!functions->blendFunc) functions->blendFunc = (GLFunctions::ptr_glBlendFunc) &glBlendFunc;
		if (!functions->clear) functions->clear = (GLFunctions::ptr_glClear) &glClear;
		if (!functions->clearColor) functions->clearColor = (GLFunctions::ptr_glClearColor) &glClearColor;
		if (!functions->clearDepth) functions->clearDepth = (GLFunctions::ptr_glClearDepth) &glClearDepth;
		if (!functions->clearStencil) functions->clearStencil = (GLFunctions::ptr_glClearStencil) &glClearStencil;
		if (!functions->colorMask) functions->colorMask = (GLFunctions::ptr_glColorMask) &glColorMask;
		if (!functions->copyTexImage1D) functions->copyTexImage1D = (GLFunctions::ptr_glCopyTexImage1D) &glCopyTexImage1D;
		if (!functions->copyTexImage2D) functions->copyTexImage2D = (GLFunctions::ptr_glCopyTexImage2D) &glCopyTexImage2D;
		if (!functions->copyTexSubImage1D) functions->copyTexSubImage1D = (GLFunctions::ptr_glCopyTexSubImage1D) &glCopyTexSubImage1D;
		if (!functions->copyTexSubImage2D) functions->copyTexSubImage2D = (GLFunctions::ptr_glCopyTexSubImage2D) &glCopyTexSubImage2D;
		if (!functions->cullFace) functions->cullFace = (GLFunctions::ptr_glCullFace) &glCullFace;
		if (!functions->deleteTextures) functions->deleteTextures = (GLFunctions::ptr_glDeleteTextures) &glDeleteTextures;
		if (!functions->depthFunc) functions->depthFunc = (GLFunctions::ptr_glDepthFunc) &glDepthFunc;
		if (!functions->depthMask) functions->depthMask = (GLFunctions::ptr_glDepthMask) &glDepthMask;
		if (!functions->depthRange) functions->depthRange = (GLFunctions::ptr_glDepthRange) &glDepthRange;
		if (!functions->disable) functions->disable = (GLFunctions::ptr_glDisable) &glDisable;
		if (!functions->drawArrays) functions->drawArrays = (GLFunctions::ptr_glDrawArrays) &glDrawArrays;
		if (!functions->drawBuffer) functions->drawBuffer = (GLFunctions::ptr_glDrawBuffer) &glDrawBuffer;
		if (!functions->drawElements) functions->drawElements = (GLFunctions::ptr_glDrawElements) &glDrawElements;
		if (!functions->enable) functions->enable = (GLFunctions::ptr_glEnable) &glEnable;
		if (!functions->finish) functions->finish = (GLFunctions::ptr_glFinish) &glFinish;
		if (!functions->flush) functions->flush = (GLFunctions::ptr_glFlush) &glFlush;
		if (!functions->frontFace) functions->frontFace = (GLFunctions::ptr_glFrontFace) &glFrontFace;
		if (!functions->genTextures) functions->genTextures = (GLFunctions::ptr_glGenTextures) &glGenTextures;
		if (!functions->getBooleanv) functions->getBooleanv = (GLFunctions::ptr_glGetBooleanv) &glGetBooleanv;
		if (!functions->getDoublev) functions->getDoublev = (GLFunctions::ptr_glGetDoublev) &glGetDoublev;
		if (!functions->getError) functions->getError = (GLFunctions::ptr_glGetError) &glGetError;
		if (!functions->getFloatv) functions->getFloatv = (GLFunctions::ptr_glGetFloatv) &glGetFloatv;
		if (!functions->getIntegerv) functions->getIntegerv = (GLFunctions::ptr_glGetIntegerv) &glGetIntegerv;
		if (!functions->getPointerv) functions->getPointerv = (GLFunctions::ptr_glGetPointerv) &glGetPointerv;
		if (!functions->getString) functions->getString = (GLFunctions::ptr_glGetString) &glGetString;
		if (!functions->getTexImage) functions->getTexImage = (GLFunctions::ptr_glGetTexImage) &glGetTexImage;
		if (!functions->getTexLevelParameterfv) functions->getTexLevelParameterfv = (GLFunctions::ptr_glGetTexLevelParameterfv) &glGetTexLevelParameterfv;
		if (!functions->getTexLevelParameteriv) functions->getTexLevelParameteriv = (GLFunctions::ptr_glGetTexLevelParameteriv) &glGetTexLevelParameteriv;
		if (!functions->getTexParameterfv) functions->getTexParameterfv = (GLFunctions::ptr_glGetTexParameterfv) &glGetTexParameterfv;
		if (!functions->getTexParameteriv) functions->getTexParameteriv = (GLFunctions::ptr_glGetTexParameteriv) &glGetTexParameteriv;
		if (!functions->hint) functions->hint = (GLFunctions::ptr_glHint) &glHint;
		if (!functions->isEnabled) functions->isEnabled = (GLFunctions::ptr_glIsEnabled) &glIsEnabled;
		if (!functions->isTexture) functions->isTexture = (GLFunctions::ptr_glIsTexture) &glIsTexture;
		if (!functions->lineWidth) functions->lineWidth = (GLFunctions::ptr_glLineWidth) &glLineWidth;
		if (!functions->logicOp) functions->logicOp = (GLFunctions::ptr_glLogicOp) &glLogicOp;
		if (!functions->pixelStoref) functions->pixelStoref = (GLFunctions::ptr_glPixelStoref) &glPixelStoref;
		if (!functions->pixelStorei) functions->pixelStorei = (GLFunctions::ptr_glPixelStorei) &glPixelStorei;
		if (!functions->pointSize) functions->pointSize = (GLFunctions::ptr_glPointSize) &glPointSize;
		if (!functions->polygonMode) functions->polygonMode = (GLFunctions::ptr_glPolygonMode) &glPolygonMode;
		if (!functions->polygonOffset) functions->polygonOffset = (GLFunctions::ptr_glPolygonOffset) &glPolygonOffset;
		if (!functions->readBuffer) functions->readBuffer = (GLFunctions::ptr_glReadBuffer) &glReadBuffer;
		if (!functions->readPixels) functions->readPixels = (GLFunctions::ptr_glReadPixels) &glReadPixels;
		if (!functions->scissor) functions->scissor = (GLFunctions::ptr_glScissor) &glScissor;
		if (!functions->stencilFunc) functions->stencilFunc = (GLFunctions::ptr_glStencilFunc) &glStencilFunc;
		if (!functions->stencilMask) functions->stencilMask = (GLFunctions::ptr_glStencilMask) &glStencilMask;
		if (!functions->stencilOp) functions->stencilOp = (GLFunctions::ptr_glStencilOp) &glStencilOp;
		if (!functions->texImage1D) functions->texImage1D = (GLFunctions::ptr_glTexImage1D) &glTexImage1D;
		if (!functions->texImage2D) functions->texImage2D = (GLFunctions::ptr_glTexImage2D) &glTexImage2D;
		if (!functions->texParameterf) functions->texParameterf = (GLFunctions::ptr_glTexParameterf) &glTexParameterf;
		if (!functions->texParameterfv) functions->texParameterfv = (GLFunctions::ptr_glTexParameterfv) &glTexParameterfv;
		if (!functions->texParameteri) functions->texParameteri = (GLFunctions::ptr_glTexParameteri) &glTexParameteri;
		if (!functions->texParameteriv) functions->texParameteriv = (GLFunctions::ptr_glTexParameteriv) &glTexParameteriv;
		if (!functions->texSubImage1D) functions->texSubImage1D = (GLFunctions::ptr_glTexSubImage1D) &glTexSubImage1D;
		if (!functions->texSubImage2D) functions->texSubImage2D = (GLFunctions::ptr_glTexSubImage2D) &glTexSubImage2D;
		if (!functions->viewport) functions->viewport = (GLFunctions::ptr_glViewport) &glViewport;

	#endif

	#ifdef __APPLE__
		// We require the real address of static links, so undef these...

		#undef glBlendColor
		#undef glBlendEquation
		#undef glActiveTexture
		#undef glCompressedTexImage2D
		#undef glCompressedTexSubImage2D
		#undef glSampleCoverage
		#undef glBlendFuncSeparate

		if (!functions->blendColor) functions->blendColor = (GLFunctions::ptr_glBlendColor) &glBlendColor;
		if (!functions->blendEquation) functions->blendEquation = (GLFunctions::ptr_glBlendEquation) &glBlendEquation;
		if (!functions->activeTexture) functions->activeTexture = (GLFunctions::ptr_glActiveTexture) &glActiveTexture;
		if (!functions->compressedTexImage2D) functions->compressedTexImage2D = (GLFunctions::ptr_glCompressedTexImage2D) &glCompressedTexImage2D;
		if (!functions->compressedTexSubImage2D) functions->compressedTexSubImage2D = (GLFunctions::ptr_glCompressedTexSubImage2D) &glCompressedTexSubImage2D;
		if (!functions->sampleCoverage) functions->sampleCoverage = (GLFunctions::ptr_glSampleCoverage) &glSampleCoverage;
		if (!functions->blendFuncSeparate) functions->blendFuncSeparate = (GLFunctions::ptr_glBlendFuncSeparate) &glBlendFuncSeparate;

		// The following functions exist in the bundle but they are not implemented by Apple:
    
		functions->pointParameterf = 0;
		functions->pointParameterfv = 0;
		functions->pointSize = 0;
    
	#endif


		// For Legacy OpenGL (For GL1 target)
		functions->clientActiveTexture = (GLFunctions::ptr_glClientActiveTextureARB) cl_get_proc_address_extension("glClientActiveTextureARB");
	#ifdef WIN32
		functions->wglCreatePbufferARB = (GLFunctions::ptr_wglCreatePbufferARB) cl_get_proc_address_extension("wglCreatePbufferARB");
		functions->wglGetPbufferDCARB = (GLFunctions::ptr_wglGetPbufferDCARB) cl_get_proc_address_extension("wglGetPbufferDCARB");
		functions->wglReleasePbufferDCARB = (GLFunctions::ptr_wglReleasePbufferDCARB) cl_get_proc_address_extension("wglReleasePbufferDCARB");
		functions->wglDestroyPbufferARB = (GLFunctions::ptr_wglDestroyPbufferARB) cl_get_proc_address_extension("wglDestroyPbufferARB");
		functions->wglQueryPbufferARB = (GLFunctions::ptr_wglQueryPbufferARB) cl_get_proc_address_extension("wglQueryPbufferARB");
	#endif
		return functions;
	}

	GLenum OpenGL::to_enum(DrawBuffer buffer)
	{
		switch(buffer)
		{
		case buffer_back: return GL_BACK;
		case buffer_back_left: return GL_BACK_LEFT;
		case buffer_back_right: return GL_BACK_RIGHT;
		case buffer_front: return GL_FRONT;
		case buffer_front_and_back: return GL_FRONT_AND_BACK;
		case buffer_front_left: return GL_FRONT_LEFT;
		case buffer_front_right: return GL_FRONT_RIGHT;
		case buffer_left: return GL_LEFT;
		case buffer_none: return GL_NONE;
		case buffer_right: return GL_RIGHT;
		default: return GL_BACK;
		}
	}

	GLenum OpenGL::to_enum(CompareFunction func)
	{
		switch( func )
		{
		case compare_never: return GL_NEVER;
		case compare_less: return GL_LESS;
		case compare_lequal: return GL_LEQUAL; 
		case compare_greater: return GL_GREATER; 
		case compare_gequal: return GL_GEQUAL; 
		case compare_equal: return GL_EQUAL; 
		case compare_notequal: return GL_NOTEQUAL; 
		case compare_always: return GL_ALWAYS; 
		default: return GL_LEQUAL;
		}
	}

	GLenum OpenGL::to_enum(StencilOp op)
	{
		switch( op )
		{
		case stencil_decr: return GL_DECR;
		case stencil_decr_wrap: return GL_DECR_WRAP;
		case stencil_incr: return GL_INCR;
		case stencil_incr_wrap: return GL_INCR_WRAP;
		case stencil_invert: return GL_INVERT;
		case stencil_keep: return GL_KEEP;
		case stencil_replace: return GL_REPLACE;
		case stencil_zero: return GL_ZERO;	
		default: return GL_KEEP;
		}
	}

	GLenum OpenGL::to_enum(CullMode mode)
	{
		switch( mode )
		{
		case cull_front: return GL_FRONT;
		case cull_back: return GL_BACK;
		case cull_front_and_back: return GL_FRONT_AND_BACK;
		default: return GL_BACK;
		}
	}

	GLenum OpenGL::to_enum(FillMode mode)
	{
		switch( mode )
		{
		case fill_point: return GL_POINT;
		case fill_line: return GL_LINE;
		case fill_polygon: return GL_FILL;
		default: return GL_FILL;
		}
	}

	GLenum OpenGL::to_enum(BlendFunc func)
	{
		switch( func )
		{
		case blend_zero: return GL_ZERO;
		case blend_one: return GL_ONE;
		case blend_dest_color: return GL_DST_COLOR;
		case blend_src_color: return GL_SRC_COLOR;
		case blend_one_minus_dest_color: return GL_ONE_MINUS_DST_COLOR;
		case blend_one_minus_src_color: return GL_ONE_MINUS_SRC_COLOR;
		case blend_src_alpha: return GL_SRC_ALPHA;
		case blend_one_minus_src_alpha: return GL_ONE_MINUS_SRC_ALPHA;
		case blend_dest_alpha: return GL_DST_ALPHA;
		case blend_one_minus_dest_alpha: return GL_ONE_MINUS_DST_ALPHA;
		case blend_src_alpha_saturate: return GL_SRC_ALPHA_SATURATE;
		case blend_constant_color: return GL_CONSTANT_COLOR;
		case blend_one_minus_constant_color: return GL_ONE_MINUS_CONSTANT_COLOR;
		case blend_constant_alpha: return GL_CONSTANT_ALPHA;
		case blend_one_minus_constant_alpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
		default: return GL_BLEND_SRC;
		}
	}

	GLenum OpenGL::to_enum(BlendEquation eq)
	{
		switch( eq )
		{
		case equation_add: return GL_FUNC_ADD;
		case equation_subtract: return GL_FUNC_SUBTRACT;
		case equation_reverse_subtract: return GL_FUNC_REVERSE_SUBTRACT;
		case equation_min: return GL_MIN;
		case equation_max: return GL_MAX;
		default: return GL_FUNC_ADD;
		}
	}

	GLenum OpenGL::to_enum(enum VertexAttributeDataType value)
	{
		switch(value)
		{
		case type_unsigned_byte:
			return GL_UNSIGNED_BYTE;
		case type_unsigned_short:
			return GL_UNSIGNED_SHORT;
		case type_unsigned_int:
			return GL_UNSIGNED_INT;
		case type_byte:
			return GL_BYTE;
		case type_short:
			return GL_SHORT;
		case type_int:
			return GL_INT;
		case type_float:
			return GL_FLOAT;
		default:
			return 0;
		}
	}

	GLenum OpenGL::to_enum(enum PrimitivesType value)
	{
		GLenum gl_mode = 0;
		switch (value)
		{
		case type_points: gl_mode = GL_POINTS; break;
		case type_line_strip: gl_mode = GL_LINE_STRIP; break;
		case type_line_loop: gl_mode = GL_LINE_LOOP; break;
		case type_lines: gl_mode = GL_LINES; break;
		case type_triangle_strip: gl_mode = GL_TRIANGLE_STRIP; break;
		case type_triangle_fan: gl_mode = GL_TRIANGLE_FAN; break;
		case type_triangles: gl_mode = GL_TRIANGLES; break;
		}
		return gl_mode;
	}

	GLenum OpenGL::to_enum(enum LogicOp op)
	{
		GLenum gl_op = 0;
		switch (op)
		{
			case logic_clear: gl_op = GL_CLEAR; break;  
			case logic_and: gl_op = GL_AND; break;
			case logic_and_reverse: gl_op = GL_AND_REVERSE; break;
			case logic_copy: gl_op = GL_COPY; break;
			case logic_and_inverted: gl_op = GL_AND_INVERTED; break;
			case logic_noop: gl_op = GL_NOOP; break;
			case logic_xor: gl_op = GL_XOR; break;
			case logic_or: gl_op = GL_OR; break;
			case logic_nor: gl_op = GL_NOR; break;
			case logic_equiv: gl_op = GL_EQUIV; break;
			case logic_invert: gl_op = GL_INVERT; break;
			case logic_or_reverse: gl_op = GL_OR_REVERSE; break;
			case logic_copy_inverted: gl_op = GL_COPY_INVERTED; break;
			case logic_or_inverted: gl_op = GL_OR_INVERTED; break;
			case logic_nand: gl_op = GL_NAND; break;
			case logic_set: gl_op = GL_SET; break;
			default: break;
		}
		return gl_op;
	}

	GLenum OpenGL::to_enum(TextureFilter filter)
	{
		switch(filter)
		{
		case filter_nearest: return GL_NEAREST;
		case filter_linear: return GL_LINEAR;
		case filter_nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
		case filter_nearest_mipmap_linear: return GL_NEAREST_MIPMAP_LINEAR;
		case filter_linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
		case filter_linear_mipmap_linear: return GL_LINEAR_MIPMAP_LINEAR;
		default: return GL_NEAREST;
		}
	}

	GLenum OpenGL::to_enum(TextureWrapMode mode)
	{
 		switch(mode)
		{
		case wrap_clamp_to_edge: return GL_CLAMP_TO_EDGE;
		case wrap_repeat: return GL_REPEAT;
		case wrap_mirrored_repeat: return GL_MIRRORED_REPEAT;
		default: return GL_CLAMP_TO_EDGE;
		}
	}

	GLenum OpenGL::to_enum(TextureCompareMode mode)
	{
 		switch(mode)
		{
		case comparemode_none: return GL_NONE;
		case comparemode_compare_r_to_texture: return GL_COMPARE_REF_TO_TEXTURE;		
		default: return GL_NONE;
		}
	}

	GLenum OpenGL::to_cube_target(int index)
	{
		// To do: make sure this order matches the order used by Direct3D
		switch (index)
		{
		case 0: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case 1: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case 2: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case 3: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case 4: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case 5: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		default: throw Exception("Invalid index specified for cube texture");
		}
	}

	GLenum OpenGL::to_enum(BufferUsage usage)
	{
		switch (usage)
		{
		case usage_stream_draw:
			return GL_STREAM_DRAW;
		case usage_stream_read:
			return GL_STREAM_READ;
		case usage_stream_copy:
			return GL_STREAM_COPY;
		case usage_static_draw:
			return GL_STATIC_DRAW;
		case usage_static_read:
			return GL_STATIC_READ;
		case usage_static_copy:
			return GL_STATIC_COPY;
		case usage_dynamic_draw:
			return GL_DYNAMIC_DRAW;
		case usage_dynamic_read:
			return GL_DYNAMIC_READ;
		case usage_dynamic_copy:
			return GL_DYNAMIC_COPY;
		default:
			return GL_STATIC_DRAW;
		}
	}

	GLenum OpenGL::to_enum(BufferAccess access)
	{
		switch (access)
		{
		case access_read_only:
			return GL_READ_ONLY;
		case access_write_only:
		case access_write_discard:
			return GL_WRITE_ONLY;
		case access_read_write:
			return GL_READ_WRITE;
		default:
			return GL_READ_WRITE;
		}
	}


	GLuint OpenGL::get_texture_handle(Texture &texture)
	{
		return static_cast<GL3TextureProvider*>(texture.get_provider())->get_handle();
	}

	Texture OpenGL::from_texture_handle(GLuint type, GLuint handle)
	{
		//FIXME For GL1
		return Texture(new GL3TextureProvider(type, handle));
	}
}

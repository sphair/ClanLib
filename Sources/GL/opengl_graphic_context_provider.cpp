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
**    Harry Storbacka
**    Ken Hirsch
**    Kenneth Gangstoe
**    Mark Page
**    Kevin J Bluck
*/

#include "GL/precomp.h"
#include "opengl_graphic_context_provider.h"
#include "opengl_occlusion_query_provider.h"
#include "opengl_texture_provider.h"
#include "opengl_program_object_provider.h"
#include "opengl_shader_object_provider.h"
#include "opengl_frame_buffer_provider.h"
#include "opengl_render_buffer_provider.h"
#include "opengl_vertex_array_buffer_provider.h"
#include "opengl_uniform_buffer_provider.h"
#include "opengl_storage_buffer_provider.h"
#include "opengl_element_array_buffer_provider.h"
#include "opengl_transfer_buffer_provider.h"
#include "opengl_pixel_buffer_provider.h"
#include "opengl_primitives_array_provider.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/System/mutex.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/2D/image.h"
#include "API/GL/opengl_window_description.h"
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include "AGL/opengl_window_provider_agl.h"
#elif !defined(WIN32)
#include "GLX/opengl_window_provider_glx.h"
#endif
#include <memory>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// OpenGLGraphicContextProvider Construction:

OpenGLGraphicContextProvider::OpenGLGraphicContextProvider(const DisplayWindowProvider * const render_window, const OpenGLWindowDescription &gldesc)
: render_window(render_window), framebuffer_bound(false), opengl_version_major(0), shader_version_major(0), scissor_enabled(false)
{
	check_opengl_version();

	int glsl_version_major;
	int glsl_version_minor;
	get_opengl_shading_language_version(glsl_version_major, glsl_version_minor);

	// Must write here, although OpenGL::SetActive() updates it, because the version number is not available on the initial OpenGL::SetActive() call
	OpenGL::opengl_version_major = opengl_version_major;
	OpenGL::opengl_version_minor = opengl_version_minor;
	OpenGL::glsl_version_major = shader_version_major;
	OpenGL::glsl_version_minor = shader_version_minor;

	create_standard_programs();

	reset_program_object();

	SharedGCData::add_provider(this);
}

OpenGLGraphicContextProvider::~OpenGLGraphicContextProvider()
{
	dispose();
}

void OpenGLGraphicContextProvider::create_standard_programs()
{
	// Find an existing provider
	std::unique_ptr<MutexSection> mutex_section;
	std::vector<GraphicContextProvider*> &gc_providers = SharedGCData::get_gc_providers(mutex_section);

	unsigned int max = gc_providers.size();
	for(unsigned int cnt=0; cnt<max; cnt++)
	{
		OpenGLGraphicContextProvider* gc_provider = dynamic_cast<OpenGLGraphicContextProvider *>(gc_providers[cnt]);
		if (gc_provider != this)
		{
			standard_programs = gc_provider->standard_programs;
			return;
		}
	}

	standard_programs = OpenGLStandardPrograms(this);

}
void OpenGLGraphicContextProvider::on_dispose()
{
	while (!disposable_objects.empty())
		disposable_objects.front()->dispose();

	standard_programs = OpenGLStandardPrograms();

	SharedGCData::remove_provider(this);
	OpenGL::remove_active(this);

}


void OpenGLGraphicContextProvider::add_disposable(DisposableObject *disposable)
{
	disposable_objects.push_back(disposable);
}

void OpenGLGraphicContextProvider::remove_disposable(DisposableObject *disposable)
{
	for (size_t i = 0; i < disposable_objects.size(); i++)
	{
		if (disposable_objects[i] == disposable)
		{
			disposable_objects.erase(disposable_objects.begin() + i);
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLGraphicContextProvider Attributes:

void OpenGLGraphicContextProvider::check_opengl_version()
{
	int version_major = 0;
	int version_minor = 0;

	get_opengl_version(version_major, version_minor);
	if ((version_major < 3) || ((version_major == 3) && (version_minor < 2)))
		throw Exception(string_format("This application requires OpenGL 3.2 or above. Your hardware only supports OpenGL %1.%2. Try updating your drivers, or upgrade to a newer graphics card.", version_major, version_minor));
}

void OpenGLGraphicContextProvider::get_opengl_version(int &version_major, int &version_minor)
{
	if (!opengl_version_major)	// Is not cached
	{

	/*	The GL_VERSION string begins with a version number. The version number uses one of these forms: 
		major_number.minor_number 
		major_number.minor_number.release_number 
		Vendor-specific information may follow the version number. Its format depends on the implementation, but a space always separates the version number and the vendor-specific information. 
		All strings are null-terminated. 
		If an error is generated, glGetString returns zero.
	*/
		OpenGL::set_active(this);

		#if defined(__APPLE__)
			opengl_version_major = 0;
			opengl_version_minor = 0;
			glGetIntegerv(GL_MAJOR_VERSION, &opengl_version_major);
            glGetIntegerv(GL_MINOR_VERSION, &opengl_version_minor);
		#else

			std::string version = (char*)glGetString(GL_VERSION);

			opengl_version_major = 0;
			opengl_version_minor = 0;
    
			std::vector<std::string> split_version = StringHelp::split_text(version, ".");
			if(split_version.size() > 0)
				opengl_version_major = StringHelp::text_to_int(split_version[0]);
			if(split_version.size() > 1)
				opengl_version_minor = StringHelp::text_to_int(split_version[1]);
		#endif
	}

	version_major = opengl_version_major;
	version_minor = opengl_version_minor;
}

void OpenGLGraphicContextProvider::get_opengl_shading_language_version(int &version_major, int &version_minor)
{
	if (!shader_version_major)	// Is not cached
	{
		// See http://www.opengl.org/wiki/Detecting_the_Shader_Model
		shader_version_major = 0;
		shader_version_minor = 0;

		if ( (opengl_version_major < 2) || ( (opengl_version_major == 2) && (opengl_version_minor < 1) ) )
		{
			OpenGL::set_active(this);
			
			std::string version = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

			std::vector<std::string> split_version = StringHelp::split_text(version, ".");
			if(split_version.size() > 0)
				shader_version_major = StringHelp::text_to_int(split_version[0]);
			if(split_version.size() > 1)
				shader_version_minor = StringHelp::text_to_int(split_version[1]);
		}
		else
		{
			if (opengl_version_major == 2)
			{
				shader_version_major = 1;
				shader_version_minor = 20;
			}
			else if ( (opengl_version_major == 3) && (opengl_version_minor == 0) )
			{
				shader_version_major = 1;
				shader_version_minor = 30;
			}
			else if ( (opengl_version_major == 3) && (opengl_version_minor == 1) )
			{
				shader_version_major = 1;
				shader_version_minor = 40;
			}
			else if ( (opengl_version_major == 3) && (opengl_version_minor == 2) )
			{
				shader_version_major = 1;
				shader_version_minor = 50;
			}
			else
			{
				shader_version_major = opengl_version_major;
				shader_version_minor = opengl_version_minor * 10;
			}
		}
	}
	version_major = shader_version_major;
	version_minor = shader_version_minor;
}

std::string OpenGLGraphicContextProvider::get_renderer_string()
{
	OpenGL::set_active(this);
	std::string renderer = (char*)glGetString(GL_RENDERER);
	return renderer;
}

std::string OpenGLGraphicContextProvider::get_vendor_string()
{
	OpenGL::set_active(this);
	std::string vendor = (char*)glGetString(GL_VENDOR);
	return vendor;
}

std::vector<std::string> OpenGLGraphicContextProvider::get_extensions()
{
	OpenGL::set_active(this);
	std::string extension_string = (char*)glGetString(GL_EXTENSIONS);
	std::vector<std::string> tmp = StringHelp::split_text(extension_string, " ");
	std::vector<std::string> extensions;
	for (std::vector<std::string>::size_type i=0; i<tmp.size(); i++)
		extensions.push_back(tmp[i]);
	return extensions;
}

int OpenGLGraphicContextProvider::get_max_attributes()
{
	OpenGL::set_active(this);
	GLint max_attributes = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
    if(max_attributes < 16)
        max_attributes = 16;
	return max_attributes;
}

Size OpenGLGraphicContextProvider::get_max_texture_size() const
{
	OpenGL::set_active(this);
	GLint max_size = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
	return Size(max_size, max_size);
}

Size OpenGLGraphicContextProvider::get_display_window_size() const
{
	return render_window->get_viewport().get_size();
}

#ifdef __APPLE__
static CFBundleRef cl_gBundleRefOpenGL = 0;
#endif

ProcAddress *OpenGLGraphicContextProvider::get_proc_address(const std::string& function_name) const
{

#ifdef WIN32
	return (void (*)())wglGetProcAddress(function_name.c_str());
#else
#ifdef __APPLE__
	// Mac OS X doesn't have an OpenGL extension fetch function. Isn't that silly?
	if (cl_gBundleRefOpenGL == 0)
	{
		cl_gBundleRefOpenGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
		if (cl_gBundleRefOpenGL == 0)
        {
            cl_gBundleRefOpenGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengles"));
            if (cl_gBundleRefOpenGL == 0)
                throw Exception("Unable to find opengl bundle");
        }
    }

	return (ProcAddress *) CFBundleGetFunctionPointerForName(
		cl_gBundleRefOpenGL,
		CFStringCreateWithCStringNoCopy(
			0,
			function_name.c_str(),
			CFStringGetSystemEncoding(),
			0));
#else

	const OpenGLWindowProvider_GLX *wptr = dynamic_cast<const OpenGLWindowProvider_GLX *> (render_window);
	if (wptr == NULL)
		return NULL;

	return wptr->get_proc_address(function_name);
#endif
#endif

}

ProgramObject OpenGLGraphicContextProvider::get_program_object(StandardProgram standard_program) const
{
	return standard_programs.get_program_object(standard_program);
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLGraphicContextProvider Operations:

OcclusionQueryProvider *OpenGLGraphicContextProvider::alloc_occlusion_query()
{
	return new OpenGLOcclusionQueryProvider(this);
}

ProgramObjectProvider *OpenGLGraphicContextProvider::alloc_program_object()
{
	return new OpenGLProgramObjectProvider();
}

ShaderObjectProvider *OpenGLGraphicContextProvider::alloc_shader_object()
{
	return new OpenGLShaderObjectProvider();
}

TextureProvider *OpenGLGraphicContextProvider::alloc_texture(TextureDimensions texture_dimensions)
{
	return new OpenGLTextureProvider(texture_dimensions);
}

FrameBufferProvider *OpenGLGraphicContextProvider::alloc_frame_buffer()
{
	return new OpenGLFrameBufferProvider(this);
}

RenderBufferProvider *OpenGLGraphicContextProvider::alloc_render_buffer()
{
	return new OpenGLRenderBufferProvider();
}

VertexArrayBufferProvider *OpenGLGraphicContextProvider::alloc_vertex_array_buffer()
{
	return new OpenGLVertexArrayBufferProvider();
}

UniformBufferProvider *OpenGLGraphicContextProvider::alloc_uniform_buffer()
{
	return new OpenGLUniformBufferProvider();
}

StorageBufferProvider *OpenGLGraphicContextProvider::alloc_storage_buffer()
{
	return new OpenGLStorageBufferProvider();
}

ElementArrayBufferProvider *OpenGLGraphicContextProvider::alloc_element_array_buffer()
{
	return new OpenGLElementArrayBufferProvider();
}

TransferBufferProvider *OpenGLGraphicContextProvider::alloc_transfer_buffer()
{
	return new OpenGLTransferBufferProvider();
}

PixelBufferProvider *OpenGLGraphicContextProvider::alloc_pixel_buffer()
{
	return new OpenGLPixelBufferProvider();
}

PrimitivesArrayProvider *OpenGLGraphicContextProvider::alloc_primitives_array()
{
	return new OpenGLPrimitivesArrayProvider(this);
}

std::shared_ptr<RasterizerStateProvider> OpenGLGraphicContextProvider::create_rasterizer_state(const RasterizerStateDescription &desc)
{
	std::map<RasterizerStateDescription, std::shared_ptr<RasterizerStateProvider> >::iterator it = rasterizer_states.find(desc);
	if (it != rasterizer_states.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<RasterizerStateProvider> state(new OpenGLRasterizerStateProvider(desc));
		rasterizer_states[desc.clone()] = state;
		return state;
	}
}

std::shared_ptr<BlendStateProvider> OpenGLGraphicContextProvider::create_blend_state(const BlendStateDescription &desc)
{
	std::map<BlendStateDescription, std::shared_ptr<BlendStateProvider> >::iterator it = blend_states.find(desc);
	if (it != blend_states.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<BlendStateProvider> state(new OpenGLBlendStateProvider(desc));
		blend_states[desc.clone()] = state;
		return state;
	}
}

std::shared_ptr<DepthStencilStateProvider> OpenGLGraphicContextProvider::create_depth_stencil_state(const DepthStencilStateDescription &desc)
{
	std::map<DepthStencilStateDescription, std::shared_ptr<DepthStencilStateProvider> >::iterator it = depth_stencil_states.find(desc);
	if (it != depth_stencil_states.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<DepthStencilStateProvider> state(new OpenGLDepthStencilStateProvider(desc));
		depth_stencil_states[desc.clone()] = state;
		return state;
	}
}

void OpenGLGraphicContextProvider::set_rasterizer_state(RasterizerStateProvider *state)
{
	if (state)
	{
		OpenGLRasterizerStateProvider *gl_state = static_cast<OpenGLRasterizerStateProvider*>(state);

		set_culled(gl_state->desc.get_culled());
		enable_line_antialiasing(gl_state->desc.get_enable_line_antialiasing());
		set_face_cull_mode(gl_state->desc.get_face_cull_mode());
		set_face_fill_mode(gl_state->desc.get_face_fill_mode());
		set_front_face(gl_state->desc.get_front_face());
		scissor_enabled = gl_state->desc.get_enable_scissor();
	}
}

void OpenGLGraphicContextProvider::set_blend_state(BlendStateProvider *state, const Vec4f &blend_color, unsigned int sample_mask)
{
	if (state)
	{
		OpenGLBlendStateProvider *gl_state = static_cast<OpenGLBlendStateProvider*>(state);

		bool red, green, blue, alpha;
		BlendEquation equation_color, equation_alpha;
		BlendFunc src, dest, src_alpha, dest_alpha;
		gl_state->desc.get_color_write(red, green, blue, alpha);
		gl_state->desc.get_blend_equation(equation_color, equation_alpha);
		gl_state->desc.get_blend_function(src, dest, src_alpha, dest_alpha);

		enable_color_write(red, green, blue, alpha);
		enable_blending(gl_state->desc.is_blending_enabled());
		set_blend_color(Colorf(blend_color));
		set_blend_equation(equation_color, equation_alpha);
		set_blend_function(src, dest, src_alpha, dest_alpha);
	}
}

void OpenGLGraphicContextProvider::set_depth_stencil_state(DepthStencilStateProvider *state, int stencil_ref)
{
	if (state)
	{
		OpenGLDepthStencilStateProvider *gl_state = static_cast<OpenGLDepthStencilStateProvider*>(state);

		CompareFunction front; int front_ref; int front_mask;
		CompareFunction back; int back_ref; int back_mask;
		unsigned char front_facing_mask, back_facing_mask;
		StencilOp fail_front, pass_depth_fail_front, pass_depth_pass_front;
		StencilOp fail_back, pass_depth_fail_back, pass_depth_pass_back;
		gl_state->desc.get_stencil_compare_front(front, front_ref, front_mask);
		gl_state->desc.get_stencil_compare_back(back, back_ref, back_mask);
		gl_state->desc.get_stencil_write_mask(front_facing_mask, back_facing_mask);
		gl_state->desc.get_stencil_op_front(fail_front, pass_depth_fail_front, pass_depth_pass_front);
		gl_state->desc.get_stencil_op_back(fail_back, pass_depth_fail_back, pass_depth_pass_back);

		enable_stencil_test(gl_state->desc.is_stencil_test_enabled());
		set_stencil_compare_front(front, front_ref, front_mask);
		set_stencil_compare_back(back, back_ref, back_mask);
		set_stencil_write_mask(front_facing_mask, back_facing_mask);
		set_stencil_op_front(fail_front, pass_depth_fail_front, pass_depth_pass_front);
		set_stencil_op_back(fail_back, pass_depth_fail_back, pass_depth_pass_back);
		enable_depth_test(gl_state->desc.is_depth_test_enabled());
		enable_depth_write(gl_state->desc.is_depth_write_enabled());
		set_depth_compare_function(gl_state->desc.get_depth_compare_function());
	}
}

PixelBuffer OpenGLGraphicContextProvider::get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const 
{
	TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
	if (!tf.valid)
		throw Exception("Unsupported texture format passed to GraphicContext::get_pixeldata");

	PixelBuffer pbuf(rect.get_width(), rect.get_height(), texture_format);
	OpenGL::set_active(this);
	if (!framebuffer_bound)
		glReadBuffer(GL_BACK);
	if (glClampColor)
		glClampColor(GL_CLAMP_READ_COLOR, clamp ? GL_TRUE : GL_FALSE);

	Size display_size = get_display_window_size();

	glReadPixels(rect.left, display_size.height - rect.bottom, rect.get_width(), rect.get_height(), tf.pixel_format, tf.pixel_datatype, pbuf.get_data());
	pbuf.flip_vertical();
	return pbuf;
}

void OpenGLGraphicContextProvider::set_uniform_buffer(int index, const UniformBuffer &buffer)
{
	OpenGL::set_active(this);
	glBindBufferBase(GL_UNIFORM_BUFFER, index, static_cast<OpenGLUniformBufferProvider*>(buffer.get_provider())->get_handle());
}

void OpenGLGraphicContextProvider::reset_uniform_buffer(int index)
{
	OpenGL::set_active(this);
	glBindBufferBase(GL_UNIFORM_BUFFER, index, 0);
}

void OpenGLGraphicContextProvider::set_storage_buffer(int index, const StorageBuffer &buffer)
{
	OpenGL::set_active(this);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<OpenGLStorageBufferProvider*>(buffer.get_provider())->get_handle());
}

void OpenGLGraphicContextProvider::reset_storage_buffer(int index)
{
	OpenGL::set_active(this);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, 0);
}

void OpenGLGraphicContextProvider::set_texture(int unit_index, const Texture &texture)
{
	OpenGL::set_active(this);

	if (glActiveTexture != 0)
	{
		glActiveTexture( GL_TEXTURE0 + unit_index );
	}
	else if (unit_index > 0)
	{
		return;
	}

	if (texture.is_null())
	{
		// Perhaps call glBindTexture(texture_type, 0) ?
	}
	else
	{
		OpenGLTextureProvider *provider = static_cast<OpenGLTextureProvider *>(texture.get_provider());
		if (OpenGL::get_opengl_version_major() < 3)
		{
			glEnable(provider->get_texture_type());
		}
		glBindTexture(provider->get_texture_type(), provider->get_handle());
	}
}

void OpenGLGraphicContextProvider::reset_texture(int unit_index)
{
	OpenGL::set_active(this);

	if (glActiveTexture != 0)
	{
		glActiveTexture( GL_TEXTURE0 + unit_index );
	}
	else if (unit_index > 0)
	{
		return;
	}
	// Set the texture to the default state
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLGraphicContextProvider::set_image_texture(int unit_index, const Texture &texture)
{
	OpenGL::set_active(this);

	if (!texture.is_null())
	{
		OpenGL::set_active(this);
		OpenGLTextureProvider *provider = static_cast<OpenGLTextureProvider *>(texture.get_provider());
		glBindImageTexture(unit_index, provider->get_handle(), 0, GL_FALSE, 0, GL_READ_WRITE, provider->get_internal_format());
	}
}

void OpenGLGraphicContextProvider::reset_image_texture(int unit_index)
{
	OpenGL::set_active(this);
	glBindImageTexture(unit_index, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
}

bool OpenGLGraphicContextProvider::is_frame_buffer_owner(const FrameBuffer &fb)
{
	OpenGLFrameBufferProvider *fb_provider = dynamic_cast<OpenGLFrameBufferProvider *>(fb.get_provider());
	if (fb_provider)
		return fb_provider->get_gc_provider() == this;
	else
		return false;
}

void OpenGLGraphicContextProvider::set_frame_buffer(const FrameBuffer &draw_buffer, const FrameBuffer &read_buffer)
{
	OpenGLFrameBufferProvider *draw_buffer_provider = dynamic_cast<OpenGLFrameBufferProvider *>(draw_buffer.get_provider());
	OpenGLFrameBufferProvider *read_buffer_provider = dynamic_cast<OpenGLFrameBufferProvider *>(read_buffer.get_provider());

	if (draw_buffer_provider->get_gc_provider() != this || read_buffer_provider->get_gc_provider() != this)
		throw Exception("FrameBuffer objects cannot be shared between multiple GraphicContext objects");

	OpenGL::set_active(this);

	draw_buffer_provider->bind_framebuffer(true);
	if (draw_buffer_provider != read_buffer_provider)		// You cannot read and write to the same framebuffer
		read_buffer_provider->bind_framebuffer(false);

	// Check for framebuffer completeness
	draw_buffer_provider->check_framebuffer_complete();
	if (draw_buffer_provider != read_buffer_provider)
		read_buffer_provider->check_framebuffer_complete();
}

void OpenGLGraphicContextProvider::reset_frame_buffer()
{
	OpenGL::set_active(this);

#ifdef __APPLE__
    cl_set_default_frame_buffer((DisplayWindowProvider*)render_window);
        
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
#endif
	glDrawBuffer(GL_BACK);	// OpenGL default
	glReadBuffer(GL_BACK);	// OpenGL default

}

void OpenGLGraphicContextProvider::set_program_object(StandardProgram standard_program)
{
	set_program_object(standard_programs.get_program_object(standard_program));
}

void OpenGLGraphicContextProvider::set_program_object(const ProgramObject &program)
{
	OpenGL::set_active(this);
	if (glUseProgram == 0)
		return;

	if (program.is_null())
		glUseProgram(0);
	else
	{
		glUseProgram(program.get_handle());
	}
}

void OpenGLGraphicContextProvider::reset_program_object()
{
	OpenGL::set_active(this);
	glUseProgram(0);
}

bool OpenGLGraphicContextProvider::is_primitives_array_owner(const PrimitivesArray &prim_array)
{
	OpenGLPrimitivesArrayProvider *prim_array_provider = dynamic_cast<OpenGLPrimitivesArrayProvider *>(prim_array.get_provider());
	if (prim_array_provider)
		return prim_array_provider->get_gc_provider() == this;
	else
		return false;
}

void OpenGLGraphicContextProvider::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &primitives_array)
{
	set_primitives_array(primitives_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void OpenGLGraphicContextProvider::set_primitives_array(const PrimitivesArray &primitives_array)
{
	OpenGLPrimitivesArrayProvider *prim_array = static_cast<OpenGLPrimitivesArrayProvider *>(primitives_array.get_provider());

	OpenGL::set_active(this);
	glBindVertexArray(prim_array->handle);
}

void OpenGLGraphicContextProvider::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
{
	OpenGL::set_active(this);
	glDrawArrays(to_enum(type), offset, num_vertices);
}

void OpenGLGraphicContextProvider::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
{
	OpenGL::set_active(this);
	glDrawArraysInstanced(to_enum(type), offset, num_vertices, instance_count);
}

void OpenGLGraphicContextProvider::set_primitives_elements(ElementArrayBufferProvider *array_provider)
{
	OpenGL::set_active(this);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<OpenGLElementArrayBufferProvider *>(array_provider)->get_handle());
}

void OpenGLGraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
{
	OpenGL::set_active(this);
	glDrawElements(to_enum(type), count, to_enum(indices_type), (const GLvoid*)offset);
}

void OpenGLGraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
{
	OpenGL::set_active(this);
	glDrawElementsInstanced(to_enum(type), count, to_enum(indices_type), (const GLvoid*)offset, instance_count);
}

void OpenGLGraphicContextProvider::reset_primitives_elements()
{
	OpenGL::set_active(this);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLGraphicContextProvider::draw_primitives_elements(
	PrimitivesType type,
	int count,
	ElementArrayBufferProvider *array_provider,
	VertexAttributeDataType indices_type,
	void *offset)
{
	OpenGL::set_active(this);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<OpenGLElementArrayBufferProvider *>(array_provider)->get_handle());
	glDrawElements(to_enum(type), count, to_enum(indices_type), offset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLGraphicContextProvider::draw_primitives_elements_instanced(
	PrimitivesType type,
	int count,
	ElementArrayBufferProvider *array_provider,
	VertexAttributeDataType indices_type,
	void *offset,
	int instance_count)
{
	OpenGL::set_active(this);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<OpenGLElementArrayBufferProvider *>(array_provider)->get_handle());
	glDrawElementsInstanced(to_enum(type), count, to_enum(indices_type), offset, instance_count);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLGraphicContextProvider::reset_primitives_array()
{
	OpenGL::set_active(this);
	glBindVertexArray(0);
}

void OpenGLGraphicContextProvider::set_scissor(const Rect &rect)
{
	OpenGL::set_active(this);

	if (!scissor_enabled)
		throw Exception("RasterizerState must be set with enable_scissor() for clipping to work");

	glEnable(GL_SCISSOR_TEST);
	glScissor(
		rect.left,
		rect.top,
		rect.get_width(),
		rect.get_height());
}

void OpenGLGraphicContextProvider::reset_scissor()
{
	OpenGL::set_active(this);
	glDisable(GL_SCISSOR_TEST);
}

void OpenGLGraphicContextProvider::dispatch(int x, int y, int z)
{
	OpenGL::set_active(this);
	glDispatchCompute(x, y, z);
}

void OpenGLGraphicContextProvider::clear(const Colorf &color)
{
	OpenGL::set_active(this);
	glClearColor(
		(GLclampf) color.r,
		(GLclampf) color.g,
		(GLclampf) color.b,
		(GLclampf) color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLGraphicContextProvider::clear_stencil(int value)
{
	OpenGL::set_active(this);
	glClearStencil(value);
	glClear(GL_STENCIL_BUFFER_BIT);
}

void OpenGLGraphicContextProvider::clear_depth(float value)
{
	OpenGL::set_active(this);
    if (glClearDepth)
        glClearDepth(value);
    else
        glClearDepthf(value);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLGraphicContextProvider::on_window_resized()
{
	window_resized_signal.invoke(render_window->get_viewport().get_size());
}

void OpenGLGraphicContextProvider::set_viewport(const Rectf &viewport)
{
	OpenGL::set_active(this);
	glViewport(
		GLsizei(viewport.left),
		GLsizei(viewport.top),
		GLsizei(viewport.right - viewport.left),
		GLsizei(viewport.bottom - viewport.top));
}

void OpenGLGraphicContextProvider::set_viewport(int index, const Rectf &viewport)
{
	if (glViewportIndexedf)
	{
		OpenGL::set_active(this);
		glViewportIndexedf(index,
			GLfloat(viewport.left),
			GLfloat(viewport.top),
			GLfloat(viewport.right - viewport.left),
			GLfloat(viewport.bottom - viewport.top));
	}
	else
	{
		if (index==0)
			set_viewport(viewport);
	}
}

void OpenGLGraphicContextProvider::set_depth_range(float n, float f)
{
	OpenGL::set_active(this);
	glDepthRange((double)n, (double)f); // glDepthRangef is from the OpenGL 4.1 extension ARB_ES2_Compatibility.
}

void OpenGLGraphicContextProvider::set_depth_range(int viewport, float n, float f)
{
	OpenGL::set_active(this);
	glDepthRangeIndexed(viewport, (float)n, (float)f);
}

void OpenGLGraphicContextProvider::enable_blending(bool value)
{
	OpenGL::set_active(this);

	if( value )
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void OpenGLGraphicContextProvider::set_blend_color(const Colorf &color)
{
	OpenGL::set_active(this);
	if (glBlendColor)
	{
		glBlendColor(
			GLclampf(color.get_red()),
			GLclampf(color.get_green()),
			GLclampf(color.get_blue()),
			GLclampf(color.get_alpha()));
	}
}

void OpenGLGraphicContextProvider::set_blend_equation(BlendEquation equation_color, BlendEquation equation_alpha)
{
	OpenGL::set_active(this);
	if (equation_color == equation_alpha)
	{
		if (glBlendEquation)
			glBlendEquation(to_enum(equation_color));
	}
	else
	{
		if (glBlendEquationSeparate)
			glBlendEquationSeparate(
				to_enum(equation_color),
				to_enum(equation_alpha) );
	}
}

void OpenGLGraphicContextProvider::set_blend_function(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha)
{
	OpenGL::set_active(this);

	if( src == src_alpha && dest == dest_alpha )
	{
		if (glBlendFunc)
			glBlendFunc(to_enum(src), to_enum(dest));
	}
	else
	{
		if (glBlendFuncSeparate)
			glBlendFuncSeparate( 
				to_enum(src),
				to_enum(dest),
				to_enum(src_alpha),
				to_enum(dest_alpha) );
	}
}

void OpenGLGraphicContextProvider::set_point_size(float value)
{
	OpenGL::set_active(this);
    if (glPointSize)
        glPointSize((GLfloat)value);

}

void OpenGLGraphicContextProvider::set_point_fade_treshold_size(float value)
{
	OpenGL::set_active(this);
	if (glPointParameterf)
		glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, (GLfloat)value);
}

void OpenGLGraphicContextProvider::set_line_width(float value)
{
	OpenGL::set_active(this);
	glLineWidth((GLfloat)value);

}

void OpenGLGraphicContextProvider::enable_line_antialiasing(bool enabled)
{
	OpenGL::set_active(this);
	if (enabled)
		glEnable(GL_LINE_SMOOTH);
	else
		glDisable(GL_LINE_SMOOTH);

}

void OpenGLGraphicContextProvider::enable_vertex_program_point_size(bool enabled)
{
	OpenGL::set_active(this);
	if (enabled)
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	else
		glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

void OpenGLGraphicContextProvider::set_point_sprite_origin(PointSpriteOrigin origin)
{
	OpenGL::set_active(this);
	if(glPointParameterf)
	{
		switch (origin)
		{
		case origin_upper_left:
			glPointParameterf(GL_POINT_SPRITE_COORD_ORIGIN, GL_UPPER_LEFT);
			break;
		case origin_lower_left:
			glPointParameterf(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
			break;
		}
	}
}

void OpenGLGraphicContextProvider::set_antialiased(bool value)
{
	OpenGL::set_active(this);
	if (value)
		glEnable(GL_POLYGON_SMOOTH);
	else
		glDisable(GL_POLYGON_SMOOTH);
}

void OpenGLGraphicContextProvider::set_culled(bool value)
{
	OpenGL::set_active(this);
	if (value)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void OpenGLGraphicContextProvider::set_point_offset(bool value)
{
	OpenGL::set_active(this);
	if (value)
		glEnable(GL_POLYGON_OFFSET_POINT);
	else
		glDisable(GL_POLYGON_OFFSET_POINT);
}

void OpenGLGraphicContextProvider::set_line_offset(bool value)
{
	OpenGL::set_active(this);
	if (value)
		glEnable(GL_POLYGON_OFFSET_LINE);
	else
		glDisable(GL_POLYGON_OFFSET_LINE);

}

void OpenGLGraphicContextProvider::set_polygon_offset(bool value)
{
	OpenGL::set_active(this);
	if (value)
		glEnable(GL_POLYGON_OFFSET_FILL);
	else
		glDisable(GL_POLYGON_OFFSET_FILL);
}

void OpenGLGraphicContextProvider::set_face_cull_mode(CullMode value)
{
	OpenGL::set_active(this);
	switch (value)
	{
	case cull_front:
		glCullFace(GL_FRONT);
		break;
	case cull_back:
		glCullFace(GL_BACK);
		break;
	case cull_front_and_back:
		glCullFace(GL_FRONT_AND_BACK);
		break;
	}

}

void OpenGLGraphicContextProvider::set_face_fill_mode(FillMode value)
{
	OpenGL::set_active(this);
   if (glPolygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, to_enum(value));
    }
}

void OpenGLGraphicContextProvider::set_front_face(FaceSide value)
{
	OpenGL::set_active(this);
	switch (value)
	{
	case face_counter_clockwise:
		glFrontFace(GL_CCW);
		break;
	case face_clockwise:
		glFrontFace(GL_CW);
		break;
	}
}

void OpenGLGraphicContextProvider::set_offset_factor(float value)
{
	OpenGL::set_active(this);
	glPolygonOffset(value, value);
}

void OpenGLGraphicContextProvider::set_offset_units(float value)
{
	//OpenGL::set_active(this);
	//TODO: Where is the code?
}

void OpenGLGraphicContextProvider::enable_logic_op(bool enabled)
{
	OpenGL::set_active(this);
	if (enabled)
	{
		glEnable(GL_COLOR_LOGIC_OP);
	}
	else
	{
		glDisable(GL_COLOR_LOGIC_OP);
	}
}

void OpenGLGraphicContextProvider::set_logic_op(LogicOp op)
{
	OpenGL::set_active(this);
		glLogicOp(to_enum(op));
}


void OpenGLGraphicContextProvider::set_draw_buffer(DrawBuffer buffer)
{
	OpenGL::set_active(this);
    if (glDrawBuffer)
        glDrawBuffer( to_enum(buffer) );

}

void OpenGLGraphicContextProvider::enable_stencil_test(bool enabled)
{
	OpenGL::set_active(this);
	if (enabled)
	{
		glEnable(GL_STENCIL_TEST);
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}
}

void OpenGLGraphicContextProvider::set_stencil_compare_front(CompareFunction compare_front, int front_ref, int front_mask)
{
	OpenGL::set_active(this);

	if (glStencilFuncSeparate)
	{
		glStencilFuncSeparate(GL_FRONT,
			to_enum(compare_front),
			front_ref,
			front_mask);
	}
}

void OpenGLGraphicContextProvider::set_stencil_compare_back(CompareFunction compare_back, int back_ref, int back_mask)
{
	OpenGL::set_active(this);

	if (glStencilFuncSeparate)
	{
		glStencilFuncSeparate(GL_BACK,
			to_enum(compare_back),
			back_ref,
			back_mask);
	}
}

void OpenGLGraphicContextProvider::set_stencil_write_mask(unsigned char front_facing_mask, unsigned char back_facing_mask)
{
	OpenGL::set_active(this);
	if (glStencilMaskSeparate)
	{
		glStencilMaskSeparate( GL_FRONT, front_facing_mask );
		glStencilMaskSeparate( GL_BACK, back_facing_mask );
	}

}

void OpenGLGraphicContextProvider::set_stencil_op_front(StencilOp fail_front, StencilOp pass_depth_fail_front, StencilOp pass_depth_pass_front)
{
	OpenGL::set_active(this);

	if (glStencilOpSeparate)
	{
		glStencilOpSeparate(GL_FRONT,
			to_enum(fail_front),
			to_enum(pass_depth_fail_front),
			to_enum(pass_depth_pass_front));
	}

}

void OpenGLGraphicContextProvider::set_stencil_op_back(StencilOp fail_back, StencilOp pass_depth_fail_back, StencilOp pass_depth_pass_back)
{
	OpenGL::set_active(this);

	if (glStencilOpSeparate)
	{
		glStencilOpSeparate(GL_BACK,
			to_enum(fail_back),
			to_enum(pass_depth_fail_back),
			to_enum(pass_depth_pass_back));
	}}

void OpenGLGraphicContextProvider::enable_depth_test(bool enabled)
{
	OpenGL::set_active(this);
	if( enabled )
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

}

void OpenGLGraphicContextProvider::enable_depth_write(bool enabled)
{
	OpenGL::set_active(this);
	glDepthMask(enabled ? 1 : 0);
}

void OpenGLGraphicContextProvider::set_depth_compare_function(CompareFunction func)
{
	OpenGL::set_active(this);
	glDepthFunc(to_enum(func));
}

void OpenGLGraphicContextProvider::enable_color_write(bool red, bool green, bool blue, bool alpha)
{
	OpenGL::set_active(this);
	glColorMask(red,green,blue,alpha);
}


/////////////////////////////////////////////////////////////////////////////
// OpenGLGraphicContextProvider Implementation:

GLenum OpenGLGraphicContextProvider::to_enum(DrawBuffer buffer)
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

GLenum OpenGLGraphicContextProvider::to_enum(CompareFunction func)
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

GLenum OpenGLGraphicContextProvider::to_enum(StencilOp op)
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

GLenum OpenGLGraphicContextProvider::to_enum(CullMode mode)
{
	switch( mode )
	{
	case cull_front: return GL_FRONT;
	case cull_back: return GL_BACK;
	case cull_front_and_back: return GL_FRONT_AND_BACK;
	default: return GL_BACK;
	}
}

GLenum OpenGLGraphicContextProvider::to_enum(FillMode mode)
{
	switch( mode )
	{
	case fill_point: return GL_POINT;
	case fill_line: return GL_LINE;
	case fill_polygon: return GL_FILL;
	default: return GL_FILL;
	}
}

GLenum OpenGLGraphicContextProvider::to_enum(BlendFunc func)
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

GLenum OpenGLGraphicContextProvider::to_enum(BlendEquation eq)
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

GLenum OpenGLGraphicContextProvider::to_enum(enum VertexAttributeDataType value)
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

GLenum OpenGLGraphicContextProvider::to_enum(enum PrimitivesType value)
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

GLenum OpenGLGraphicContextProvider::to_enum(enum LogicOp op)
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

}

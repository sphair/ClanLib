/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "gl3_graphic_context_provider.h"
#include "gl3_occlusion_query_provider.h"
#include "gl3_texture_provider.h"
#include "gl3_program_object_provider.h"
#include "gl3_shader_object_provider.h"
#include "gl3_frame_buffer_provider.h"
#include "gl3_render_buffer_provider.h"
#include "gl3_vertex_array_buffer_provider.h"
#include "gl3_uniform_buffer_provider.h"
#include "gl3_storage_buffer_provider.h"
#include "gl3_element_array_buffer_provider.h"
#include "gl3_transfer_buffer_provider.h"
#include "gl3_pixel_buffer_provider.h"
#include "gl3_primitives_array_provider.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
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
#include "API/GL/opengl_context_description.h"
#ifdef __APPLE__
#include "../Platform/OSX/opengl_window_provider_osx.h"
#elif !defined(WIN32)

#if defined(__ANDROID__)
#include "../Platform/Android/opengl_window_provider_android.h"
#else
#include "../Platform/GLX/opengl_window_provider_glx.h"
#endif
#endif

#ifdef WIN32
#include "../Platform/WGL/opengl_window_provider_wgl.h"
#endif

#include <memory>

namespace clan
{
	GL3GraphicContextProvider::GL3GraphicContextProvider(const OpenGLWindowProvider * const render_window)
		: render_window(render_window), framebuffer_bound(false), opengl_version_major(0), shader_version_major(0), scissor_enabled(false)
	{
		check_opengl_version();
		calculate_shading_language_version();

		create_standard_programs();

		reset_program_object();

		SharedGCData::add_provider(this);
	}

	GL3GraphicContextProvider::~GL3GraphicContextProvider()
	{
		dispose();
	}

	void GL3GraphicContextProvider::create_standard_programs()
	{
		// Find an existing provider
		std::unique_ptr<std::unique_lock<std::recursive_mutex>> mutex_section;
		std::vector<GraphicContextProvider*> &gc_providers = SharedGCData::get_gc_providers(mutex_section);

		unsigned int max = gc_providers.size();
		for (unsigned int cnt = 0; cnt < max; cnt++)
		{
			GL3GraphicContextProvider* gc_provider = dynamic_cast<GL3GraphicContextProvider *>(gc_providers[cnt]);
			if (gc_provider != this)
			{
				standard_programs = gc_provider->standard_programs;
				return;
			}
		}

		standard_programs = GL3StandardPrograms(this);

	}
	void GL3GraphicContextProvider::on_dispose()
	{
		while (!disposable_objects.empty())
			disposable_objects.front()->dispose();

		standard_programs = GL3StandardPrograms();

		SharedGCData::remove_provider(this);
		OpenGL::remove_active(this);

	}


	void GL3GraphicContextProvider::add_disposable(DisposableObject *disposable)
	{
		disposable_objects.push_back(disposable);
	}

	void GL3GraphicContextProvider::remove_disposable(DisposableObject *disposable)
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

	ProcAddress *GL3GraphicContextProvider::get_proc_address(const std::string& function_name) const
	{
		return render_window->get_proc_address(function_name);
	}

	void GL3GraphicContextProvider::check_opengl_version()
	{
		int version_major = 0;
		int version_minor = 0;

		get_opengl_version(version_major, version_minor);
		if (version_major < 3)
			throw Exception(string_format("This application requires OpenGL 3.0 or above. Your hardware only supports OpenGL %1.%2. Try updating your drivers, or upgrade to a newer graphics card.", version_major, version_minor));
	}

	void GL3GraphicContextProvider::get_opengl_version(int &version_major, int &version_minor) const
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

#if defined(__IOS__)
			opengl_version_major = 0;
			opengl_version_minor = 0;
			glGetIntegerv(GL_MAJOR_VERSION, &opengl_version_major);
			glGetIntegerv(GL_MINOR_VERSION, &opengl_version_minor);
#else

			std::string version = (char*)glGetString(GL_VERSION);

#ifndef CLANLIB_OPENGL_ES3

			opengl_version_major = 0;
			opengl_version_minor = 0;

			std::vector<std::string> split_version = StringHelp::split_text(version, ".");
			if (split_version.size() > 0)
				opengl_version_major = StringHelp::text_to_int(split_version[0]);
			if (split_version.size() > 1)
				opengl_version_minor = StringHelp::text_to_int(split_version[1]);

#else
			opengl_version_major = 3;	//FIXME
			opengl_version_minor = 2;	//FIXME
#endif

#endif
		}

		version_major = opengl_version_major;
		version_minor = opengl_version_minor;
	}

	void GL3GraphicContextProvider::calculate_shading_language_version()
	{
		// See http://www.opengl.org/wiki/Detecting_the_Shader_Model
		shader_version_major = 0;
		shader_version_minor = 0;

		if ((opengl_version_major < 2) || ((opengl_version_major == 2) && (opengl_version_minor < 1)))
		{
			OpenGL::set_active(this);

			std::string version = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

			std::vector<std::string> split_version = StringHelp::split_text(version, ".");
			if (split_version.size() > 0)
				shader_version_major = StringHelp::text_to_int(split_version[0]);
			if (split_version.size() > 1)
				shader_version_minor = StringHelp::text_to_int(split_version[1]);
		}
		else
		{
			if (opengl_version_major == 2)
			{
				shader_version_major = 1;
				shader_version_minor = 20;
			}
			else if ((opengl_version_major == 3) && (opengl_version_minor == 0))
			{
				shader_version_major = 1;
				shader_version_minor = 30;
			}
			else if ((opengl_version_major == 3) && (opengl_version_minor == 1))
			{
				shader_version_major = 1;
				shader_version_minor = 40;
			}
			else if ((opengl_version_major == 3) && (opengl_version_minor == 2))
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

	int GL3GraphicContextProvider::get_max_attributes()
	{
		OpenGL::set_active(this);
		GLint max_attributes = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
		if (max_attributes < 16)
			max_attributes = 16;
		return max_attributes;
	}

	Size GL3GraphicContextProvider::get_max_texture_size() const
	{
		OpenGL::set_active(this);
		GLint max_size = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
		return Size(max_size, max_size);
	}

	Size GL3GraphicContextProvider::get_display_window_size() const
	{
		return render_window->get_viewport().get_size();
	}

	ProgramObject GL3GraphicContextProvider::get_program_object(StandardProgram standard_program) const
	{
		return standard_programs.get_program_object(standard_program);
	}

	float GL3GraphicContextProvider::get_pixel_ratio() const
	{
		return render_window->get_pixel_ratio();
	}

	std::unique_ptr<OcclusionQueryProvider> GL3GraphicContextProvider::alloc_occlusion_query()
	{
		return std::make_unique<GL3OcclusionQueryProvider>(this);
	}

	std::unique_ptr<ProgramObjectProvider> GL3GraphicContextProvider::alloc_program_object()
	{
		return std::make_unique<GL3ProgramObjectProvider>();
	}

	std::unique_ptr<ShaderObjectProvider> GL3GraphicContextProvider::alloc_shader_object()
	{
		return std::make_unique<GL3ShaderObjectProvider>();
	}

	std::unique_ptr<TextureProvider> GL3GraphicContextProvider::alloc_texture(TextureDimensions texture_dimensions)
	{
		return std::make_unique<GL3TextureProvider>(texture_dimensions);
	}

	std::unique_ptr<FrameBufferProvider> GL3GraphicContextProvider::alloc_frame_buffer()
	{
		return std::make_unique<GL3FrameBufferProvider>(this);
	}

	std::unique_ptr<RenderBufferProvider> GL3GraphicContextProvider::alloc_render_buffer()
	{
		return std::make_unique<GL3RenderBufferProvider>();
	}

	std::unique_ptr<VertexArrayBufferProvider> GL3GraphicContextProvider::alloc_vertex_array_buffer()
	{
		return std::make_unique<GL3VertexArrayBufferProvider>();
	}

	std::unique_ptr<UniformBufferProvider> GL3GraphicContextProvider::alloc_uniform_buffer()
	{
		return std::make_unique<GL3UniformBufferProvider>();
	}

	std::unique_ptr<StorageBufferProvider> GL3GraphicContextProvider::alloc_storage_buffer()
	{
		return std::make_unique<GL3StorageBufferProvider>();
	}

	std::unique_ptr<ElementArrayBufferProvider> GL3GraphicContextProvider::alloc_element_array_buffer()
	{
		return std::make_unique<GL3ElementArrayBufferProvider>();
	}

	std::unique_ptr<TransferBufferProvider> GL3GraphicContextProvider::alloc_transfer_buffer()
	{
		return std::make_unique<GL3TransferBufferProvider>();
	}

	std::unique_ptr<PixelBufferProvider> GL3GraphicContextProvider::alloc_pixel_buffer()
	{
		return std::make_unique<GL3PixelBufferProvider>();
	}

	std::unique_ptr<PrimitivesArrayProvider> GL3GraphicContextProvider::alloc_primitives_array()
	{
		return std::make_unique<GL3PrimitivesArrayProvider>(this);
	}

	std::unique_ptr<RasterizerStateProvider> GL3GraphicContextProvider::alloc_rasterizer_state(const RasterizerStateDescription &desc)
	{
		return std::make_unique<OpenGLRasterizerStateProvider>(desc);
	}

	std::unique_ptr<BlendStateProvider> GL3GraphicContextProvider::alloc_blend_state(const BlendStateDescription &desc)
	{
		return std::make_unique<OpenGLBlendStateProvider>(desc);
	}

	std::unique_ptr<DepthStencilStateProvider> GL3GraphicContextProvider::alloc_depth_stencil_state(const DepthStencilStateDescription &desc)
	{
		return std::make_unique<OpenGLDepthStencilStateProvider>(desc);
	}

	void GL3GraphicContextProvider::set_rasterizer_state(RasterizerStateProvider *state)
	{
		if (state)
		{
			OpenGLRasterizerStateProvider *gl3_state = static_cast<OpenGLRasterizerStateProvider*>(state);
			if (gl3_state)
			{
				selected_rasterizer_state.set(gl3_state->desc);
				OpenGL::set_active(this);
				selected_rasterizer_state.apply();
				scissor_enabled = gl3_state->desc.get_enable_scissor();
			}
		}
	}

	void GL3GraphicContextProvider::set_blend_state(BlendStateProvider *state, const Colorf &blend_color, unsigned int sample_mask)
	{
		if (state)
		{
			OpenGLBlendStateProvider *gl3_state = static_cast<OpenGLBlendStateProvider*>(state);
			if (gl3_state)
			{
				selected_blend_state.set(gl3_state->desc, blend_color);
				OpenGL::set_active(this);
				selected_blend_state.apply();
			}
		}
	}

	void GL3GraphicContextProvider::set_depth_stencil_state(DepthStencilStateProvider *state, int stencil_ref)
	{
		if (state)
		{
			OpenGLDepthStencilStateProvider *gl3_state = static_cast<OpenGLDepthStencilStateProvider*>(state);
			if (gl3_state)
			{
				selected_depth_stencil_state.set(gl3_state->desc);
				OpenGL::set_active(this);
				selected_depth_stencil_state.apply();
			}
		}
	}

	PixelBuffer GL3GraphicContextProvider::get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const
	{
		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (!tf.valid)
			throw Exception("Unsupported texture format passed to GraphicContext::get_pixeldata");

		PixelBuffer pbuf(rect.get_width(), rect.get_height(), texture_format);
		OpenGL::set_active(this);
		if (!framebuffer_bound)
		{
			render_window->is_double_buffered() ? glReadBuffer(GL_BACK) : glReadBuffer(GL_FRONT);
		}
#ifndef CLANLIB_OPENGL_ES3
		if (glClampColor)
			glClampColor(GL_CLAMP_READ_COLOR, clamp ? GL_TRUE : GL_FALSE);
#endif
		Size display_size = get_display_window_size();

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ROW_LENGTH, pbuf.get_pitch() / pbuf.get_bytes_per_pixel());
		glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_PACK_SKIP_ROWS, 0);
		glReadPixels(rect.left, display_size.height - rect.bottom, rect.get_width(), rect.get_height(), tf.pixel_format, tf.pixel_datatype, pbuf.get_data());
		pbuf.flip_vertical();
		return pbuf;
	}

	void GL3GraphicContextProvider::set_uniform_buffer(int index, const UniformBuffer &buffer)
	{
		OpenGL::set_active(this);
		glBindBufferBase(GL_UNIFORM_BUFFER, index, static_cast<GL3UniformBufferProvider*>(buffer.get_provider())->get_handle());
	}

	void GL3GraphicContextProvider::reset_uniform_buffer(int index)
	{
		OpenGL::set_active(this);
		glBindBufferBase(GL_UNIFORM_BUFFER, index, 0);
	}

	void GL3GraphicContextProvider::set_storage_buffer(int index, const StorageBuffer &buffer)
	{
		OpenGL::set_active(this);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<GL3StorageBufferProvider*>(buffer.get_provider())->get_handle());
	}

	void GL3GraphicContextProvider::reset_storage_buffer(int index)
	{
		OpenGL::set_active(this);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, 0);
	}

	void GL3GraphicContextProvider::set_texture(int unit_index, const Texture &texture)
	{
		OpenGL::set_active(this);

		if (glActiveTexture != nullptr)
		{
			glActiveTexture(GL_TEXTURE0 + unit_index);
		}
		else if (unit_index > 0)
		{
			return;
		}

		if (!texture.is_null())
		{
			GL3TextureProvider *provider = static_cast<GL3TextureProvider *>(texture.get_provider());
			glBindTexture(provider->get_texture_type(), provider->get_handle());
		}
	}

	void GL3GraphicContextProvider::reset_texture(int unit_index)
	{
		OpenGL::set_active(this);

		if (glActiveTexture != nullptr)
		{
			glActiveTexture(GL_TEXTURE0 + unit_index);
		}
		else if (unit_index > 0)
		{
			return;
		}
		// Set the texture to the default state
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GL3GraphicContextProvider::set_image_texture(int unit_index, const Texture &texture)
	{
		OpenGL::set_active(this);

		if (!texture.is_null())
		{
			OpenGL::set_active(this);
			GL3TextureProvider *provider = static_cast<GL3TextureProvider *>(texture.get_provider());
			glBindImageTexture(unit_index, provider->get_handle(), 0, GL_FALSE, 0, GL_READ_WRITE, provider->get_internal_format());
		}
	}

	void GL3GraphicContextProvider::reset_image_texture(int unit_index)
	{
		OpenGL::set_active(this);
		glBindImageTexture(unit_index, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	}

	bool GL3GraphicContextProvider::is_frame_buffer_owner(const FrameBuffer &fb)
	{
		GL3FrameBufferProvider *fb_provider = dynamic_cast<GL3FrameBufferProvider *>(fb.get_provider());
		if (fb_provider)
			return fb_provider->get_gc_provider() == this;
		else
			return false;
	}

	void GL3GraphicContextProvider::set_frame_buffer(const FrameBuffer &draw_buffer, const FrameBuffer &read_buffer)
	{
		GL3FrameBufferProvider *draw_buffer_provider = dynamic_cast<GL3FrameBufferProvider *>(draw_buffer.get_provider());
		GL3FrameBufferProvider *read_buffer_provider = dynamic_cast<GL3FrameBufferProvider *>(read_buffer.get_provider());

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

		framebuffer_bound = true;
	}

	void GL3GraphicContextProvider::reset_frame_buffer()
	{
		OpenGL::set_active(this);

		// To do: move this to OpenGLWindowProvider abstraction (some targets doesn't have a default frame buffer)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifndef CLANLIB_OPENGL_ES3
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		if (render_window->is_double_buffered())
		{
			glDrawBuffer(GL_BACK);
			glReadBuffer(GL_BACK);
		}
		else
		{
			glDrawBuffer(GL_FRONT);
			glReadBuffer(GL_FRONT);
		}
#endif
		framebuffer_bound = false;

	}

	void GL3GraphicContextProvider::set_program_object(StandardProgram standard_program)
	{
		set_program_object(standard_programs.get_program_object(standard_program));
	}

	void GL3GraphicContextProvider::set_program_object(const ProgramObject &program)
	{
		OpenGL::set_active(this);
		if (glUseProgram == nullptr)
			return;

		if (program.is_null())
			glUseProgram(0);
		else
		{
			glUseProgram(program.get_handle());
		}
	}

	void GL3GraphicContextProvider::reset_program_object()
	{
		OpenGL::set_active(this);
		glUseProgram(0);
	}

	bool GL3GraphicContextProvider::is_primitives_array_owner(const PrimitivesArray &prim_array)
	{
		GL3PrimitivesArrayProvider *prim_array_provider = dynamic_cast<GL3PrimitivesArrayProvider *>(prim_array.get_provider());
		if (prim_array_provider)
			return prim_array_provider->get_gc_provider() == this;
		else
			return false;
	}

	void GL3GraphicContextProvider::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &primitives_array)
	{
		set_primitives_array(primitives_array);
		draw_primitives_array(type, 0, num_vertices);
		reset_primitives_array();
	}

	void GL3GraphicContextProvider::set_primitives_array(const PrimitivesArray &primitives_array)
	{
		GL3PrimitivesArrayProvider *prim_array = static_cast<GL3PrimitivesArrayProvider *>(primitives_array.get_provider());

		OpenGL::set_active(this);
		glBindVertexArray(prim_array->handle);
	}

	void GL3GraphicContextProvider::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
	{
		OpenGL::set_active(this);
		glDrawArrays(OpenGL::to_enum(type), offset, num_vertices);
	}

	void GL3GraphicContextProvider::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
	{
		OpenGL::set_active(this);
		glDrawArraysInstanced(OpenGL::to_enum(type), offset, num_vertices, instance_count);
	}

	void GL3GraphicContextProvider::set_primitives_elements(ElementArrayBufferProvider *array_provider)
	{
		OpenGL::set_active(this);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GL3ElementArrayBufferProvider *>(array_provider)->get_handle());
	}

	void GL3GraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
	{
		OpenGL::set_active(this);
		glDrawElements(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), (const GLvoid*)offset);
	}

	void GL3GraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		OpenGL::set_active(this);
		glDrawElementsInstanced(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), (const GLvoid*)offset, instance_count);
	}

	void GL3GraphicContextProvider::reset_primitives_elements()
	{
		OpenGL::set_active(this);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GL3GraphicContextProvider::draw_primitives_elements(
		PrimitivesType type,
		int count,
		ElementArrayBufferProvider *array_provider,
		VertexAttributeDataType indices_type,
		void *offset)
	{
		OpenGL::set_active(this);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GL3ElementArrayBufferProvider *>(array_provider)->get_handle());
		glDrawElements(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), offset);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GL3GraphicContextProvider::draw_primitives_elements_instanced(
		PrimitivesType type,
		int count,
		ElementArrayBufferProvider *array_provider,
		VertexAttributeDataType indices_type,
		void *offset,
		int instance_count)
	{
		OpenGL::set_active(this);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GL3ElementArrayBufferProvider *>(array_provider)->get_handle());
		glDrawElementsInstanced(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), offset, instance_count);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GL3GraphicContextProvider::reset_primitives_array()
	{
		OpenGL::set_active(this);
		glBindVertexArray(0);
	}

	void GL3GraphicContextProvider::set_scissor(const Rect &rect)
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

	void GL3GraphicContextProvider::reset_scissor()
	{
		OpenGL::set_active(this);
		glDisable(GL_SCISSOR_TEST);
	}

	void GL3GraphicContextProvider::dispatch(int x, int y, int z)
	{
		OpenGL::set_active(this);
		glDispatchCompute(x, y, z);
	}

	void GL3GraphicContextProvider::clear(const Colorf &color)
	{
		OpenGL::set_active(this);
		glClearColor(
			(GLclampf)color.r,
			(GLclampf)color.g,
			(GLclampf)color.b,
			(GLclampf)color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GL3GraphicContextProvider::clear_stencil(int value)
	{
		OpenGL::set_active(this);
		glClearStencil(value);
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	void GL3GraphicContextProvider::clear_depth(float value)
	{
		OpenGL::set_active(this);
#ifndef CLANLIB_OPENGL_ES3
		if (glClearDepth)
			glClearDepth(value);
		else
#endif
		{
			glClearDepthf(value);
		}
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void GL3GraphicContextProvider::on_window_resized()
	{
		window_resized_signal(render_window->get_viewport().get_size());
	}

	void GL3GraphicContextProvider::set_viewport(const Rectf &viewport)
	{
		OpenGL::set_active(this);
		glViewport(
			GLsizei(viewport.left),
			GLsizei(viewport.top),
			GLsizei(viewport.right - viewport.left),
			GLsizei(viewport.bottom - viewport.top));
	}

	void GL3GraphicContextProvider::set_viewport(int index, const Rectf &viewport)
	{
#ifndef CLANLIB_OPENGL_ES3
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
			if (index == 0)
				set_viewport(viewport);
		}
#else
		if (index == 0)
			set_viewport(viewport);
#endif
	}

	void GL3GraphicContextProvider::set_depth_range(float n, float f)
	{
		OpenGL::set_active(this);
#ifndef CLANLIB_OPENGL_ES3
		glDepthRange((double)n, (double)f); // glDepthRangef is from the OpenGL 4.1 extension ARB_ES2_Compatibility.
#endif
	}

	void GL3GraphicContextProvider::set_depth_range(int viewport, float n, float f)
	{
		OpenGL::set_active(this);
#ifndef CLANLIB_OPENGL_ES3
		glDepthRangeIndexed(viewport, (float)n, (float)f);
#endif
	}

	void GL3GraphicContextProvider::set_draw_buffer(DrawBuffer buffer)
	{
		OpenGL::set_active(this);

		if (!render_window->is_double_buffered())	// Silently fix incorrect render buffers
		{
			if (buffer == DrawBuffer::back)
				buffer = DrawBuffer::front;
		}

#ifndef CLANLIB_OPENGL_ES3
		if (glDrawBuffer)
			glDrawBuffer(OpenGL::to_enum(buffer));
#endif

	}

	void GL3GraphicContextProvider::make_current() const
	{
		render_window->make_current();
	}

	void GL3GraphicContextProvider::flush()
	{
		OpenGL::set_active(this);
		glFlush();
	}

	const DisplayWindowProvider & GL3GraphicContextProvider::get_render_window() const
	{
		return *render_window;
	}
}

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
**    Claudio Zanella
*/

#include "GL/precomp.h"
#include "API/GL/opengl_defines.h"
#ifndef CLANLIB_OPENGL_ES3

#include "gl1_graphic_context_provider.h"
#include "gl1_texture_provider.h"
#include "gl1_program_object_provider.h"
#include "gl1_render_buffer_provider.h"
#include "gl1_primitives_array_provider.h"
#include "gl1_vertex_array_buffer_provider.h"
#include "gl1_uniform_buffer_provider.h"
#include "gl1_transfer_buffer_provider.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/2D/image.h"
#include "gl1_frame_buffer_provider.h"
#include "Display/2D/render_batch_triangle.h"

#ifdef WIN32
#include "../Platform/WGL/opengl_window_provider_wgl.h"
#elif defined(__APPLE__)
// To do: add Cocoa window provider here
#else
#include "../Platform/GLX/opengl_window_provider_glx.h"
#endif

namespace clan
{
	class GL1SelectedTexture
	{
	public:
		GL1TextureProvider *texture = nullptr;
	};

	GL1GraphicContextProvider::GL1GraphicContextProvider(OpenGLWindowProvider * render_window)
		: render_window(render_window),
		prim_arrays_set(false), num_set_tex_arrays(0),
		primitives_array_texture_set(false), primitives_array_texindex_set(false), scissor_enabled(false),
		framebuffer_bound(false)
	{
		check_opengl_version();
		max_texture_coords = get_max_texture_coords();
		// Limit the internal texture coords, to avoid situations where the opengl driver says there are unlimited texture coords
		if (max_texture_coords > 32)
			max_texture_coords = 32;
		// Hack, so the sprite render batcher does not exceed the allowed number of textures
		if (max_texture_coords < RenderBatchTriangle::max_textures)
		{
			if (max_texture_coords > 0)
			{
				RenderBatchTriangle::max_textures = max_texture_coords;
			}
			else
			{
				RenderBatchTriangle::max_textures = 1;
			}
		}

		selected_textures.resize(max_texture_coords);
		internal_program = ProgramObject(std::make_unique<GL1ProgramObjectProvider>(this));

		// Enable point sprites for legacy opengl
		glEnable(GL_POINT_SPRITE);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		SharedGCData::add_provider(this);
	}

	GL1GraphicContextProvider::~GL1GraphicContextProvider()
	{
		dispose();
	}

	void GL1GraphicContextProvider::on_dispose()
	{
		while (!disposable_objects.empty())
			disposable_objects.front()->dispose();

		SharedGCData::remove_provider(this);
		OpenGL::remove_active(this);
	}

	void GL1GraphicContextProvider::add_disposable(DisposableObject *disposable)
	{
		disposable_objects.push_back(disposable);
	}

	void GL1GraphicContextProvider::remove_disposable(DisposableObject *disposable)
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

	void GL1GraphicContextProvider::check_opengl_version()
	{
		int version_major = 0;
		int version_minor = 0;
		int version_release = 0;

		get_opengl_version(version_major, version_minor, version_release);
		bool valid_version = false;
		if (version_major > 1)
			valid_version = true;
		if ((version_major == 1) && (version_minor >= 3))
			valid_version = true;

		if (!valid_version)
			throw Exception(string_format("This application requires OpenGL 1.3. Your hardware only supports OpenGL %1.%2.%3. Try updating your drivers, or upgrade to a newer graphics card.", version_major, version_minor, version_release));
	}

	void GL1GraphicContextProvider::get_opengl_version(int &version_major, int &version_minor, int &version_release) const
	{
		/*	The GL_VERSION string begins with a version number. The version number uses one of these forms:
			major_number.minor_number
			major_number.minor_number.release_number
			Vendor-specific information may follow the version number. Its format depends on the implementation, but a space always separates the version number and the vendor-specific information.
			All strings are null-terminated.
			If an error is generated, glGetString returns zero.
			*/
		OpenGL::set_active(this);
		std::string version = (char*)glGetString(GL_VERSION);
		version_major = 0;
		version_minor = 0;
		version_release = 0;

		std::vector<std::string> split_version = StringHelp::split_text(version, ".");
		if (split_version.size() > 0)
			version_major = StringHelp::text_to_int(split_version[0]);
		if (split_version.size() > 1)
			version_minor = StringHelp::text_to_int(split_version[1]);
		if (split_version.size() > 2)
			version_release = StringHelp::text_to_int(split_version[2]);
	}

	ProcAddress *GL1GraphicContextProvider::get_proc_address(const std::string& function_name) const
	{
		return render_window->get_proc_address(function_name);
	}

	int GL1GraphicContextProvider::get_max_texture_coords()
	{
		set_active();
		GLint max_textures = 0;
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &max_textures);

		if (glGetError() == GL_INVALID_ENUM)
			glGetIntegerv(GL_MAX_TEXTURE_UNITS, &max_textures);

		return max_textures;
	}

	int GL1GraphicContextProvider::get_max_attributes()
	{
		set_active();
		GLint max_attributes = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
		if (max_attributes < 16)
			max_attributes = 16;
		return max_attributes;
	}


	Size GL1GraphicContextProvider::get_max_texture_size() const
	{
		set_active();
		GLint max_size = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
		return Size(max_size, max_size);
	}

	Size GL1GraphicContextProvider::get_display_window_size() const
	{
		return render_window->get_viewport().get_size();
	}

	float GL1GraphicContextProvider::get_pixel_ratio() const
	{
		return render_window->get_pixel_ratio();
	}

	void GL1GraphicContextProvider::set_active() const
	{
		if (framebuffer_bound)
		{
			framebuffer_provider->set_active();
		}
		else
		{
			OpenGL::set_active(this);
		}
	}

	std::unique_ptr<OcclusionQueryProvider> GL1GraphicContextProvider::alloc_occlusion_query()
	{
		throw Exception("Occlusion Queries are not supported for OpenGL 1.3");
	}

	std::unique_ptr<ProgramObjectProvider> GL1GraphicContextProvider::alloc_program_object()
	{
		throw Exception("Program Objects are not supported for OpenGL 1.3");
	}

	std::unique_ptr<ShaderObjectProvider> GL1GraphicContextProvider::alloc_shader_object()
	{
		throw Exception("Shader Objects are not supported for OpenGL 1.3");
	}

	std::unique_ptr<TextureProvider> GL1GraphicContextProvider::alloc_texture(TextureDimensions texture_dimensions)
	{
		return std::make_unique<GL1TextureProvider>(texture_dimensions);
	}

	std::unique_ptr<FrameBufferProvider> GL1GraphicContextProvider::alloc_frame_buffer()
	{
		return std::make_unique<GL1FrameBufferProvider>(this);
	}

	std::unique_ptr<RenderBufferProvider> GL1GraphicContextProvider::alloc_render_buffer()
	{
		return std::make_unique<GL1RenderBufferProvider>(this);
	}

	std::unique_ptr<VertexArrayBufferProvider> GL1GraphicContextProvider::alloc_vertex_array_buffer()
	{
		return std::make_unique<GL1VertexArrayBufferProvider>();
	}

	std::unique_ptr<UniformBufferProvider> GL1GraphicContextProvider::alloc_uniform_buffer()
	{
		return std::make_unique<GL1UniformBufferProvider>();
	}

	std::unique_ptr<StorageBufferProvider> GL1GraphicContextProvider::alloc_storage_buffer()
	{
		throw Exception("Storage Buffers are not supported for OpenGL 1.3");
	}

	std::unique_ptr<ElementArrayBufferProvider> GL1GraphicContextProvider::alloc_element_array_buffer()
	{
		throw Exception("Element Array Buffers are not supported for OpenGL 1.3");
	}

	std::unique_ptr<TransferBufferProvider> GL1GraphicContextProvider::alloc_transfer_buffer()
	{
		return std::make_unique<GL1TransferBufferProvider>();
	}

	std::unique_ptr<PixelBufferProvider> GL1GraphicContextProvider::alloc_pixel_buffer()
	{
		throw Exception("Pixel Buffers Objects are not supported for OpenGL 1.3");
	}

	std::unique_ptr<PrimitivesArrayProvider> GL1GraphicContextProvider::alloc_primitives_array()
	{
		return std::make_unique<GL1PrimitivesArrayProvider>();
	}

	std::unique_ptr<RasterizerStateProvider> GL1GraphicContextProvider::alloc_rasterizer_state(const RasterizerStateDescription &desc)
	{
		return std::make_unique<OpenGLRasterizerStateProvider>(desc);
	}

	std::unique_ptr<BlendStateProvider> GL1GraphicContextProvider::alloc_blend_state(const BlendStateDescription &desc)
	{
		return std::make_unique<OpenGLBlendStateProvider>(desc);
	}

	std::unique_ptr<DepthStencilStateProvider> GL1GraphicContextProvider::alloc_depth_stencil_state(const DepthStencilStateDescription &desc)
	{
		return std::make_unique<OpenGLDepthStencilStateProvider>(desc);
	}

	void GL1GraphicContextProvider::set_rasterizer_state(RasterizerStateProvider *state)
	{
		if (state)
		{
			OpenGLRasterizerStateProvider *gl1_state = static_cast<OpenGLRasterizerStateProvider*>(state);
			if (gl1_state)
			{
				selected_state.rasterizer.set(gl1_state->desc);
				set_active();
				framebuffer_bound ? framebuffer_provider->set_state(selected_state.rasterizer) : selected_state.rasterizer.apply();

				scissor_enabled = gl1_state->desc.get_enable_scissor();
			}

		}
	}

	void GL1GraphicContextProvider::set_blend_state(BlendStateProvider *state, const Colorf &blend_color, unsigned int sample_mask)
	{
		if (state)
		{
			OpenGLBlendStateProvider *gl1_state = static_cast<OpenGLBlendStateProvider*>(state);
			if (gl1_state)
			{
				selected_state.blend.set(gl1_state->desc, blend_color);
				set_active();
				framebuffer_bound ? framebuffer_provider->set_state(selected_state.blend) : selected_state.blend.apply();
			}
		}
	}

	void GL1GraphicContextProvider::set_depth_stencil_state(DepthStencilStateProvider *state, int stencil_ref)
	{
		if (state)
		{
			OpenGLDepthStencilStateProvider *gl1_state = static_cast<OpenGLDepthStencilStateProvider*>(state);
			if (gl1_state)
			{
				selected_state.depth_stencil.set(gl1_state->desc);
				set_active();
				framebuffer_bound ? framebuffer_provider->set_state(selected_state.depth_stencil) : selected_state.depth_stencil.apply();
			}
		}
	}

	PixelBuffer GL1GraphicContextProvider::get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const
	{
		GLenum format;
		GLenum type;
		bool found = GL1TextureProvider::to_opengl_pixelformat(texture_format, format, type);
		if (!found)
			throw Exception("Unsupported pixel format passed to GraphicContext::get_pixeldata");

		PixelBuffer pbuf(rect.get_width(), rect.get_height(), texture_format);
		set_active();
		if (!framebuffer_bound)
			render_window->is_double_buffered() ? glReadBuffer(GL_BACK) : glReadBuffer(GL_FRONT);

		Size display_size = get_display_window_size();

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ROW_LENGTH, pbuf.get_pitch() / pbuf.get_bytes_per_pixel());
		glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_PACK_SKIP_ROWS, 0);

		glReadPixels(rect.left, display_size.height - rect.bottom, rect.get_width(), rect.get_height(), format, type, pbuf.get_data());
		pbuf.flip_vertical();
		return pbuf;
	}

	void GL1GraphicContextProvider::set_uniform_buffer(int index, const UniformBuffer &buffer)
	{
		//GL1UniformBufferProvider *provider = static_cast<GL1UniformBufferProvider*>(buffer.get_provider());
	}

	void GL1GraphicContextProvider::reset_uniform_buffer(int index)
	{
	}

	void GL1GraphicContextProvider::set_storage_buffer(int index, const StorageBuffer &buffer)
	{
	}

	void GL1GraphicContextProvider::reset_storage_buffer(int index)
	{
	}

	void GL1GraphicContextProvider::set_texture(int unit_index, const Texture &texture)
	{
		set_active();

		if ((unit_index < 0) || (unit_index >= max_texture_coords))
		{
			throw Exception("Invalid texture unit index in GL1 target");
		}
		selected_textures[unit_index].texture = nullptr;

		if (glActiveTexture != nullptr)
		{
			glActiveTexture(GL_TEXTURE0 + unit_index);
		}
		else if (unit_index > 0)
		{
			return;
		}

		if (texture.is_null())
		{
			glDisable(GL_TEXTURE_1D);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_3D);
			glDisable(GL_TEXTURE_CUBE_MAP);
		}
		else
		{
			GL1TextureProvider *provider = static_cast<GL1TextureProvider *>(texture.get_provider());
			selected_textures[unit_index].texture = provider;

			glEnable(provider->get_texture_type());
			glBindTexture(provider->get_texture_type(), provider->get_handle());
		}
	}

	void GL1GraphicContextProvider::reset_texture(int unit_index)
	{
		set_active();

		if ((unit_index >= 0) && (unit_index < max_texture_coords))
		{
			selected_textures[unit_index].texture = nullptr;
		}

		if (glActiveTexture != nullptr)
		{
			glActiveTexture(GL_TEXTURE0 + unit_index);
		}
		else if (unit_index > 0)
		{
			return;
		}

		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
		glDisable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void GL1GraphicContextProvider::set_image_texture(int unit_index, const Texture &texture)
	{
	}

	void GL1GraphicContextProvider::reset_image_texture(int unit_index)
	{
	}

	bool GL1GraphicContextProvider::is_frame_buffer_owner(const FrameBuffer &fb)
	{
		return true;
	}

	void GL1GraphicContextProvider::set_frame_buffer(const FrameBuffer &w_buffer, const FrameBuffer &r_buffer)
	{
		framebuffer_provider = dynamic_cast<GL1FrameBufferProvider *>(w_buffer.get_provider());
		framebuffer_provider->set_active();
		framebuffer_provider->set_state(selected_state);
		framebuffer_provider->start();

		framebuffer_bound = true;
	}

	void GL1GraphicContextProvider::reset_frame_buffer()
	{
		if (framebuffer_bound)
		{
			framebuffer_bound = false;

			framebuffer_provider->set_active();
			framebuffer_provider->stop();
			OpenGL::set_active(this);
			selected_state.apply();
		}
	}

	void GL1GraphicContextProvider::set_program_object(StandardProgram standard_program)
	{
	}

	ProgramObject GL1GraphicContextProvider::get_program_object(StandardProgram standard_program) const
	{
		return internal_program;
	}

	void GL1GraphicContextProvider::set_program_object(const ProgramObject &program)
	{
		throw Exception("GLSL program objects are not supported on OpenGL 1.3");
	}

	void GL1GraphicContextProvider::reset_program_object()
	{
	}

	bool GL1GraphicContextProvider::is_primitives_array_owner(const PrimitivesArray &primitives_array)
	{
		return true;
	}

	void GL1GraphicContextProvider::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &primitives_array)
	{
		set_primitives_array(primitives_array);
		draw_primitives_array(type, 0, num_vertices);
		reset_primitives_array();
	}

	void GL1GraphicContextProvider::set_primitives_array(const PrimitivesArray &primitives_array)
	{
		GL1PrimitivesArrayProvider * prim_array = static_cast<GL1PrimitivesArrayProvider *>(primitives_array.get_provider());
		if (prim_arrays_set)
			reset_primitives_array();
		set_active();
		prim_arrays_set = true;

		num_set_tex_arrays = 0;

		for (size_t attribute_index = 0; attribute_index < prim_array->attributes.size(); attribute_index++)
		{
			if (!prim_array->attribute_set[attribute_index])
				continue;

			const PrimitivesArrayProvider::VertexData &attribute = prim_array->attributes[attribute_index];

			GL1VertexArrayBufferProvider *vertex_array_ptr = static_cast<GL1VertexArrayBufferProvider *>(attribute.array_provider);
			if (!vertex_array_ptr)
				throw Exception("Invalid BindBuffer Provider");

			const char *data_ptr = ((const char *)vertex_array_ptr->get_data()) + attribute.offset;

			switch (attribute_index)
			{
			case 0: // POSITION
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(attribute.size, OpenGL::to_enum(attribute.type), attribute.stride, data_ptr);
				break;
			case 1: // COLOR
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(attribute.size, OpenGL::to_enum(attribute.type), attribute.stride, data_ptr);

				break;
			case 2: // TEXTURE
				primitives_array_texture = attribute;
				primitives_array_texture_set = true;
				break;
			case 3: // TEXINDEX
				primitives_array_texindex = attribute;
				primitives_array_texindex_set = true;
				break;
			case 4: // NORMAL
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(OpenGL::to_enum(attribute.type), attribute.stride, data_ptr);
				break;
			}
		}
	}

	void GL1GraphicContextProvider::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
	{
		set_active();

		// Simple condition - No textures set
		if (!primitives_array_texture_set)
		{
			glDrawArrays(OpenGL::to_enum(type), offset, num_vertices);
			return;
		}

		GLenum primitive_type = OpenGL::to_enum(type);

		int total_vertices = offset + num_vertices;

		// Normal condition - No texture index set
		if (!primitives_array_texindex_set)
		{
			set_primitive_texture(0, primitives_array_texture, offset, num_vertices, total_vertices);
			glDrawArrays(primitive_type, offset, num_vertices);
			reset_primitive_texture(0);
			return;
		}

		// For code simplicity, disable all textures
		reset_primitive_texture_all();

		GL1VertexArrayBufferProvider *vertex_array_ptr = static_cast<GL1VertexArrayBufferProvider *>(primitives_array_texindex.array_provider);
		if (!vertex_array_ptr)
			throw Exception("Invalid BindBuffer Provider");

		const char *data_ptr = ((const char *)vertex_array_ptr->get_data()) + primitives_array_texindex.offset;

		// Difficult condition - Draw all texture indexes
		while (num_vertices > 0)
		{
			// First, find these values...
			int texture_index;
			int num_vertices_in_group;

			if (primitives_array_texindex.size <= 0)
			{
				// Invalid texindex array (ignore)
				texture_index = 0;
				num_vertices_in_group = num_vertices;
			}
			// Multiple textures possible
			switch (primitives_array_texindex.type)
			{
			case VertexAttributeDataType::type_int:
			{
				int stride_float;
				if (primitives_array_texindex.stride)
				{
					stride_float = (primitives_array_texindex.stride / sizeof(int));
				}
				else
				{
					stride_float = primitives_array_texindex.size;
				}

				int *dptr = (int *)data_ptr;
				dptr += offset * stride_float;
				num_vertices_in_group = 1;
				texture_index = *dptr;
				dptr += stride_float;
				for (int texcnt = 1; texcnt < num_vertices; ++texcnt)
				{
					// Note, we assume all textures in "primitives_array_texindex.size" are identical
					if ((*dptr) != texture_index)
						break;
					dptr += stride_float;
					num_vertices_in_group++;
				}

				break;
			}
			default:
				throw Exception("Implement me!");
			}

			set_primitive_texture(texture_index, primitives_array_texture, offset, num_vertices_in_group, total_vertices);
			glDrawArrays(primitive_type, offset, num_vertices_in_group);
			reset_primitive_texture(texture_index);

			offset += num_vertices_in_group;
			num_vertices -= num_vertices_in_group;
		}
	}

	void GL1GraphicContextProvider::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
	{
		throw Exception("Cannot draw instanced for the OpenGL 1.3 target");
	}

	void GL1GraphicContextProvider::set_primitives_elements(ElementArrayBufferProvider *array_provider)
	{
		throw Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
	}

	void GL1GraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
	{
		throw Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
	}

	void GL1GraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		throw Exception("Cannot draw instanced for the OpenGL 1.3 target");
	}

	void GL1GraphicContextProvider::reset_primitives_elements()
	{
		throw Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
	}

	void GL1GraphicContextProvider::draw_primitives_elements(
		PrimitivesType type,
		int count,
		ElementArrayBufferProvider *array_provider,
		VertexAttributeDataType indices_type,
		void *offset)
	{
		throw Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
	}

	void GL1GraphicContextProvider::draw_primitives_elements_instanced(
		PrimitivesType type,
		int count,
		ElementArrayBufferProvider *array_provider,
		VertexAttributeDataType indices_type,
		void *offset,
		int instance_count)
	{
		throw Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
	}

	void GL1GraphicContextProvider::reset_primitives_array()
	{
		set_active();

		primitives_array_texture_set = false;
		primitives_array_texindex_set = false;

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_EDGE_FLAG_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (glClientActiveTexture)
		{
			for (int i = 0; i < num_set_tex_arrays; ++i)
			{
				glClientActiveTexture(GL_TEXTURE0 + i);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			num_set_tex_arrays = 0;
		}
		else
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		prim_arrays_set = false;
	}

	void GL1GraphicContextProvider::set_scissor(const Rect &rect)
	{
		set_active();

		if (!scissor_enabled)
			throw Exception("RasterizerState must be set with enable_scissor() for clipping to work");

		glEnable(GL_SCISSOR_TEST);
		glScissor(
			rect.left,
			rect.top,
			rect.get_width(),
			rect.get_height());
	}

	void GL1GraphicContextProvider::reset_scissor()
	{
		set_active();
		glDisable(GL_SCISSOR_TEST);
	}

	void GL1GraphicContextProvider::dispatch(int x, int y, int z)
	{
		throw Exception("Dispatch not supported by OpenGL 1!");
	}

	void GL1GraphicContextProvider::clear(const Colorf &color)
	{
		set_active();
		glClearColor(
			(GLclampf)color.r,
			(GLclampf)color.g,
			(GLclampf)color.b,
			(GLclampf)color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GL1GraphicContextProvider::clear_stencil(int value)
	{
		set_active();
		glClearStencil(value);
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	void GL1GraphicContextProvider::clear_depth(float value)
	{
		set_active();
		glClearDepth(value);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void GL1GraphicContextProvider::on_window_resized()
	{
		window_resized_signal(render_window->get_viewport().get_size());
	}

	void GL1GraphicContextProvider::set_viewport(const Rectf &viewport)
	{
		set_active();
		glViewport(
			GLsizei(viewport.left),
			GLsizei(viewport.top),
			GLsizei(viewport.right - viewport.left),
			GLsizei(viewport.bottom - viewport.top));
	}

	void GL1GraphicContextProvider::set_viewport(int index, const Rectf &viewport)
	{
		if (index == 0)
			set_viewport(viewport);
	}

	void GL1GraphicContextProvider::set_depth_range(float n, float f)
	{
		set_active();
		glDepthRange((float)n, (float)f);
	}

	void GL1GraphicContextProvider::set_depth_range(int viewport, float n, float f)
	{
		if (viewport == 0)
		{
			set_active();
			glDepthRange((float)n, (float)f);
		}
	}

	void GL1GraphicContextProvider::set_draw_buffer(DrawBuffer buffer)
	{
		set_active();

		if (!render_window->is_double_buffered())	// Silently fix incorrect render buffers
		{
			if (buffer == DrawBuffer::back)
				buffer = DrawBuffer::front;
		}

		glDrawBuffer(OpenGL::to_enum(buffer));

	}

	void GL1GraphicContextProvider::make_current() const
	{
		render_window->make_current();
	}

	void GL1GraphicContextProvider::flush()
	{
		set_active();
		glFlush();
	}

	const DisplayWindowProvider & GL1GraphicContextProvider::get_render_window() const
	{
		return *render_window;
	}

	OpenGLWindowProvider & GL1GraphicContextProvider::get_opengl_window()
	{
		return *render_window;
	}

	void GL1GraphicContextProvider::set_primitive_texture(int texture_index, PrimitivesArrayProvider::VertexData &array_texture, int offset, int num_vertices, int total_vertices)
	{
		GL1TextureProvider *texture;
		if ((texture_index < 0) || (texture_index >= max_texture_coords))
		{
			texture = nullptr;		// Ignore invalid texture index's
		}
		else
		{
			texture = selected_textures[texture_index].texture;
		}

		if (texture)
		{
			if (glActiveTexture != nullptr)
				glActiveTexture(GL_TEXTURE0 + texture_index);

			glEnable(texture->get_texture_type());
			glBindTexture(texture->get_texture_type(), texture->get_handle());

			if (glClientActiveTexture)
				glClientActiveTexture(GL_TEXTURE0 + texture_index);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			if (texture->is_power_of_two_texture() || (num_vertices == 0))
			{
				GL1VertexArrayBufferProvider *vertex_array_ptr = static_cast<GL1VertexArrayBufferProvider *>(array_texture.array_provider);
				if (!vertex_array_ptr)
					throw Exception("Invalid BindBuffer Provider");

				const char *data_ptr = ((const char *)vertex_array_ptr->get_data()) + array_texture.offset;

				glTexCoordPointer(array_texture.size, OpenGL::to_enum(array_texture.type), array_texture.stride, data_ptr);
			}
			else
			{
				// A hack to handle non-power-of-two textures
				texture->transform_coordinate(array_texture, transformed_coords, offset, num_vertices, total_vertices);
				glTexCoordPointer(array_texture.size, GL_FLOAT, 0, &transformed_coords[0]);
			}
		}
		else
		{
			//Disabled this exception because the sprite render batcher does this intentionally
			//throw Exception("Attempt to draw a texture that was not selected in the GL1 target");
		}

	}

	void GL1GraphicContextProvider::reset_primitive_texture(int texture_index)
	{
		if ((texture_index < 0) || (texture_index >= max_texture_coords))
		{
			return;
		}

		GL1TextureProvider *texture = selected_textures[texture_index].texture;
		if (texture)
		{
			if (glActiveTexture != nullptr)
				glActiveTexture(GL_TEXTURE0 + texture_index);

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}

	}

	void GL1GraphicContextProvider::reset_primitive_texture_all()
	{
		for (int cnt = 0; cnt < max_texture_coords; cnt++)
		{
			GL1TextureProvider *texture = selected_textures[cnt].texture;
			if (texture)
			{
				if (glActiveTexture != nullptr)
					glActiveTexture(GL_TEXTURE0 + cnt);

				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);
			}
		}
	}
}
#endif

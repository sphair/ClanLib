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
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/element_array_buffer.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Math/angle.h"
#include "primitives_array_impl.h"
#include "graphic_context_impl.h"
#include "API/Display/Render/shared_gc_data.h"


namespace clan
{
	GraphicContext::GraphicContext()
	{
	}

	GraphicContext::GraphicContext(GraphicContextProvider *provider)
		: impl(std::make_shared<GraphicContext_Impl>(provider))
	{
		reset_rasterizer_state();
		reset_blend_state();
		reset_depth_stencil_state();

	}

	GraphicContext GraphicContext::create() const
	{
		GraphicContext gc;
		gc.impl = std::make_shared<GraphicContext_Impl>(impl.get(), false);

		return gc;
	}

	GraphicContext GraphicContext::create(FrameBuffer &buffer) const
	{
		GraphicContext gc;
		gc.impl = std::make_shared<GraphicContext_Impl>(impl.get(), false);

		gc.set_frame_buffer(buffer);
		gc.set_viewport(gc.get_size());

		return gc;
	}


	GraphicContext GraphicContext::clone() const
	{
		GraphicContext gc;
		gc.impl = std::make_shared<GraphicContext_Impl>(impl.get(), true);

		return gc;
	}

	GraphicContext::~GraphicContext()
	{
	}

	void GraphicContext::throw_if_null() const
	{
		if (!impl)
			throw Exception("GraphicContext is null");
	}

	ClipZRange GraphicContext::get_clip_z_range() const
	{
		return get_provider()->get_clip_z_range();
	}

	TextureImageYAxis GraphicContext::get_texture_image_y_axis() const
	{
		return get_provider()->get_texture_image_y_axis();
	}

	ShaderLanguage GraphicContext::get_shader_language() const
	{
		return get_provider()->get_shader_language();
	}

	int GraphicContext::get_major_version() const
	{
		return get_provider()->get_major_version();
	}

	int GraphicContext::get_minor_version() const
	{
		return get_provider()->get_minor_version();
	}

	bool GraphicContext::has_compute_shader_support() const
	{
		return get_provider()->has_compute_shader_support();
	}

	Texture GraphicContext::get_texture(int unit) const
	{
		if ((unit < 0) || (unit >= impl->textures.size()))
			return Texture();
		else
			return impl->textures[unit];
	}

	std::vector<Texture> GraphicContext::get_textures() const
	{
		return impl->textures;
	}

	FrameBuffer GraphicContext::get_write_frame_buffer() const
	{
		return impl->write_frame_buffer;
	}

	FrameBuffer GraphicContext::get_read_frame_buffer() const
	{
		return impl->read_frame_buffer;
	}

	int GraphicContext::get_width() const
	{
		return impl->get_size().width;
	}

	int GraphicContext::get_height() const
	{
		return impl->get_size().height;
	}

	Size GraphicContext::get_size() const
	{
		return impl->get_size();
	}

	float GraphicContext::get_pixel_ratio() const
	{
		if (!impl->write_frame_buffer.is_null())
		{
			float ratio = impl->write_frame_buffer.get_pixel_ratio();
			if (ratio != 0.0f)
				return ratio;
			return 1.0f;
		}
		return impl->graphic_screen->get_provider()->get_pixel_ratio();
	}

	Size GraphicContext::get_max_texture_size() const
	{
		return impl->graphic_screen->get_provider()->get_max_texture_size();
	}

	GraphicContext::operator const GraphicContextProvider* () const { return get_provider(); }
	GraphicContext::operator GraphicContextProvider* () { return get_provider(); }

	GraphicContextProvider *GraphicContext::get_provider()
	{
		if (impl)
			return impl->graphic_screen->get_provider();
		else
			return nullptr;
	}

	const GraphicContextProvider * GraphicContext::get_provider() const
	{
		if (impl)
			return impl->graphic_screen->get_provider();
		else
			return nullptr;
	}

	PixelBuffer GraphicContext::get_pixeldata(const Rect &rect2, TextureFormat texture_format, bool clamp)
	{
		Rect rect = rect2;
		if (rect == Rect())
		{
			rect = Rect(0, 0, get_size());
		}
		else
		{
			Size size = get_size();;
			if ((rect.left < 0) || (rect.top < 0) || (rect.right > size.width) || (rect.bottom > size.height))
				throw Exception("Specified rect exceeds pixel data size");
		}

		return get_provider()->get_pixeldata(rect, texture_format, clamp);
	}

	PixelBuffer GraphicContext::get_pixeldata(TextureFormat texture_format, bool clamp)
	{
		return get_provider()->get_pixeldata(Rect(0, 0, get_size()), texture_format, clamp);
	}

	void GraphicContext::set_uniform_buffer(int index, const UniformBuffer &buffer)
	{
		impl->set_uniform_buffer(index, buffer);
	}

	void GraphicContext::reset_uniform_buffer(int index)
	{
		UniformBuffer null_buffer;
		impl->set_uniform_buffer(index, null_buffer);
	}

	void GraphicContext::set_storage_buffer(int index, const StorageBuffer &buffer)
	{
		impl->set_storage_buffer(index, buffer);
	}

	void GraphicContext::reset_storage_buffer(int index)
	{
		StorageBuffer null_buffer;
		impl->set_storage_buffer(index, null_buffer);
	}

	void GraphicContext::set_texture(int unit_index, const Texture &texture)
	{
		impl->set_texture(unit_index, texture);
	}

	void GraphicContext::set_textures(std::vector<Texture> &textures)
	{
		impl->set_textures(textures);
	}

	void GraphicContext::reset_texture(int unit_index)
	{
		Texture null_texture;
		impl->set_texture(unit_index, null_texture);
	}

	void GraphicContext::reset_textures()
	{
		Texture null_texture;
		std::vector<Texture> null_textures;
		impl->set_textures(null_textures);
	}

	void GraphicContext::set_image_texture(int unit_index, const Texture &texture)
	{
		impl->set_image_texture(unit_index, texture);
	}

	void GraphicContext::set_image_texture(std::vector<Texture> &textures)
	{
		impl->set_image_textures(textures);
	}

	void GraphicContext::reset_image_texture(int unit_index)
	{
		Texture null_texture;
		impl->set_image_texture(unit_index, null_texture);
	}

	void GraphicContext::reset_image_textures()
	{
		Texture null_texture;
		std::vector<Texture> null_textures;
		impl->set_image_textures(null_textures);
	}

	void GraphicContext::set_rasterizer_state(const RasterizerState &state)
	{
		impl->set_rasterizer_state(state);
	}

	void GraphicContext::set_blend_state(const BlendState &state, const Colorf &blend_color, unsigned int sample_mask)
	{
		impl->set_blend_state(state, blend_color, sample_mask);
	}

	void GraphicContext::set_depth_stencil_state(const DepthStencilState &state, int stencil_ref)
	{
		impl->set_depth_stencil_state(state, stencil_ref);
	}

	void GraphicContext::reset_rasterizer_state()
	{
		set_rasterizer_state(impl->graphic_screen->default_rasterizer_state);
	}

	void GraphicContext::reset_blend_state()
	{
		set_blend_state(impl->graphic_screen->default_blend_state);
	}

	void GraphicContext::reset_depth_stencil_state()
	{
		set_depth_stencil_state(impl->graphic_screen->default_depth_stencil_state);
	}

	ProgramObject GraphicContext::get_program_object() const
	{
		return impl->get_program_object();
	}

	void GraphicContext::set_program_object(StandardProgram standard_program)
	{
		impl->set_program_object(standard_program);
	}

	void GraphicContext::set_program_object(const ProgramObject &program)
	{
		impl->set_program_object(program);
	}

	void GraphicContext::reset_program_object()
	{
		impl->reset_program_object();
	}

	bool GraphicContext::is_primitives_array_owner(const PrimitivesArray &primitives_array)
	{
		return get_provider()->is_primitives_array_owner(primitives_array);
	}

	void GraphicContext::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &prim_array)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->draw_primitives(type, num_vertices, prim_array);
	}

	void GraphicContext::set_primitives_array(const PrimitivesArray &prim_array)
	{
		get_provider()->set_primitives_array(prim_array);
	}

	void GraphicContext::draw_primitives_array(PrimitivesType type, int num_vertices)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->draw_primitives_array(type, 0, num_vertices);
	}

	void GraphicContext::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->draw_primitives_array(type, offset, num_vertices);
	}

	void GraphicContext::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->draw_primitives_array_instanced(type, offset, num_vertices, instance_count);
	}

	void GraphicContext::set_primitives_elements(ElementArrayBuffer &element_array)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->set_primitives_elements(element_array.get_provider());
	}

	void GraphicContext::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->draw_primitives_elements(type, count, indices_type, offset);
	}

	void GraphicContext::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->draw_primitives_elements_instanced(type, count, indices_type, offset, instance_count);
	}

	void GraphicContext::reset_primitives_elements()
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->reset_primitives_elements();
	}

	void GraphicContext::draw_primitives_elements(PrimitivesType type, int count, ElementArrayBuffer &elements_array, VertexAttributeDataType indices_type, size_t offset)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->draw_primitives_elements(type, count, elements_array.get_provider(), indices_type, (void*)offset);
	}

	void GraphicContext::draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBuffer &elements_array, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->draw_primitives_elements_instanced(type, count, elements_array.get_provider(), indices_type, (void*)offset, instance_count);
	}

	void GraphicContext::reset_primitives_array()
	{
		get_provider()->reset_primitives_array();
	}

	void GraphicContext::dispatch(int x, int y, int z)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->dispatch(x, y, z);
	}

	void GraphicContext::clear(const Colorf &color)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->clear(color);
	}

	void GraphicContext::clear_stencil(int value)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->clear_stencil(value);
	}

	void GraphicContext::clear_depth(float value)
	{
		impl->graphic_screen->set_active(impl.get());
		get_provider()->clear_depth(value);
	}

	void GraphicContext::set_scissor(const Rect &rect, TextureImageYAxis y_axis)
	{
		Rect rect2 = rect;
		if (get_texture_image_y_axis() != y_axis)
		{
			rect2 = Rect(Point(rect.left, get_size().height - rect.bottom), rect.get_size());
		}

		impl->set_scissor(rect2);
	}

	void GraphicContext::reset_scissor()
	{
		impl->reset_scissor();
	}

	bool GraphicContext::is_frame_buffer_owner(const FrameBuffer &fb)
	{
		return get_provider()->is_frame_buffer_owner(fb);
	}

	void GraphicContext::set_frame_buffer(const FrameBuffer &write_buffer)
	{
		set_frame_buffer(write_buffer, write_buffer);
	}

	void GraphicContext::set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer)
	{
		impl->set_frame_buffer(write_buffer, read_buffer);
	}

	void GraphicContext::reset_frame_buffer()
	{
		FrameBuffer null_fb = FrameBuffer();
		impl->set_frame_buffer(null_fb, null_fb);
	}

	void GraphicContext::set_viewport(const Rectf &viewport)
	{
		if (get_texture_image_y_axis() == TextureImageYAxis::y_top_down)
		{
			impl->set_viewport(-1, viewport);
		}
		else
		{
			Rectf viewport_flipped(Pointf(viewport.left, get_size().height - viewport.bottom), viewport.get_size());
			impl->set_viewport(-1, viewport_flipped);
		}
	}

	void  GraphicContext::set_viewport(int index, const Rectf &viewport)
	{
		if (get_texture_image_y_axis() == TextureImageYAxis::y_top_down)
		{
			impl->set_viewport(index, viewport);
		}
		else
		{
			Rectf viewport_flipped(Pointf(viewport.left, get_size().height - viewport.bottom), viewport.get_size());
			impl->set_viewport(index, viewport_flipped);
		}
	}

	void GraphicContext::set_depth_range(float n, float f)
	{
		impl->set_depth_range(-1, n, f);
	}

	void GraphicContext::set_depth_range(int viewport, float n, float f)
	{
		impl->set_depth_range(viewport, n, f);
	}

	void GraphicContext::set_draw_buffer(DrawBuffer buffer)
	{
		impl->set_draw_buffer(buffer);
	}

	void GraphicContext::flush()
	{
		impl->flush();
	}
}

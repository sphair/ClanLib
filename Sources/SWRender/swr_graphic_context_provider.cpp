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
*/

#include "SWRender/precomp.h"
#include "swr_graphic_context_provider.h"
#include "swr_display_window_provider.h"
#include "swr_texture_provider.h"
#include "swr_element_array_buffer_provider.h"
#include "swr_pixel_buffer_provider.h"
#include "swr_frame_buffer_provider.h"
#include "swr_occlusion_query_provider.h"
#include "swr_program_object_provider.h"
#include "swr_render_buffer_provider.h"
#include "swr_shader_object_provider.h"
#include "swr_vertex_array_buffer_provider.h"
#include "Canvas/pixel_canvas.h"
#include "API/SWRender/pixel_command.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/blend_state.h"
#include "API/SWRender/swr_program_object.h"
#include "API/Display/Render/shared_gc_data.h"
#include "swr_primitives_array_provider.h"
#include "swr_uniform_buffer_provider.h"
#include "swr_transfer_buffer_provider.h"
namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderGraphicContextProvider Construction:

SWRenderGraphicContextProvider::SWRenderGraphicContextProvider(SWRenderDisplayWindowProvider *window)
: window(window), current_program_provider(0), is_sprite_program(false)
{
	canvas.reset(new PixelCanvas(window->get_viewport().get_size()));
	program_object_standard = ProgramObject_SWRender(&cl_software_program_standard, false);
	program_object_standard.bind_attribute_location(0, "Position");
	program_object_standard.bind_attribute_location(1, "Color0");
	program_object_standard.bind_attribute_location(2, "TexCoord0");
	program_object_standard.bind_attribute_location(3, "TexIndex0");
	set_program_object(program_single_texture);
	SharedGCData::add_provider(this);

}

SWRenderGraphicContextProvider::~SWRenderGraphicContextProvider()
{
	dispose();
	canvas.reset();
}

void SWRenderGraphicContextProvider::on_dispose()
{
	canvas.reset();
	SharedGCData::remove_provider(this);
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderGraphicContextProvider Attributes:

int SWRenderGraphicContextProvider::get_max_attributes()
{
	return num_attribute_fetchers;
}

Size SWRenderGraphicContextProvider::get_max_texture_size() const
{
	return Size(8*1024, 8*1024);
}

Size SWRenderGraphicContextProvider::get_display_window_size() const
{
	return Size(canvas->get_width(), canvas->get_height());
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderGraphicContextProvider Operations:

PixelBuffer SWRenderGraphicContextProvider::get_pixeldata(const Rect& rect, TextureFormat pixel_format, bool clamp) const
{
	return canvas->to_pixelbuffer().copy(rect).to_format(pixel_format);
}

TextureProvider *SWRenderGraphicContextProvider::alloc_texture(TextureDimensions texture_dimensions)
{
	return new SWRenderTextureProvider;
}

OcclusionQueryProvider *SWRenderGraphicContextProvider::alloc_occlusion_query()
{
	return new SWRenderOcclusionQueryProvider;
}

ProgramObjectProvider *SWRenderGraphicContextProvider::alloc_program_object()
{
	return new SWRenderProgramObjectProvider;
}

ShaderObjectProvider *SWRenderGraphicContextProvider::alloc_shader_object()
{
	return new SWRenderShaderObjectProvider;
}

FrameBufferProvider *SWRenderGraphicContextProvider::alloc_frame_buffer()
{
	return new SWRenderFrameBufferProvider;
}

RenderBufferProvider *SWRenderGraphicContextProvider::alloc_render_buffer()
{
	return new SWRenderRenderBufferProvider;
}

VertexArrayBufferProvider *SWRenderGraphicContextProvider::alloc_vertex_array_buffer()
{
	return new SWRenderVertexArrayBufferProvider;
}

UniformBufferProvider *SWRenderGraphicContextProvider::alloc_uniform_buffer()
{
	return new SWRenderUniformBufferProvider;
}

ElementArrayBufferProvider *SWRenderGraphicContextProvider::alloc_element_array_buffer()
{
	return new SWRenderElementArrayBufferProvider;
}

PixelBufferProvider *SWRenderGraphicContextProvider::alloc_pixel_buffer()
{
	return new SWRenderPixelBufferProvider;
}

TransferBufferProvider *SWRenderGraphicContextProvider::alloc_transfer_buffer()
{
	return new SWRenderTransferBufferProvider;
}

PrimitivesArrayProvider *SWRenderGraphicContextProvider::alloc_primitives_array()
{
	return new SWRenderPrimitivesArrayProvider();
}

std::shared_ptr<RasterizerStateProvider> SWRenderGraphicContextProvider::create_rasterizer_state(const RasterizerStateDescription &desc)
{
	std::map<RasterizerStateDescription, std::shared_ptr<RasterizerStateProvider> >::iterator it = rasterizer_states.find(desc);
	if (it != rasterizer_states.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<RasterizerStateProvider> state(new SWRenderRasterizerStateProvider(desc));
		rasterizer_states[desc.clone()] = state;
		return state;
	}
}

std::shared_ptr<BlendStateProvider> SWRenderGraphicContextProvider::create_blend_state(const BlendStateDescription &desc)
{
	std::map<BlendStateDescription, std::shared_ptr<BlendStateProvider> >::iterator it = blend_states.find(desc);
	if (it != blend_states.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<BlendStateProvider> state(new SWRenderBlendStateProvider(desc));
		blend_states[desc.clone()] = state;
		return state;
	}
}

std::shared_ptr<DepthStencilStateProvider> SWRenderGraphicContextProvider::create_depth_stencil_state(const DepthStencilStateDescription &desc)
{
	std::map<DepthStencilStateDescription, std::shared_ptr<DepthStencilStateProvider> >::iterator it = depth_stencil_states.find(desc);
	if (it != depth_stencil_states.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<DepthStencilStateProvider> state(new SWRenderDepthStencilStateProvider(desc));
		depth_stencil_states[desc.clone()] = state;
		return state;
	}
}

void SWRenderGraphicContextProvider::set_rasterizer_state(RasterizerStateProvider *state)
{
	if (state)
	{
		SWRenderRasterizerStateProvider *swr_state = static_cast<SWRenderRasterizerStateProvider*>(state);

/*		set_culled(swr_state->desc.get_culled());
		enable_line_antialiasing(swr_state->desc.get_enable_line_antialiasing());
		set_face_cull_mode(swr_state->desc.get_face_cull_mode());
		set_face_fill_mode(swr_state->desc.get_face_fill_mode());
		set_front_face(swr_state->desc.get_front_face());
		scissor_enabled = swr_state->desc.get_enable_scissor();
*/
	}
}

void SWRenderGraphicContextProvider::set_blend_state(BlendStateProvider *state, const Vec4f &blend_color, unsigned int sample_mask)
{
	if (state)
	{
		SWRenderBlendStateProvider *swr_state = static_cast<SWRenderBlendStateProvider*>(state);

		bool red, green, blue, alpha;
		BlendEquation equation_color, equation_alpha;
		BlendFunc src, dest, src_alpha, dest_alpha;
		swr_state->desc.get_color_write(red, green, blue, alpha);
		swr_state->desc.get_blend_equation(equation_color, equation_alpha);
		swr_state->desc.get_blend_function(src, dest, src_alpha, dest_alpha);

		if (swr_state->desc.is_blending_enabled())
		{
			canvas->set_blend_function(src, dest, src_alpha, dest_alpha, Colorf(blend_color));
		}
		else
		{
			canvas->set_blend_function(blend_one, blend_zero, blend_one, blend_zero, Colorf::black);
		}
	}
}

void SWRenderGraphicContextProvider::set_depth_stencil_state(DepthStencilStateProvider *state, int stencil_ref)
{
	if (state)
	{
		SWRenderDepthStencilStateProvider *swr_state = static_cast<SWRenderDepthStencilStateProvider*>(state);

		CompareFunction front; int front_ref; int front_mask;
		CompareFunction back; int back_ref; int back_mask;
		unsigned char front_facing_mask, back_facing_mask;
		StencilOp fail_front, pass_depth_fail_front, pass_depth_pass_front;
		StencilOp fail_back, pass_depth_fail_back, pass_depth_pass_back;
		swr_state->desc.get_stencil_compare_front(front, front_ref, front_mask);
		swr_state->desc.get_stencil_compare_back(back, back_ref, back_mask);
		swr_state->desc.get_stencil_write_mask(front_facing_mask, back_facing_mask);
		swr_state->desc.get_stencil_op_front(fail_front, pass_depth_fail_front, pass_depth_pass_front);
		swr_state->desc.get_stencil_op_back(fail_back, pass_depth_fail_back, pass_depth_pass_back);

	/*	enable_stencil_test(swr_state->desc.is_stencil_test_enabled());
		set_stencil_compare_front(front, front_ref, front_mask);
		set_stencil_compare_back(back, back_ref, back_mask);
		set_stencil_write_mask(front_facing_mask, back_facing_mask);
		set_stencil_op_front(fail_front, pass_depth_fail_front, pass_depth_pass_front);
		set_stencil_op_back(fail_back, pass_depth_fail_back, pass_depth_pass_back);
		enable_depth_test(swr_state->desc.is_depth_test_enabled());
		enable_depth_write(swr_state->desc.is_depth_write_enabled());
		set_depth_compare_function(swr_state->desc.get_depth_compare_function());
	*/
	}
}

void SWRenderGraphicContextProvider::set_program_object(StandardProgram standard_program)
{
	set_program_object(program_object_standard);
	is_sprite_program = (standard_program == program_sprite);
}

void SWRenderGraphicContextProvider::set_program_object(const ProgramObject &program)
{
	ProgramObject_SWRender swr_program(program);
	is_sprite_program = swr_program.is_sprite_program();
	current_program_provider = static_cast<SWRenderProgramObjectProvider *>(swr_program.get_provider());
}

void SWRenderGraphicContextProvider::reset_program_object()
{
	set_program_object(program_sprite);
}

void SWRenderGraphicContextProvider::set_texture(int unit_index, const Texture &texture)
{
	bound_textures[unit_index] = texture;
	canvas->set_sampler(unit_index, dynamic_cast<SWRenderTextureProvider*>(bound_textures[unit_index].get_provider())->get_image());
}

void SWRenderGraphicContextProvider::reset_texture(int unit_index)
{
	std::map<int, Texture>::iterator it = bound_textures.find(unit_index);
	if (it != bound_textures.end())
		bound_textures.erase(it);
	canvas->reset_sampler(unit_index);
}

void SWRenderGraphicContextProvider::set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer)
{
	canvas->set_framebuffer(write_buffer);
}

void SWRenderGraphicContextProvider::reset_frame_buffer()
{
	canvas->reset_framebuffer();
}

void SWRenderGraphicContextProvider::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &primitives_array)
{
	set_primitives_array(primitives_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void SWRenderGraphicContextProvider::set_primitives_array(const PrimitivesArray &primitives_array)
{
	current_prim_array = primitives_array;

	for (int i = 0; i < num_attribute_fetchers; i++)
		attribute_fetchers[i].bind(current_prim_array, i);
}

void SWRenderGraphicContextProvider::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
{
	if (type == type_triangles)
	{
		int end_vertices = offset+num_vertices;

		if (is_sprite_program)
		{
			for (int i = offset; i+2 < end_vertices; i+=6)
				draw_sprite(i+0, i+1, i+2);
		}
		else
		{
			for (int i = offset; i+2 < end_vertices; i+=3)
				draw_triangle(i+0, i+1, i+2);
		}
	}
	else if (type == type_lines)
	{
		int end_vertices = offset+num_vertices;

		for (int i = offset; i+1 < end_vertices; i+=2)
			draw_line(i+0, i+1);
	}
	else if (type == type_line_loop)
	{
		int end_vertices = offset+num_vertices;
		int i;
		
		for (i = offset; i < end_vertices-1; i++)
		{
			draw_line(i, i+1);
		}
		draw_line(i, offset);
	}
}

void SWRenderGraphicContextProvider::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
{
}
void SWRenderGraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
{
}

void SWRenderGraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
{

}

void SWRenderGraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset)
{
}

void SWRenderGraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset, int instance_count)
{
}

void SWRenderGraphicContextProvider::reset_primitives_array()
{
	for (int i = 0; i < num_attribute_fetchers; i++)
		attribute_fetchers[i].clear();

	current_prim_array = PrimitivesArray();
}

void SWRenderGraphicContextProvider::draw_pixels(GraphicContext &gc, float x, float y, float zoom_x, float zoom_y, const PixelBuffer &pixel_buffer, const Rect &src_rect, const Colorf &color)
{
	Vec4f pos(x, y, 1.0, 1.0);
	Vec2f screen_pos = cl_software_program_standard.transform(pos);

	Rect dest(Point((int)screen_pos.x,(int)screen_pos.y),Size((int)(src_rect.get_width()*zoom_x),(int)(src_rect.get_height()*zoom_y)));
	canvas->draw_pixels(dest, pixel_buffer, src_rect, color);
}

void SWRenderGraphicContextProvider::draw_pixels_bicubic(float x, float y, int zoom_number, int zoom_denominator, const PixelBuffer &pixels)
{
	Vec4f pos(x, y, 1.0, 1.0);
	Vec2f screen_pos = cl_software_program_standard.transform(pos);
	canvas->draw_pixels_bicubic((int)screen_pos.x,(int)screen_pos.y, zoom_number, zoom_denominator, pixels);
}

void SWRenderGraphicContextProvider::queue_command(std::unique_ptr<PixelCommand> &command)
{
	canvas->queue_command(command);
}

void SWRenderGraphicContextProvider::set_scissor(const Rect &rect)
{
	canvas->set_clip_rect(rect);
}

void SWRenderGraphicContextProvider::reset_scissor()
{
	canvas->reset_clip_rect();
}

void SWRenderGraphicContextProvider::clear(const Colorf &color)
{
	canvas->clear(color);
}

void SWRenderGraphicContextProvider::clear_depth(float value)
{
}

void SWRenderGraphicContextProvider::clear_stencil(int value)
{
}

void SWRenderGraphicContextProvider::set_viewport(const Rectf &viewport)
{
}

void SWRenderGraphicContextProvider::on_window_resized()
{
	canvas->resize(window->get_viewport().get_size());
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderGraphicContextProvider Implementation:

void SWRenderGraphicContextProvider::draw_triangle(int index1, int index2, int index3)
{
	int indexes[3] = { index1, index2, index3 };
	const std::vector<int> &bind_locations = current_program_provider->get_bind_locations();
	const std::vector<Vec4f> &attribute_defaults = current_program_provider->get_attribute_defaults();
	std::vector<Vec4f> &current_attribute_values = current_program_provider->get_current_attribute_values();

	for (size_t i = 0; i < bind_locations.size(); i++)
		attribute_fetchers[bind_locations[i]]->fetch(&current_attribute_values[i*3], indexes, 3, attribute_defaults[i]);

	std::unique_ptr<PixelCommand> command(current_program_provider->get_program()->draw_triangle(canvas->get_pipeline(), current_attribute_values));
	if (command.get())
		canvas->queue_command(command);
}

void SWRenderGraphicContextProvider::draw_sprite(int index1, int index2, int index3)
{
	int indexes[3] = { index1, index2, index3 };
	const std::vector<int> &bind_locations = current_program_provider->get_bind_locations();
	const std::vector<Vec4f> &attribute_defaults = current_program_provider->get_attribute_defaults();
	std::vector<Vec4f> &current_attribute_values = current_program_provider->get_current_attribute_values();

	for (size_t i = 0; i < bind_locations.size(); i++)
		attribute_fetchers[bind_locations[i]]->fetch(&current_attribute_values[i*3], indexes, 3, attribute_defaults[i]);

	std::unique_ptr<PixelCommand> command(current_program_provider->get_program()->draw_sprite(canvas->get_pipeline(), current_attribute_values));
	if (command.get())
		canvas->queue_command(command);
}

void SWRenderGraphicContextProvider::draw_line(int index1, int index2)
{
	int indexes[2] = { index1, index2 };
	const std::vector<int> &bind_locations = current_program_provider->get_bind_locations();
	const std::vector<Vec4f> &attribute_defaults = current_program_provider->get_attribute_defaults();
	std::vector<Vec4f> &current_attribute_values = current_program_provider->get_current_attribute_values();

	for (size_t i = 0; i < bind_locations.size(); i++)
		attribute_fetchers[bind_locations[i]]->fetch(&current_attribute_values[i*2], indexes, 2, attribute_defaults[i]);

	std::unique_ptr<PixelCommand> command(current_program_provider->get_program()->draw_line(canvas->get_pipeline(), current_attribute_values));
	if (command.get())
		canvas->queue_command(command);
}
ProgramObject SWRenderGraphicContextProvider::get_program_object(StandardProgram standard_program) const
{
	//FIXME: Stub
	throw Exception("Stub");
}
StorageBufferProvider *SWRenderGraphicContextProvider::alloc_storage_buffer()
{
	//FIXME: Stub
	throw Exception("Stub");
}
void SWRenderGraphicContextProvider::set_uniform_buffer(int index, const UniformBuffer &buffer)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::reset_uniform_buffer(int index)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_storage_buffer(int index, const StorageBuffer &buffer)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::reset_storage_buffer(int index)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_image_texture(int unit_index, const Texture &texture)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::reset_image_texture(int unit_index)
{
	//FIXME: Stub
}
bool SWRenderGraphicContextProvider::is_frame_buffer_owner(const FrameBuffer &fb)
{
	//FIXME: Stub
	return true;
}
void SWRenderGraphicContextProvider::set_point_size(float value)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_point_fade_treshold_size(float value)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_line_width(float value)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::enable_vertex_program_point_size(bool enabled)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_point_sprite_origin(PointSpriteOrigin origin)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_antialiased(bool value)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_point_offset(bool value)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_line_offset(bool value)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_polygon_offset(bool value)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_offset_factor(float value)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_offset_units(float value)
{
	//FIXME: Stub
}
bool SWRenderGraphicContextProvider::is_primitives_array_owner(const PrimitivesArray &primitives_array)
{
	//FIXME: Stub
	return true;
}
void SWRenderGraphicContextProvider::set_primitives_elements(ElementArrayBufferProvider *array_provider)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::reset_primitives_elements()
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::dispatch(int x, int y, int z)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_depth_range(float n, float f)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_depth_range(int viewport, float n, float f)
{
	//FIXME: Stub
}

void SWRenderGraphicContextProvider::enable_logic_op(bool enabled)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_logic_op(LogicOp op)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_draw_buffer(DrawBuffer buffer)
{
	//FIXME: Stub
}
void SWRenderGraphicContextProvider::set_viewport(int index, const Rectf &viewport)
{
	//FIXME: Stub
}

}

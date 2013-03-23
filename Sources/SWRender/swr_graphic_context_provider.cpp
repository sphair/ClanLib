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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderGraphicContextProvider Construction:

SWRenderGraphicContextProvider::SWRenderGraphicContextProvider(SWRenderDisplayWindowProvider *window)
: window(window), current_prim_array(0), modelview_matrix(Mat4f::identity()), current_program_provider(0), is_sprite_program(false)
{
	canvas.reset(new PixelCanvas(window->get_viewport().get_size()));
	program_object_standard = ProgramObject_SWRender(&cl_software_program_standard, false);
	program_object_standard.bind_attribute_location(0, "Position");
	program_object_standard.bind_attribute_location(1, "Color0");
	program_object_standard.bind_attribute_location(2, "TexCoord0");
	program_object_standard.bind_attribute_location(3, "TexIndex0");
	set_program_object(cl_program_single_texture);
}

SWRenderGraphicContextProvider::~SWRenderGraphicContextProvider()
{
	canvas.reset();
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

int SWRenderGraphicContextProvider::get_width() const
{
	return canvas->get_width();
}

int SWRenderGraphicContextProvider::get_height() const
{
	return canvas->get_height();
}

#ifdef WIN32
HDC SWRenderGraphicContextProvider::get_drawable() const
{
	return 0; //static_cast<SWRenderDisplayWindowProvider *>(window)->get_device_context();
}
#endif

/////////////////////////////////////////////////////////////////////////////
// SWRenderGraphicContextProvider Operations:

void SWRenderGraphicContextProvider::destroy()
{
	delete this;
}

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

ElementArrayBufferProvider *SWRenderGraphicContextProvider::alloc_element_array_buffer()
{
	return new SWRenderElementArrayBufferProvider;
}

PixelBufferProvider *SWRenderGraphicContextProvider::alloc_pixel_buffer()
{
	return new SWRenderPixelBufferProvider;
}

void SWRenderGraphicContextProvider::set_program_object(StandardProgram standard_program)
{
	set_program_object(program_object_standard, cl_program_matrix_all_standard);
	is_sprite_program = (standard_program == cl_program_sprite);
}

void SWRenderGraphicContextProvider::set_program_object(const ProgramObject &program, int program_matrix_flags)
{
	ProgramObject_SWRender swr_program(program);
	is_sprite_program = swr_program.is_sprite_program();
	current_program_provider = static_cast<SWRenderProgramObjectProvider *>(swr_program.get_provider());
	if (program_matrix_flags & cl_program_matrix_modelview)
		current_program_provider->get_program()->set_uniform_matrix("cl_ModelView", modelview_matrix);
}

void SWRenderGraphicContextProvider::reset_program_object()
{
	set_program_object(cl_program_sprite);
}

void SWRenderGraphicContextProvider::set_texture(int unit_index, const Texture &texture)
{
	bound_textures[unit_index] = texture;
	canvas->set_sampler(unit_index, dynamic_cast<SWRenderTextureProvider*>(bound_textures[unit_index].get_provider())->get_image());
}

void SWRenderGraphicContextProvider::reset_texture(int unit_index, const Texture &texture)
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

void SWRenderGraphicContextProvider::set_blend_mode(const BlendMode &blendmode)
{
	if (blendmode.is_blend_enabled())
	{
		canvas->set_blend_function(
			blendmode.get_blend_function_src(),
			blendmode.get_blend_function_dest(),
			blendmode.get_blend_function_src_alpha(),
			blendmode.get_blend_function_dest_alpha(),
			blendmode.get_blend_color());
	}
	else
	{
		canvas->set_blend_function(cl_blend_one, cl_blend_zero, cl_blend_one, cl_blend_zero, Colorf::black);
	}
}

void SWRenderGraphicContextProvider::set_buffer_control(const BufferControl &buffer_control)
{
}

void SWRenderGraphicContextProvider::set_pen(const Pen &pen)
{
}

void SWRenderGraphicContextProvider::set_polygon_rasterizer(const PolygonRasterizer &raster)
{
}

void SWRenderGraphicContextProvider::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayData * const prim_array)
{
	set_primitives_array(prim_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void SWRenderGraphicContextProvider::set_primitives_array(const PrimitivesArrayData * const prim_array)
{
	current_prim_array = prim_array;

	for (int i = 0; i < num_attribute_fetchers; i++)
		attribute_fetchers[i].bind(current_prim_array, i);
}

void SWRenderGraphicContextProvider::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
{
	if (type == cl_triangles)
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
	else if (type == cl_lines)
	{
		int end_vertices = offset+num_vertices;

		for (int i = offset; i+1 < end_vertices; i+=2)
			draw_line(i+0, i+1);
	}
	else if (type == cl_line_loop)
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

void SWRenderGraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, unsigned int *indices)
{
	if (type == cl_triangles)
	{
		if (is_sprite_program)
		{
			for (int i = 0; i+2 < count; i+=6)
				draw_sprite(indices[i], indices[i+1], indices[i+2]);
		}
		else
		{
			for (int i = 0; i+2 < count; i+=3)
				draw_triangle(indices[i], indices[i+1], indices[i+2]);
		}
	}
	else if (type == cl_lines)
	{
			for (int i = 0; i+1 < count; i+=2)
				draw_line(indices[i], indices[i+1]);
	}
	else if (type == cl_line_loop)
	{
		int i;
		
		for (i = 0; i < count-1; i++)
		{
			draw_line(indices[i], indices[i+1]);
		}
		draw_line(indices[i], indices[0]);
	}
}

void SWRenderGraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, unsigned short *indices)
{
	if (type == cl_triangles)
	{
		if (is_sprite_program)
		{
			for (int i = 0; i+2 < count; i+=6)
				draw_sprite(indices[i], indices[i+1], indices[i+2]);
		}
		else
		{
			for (int i = 0; i+2 < count; i+=3)
				draw_triangle(indices[i], indices[i+1], indices[i+2]);
		}
	}
	else if (type == cl_lines)
	{
		for (int i = 0; i+1 < count; i+=2)
			draw_line(indices[i], indices[i+1]);
	}
	else if (type == cl_line_loop)
	{
		int i;
		
		for (i = 0; i < count-1; i++)
		{
			draw_line(indices[i], indices[i+1]);
		}
		draw_line(indices[i], indices[0]);
	}
}

void SWRenderGraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, unsigned char *indices)
{
	if (type == cl_triangles)
	{
		if (is_sprite_program)
		{
			for (int i = 0; i+2 < count; i+=6)
				draw_sprite(indices[i], indices[i+1], indices[i+2]);
		}
		else
		{
			for (int i = 0; i+2 < count; i+=3)
				draw_triangle(indices[i], indices[i+1], indices[i+2]);
		}
	}
	else if (type == cl_lines)
	{
		for (int i = 0; i+1 < count; i+=2)
			draw_line(indices[i], indices[i+1]);
	}
	else if (type == cl_line_loop)
	{
		int i;
		
		for (i = 0; i < count-1; i++)
		{
			draw_line(indices[i], indices[i+1]);
		}
		draw_line(indices[i], indices[0]);
	}
}

void SWRenderGraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, unsigned int *indices, int instance_count)
{
}

void SWRenderGraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, unsigned short *indices, int instance_count)
{
}

void SWRenderGraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, unsigned char *indices, int instance_count)
{
}

void SWRenderGraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset)
{
}

void SWRenderGraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset, int instance_count)
{
}

void SWRenderGraphicContextProvider::primitives_array_freed(const PrimitivesArrayData * const prim_array)
{
}

void SWRenderGraphicContextProvider::reset_primitives_array()
{
	for (int i = 0; i < num_attribute_fetchers; i++)
		attribute_fetchers[i].clear();

	current_prim_array = 0;
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

void SWRenderGraphicContextProvider::queue_command(UniquePtr<PixelCommand> &command)
{
	canvas->queue_command(command);
}

void SWRenderGraphicContextProvider::set_clip_rect(const Rect &rect)
{
	canvas->set_clip_rect(rect);
}

void SWRenderGraphicContextProvider::reset_clip_rect()
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

void SWRenderGraphicContextProvider::set_map_mode(MapMode mode)
{
}

void SWRenderGraphicContextProvider::set_viewport(const Rectf &viewport)
{
}

void SWRenderGraphicContextProvider::set_projection(const Mat4f &matrix)
{
	current_program_provider->get_program()->set_uniform_matrix("cl_Projection", matrix);
}

void SWRenderGraphicContextProvider::set_modelview(const Mat4f &matrix)
{
	modelview_matrix = matrix;
	current_program_provider->get_program()->set_uniform_matrix("cl_ModelView", matrix);
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

	UniquePtr<PixelCommand> command(current_program_provider->get_program()->draw_triangle(canvas->get_pipeline(), current_attribute_values));
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

	UniquePtr<PixelCommand> command(current_program_provider->get_program()->draw_sprite(canvas->get_pipeline(), current_attribute_values));
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

	UniquePtr<PixelCommand> command(current_program_provider->get_program()->draw_line(canvas->get_pipeline(), current_attribute_values));
	if (command.get())
		canvas->queue_command(command);
}
}

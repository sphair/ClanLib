/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font.h"
#include "API/Display/Render/frame_buffer.h"
#include "vertex_attribute_fetcher.h"
#include <map>
#include <SDL/SDL.h>

class CL_SDLDisplayWindowProvider;

class CL_SDLGraphicContextProvider : public CL_GraphicContextProvider
{
/// \name Construction
/// \{
public:
	CL_SDLGraphicContextProvider(CL_SDLDisplayWindowProvider *window);
	~CL_SDLGraphicContextProvider();
/// \}

/// \name Attributes
/// \{
public:
	int get_max_attributes();
	CL_Size get_max_texture_size() const;
	int get_width() const;
	int get_height() const;
#ifdef WIN32
	HDC get_drawable() const;
#endif
/// \}

/// \name Operations
/// \{
public:
	void destroy();
	CL_GraphicContext create_worker_gc();
	CL_PixelBuffer get_pixeldata(const CL_Rect& rect);
	CL_TextureProvider *alloc_texture(CL_TextureDimensions texture_dimensions);
	CL_OcclusionQueryProvider *alloc_occlusion_query();
	CL_ProgramObjectProvider *alloc_program_object();
	CL_ShaderObjectProvider *alloc_shader_object();
	CL_FrameBufferProvider *alloc_frame_buffer();
	CL_RenderBufferProvider *alloc_render_buffer();
	CL_VertexArrayBufferProvider *alloc_vertex_array_buffer();
	CL_ElementArrayBufferProvider *alloc_element_array_buffer();
	void set_program_object(CL_StandardProgram standard_program);
	void set_program_object(const CL_ProgramObject &program);
	void reset_program_object();
	void set_texture(int unit_index, const CL_Texture &texture);
	void reset_texture(int unit_index);
	void set_frame_buffer(const CL_FrameBuffer &buffer);
	void reset_frame_buffer();
	void set_blend_mode(const CL_BlendMode &blendmode);
	void set_buffer_control(const CL_BufferControl &buffer_control);
	void set_pen(const CL_Pen &pen);
	void set_polygon_rasterizer(const CL_PolygonRasterizer &raster);
	void draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array);
	void set_primitives_array(const CL_PrimitivesArrayData * const prim_array);
	void draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices);
	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices);
	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices);
	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices);
	void draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset);
	void primitives_array_freed(const CL_PrimitivesArrayData * const prim_array);
	void reset_primitives_array();
	void draw_pixels(float x, float y, float zoom_x, float zoom_y, const CL_PixelBufferRef &pixel_buffer, const CL_Colorf &color);
	void set_clip_rect(const CL_Rect &rect);
	void reset_clip_rect();
	void clear(const CL_Colorf &color);
	void clear_depth(float value);
	void clear_stencil(int value);
	void set_map_mode(CL_MapMode mode);
	void set_viewport(const CL_Rectf &viewport);
	void set_projection(const CL_Mat4f &matrix);
	void set_modelview(const CL_Mat4f &matrix);
	void on_window_resized();
/// \}

/// \name Implementation
/// \{
private:
	void draw_triangle(int index1, int index2, int index3);
	void draw_quad(int index1, int index2, int index3);
	void draw_line(int index1, int index2);
	CL_Vec2f transform(const CL_Vec4f &vertex) const;
	void modified_framebuffer();
	void apply_rgba_to_suface(SDL_Surface *sptr, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);

	CL_SDLDisplayWindowProvider *window;
	SDL_Surface *current_surface;
	std::map<int, CL_Texture> bound_textures;
	const CL_PrimitivesArrayData * current_prim_array;
	VertexAttributeFetcherPtr pos_fetcher;
	VertexAttributeFetcherPtr tex_fetcher;
	VertexAttributeFetcherPtr color_fetcher;
	VertexAttributeFetcherPtr tex_index_fetcher;
	CL_Mat4f modelview_matrix;
	CL_Mat4f projection_matrix;
	mutable CL_Mat4f modelview_projection;
	mutable bool modelview_projection_invalid;

	bool is_sprite_program;

	CL_FrameBuffer framebuffer;
	CL_Slot slot_framebuffer_modified;

/// \}
};


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

#include "SDL/precomp.h"
#include "sdl_graphic_context_provider.h"
#include "sdl_display_window_provider.h"
#include "sdl_texture_provider.h"
#include "sdl_element_array_buffer_provider.h"
#include "sdl_frame_buffer_provider.h"
#include "sdl_occlusion_query_provider.h"
#include "sdl_program_object_provider.h"
#include "sdl_render_buffer_provider.h"
#include "sdl_shader_object_provider.h"
#include "sdl_vertex_array_buffer_provider.h"
#include "Display/Font/font_cache_native.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>

#define cl_sdl_color_ARGB(color) ( (static_cast<unsigned int>(color.a * 255) << 24) \
			  | (static_cast<unsigned int>(color.r * 255) << 16) \
			  | (static_cast<unsigned int>(color.g * 255) << 8) \
			  | (static_cast<unsigned int>(color.b * 255)) )

#define cl_sdl_color_ABGR(red, green, blue, alpha) (( alpha << 24) \
			  | (blue << 16) \
			  | (green << 8) \
			  | (red) )

/////////////////////////////////////////////////////////////////////////////
// CL_SDLGraphicContextProvider Construction:

CL_SDLGraphicContextProvider::CL_SDLGraphicContextProvider(CL_SDLDisplayWindowProvider *window)
: window(window), current_surface(0), current_prim_array(0), modelview_matrix(CL_Mat4f::identity()), projection_matrix(CL_Mat4f::identity()),
  modelview_projection(CL_Mat4f::identity()), modelview_projection_invalid(false),
  is_sprite_program(false)
{
	current_surface = window->get_screen();
}

CL_SDLGraphicContextProvider::~CL_SDLGraphicContextProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLGraphicContextProvider Attributes:

int CL_SDLGraphicContextProvider::get_max_attributes()
{
	return 4;
}

CL_Size CL_SDLGraphicContextProvider::get_max_texture_size() const
{
	return CL_Size(8*1024, 8*1024);
}

int CL_SDLGraphicContextProvider::get_width() const
{
	return current_surface->w;
}

int CL_SDLGraphicContextProvider::get_height() const
{
	return current_surface->h;
}

#ifdef WIN32
HDC CL_SDLGraphicContextProvider::get_drawable() const
{
	return 0; //static_cast<CL_SDLDisplayWindowProvider *>(window)->get_device_context();
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_SDLGraphicContextProvider Operations:

void CL_SDLGraphicContextProvider::destroy()
{
	delete this;
}

CL_GraphicContext CL_SDLGraphicContextProvider::create_worker_gc()
{
	return CL_GraphicContext();
}

CL_PixelBuffer CL_SDLGraphicContextProvider::get_pixeldata(const CL_Rect& rect)
{
	return CL_PixelBuffer();
}

CL_TextureProvider *CL_SDLGraphicContextProvider::alloc_texture(CL_TextureDimensions texture_dimensions)
{
	return new CL_SDLTextureProvider;
}

CL_OcclusionQueryProvider *CL_SDLGraphicContextProvider::alloc_occlusion_query()
{
	return new CL_SDLOcclusionQueryProvider;
}

CL_ProgramObjectProvider *CL_SDLGraphicContextProvider::alloc_program_object()
{
	return new CL_SDLProgramObjectProvider;
}

CL_ShaderObjectProvider *CL_SDLGraphicContextProvider::alloc_shader_object()
{
	return new CL_SDLShaderObjectProvider;
}

CL_FrameBufferProvider *CL_SDLGraphicContextProvider::alloc_frame_buffer()
{
	return new CL_SDLFrameBufferProvider;
}

CL_RenderBufferProvider *CL_SDLGraphicContextProvider::alloc_render_buffer()
{
	return new CL_SDLRenderBufferProvider;
}

CL_VertexArrayBufferProvider *CL_SDLGraphicContextProvider::alloc_vertex_array_buffer()
{
	return new CL_SDLVertexArrayBufferProvider;
}

CL_ElementArrayBufferProvider *CL_SDLGraphicContextProvider::alloc_element_array_buffer()
{
	return new CL_SDLElementArrayBufferProvider;
}

void CL_SDLGraphicContextProvider::set_program_object(CL_StandardProgram standard_program)
{
	if (standard_program == cl_program_sprite)
		is_sprite_program = true;
	else
		is_sprite_program = false;
}

void CL_SDLGraphicContextProvider::set_program_object(const CL_ProgramObject &program)
{
	is_sprite_program = false;
}

void CL_SDLGraphicContextProvider::reset_program_object()
{
	is_sprite_program = false;
}

void CL_SDLGraphicContextProvider::set_texture(int unit_index, const CL_Texture &texture)
{
	bound_textures[unit_index] = texture;
}

void CL_SDLGraphicContextProvider::reset_texture(int unit_index)
{
	std::map<int, CL_Texture>::iterator it = bound_textures.find(unit_index);
	if (it != bound_textures.end())
		bound_textures.erase(it);
}

void CL_SDLGraphicContextProvider::set_frame_buffer(const CL_FrameBuffer &buffer)
{
	CL_SDLFrameBufferProvider *sdl_framebuffer = dynamic_cast<CL_SDLFrameBufferProvider *>(buffer.get_provider());
	if (!sdl_framebuffer)
	{
		throw CL_Exception(cl_text("Invalid FrameBuffer"));
	}

	framebuffer = buffer;

	slot_framebuffer_modified = sdl_framebuffer->get_sig_changed_event().connect(this, &CL_SDLGraphicContextProvider::modified_framebuffer);

	current_surface = sdl_framebuffer->get_colorbuffer0();
	// Check for invalid color buffer
	if (current_surface == NULL)
		current_surface = window->get_screen();

}

void CL_SDLGraphicContextProvider::modified_framebuffer()
{
	CL_SDLFrameBufferProvider *sdl_framebuffer = dynamic_cast<CL_SDLFrameBufferProvider *>(framebuffer.get_provider());

	current_surface = sdl_framebuffer->get_colorbuffer0();
	// Check for invalid color buffer
	if (current_surface == NULL)
		current_surface = window->get_screen();

}

void CL_SDLGraphicContextProvider::reset_frame_buffer()
{
	current_surface = window->get_screen();

	slot_framebuffer_modified = CL_Slot();
	framebuffer = CL_FrameBuffer();
}

void CL_SDLGraphicContextProvider::set_blend_mode(const CL_BlendMode &blendmode)
{
}

void CL_SDLGraphicContextProvider::set_buffer_control(const CL_BufferControl &buffer_control)
{
}

void CL_SDLGraphicContextProvider::set_pen(const CL_Pen &pen)
{
}

void CL_SDLGraphicContextProvider::set_polygon_rasterizer(const CL_PolygonRasterizer &raster)
{
}

void CL_SDLGraphicContextProvider::draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array)
{
	set_primitives_array(prim_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void CL_SDLGraphicContextProvider::set_primitives_array(const CL_PrimitivesArrayData * const prim_array)
{
	current_prim_array = prim_array;
	pos_fetcher.bind(current_prim_array, 0);
	color_fetcher.bind(current_prim_array, 1);
	tex_fetcher.bind(current_prim_array, 2);
	tex_index_fetcher.bind(current_prim_array, 3);

}

void CL_SDLGraphicContextProvider::draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices)
{
	if (type == cl_triangles)
	{
		int end_vertices = offset+num_vertices;

		if (is_sprite_program)
		{
			for (int i = offset; i+2 < end_vertices; i+=6)
				draw_quad(i+0, i+1, i+2);
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
}

void CL_SDLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices)
{
	if (type == cl_triangles)
	{
		if (is_sprite_program)
		{
			for (int i = 0; i+2 < count; i+=6)
				draw_quad(indices[i], indices[i+1], indices[i+2]);
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
}

void CL_SDLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices)
{
	if (type == cl_triangles)
	{
		if (is_sprite_program)
		{
			for (int i = 0; i+2 < count; i+=6)
				draw_quad(indices[i], indices[i+1], indices[i+2]);
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

}

void CL_SDLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices)
{
	if (type == cl_triangles)
	{
		if (is_sprite_program)
		{
			for (int i = 0; i+2 < count; i+=6)
				draw_quad(indices[i], indices[i+1], indices[i+2]);
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

}

void CL_SDLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset)
{

}

void CL_SDLGraphicContextProvider::primitives_array_freed(const CL_PrimitivesArrayData * const prim_array)
{
}

void CL_SDLGraphicContextProvider::reset_primitives_array()
{
	pos_fetcher.clear();
	tex_fetcher.clear();
	color_fetcher.clear();
	tex_index_fetcher.clear();
	current_prim_array = 0;
}

void CL_SDLGraphicContextProvider::draw_pixels(float x, float y, float zoom_x, float zoom_y, const CL_PixelBufferRef &pixel_buffer, const CL_Colorf &color)
{

}

void CL_SDLGraphicContextProvider::set_clip_rect(const CL_Rect &rect)
{
	SDL_Rect source_rect;
	source_rect.x = rect.left;
	source_rect.y = rect.top;
	source_rect.w = rect.get_width();
	source_rect.h = rect.get_height();
	SDL_SetClipRect(current_surface, &source_rect);
}

void CL_SDLGraphicContextProvider::reset_clip_rect()
{
	SDL_SetClipRect(current_surface, NULL);
}

void CL_SDLGraphicContextProvider::clear(const CL_Colorf &color)
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = current_surface->w;
	rect.h = current_surface->h;

	SDL_FillRect(current_surface, &rect, cl_sdl_color_ARGB(color));
}

void CL_SDLGraphicContextProvider::clear_depth(float value)
{
}

void CL_SDLGraphicContextProvider::clear_stencil(int value)
{
}

void CL_SDLGraphicContextProvider::set_map_mode(CL_MapMode mode)
{
}

void CL_SDLGraphicContextProvider::set_viewport(const CL_Rectf &viewport)
{
}

void CL_SDLGraphicContextProvider::set_projection(const CL_Mat4f &matrix)
{
	projection_matrix = matrix;
	modelview_projection_invalid = true;
}

void CL_SDLGraphicContextProvider::set_modelview(const CL_Mat4f &matrix)
{
	modelview_matrix = matrix;
	modelview_projection_invalid = true;
}

void CL_SDLGraphicContextProvider::on_window_resized()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLGraphicContextProvider Implementation:

void CL_SDLGraphicContextProvider::draw_triangle(int index1, int index2, int index3)
{
	static CL_Vec4f default_pos(0.0f, 0.0f, 1.0f, 1.0f);
	static CL_Vec4f default_color(1.0f, 1.0f, 1.0f, 1.0f);
	static CL_Vec4f default_tex_coord(0.0f, 0.0f, 0.0f, 0.0f);
	static CL_Vec4f default_sampler(0.0f, 0.0f, 0.0f, 0.0f);

	int indexes[3] = { index1, index2, index3 };
	CL_Vec4f pos[3];
	CL_Vec4f primary_color[3];
	CL_Vec2f tex_coords[3];
	CL_Vec1f sampler_index;

	pos_fetcher->fetch(pos, indexes, 3, default_pos);
	color_fetcher->fetch(primary_color, indexes, 3, default_color);
	tex_fetcher->fetch(tex_coords, indexes, 3, default_tex_coord);
	tex_index_fetcher->fetch(&sampler_index, indexes, 1, default_sampler);

	CL_Vec2i screen_pos[3];
	for (int v=0; v<3; v++)
		screen_pos[v] = transform(pos[v]);

	SDL_Surface *texture_surface = NULL;

	std::map<int, CL_Texture>::iterator it = bound_textures.find( (int) sampler_index.x);
	if (it != bound_textures.end())
	{
		CL_SDLTextureProvider *tptr =  dynamic_cast<CL_SDLTextureProvider *>(it->second.get_provider());
		if (tptr)
		{
			texture_surface = tptr->get_surface();
		}
	}

	if (texture_surface)
	{
		throw CL_Exception(cl_text("The SDL target cannot draw textured triangles!"));
	}
	else
	{
		unsigned int red = static_cast<unsigned int> (primary_color[0].r * 255.0);
		unsigned int green= static_cast<unsigned int> (primary_color[0].g * 255.0);
		unsigned int blue = static_cast<unsigned int> (primary_color[0].b * 255.0);
		unsigned int alpha = static_cast<unsigned int> (primary_color[0].a * 255.0);

		if (red > 255) red = 255;
		if (green > 255) green = 255;
		if (blue > 255) blue = 255;
		if (alpha > 255) alpha = 255;

		// Note, only using the first primiary color, SDL gfx is rather limiting

		filledTrigonRGBA(current_surface, screen_pos[0].x, screen_pos[0].y, screen_pos[1].x, screen_pos[1].y, screen_pos[2].x, screen_pos[2].y,
			red, green, blue, alpha);
	}
}

void CL_SDLGraphicContextProvider::draw_quad(int index1, int index2, int index3)
{
	static CL_Vec4f default_pos(0.0f, 0.0f, 1.0f, 1.0f);
	static CL_Vec4f default_color(1.0f, 1.0f, 1.0f, 1.0f);
	static CL_Vec4f default_tex_coord(0.0f, 0.0f, 0.0f, 0.0f);
	static CL_Vec4f default_sampler(0.0f, 0.0f, 0.0f, 0.0f);

	int indexes[3] = { index1, index2, index3 };
	CL_Vec4f pos[3];
	CL_Vec4f primary_color[3];
	CL_Vec2f tex_coords[3];
	CL_Vec1f sampler_index;

	pos_fetcher->fetch(pos, indexes, 3, default_pos);
	color_fetcher->fetch(primary_color, indexes, 3, default_color);
	tex_fetcher->fetch(tex_coords, indexes, 3, default_tex_coord);
	tex_index_fetcher->fetch(&sampler_index, indexes, 1, default_sampler);

	unsigned int red = static_cast<unsigned int> (primary_color[0].r * 255.0);
	unsigned int green= static_cast<unsigned int> (primary_color[0].g * 255.0);
	unsigned int blue = static_cast<unsigned int> (primary_color[0].b * 255.0);
	unsigned int alpha = static_cast<unsigned int> (primary_color[0].a * 255.0);
	if (red > 255) red = 255;
	if (green > 255) green = 255;
	if (blue > 255) blue = 255;
	if (alpha > 255) alpha = 255;

	if (alpha <= 0) return;

	CL_Vec2i screen_pos[3];
	for (int v=0; v<3; v++)
		screen_pos[v] = transform(pos[v]);

	SDL_Surface *texture_surface = NULL;
	CL_SDLTextureProvider *tptr = NULL;

	std::map<int, CL_Texture>::iterator it = bound_textures.find( (int) sampler_index.x);
	if (it != bound_textures.end())
	{
		tptr =  dynamic_cast<CL_SDLTextureProvider *>(it->second.get_provider());
		if (tptr)
		{
			texture_surface = tptr->get_surface();
		}
	}

	if (texture_surface)
	{
		// Transform texture coords to pixel coords
		tex_coords[0].x *= texture_surface->w;
		tex_coords[0].y *= texture_surface->h;
		tex_coords[1].x *= texture_surface->w;
		tex_coords[1].y *= texture_surface->h;
		tex_coords[2].x *= texture_surface->w;
		tex_coords[2].y *= texture_surface->h;

		CL_Vec2i tex_coords_int[3];
		tex_coords_int[0] = tex_coords[0];
		tex_coords_int[1] = tex_coords[1];
		tex_coords_int[2] = tex_coords[2];

		bool direct_copy = false;

		// Test for direct copy
		if ( (screen_pos[0].y == screen_pos[1].y) && (screen_pos[0].x == screen_pos[2].x) )
		{
			if ( (tex_coords_int[0].y == tex_coords_int[1].y) && (tex_coords_int[0].x == tex_coords_int[2].x) )
			{
				if ( (screen_pos[1].x - screen_pos[0].x) == (tex_coords_int[1].x - tex_coords_int[0].x) )
				{
					if ( (screen_pos[2].y - screen_pos[0].y) == (tex_coords_int[2].y - tex_coords_int[0].y) )
					{
						direct_copy = true;
					}
				}
			}
		}
		if (direct_copy)
		{
			SDL_Rect source_rect;
			source_rect.x = tex_coords_int[0].x;
			source_rect.y = tex_coords_int[0].y;
			source_rect.w = tex_coords_int[1].x - tex_coords_int[0].x;
			source_rect.h = tex_coords_int[2].y - tex_coords_int[0].y;

			SDL_Rect dest_rect;
			dest_rect.x = screen_pos[0].x;
			dest_rect.y = screen_pos[0].y;
			dest_rect.w = screen_pos[1].x - screen_pos[0].x;
			dest_rect.h = screen_pos[2].y - screen_pos[0].y;

			if ( (alpha == 255) && (red == 255) && (green == 255) && (blue == 255) )
			{
				// Standard direct copy
				SDL_BlitSurface(texture_surface, &source_rect, current_surface, &dest_rect);
			}else
			{
				// Use the primary colour
				SDL_Surface *temp_surface_texture = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, source_rect.w, source_rect.h, 32, texture_surface->format->Rmask , texture_surface->format->Gmask, texture_surface->format->Bmask, texture_surface->format->Amask);
				if (!temp_surface_texture)
				{
					throw CL_Exception(cl_text("Cannot create temporary surface"));
				}


				SDL_Rect texture_rect;
				texture_rect.x = 0;
				texture_rect.y = 0;
				texture_rect.w = temp_surface_texture->w;
				texture_rect.h = temp_surface_texture->h;

				SDL_SetAlpha(texture_surface, 0, 255);
				SDL_BlitSurface(texture_surface, &source_rect, temp_surface_texture, &texture_rect);
				SDL_SetAlpha(texture_surface, SDL_SRCALPHA, 0);

				apply_rgba_to_suface(temp_surface_texture, red, green, blue, alpha);

				SDL_BlitSurface(temp_surface_texture, &texture_rect, current_surface, &dest_rect);

				SDL_FreeSurface(temp_surface_texture);

			}
		}
		else
		{
			if ( (tex_coords_int[0].y != tex_coords_int[1].y) || (tex_coords_int[0].x != tex_coords_int[2].x) )
			{
				throw CL_Exception(cl_text("Only texture rectangles supported for the SDL target"));
			}

			// Check for stretched destination
			if ( (screen_pos[0].y == screen_pos[1].y) && (screen_pos[0].x == screen_pos[2].x) )
			{

				int xoffset = tex_coords_int[0].x;
				int yoffset = tex_coords_int[0].y;

				if ( (xoffset < 0) || (xoffset >= texture_surface->w) || (yoffset < 0) || (yoffset >= texture_surface->h)
				|| (tex_coords_int[1].x > texture_surface->w) || (tex_coords_int[2].y > texture_surface->h)
				|| (xoffset > tex_coords_int[1].x) || (yoffset > tex_coords_int[2].y ) )
				{
				//	throw CL_Exception(cl_text("Texture rectangle is too complex for the SDL target"));
					return;
				}

				int texture_width = tex_coords_int[1].x - tex_coords_int[0].x;
				int texture_height = tex_coords_int[2].y - tex_coords_int[0].y;

				if (( texture_width == 0) || (texture_height == 0) )
					return;

				double zoom_x =  (screen_pos[1].x - screen_pos[0].x) / (double) texture_width;
				double zoom_y =  (screen_pos[2].y - screen_pos[0].y) / (double) texture_height;

				// Create a temporary surface
				SDL_LockSurface(texture_surface);

				unsigned char *pixels = (unsigned char *) texture_surface->pixels;
				if (pixels == NULL)
				{
					SDL_UnlockSurface(texture_surface);
					throw CL_Exception(cl_text("Cannot obtain the texture surface pixel buffer"));
				}

				pixels += xoffset * texture_surface->format->BytesPerPixel;
				pixels += yoffset * texture_surface->pitch;

				SDL_Surface *ref_surface = SDL_CreateRGBSurfaceFrom( pixels, texture_width, texture_height, 
					texture_surface->format->BitsPerPixel,
					texture_surface->pitch,
					texture_surface->format->Rmask, texture_surface->format->Gmask,  texture_surface->format->Bmask,  texture_surface->format->Amask);

				if (!ref_surface)
				{
					SDL_UnlockSurface(texture_surface);
					throw CL_Exception(cl_text("Cannot obtain a temporary surface"));
				}

				int smooth = 0;

				if ( (zoom_x < 1.0) || (zoom_y < 1.0) )
				{
					if (tptr->get_min_filter() == cl_filter_nearest)
						smooth = 1;
				}
				else
				{
					if ( (zoom_x > 2.0) || (zoom_y > 1.0) )
					{	
						if (tptr->get_mag_filter() == cl_filter_nearest)
							smooth = 1;
					}
				}

				SDL_Surface *stretched_surface = zoomSurface( ref_surface, zoom_x, zoom_y, smooth );
				SDL_FreeSurface(ref_surface);
				SDL_UnlockSurface(texture_surface);

				SDL_Rect source_rect;
				source_rect.x = 0;
				source_rect.y = 0;
				source_rect.w = screen_pos[1].x - screen_pos[0].x;
				source_rect.h = screen_pos[2].y - screen_pos[0].y;

				SDL_Rect dest_rect;
				dest_rect.x = screen_pos[0].x;
				dest_rect.y = screen_pos[0].y;
				dest_rect.w = screen_pos[1].x - screen_pos[0].x;
				dest_rect.h = screen_pos[2].y - screen_pos[0].y;
				SDL_BlitSurface(stretched_surface, &source_rect, current_surface, &dest_rect);

				SDL_FreeSurface(stretched_surface);

			}else
			{
				throw CL_Exception(cl_text("Texture rotations are currently not supported for the SDL target"));
			}
		}
	}
	else
	{
		// Test for rectangle draw
		if ( (screen_pos[0].y == screen_pos[1].y) && (screen_pos[0].x == screen_pos[2].x) )
		{
			boxRGBA(current_surface,
					screen_pos[0].x,
					screen_pos[0].y, 
					screen_pos[1].x,
					screen_pos[2].y,
				red, green, blue, alpha);
		}
		else
		{
			Sint16 x_positions[4];
			Sint16 y_positions[4];

			x_positions[0] = screen_pos[0].x;
			y_positions[0] = screen_pos[0].y;
			x_positions[1] = screen_pos[1].x;
			y_positions[1] = screen_pos[1].y;
			x_positions[2] = screen_pos[2].x + screen_pos[1].x - screen_pos[0].x;
			y_positions[2] = screen_pos[2].y + screen_pos[1].y - screen_pos[0].y;
			x_positions[3] = screen_pos[2].x;
			y_positions[3] = screen_pos[2].y;
			filledPolygonRGBA(current_surface, x_positions, y_positions, 4, red, green, blue, alpha);
		}
	}
}

void CL_SDLGraphicContextProvider::draw_line(int index1, int index2)
{
	static CL_Vec4f default_pos(0.0f, 0.0f, 1.0f, 1.0f);
	static CL_Vec4f default_color(1.0f, 1.0f, 1.0f, 1.0f);
	static CL_Vec4f default_tex_coord(0.0f, 0.0f, 0.0f, 0.0f);
	static CL_Vec4f default_sampler(0.0f, 0.0f, 0.0f, 0.0f);

	int indexes[3] = { index1, index2 };
	CL_Vec4f pos[2];
	CL_Vec4f primary_color[2];
	CL_Vec2f tex_coords[2];
	CL_Vec1f sampler_index;

	pos_fetcher->fetch(pos, indexes, 2, default_pos);
	color_fetcher->fetch(primary_color, indexes, 2, default_color);
	tex_fetcher->fetch(tex_coords, indexes, 2, default_tex_coord);
	tex_index_fetcher->fetch(&sampler_index, indexes, 1, default_sampler);

	CL_Vec2i screen_pos[2];
	for (int v=0; v<2; v++)
		screen_pos[v] = transform(pos[v]);

	SDL_Surface *texture_surface = NULL;

	std::map<int, CL_Texture>::iterator it = bound_textures.find( (int) sampler_index.x );
	if (it != bound_textures.end())
	{
		CL_SDLTextureProvider *tptr =  dynamic_cast<CL_SDLTextureProvider *>(it->second.get_provider());
		if (tptr)
		{
			texture_surface = tptr->get_surface();
		}
	}

	if (texture_surface)
	{
		throw CL_Exception(cl_text("Texture SDL line is not supported"));
	}
	else
	{
		unsigned int red = static_cast<unsigned int> (primary_color[0].r * 255.0);
		unsigned int green= static_cast<unsigned int> (primary_color[0].g * 255.0);
		unsigned int blue = static_cast<unsigned int> (primary_color[0].b * 255.0);
		unsigned int alpha = static_cast<unsigned int> (primary_color[0].a * 255.0);

		lineRGBA(current_surface, screen_pos[0].x, screen_pos[0].y, screen_pos[1].x, screen_pos[1].y,
			red, green, blue, alpha);
	}

}

CL_Vec2f CL_SDLGraphicContextProvider::transform(const CL_Vec4f &vertex) const
{
	if (modelview_projection_invalid)
	{
		modelview_projection = projection_matrix * modelview_matrix;
		modelview_projection_invalid = false;
	}

	CL_Vec4f v = modelview_projection * vertex;
	return CL_Vec2f(v.x, v.y);
}

void CL_SDLGraphicContextProvider::apply_rgba_to_suface(SDL_Surface *sptr, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
{
	int width = sptr->w;
	int height = sptr->h;

	if ( (width<=0) || (height<=0) )
		return;

	if (sptr->format->BytesPerPixel != 4)
	{
			throw CL_Exception(cl_text("Expecting rgba sdl surface"));
	}

	SDL_LockSurface(sptr);
	unsigned char *pixels = (unsigned char *) sptr->pixels;
	if (pixels == NULL)
	{
			SDL_UnlockSurface(sptr);
			throw CL_Exception(cl_text("Cannot obtain the texture surface pixel buffer"));
	}
	int pitch = (sptr->pitch) - (width * sizeof(int));

	red = 255 - red;
	green = 255 - green;
	blue = 255 - blue;
	alpha = 255 - alpha;

	unsigned int color;
	int new_color;

	for (; height >0; --height)
	{
		for (int xcnt = 0; xcnt < width; ++xcnt)
		{
			new_color = *(pixels) - red;
			if (new_color < 0) new_color = 0;
			*(pixels++) = new_color;

			new_color = *(pixels) - green;
			if (new_color < 0) new_color = 0;
			*(pixels++) = new_color;

			new_color = *(pixels) - blue;
			if (new_color < 0) new_color = 0;
			*(pixels++) = new_color;

			new_color = *(pixels) - alpha;
			if (new_color < 0) new_color = 0;
			*(pixels++) = new_color;
		}
		pixels += pitch;
	}

	SDL_UnlockSurface(sptr);

}


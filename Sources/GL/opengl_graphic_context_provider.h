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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Kevin J Bluck
*/

#pragma once

#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/GL/opengl.h"
#include "API/Core/Math/mat4.h"
#include "API/Core/Signals/signal_v0.h"
#include "API/Display/Render/buffer_control.h"
#include "API/Display/Render/polygon_rasterizer.h"
#include "API/Display/Render/blend_mode.h"
#include "API/Display/Render/program_object.h"

#ifndef WIN32
#include <map>
#endif

class CL_RenderWindowProvider;
class CL_ShaderObjectProvider;
class CL_FrameBufferProvider;
class CL_RenderBufferProvider;
class CL_OpenGLFrameBufferProvider;

class CL_OpenGLGraphicContextProvider : public CL_GraphicContextProvider
{
/// \name Construction
/// \{
public:
	/// \brief Creates a new OpenGL graphic context provider for a rendering window.
	/** <p>The pointer to the rendering window provider will be owned by this graphic
	    context provider object, and will be deleted upon its destruction.</p>*/
	CL_OpenGLGraphicContextProvider(const CL_RenderWindowProvider * const render_window);

	~CL_OpenGLGraphicContextProvider();

/// \}
/// \name Attributes
/// \{
public:
	int get_max_attributes();
	CL_Size get_max_texture_size() const;

	/// \brief Get the opengl version major number
	int get_opengl_version_major() const {return opengl_version_major;}

	/// \brief Get the opengl version minor number
	int get_opengl_version_minor() const {return opengl_version_minor;}

	/// \brief Get the glsl version major number
	int get_glsl_version_major() const {return shader_version_major;}

	/// \brief Get the glsl version minor number
	int get_glsl_version_minor() const {return shader_version_minor;}

	const CL_Mat4f &get_modelview() const { return modelview; }
	const CL_RenderWindowProvider & get_render_window() const { return *render_window; }
	int get_width() const;
	int get_height() const;
	void get_opengl_version(int &version_major, int &version_minor);
	void get_opengl_shading_language_version(int &version_major, int &version_minor);
	CL_String get_renderer_string();
	CL_String get_vendor_string();
	std::vector<CL_String> get_extensions();

/// \}
/// \name Operations
/// \{
public:
	void destroy();
	CL_TextureProvider *alloc_texture(CL_TextureDimensions texture_dimensions);
	CL_OcclusionQueryProvider *alloc_occlusion_query();
	CL_ProgramObjectProvider *alloc_program_object();
	CL_ShaderObjectProvider *alloc_shader_object();
	CL_FrameBufferProvider *alloc_frame_buffer();
	CL_RenderBufferProvider *alloc_render_buffer();
	CL_PixelBufferProvider *alloc_pixel_buffer();
	CL_VertexArrayBufferProvider *alloc_vertex_array_buffer();
	CL_ElementArrayBufferProvider *alloc_element_array_buffer();
	CL_PixelBuffer get_pixeldata(const CL_Rect& rect, CL_TextureFormat pixel_format, bool clamp) const;
	void set_texture(int unit_index, const CL_Texture &texture);
	void reset_texture(int unit_index, const CL_Texture &texture);
	void set_frame_buffer(const CL_FrameBuffer &write_buffer, const CL_FrameBuffer &read_buffer);
	void reset_frame_buffer();
	void set_program_object(CL_StandardProgram standard_program);
	void set_program_object(const CL_ProgramObject &program, int program_matrix_flags);
	void reset_program_object();
	void set_buffer_control(const CL_BufferControl &buffer_control);
	void draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array);
	void set_primitives_array(const CL_PrimitivesArrayData * const prim_array);
	void draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices);
	void draw_primitives_array_instanced(CL_PrimitivesType type, int offset, int num_vertices, int instance_count);
	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices);
	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices);
	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices);
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned int *indices, int instance_count);
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned short *indices, int instance_count);
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned char *indices, int instance_count);
	void draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset);
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset, int instance_count);
	void primitives_array_freed(const CL_PrimitivesArrayData * const prim_array);
	void reset_primitives_array();
	void draw_pixels(CL_GraphicContext &gc, float x, float y, float zoom_x, float zoom_y, const CL_PixelBuffer &pixel_buffer, const CL_Rect &src_rect, const CL_Colorf &color);
	void set_clip_rect(const CL_Rect &rect);
	void reset_clip_rect();
	void clear(const CL_Colorf &color);
	void clear_depth(float value);
	void clear_stencil(int value);
	void set_map_mode(CL_MapMode mode);
	void set_viewport(const CL_Rectf &viewport);
	void set_projection(const CL_Mat4f &matrix);
	void set_modelview(const CL_Mat4f &matrix);
	void set_blend_mode(const CL_BlendMode &blendmode);
	void set_pen(const CL_Pen &pen);
	void set_polygon_rasterizer(const CL_PolygonRasterizer &raster);
	void on_window_resized();

	/// \brief Get OpenGL extension specific function address.
	CL_ProcAddress *get_proc_address(const CL_String8& function_name) const ;

/// \}
/// \name Implementation
/// \{
private:
	void set_current_program_object_matricies();
	void draw_primitives_legacy(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array);
	void check_opengl_version();

	GLenum to_enum(enum CL_DrawBuffer buf);
	GLenum to_enum(enum CL_CompareFunction func);
	GLenum to_enum(enum CL_StencilOp op);
	GLenum to_enum(enum CL_CullMode mode);
	GLenum to_enum(enum CL_FillMode mode);
	GLenum to_enum(enum CL_BlendFunc func);
	GLenum to_enum(enum CL_BlendEquation eq);
	GLenum to_enum(enum CL_VertexAttributeDataType value);
	GLenum to_enum(enum CL_PrimitivesType value);
	GLenum to_enum(enum CL_LogicOp op);

	/// \brief OpenGL render window.
	const CL_RenderWindowProvider * const render_window;

	/// \brief OpenGL FrameBuffer.
	/** Can be null if attached to a window instead.*/
	CL_OpenGLFrameBufferProvider *draw_buffer_provider;
	CL_OpenGLFrameBufferProvider *read_buffer_provider;

	/// \brief Coordinate mapping/projection mode for graphic context.
	CL_MapMode map_mode;

	/// \brief Coordinate mapping/projection mode for graphic context - before the framebuffer was set.
	CL_MapMode map_mode_before_framebuffer;

	/// \brief Projection matrix in user projection mapping mode.
	CL_Mat4f projection;

	/// \brief Currently active modelview matrix.
	CL_Mat4f modelview;

	bool framebuffer_bound;

	bool prim_arrays_set;

	int num_set_program_attribute_arrays;

	CL_Rect last_clip_rect;

	const CL_PrimitivesArrayData *cur_prim_array;

	std::vector<CL_ProgramObject> standard_programs;

	int current_program_matrix_flags;	// CL_ProgramMatrixFlags bitmask flags
	CL_ProgramObject current_program_object;

	bool modelview_projection_matrix_valid;	// Calculated by set_current_program_object_matricies()
	CL_Mat4f modelview_projection_matrix;

	bool normal_matrix_valid;	// Calculated by set_current_program_object_matricies()
	CL_Mat3f normal_matrix;

	int opengl_version_major;
	int opengl_version_minor;

	int shader_version_major;
	int shader_version_minor;

	bool use_glsl_1_50;	// Available with OpenGL 3.2 and above
	bool use_open_3_1;	// Available with OpenGL 3.1 and above
/// \}
};

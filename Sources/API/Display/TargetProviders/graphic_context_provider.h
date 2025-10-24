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
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Text/string_types.h"
#include "../Render/graphic_context.h"
#include "../Render/primitives_array.h"
#include "../Render/texture.h"
#include "../../Core/Math/mat4.h"

class CL_Texture;
class CL_Stencil;
class CL_TextureProvider;
class CL_FontProvider;
class CL_BlendMode;
class CL_PolygonRasterizer;
class CL_Font;
class CL_OcclusionQueryProvider;
class CL_ProgramObjectProvider;
class CL_ShaderObjectProvider;
class CL_FrameBufferProvider;
class CL_RenderBufferProvider;
class CL_VertexArrayBufferProvider;
class CL_ElementArrayBufferProvider;
class CL_FontDescription;
class CL_PixelBufferProvider;

/// \brief Primitives array data supplied to clanDisplay provider objects.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_PrimitivesArrayData
{
public:
	struct VertexData
	{
		int size;
		CL_VertexAttributeDataType type;
		int stride;
		CL_VertexArrayBufferProvider *array_provider;
		bool single_value;
		union
		{
			void *data;
			unsigned char value_ubyte[4];
			char value_byte[4];
			unsigned short value_ushort[4];
			short value_short[4];
			unsigned int value_uint[4];
			int value_int[4];
			float value_float[4];
			bool value_boolean;
		};
	};

	int num_attributes;
	int *attribute_indexes;
	VertexData *attributes;
	bool *normalize_attributes;
};

/// \brief Interface for implementing a CL_GraphicContext target.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_GraphicContextProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_GraphicContextProvider() { return; }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the maximum amount of attributes available.
	virtual int get_max_attributes() = 0;

	/// \brief Returns the maximum size of a texture this graphic context supports.
	/** <p>It returns CL_Size(0,0) if there is no known limitation to the max
	    texture size.</p>*/
	virtual CL_Size get_max_texture_size() const = 0;

	/// \brief Returns the current width of the context.
	virtual int get_width() const = 0;

	/// \brief Returns the current height of the context.
	virtual int get_height() const = 0;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys graphic context provider.
	virtual void destroy() = 0;
	
	/// \brief Return the content of the draw buffer into a pixel buffer.
	virtual CL_PixelBuffer get_pixeldata(const CL_Rect& rect, CL_TextureFormat pixel_format, bool clamp) const = 0;

	/// \brief Allocate texture provider for this gc.
	virtual CL_TextureProvider *alloc_texture(CL_TextureDimensions texture_dimensions) = 0;

	/// \brief Allocate occlusion query provider of this gc.
	virtual CL_OcclusionQueryProvider *alloc_occlusion_query() = 0;

	/// \brief Allocate program object provider of this gc.
	virtual CL_ProgramObjectProvider *alloc_program_object() = 0;

	/// \brief Allocate shader object provider of this gc.
	virtual CL_ShaderObjectProvider *alloc_shader_object() = 0;

	/// \brief Allocate frame buffer provider for this gc.
	virtual CL_FrameBufferProvider *alloc_frame_buffer() = 0;

	/// \brief Allocate render buffer provider for this gc.
	virtual CL_RenderBufferProvider *alloc_render_buffer() = 0;

	/// \brief Allocate vertex array buffer provider for this gc.
	virtual CL_VertexArrayBufferProvider *alloc_vertex_array_buffer() = 0;

	/// \brief Allocate element array buffer provider for this gc.
	virtual CL_ElementArrayBufferProvider *alloc_element_array_buffer() = 0;

	/// \brief Allocate pixel buffer provider for this gc.
	virtual CL_PixelBufferProvider *alloc_pixel_buffer() = 0;

	/// \brief Set active program object to the standard program specified.
	virtual void set_program_object(CL_StandardProgram standard_program) = 0;

	/// \brief Set active program object.
	///
	/// \param program = Program to set
	/// \param program_matrix_flags = Which matricies will be set in the program object ( CL_ProgramMatrixFlags bitmask flags )
	virtual void set_program_object(const CL_ProgramObject &program, int program_matrix_flags) = 0;

	/// \brief Remove active program object.
	virtual void reset_program_object() = 0;

	/// \brief Select texture into unit.
	virtual void set_texture(int unit_index, const CL_Texture &texture) = 0;

	/// \brief Remove texture from unit.
	virtual void reset_texture(int unit_index, const CL_Texture &texture) = 0;

	/// \brief Set a frame buffer for off-screen rendering.
	virtual void set_frame_buffer(const CL_FrameBuffer &write_buffer, const CL_FrameBuffer &read_buffer) = 0;

	/// \brief Set the rendering buffer back to the screen.
	virtual void reset_frame_buffer() = 0;

	/// \brief Set blending modes.
	virtual void set_blend_mode(const CL_BlendMode &blendmode) = 0;

	/// \brief Set buffer control states.
	virtual void set_buffer_control(const CL_BufferControl &buffer_control) = 0;

	/// \brief Select pen.
	virtual void set_pen(const CL_Pen &pen) = 0;

	/// \brief Set polygon rasterizer settings.
	virtual void set_polygon_rasterizer(const CL_PolygonRasterizer &raster) = 0;

	/// \brief Draw primitives on gc.
	virtual void draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array) = 0;

	/// \brief Set the primitives array on the gc.
	virtual void set_primitives_array(const CL_PrimitivesArrayData * const prim_array) = 0;

	/// \brief Draws primitives from the current assigned primitives array.
	virtual void draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices) = 0;

	/// \brief Draws instanced primitives from the current assigned primitives array.
	virtual void draw_primitives_array_instanced(CL_PrimitivesType type, int offset, int num_vertices, int instance_count) = 0;

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = value
	virtual void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices) = 0;

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = short
	virtual void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices) = 0;

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = char
	virtual void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices) = 0;

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = value
	/// \param instance_count = number of instances drawn
	virtual void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned int *indices, int instance_count) = 0;

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = short
	/// \param instance_count = number of instances drawn
	virtual void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned short *indices, int instance_count) = 0;

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = char
	/// \param instance_count = number of instances drawn
	virtual void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned char *indices, int instance_count) = 0;

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param array_provider = Element Array Buffer Provider
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	virtual void draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset) = 0;

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param array_provider = Element Array Buffer Provider
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	virtual void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset, int instance_count) = 0;

	/// \brief Called when a primitives array is being recycled.
	virtual void primitives_array_freed(const CL_PrimitivesArrayData * const prim_array) = 0;

	/// \brief Reset the primitives arrays.
	virtual void reset_primitives_array() = 0;

	/// \brief Draw pixel buffer on gc.
	virtual void draw_pixels(CL_GraphicContext &gc, float x, float y, float zoom_x, float zoom_y, const CL_PixelBuffer &pixel_buffer, const CL_Rect &src_rect, const CL_Colorf &color) = 0;

	/// \brief Set clip rect.
	virtual void set_clip_rect(const CL_Rect &rect) = 0;

	/// \brief Reset clip rect.
	virtual void reset_clip_rect() = 0;

	/// \brief Clears the whole context using the specified color.
	virtual void clear(const CL_Colorf &color) = 0;

	/// \brief Clear the depth buffer
	///
	/// \param value: value to clear to. Range 0.0 - 1.0.
	virtual void clear_depth(float value) = 0;

	/// \brief Clear the stencil buffer
	///
	/// \param value value to clear to.
	virtual void clear_stencil(int value) = 0;

	/// \brief Set the projection mapping mode.
	virtual void set_map_mode(CL_MapMode mode) = 0;

	/// \brief Set the viewport to be used in user projection map mode.
	virtual void set_viewport(const CL_Rectf &viewport) = 0;

	/// \brief Set the projection matrix to be used in user projection map mode.
	virtual void set_projection(const CL_Mat4f &matrix) = 0;

	/// \brief Sets the model view matrix to a new matrix.
	virtual void set_modelview(const CL_Mat4f &matrix) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}

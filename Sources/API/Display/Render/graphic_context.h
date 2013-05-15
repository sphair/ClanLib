/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../2D/color.h"
#include "../Image/texture_format.h"
#include <memory>
#include "../../Core/Math/mat4.h"
#include "../../Core/IOData/virtual_directory.h"
#include "../../Core/Math/rect.h"
#include "../../Core/Signals/signal_v0.h"
#include "primitives_array.h"
#include "frame_buffer.h"
#include "element_array_vector.h"

namespace clan
{

class Size;
class Texture;
class FrameBuffer;
class PixelBuffer;
class PrimitivesArray;
class Font;
class FontMetrics;
class GraphicContextProvider;
class GraphicContext_Impl;
class ProgramObject;
class Angle;
class RenderBatcher;
class FontProvider_Freetype;
class UniformBuffer;
class StorageBuffer;
class RasterizerState;
class BlendState;
class DepthStencilState;

/// \brief Polygon culling modes.
enum CullMode
{
	cull_front,
	cull_back,
	cull_front_and_back
};

/// \brief Polygon filling modes.
enum FillMode
{
	fill_point,
	fill_line,
	fill_polygon
};

/// \brief Front face modes.
enum FaceSide
{
	face_clockwise,
	face_counter_clockwise
};

/// \brief Compare functions.
enum CompareFunction
{
	compare_lequal,
	compare_gequal,
	compare_less,
	compare_greater,
	compare_equal,
	compare_notequal,
	compare_always,
	compare_never
};

/// \brief Stencil operations
enum StencilOp
{
	stencil_keep,
	stencil_zero,
	stencil_replace,
	stencil_incr,
	stencil_decr,
	stencil_invert,
	stencil_incr_wrap,
	stencil_decr_wrap
};

/// \brief Drawing buffers.
enum DrawBuffer
{
	buffer_none,
	buffer_front_left,
	buffer_front_right,
	buffer_back_left,
	buffer_back_right,
	buffer_front,
	buffer_back,
	buffer_left,
	buffer_right,
	buffer_front_and_back
};

/// \brief Logic Op
enum LogicOp
{
	logic_clear,
	logic_and, 
	logic_and_reverse,
	logic_copy,
	logic_and_inverted,
	logic_noop,
	logic_xor,
	logic_or,
	logic_nor,
	logic_equiv,
	logic_invert,
	logic_or_reverse,
	logic_copy_inverted,
	logic_or_inverted,
	logic_nand,
	logic_set
};

/// \brief Blending functions.
enum BlendFunc
{
	/// \brief source or destination (0, 0, 0, 0)
	blend_zero,

	/// \brief source or destination (1, 1, 1, 1)
	blend_one,

	/// \brief source (Rd, Gd, Bd, Ad)
	blend_dest_color,

	/// \brief destination (Rs, Gs, Bs, As)
	blend_src_color,

	/// \brief source (1, 1, 1, 1) - (Rd, Gd, Bd, Ad)
	blend_one_minus_dest_color,

	/// \brief destination (1, 1, 1, 1) - (Rs, Gs, Bs, As)
	blend_one_minus_src_color,

	/// \brief source or destination (As, As, As, As)
	blend_src_alpha,

	/// \brief source or destination (1, 1, 1, 1) - (As, As, As, As)
	blend_one_minus_src_alpha,

	/// \brief source or destination (Ad, Ad, Ad, Ad)
	blend_dest_alpha,

	/// \brief source or destination (1, 1, 1, 1) - (Ad, Ad, Ad, Ad)
	blend_one_minus_dest_alpha,

	/// \brief source (f, f, f, 1) - f = min(As, 1 - Ad)
	blend_src_alpha_saturate,

	/// \brief source or destination (Rc, Gc, Bc, Ac)
	blend_constant_color,

	/// \brief source or destination (1, 1, 1, 1) - (Rc, Gc, Bc, Ac)
	blend_one_minus_constant_color,

	/// \brief source or destination (Ac, Ac, Ac, Ac)
	blend_constant_alpha,

	/// \brief source or destination (1, 1, 1, 1) - (Ac, Ac, Ac, Ac)
	blend_one_minus_constant_alpha
};

/// \brief Blending equations.
enum BlendEquation
{
	equation_add,
	equation_subtract,
	equation_reverse_subtract,
	equation_min,
	equation_max
};

/// \brief Point Sprite Origin
enum PointSpriteOrigin
{
	origin_upper_left,
	origin_lower_left
};

/// \brief Primitive types.
enum PrimitivesType
{
	type_points,
	type_line_strip,
	type_line_loop,
	type_lines,
	type_triangle_strip,
	type_triangle_fan,
	type_triangles
};

/// \brief Y axis direction for viewports, clipping rects, textures and render targets
enum TextureImageYAxis
{
	y_axis_bottom_up,  // OpenGL, origin is lower left with Y going upwards
	y_axis_top_down    // Direct3D, origin is upper left with Y going downwards
};

/// \brief Standard Program
enum StandardProgram
{
	program_color_only,
	program_single_texture,
	program_sprite
};

/// \brief Shader language used
enum ShaderLanguage
{
	shader_glsl,
	shader_hlsl,
	shader_fixed_function,
	num_shader_languages
};

/// \brief Interface to drawing graphics.
class CL_API_DISPLAY GraphicContext
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance.
	GraphicContext();

	/// \brief Constructs a GraphicContext
	///
	/// \param provider = Graphic Context Provider
	GraphicContext(GraphicContextProvider *provider);

	~GraphicContext();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns in what range clip space z values are clipped.
	ClipZRange get_clip_z_range() const;

	/// \brief Returns the Y axis direction for viewports, clipping rects, textures and render targets
	TextureImageYAxis get_texture_image_y_axis() const;

	/// \brief Returns the shader language used
	ShaderLanguage get_shader_language() const;

	/// \brief Returns the currently selected texture for the specified index.
	///
	/// \param index = 0 to x, the index of the texture
	/// \return The texture. Use texture.is_null() to determine if the texture was not selected
	Texture get_texture(int index) const;

	/// \brief Returns the currently selected textures
	///
	/// \return The selected textures (placed at unit_index 0 to size()-1).  These may contain null textures if textures were not selected
	std::vector<Texture> get_textures() const;

	/// \brief Returns the currently selected write frame buffer.
	///
	/// \return The frame buffer. Use frame_buffer.is_null() to determine if the frame buffer was not selected
	FrameBuffer get_write_frame_buffer() const;

	/// \brief Returns the currently selected read frame buffer.
	///
	/// \return The frame buffer. Use frame_buffer.is_null() to determine if the frame buffer was not selected
	FrameBuffer get_read_frame_buffer() const;

	/// \brief Returns the current width of the context.
	int get_width() const;

	/// \brief Returns the current height of the context.
	int get_height() const;

	/// \brief Returns the current size of the context.
	Size get_size() const;

	/// \brief Returns the maximum size of a texture this graphic context supports.
	/** <p>It returns Size(0,0) if there is no known limitation to the max
	    texture size.</p>*/
	Size get_max_texture_size() const;

	/// \brief Returns the provider for this graphic context.
	GraphicContextProvider *get_provider();

	const GraphicContextProvider * const get_provider() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Create a new default graphic context compatible with this one
	GraphicContext create() const;

	/// \brief Create a new default graphic context with a frame buffer selected
	GraphicContext create(FrameBuffer &buffer) const;

	/// \brief Create a new default graphic context cloned with this one
	GraphicContext clone() const;

	/// \brief Return the content of the read buffer into a pixel buffer.
	PixelBuffer get_pixeldata(const Rect& rect, TextureFormat texture_format = tf_rgba8, bool clamp = true);

	/// \brief Return the content of the read buffer into a pixel buffer.
	PixelBuffer get_pixeldata(TextureFormat texture_format = tf_rgba8, bool clamp = true);

	/// \brief Returns true if this frame buffer object is owned by this graphic context.
	///
	/// Frame buffer objects cannot be shared between graphic contexts.  This function verifies that the frame buffer object
	/// belongs to this graphic context.
	bool is_frame_buffer_owner(const FrameBuffer &fb);

	/// \brief Sets the current frame buffer.
	void set_frame_buffer(const FrameBuffer &write_buffer);
	void set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer);

	/// \brief Resets the current frame buffer to be the initial frame buffer.
	void reset_frame_buffer();

	/// \brief Select uniform buffer into index
	void set_uniform_buffer(int index, const UniformBuffer &buffer);

	/// \brief Remove uniform buffer from index
	void reset_uniform_buffer(int index);

	/// \brief Select storage buffer into index
	void set_storage_buffer(int index, const StorageBuffer &buffer);

	/// \brief Remove storage buffer from index
	void reset_storage_buffer(int index);

	/// \brief Select texture into index.
	///
	/// \param unit_index = 0 to x, the index of this texture
	/// \param texture = The texture to select.  This can be an empty texture Texture()
	void set_texture(int unit_index, const Texture &texture);

	/// \brief Select textures
	///
	/// Only textures units from 0 to textures.size()-1 are set.
	///
	/// \param textures = The texture to select (placed at unit_index 0 to texture.size()-1).  These may contain null textures
	void set_textures(std::vector<Texture> &textures);

	/// \brief Remove texture from index.
	///
	/// \param unit_index = 0 to x, the index of the texture
	void reset_texture(int unit_index);

	/// \brief Remove all selected textures
	void reset_textures();

	/// \brief Select texture image into index.
	///
	/// \param unit_index = 0 to x, the index of this texture
	/// \param texture = The texture to select.  This can be an empty texture Texture()
	void set_image_texture(int unit_index, const Texture &texture);

	/// \brief Select texture images
	///
	/// Only textures units from 0 to textures.size()-1 are set.
	///
	/// \param textures = The texture to select (placed at unit_index 0 to texture.size()-1).  These may contain null textures
	void set_image_texture(std::vector<Texture> &textures);

	/// \brief Remove texture from index.
	///
	/// \param unit_index = 0 to x, the index of the texture
	void reset_image_texture(int unit_index);

	/// \brief Remove all selected textures
	void reset_image_textures();

	/// \brief Set active rasterizer state
	void set_rasterizer_state(const RasterizerState &state);

	/// \brief Set active blend state
	void set_blend_state(const BlendState &state, const Vec4f &blend_color = Vec4f(1.0f), unsigned int sample_mask = 0xffffffff);

	/// \brief Set active depth stencil state
	void set_depth_stencil_state(const DepthStencilState &state, int stencil_ref = 0);

	/// \brief Set active rasterizer state
	void reset_rasterizer_state();

	/// \brief Set active blend state
	void reset_blend_state();

	/// \brief Set active depth stencil state
	void reset_depth_stencil_state();

	/// \brief Set active program object to the standard program specified.
	void set_program_object(StandardProgram standard_program);

	/// \brief Set active program object.
	///
	/// \param program = Program to set
	void set_program_object(const ProgramObject &program);

	/// \brief Remove active program object.
	void reset_program_object();

	/// \brief Returns true if this primitives array is owned by this graphic context.
	///
	/// Primitive array objects cannot be shared between graphic contexts.  This function verifies that the primitives array
	/// belongs to this graphic context.
	bool is_primitives_array_owner(const PrimitivesArray &primitives_array);

	/// \brief Draw primitives on gc.
	void draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &array);

	/// \brief Set the primitives array on the gc.
	void set_primitives_array(const PrimitivesArray &array);

	/// \brief Draws primitives from the current assigned primitives array.
	void draw_primitives_array(PrimitivesType type, int num_vertices);

	/// \brief Draw primitives array
	///
	/// \param type = Primitives Type
	/// \param offset = value
	/// \param num_vertices = value
	void draw_primitives_array(PrimitivesType type, int offset, int num_vertices);

	/// \brief Draw primitives array instanced
	///
	/// \param type = Primitives Type
	/// \param offset = value
	/// \param num_vertices = value
	/// \param instance_count = number of instances drawn
	void draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count);

	/// \brief Sets current elements array buffer
	void set_primitives_elements(ElementArrayBuffer &element_array);

	/// \brief Sets current elements array buffer
	template<typename Type>
	void set_primitives_elements(ElementArrayVector<Type> &element_array)
	{
		set_primitives_elements((ElementArrayBuffer&)element_array);
	}

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	void draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset = 0);

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	/// \param instance_count = number of instances drawn
	void draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count);

	/// \brief Resets current elements array buffer
	void reset_primitives_elements();

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	void draw_primitives_elements(PrimitivesType type, int count, ElementArrayBuffer &element_array, VertexAttributeDataType indices_type, size_t offset = 0);

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	void draw_primitives_elements(PrimitivesType type, int count, ElementArrayVector<unsigned int> &element_array, size_t offset = 0)
	{
		draw_primitives_elements(type, count, (ElementArrayBuffer&)element_array, type_unsigned_int, offset * sizeof(unsigned int));
	}

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	void draw_primitives_elements(PrimitivesType type, int count, ElementArrayVector<unsigned short> &element_array, size_t offset = 0)
	{
		draw_primitives_elements(type, count, (ElementArrayBuffer&)element_array, type_unsigned_short, offset * sizeof(unsigned short));
	}

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	void draw_primitives_elements(PrimitivesType type, int count, ElementArrayVector<unsigned char> &element_array, size_t offset = 0)
	{
		draw_primitives_elements(type, count, (ElementArrayBuffer&)element_array, type_unsigned_byte, offset * sizeof(unsigned char));
	}

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	/// \param instance_count = number of instances drawn
	void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBuffer &element_array, VertexAttributeDataType indices_type, size_t offset, int instance_count);

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	/// \param instance_count = number of instances drawn
	void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayVector<unsigned int> &element_array, size_t offset, int instance_count)
	{
		draw_primitives_elements_instanced(type, count, (ElementArrayBuffer&)element_array, type_unsigned_int, offset * sizeof(unsigned int), instance_count);
	}

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	/// \param instance_count = number of instances drawn
	void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayVector<unsigned short> &element_array, size_t offset, int instance_count)
	{
		draw_primitives_elements_instanced(type, count, (ElementArrayBuffer&)element_array, type_unsigned_short, offset * sizeof(unsigned short), instance_count);
	}

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	/// \param instance_count = number of instances drawn
	void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayVector<unsigned char> &element_array, size_t offset, int instance_count)
	{
		draw_primitives_elements_instanced(type, count, (ElementArrayBuffer&)element_array, type_unsigned_byte, offset * sizeof(unsigned char), instance_count);
	}

	/// \brief Reset the primitives arrays.
	void reset_primitives_array();

	/// \brief Execute a compute shader.
	void dispatch(int x = 1, int y = 1, int z = 1);

	/// \brief Clears the whole context using the specified color.
	void clear(const Colorf &color = Colorf::black);

	/// \brief Clear the stencil buffer
	///
	/// \param value value to clear to.
	void clear_stencil(int value = 0);

	/// \brief Clear the depth buffer
	///
	/// \param value: value to clear to. Range 0.0 - 1.0.
	void clear_depth(float value = 0);

	/// \brief Set the current clipping rectangle.
	void set_scissor(const Rect &rect, TextureImageYAxis y_axis);

	/// \brief Removes the set clipping rectangle 
	void reset_scissor();

	/// \brief Set the viewport to be used in user projection map mode.
	///
	/// \param viewport = The viewport to set
	void set_viewport(const Rectf &viewport);

	/// \brief Set the specified viewport to be used in user projection map mode.
	///
	/// \param index = The viewport index (0 to x)
	/// \param viewport = The viewport to set
	void set_viewport(int index, const Rectf &viewport);

	/// \brief Specifies the depth range for all viewports
	void set_depth_range(float n, float f);

	/// \brief Specifies the depth range for the specified viewport
	void set_depth_range(int viewport, float n, float f);

	/// \brief The default value is 1.0
	void set_point_size(float);

	/// \brief Alpha fade point once minimum size treshold reached. Requires multisampling to be enabled.
	void set_point_fade_treshold_size(float);

	/// \brief Enables if points sizes is set by the vertex shader.
	void enable_vertex_program_point_size(bool enable);

	/// \brief Sets the origin of texture point sprites.
	void set_point_sprite_origin(PointSpriteOrigin origin);

	/// \brief Set used draw buffer.
	void set_draw_buffer(DrawBuffer buffer);

	/// \brief Enables/disables anti-aliasing.
	void set_antialiased(bool value);

	/// \brief Enables/disables point offsetting.
	void set_offset_point(bool value);

	/// \brief Enables/disables line offsetting.
	void set_offset_line(bool value);

	/// \brief Enables/disables polygon offsetting.
	void set_offset_fill(bool value);

	/// \brief Sets the offset factor.
	void set_polygon_offset(float factor, float units);

/// \}
/// \name Events
/// \{
public:

/// \}
/// \name Operators
/// \{
public:
	bool operator ==(const GraphicContext &other) const { return impl == other.impl; }
	bool operator !=(const GraphicContext &other) const { return impl != other.impl; }

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<GraphicContext_Impl> impl;

	friend class OpenGL;
/// \}
};

const float pixelcenter_constant = 0.375f;

}

/// \}

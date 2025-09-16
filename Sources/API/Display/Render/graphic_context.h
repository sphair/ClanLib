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

#pragma once

#include "../2D/color.h"
#include "../Image/texture_format.h"
#include <memory>
#include "../../Core/Math/mat4.h"
#include "../../Core/Math/rect.h"
#include "../../Core/Signals/signal.h"
#include "primitives_array.h"
#include "frame_buffer.h"
#include "element_array_vector.h"

namespace clan
{
	/// \addtogroup clanDisplay_Display clanDisplay Display
	/// \{

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

	/// Polygon culling modes.
	enum class CullMode
	{
		front,
		back,
		front_and_back
	};

	/// Polygon filling modes.
	enum class FillMode
	{
		point,
		line,
		polygon
	};

	/// Front face modes.
	enum class FaceSide
	{
		clockwise,
		counter_clockwise
	};

	/// Compare functions.
	enum class CompareFunction
	{
		lequal,
		gequal,
		less,
		greater,
		equal,
		notequal,
		always,
		never
	};

	/// Stencil operations
	enum class StencilOp
	{
		keep,
		zero,
		replace,
		incr,
		decr,
		invert,
		incr_wrap,
		decr_wrap
	};

	/// Drawing buffers.
	enum class DrawBuffer
	{
		none,
		front_left,
		front_right,
		back_left,
		back_right,
		front,
		back,
		left,
		right,
		front_and_back
	};

	/// Logic Op
	enum class LogicOp
	{
		_clear,
		_and,
		_and_reverse,
		_copy,
		_and_inverted,
		_noop,
		_xor,
		_or,
		_nor,
		_equiv,
		_invert,
		_or_reverse,
		_copy_inverted,
		_or_inverted,
		_nand,
		_set
	};

	/// Blending functions.
	enum class BlendFunc
	{
		/// source or destination (0, 0, 0, 0)
		zero,

		/// source or destination (1, 1, 1, 1)
		one,

		/// source (Rd, Gd, Bd, Ad)
		dest_color,

		/// destination (Rs, Gs, Bs, As)
		src_color,

		/// source (1, 1, 1, 1) - (Rd, Gd, Bd, Ad)
		one_minus_dest_color,

		/// destination (1, 1, 1, 1) - (Rs, Gs, Bs, As)
		one_minus_src_color,

		/// source or destination (As, As, As, As)
		src_alpha,

		/// source or destination (1, 1, 1, 1) - (As, As, As, As)
		one_minus_src_alpha,

		/// source or destination (Ad, Ad, Ad, Ad)
		dest_alpha,

		/// source or destination (1, 1, 1, 1) - (Ad, Ad, Ad, Ad)
		one_minus_dest_alpha,

		/// source (f, f, f, 1) - f = min(As, 1 - Ad)
		src_alpha_saturate,

		/// source or destination (Rc, Gc, Bc, Ac)
		constant_color,

		/// source or destination (1, 1, 1, 1) - (Rc, Gc, Bc, Ac)
		one_minus_constant_color,

		/// source or destination (Ac, Ac, Ac, Ac)
		constant_alpha,

		/// source or destination (1, 1, 1, 1) - (Ac, Ac, Ac, Ac)
		one_minus_constant_alpha
	};

	/// Blending equations.
	enum class BlendEquation
	{
		add,
		subtract,
		reverse_subtract,
		min,
		max
	};

	/// Point Sprite Origin
	enum class PointSpriteOrigin
	{
		upper_left,
		lower_left
	};

	/// Primitive types.
	enum class PrimitivesType
	{
		points,
		line_strip,
		line_loop,
		lines,
		triangle_strip,
		triangle_fan,
		triangles
	};

	/// Y axis direction for viewports, clipping rects, textures and render targets
	enum class TextureImageYAxis
	{
		y_bottom_up,  //!< OpenGL, origin is lower left with Y going upwards
		y_top_down    //!< Direct3D, origin is upper left with Y going downwards
	};

	/// Standard Program
	enum class StandardProgram
	{
		color_only,
		single_texture,
		sprite,
		path
	};

	/// Shader language used
	enum class ShaderLanguage
	{
		glsl,
		hlsl,
		fixed_function,
		num_shader_languages
	};

	/// Interface to drawing graphics.
	class GraphicContext
	{
	public:
		/// Constructs a null instance.
		GraphicContext();

		/** Constructs a new graphic context from a provider.
		 *  \param provider = Graphic Context Provider
		 */
		GraphicContext(GraphicContextProvider *provider);

		~GraphicContext();

		/// Returns true if this object is invalid.
		bool is_null() const { return !impl; }
		explicit operator bool() const { return bool(impl); }

		/// Throw an exception if this object is invalid.
		void throw_if_null() const;

		/// Returns in what range clip space z values are clipped.
		ClipZRange get_clip_z_range() const;

		/// Returns the Y axis direction for viewports, clipping rects, textures and render targets
		TextureImageYAxis get_texture_image_y_axis() const;

		/// Returns the shader language used
		ShaderLanguage get_shader_language() const;

		/** Returns the major version / feature level supported by the hardware.
		 *  For an OpenGL target, this returns the major OpenGL version the driver supports.
		 *  For a Direct3D target, this returns the major feature level.
		 */
		int get_major_version() const;

		/** Returns the minor version / feature level supported by the hardware.
		 *  For an OpenGL target, this returns the minor OpenGL version the driver supports.
		 *  For a Direct3D target, this returns the minor feature level.
		 */
		int get_minor_version() const;

		/** Returns `true` if the hardware supports compute shaders.
		 *  This function will always returns true for OpenGL 4.3 or newer, or
		 *  Direct3D 11.0 or newer. For Direct3D 10.0 and 10.1, the support for
		 *  compute shaders is optional.
		 */
		bool has_compute_shader_support() const;

		/** Retrieves the texture selected in this context with an index number.
		 *  \param index The texture index number to retrieve. [0 to n]
		 *  \return The texture on the specified index. Use Texture::is_null() to
		 *          determine whether the texture has been selected by the context.
		 */
		Texture get_texture(int index) const;

		/** Returns the textures currently selected in this context.
		 *  \return A vector containing the selected textures. The vector may
		 *          contain null (unselected) texture elements within it..
		 */
		std::vector<Texture> get_textures() const;

		/** Returns the currently selected write frame buffer.
		 *  \return The frame buffer. Use frame_buffer.is_null() to determine if the frame buffer was not selected
		 */
		FrameBuffer get_write_frame_buffer() const;

		/// Returns the currently selected read frame buffer.
		///
		/// \return The frame buffer. Use frame_buffer.is_null() to determine if the frame buffer was not selected
		FrameBuffer get_read_frame_buffer() const;

		/// Returns the currently selected program object
		ProgramObject get_program_object() const;

		/// Returns the current actual width of the context.
		int get_width() const;

		/// Returns the current actual height of the context.
		int get_height() const;

		/// Returns the current actual size of the context.
		Size get_size() const;

		/// Retrieves the display pixel ratio of the context.
		/// \seealso Resolution Independence
		float get_pixel_ratio() const;

		/// Calculates the device independent width of the context.
		/// \seealso Resolution Independence
		float get_dip_width() const { return get_width() / get_pixel_ratio(); }

		/// Calculates the device independent height of the context.
		/// \seealso Resolution Independence
		float get_dip_height() const { return get_height() / get_pixel_ratio(); }

		/// Calculates the device independent dimensions of the context.
		/// \seealso Resolution Independence
		Sizef get_dip_size() const { return Sizef{ get_dip_width(), get_dip_height() }; }

		/** Retrieves the maximum size for a texture that this graphic context will
		 *  allow. Size(0, 0) will be returned if there is no known limitation to
		 *  the maximum texture size allowed for the context.
		 */
		Size get_max_texture_size() const;

		/// Returns the provider for this graphic context.
		GraphicContextProvider *get_provider();

		const GraphicContextProvider * get_provider() const;

		/// Create a new default graphic context compatible with this one
		GraphicContext create() const;

		/// Create a new default graphic context with a frame buffer selected
		GraphicContext create(FrameBuffer &buffer) const;

		/// Create a new default graphic context cloned with this one
		GraphicContext clone() const;

		/// Return the content of the read buffer into a pixel buffer.
		PixelBuffer get_pixeldata(const Rect& rect, TextureFormat texture_format = TextureFormat::rgba8, bool clamp = true);

		/// Return the content of the read buffer into a pixel buffer.
		PixelBuffer get_pixeldata(TextureFormat texture_format = TextureFormat::rgba8, bool clamp = true);

		/** Returns `true` if this frame buffer object is owned by this graphic
		 *  context.
		 *
		 *  Frame buffer objects cannot be shared between graphic contexts. This
		 *  function will verify if the frame buffer object belongs to this graphic
		 *  context.
		 */
		bool is_frame_buffer_owner(const FrameBuffer &fb);

		/// Sets the current frame buffer.
		void set_frame_buffer(const FrameBuffer &write_buffer);
		void set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer);

		/// Resets the current frame buffer to be the initial frame buffer.
		void reset_frame_buffer();

		/// Select uniform buffer into index
		void set_uniform_buffer(int index, const UniformBuffer &buffer);

		/// Remove uniform buffer from index
		void reset_uniform_buffer(int index);

		/// Select storage buffer into index
		void set_storage_buffer(int index, const StorageBuffer &buffer);

		/// Remove storage buffer from index
		void reset_storage_buffer(int index);

		/// Select texture into index.
		///
		/// \param unit_index = 0 to x, the index of this texture
		/// \param texture = The texture to select.  This can be an empty texture Texture()
		void set_texture(int unit_index, const Texture &texture);

		/// Select textures
		///
		/// Only textures units from 0 to textures.size()-1 are set.
		///
		/// \param textures = The texture to select (placed at unit_index 0 to texture.size()-1).  These may contain null textures
		void set_textures(std::vector<Texture> &textures);

		/// Remove texture from index.
		///
		/// \param unit_index = 0 to x, the index of the texture
		void reset_texture(int unit_index);

		/// Remove all selected textures
		void reset_textures();

		/// Select texture image into index.
		///
		/// \param unit_index = 0 to x, the index of this texture
		/// \param texture = The texture to select.  This can be an empty texture Texture()
		void set_image_texture(int unit_index, const Texture &texture);

		/// Select texture images
		///
		/// Only textures units from 0 to textures.size()-1 are set.
		///
		/// \param textures = The texture to select (placed at unit_index 0 to texture.size()-1).  These may contain null textures
		void set_image_texture(std::vector<Texture> &textures);

		/// Remove texture from index.
		///
		/// \param unit_index = 0 to x, the index of the texture
		void reset_image_texture(int unit_index);

		/// Remove all selected textures
		void reset_image_textures();

		/// Set active rasterizer state
		void set_rasterizer_state(const RasterizerState &state);

		/// Set active blend state
		void set_blend_state(const BlendState &state, const Colorf &blend_color = StandardColorf::white(), unsigned int sample_mask = 0xffffffff);

		/// Set active depth stencil state
		void set_depth_stencil_state(const DepthStencilState &state, int stencil_ref = 0);

		/// Set active rasterizer state
		void reset_rasterizer_state();

		/// Set active blend state
		void reset_blend_state();

		/// Set active depth stencil state
		void reset_depth_stencil_state();

		/// Set active program object to the standard program specified.
		void set_program_object(StandardProgram standard_program);

		/// Set active program object.
		///
		/// \param program = Program to set
		void set_program_object(const ProgramObject &program);

		/// Remove active program object.
		void reset_program_object();

		/// Returns true if this primitives array is owned by this graphic context.
		///
		/// Primitive array objects cannot be shared between graphic contexts.  This function verifies that the primitives array
		/// belongs to this graphic context.
		bool is_primitives_array_owner(const PrimitivesArray &primitives_array);

		/// Draw primitives on gc.
		void draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &array);

		/// Set the primitives array on the gc.
		void set_primitives_array(const PrimitivesArray &array);

		/// Draws primitives from the current assigned primitives array.
		void draw_primitives_array(PrimitivesType type, int num_vertices);

		/// Draw primitives array
		///
		/// \param type = Primitives Type
		/// \param offset = value
		/// \param num_vertices = value
		void draw_primitives_array(PrimitivesType type, int offset, int num_vertices);

		/// Draw primitives array instanced
		///
		/// \param type = Primitives Type
		/// \param offset = value
		/// \param num_vertices = value
		/// \param instance_count = number of instances drawn
		void draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count);

		/// Sets current elements array buffer
		void set_primitives_elements(ElementArrayBuffer &element_array);

		/// Sets current elements array buffer
		template<typename Type>
		void set_primitives_elements(ElementArrayVector<Type> &element_array)
		{
			set_primitives_elements((ElementArrayBuffer&)element_array);
		}

		/// Draw primitives elements
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		void draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset = 0);

		/// Draw primitives elements instanced
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		/// \param instance_count = number of instances drawn
		void draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count);

		/// Resets current elements array buffer
		void reset_primitives_elements();

		/// Draw primitives elements
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		void draw_primitives_elements(PrimitivesType type, int count, ElementArrayBuffer &element_array, VertexAttributeDataType indices_type, size_t offset = 0);

		/// Draw primitives elements
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		void draw_primitives_elements(PrimitivesType type, int count, ElementArrayVector<unsigned int> &element_array, size_t offset = 0)
		{
			draw_primitives_elements(type, count, (ElementArrayBuffer&)element_array, VertexAttributeDataType::type_unsigned_int, offset * sizeof(unsigned int));
		}

		/// Draw primitives elements
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		void draw_primitives_elements(PrimitivesType type, int count, ElementArrayVector<unsigned short> &element_array, size_t offset = 0)
		{
			draw_primitives_elements(type, count, (ElementArrayBuffer&)element_array, VertexAttributeDataType::type_unsigned_short, offset * sizeof(unsigned short));
		}

		/// Draw primitives elements
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		void draw_primitives_elements(PrimitivesType type, int count, ElementArrayVector<unsigned char> &element_array, size_t offset = 0)
		{
			draw_primitives_elements(type, count, (ElementArrayBuffer&)element_array, VertexAttributeDataType::type_unsigned_byte, offset * sizeof(unsigned char));
		}

		/// Draw primitives elements instanced
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		/// \param instance_count = number of instances drawn
		void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBuffer &element_array, VertexAttributeDataType indices_type, size_t offset, int instance_count);

		/// Draw primitives elements instanced
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		/// \param instance_count = number of instances drawn
		void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayVector<unsigned int> &element_array, size_t offset, int instance_count)
		{
			draw_primitives_elements_instanced(type, count, (ElementArrayBuffer&)element_array, VertexAttributeDataType::type_unsigned_int, offset * sizeof(unsigned int), instance_count);
		}

		/// Draw primitives elements instanced
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		/// \param instance_count = number of instances drawn
		void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayVector<unsigned short> &element_array, size_t offset, int instance_count)
		{
			draw_primitives_elements_instanced(type, count, (ElementArrayBuffer&)element_array, VertexAttributeDataType::type_unsigned_short, offset * sizeof(unsigned short), instance_count);
		}

		/// Draw primitives elements instanced
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		/// \param instance_count = number of instances drawn
		void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayVector<unsigned char> &element_array, size_t offset, int instance_count)
		{
			draw_primitives_elements_instanced(type, count, (ElementArrayBuffer&)element_array, VertexAttributeDataType::type_unsigned_byte, offset * sizeof(unsigned char), instance_count);
		}

		/// Reset the primitives arrays.
		void reset_primitives_array();

		/// Execute a compute shader.
		void dispatch(int x = 1, int y = 1, int z = 1);

		/// Clears the whole context using the specified color.
		void clear(const Colorf &color = StandardColorf::black());

		/// Clear the stencil buffer
		///
		/// \param value value to clear to.
		void clear_stencil(int value = 0);

		/// Clear the depth buffer
		///
		/// \param value: value to clear to. Range 0.0 - 1.0.
		void clear_depth(float value = 0);

		/// Set the current clipping rectangle.
		void set_scissor(const Rect &rect, TextureImageYAxis y_axis);

		/// Removes the set clipping rectangle
		void reset_scissor();

		/// Set the viewport to be used in user projection map mode.
		///
		/// \param viewport = The viewport to set
		void set_viewport(const Rectf &viewport);

		/// Set the specified viewport to be used in user projection map mode.
		///
		/// \param index = The viewport index (0 to x)
		/// \param viewport = The viewport to set
		void set_viewport(int index, const Rectf &viewport);

		/// Specifies the depth range for all viewports
		void set_depth_range(float n, float f);

		/// Specifies the depth range for the specified viewport
		void set_depth_range(int viewport, float n, float f);

		/// Set used draw buffer.
		void set_draw_buffer(DrawBuffer buffer);

		/// Flush the command buffer
		void flush();

		bool operator ==(const GraphicContext &other) const { return impl == other.impl; }
		bool operator !=(const GraphicContext &other) const { return impl != other.impl; }

	private:
		std::shared_ptr<GraphicContext_Impl> impl;

		friend class OpenGL;
	};

	const float pixelcenter_constant = 0.375f;

	/// \}
}

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
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../2D/color.h"
#include "../Image/texture_format.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Math/mat4.h"
#include "../../Core/IOData/virtual_directory.h"
#include "../../Core/Math/rect.h"
#include "../../Core/Signals/signal_v0.h"
#include "primitives_array.h"
#include "frame_buffer.h"

class CL_Size;
class CL_BufferControl;
class CL_PolygonRasterizer;
class CL_Pen;
class CL_BlendMode;
class CL_Texture;
class CL_FrameBuffer;
class CL_PixelBuffer;
class CL_PrimitivesArray;
class CL_Font;
class CL_FontMetrics;
class CL_GraphicContextProvider;
class CL_GraphicContext_Impl;
class CL_ProgramObject;
class CL_ElementArrayBuffer;
class CL_Angle;
class CL_RenderBatcher;
class CL_FontProvider_Freetype;

/// \brief Primitive types.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_PrimitivesType
{
	cl_points,
	cl_line_strip,
	cl_line_loop,
	cl_lines,
	cl_triangle_strip,
	cl_triangle_fan,
	cl_triangles
};

/// \brief Mapping modes.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_MapMode
{
	cl_map_2d_upper_left,
	cl_map_2d_lower_left,
	cl_user_projection
};

/// \brief Standard Program
///
/// See "CL_StandardAttributeIndex" if using the standard program with CL_PrimitivesArray
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_StandardProgram
{
	cl_program_color_only,
	cl_program_single_texture,
	cl_program_sprite
};

/// \brief Program Object Matrix Flags
///
/// These defines determine which matricies will be set in the program object
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_ProgramMatrixFlags
{
	cl_program_matrix_none = 0,
	cl_program_matrix_modelview = 0x1,	///< cl_ModelViewMatrix (mat4)
	cl_program_matrix_projection = 0x2,	///< cl_ProjectionMatrix (mat4)
	cl_program_matrix_modelview_projection = 0x4,	///< cl_ModelViewProjectionMatrix (mat4)
	cl_program_matrix_normal = 0x8,	///< cl_NormalMatrix (mat3 - inverse of cl_ModelViewMatrix)

	cl_program_matrix_all_standard = cl_program_matrix_modelview | cl_program_matrix_projection | cl_program_matrix_modelview_projection | cl_program_matrix_normal	///< All the standard OpenGL uniforms

};

/// \brief Interface to drawing graphics.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_GraphicContext
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance.
	CL_GraphicContext();

	/// \brief Constructs a GraphicContext
	///
	/// \param provider = Graphic Context Provider
	CL_GraphicContext(CL_GraphicContextProvider *provider);

	~CL_GraphicContext();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the currently selected texture for the specified index.
	///
	/// \param index = 0 to x, the index of the texture
	/// \return The texture. Use texture.is_null() to determine if the texture was not selected
	CL_Texture get_texture(int index) const;

	/// \brief Returns the currently selected textures
	///
	/// \return The selected textures (placed at unit_index 0 to size()-1).  These may contain null textures if textures were not selected
	std::vector<CL_Texture> get_textures() const;

	/// \brief Returns the blending mode description.
	///
	/// \return The blend mode.
	CL_BlendMode get_blend_mode() const;

	/// \brief Returns the buffer control description.
	///
	/// \return The buffer control
	CL_BufferControl get_buffer_control() const;

	/// \brief Returns the polygon rasterizer setup.
	///
	/// \return The polygon rasterizer
	CL_PolygonRasterizer get_polygon_rasterizer() const;

	/// \brief Returns the currently selected write frame buffer.
	///
	/// \return The frame buffer. Use frame_buffer.is_null() to determine if the frame buffer was not selected
	CL_FrameBuffer get_write_frame_buffer() const;

	/// \brief Returns the currently selected read frame buffer.
	///
	/// \return The frame buffer. Use frame_buffer.is_null() to determine if the frame buffer was not selected
	CL_FrameBuffer get_read_frame_buffer() const;

	/// \brief Returns the currently selected pen.
	///
	/// \return The pen.
	CL_Pen get_pen() const;

	/// \brief Returns the current width of the context.
	int get_width() const;

	/// \brief Returns the current height of the context.
	int get_height() const;

	/// \brief Returns the current size of the context.
	CL_Size get_size() const;

	/// \brief Returns the current clipping rectangle used on the graphic context.
	CL_Rect get_cliprect() const;

	/// \brief Returns the current effective modelview matrix.
	const CL_Mat4f &get_modelview() const;

	/// \brief Returns the maximum size of a texture this graphic context supports.
	/** <p>It returns CL_Size(0,0) if there is no known limitation to the max
	    texture size.</p>*/
	CL_Size get_max_texture_size() const;

	/// \brief Returns the provider for this graphic context.
	CL_GraphicContextProvider *get_provider();

	const CL_GraphicContextProvider * const get_provider() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Return the content of the read buffer into a pixel buffer.
	CL_PixelBuffer get_pixeldata(const CL_Rect& rect, CL_TextureFormat pixel_format = cl_abgr8, bool clamp = true) const;

	/// \brief Return the content of the read buffer into a pixel buffer.
	CL_PixelBuffer get_pixeldata(CL_TextureFormat pixel_format = cl_abgr8, bool clamp = true) const;

	/// \brief Sets the current frame buffer.
	void set_frame_buffer(const CL_FrameBuffer &write_buffer);
	void set_frame_buffer(const CL_FrameBuffer &write_buffer, const CL_FrameBuffer &read_buffer);

	/// \brief Resets the current frame buffer to be the initial frame buffer.
	void reset_frame_buffer();

	/// \brief Select texture into index.
	///
	/// \param unit_index = 0 to x, the index of this texture
	/// \param texture = The texture to select.  This can be an empty texture CL_Texture()
	void set_texture(int unit_index, const CL_Texture &texture);

	/// \brief Select textures
	///
	/// Only textures units from 0 to textures.size()-1 are set.
	///
	/// \param textures = The texture to select (placed at unit_index 0 to texture.size()-1).  These may contain null textures
	void set_textures(std::vector<CL_Texture> &textures);

	/// \brief Remove texture from index.
	///
	/// \param unit_index = 0 to x, the index of the texture
	void reset_texture(int unit_index);

	/// \brief Remove all selected textures
	void reset_textures();

	/// \brief Set active program object to the standard program specified.
	void set_program_object(CL_StandardProgram standard_program);

	/// \brief Set active program object.
	///
	/// \param program = Program to set
	/// \param program_matrix_flags = Which matricies will be set in the program object ( CL_ProgramMatrixFlags bitmask flags)
	void set_program_object(const CL_ProgramObject &program, int program_matrix_flags = cl_program_matrix_all_standard);

	/// \brief Remove active program object.
	void reset_program_object();

	/// \brief Set blending modes.
	void set_blend_mode(const CL_BlendMode &blend_mode);

	/// \brief Reset blending to the default.
	void reset_blend_mode();

	/// \brief Set buffer control states.
	void set_buffer_control(const CL_BufferControl &buffer_control);

	/// \brief Set default buffer control states.
	void reset_buffer_control();

	/// \brief Select pen.
	void set_pen(const CL_Pen &pen);

	/// \brief Reset pen settings to defaults.
	void reset_pen();

	/// \brief Set polygon rasterizer settings.
	void set_polygon_rasterizer(const CL_PolygonRasterizer &raster);

	/// \brief Reset polygon rasterizer settings to defaults.
	void reset_polygon_rasterizer();

	/// \brief Draw primitives on gc.
	void draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArray &array);

	/// \brief Set the primitives array on the gc.
	void set_primitives_array(const CL_PrimitivesArray &array);

	/// \brief Draws primitives from the current assigned primitives array.
	void draw_primitives_array(CL_PrimitivesType type, int num_vertices);

	/// \brief Draw primitives array
	///
	/// \param type = Primitives Type
	/// \param offset = value
	/// \param num_vertices = value
	void draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices);

	/// \brief Draw primitives array instanced
	///
	/// \param type = Primitives Type
	/// \param offset = value
	/// \param num_vertices = value
	/// \param instance_count = number of instances drawn
	void draw_primitives_array_instanced(CL_PrimitivesType type, int offset, int num_vertices, int instance_count);

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = value
	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices);

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = short
	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices);

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = char
	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices);

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = value
	/// \param instance_count = number of instances drawn
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned int *indices, int instance_count);

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = short
	/// \param instance_count = number of instances drawn
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned short *indices, int instance_count);

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param indices = char
	/// \param instance_count = number of instances drawn
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned char *indices, int instance_count);

	/// \brief Draw primitives elements
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	void draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBuffer &element_array, CL_VertexAttributeDataType indices_type, void *offset = 0);

	/// \brief Draw primitives elements instanced
	///
	/// \param type = Primitives Type
	/// \param count = value
	/// \param element_array = Element Array Buffer
	/// \param indices_type = Vertex Attribute Data Type
	/// \param offset = void
	/// \param instance_count = number of instances drawn
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, CL_ElementArrayBuffer &element_array, CL_VertexAttributeDataType indices_type, void *offset, int instance_count);

	/// \brief Reset the primitives arrays.
	void reset_primitives_array();

	/// \brief Draw pixel buffer on gc.
	///
	/// Note, For OpenGL targets this is slow. Consider drawing a texture with a pixelbuffer object (created via CL_PixelBuffer(gc, ...) )
	void draw_pixels(float x, float y, const CL_PixelBuffer &pixel_buffer, const CL_Rect &src_rect, const CL_Colorf &color = CL_Colorf::white);

	/// \brief Draw pixels
	///
	/// Note, For OpenGL targets this is slow. Consider drawing a texture with a pixelbuffer object (created via CL_PixelBuffer(gc, ...) )
	///
	/// \param x = value
	/// \param y = value
	/// \param zoom_x = value
	/// \param zoom_y = value
	/// \param pixel_buffer = Pixel Buffer Ref
	/// \param src_rect = Source rect
	/// \param color = Colorf
	void draw_pixels(float x, float y, float zoom_x, float zoom_y, const CL_PixelBuffer &pixel_buffer, const CL_Rect &src_rect, const CL_Colorf &color = CL_Colorf::white);

	/// \brief Clears the whole context using the specified color.
	void clear(const CL_Colorf &color = CL_Colorf::black);

	/// \brief Clear the stencil buffer
	///
	/// \param value value to clear to.
	void clear_stencil(int value = 0);

	/// \brief Clear the depth buffer
	///
	/// \param value: value to clear to. Range 0.0 - 1.0.
	void clear_depth(float value = 0);

	/// \brief Set the current clipping rectangle.
	void set_cliprect(const CL_Rect &rect);

	/// \brief Push current clipping rectangle to stack.
	/** <p>If a rectangle is passed, it afterwards sets clipping
	    rectangle to the union of the current rectangle and the passed
	    rectangle.</p>*/
	void push_cliprect(const CL_Rect &rect);

	/// \brief Push cliprect
	void push_cliprect();

	/// \brief Pop current clipping rectangle from the stack.
	void pop_cliprect();

	/// \brief Removes the set clipping rectangle and empties the cliprect stack.
	void reset_cliprect();

	/// \brief Set the projection mapping mode.
	void set_map_mode(CL_MapMode mode);

	/// \brief Set the viewport to be used in user projection map mode.
	void set_viewport(const CL_Rectf &viewport);

	/// \brief Set the projection matrix to be used in user projection map mode.
	void set_projection(const CL_Mat4f &matrix);

	/// \brief Sets the model view matrix to a new matrix.
	void set_modelview(const CL_Mat4f &matrix);

	/// \brief Multiplies the passed matrix onto the model view matrix.
	void mult_modelview(const CL_Mat4f &matrix);

	/// \brief Pushes current model view matrix onto the model view stack.
	void push_modelview();

	/// \brief Sets a translate offset matrix, ignoring any earlier model view settings.
	/** <p> This offset will affect any subsequent display operations on the current
	    displaycard, by translating the position of the display operation with the offset.</p>*/
	void set_translate(float x, float y, float z = 0.0);

	/// \brief Adds the translate offset.
	/** <p> This offset will affect any subsequent display operations on the current
	    displaycard, by translating the position of the display operation with the offset.
	    The offset will be offset by any previous offsets pushed onto the stack,
	    eg. it inherits the previous offset. </p>*/
	void mult_translate(float x, float y, float z = 0.0);

	/// \brief Push translation offset onto model view stack.
	/** <p>This function is a convenience function for calling push_modelview, then mult_translate.</p>
	    <p> This offset will affect any subsequent display operations on the current
	    displaycard, by translating the position of the display operation with the offset.
	    The offset will be offset by any previous offsets pushed onto the stack,
	    eg. it inherits the previous offset. </p>*/
	void push_translate(float x, float y, float z = 0.0);

	/// \brief Sets a rotation matrix, ignoring any earlier model view settings.
	void set_rotate(const CL_Angle &angle, float x = 0.0, float y = 0.0, float z = 1.0, bool normalize = true);

	/// \brief Adds a rotation matrix to existing model view.
	void mult_rotate(const CL_Angle &angle, float x = 0.0, float y = 0.0, float z = 1.0, bool normalize = true);

	/// \brief Pushes a rotation matrix onto model view stack.
	void push_rotate(const CL_Angle &angle, float x = 0.0, float y = 0.0, float z = 1.0);

	/// \brief Sets a scale matrix, ignoring any earlier model view settings.
	void set_scale(float x, float y, float z = 1.0);

	/// \brief Adds a scale matrix to existing model view.
	void mult_scale(float x, float y, float z = 1.0);

	/// \brief Pushes a scale matrix onto model view stack.
	void push_scale(float x, float y, float z = 1.0);

	/// \brief Pops last pushed model view matrix off the stack and makes it the active one.
	void pop_modelview();

	/// \brief Flushes the render batcher currently active.
	void flush_batcher();

	/// \brief Specifies which render batcher is to be currently active
	///
	/// If the render batcher is already active, nothing happens. If a different render batcher
	/// is currently active, it is flushed before the new batcher is made active.
	void set_batcher(CL_RenderBatcher *batcher);

/// \}
/// \name Events
/// \{
public:
	/// \brief Called when the last GC in the thread is about to be destroyed.
	CL_Signal_v0 &sig_gc_destruction_imminent();

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_GraphicContext_Impl> impl;

	friend class CL_PrimitivesArray;
	friend class CL_Sprite_Impl;
	friend class CL_Image;
	friend class CL_GlyphCache;
	friend class CL_Draw;
/// \}
};

#ifndef cl_pixelcenter_constant
#define cl_pixelcenter_constant 0.375f
#endif

/// \}

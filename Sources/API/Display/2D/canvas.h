/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Mark Page
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../Render/graphic_context.h"

namespace clan
{

class Canvas_Impl;
class GraphicContext;
class PrimitivesArray;
class Sprite_Impl;
class Image;
class GlyphCache;
class Draw;
class RenderBatcher;
class Colorf;
class Pointf;
class Gradient;
class LineSegment2f;
class LineSegment2;
class Trianglef;
class Triangle;
class DisplayWindow;
class DisplayWindowDescription;
class FontManager;

/// \brief Mapping modes.
enum MapMode
{
	map_2d_upper_left,
	map_2d_lower_left,
	map_user_projection
};

/// \brief 2D Graphics Canvas
class CL_API_DISPLAY Canvas
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance.
	Canvas();

	/// \brief Constructs a Canvas (based on a specific GraphicContext)
	explicit Canvas(GraphicContext &context);

	/// \brief Constructs a Canvas based on a framebuffer.  (based on the COPY of the GraphicContext)
	Canvas(GraphicContext &context, FrameBuffer &framebuffer);

	/// \brief Constructs a Canvas (based on a specific GraphicContext held in the DisplayWindow)
	explicit Canvas(DisplayWindow &window);

	/// \brief Constructs a Canvas (based on a DisplayWindow description)
	explicit Canvas(DisplayWindowDescription &desc);

	~Canvas();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get gc
	///
	/// \return Graphic Context
	GraphicContext& get_gc() const;

	/// \brief Returns the current effective modelview matrix.
	const Mat4f &get_modelview() const;

	/// \brief Returns the current effective projection matrix.
	const Mat4f &get_projection() const;

	operator GraphicContext&() const {return get_gc();}

	/// \brief Returns the current width of the context.
	inline int get_width() const {return get_gc().get_width();}

	/// \brief Returns the current height of the context.
	inline int get_height() const {return get_gc().get_height();}

	/// \brief Returns the current size of the context.
	inline Size get_size() const {return get_gc().get_size();}

	/// \brief Returns the current clipping rectangle
	Rect get_cliprect() const;

	/// \brief Return the content of the read buffer into a pixel buffer.
	PixelBuffer get_pixeldata(const Rect& rect, TextureFormat texture_format = tf_rgba8, bool clamp = true);

	/// \brief Return the content of the read buffer into a pixel buffer.
	PixelBuffer get_pixeldata(TextureFormat texture_format = tf_rgba8, bool clamp = true);

	/// \brief Get the font manager
	FontManager get_font_manager() const;

	/// \brief Get the display window attached to this canvas
	///
	/// Returns an empty object if no display window exists
	DisplayWindow get_window() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Constructs an independent canvas based on this one
	Canvas create();

	/// \brief Constructs an independent canvas based on a framebuffer.
	Canvas create(FrameBuffer &framebuffer);

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

	/// \brief Enable/disable logic op.
	void enable_logic_op(bool enabled);

	/// \brief Set logic operation.
	void set_logic_op(LogicOp op);

	/// \brief Set the current clipping rectangle.
	void set_cliprect(const Rect &rect);

	/// \brief The default value is 1.0
	void set_point_size(float);

	/// \brief Alpha fade point once minimum size treshold reached. Requires multisampling to be enabled.
	void set_point_fade_treshold_size(float);

	/// \brief Sets the origin of texture point sprites.
	void set_point_sprite_origin(PointSpriteOrigin origin);

	/// \brief Push current clipping rectangle to stack.
	/** <p>If a rectangle is passed, it afterwards sets clipping
	    rectangle to the union of the current rectangle and the passed
	    rectangle.</p>*/
	void push_cliprect(const Rect &rect);

	/// \brief Push cliprect
	void push_cliprect();

	/// \brief Pop current clipping rectangle from the stack.
	void pop_cliprect();

	/// \brief Removes the set clipping rectangle and empties the cliprect stack.
	void reset_cliprect();


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

	/// \brief Set active program object to the standard program specified.
	void set_program_object(StandardProgram standard_program);

	/// \brief Set the projection mapping mode.
	void set_map_mode(MapMode mode);

	/// \brief Set the projection matrix to be used in user projection map mode.
	void set_projection(const Mat4f &matrix);

	/// \brief Specifies which render batcher is to be currently active
	///
	/// If the render batcher is already active, nothing happens. If a different render batcher
	/// is currently active, it is flushed before the new batcher is made active.
	void set_batcher(RenderBatcher *batcher);

	/// \brief Sets the model view matrix to a new matrix.
	void set_modelview(const Mat4f &matrix);

	/// \brief Multiplies the passed matrix onto the model view matrix.
	void mult_modelview(const Mat4f &matrix);

	/// \brief Pushes current model view matrix onto the model view stack.
	void push_modelview();

	/// \brief Sets a translate offset matrix, ignoring any earlier model view settings.
	/** <p> This offset will affect any subsequent display operations on the current
	    displaycard, by translating the position of the display operation with the offset.</p>*/
	void set_translate(float x, float y, float z = 0.0);
	void set_translate(const Vec2f &vec) { set_translate(vec.x, vec.y, 0.0f); }
	void set_translate(const Vec3f &vec) { set_translate(vec.x, vec.y, vec.z); }
	void set_translate(int x, int y, int z = 0) { set_translate((float) x, (float) y, (float) z); }

	/// \brief Adds the translate offset.
	/** <p> This offset will affect any subsequent display operations on the current
	    displaycard, by translating the position of the display operation with the offset.
	    The offset will be offset by any previous offsets pushed onto the stack,
	    eg. it inherits the previous offset. </p>*/
	void mult_translate(float x, float y, float z = 0.0);
	void mult_translate(const Vec2f &vec) { mult_translate(vec.x, vec.y, 0.0f); }
	void mult_translate(const Vec3f &vec) { mult_translate(vec.x, vec.y, vec.z); }
	void mult_translate(int x, int y, int z = 0) { mult_translate((float) x, (float) y, (float) z); }

	/// \brief Push translation offset onto model view stack.
	/** <p>This function is a convenience function for calling push_modelview, then mult_translate.</p>
	    <p> This offset will affect any subsequent display operations on the current
	    displaycard, by translating the position of the display operation with the offset.
	    The offset will be offset by any previous offsets pushed onto the stack,
	    eg. it inherits the previous offset. </p>*/
	void push_translate(float x, float y, float z = 0.0);
	void push_translate(const Vec2f &vec) { push_translate(vec.x, vec.y, 0.0f); }
	void push_translate(const Vec3f &vec) { push_translate(vec.x, vec.y, vec.z); }
	void push_translate(int x, int y, int z = 0) { push_translate((float) x, (float) y, (float) z); }

	/// \brief Sets a rotation matrix, ignoring any earlier model view settings.
	void set_rotate(const Angle &angle, float x = 0.0, float y = 0.0, float z = 1.0, bool normalize = true);
	void set_rotate(const Angle &angle, const Vec2f &vec, bool normalize = true) { set_rotate(angle, vec.x, vec.y, 1.0f, normalize); }
	void set_rotate(const Angle &angle, const Vec3f &vec, bool normalize = true) { set_rotate(angle, vec.x, vec.y, vec.z, normalize); }

	/// \brief Adds a rotation matrix to existing model view.
	void mult_rotate(const Angle &angle, float x = 0.0, float y = 0.0, float z = 1.0, bool normalize = true);
	void mult_rotate(const Angle &angle, const Vec2f &vec, bool normalize = true) { mult_rotate(angle, vec.x, vec.y, 1.0f, normalize); }
	void mult_rotate(const Angle &angle, const Vec3f &vec, bool normalize = true) { mult_rotate(angle, vec.x, vec.y, vec.z, normalize); }

	/// \brief Pushes a rotation matrix onto model view stack.
	void push_rotate(const Angle &angle, float x = 0.0, float y = 0.0, float z = 1.0);
	void push_rotate(const Angle &angle, const Vec2f &vec) { push_rotate(angle, vec.x, vec.y, 1.0f); }
	void push_rotate(const Angle &angle, const Vec3f &vec) { push_rotate(angle, vec.x, vec.y, vec.z); }

	/// \brief Sets a scale matrix, ignoring any earlier model view settings.
	void set_scale(float x, float y, float z = 1.0);
	void set_scale(const Vec2f &vec) { set_scale(vec.x, vec.y, 1.0f); }
	void set_scale(const Vec3f &vec) { set_scale(vec.x, vec.y, vec.z); }
	void set_scale(int x, int y, int z = 1) { set_scale((float) x, (float) y, (float) z); }

	/// \brief Adds a scale matrix to existing model view.
	void mult_scale(float x, float y, float z = 1.0);
	void mult_scale(const Vec2f &vec) { mult_scale(vec.x, vec.y, 1.0f); }
	void mult_scale(const Vec3f &vec) { mult_scale(vec.x, vec.y, vec.z); }
	void mult_scale(int x, int y, int z = 1) { mult_scale((float) x, (float) y, (float) z); }

	/// \brief Pushes a scale matrix onto model view stack.
	void push_scale(float x, float y, float z = 1.0);
	void push_scale(const Vec2f &vec) { push_scale(vec.x, vec.y, 1.0f); }
	void push_scale(const Vec3f &vec) { push_scale(vec.x, vec.y, vec.z); }
	void push_scale(int x, int y, int z = 1) { push_scale((float) x, (float) y, (float) z); }

	/// \brief Pops last pushed model view matrix off the stack and makes it the active one.
	void pop_modelview();

	/// \brief Flushes the render batcher currently active.
	void flush();

	/// \brief Draw a point.
	void draw_point(float x1, float y1, const Colorf &color);

	/// \brief Point
	///
	/// \param gc = Graphic Context
	/// \param point = Pointf
	/// \param color = Colorf
	void draw_point(const Pointf &point, const Colorf &color);

	/// \brief Draw a line.
	void draw_line(float x1, float y1, float x2, float y2, const Colorf &color = Colorf::white);

	/// \brief Line
	///
	/// \param gc = Graphic Context
	/// \param start = Pointf
	/// \param end = Pointf
	/// \param color = Colorf
	void draw_line(const Pointf &start, const Pointf &end, const Colorf &color = Colorf::white);

	/// \brief Line
	///
	/// \param gc = Graphic Context
	/// \param line_segment = The line
	/// \param color = Colorf
	void draw_line(const LineSegment2f &line_segment, const Colorf &color = Colorf::white);

	/// \brief Lines
	void draw_lines(const Vec2f *positions, int num_vertices, const Colorf &color = Colorf::white);

	/// \brief Lines
	void draw_lines(const Vec2f *line_positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf &line_color = Colorf::white);

	/// \brief Line Strip
	void draw_line_strip(const Vec2f *positions, int num_vertices, const Colorf &color = Colorf::white);

	/// \brief Draw a box / rectangle.
	void draw_box(float x1, float y1, float x2, float y2, const Colorf &color = Colorf::white);

	/// \brief Box
	///
	/// \param gc = Graphic Context
	/// \param start = Pointf
	/// \param end = Pointf
	/// \param color = Colorf
	void draw_box(const Pointf &start, const Pointf &end, const Colorf &color = Colorf::white);

	/// \brief Box
	///
	/// \param gc = Graphic Context
	/// \param rect = Rectf
	/// \param color = Colorf
	void draw_box(const Rectf &rect, const Colorf &color);

	/// \brief Draw a filled box / rectangle.
	void fill_rect(float x1, float y1, float x2, float y2, const Colorf &color = Colorf::white);

	/// \brief Fill
	///
	/// \param gc = Graphic Context
	/// \param start = Pointf
	/// \param end = Pointf
	/// \param color = Colorf
	void fill_rect(const Pointf &start, const Pointf &end, const Colorf &color = Colorf::white);

	/// \brief Fill
	///
	/// \param gc = Graphic Context
	/// \param rect = Rectf
	/// \param color = Colorf
	void fill_rect(const Rectf &rect, const Colorf &color);

	/// \brief Gradient fill
	///
	/// \param gc = Graphic Context
	/// \param x1 = value
	/// \param y1 = value
	/// \param x2 = value
	/// \param y2 = value
	/// \param color = Gradient
	void fill_rect(float x1, float y1, float x2, float y2, const Gradient &color);

	/// \brief Gradient fill
	///
	/// \param gc = Graphic Context
	/// \param start = Pointf
	/// \param end = Pointf
	/// \param gradient = Gradient
	void fill_rect(const Pointf &start, const Pointf &end, const Gradient &gradient);

	/// \brief Gradient fill
	///
	/// \param gc = Graphic Context
	/// \param rect = Rectf
	/// \param gradient = Gradient
	void fill_rect(const Rectf &rect, const Gradient &gradient);

	/// \brief Draw a circle.
	void fill_circle(float center_x, float center_y, float radius, const Colorf &color = Colorf::white);

	/// \brief Circle
	///
	/// \param gc = Graphic Context
	/// \param center = Pointf
	/// \param radius = value
	/// \param color = Colorf
	void fill_circle(const Pointf &center, float radius, const Colorf &color = Colorf::white);

	/// \brief Gradient circle
	///
	/// \param gc = Graphic Context
	/// \param center = Pointf
	/// \param radius = value
	/// \param gradient = Gradient
	void fill_circle(const Pointf &center, float radius, const Gradient &gradient);

	/// \brief Gradient circle
	///
	/// \param gc = Graphic Context
	/// \param center = Pointf
	/// \param centergradient = Pointf
	/// \param radius = value
	/// \param gradient = Gradient
	void fill_circle(const Pointf &center, const Pointf &centergradient, float radius, const Gradient &gradient);

	/// \brief Draw a triangle.
	void fill_triangle(const Pointf &a, const Pointf &b, const Pointf &c, const Colorf &color = Colorf::white);

	/// \brief Draw a triangle.
	void fill_triangle(const Trianglef &dest_triangle, const Colorf &color = Colorf::white);

	/// \brief Draw triangles
	void fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Colorf &color = Colorf::white);

	/// \brief Draw triangles
	void fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Gradient &gradient);

	/// \brief Draw triangles
	void fill_triangles(const Vec2f *triangle_positions, const Colorf *colors, int num_vertices);

	/// \brief Draw triangles
	void fill_triangles(const std::vector<Vec2f> &triangles, const Colorf *colors);

	/// \brief Draw triangles
	void fill_triangles(const std::vector<Vec2f> &triangles, const Colorf &color = Colorf::white);

	/// \brief Draw triangles
	void fill_triangles(const std::vector<Vec2f> &triangles, const Gradient &gradient);

	/// \brief Draw triangles
	void fill_triangles(const Vec2f *positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf &color = Colorf::white);

	/// \brief Draw triangles
	void fill_triangles(const Vec2f *positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf *colors);

	/// \brief Draw triangles
	void fill_triangles(const std::vector<Vec2f> &positions, const std::vector<Vec2f> &texture_positions, const Texture2D &texture, const Colorf &color = Colorf::white);

	/// \brief Draw triangles
	void fill_triangles(const std::vector<Vec2f> &positions, const std::vector<Vec2f> &texture_positions, const Texture2D &texture, const std::vector<Colorf> &colors);

	/// \brief Draw triangles
	void fill_triangles(const std::vector<Vec2f> &positions, const Texture2D &texture, const Colorf &color = Colorf::white);

	/// \brief Draw triangles
	void fill_triangles(const std::vector<Vec2f> &positions, const Texture2D &texture, const Rect &texture_rect, const Colorf &color = Colorf::white);

	/// \brief Draw triangles
	void fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Texture2D &texture, const Colorf &color = Colorf::white);

	/// \brief Draw triangles
	void fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Texture2D &texture, const Rect &texture_rect, const Colorf &color = Colorf::white);

	/// \brief Draw triangles
	void fill_triangles(const std::vector<Vec2f> &positions, const Texture2D &texture, const Gradient &gradient);

	/// \brief Draw triangles
	void fill_triangles(const std::vector<Vec2f> &positions, const Texture2D &texture, const Rect &texture_rect, const Gradient &gradient);

	/// \brief Draw triangles
	void fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Texture2D &texture, const Gradient &gradient);

	/// \brief Draw triangles
	void fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Texture2D &texture, const Rect &texture_rect, const Gradient &gradient);

	/// \brief Draw a filled ellipse.
	void fill_ellipse(const Pointf &center, float radius_x, float radius_y, const Colorf &color = Colorf::white);

	/// \brief Draw a gradient filled ellipse.
	void fill_ellipse(const Pointf &center, float radius_x, float radius_y, const Gradient &gradient);

	/// \brief Set a new font manager
	void set_font_manager(FontManager &font_manager);


	/// \brief Flip back buffer to front, making changes visible on screen.
	///
	/// This function calls flush()
	///
	/// <p>The parameter interval specifies the minimum number of video frames
	/// that are displayed before a buffer swap will occur.</p>
	/// <p>If interval is set to a value of 0, buffer swaps are not synchronized
	/// to a video frame. </p>
	/// <p>If interval is set to a value of -1 (the default), then it will use
	/// the buffer swap used for previous flip. If its the first flip, it will
	/// use the system default.</p>
	///
	/// \param interval = See note
	void flip(DisplayWindow &window, int interval = -1);

	/// \brief Flip back buffer to front, making changes visible on screen.
	///
	/// This function calls flush()
	///
	/// <p>The parameter interval specifies the minimum number of video frames
	/// that are displayed before a buffer swap will occur.</p>
	/// <p>If interval is set to a value of 0, buffer swaps are not synchronized
	/// to a video frame. </p>
	/// <p>If interval is set to a value of -1 (the default), then it will use
	/// the buffer swap used for previous flip. If its the first flip, it will
	/// use the system default.</p>
	///
	/// \param interval = See note
	void flip(int interval = -1);

/// \}
/// \name Events
/// \{
public:

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<Canvas_Impl> impl;

	friend class Sprite_Impl;
	friend class Image;
	friend class GlyphCache;
/// \}
};

}

/// \}

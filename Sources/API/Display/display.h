/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_display
#define header_display

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include <string>
#include "color.h"
#include "gradient.h"
#include "../signals.h"
#include "../Core/Math/rect.h"
#include "../Core/Math/quad.h"
#include "../Core/Math/size.h"
#include "../Core/Math/point.h"
#include "../Core/Math/matrix4x4.h"

class CL_DisplayWindow;
class CL_InputEvent;
class CL_PixelBuffer;
class CL_DisplayMode;
class CL_Surface;

//: Top level display class.
//- !group=Display/Display 2D!
//- !header=display.h!
//- <p>The display class provides a static function interface to
//- CL_DisplayWindow and other clanDisplay classes. It uses a selected
//- display window (by default the first created window) to call the
//- equalent functions in CL_DisplayWindow, CL_GraphicContext and such.</p>
//- <p>The entire point of this is to allow applications with only one
//- window to not pass around a pointer to the display window.</p>
class CL_API_DISPLAY CL_Display
{
//! Attributes:
public:
	//: Returns the currently selected window.
	static CL_DisplayWindow *get_current_window();

	//: Returns the current width of the window.
	static int get_width();

	//: Returns the current height of the window.
	static int get_height();

	//: Returns true if window is currently running fullscreen.
	static bool is_fullscreen();

	//: Returns true if window has focus.
	static bool has_focus();

	//: Returns the pixel buffer for the specified flipping buffer.
	static CL_PixelBuffer get_buffer(int i);

	//: Returns the amount of flipping buffers being used.
	static int get_buffer_count();

	//: Returns the current flipping buffer being used as the front buffer.
	static CL_PixelBuffer get_front_buffer();

	//: Returns the current flipping buffer being used as the back buffer.
	static CL_PixelBuffer get_back_buffer();

	//: Returns the current clipping rectangle used on the graphic context.
	static const CL_Rect &get_cliprect();

	//: Returns the current effective modelview.
	static const CL_Matrix4x4 &get_modelview();
	
//! Operations:
public:
	//: Sets the currently selected window.
	static void set_current_window(CL_DisplayWindow *window);

	//: Change window to running fullscreen mode.
	static void set_fullscreen(int width, int height, int bpp);

	//: Change window to running fullscreen mode.
	static void set_fullscreen(const CL_DisplayMode &display_mode);

	//: Change window to running windowed mode.
	static void set_windowed();

	//: Change window title.
	static void set_title(const std::string &title);

	//: Set window position and size.
	static void set_position(const CL_Rect &pos);

	static void set_position(int x, int y);

	//: Resize window.
	static void set_size(int width, int height);

	//: Changes the amount of surface buffers used in the flipping system.
	//: (2 = double buffer, 3 = triple buffer)
	static void set_buffer_count(int flipping_buffers);

	//: Copy the specified rectangle area from back buffer to front buffer.
	static void update(const CL_Rect &rect);

	//: Flip back buffer to front, making changes visible on screen.
	//param interval: Specifies the minimum number of video frames that are displayed before a buffer swap will occur.
	//- <p>The parameter interval specifies the minimum number of video frames
	//- that are displayed before a buffer swap will occur.</p>
	//- <p>A video frame period is the time required by the monitor to display a 
	//- full frame of video data.  In the case of an interlaced monitor,
	//- this is typically the time required to display both the even and odd 
	//- fields of a frame of video data.  An interval set to a value of 2
	//- means that the color buffers will be swapped at most every other video
	//- frame.</p>
	//- <p>If interval is set to a value of 0, buffer swaps are not synchronized
	//- to a video frame.  The interval value is silently clamped to
	//- the maximum implementation-dependent value supported before being
	//- stored.</p>
	//- <p>If interval is set to a value of -1 (the default), then it will use
	//- the buffer swap used for previous flip. If its the first flip, it will
	//- use the system default, which per default is 1.</p>
	static void flip(int interval = -1);

	//: Flushes current rendering batch.
	//- <p>With the OpenGL target, this causes the graphic context to end its current
	//- active glBegin()/glEnd() pair, making it possible to call other OpenGL calls.</p>
	static void flush();

	//: Draw a pixel at (x, y) using the specified color.
	static void draw_pixel(int x, int y, const CL_Color &color);

	//: Draw a line from (x1, y1) to (x2, y2) using the specified color.
	static void draw_line(float x1, float y1, float x2, float y2, const CL_Color &color);
	static void draw_line(const CL_Pointf &a, const CL_Pointf &b, const CL_Color &color);

	//: Draw multiple lines using the specified color.
	//param double *vertices : start/end points of line segments in format x,y,x,y,x...
	//param CL_Color &color : color of lines
	static void draw_lines(int count, double *vertices, const CL_Color &color);

	//: Draw a rectangle using the specified color.
	static void draw_rect(const CL_Rectf &rect, const CL_Color &color);

	//: Draw a gradient rectangle using the specified gradient.
	static void fill_rect(const CL_Rectf &rect, const CL_Gradient &gradient);

	//: Draw a filled rectangle using the specified color.
	static void fill_rect(const CL_Rectf &rect, const CL_Color &color);
	
	//: Draw a quad using the specified color.
	static void draw_quad(const CL_Quad &quad, const CL_Color &color);

	//: Draw a gradient quad using the specified gradient.
	static void fill_quad(const CL_Quad &quad, const CL_Gradient &gradient);

	//: Draw a filled quad using the specified color.
	static void fill_quad(const CL_Quad &quad, const CL_Color &color);

	//: Draw a triangle using the specified color.
	//param double x : x coordinates of triangle vertices
	//param double y : y coordinates of triangle vertices
	//param CL_Color &Color : color to use
	static void draw_triangle(
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		const CL_Color &color);

	//: Draw a gradient filled triangle.
	//param double x : x coordinates of triangle vertices
	//param double y : y coordinates of triangle vertices
	//param CL_Gradient &gradient : color gradient to use
	static void fill_triangle(
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		const CL_Gradient &gradient);

	//: Draw triangles with color array (optionally textured).
	//- <p>uchar color array, range: 0-255
	//- integer (pixel) texture coordinates</p>
	//param unsigned int count : number of triangles to draw
	//param bool fill : draw filled/outline
	//param double *vertices : pointer to array of triangle vertices (x y z x y z x...)
	//param unsigned char *color : pointer to array of triangle vertice colors (r b g a r g b a r...)
	//param int *uv : (optional) pointer to array of triangle vertice texture coordinates (u v u v u...)
	//param CL_Surface *texture : (optional) texture to use
	static void draw_triangles(
		unsigned int count,
		bool fill,
		double *vertices,
		unsigned char *color,
		int *uv=0,
		CL_Surface *texture=0);

	//: Draw triangles with single color (optionally textured).
	//- <p>single color
	//- integer (pixel) texture coordinates</p>
	//param unsigned int count : number of triangles to draw
	//param bool fill : draw filled/outline
	//param double *vertices : pointer to array of triangle vertices (x y z x y z x...)
	//param CL_Color &color : triangle color
	//param int *uv : (optional) pointer to array of triangle vertice texture coordinates (u v u v u...)
	//param CL_Surface *texture : (optional) texture to use
	static void draw_triangles(
		unsigned int count,
		bool fill,
		double *vertices,
		const CL_Color &color,
		int *uv=0,
		CL_Surface *texture=0);

	//: Draw triangle array with color array (optionally textured).
	//- <p> double color array, range: 0-1
	//- double (0-1) texture coordinates</p>
	//param unsigned int count : number of triangles to draw
	//param bool fill : draw filled/outline
	//param double *vertices : pointer to array of triangle vertices (x y z x y z x...)
	//param double *color : pointer to array of triangle vertice colors (r b g a r g b a r...)
	//param int *uv : (optional) pointer to array of triangle vertice texture coordinates (u v u v u...)
	//param CL_Surface *texture : (optional) texture to use
	static void draw_trianglesd(
		unsigned int count,
		bool fill,
		double *vertices,
		double *color=0,
		double *uv=0,
		CL_Surface *texture=0);

	//: Draw triangles with single color (optionally textured).
	//- <p>single color
	//- double (0-1) texture coordinates</p>
	//param unsigned int count : number of triangles to draw
	//param bool fill : draw filled/outline
	//param double *vertices : pointer to array of triangle vertices (x y z x y z x...)
	//param CL_Color &color : triangle color
	//param double *uv : (optional) pointer to array of triangle vertice texture coordinates (u v u v u...)
	//param CL_Surface *texture : (optional) texture to use
	static void draw_trianglesd(
		unsigned int count,
		bool fill,
		double *vertices,
		const CL_Color &color,
		double *uv=0,
		CL_Surface *texture=0);

	//: Clears the whole window using the specified color.
	static void clear(const CL_Color &color = CL_Color(0,0,0));

	//: Set the current clipping rectangle.
	static void set_cliprect(const CL_Rect &rect);

	//: Push current clipping rectangle to stack.
	//- <p>If a rectangle is passed, it afterwards sets clipping
	//- rectangle to the union of the current rectangle and the passed
	//- rectangle.</p>
	static void push_cliprect(const CL_Rect &rect);

	static void push_cliprect();

	//: Pop current clipping rectangle from the stack.
	static void pop_cliprect();

	//: Sets the model view matrix to a new matrix.
	static void set_modelview(const CL_Matrix4x4 &matrix);

	//: Multiplies the passed matrix onto the model view matrix.
	static void add_modelview(const CL_Matrix4x4 &matrix);

	//: Pushes current model view matrix onto the model view stack.
	static void push_modelview();

	//: Sets a translate offset matrix, ignoring any earlier modelview settings.
	//- <p> This offset will affect any subsequent display operations on the current
	//- displaycard, by translating the position of the display operation with the offset.</p>
	static void set_translate(double x, double y, double z = 0.0);

	//: Adds the translate offset.
	//- <p> This offset will affect any subsequent display operations on the current
	//- displaycard, by translating the position of the display operation with the offset.
	//- The offset will be offset by any previous offsets pushed onto the stack,
	//- eg. it inherits the previous offset. </p>
	static void add_translate(double x, double y, double z = 0.0);
	
	//: Push translation offset onto model view stack.
	//- <p>This function is a convenience function for calling push_modelview, then add_translate.</p>
	//- <p> This offset will affect any subsequent display operations on the current
	//- displaycard, by translating the position of the display operation with the offset.
	//- The offset will be offset by any previous offsets pushed onto the stack,
	//- eg. it inherits the previous offset. </p>
	static void push_translate(double x, double y, double z = 0.0);

	//: Sets a rotation matrix, ignoring any earlier model view settings.
	static void set_rotate(double angle, double x = 0.0, double y = 0.0, double z = 1.0);

	//: Adds a rotation matrix to existing model view.
	static void add_rotate(double angle, double x = 0.0, double y = 0.0, double z = 1.0);

	//: Pushes a rotation matrix onto model view stack.
	static void push_rotate(double angle, double x = 0.0, double y = 0.0, double z = 1.0);

	//: Sets a scale matrix, ignoring any earlier model view settings.
	static void set_scale(double x, double y, double z = 1.0);

	//: Adds a scale matrix to existing model view.
	static void add_scale(double x, double y, double z = 1.0);

	//: Pushes a scale matrix onto model view stack.
	static void push_scale(double x, double y, double z = 1.0);

	//: Pops last pushed model view matrix off the stack and makes it the active one.
	static void pop_modelview();

//! Signals:
public:
	//: Signal emitted when window is resized.
	static CL_Signal_v2<int, int> &sig_resize();

	//: Signal emitted when an area of the window is invalidated.
	static CL_Signal_v1<const CL_Rect &> &sig_paint();

	//: Signal emitted when window lost focus.
	static CL_Signal_v0 &sig_lost_focus();

	//: Signal emitted when window gain focus.
	static CL_Signal_v0 &sig_got_focus();

	//: Signal emitted when window is closed.
	static CL_Signal_v0 &sig_window_close();
};

#endif

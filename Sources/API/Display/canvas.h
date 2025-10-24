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

#ifndef header_canvas
#define header_canvas

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

#include "../Core/System/sharedptr.h"

class CL_Surface;
class CL_PixelBuffer;
class CL_Rect;
class CL_Point;
class CL_GraphicContext;
class CL_Canvas_Generic;

//: Construct hardware accelerated (where available) off-screen canvas buffer.
//- !group=Display/Display 2D!
//- !header=display.h!
//- <p>For the OpenGL target, this object represents an OpenGL "pbuffer". A pbuffer
//- is an off-screen OpenGL context that can function as hardware accelerated rendering
//- to a canvas.</p>
//- <p>To use CL_Canvas, first create an instance of CL_Canvas. Then load a surface into
//- the canvas. This will make ClanLib copy the contents of the surface (texture for GL)
//- to the canvas. After this rendering operations can be performed on the canvas.</p>
//- <p>If the surface selected into the canvas is drawn, ClanLib will automatically upload
//- the contents of the canvas back into the surface. Likewise if another surface is selected
//- into the canvas the surface will be updated.</p>
//- <p>It is recommended to limit the amount of canvas objects in the application. Every
//- canvas require construction of a new OpenGL context, and canvases consume frame buffer
//- memory.</p>
//- <p>In Windows a pbuffer can get 'lost', meaning that the operating system can suddenly
//- just use the memory for something else, without warning. Because of this
//- it is adviced to keep data in the canvas for as little time as possible - sync_surface()
//- can be used force uploading of the canvas' contents to the surface. Generally you do not
//- need to do this since drawing the surface will force such an upload, but if the surface
//- is not be drawn for some time, forcing an upload is a good idea.</p>
//- <p>If the pbuffer is lost, ClanLib will automatically try to reclaim the canvas and sync
//- it with the latest version stored in the surface (the texture). As long as the pbuffer is
//- lost, any drawing commands on the canvas will be silently ignored. This cannot guarantee
//- never loosing what is drawn to the canvas, but at least not everything will be completely
//- lost.</p>
//- <p>Example of usage:</p>
//- <p>
//- CL_Surface surface_foo("foo.png");<br>
//- CL_Surface surface_bar("bar.png");<br>
//- CL_Canvas canvas;<br>
//- canvas.select_surface(surface_foo);<br>
//- surface_bar.draw(5, 5, canvas.get_gc());<br>
//- canvas.get_gc()->draw_line(0, 0, 100, 100, CL_Color::yellow);<br>
//- canvas.sync_surface();
//- </p>
class CL_API_DISPLAY CL_Canvas
{
//! Construction:
public:
	//: Constructs canvas buffer.
	CL_Canvas();
	
	CL_Canvas(const CL_Surface &initial_active_surface);
	
	CL_Canvas(const CL_Canvas &copy);
	
	virtual ~CL_Canvas();

//! Attributes:
public:
	//: Returns the graphic context for the canvas.
	CL_GraphicContext *get_gc();

	//: Returns the contents of the canvas.
	CL_PixelBuffer get_pixeldata();
	
	CL_PixelBuffer get_pixeldata(const CL_Rect &area);

	//: Returns the width of currently selected surface.
	int get_width() const;
	
	//: Returns the height of currently selected surface.
	int get_height() const;

//! Operations:
public:
	//: Copy assignment operator.
	CL_Canvas &operator =(const CL_Canvas &copy);

	//: Set contents of canvas.
	void set_pixeldata(const CL_PixelBuffer &data);

	void set_pixeldata(const CL_Point &dest, const CL_PixelBuffer &data);

	void set_pixeldata(const CL_Point &dest, const CL_Rect &src, const CL_PixelBuffer &data);

	//: Sets the surface that canvas is currently drawing on.
	void select_surface(CL_Surface &surface);
	
	//: Forces syncronization of the contents between canvas and selected surface.
	void sync_surface();
	
//! Implementation:
private:
	CL_SharedPtr<CL_Canvas_Generic> impl;
};

#endif

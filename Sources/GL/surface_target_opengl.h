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

#ifndef header_surface_target_opengl
#define header_surface_target_opengl

#if _MSC_VER > 1000
#pragma once
#endif

#include "Display/surface_generic.h"
#include "Display/surface_target.h"
#include "API/Display/surface.h"
#include "API/Core/Math/size.h"
#include "GL/gl_state_checker.h"

class CL_Canvas_OpenGL;

class CL_Surface_Target_OpenGL : public CL_Surface_Target
{
//! Construction:
public:
	CL_Surface_Target_OpenGL(
		CL_PixelBuffer provider,
		int flag);

	CL_Surface_Target_OpenGL(
		const CL_PixelFormat &pf,
		const CL_Size &size);

	virtual ~CL_Surface_Target_OpenGL();

//! Attributes:
public:
	//: Returns the width.
	virtual int get_width() const;
	
	//: Returns the height.
	virtual int get_height() const;

	//: Returns a pixelbuffer object for accessing surface data.
	virtual CL_PixelBuffer get_pixeldata();
	
	//: Returns a graphic context object for rendering to this surface.
	virtual CL_GraphicContext *get_gc();

	//: OpenGL texture handle.
	CLuint handle;

	CL_Size surface_size;

	CL_Size texture_size;

	CL_Canvas_OpenGL *selected_canvas;

	CL_PixelBuffer original_pixelbuffer;

//! Operations:
public:
	//: Uploads pixel data to surface.
	//!param pos: Position on surface where pixel data should be put.
	//!param pixeldata: CL_PixelBuffer containing the image to be uploaded.
	//!param src_rect: Part of pixelbuffer that should be uploaded.
	virtual void set_pixeldata(const CL_Point &pos, const CL_Rect &src_rect, CL_PixelBuffer &pixeldata);

	//: Draw surface on screen.
	virtual void draw(
		CL_Surface_Generic *attributes,
		const CL_Surface_DrawParams1& params1,
		CL_GraphicContext *gc);
		
	//  Get the surface handle
	int get_handle();

//! Implementation:
private:
	CL_GLState state;
};

#endif

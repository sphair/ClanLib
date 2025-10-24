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

#ifndef header_rle_surface
#define header_rle_surface

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

class CL_PixelBuffer;
class CL_GraphicContext;
class CL_InputSource;
class CL_OutputSource;
class CL_RLESurface_Generic;

//: Runlength encoded surface.
//- !group=Display/Display 2D!
//- !header=display.h!
class CL_API_DISPLAY CL_RLESurface
{
//! Construction:
public:
	//: Constructs a run length encoded surface.
	//param provider: Pixel buffer containing image data to be used for constructing RLE surface.
	//param delete_provider: If true, deletes the provider after copying its image data.
	//param input: Input source to load a surface in encoded form.
	CL_RLESurface(CL_PixelBuffer provider);

	CL_RLESurface(CL_InputSource *input);

//! Operations:
public:
	//: Draw surface on screen.
	void put_screen(int x, int y, CL_GraphicContext *context = 0);

	//: Draw surface onto pixel buffer.
	void blit(int x, int y, CL_PixelBuffer buffer);

	//: Save surface in encoded form to an output source.
	void save(CL_OutputSource *output);

//! Implementation:
private:
//	CL_RLESurface_Generic *impl;
};

#endif

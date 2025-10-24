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
**    Harry Storbacka
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

//! clanDisplay="Collision"
//! header=display.h

#ifndef header_outline_provider_bitmap
#define header_outline_provider_bitmap

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

#include <vector>
#include <string>

#include "outline_provider.h"
#include "outline_accuracy.h"

class CL_OutlineProviderBitmap_Generic;
class CL_PixelBuffer;

//: Bitmap outline provider.
//- !group=Display/Collision !
//- !header=display.h!
//- <p>A CL_OutlineProviderBitmap is used to find outlines based on the alpha channel of images.</p>
class CL_API_DISPLAY CL_OutlineProviderBitmap : public CL_OutlineProvider
{
//! Construction:
 public:
	//: Construct a contour
	//param CL_PixelBuffer *pbuf : PixelBuffer containing image data.
	//param int alpha_limit : Minimum alpha value for a colliding pixel
	CL_OutlineProviderBitmap(CL_PixelBuffer pbuf, int alpha_limit=128, bool get_insides=true);
	~CL_OutlineProviderBitmap();

//! Attributes:
 public:
	//: return the countours that make up the outline
	virtual std::vector<CL_Contour> get_contours();

	//: return the width of the image used as basis for outline creation.
	virtual int get_width();

	//: return the height of the image used as basis for outline creation.
	virtual int get_height();

//! Implementation:
 private:
	CL_OutlineProviderBitmap_Generic *impl;
};

#endif

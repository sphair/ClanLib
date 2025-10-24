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

#include "Display/display_precomp.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/Collision/outline_provider_bitmap.h"
#include "API/Display/Collision/outline_accuracy.h"
#include "outline_provider_bitmap_generic.h"
#include "API/Display/Collision/outline_circle.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineProviderBitmap construction:

CL_OutlineProviderBitmap::CL_OutlineProviderBitmap(CL_PixelBuffer pbuf, int alpha_limit, bool get_insides)
{
	impl = new CL_OutlineProviderBitmap_Generic( pbuf, alpha_limit, get_insides);
}

CL_OutlineProviderBitmap::~CL_OutlineProviderBitmap()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineProviderBitmap attributes:

std::vector<CL_Contour> CL_OutlineProviderBitmap::get_contours()
{
	return impl->contours;
}

int CL_OutlineProviderBitmap::get_width()
{
	return impl->width;
}

int CL_OutlineProviderBitmap::get_height()
{
	return impl->height;
}


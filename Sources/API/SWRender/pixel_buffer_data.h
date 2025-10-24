/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
*/

/// \addtogroup clanSWRender_Display clanSWRender Display
/// \{

#pragma once

#include "../Display/Image/pixel_buffer.h"

/// \brief Frame buffer or texture data for pixel commands
///
/// \xmlonly !group=SWRender/Display! !header=swrender.h! \endxmlonly
class CL_PixelBufferData
{
//!Construction
public:
	CL_PixelBufferData() : data(0) { }

//!Attributes
public:
	CL_PixelBuffer pixelbuffer;
	CL_Size size;
	unsigned int *data;

//!Operations
public:
	void set(const CL_PixelBuffer &new_pixelbuffer)
	{
		pixelbuffer = new_pixelbuffer;
		size = pixelbuffer.get_size();
		data = static_cast<unsigned int *>(pixelbuffer.get_data());
	}
};
/// \}

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

#include "Display/display_precomp.h"
#include "API/Display/pixel_format.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PixelFormat construction:

CL_PixelFormat::CL_PixelFormat()
: type(pixelformat_rgba), colorkey_enabled(false), colorkey(0), red_mask(0), green_mask(0), blue_mask(0), alpha_mask(0), depth(0)
{
}

CL_PixelFormat::CL_PixelFormat(const CL_PixelFormat &copy)
{
	colorkey_enabled = copy.colorkey_enabled;
	colorkey = copy.colorkey;
	red_mask = copy.red_mask;
	green_mask = copy.green_mask;
	blue_mask = copy.blue_mask;
	alpha_mask = copy.alpha_mask;
	depth = copy.depth;
	type = copy.type;
}

CL_PixelFormat::CL_PixelFormat(
	int depth,
	unsigned int red_mask,
	unsigned int green_mask,
	unsigned int blue_mask,
	unsigned int alpha_mask,
	bool use_colorkey,
	unsigned int colorkey,
	CL_PixelFormatType type)
: type(type), colorkey_enabled(use_colorkey), colorkey(colorkey), red_mask(red_mask), green_mask(green_mask), blue_mask(blue_mask), alpha_mask(alpha_mask), depth(depth)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelFormat attributes:

bool CL_PixelFormat::operator ==(const CL_PixelFormat &other) const
{
	if (type != other.type) return false;
	if (type == pixelformat_index)
	{
		if (
			depth != other.depth ||
			colorkey_enabled != other.colorkey_enabled
			)
			return false;
		if (colorkey_enabled && colorkey != other.colorkey)
			return false;
		return true;
	}
	else if (type == pixelformat_rgba)
	{
		if (
			depth != other.depth ||
			red_mask != other.red_mask ||
			green_mask != other.green_mask ||
			blue_mask != other.blue_mask ||
			alpha_mask != other.alpha_mask ||
			colorkey_enabled != other.colorkey_enabled
			)
			return false;
		if (colorkey_enabled && colorkey != other.colorkey)
			return false;
		return true;
	}
	else
		return false;
}

bool CL_PixelFormat::operator !=(const CL_PixelFormat &other) const
{
	return !( *this == other );
}


/////////////////////////////////////////////////////////////////////////////
// CL_PixelFormat statics:

int CL_PixelFormat::get_mask_shift(unsigned int mask)
{
	if (mask == 0)
		return 0;

	for (int shift = 0; shift < 32; shift++)
	{
		if (mask & 1)
			return shift;
		else
			mask >>= 1;
	}
	return 0;
}

int CL_PixelFormat::get_mask_bits(unsigned int mask)
{
	if (mask == 0)
		return 0;

	const int shift = get_mask_shift(mask);
	mask >>= shift;

	int bits;
	for (bits = 0; bits < (32 - shift); bits++)
	{
		if (!(mask & 1))
			return bits;
		else
			mask >>= 1;
	}
	return bits;
}

unsigned int CL_PixelFormat::get_bitmask(int bits, int shift)
{
	unsigned int mask = 0;
	for (int i = 0; i < bits; i++)
		mask |= 1 << i;
	return mask << shift;
}

CL_PixelFormat CL_PixelFormat::rgba8888(32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

CL_PixelFormat CL_PixelFormat::rgb888(24, 0xff0000, 0x00ff00, 0x0000ff, 0x000000);

CL_PixelFormat CL_PixelFormat::rgba4444(16, 0xf000, 0x0f00, 0x00f0, 0x000f);

CL_PixelFormat CL_PixelFormat::rgb555(16, 0x7c00, 0x03e0, 0x001f, 0x0000);

CL_PixelFormat CL_PixelFormat::rgb565(16, 0xf800, 0x07e0, 0x001f, 0x0000);

CL_PixelFormat CL_PixelFormat::abgr8888(32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

CL_PixelFormat CL_PixelFormat::bgr888(24, 0x0000ff, 0x00ff00, 0xff0000, 0x000000);

CL_PixelFormat CL_PixelFormat::abgr4444(16, 0x000f, 0x00f0, 0x0f00, 0xf000);

CL_PixelFormat CL_PixelFormat::bgr555(16, 0x001f, 0x03e0, 0x7c00, 0x0000);

CL_PixelFormat CL_PixelFormat::bgr565(16, 0x001f, 0x07e0, 0xf800, 0x0000);
